// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h" 

///////////////////////////////////////////////////////////////////////////////////////
//  ВАЖНО!  1)  MaxPower > 0
//              если MaxPower > 0, т.е. множество конечное, то BitField создаёстся длиной BitLen = MaxPower
//              иначе вызывается исключение в конструкторах TBitField и TSet
///////////////////////////////////////////////////////////////////////////////////////

TSet::TSet(int mp) : BitField(mp), MaxPower(mp)
{
}

// конструктор копирования
TSet::TSet(const TSet& s) : BitField(s.BitField), MaxPower(s.MaxPower)
{
}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : BitField(bf), MaxPower(bf.GetLength())
{
}

TSet::operator TBitField()
{
    return BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    if (Elem < 0 || Elem >= MaxPower)
        throw out_of_range("EXCEPTION: TSet::IsMember");

    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if (Elem < 0 || Elem >= MaxPower)
        throw out_of_range("TSet::InsElem");

    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if (Elem < 0 || Elem >= MaxPower)
        throw out_of_range("TSet::InsElem");

    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    BitField = s.BitField;
    MaxPower = s.MaxPower;

    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    return BitField == s.BitField;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return !(this->operator==(s));
}

TSet TSet::operator+(const TSet &s) // объединение
{
    TSet result(this->BitField | s.BitField);

    return result;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    if (Elem < 0)
        throw out_of_range("TSet::operator+(const int)");
    
    if (0 <= Elem < MaxPower) {
        TSet result(*this);
        result.InsElem(Elem);

        return result;
    }
    else {
        TSet result(Elem);
        result.InsElem(Elem);
        result = result + *this;

        return result;
    }
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    if (Elem < 0)
        throw out_of_range("TSet::operator-(const int)");

    TSet result(*this);

    if (0 <= Elem < MaxPower)
        result.DelElem(Elem);

    return result;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    TSet result(this->BitField & s.BitField);

    return result;
}

TSet TSet::operator~(void) // дополнение
{
    TSet result(*this);
    result.BitField = ~result.BitField;

    return result;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    char c;             // переменная, в которую будет записываться входящий символ
    int number;         // число
    int width_number;   // сколько символов было затрачено, чтобы ввести number

    number = 0;
    width_number = 0;
    istr >> c;
    
    while (c != '\n' && c != EOF) {
        if ((c < '0' || '9' < c) && c != ' ')
            throw "TSet::operator>>, wrong incoming char";

        if (c == ' ' && width_number > 0) {
            if (number >= s.GetMaxPower())
                throw out_of_range("TSet::operator>>, wrong incoming number");

            s.InsElem(number);
            number = 0;
            width_number = 0;
        }
        else {
            number = number * 10 + int(c - '0');
            ++width_number;
        }
    }

    return istr; 
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    for (int i = 0; i < s.MaxPower; ++i)
        if (s.IsMember(i))
            ostr << i << ' ';
    ostr << '\b';

    return ostr;
}
