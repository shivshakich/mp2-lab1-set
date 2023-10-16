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

	int temp = len / BITS_IN_TELEM;

	BitLen = len;
	MemLen = temp + (len != temp * BITS_IN_TELEM);
	pMem = new TELEM[MemLen];

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
	pMem = nullptr;
	BitLen = 0;
	MemLen = 0;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n < 0 || n >= BitLen)
		throw out_of_range("EXCEPTION: TBitField::GetMemIndex, n < 0 || n >= BitLen");

<<<<<<< HEAD
	return SHIFT_TELEM > 0 ? n << SHIFT_TELEM : n / BITS_IN_TELEM;
=======
	return n / BITS_IN_TELEM;
>>>>>>> 2c2caa15f898af9151e7367449ffa0c7893c5378
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
<<<<<<< HEAD
	int ElemIndex = n - BITS_IN_TELEM * GetMemIndex(n);	// какой номер внутри TELEM pMem[MemIndex]
	TELEM MemMask = TELEM(1) << ElemIndex;	// получаем 00...010...00, где 1 стоит на позиции ElemIndex

	return MemMask;
=======
	int memIndex = GetMemIndex(n);
	// int telemIndex == n % BITS_IN_TELEM;
	int telemIndex = n - memIndex * BITS_IN_TELEM;
	// получили 0 <= telemIndex < BITS_IN_TELEM

	TELEM memMask = 1;
	memMask = memMask << telemIndex;

	return memMask;
>>>>>>> 2c2caa15f898af9151e7367449ffa0c7893c5378
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	// SetBit присваивает биту n значение 1 
<<<<<<< HEAD

	int MemIndex = GetMemIndex(n);
	TELEM MemMask = GetMemMask(n);

	pMem[MemIndex] = pMem[MemIndex] | MemMask;
=======

	if (n < 0 || n >= BitLen)
		throw out_of_range("EXCEPTION: TBitField::SetBit, n < 0 || n >= BitLen");

	int memIndex = GetMemIndex(n);
	TELEM memMask = GetMemMask(n);

	pMem[memIndex] = pMem[memIndex] | memMask;
>>>>>>> 2c2caa15f898af9151e7367449ffa0c7893c5378
}

void TBitField::ClrBit(const int n) // очистить бит
{
	// operator~ - побитовая инверсия
	// bitMask = 0...010...0	==>		bitMask = 1...101...1
<<<<<<< HEAD

	int MemIndex = GetMemIndex(n);
	TELEM MemMask = GetMemMask(n);
	
	MemMask = ~(MemMask);

	pMem[MemIndex] = pMem[MemIndex] & MemMask;
=======

	if (n < 0 || n >= BitLen)
		throw out_of_range("EXCEPTION: TBitField::ClrBit, n < 0 || n >= BitLen");

	int memIndex = GetMemIndex(n);
	TELEM memMask = GetMemMask(n);

	memMask = ~memMask;

	pMem[memIndex] = pMem[memIndex] & memMask;
>>>>>>> 2c2caa15f898af9151e7367449ffa0c7893c5378
}

int TBitField::GetBit(const int n) const // получить значение бита
{
<<<<<<< HEAD
	return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
=======
	if (n < 0 || n >= BitLen)
		throw out_of_range("EXCEPTION: TBitField::GetBit, n < 0 || n >= BitLen");

	int memIndex = GetMemIndex(n);
	TELEM memMask = GetMemMask(n);

	TELEM result = (pMem[memIndex] & memMask) == 0 ? 0 : 1;

	return result;
>>>>>>> 2c2caa15f898af9151e7367449ffa0c7893c5378
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this != &bf) {
<<<<<<< HEAD
		if (this->MemLen != bf.MemLen) {
=======
		if (MemLen != bf.MemLen) {
>>>>>>> 2c2caa15f898af9151e7367449ffa0c7893c5378
			delete[] pMem;
			BitLen = 0;
			MemLen = 0;

			pMem = new TELEM[bf.MemLen];
<<<<<<< HEAD
			MemLen = bf.MemLen;
		}
		// теперь MemLen == bf.MemLen

		BitLen = bf.BitLen;

		for (int i = 0; i < MemLen; ++i)
			this->pMem[i] = bf.pMem[i];
=======
			BitLen = bf.BitLen;
			MemLen = bf.MemLen;
		}

		for (int i = 0; i < MemLen; ++i)
			pMem[i] = bf.pMem[i];
>>>>>>> 2c2caa15f898af9151e7367449ffa0c7893c5378
	}

	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	int result = 0;

