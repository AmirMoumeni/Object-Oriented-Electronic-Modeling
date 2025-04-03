#include "gate.hpp"

void Or::evl()
{
    if (i1->value() == ONE || i2->value() == ONE)
        o1->setValue(ONE);
    else if (i1->value() == ZERO && i2->value() == ZERO)
        o1->setValue(ZERO);
    else
        o1->setValue(UNKNOWN);

    flag = (lastOutputValue == o1->value());
    lastOutputValue = o1->value();
}

void And::evl()
{
    if (i1->value() == ONE && i2->value() == ONE)
        o1->setValue(ONE);
    else if (i1->value() == ZERO || i2->value() == ZERO)
        o1->setValue(ZERO);
    else
        o1->setValue(UNKNOWN);

    flag = (lastOutputValue == o1->value());
    lastOutputValue = o1->value();
}

void Not::evl()
{
    if (i1->value() == ONE)
        o1->setValue(ZERO);
    else if (i1->value() == ZERO)
        o1->setValue(ONE);
    else
        o1->setValue(UNKNOWN);

    flag = (lastOutputValue == o1->value());
    lastOutputValue = o1->value();
}

void Nand::evl()
{
    if (i1->value() == ONE && i2->value() == ONE)
        o1->setValue(ZERO);
    else if (i1->value() == ZERO || i2->value() == ZERO)
        o1->setValue(ONE);
    else
        o1->setValue(UNKNOWN);

    flag = (lastOutputValue == o1->value());
    lastOutputValue = o1->value();
}

void Nor::evl()
{
    if (i1->value() == ONE || i2->value() == ONE)
        o1->setValue(ZERO);
    else if (i1->value() == ZERO && i2->value() == ZERO)
        o1->setValue(ONE);
    else
        o1->setValue(UNKNOWN);

    flag = (lastOutputValue == o1->value());
    lastOutputValue = o1->value();
}

void Xor::evl()
{
    if ( (i1->value() == ONE && i2->value() == ZERO) || (i1->value() == ZERO && i2->value() == ONE))
        o1->setValue(ONE);
    else if ( (i1->value() == ZERO && i2->value() == ZERO) || (i1->value() == ONE && i2->value() == ONE))
        o1->setValue(ZERO);
    else
        o1->setValue(UNKNOWN);

    flag = (lastOutputValue == o1->value());
    lastOutputValue = o1->value();
}

void Or_3::evl()
{
    if (i1->value() == ONE || i2->value() == ONE || i3->value() == ONE)
        o1->setValue(ONE);
    else if (i1->value() == ZERO && i2->value() == ZERO && i3->value() == ZERO)
        o1->setValue(ZERO);
    else
        o1->setValue(UNKNOWN);
    flag = (lastOutputValue == o1->value());
    lastOutputValue = o1->value();
}

void Or_4::evl()
{
    if (i1->value() == ONE || i2->value() == ONE || i3->value() == ONE || i4->value() == ONE)
        o1->setValue(ONE);
    else if (i1->value() == ZERO && i2->value() == ZERO && i3->value() == ZERO && i4->value() == ZERO)
        o1->setValue(ZERO);
    else
        o1->setValue(UNKNOWN);
    flag = (lastOutputValue == o1->value());
    lastOutputValue = o1->value();
}

void And_3::evl()
{
    if (i1->value() == ONE && i2->value() == ONE && i3->value() == ONE)
        o1->setValue(ONE);
    else if (i1->value() == ZERO || i2->value() == ZERO || i3->value() == ZERO)
        o1->setValue(ZERO);
    else
        o1->setValue(UNKNOWN);

    flag = (lastOutputValue == o1->value());
    lastOutputValue = o1->value();
}

void gates::showWires(){
    cout << typeid(*this).name() <<" out: " <<o1->getName() <<" in: " << i1->getName() << " " << i2->getName() << endl;
}