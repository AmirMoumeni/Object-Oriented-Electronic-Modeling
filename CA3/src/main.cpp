
#include <systemc.h>
#include "multiplier.h"
#include "approx_multiplier.h"
#include "ApproxMulBFM.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

int sc_main(int argc, char* argv[]) {

    sc_clock clk("clk", sc_time(10, SC_NS));
    sc_signal<bool> reset_n;


    sc_signal<sc_uint<8>> A8, B8;
    sc_signal<sc_uint<16>> W8;
    sc_signal<bool> start8, ready8;

    sc_signal<sc_uint<16>> A16_rtl, B16_rtl;
    sc_signal<sc_uint<16>> W16_rtl;
    sc_signal<bool> start16_rtl, ready16_rtl;

    sc_signal<sc_logic> start_bfm, ready_bfm;
    sc_signal<sc_lv<DATA_WIDTH>> A16_bfm, B16_bfm, W16_bfm;

    Multiplier mul8("Multiplier8");
    mul8.clk(clk); mul8.rst(reset_n);
    mul8.A(A8); mul8.B(B8); mul8.W(W8);
    mul8.startB(start8); mul8.readyB(ready8);

    ApproxMultiplier rtl16("ApproxRTL16");
    rtl16.clk(clk); rtl16.rst(reset_n);
    rtl16.A(A16_rtl); rtl16.B(B16_rtl); rtl16.W(W16_rtl);
    rtl16.start(start16_rtl); rtl16.ready(ready16_rtl);

    ApproxMulBFM bfm16("ApproxBFM16");
    bfm16.clk(clk); bfm16.reset_n(reset_n);
    bfm16.start_sig(start_bfm); bfm16.ready_sig(ready_bfm);
    bfm16.in_A(A16_bfm); bfm16.in_B(B16_bfm); bfm16.out_mul(W16_bfm);

    reset_n = false;
    start8 = false; start16_rtl = false; start_bfm = SC_LOGIC_0;
    sc_start(20, SC_NS);
    reset_n = true;
    sc_start(20, SC_NS);

    srand(time(nullptr));

    for (int i = 0; i < 5; ++i) {
        unsigned int a8 = rand() % 256;
        unsigned int b8 = rand() % 256;
        A8.write(a8); B8.write(b8);
        start8 = true;
        sc_start(10, SC_NS);
        start8 = false;
        while (!ready8.read()) sc_start(10, SC_NS);
        unsigned int exact = a8 * b8;
        cout << "[8-bit RTL] A=" << a8 << " B=" << b8
                  << " => W=" << W8.read() << " (expected=" << exact << ")";

        unsigned int a16 = rand() % 8192;
        unsigned int b16 = rand() % 8192;
        A16_rtl.write(a16); B16_rtl.write(b16);
        start16_rtl = true;
        sc_start(10, SC_NS);
        start16_rtl = false;
        while (!ready16_rtl.read()) sc_start(10, SC_NS);
        cout << "[16-bit RTL Approx] A=" << a16 << " B=" << b16
                  << " => W=" << W16_rtl.read() << " ";

        A16_bfm = sc_lv<DATA_WIDTH>(a16);
        B16_bfm = sc_lv<DATA_WIDTH>(b16);
        start_bfm = SC_LOGIC_1;
        sc_start(10, SC_NS);
        start_bfm = SC_LOGIC_0;
        while (ready_bfm.read() != SC_LOGIC_1) sc_start(10, SC_NS);
        cout << "[16-bit BFM Approx] A=" << hex << a16 << " B=" << b16 << " => W=0x" << W16_bfm.read().to_uint() << dec << " ";
    }

    sc_stop();
    return 0;
}
