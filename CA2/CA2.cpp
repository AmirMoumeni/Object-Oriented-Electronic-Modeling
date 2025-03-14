#include <iostream>

using namespace std;

class wire
{
private:
    char val;

public:
    char value() {return val;}
    void setValue(char v) {val = v;}
};

class gates
{
protected:
    wire *i1, *i2, *o1;
    int gateDalay;

public:
    gates(wire &a, wire &b, wire &w, int d) : i1(&a), i2(&b), o1(&w), gateDalay(d) {}
    gates(wire &a, wire &w , int d) : i1(&a), o1(&w), gateDalay(d) {}
    gates() {};
    ~gates() {};

    void evl() {};
};

class Or : public gates
{
public:
    Or(wire &a, wire &b, wire &w, int d) : gates(a, b, w, d) {}
    ~Or() {};
    void evl();
};

class And : public gates
{
public:
    And(wire &a, wire &b, wire &w, int d) : gates(a, b, w, d) {}
    ~And() {};
    void evl();
};

class Not : public gates
{
public:
    Not(wire &a, wire &w, int d) : gates(a, w, d) {}
    ~Not() {};
    void evl();
};

class Xor : public gates
{
public:
    Xor(wire &a, wire &b, wire &w, int d) : gates(a, b, w, d) {}
    ~Xor() {};
    void evl();
};

void Or::evl()
{
    if (i1->value() == '1' || i2->value() == '1')
        o1->setValue('1');
    else if (i1->value() == '0' && i2->value() == '0')
        o1->setValue('0');
    else 
        o1->setValue('X');
}

void And::evl()
{
    if (i1->value() == '1' && i2->value() == '1')
        o1->setValue('1');
    else if (i1->value() == '0' || i2->value() == '0')
        o1->setValue('0');
    else 
        o1->setValue('X');
}

void Not::evl()
{
    if (i1->value() == '1')
        o1->setValue('0');
    else if (i1->value() == '0')
        o1->setValue('1');
    else 
        o1->setValue('X');
}

void Xor::evl()
{
    if ( (i1->value() == '1' && i2->value() == '0') || (i1->value() == '0' && i2->value() == '1'))
        o1->setValue('1');
    else if ( (i1->value() == '0' && i2->value() == '0') || (i1->value() == '1' && i2->value() == '1'))
        o1->setValue('0');
    else 
        o1->setValue('X');
}