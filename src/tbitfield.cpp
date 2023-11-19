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

// можно ли представить BITS_IN_TELEM в виде 2^i (i - неотриц. целое), если да, то возвращает i, иначе -1
static int SHIFT() {	
	int shift = 3;	// BITS_IN_TELEM нацело делится на 8
	const unsigned int size_TELEM = sizeof(TELEM);
	unsigned int number = 1;	// 2^i

	while (number <= size_TELEM) {
		if (number == size_TELEM)
			break;
		else {
			number = number << 1;	// умножаем number на 2
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

	int temp = SHIFT_TELEM > 0 ? len >> SHIFT_TELEM : len / BITS_IN_TELEM;	// целочисленное деление len на BITS_IN_TELEM

	BitLen = len;
	MemLen = temp + ((len - BITS_IN_TELEM * temp) > 0);
	pMem = new TELEM[MemLen];	// выделение динамической памяти для pMem

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
	delete[] pMem;	// освобождение динамической памяти от pMem
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n < 0)
		throw out_of_range("EXCEPTION: TBitField::GetMemIndex, n < 0");

	// return n / BITS_IN_TELEM;

	return SHIFT_TELEM > 0 ? n >> SHIFT_TELEM : n / BITS_IN_TELEM;
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

	if (n < 0 || n >= BitLen)
		throw out_of_range("EXCEPTION: TBitField::SetBit, n < 0 || n >= BitLen");

	int MemIndex = GetMemIndex(n);
	TELEM MemMask = GetMemMask(n);

	pMem[MemIndex] = pMem[MemIndex] | MemMask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
	// operator~ - побитовая инверсия
	// bitMask = 0...010...0	==>		bitMask = 1...101...1

	if (n < 0 || n >= BitLen)
		throw out_of_range("EXCEPTION: TBitField::ClrBit, n < 0 || n >= BitLen");

	int MemIndex = GetMemIndex(n);
	TELEM MemMask = GetMemMask(n);
	
	MemMask = ~(MemMask);

	pMem[MemIndex] = pMem[MemIndex] & MemMask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0 || n >= BitLen)
		throw out_of_range("EXCEPTION: TBitField::GetBit, n < 0 || n >= BitLen");

	return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this != &bf) {
		if (MemLen != bf.MemLen) {
			delete[] pMem;
			BitLen = 0;
			MemLen = 0;

			pMem[bf.MemLen];
			MemLen = bf.MemLen;
		}	// теперь MemLen == bf.MemLen

		BitLen = bf.BitLen;

		for (int i = 0; i < MemLen; ++i)
			this->pMem[i] = bf.pMem[i];
	}

	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	int result = 0;

	if (this == &bf)
		result = 1;
	else if (BitLen == bf.BitLen) {
		result = 1;

		for (int i = 0; i < MemLen - 1; ++i)	// проверяем элементы с 0 по MemLen - 2
			if (pMem[i] != bf.pMem[i]) {
				result = 0;
				break;
			}

		if (result == 1) {	// если предыдущие элементы равны

			// сравниваем биты от 0 до (BitLen % BITS_IN_TELEM) чисел number1, number2
			// number1 = aa...aaxxx...x		||	нужно сравнить xxx...x и yyy...y
			// number2 = bb...bbyyy...y		||	для этого нужно найти их битовую длину, которая одинаковая у них
			// int bitWidth = BitLen % BITS_IN_TELEM
			// BitLen == B_I_T * (MemLen - 1) + (BitLen % B_I_T)
			// bitWidth - битовая ширина xx...xx и yy...yy, кол-во бит в xx...xx и  yy...yy

			TELEM number1 = pMem[MemLen - 1], number2 = pMem[MemLen - 1];
			int bitWidth = BitLen - BITS_IN_TELEM * (MemLen - 1);
			int shift = BITS_IN_TELEM - bitWidth;

			number1 = number1 << shift;
			number2 = number2 << shift;
			// теперь number1 = xx...xx00..00, number2 = yy...yy00...00

			result = number1 == number2;
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
	// для result вызов конструктора копирования TBitField(const TBitField&), принимающее битовое поле наибольшей длины
	TBitField result = BitLen > bf.BitLen ? *this : bf;
	// выделяем динамическую память
	const TBitField* minBitField;	// указатель, указывающий на битовое поле наименьшей длины
	const TBitField* maxBitField;	// аналогично, но для битового поля наибольшей длины
	int temp;

	if (BitLen < bf.BitLen) {
		minBitField = this;
		maxBitField = &bf;
	}
	else {
		minBitField = &bf;
		maxBitField = this;
	}

	temp = minBitField->MemLen;
	for (int i = 0; i < temp - 1; ++i) {	// применяем operator| для элементов pMem от 0 до minMemLen - 2
		result.pMem[i] = minBitField->pMem[i] | maxBitField->pMem[i];
	}

	// ДОПИСАТЬ!!!

	// переназначаем указатели, чтобы не удалялись массивы pMem у *this и bf
	// minBitField = nullptr;	
	// maxBitField = nullptr;
	// Нет нужды, так как minBitField и maxBitField - это не объекты, а указатели на них

	// освобождаем память от динамических данных
	delete minBitField;
	delete maxBitField;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{

}

TBitField TBitField::operator~(void) // отрицание
{

}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	
}

// TBitField bf1: MemLen = 4, BitLen = 15
// |----+---+---+---|---+---+---+---|---+---+---+---|***+---+---+---|	
// |3	|2	|1	|0	|7	|6	|5	|4	|11	|10	|9	|8	|***|14	|13	|12	|
// |----+---+---+---|---+---+---+---|---+---+---+---|***+---+---+---|
//			0				1				2				3
// +----+---+---+---|---+---+---+---|*******+---+---|
// |3	|2	|1	|0	|7	|6	|5	|4	|*******|9	|8	|
// +----+---+---+---|---+---+---+---|*******+---+---|
// TBitField bf2: MemLen = 3, BitLen = 10