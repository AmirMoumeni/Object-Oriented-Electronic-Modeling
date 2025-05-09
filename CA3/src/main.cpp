#include <systemc.h>
#include "multiplier.h"
#include "approx_multiplier.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

int sc_main(int argc, char* argv[]) {
    sc_clock clk("clk", 10, SC_NS);
    sc_signal<bool> rst;

    // ----- Signals for Multiplier -----
    sc_signal<sc_uint<8>> A8, B8;
    sc_signal<sc_uint<16>> W8;
    sc_signal<bool> start8, ready8;

    // ----- Signals for ApproxMultiplier -----
    sc_signal<sc_uint<16>> A16, B16, W16;
    sc_signal<bool> start16, ready16;

    // Instantiate modules
    Multiplier multiplier("multiplier");
    multiplier.A(A8); multiplier.B(B8); multiplier.W(W8);
    multiplier.startB(start8); multiplier.readyB(ready8);
    multiplier.clk(clk); multiplier.rst(rst);

    ApproxMultiplier approx("approx_multiplier");
    approx.A(A16); approx.B(B16); approx.W(W16);
    approx.start(start16); approx.ready(ready16);
    approx.clk(clk); approx.rst(rst);

    // Start simulation
    sc_start(1, SC_NS);
    rst.write(true);
    sc_start(10, SC_NS);
    rst.write(false);
    sc_start(10, SC_NS);

    std::srand(std::time(nullptr));

    // ----------- Test Multiplier -----------
    sc_uint<8> a_val = std::rand() % 256;
    sc_uint<8> b_val = std::rand() % 256;

    A8.write(a_val);
    B8.write(b_val);
    start8.write(true);
    sc_start(10, SC_NS);
    start8.write(false);

    while (!ready8.read()) {
        sc_start(10, SC_NS);
    }

    sc_uint<16> expected_exact = a_val * b_val;

    std::cout << "[Multiplier]\n"
              << "  A = " << a_val << ", B = " << b_val << "\n"
              << "  Output     = " << W8.read() << "\n"
              << "  Expected   = " << expected_exact << "\n\n";

    // ----------- Test ApproxMultiplier -----------
    sc_uint<16> a16_val = std::rand() % 8192;
    sc_uint<16> b16_val = std::rand() % 8192;

    A16.write(a16_val);
    B16.write(b16_val);
    start16.write(true);
    sc_start(10, SC_NS);
    start16.write(false);

    while (!ready16.read()) {
        sc_start(10, SC_NS);
    }

    sc_uint<32> expected_approx = (a16_val * b16_val);  

    std::cout << "[ApproxMultiplier]\n"
              << "  A = " << a16_val << ", B = " << b16_val << "\n"
              << "  Output     = " << W16.read() << "\n"
              << "  Expected   = " << expected_approx << "\n";

    sc_stop();
    return 0;
}
