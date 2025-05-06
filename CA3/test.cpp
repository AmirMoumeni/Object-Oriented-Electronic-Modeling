#include <systemc.h>

constexpr unsigned int W8 = 8;
constexpr unsigned int W16 = 16;
constexpr unsigned int SLICE = W8;
constexpr unsigned int CNT_MAX = W8;
constexpr unsigned int CNT_WIDTH = 4;
constexpr int CLK_PERIOD = 10;      // ns
constexpr int RESET_CYCLES = 1;
constexpr int TEST_CYCLES8 = CNT_MAX + 2;
constexpr int TEST_CYCLES16 = TEST_CYCLES8 * 2;

SC_MODULE(Mult8) {
    sc_in<bool> clk, rst, start;
    sc_in<sc_uint<W8>> A, B;
    sc_out<bool> ready;
    sc_out<sc_uint<W16>> P;

    sc_signal<sc_uint<W8>> a_reg, b_reg;
    sc_signal<sc_uint<W16>> p_reg;
    sc_signal<sc_uint<CNT_WIDTH>> cnt;
    sc_signal<bool> busy;

    void run() {
        if (rst) {
            busy = false;
            ready.write(false);
        } else if (!busy) {
            ready.write(false);
            if (start) {
                a_reg = A;
                b_reg = B;
                p_reg = 0;
                cnt = 0;
                busy = true;
            }
        } else {
            if (b_reg.read()[0]) p_reg = p_reg.read() + a_reg.read();
            a_reg = a_reg.read() << 1;
            b_reg = b_reg.read() >> 1;
            cnt = cnt.read() + 1;
            if (cnt.read() == CNT_MAX) {
                P.write(p_reg);
                ready.write(true);
                busy = false;
            }
        }
    }

    SC_CTOR(Mult8) {
        SC_METHOD(run);
        sensitive << clk.pos();
        dont_initialize();
    }
};

SC_MODULE(VAM16) {
    sc_in<bool> clk, rst, start;
    sc_in<sc_uint<W16>> A, B;
    sc_out<bool> ready;
    sc_out<sc_uint<W16>> P;

    sc_signal<sc_uint<W8>> a_s, b_s;
    sc_signal<unsigned int> ia, ib;
    sc_signal<bool> start8, ready8;
    sc_signal<sc_uint<W16>> p8;

    Mult8* core;

    void extract() {
        if (rst) {
            start8 = false;
            ready.write(false);
        } else if (start) {
            sc_uint<W16> a = A.read(), b = B.read();
            int pa = -1, pb = -1;
            for (unsigned int i = W16; i-- > 0;) if (a[i]) { pa = i; break; }
            for (unsigned int i = W16; i-- > 0;) if (b[i]) { pb = i; break; }
            ia = (pa >= SLICE - 1 ? pa - (SLICE - 1) : SLICE);
            ib = (pb >= SLICE - 1 ? pb - (SLICE - 1) : SLICE);
            a_s = a >> (pa >= SLICE - 1 ? pa - (SLICE - 1) : SLICE);
            b_s = b >> (pb >= SLICE - 1 ? pb - (SLICE - 1) : SLICE);
            start8 = true;
            ready.write(false);
        } else {
            start8 = false;
        }
    }

    void collect() {
        if (rst) {
            ready.write(false);
        } else if (ready8) {
            unsigned int shift = ia.read() + ib.read();
            P.write(p8.read() << shift);
            ready.write(true);
        } else {
            ready.write(false);
        }
    }

    SC_CTOR(VAM16) {
        core = new Mult8("core8");
        core->clk(clk);
        core->rst(rst);
        core->start(start8);
        core->A(a_s);
        core->B(b_s);
        core->ready(ready8);
        core->P(p8);

        SC_METHOD(extract);
        sensitive << clk.pos();
        dont_initialize();

        SC_METHOD(collect);
        sensitive << clk.pos();
        dont_initialize();
    }
};

int sc_main(int, char*[]) {
    sc_clock clk("clk", CLK_PERIOD, SC_NS);
    sc_signal<bool> rst;

    sc_signal<bool> s8, r8;
    sc_signal<sc_uint<W8>> a8, b8;
    sc_signal<sc_uint<W16>> p8;

    sc_signal<bool> s16, r16;
    sc_signal<sc_uint<W16>> a16, b16, p16;

    Mult8 m8("m8");
    m8.clk(clk);
    m8.rst(rst);
    m8.start(s8);
    m8.A(a8);
    m8.B(b8);
    m8.ready(r8);
    m8.P(p8);

    VAM16 m16("m16");
    m16.clk(clk);
    m16.rst(rst);
    m16.start(s16);
    m16.A(a16);
    m16.B(b16);
    m16.ready(r16);
    m16.P(p16);

    sc_trace_file* tf = sc_create_vcd_trace_file("wave");
    sc_trace(tf, clk, "clk");
    sc_trace(tf, rst, "rst");
    sc_trace(tf, a8, "a8");
    sc_trace(tf, b8, "b8");
    sc_trace(tf, p8, "p8");
    sc_trace(tf, r8, "r8");
    sc_trace(tf, a16, "a16");
    sc_trace(tf, b16, "b16");
    sc_trace(tf, p16, "p16");
    sc_trace(tf, r16, "r16");

    rst = true;
    sc_start(RESET_CYCLES * CLK_PERIOD, SC_NS);
    rst = false;

    a8 = 13;
    b8 = 7;
    s8 = true;
    sc_start(TEST_CYCLES8 * CLK_PERIOD, SC_NS);
    s8 = false;

    a16 = 0x1234;
    b16 = 0x00AB;
    s16 = true;
    sc_start(TEST_CYCLES16 * CLK_PERIOD, SC_NS);
    s16 = false;

    sc_close_vcd_trace_file(tf);
    return 0;
}
