#ifndef GATE_INCLUDE
#define GATE_INCLUDE

#include "global.hpp"
#include "wire.hpp"

class gates
{
protected:
    string name;
    Wire *i1, *i2, *i3, *i4, *o1;
    int gateDelay;
    char lastOutputValue;

public:
    gates(Wire &a, Wire &b, Wire &c , Wire &d, Wire &w, int delay) :  i1(&a), i2(&b), i3(&c), i4(&d), o1(&w), gateDelay(delay) {};
    gates(Wire &a, Wire &b, Wire &c , Wire &w, int delay) :  i1(&a), i2(&b), i3(&c), o1(&w), gateDelay(delay) {};
    gates(Wire &a, Wire &b, Wire &w, int delay) : i1(&a), i2(&b), o1(&w), gateDelay(delay) {};
    gates(Wire &a, Wire &w , int delay) : i1(&a), o1(&w), gateDelay(delay) {};
    gates() {};
    virtual ~gates() {};
    bool flag;
    bool getFlag() {return flag;}
    virtual void evl() {};
    char out() {return o1->value(); }
    void introduce();
};

class Or : public gates
{
public:
    Or(Wire &a, Wire &b, Wire &w, int delay) : gates(a, b, w, delay) { name = "OR";};
    ~Or() {};
    void evl();
};

class And : public gates
{
public:
    And(Wire &a, Wire &b, Wire &w, int delay) : gates(a, b, w, delay) {name = "AND";};
    ~And() {};
    void evl();
};

class Not : public gates
{
public:
    Not(Wire &a, Wire &w, int delay) : gates(a, w, delay) {name = "NOT";};
    ~Not() {};
    void evl();
};

class Nand : public gates
{
public:
    Nand(Wire &a, Wire &b, Wire &w, int delay) : gates(a, b, w, delay) {name = "NAND";};
    ~Nand() {};
    void evl();
};

class Nor : public gates
{
public:
    Nor(Wire &a, Wire &b, Wire &w, int delay) : gates(a, b, w, delay) {name = "NOR";};
    ~Nor() {};
    void evl();
};

class Xor : public gates
{
public:
    Xor(Wire &a, Wire &b, Wire &w, int delay) : gates(a, b, w, delay) {name = "XOR";};
    ~Xor() {};
    void evl();
};

class And_3 : public gates
{
    public:
    And_3(Wire &a, Wire &b, Wire &c, Wire &w, int delay) : gates(a, b, c, w, delay) {};
    ~And_3() {};
    void evl();
};

class Or_3 : public gates
{
    public:
    Or_3(Wire &a, Wire &b, Wire &c, Wire &w, int delay) : gates(a, b, c, w, delay) {};
    ~Or_3() {};
    void evl();
};

class Or_4 : public gates
{
    public:
    Or_4(Wire &a, Wire &b, Wire &c, Wire &d, Wire &w, int delay) : gates(a, b, c, d, w, delay) {};
    ~Or_4() {};
    void evl();
};

#endif