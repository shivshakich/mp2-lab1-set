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
	const TBitField* maxBitField;					// указатель на объект-битовое_поле максимальной длины 
	const TBitField* minBitField;					// указатель на объект-битовое_поле минимальной длины 

	// находим минимальное и максимальное по длине битовые поля 
	if (this->BitLen < bf.BitLen) {
		minBitField = this;
		maxBitField = &bf;
	}
	else {
		minBitField = &bf;
		maxBitField = this;
	}

	TBitField result(*maxBitField);

	int temp = minBitField->MemLen - 1;
	for (int i = 0; i < temp; ++i) {
		result.pMem[i] = minBitField->pMem[i] | maxBitField->pMem[i];
	}

	int bitWidth = minBitField->BitLen - BITS_IN_TELEM * temp;	// ширина последнего элемента minBitField->pMem
	int shift = BITS_IN_TELEM - bitWidth;
	TELEM bits = ((minBitField->pMem[temp]) << shift) >> shift;
	result.pMem[temp] = bits | maxBitField->pMem[temp];

	// a | 0 == a, поэтому дальше не преобразуем элементы result.pMem

	// не знаю, как работает delete 
	maxBitField = nullptr;
	minBitField = nullptr;
	delete maxBitField;
	delete minBitField;

	return result;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	const TBitField* maxBitField;					// указатель на объект-битовое_поле максимальной длины 
	const TBitField* minBitField;					// указатель на объект-битовое_поле минимальной длины 

	// находим минимальное и максимальное по длине битовые поля 
	if (this->BitLen < bf.BitLen) {		
		minBitField = this;
		maxBitField = &bf;
	}
	else {
		minBitField = &bf;
		maxBitField = this;
	}

	TBitField result(maxBitField->BitLen);	

	int temp = minBitField->MemLen - 1;
	for (int i = 0; i < temp; ++i) {
		result.pMem[i] = minBitField->pMem[i] & maxBitField->pMem[i];
	}
	
	int bitWidth = minBitField->BitLen - BITS_IN_TELEM * temp;	// ширина последнего элемента minBitField->pMem
	int shift = BITS_IN_TELEM - bitWidth;
	TELEM bits = ((minBitField->pMem[temp]) << shift) >> shift;
	result.pMem[temp] = bits & maxBitField->pMem[temp];

	// все остальные элементы result.pMem и так равны 0

	// не знаю, как работает delete 
	maxBitField = nullptr;
	minBitField = nullptr;
	delete maxBitField;
	delete minBitField;

	return result;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField res(*this);

	int temp = res.MemLen;
	for (int i = 0; i < temp; ++i)
		res.pMem[i] = ~(res.pMem[i]);

	return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	char c;							// переменная, принимающая входные значения
	TELEM bits;						// переменная, в которую будут записываться c
	int index_bits;					// кол-во занятых ячеек в bits

	TBitField temp(bf.BitLen);		// куда будут записываться c
	int bitLen;						// сколько раз переназначалась c
	int memLen_temp;				// скольок раз элементам temp.pMem присваивалось значение bits

	istr >> c;

	if (c != '0' && c != '1')
		throw "TBitField::operator>>, first incoming symbol must be '0' or '1'";

	bits = TELEM(c - '0');
	index_bits = 1;
	bitLen = 1;
	memLen_temp = 0;

	const int BF_BITLEN = bf.BitLen;
	while (bitLen < BF_BITLEN) 
	{
		istr >> c;

		if (c == ' ' || c == '\n' || c == EOF) {
			temp.pMem[memLen_temp++] = bits;
			break;
		}
		else if (c != '0' && c != '1')
			throw "TBitField::operator>>, wrong incoming symbol";

		bits = (bits << 1) + TELEM(c - '0');
		++index_bits;
		++bitLen;

		if (index_bits == BITS_IN_TELEM) {
			temp.pMem[memLen_temp++] = bits;
			bits = 0;
			index_bits = 0;
		}
	}

	if (bitLen != BF_BITLEN) 
		bf = TBitField(bitLen);

	for (int i = 0; i < temp.MemLen; ++i)
		bf.pMem[i] = temp.pMem[i];

	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	TELEM c;

	int temp = bf.MemLen - 1;
	for (int i = 0; i < temp; ++i) {
		TELEM bits = bf.pMem[i];

		for (int j = 0; j < BITS_IN_TELEM; ++j) {
			c = bits & TELEM(1);
			bits = bits >> 1;
			ostr << c;
		}
	}

	TELEM bits = bf.pMem[temp];

	for (int j = 0, temp = bf.BitLen - BITS_IN_TELEM * temp; j < temp; ++j) {
		c = bits & TELEM(1);
		bits = bits >> 1;
		ostr << c;
	}

	return ostr;
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