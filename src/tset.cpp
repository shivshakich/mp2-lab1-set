// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h" 

///////////////////////////////////////////////////////////////////////////////////////
//  ВАЖНО!  1)  MaxPower >= 0
//              если MaxPower = 0, т.е. пустое множество, то BitField должен иметь указывать на nullptr
//              если MaxPower > 0, т.е. множество конечное, то BitField создаёстся длиной MaxPower
//              иначе вызывается исключение в конструкторах TBitField и TSet
///////////////////////////////////////////////////////////////////////////////////////

TSet::TSet(int mp) : BitField(mp == 0 ? 1 : mp)
{
    if (mp < 0)
        throw out_of_range("EXCEPTION: TSet::TSet(int), mp < 0");

    if (mp == 0) // присваиваем BitField = nullptr и освобождаем память от BitField
        BitField.~TBitField();
}

// конструктор копирования
TSet::TSet(const TSet& s) : BitField(s.BitField.GetLength() == 0 ? 1 : s.BitField)
{
    // s.BitField.GetLength() == 0, тогда и только тогда, когда s был создан через конструктор TSet::TSet(int),
    // которому передали 0 в качестве аргумента

    MaxPower = s.MaxPower;

    if (MaxPower == 0)  // если требуется пустое множество
        BitField.~TBitField();

    // для BitField ещё в заголовке был отработан конструктор копирования TBitField::TBitField(const TBitField&) 
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf)
{
    // здесь принимается TBitField, который не может указывать на nullptr 
    // (если, конечно, к нему заранее не был применён деструктор ~TBitField)

    MaxPower = bf.GetLength();
}

TSet::operator TBitField()
{
    if (MaxPower = 0) { // вернётся TBitField длины BITS_IN_TELEM битов
        TBitField result(1);    // pMem[0] == 0 согласно конструктору TBitField::TBitField(int)
        return result;
    }
    else
        return BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    int result = (0 <= Elem && Elem < MaxPower);
    // если MaxPower == 0, то только 0
    // result проверяет, входит ли Elem в диапозон BitField

    if (result) // если Elem входит в диапозон BitField
        result = BitField.GetBit(Elem);

    return result;
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
<<<<<<< HEAD
    return;
=======
    if (Elem < 0)
        throw "EXCEPTION: TSet::InsElem, Elem < 0";

>>>>>>> 2c2caa15f898af9151e7367449ffa0c7893c5378
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
