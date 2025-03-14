#include "mux.hpp"

Mux::Mux(Wire &a, Wire &b, Wire &c, Wire &d, Wire &e, Wire &f, Wire &w, int delay)
: i0(&a), i1(&b), d0(&c), d1(&d), d2(&e), d3(&f), o1(&w), moduleDelay(delay) {
    not_i0 = new Wire();
    not_i1 = new Wire();

    Not *notGate = new Not(*i0, *not_i0, 0);
    Not *notGate1 = new Not(*i1, *not_i1, 0);
    notGate->evl();
    notGate1->evl();

}

void Mux::evl()
{
    Wire *out0, *out1, *out2, *out3;
    out0 = new Wire();
    out1 = new Wire();
    out2 = new Wire();
    out3 = new Wire();

    And_3 *out0And = new And_3(*not_i0, *not_i1, *d0, *out0, 0);
    And_3 *out1And = new And_3(*i0, *not_i1, *d1, *out1, 0);
    And_3 *out2And = new And_3(*not_i0, *i1, *d2, *out2, 0);
    And_3 *out3And = new And_3(*i0, *i1, *d3, *out3, 0);

    out0And->evl();
    out1And->evl();
    out2And->evl();
    out3And->evl();

    Or_4 *finalGate = new Or_4(*out0, *out1, *out2, *out3, *o1, 0);
    finalGate->evl();

    flag = (lastOutputValue == o1->value());
    lastOutputValue = o1->value();

    delete out0And;
    delete out1And;
    delete out2And;
    delete out3And;
    delete finalGate;
    delete out0;
    delete out1;
    delete out2;
    delete out3;
}

Mux::~Mux() {
    delete not_i0;
    delete not_i0;
}