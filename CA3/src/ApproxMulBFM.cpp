#include "ApproxMulBFM.h"
#include <iostream>

void ApproxMulBFM::run() {
    ready_sig.write(SC_LOGIC_1);
    out_mul.write(0);
    wait();

    while (true) {
        if (start_sig.read() == SC_LOGIC_1) {
            ready_sig.write(SC_LOGIC_0);
            do { wait(); } while (start_sig.read() == SC_LOGIC_1);

            unsigned int opA_val = in_A.read().to_uint();
            unsigned int opB_val = in_B.read().to_uint();

            unsigned int lz_A = count_leading_zeros(opA_val);
            unsigned int lz_B = count_leading_zeros(opB_val);

            unsigned int total_wait = lz_A + lz_B + BASE_LATENCY + INPUT_REG_LAT + OUTPUT_REG_LAT + FIXED_OVERHEAD;

            while (total_wait--) wait();

            unsigned long full_product = static_cast<unsigned long>(opA_val) * opB_val;
            sc_lv<DATA_WIDTH * 2> full_lv(full_product);
            sc_lv<DATA_WIDTH> high_half = full_lv.range(DATA_WIDTH*2 - 1, DATA_WIDTH);

            out_mul.write(high_half);
            ready_sig.write(SC_LOGIC_1);
        }
        wait();
    }
}

unsigned int ApproxMulBFM::count_leading_zeros(unsigned int value) const {
    unsigned int cnt = 0;
    for (int bit = DATA_WIDTH - 1; bit >= static_cast<int>(DATA_WIDTH/2); --bit) {
        if (((value >> bit) & 0x1) == 0) cnt++;
        else break;
    }
    return cnt;
}
