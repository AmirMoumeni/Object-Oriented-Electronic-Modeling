#ifndef WIRE_INCLUDE
#define WIRE_INCLUDE

#include "global.hpp"

class Wire
{
private:
    char val;

public:
    Wire() {};
    char value() {return val;}
    void setValue(char v) {val = v;}
};

#endif