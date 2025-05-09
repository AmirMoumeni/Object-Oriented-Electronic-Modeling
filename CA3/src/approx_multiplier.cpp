#include "approx_multiplier.h"

ApproxMultiplier::ApproxMultiplier(sc_module_name name) : sc_module(name) {
    mult8 = new Multiplier("mult8");
    mult8->A(a_part);
    mult8->B(b_part);
    mult8->W(result);
    mult8->startB(start8);
    mult8->readyB(ready8);
    mult8->clk(clk);
    mult8->rst(rst);

    SC_METHOD(approx_process);
    sensitive << clk.pos();
}

void ApproxMultiplier::approx_process() {
    if (rst.read()) {
        W.write(0);
        ready.write(false);
        start8.write(false);
    } else if (start.read()) {
        auto extract_8bits = [](sc_uint<16> val) -> sc_uint<8> {
            for (int i = 15; i >= 7; --i) {
                if (val[i]) {
                    return val.range(i - 1, i - 8);
                }
            }
            return val.range(7, 0);
        };

        a_part.write(extract_8bits(A.read()));
        b_part.write(extract_8bits(B.read()));

        start8.write(true);
    } else {
        start8.write(false);
        if (ready8.read()) {
            W.write(result.read());
            ready.write(true);
        } else {
            ready.write(false);
        }
    }
}
