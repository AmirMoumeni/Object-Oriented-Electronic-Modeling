#include "multiplier.h"

void Multiplier::multiply_process() {
    if (rst.read()) {
        reg_A.write(0);
        reg_B.write(0);
        result.write(0);
        readyB.write(false);
        busy.write(false);
    } else if (clk.posedge()) {
        if (startB.read() && !busy.read()) {
            reg_A.write(A.read());
            reg_B.write(B.read());
            result.write(A.read() * B.read());
            busy.write(true);
            readyB.write(false);
        } else if (busy.read()) {
            W.write(result.read());
            readyB.write(true);
            busy.write(false);
        } else {
            readyB.write(false);
        }
    }
}
