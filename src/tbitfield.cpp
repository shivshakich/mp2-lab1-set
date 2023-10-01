// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

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
		a >> 1;
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
	return 0;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return 0;
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
	return*this;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	return istr;

}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	return ostr;

}
