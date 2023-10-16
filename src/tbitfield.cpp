// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// ВАЖНО!	1)	BitLen > 0 - количество бит 
//			2)	pMem хранит биты с номерами от 0 до (BitLen - 1) включительно 
//			3)	в неиспользуемых битовых ячейках находятся произвольные числа,
//				необязательно только 0
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// TODO:																													
// 2)	метод GetMemIndex должен проверять n на его вхождение в область определения метода ?			
//		Если да, то нужно переписать operator>>,														
//		если нет, то почему GetMemIndex - метод, а дружественная функция, например						
// 3)	реализация operator& и operator|																
// 4)	Должны ли operator&, operator| работать с битовыми полями разных длин?		
// 5)	Скачать файл git ignore
//////////////////////////////////////////////////////////////////////////////////////////////////////////

static const int BITS_IN_TELEM = 8 * sizeof(TELEM);

static int SHIFT() {	// можно ли представить BITS_IN_TELEM в виде 2^i (i - неотриц. целое), если да, то возвращает i, иначе -1
	int shift = 3; // BITS_INN_TELEM нацело делится на 8
	const unsigned int size_TELEM = sizeof(TELEM);
	unsigned int number = 1;	// 2^i

	while (number <= size_TELEM) {
		if (number == size_TELEM)
			break;
		else {
			number << 1;	// умножаем number на 2
			shift += 1;
		}
	}

	if (number != size_TELEM)	// значит BITS_IN_TELEM нельзя представить как 2^i
		shift = -1;

	return shift;
}
static const int SHIFT_TELEM = SHIFT();

TBitField::TBitField(int len)
{
	if (len <= 0)
		throw out_of_range("EXCEPTION: constructor TBitField, len <= 0");

	int temp = SHIFT_TELEM > 0 ? len << SHIFT_TELEM : len / BITS_IN_TELEM;	// целочисленное деление len на BITS_IN_TELEM

	BitLen = len;
	MemLen = temp + ((len - BITS_IN_TELEM * temp) > 0);	// MemLen = кол-во TELEM, где используются все биты; ++MemLen, если остаток при делении ненулевой
	pMem = new TELEM[MemLen];	// выделение динамической памяти

	for (int i = 0; i < MemLen; ++i)
		pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];

	for (int i = 0; i < MemLen; ++i)
		this->pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	// return n / BITS_IN_TELEM;

	return SHIFT_TELEM > 0 ? n << SHIFT_TELEM : n / BITS_IN_TELEM;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	int ElemIndex = n - BITS_IN_TELEM * GetMemIndex(n);	// какой номер внутри TELEM pMem[MemIndex]
	TELEM MemMask = TELEM(1) << ElemIndex;	// получаем 00...010...00, где 1 стоит на позиции ElemIndex

	return MemMask;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	// SetBit присваивает биту n значение 1 

	int MemIndex = GetMemIndex(n);
	TELEM MemMask = GetMemMask(n);

	pMem[MemIndex] = pMem[MemIndex] | MemMask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
	// operator~ - побитовая инверсия
	// bitMask = 0...010...0	==>		bitMask = 1...101...1

	int MemIndex = GetMemIndex(n);
	TELEM MemMask = GetMemMask(n);
	
	MemMask = ~(MemMask);

	pMem[MemIndex] = pMem[MemIndex] & MemMask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this != &bf) {
		if (this->MemLen != bf.MemLen) {
			delete[] pMem;
			BitLen = 0;
			MemLen = 0;

			pMem = new TELEM[bf.MemLen];
			MemLen = bf.MemLen;
		}
		// теперь MemLen == bf.MemLen

		BitLen = bf.BitLen;

		for (int i = 0; i < MemLen; ++i)
			this->pMem[i] = bf.pMem[i];
	}

	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	int result = 0;

	if (this == &bf)	// указатели указывают на один и тот же объект
		result = 1;
	else if (BitLen == bf.BitLen) {
		result = 1;

		for (int i = 0; i < MemLen - 1; ++i)	// сравниваем элементы pMem от 0 до MemLen - 2
			if (pMem[i] != bf.pMem[i]) {
				result = 0;
				break;
			}

		if (result == 1) {	// сравниваем последние элементы pMem, если предыдущие оказались равными
			TELEM number1 = pMem[MemLen - 1], number2 = bf.pMem[MemLen - 1];
			
			// сравниваем биты от 0 до (BitLen % BITS_IN_TELEM) чисел number1, number2
			// number1 = aa...aaxxx...x		||	нужно сравнить xxx...x и yyy...y
			// number2 = bb...bbyyy...y		||	для этого нужно найти их битовую длину, которая одинаковая у них

			// int bitWidth = BitLen % BITS_IN_TELEM
			// BitLen == B_I_T * (MemLen - 1) + (BitLen % B_I_T)
			int bitWidth = BitLen - BITS_IN_TELEM * (MemLen - 1);	// сколько битов нужно сравнить в последних элементах
			// bitWidth - битовая ширина xx...xx и yy...yy

			number1 = number1 << (BITS_IN_TELEM - bitWidth);
			number2 = number2 << (BITS_IN_TELEM - bitWidth);
			// теперь	number1 == x...x0...0
			//			number2 == y...y0...0

			result = (number1 == number2);
		}
	}

	return result;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	int result = this->operator==(bf);
	result = !(result);

	return result;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	// вызовется конструктор копирования, которому передастся битовое поле наибольшей длины
	TBitField result = BitLen > bf.BitLen ? *this : bf;

	int minBitLen, maxBitLen, minMemLen, maxMemLen;

	maxBitLen = result.BitLen;
	maxMemLen = result.MemLen;
	if (BitLen > bf.BitLen) {
		minBitLen = bf.BitLen;
		minMemLen = bf.MemLen;
	}
	else {
		minBitLen = BitLen;
		minMemLen = MemLen;
	}


}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	return* this;
}

