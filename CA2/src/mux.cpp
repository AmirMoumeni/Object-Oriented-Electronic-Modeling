#include "mux.hpp"

Mux::Mux(Wire &a, Wire &b, Wire &c, Wire &d, Wire &e, Wire &f, Wire &w, int delay)
: i0(&a), i1(&b), d0(&c), d1(&d), d2(&e), d3(&f), o1(&w), moduleDelay(delay) {
    not_i0 = new Wire();
    not_i1 = new Wire();

    Not *notGate = new Not(*i0, *not_i0, 0);
    Not *notGate1 = new Not(*i1, *not_i1, 0);
    notGate->evl();
    notGate1->evl();
    delete notGate;
    delete notGate1;

}


void Mux::evl() {
    Wire out0, out1, out2, out3;

    And_3 out0And(*not_i0, *not_i1, *d0, out0, 0);
    And_3 out1And(*i0, *not_i1, *d1, out1, 0);
    And_3 out2And(*not_i0, *i1, *d2, out2, 0);
    And_3 out3And(*i0, *i1, *d3, out3, 0);

    do {
        out0And.evl();
        out1And.evl();
        out2And.evl();
        out3And.evl();
    }
    while (out0And.getFlag() || out1And.getFlag() || out2And.getFlag() || out3And.getFlag());

    Or_4 finalGate(out0, out1, out2, out3, *o1, 0);
    finalGate.evl();
    flag = (lastOutputValue == o1->value());
    lastOutputValue = o1->value();
}

Mux::~Mux() {
    delete not_i0;
    delete not_i1;
}