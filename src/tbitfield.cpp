// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// TODO:
// 1)	В последнем элементе pMem неиспользуемые ячейки обязаны иметь значение 0 ?
// 2)	метод GetMemIndex должен проверять n на его вхождение в область определения метода ?
//		Если да, то нужно переписать operator>>,
//		если нет, то почему GetMemIndex - метод, а дружественная функция, например
static const int BITS_IN_TELEM = 8 * sizeof(TELEM);

TBitField::TBitField(int len)
{
	if (len <= 0)
		throw out_of_range("constructor TBitField, len <= 0");

	BitLen = len;
	MemLen = BitLen / BITS_IN_TELEM + ((BitLen % BITS_IN_TELEM) > 0);
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
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	// return n / BITS_IN_TELEM;

	int a = BITS_IN_TELEM;
	int bitWidth = 0;
	while (a > 0) {
		a = a >> 1;
		++bitWidth;
	}
	// число BITS_IN_TELEM занимает bitWidth бит

	return n >> (bitWidth - 1);	
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	// int elemIndex = n % BITS_IN_TELEM;
	// n == B_I_T * (n / B_I_T) + (n % B_I_T)	==>		n % B_I_T == n - B_I_T * (n / B_I_T)
	// n / B_I_T == GetMemIndex(n)

	int elemIndex = n - BITS_IN_TELEM * GetMemIndex(n);
	// получили 0 <= elemIndex < BITS_IN_TELEM

	return TELEM(1 << elemIndex);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	// SetBit присваивает биту n значение 1 ?

	int memIndex = GetMemIndex(n);
	TELEM bitMask = GetMemMask(n);

	pMem[memIndex] = pMem[memIndex] | bitMask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
	int memIndex = GetMemIndex(n);
	TELEM bitMask = GetMemMask(n);

	bitMask = ~(bitMask);	// operator~ - побитовая инверсия
	// bitMask = 0...010...0	==>		bitMask = 1...101...1

	pMem[memIndex] = pMem[memIndex] & bitMask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	int memIndex = GetMemIndex(n);
	TELEM number = pMem[memIndex];

	TELEM bitMask = GetMemMask(n);

	return int((number & bitMask) > 0);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this != &bf) {
		TBitField temp(bf);

		delete[] pMem;
		pMem = nullptr;
		BitLen = 0;
		MemLen = 0;

		pMem = temp.pMem;
		temp.pMem = nullptr;
		BitLen = temp.BitLen;
		MemLen = temp.MemLen;
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

		for (int i = 0; i < MemLen - 1; ++i)
			if (pMem[i] != bf.pMem[i]) {
				result = 0;
				break;
			}

		if (result == 1) {
			TELEM number1 = pMem[MemLen - 1], number2 = bf.pMem[MemLen - 1];
			
			// сравниваем биты от 0 до (BitLen % BITS_IN_TELEM) чисел number1, number2
			// number1 = aa...aaxxx...x		||	нужно сравнить xxx...x и yyy...y
			// number2 = bb...bbyyy...y		||	для этого нужно найти их битовую длину, которая одинаковая у них

			// int bitWidth = BitLen % BITS_IN_TELEM
			// BitLen == B_I_T * (MemLen - 1) + (BitLen % B_I_T)
			int bitWidth = BitLen - BITS_IN_TELEM * (MemLen - 1);
			// bitWidth - битовая ширина xx...xx и yy...yy

			number1 = number1 << (BITS_IN_TELEM - bitWidth);
			number2 = number2 << (BITS_IN_TELEM - bitWidth);
			// теперь	number1 == x...x0...0
			//			number2 == y...y0...0

			if (number1 != number2)
				result = 0;
		}
	}

	return result;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	int result = this->operator==(bf);
	result = result == 0 ? 1 : 0;

	return result;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	return*this;
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