<<<<<<< HEAD
	if (this == &bf)	// указатели указывают на один и тот же объект
		result = 1;
	else if (BitLen == bf.BitLen) {
		result = 1;

		for (int i = 0; i < MemLen - 1; ++i)	// сравниваем элементы pMem от 0 до MemLen - 2
=======
	if (BitLen == bf.BitLen) {
		result = 1;

		for (int i = 0; i < MemLen - 1; ++i) 
>>>>>>> 2c2caa15f898af9151e7367449ffa0c7893c5378
			if (pMem[i] != bf.pMem[i]) {
				result = 0;
				break;
			}

<<<<<<< HEAD
		if (result == 1) {	// сравниваем последние элементы pMem, если предыдущие оказались равными
			TELEM number1 = pMem[MemLen - 1], number2 = bf.pMem[MemLen - 1];
			
			// сравниваем биты от 0 до (BitLen % BITS_IN_TELEM) чисел number1, number2
			// number1 = aa...aaxxx...x		||	нужно сравнить xxx...x и yyy...y
			// number2 = bb...bbyyy...y		||	для этого нужно найти их битовую длину, которая одинаковая у них

			// int bitWidth = BitLen % BITS_IN_TELEM
			// BitLen == B_I_T * (MemLen - 1) + (BitLen % B_I_T)
			int bitWidth = BitLen - BITS_IN_TELEM * (MemLen - 1);	// сколько битов нужно сравнить в последних элементах
			// bitWidth - битовая ширина xx...xx и yy...yy
=======
		if (result == 1) {
			// bitWidth - количество бит занятых в последнем TELEM
			// 0 < bitWidth <= BITS_IN_TELEM
			int bitWidth = BitLen - BITS_IN_TELEM * (MemLen - 1);
>>>>>>> 2c2caa15f898af9151e7367449ffa0c7893c5378

			TELEM number1 = pMem[MemLen - 1], number2 = pMem[MemLen - 1];

<<<<<<< HEAD
			result = (number1 == number2);
=======
			TELEM bitMask = 1;
			bitMask = bitMask << bitWidth;	// bitMask занимает (bitWidth + 1) битов, единица находится на bitWidth позиции
			bitMask -= 1;					// теперь bitMask в ширину bitWidth битов, с 0 по (bitWidth - 1) - единицы

			result = (number1 & bitMask) == (number2 & bitMask);
>>>>>>> 2c2caa15f898af9151e7367449ffa0c7893c5378
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
<<<<<<< HEAD
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


=======
	if (BitLen != bf.BitLen)
		throw "EXCEPTION: TBitField::operator|, BitLen != bf.BitLen";

	TBitField result(*this);

	for (int i = 0; i < result.MemLen; ++i)
		result.pMem[i] = result.pMem[i] | bf.pMem[i];

	return result;	// создастся копия
>>>>>>> 2c2caa15f898af9151e7367449ffa0c7893c5378
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	if (BitLen != bf.BitLen)
		throw "EXCEPTION: TBitField::operator& BitLen != bf.BitLen";

	TBitField result(*this);

	for (int i = 0; i < result.BitLen; ++i)
		result.pMem[i] = result.pMem[i] & bf.pMem[i];

	return result;	// создастся копия 
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField result(*this);

	for (int i = 0; i < result.BitLen; ++i)
		result.pMem[i] = ~(result.pMem[i]);

	return result;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	char* s;
	int sLen;
	TELEM number;
	TELEM digit;
	int lim;		// количество символов, что пойдёт в элементы pMem с 0 до предпоследнего 

	istr >> s;	// ввод символов
	sLen = strlen(s);

	if (sLen <= 0)	// проверка на исключение
		throw "EXCEPTION: TBitField::operator>>, length of s <= 0";

	for (int i = 0; i < sLen; ++i)	// проверка на исключение
		if (s[i] != '0' && s[i] != '1')
			throw "EXCEPTION: TBitField::operator>>, wrong input flow of chars";

	delete[] bf.pMem;	// освобождаем память от текущего pMem; если pMem == nullptr, то ошибки не будет
	bf.MemLen = 0;		
	bf.BitLen = 0;

	bf.MemLen = bf.GetMemIndex(sLen);	// получаем MemLen
	bf.pMem = new TELEM[bf.MemLen];		// выделяем память для pMem
	bf.BitLen = sLen;					// присваиваем значение для BitLen

	lim = BITS_IN_TELEM * (bf.MemLen - 1);	// получаем значение для lim

	for (int i = 0; i < lim; ++i) {	// пробегаем символы, которые пойдут в элементы pMem с номерами от 0 до предпоследнего
		TELEM number = 0;

		for (int j = 0; j < BITS_IN_TELEM; ++j) {	// формируем значение для pMem[i]
			digit = TELEM(s[i * BITS_IN_TELEM + j] - '0');	// digit - текущий символ s

			number = number << 1;
			number += digit;			// записываем digit в number
		}

		bf.pMem[i] = number;			// записываем значение для pMem[i]
	}

	number = 0;
	for (int i = 0; i < bf.BitLen - lim; ++i) {	// пробегаем оставшиеся символы
		digit = TELEM(s[BITS_IN_TELEM * (bf.MemLen - 1) + i] - '0'); // digit - текущий символ s
		
		number << 1;
		number += digit;
	}
	bf.pMem[bf.MemLen - 1] = number;	// записываем значение для pMem[MemLen - 1]

	delete[] s;	// освобождение памяти от динамического массива s

	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	char* s = new char[bf.BitLen];	// создаём s, выделяем для него память
	TELEM number, digit;
	int lim;

	for (int i = 0; i < bf.MemLen - 1; ++i) {	// пробегаем элементы pMem от 0 до предпоследнего 
		number = bf.pMem[i];

		for (int j = 0; j < BITS_IN_TELEM; ++j) {	// пробегаем биты pMem[i] справа налево
			digit = TELEM(1) & number;

			s[i * BITS_IN_TELEM + j] = (digit != 0) + '0';

			number = number >> 1;
		}
	}

	lim = BITS_IN_TELEM * (bf.MemLen - 1);

	number = bf.pMem[bf.MemLen - 1];
	for (int i = 0; i < bf.BitLen - lim; ++i) {	// пробегаем BitLen - lim битов в pMem[MemLen - 1]
		digit = TELEM(1) & number;

		s[lim + i] = (digit != 0) + '0';

		number = number >> 1;
	}

	cout << s;	// вывод s

	delete[] s;	// освобождение памяти от s

	return ostr;
}
