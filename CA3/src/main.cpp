#include <systemc.h>
#include <cstdlib>   
#include <ctime>    
#include "multiplier.h"

int sc_main(int argc, char* argv[]) {
    sc_signal<sc_uint<8>> A, B;
    sc_signal<sc_uint<16>> W;
    sc_signal<bool> startB, readyB;
    sc_clock clk("clk", 10, SC_NS);
    sc_signal<bool> rst;

    Multiplier multiplier("multiplier");
    multiplier.A(A);
    multiplier.B(B);
    multiplier.W(W);
    multiplier.startB(startB);
    multiplier.readyB(readyB);
    multiplier.clk(clk);
    multiplier.rst(rst);

    srand(time(NULL)); 

    sc_start(0, SC_NS);
    rst.write(true);
    startB.write(false);
    sc_start(20, SC_NS);

    rst.write(false);

    for (int i = 0; i < 5; ++i) {
        sc_uint<8> a = rand() % 256;
        sc_uint<8> b = rand() % 256;

        A.write(a);
        B.write(b);
        startB.write(true);
        sc_start(10, SC_NS);
        startB.write(false);

        while (!readyB.read()) {
            sc_start(10, SC_NS);
        }

        sc_uint<16> result = W.read();
        cout << "Test " << i + 1 << ": "
             << "A = " << a << ", B = " << b
             << ", Result = " << result
             << " (Expected: " << (a * b) << ")" << endl;
    }

    return 0;
}