TBitField TBitField::operator~(void) // отрицание
{
	for (int i = 0; i < MemLen; ++i)
		pMem[i] = ~pMem[i];

	// pMem[MemLen - 1] == 00...00xx...xx		- это последний элемент pMem
	// 00...00xx...xx	==>		11...11(~x)(~x)...(~x)(~x)		после цикла for

	int bitWidth = BitLen - BITS_IN_TELEM * (MemLen - 1);
	// int bitWidth = BitLen % BITS_IN_TELEM;
	// bitWidth - количество занятых битов в последнем элементе pMem

	TELEM tempMask = 0;
	for (int i = 0; i < bitWidth; ++i) {
		tempMask += 1;
		tempMask = tempMask << 1;
	}
	// теперь tempMask == 00...0011..11, где bitWidth единиц

	pMem[MemLen - 1] = pMem[MemLen - 1] & tempMask;
	// 11...11(~x)(~x)...(~x)(~x) & 00...0011...11 == 00...00(~x)(~x)...(~x)(~x)

	return *this;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	char* s;
	TELEM number;
	int digit;

	istr >> s;

	int sLen = strlen(s);

	delete[] bf.pMem;
	bf.MemLen = 0;
	bf.BitLen = 0;

	int temp = bf.GetMemIndex(sLen);
	// хоть метод и применяется к объекту bf, метод не работает с полями объекта
	bf.pMem = new TELEM[temp];
	bf.MemLen = temp;
	bf.BitLen = sLen;

	// обработка исключений
	for (int i = 0; i < sLen; ++i)
		if (s[i] != '0' && s[i] != '1')
			throw "Поток ввода получил неправильную строку символов";

	// заполнение элементов pMem от 0 до (MemLen - 1) - 1
	for (int i = 0; i < bf.MemLen - 1; ++i) {
		number = 0;

		for (int j = 0; j < BITS_IN_TELEM; ++j) {
			digit = s[i * BITS_IN_TELEM + j] - '0';

			number = number + digit;
			number = number << 1;
		}

		bf.pMem[i] = number;
	}

	// предыдущий temp и текущий не имеют между собой ничего общего
	temp = BITS_IN_TELEM * (bf.MemLen - 1);
	number = 0;
	for (int i = 0; i < (bf.BitLen - temp); ++i) {
		digit = s[temp + i] - '0';
		number = number + digit;
		number = number << 1;
	}
	bf.pMem[bf.MemLen - 1] = number;

	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	char* s = new char[bf.BitLen];
	for (int i = 0; i < bf.MemLen - 1; ++i) {
		TELEM number = bf.pMem[i];

		for (int j = 0; j < BITS_IN_TELEM; ++j) {
			s[i * BITS_IN_TELEM + j] = '0' + (number & 1);
			number = number >> 1;
		}
	}

	TELEM number = bf.pMem[bf.MemLen - 1];
	for (int i = BITS_IN_TELEM * (bf.MemLen - 1); i < bf.BitLen; ++i) {
		s[i] = '0' + (number & 1);
		number = number >> 1;
	}

	ostr << s;

	return ostr;
}
