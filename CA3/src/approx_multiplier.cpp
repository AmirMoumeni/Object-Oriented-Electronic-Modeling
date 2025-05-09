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
        auto extract_high8 = [](sc_uint<16> val) -> sc_uint<8> {
            for (int i = 15; i >= 7; --i) {
                if (val[i]) {
                    return val.range(i, i - 7);
                }
            }
            return val.range(7, 0);
        };

        a_part.write(extract_high8(A.read()));
        b_part.write(extract_high8(B.read()));

        start8.write(true);
    } else {
        start8.write(false);
        if (ready8.read()) {
            // گرفتن خروجی اولیه از ضرب کننده 8 بیتی
            sc_uint<16> base_result = result.read();
            sc_uint<32> shifted_result = ((sc_uint<32>)base_result) << 16; // انتقال به موقعیت بالا برای جستجوی بیت مهم

            // یافتن اولین بیت '1' از چپ و استخراج 16 بیت از آنجا
            sc_uint<16> final_result = 0;
            for (int i = 31; i >= 15; --i) {
                if (shifted_result[i]) {
                    final_result = shifted_result.range(i, i - 15);
                    break;
                }
            }

            W.write(final_result);
            ready.write(true);
        } else {
            ready.write(false);
        }
    }
}
