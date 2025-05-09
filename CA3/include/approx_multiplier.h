#ifndef APPROX_MULTIPLIER_H
#define APPROX_MULTIPLIER_H

#include <systemc.h>
#include "multiplier.h"

SC_MODULE(ApproxMultiplier) {
    sc_in<sc_uint<16>> A, B;
    sc_out<sc_uint<16>> W;
    sc_in<bool> start;
    sc_out<bool> ready;
    sc_in<bool> clk, rst;

    sc_signal<sc_uint<8>> a_part, b_part;
    sc_signal<sc_uint<16>> result;
    sc_signal<bool> start8, ready8;

    Multiplier* mult8;

    void approx_process();

    SC_CTOR(ApproxMultiplier);
};

#endif
