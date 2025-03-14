#ifndef GATE_INCLUDE
#define GATE_INCLUDE

#include "global.hpp"
#include "wire.hpp"

class gates
{
protected:
    Wire *i1, *i2, *i3, *o1;
    int gateDalay;
    char lastOutputValue;
    bool flag;

public:
    gates(Wire &a, Wire &b, Wire &c , Wire &w, int d) :  i1(&a), i2(&b), i3(&c), o1(&w), gateDalay(d) {}
    gates(Wire &a, Wire &b, Wire &w, int d) : i1(&a), i2(&b), o1(&w), gateDalay(d) {}
    gates(Wire &a, Wire &w , int d) : i1(&a), o1(&w), gateDalay(d) {}
    gates() {};
    ~gates() {};

    void evl() {};
    char out() {return o1->value(); }
};

class Or : public gates
{
public:
    Or(Wire &a, Wire &b, Wire &w, int d) : gates(a, b, w, d) {}
    ~Or() {};
    void evl();
};

class And : public gates
{
public:
    And(Wire &a, Wire &b, Wire &w, int d) : gates(a, b, w, d) {}
    ~And() {};
    void evl();
};

class Not : public gates
{
public:
    Not(Wire &a, Wire &w, int d) : gates(a, w, d) {}
    ~Not() {};
    void evl();
};

class Xor : public gates
{
public:
    Xor(Wire &a, Wire &b, Wire &w, int d) : gates(a, b, w, d) {}
    ~Xor() {};
    void evl();
};

class And_3 : public gates
{
    public:
    And_3(Wire &a, Wire &b, Wire &c, Wire &w, int d) : gates(a, b, c, w, d) {}
    ~And_3() {};
    void evl();
};

class Or_3 : public gates
{
    public:
    Or_3(Wire &a, Wire &b, Wire &c, Wire &w, int d) : gates(a, b, c, w, d) {}
    ~Or_3() {};
    void evl();
};




#endif