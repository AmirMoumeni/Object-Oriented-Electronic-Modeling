// #include <systemc.h>
// #include "multiplier.h"
// #include "approx_multiplier.h"
// #include <cstdlib>
// #include <ctime>
// #include <iostream>

// int sc_main(int argc, char* argv[]) {
//     sc_clock clk("clk", 10, SC_NS);
//     sc_signal<bool> rst;

//     sc_signal<sc_uint<8>> A8, B8;
//     sc_signal<sc_uint<16>> W8;
//     sc_signal<bool> start8, ready8;

//     sc_signal<sc_uint<16>> A16, B16, W16;
//     sc_signal<bool> start16, ready16;

//     Multiplier multiplier("multiplier");
//     multiplier.A(A8); multiplier.B(B8); multiplier.W(W8);
//     multiplier.startB(start8); multiplier.readyB(ready8);
//     multiplier.clk(clk); multiplier.rst(rst);

//     ApproxMultiplier approx("approx_multiplier");
//     approx.A(A16); approx.B(B16); approx.W(W16);
//     approx.start(start16); approx.ready(ready16);
//     approx.clk(clk); approx.rst(rst);

//     sc_start(1, SC_NS);
//     rst.write(true);
//     sc_start(10, SC_NS);
//     rst.write(false);
//     sc_start(10, SC_NS);

//     std::srand(std::time(nullptr));

//     sc_uint<8> a_val = std::rand() % 256;
//     sc_uint<8> b_val = std::rand() % 256;

//     A8.write(a_val);
//     B8.write(b_val);
//     start8.write(true);
//     sc_start(10, SC_NS);
//     start8.write(false);

//     while (!ready8.read()) {
//         sc_start(10, SC_NS);
//     }

//     sc_uint<16> expected_exact = a_val * b_val;

//     std::cout << "[Multiplier]\n"
//               << "  A = " << a_val << ", B = " << b_val << "\n"
//               << "  Output     = " << W8.read() << "\n"
//               << "  Expected   = " << expected_exact << "\n\n";

//     sc_uint<16> a16_val = std::rand() % 8192;
//     sc_uint<16> b16_val = std::rand() % 8192;

//     A16.write(a16_val);
//     B16.write(b16_val);
//     start16.write(true);
//     sc_start(10, SC_NS);
//     start16.write(false);

//     while (!ready16.read()) {
//         sc_start(10, SC_NS);
//     }

//     sc_uint<32> expected_approx = (a16_val * b16_val);  

//     std::cout << "[ApproxMultiplier]\n"
//               << "  A = " << a16_val << ", B = " << b16_val << "\n"
//               << "  Output     = " << W16.read() << "\n"
//               << "  Expected   = " << expected_approx << "\n";

//     sc_stop();
//     return 0;
// }


#include <systemc.h>
#include "ApproxMulBFM.h"
#include <iostream>

int sc_main(int argc, char* argv[]) {

    sc_clock clk("clk", sc_time(10, SC_NS));
    sc_signal<bool> reset_n;
    sc_signal<sc_logic> start_sig;
    sc_signal<sc_logic> ready_sig;
    sc_signal<sc_lv<DATA_WIDTH>> sigA, sigB, sigR;

    ApproxMulBFM dut("ApproxMulBFM");
    dut.clk(clk);
    dut.reset_n(reset_n);
    dut.start_sig(start_sig);
    dut.ready_sig(ready_sig);
    dut.in_A(sigA);
    dut.in_B(sigB);
    dut.out_mul(sigR);

    sc_trace_file* tf = sc_create_vcd_trace_file("ApproxBFM_tb");
    sc_trace(tf, clk, "clk");
    sc_trace(tf, reset_n, "reset_n");
    sc_trace(tf, start_sig, "start");
    sc_trace(tf, sigA, "A");
    sc_trace(tf, sigB, "B");
    sc_trace(tf, ready_sig, "ready");
    sc_trace(tf, sigR, "result");

    reset_n = false;
    start_sig = SC_LOGIC_0;
    sc_start(20, SC_NS);

    reset_n = true;
    sc_start(20, SC_NS);

    sigA = 0x0F0F;
    sigB = 0x00FF;
    start_sig = SC_LOGIC_1;
    sc_start(10, SC_NS);
    start_sig = SC_LOGIC_0;

    while (ready_sig.read() != SC_LOGIC_1) sc_start(10, SC_NS);
    std::cout << "Input A=0x" << std::hex << sigA.read().to_uint()
              << " B=0x" << sigB.read().to_uint()
              << " => Result=0x" << sigR.read().to_uint() << std::endl;


    sc_close_vcd_trace_file(tf);
    return 0;
}
