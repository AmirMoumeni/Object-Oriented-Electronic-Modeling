#ifndef APPROX_MUL_BFM_HPP
#define APPROX_MUL_BFM_HPP

#include <systemc.h>

static constexpr unsigned int DATA_WIDTH    = 16;
static constexpr unsigned int BASE_LATENCY  = 8;
static constexpr unsigned int INPUT_REG_LAT = 1;
static constexpr unsigned int OUTPUT_REG_LAT= 1;
static constexpr unsigned int FIXED_OVERHEAD= 2;

SC_MODULE(ApproxMulBFM) {
    sc_in_clk clk;
    sc_in<bool> reset_n;
    sc_in<sc_logic> start_sig;
    sc_out<sc_logic> ready_sig;
    sc_in<sc_lv<DATA_WIDTH>> in_A;
    sc_in<sc_lv<DATA_WIDTH>> in_B;
    sc_out<sc_lv<DATA_WIDTH>> out_mul;

    SC_CTOR(ApproxMulBFM) {
        SC_CTHREAD(run, clk.pos());
        reset_signal_is(reset_n, false);
    }

    void run();

private:
    unsigned int count_leading_zeros(unsigned int value) const;
};

#endif