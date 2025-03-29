#ifndef WIRE_INCLUDE
#define WIRE_INCLUDE

#include "global.hpp"

class Wire
{
private:
    char val;
    string name;

public:
    Wire(string assign_name = "") {name = assign_name;};
    char value() {return val;}
    void setValue(char v) {val = v;}
    string getName() {return name;}
};

#endif