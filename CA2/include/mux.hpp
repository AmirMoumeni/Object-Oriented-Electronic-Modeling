#ifndef MUX_INCLUDE
#define MUX_INCLUDE

#include "global.hpp"
#include "wire.hpp"
#include "gate.hpp"

class Mux {
protected:
    Wire *i0, *i1;
    Wire *not_i0, *not_i1;
    Wire *d0, *d1, *d2, *d3;
    Wire *o1;
    int moduleDelay;
    char lastOutputValue;
    bool flag;
public:
    Mux(Wire &a, Wire &b, Wire &c, Wire &d, Wire &e, Wire &f, Wire &w, int delay);
    Mux() {};
    ~Mux();

    void evl();
    char out() {return o1->value(); }
};

#endif