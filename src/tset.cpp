// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

//////////////////////////////////////////////////////////////////////////////
//  ВАЖНО!  TSet будет хранить только числа от 0 до (MaxPower - 1)
//          Пустое множество: MaxPower = 0, создаcтся битовое поле длины 1
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// 1)   проверить, что возвращает оператор преобразования типа
//////////////////////////////////////////////////////////////////////////////

TSet::TSet(int mp) : BitField(mp == 0 ? 1 : mp)
{
    // если mp < 0, то исключение вызовется в конструкторе TBitField(int), которое примет значение mp
    // если mp == 0, то TBitField(int) примет 1, и исключение в нём не сработает
    // если mp > 0, то конструктор сработает без вызова исключений

    if (mp < 0)
        throw out_of_range("EXCEPTION: constructor TSet(int mp), mp < 0");

    MaxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.BitField)
{
    MaxPower = s.MaxPower;
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf)
{
    MaxPower = bf.GetLength();
}

TSet::operator TBitField()
{
    // возвращает копию или ссылку ?
    return BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    int result = 0;

    // нужно ли тут MaxPower == 0 ?
    if (Elem < 0 || Elem > MaxPower - 1 || MaxPower == 0)
        result = 0;
    else
        result = BitField.GetBit(Elem);

    return 0;
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    return;
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    return 0;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return 0;
}

TSet TSet::operator+(const TSet &s) // объединение
{
    return *this;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    return *this;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    return *this;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    return *this;
}

TSet TSet::operator~(void) // дополнение
{
    return *this;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    return ostr;
}
