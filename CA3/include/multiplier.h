#ifndef MULTIPLIER_H
#define MULTIPLIER_H

#include <systemc.h>

SC_MODULE(Multiplier) {
    sc_in<bool> clk;
    sc_in<bool> rst;

    sc_in<sc_uint<8>> A;
    sc_in<sc_uint<8>> B;
    sc_in<bool> startB;

    sc_out<sc_uint<16>> W;
    sc_out<bool> readyB;

    void multiply_process();

    SC_CTOR(Multiplier) {
        SC_METHOD(multiply_process);
        sensitive << clk.pos();
        sensitive << rst;
    }

private:
    sc_signal<sc_uint<8>> reg_A, reg_B;
    sc_signal<sc_uint<16>> result;
    sc_signal<bool> busy;
};

#endif
