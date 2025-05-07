#include <systemc.h>

constexpr unsigned int W8 = 8;
constexpr unsigned int W16 = 16;
constexpr unsigned int SLICE = W8;
constexpr unsigned int CNT_MAX = W8;
constexpr unsigned int CNT_WIDTH = 4;

SC_MODULE(Mult8Datapath) {
    sc_in<bool> clk;
    sc_in<sc_uint<W8>> A, B;
    sc_in<bool> load, add, shift;
    sc_out<sc_uint<W16>> P_out;
    sc_out<sc_uint<CNT_WIDTH>> cnt_out;

    sc_signal<sc_uint<W8>> a_reg;
    sc_signal<sc_uint<W8>> b_reg;
    sc_signal<sc_uint<W16>> p_reg;
    sc_signal<sc_uint<CNT_WIDTH>> cnt;

    void process() {
        if (load) {
            a_reg = A.read();
            b_reg = B.read();
            p_reg = 0;
            cnt = 0;
        } else {
            if (add && b_reg.read()[0]) p_reg = p_reg.read() + a_reg.read();
            if (shift) {
                a_reg = a_reg.read() << 1;
                b_reg = b_reg.read() >> 1;
                cnt = cnt.read() + 1;
            }
        }
        P_out.write(p_reg);
        cnt_out.write(cnt);
    }

    SC_CTOR(Mult8Datapath) {
        SC_METHOD(process);
        sensitive << clk.pos();
        dont_initialize();
    }
};

SC_MODULE(Mult8Ctrl) {
    sc_in<bool> clk, rst, start;
    sc_in<sc_uint<CNT_WIDTH>> cnt;
    sc_out<bool> load, add, shift, ready;

    enum State {IDLE, LOAD, CALC, DONE};
    sc_signal<State> state, next_state;

    void fsm() {
        load.write(false);
        add.write(false);
        shift.write(false);
        ready.write(false);

        switch(state.read()) {
            case IDLE:
                next_state = start.read() ? LOAD : IDLE;
                break;
            case LOAD:
                load.write(true);
                next_state = CALC;
                break;
            case CALC:
                add.write(true);
                shift.write(true);
                next_state = (cnt.read() == CNT_MAX) ? DONE : CALC;
                break;
            case DONE:
                ready.write(true);
                next_state = IDLE;
                break;
        }
    }

    void reg_state() {
        state = rst.read() ? IDLE : next_state;
    }

    SC_CTOR(Mult8Ctrl) {
        SC_METHOD(fsm);
        sensitive << state << start << cnt;
        SC_METHOD(reg_state);
        sensitive << clk.pos();
        dont_initialize();
    }
};

SC_MODULE(VAM16Datapath) {
    sc_in<bool> clk, start, rst;
    sc_in<sc_uint<W16>> A, B;
    sc_out<bool> start8;
    sc_out<sc_uint<W8>> a_s, b_s;
    sc_out<unsigned int> ia, ib;
    sc_in<bool> ready8;
    sc_in<sc_uint<W16>> p8;
    sc_out<bool> ready;
    sc_out<sc_uint<W16>> P_out;

    sc_signal<int> pa, pb;

    void extract() {
        if (rst.read()) {
            start8.write(false);
            ready.write(false);
        } else if (start.read()) {
            sc_uint<W16> a = A.read(), b = B.read();
            pa = pb = -1;
            for (int i = W16-1; i >= 0; --i) {
                if (pa < 0 && a[i]) pa = i;
                if (pb < 0 && b[i]) pb = i;
            }
            ia.write((pa >= SLICE-1) ? pa-(SLICE-1) : SLICE);
            ib.write((pb >= SLICE-1) ? pb-(SLICE-1) : SLICE);
            a_s.write(a >> ((pa >= SLICE-1) ? pa-(SLICE-1) : SLICE));
            b_s.write(b >> ((pb >= SLICE-1) ? pb-(SLICE-1) : SLICE));
            start8.write(true);
            ready.write(false);
        } else {
            start8.write(false);
        }
    }

    void collect() {
        if (rst.read()) {
            ready.write(false);
        } else if (ready8.read()) {
            unsigned int shift = ia.read() + ib.read();
            P_out.write(p8.read() << shift);
            ready.write(true);
        } else {
            ready.write(false);
        }
    }

    SC_CTOR(VAM16Datapath) {
        SC_METHOD(extract);
        sensitive << clk.pos();
        dont_initialize();
        SC_METHOD(collect);
        sensitive << clk.pos();
        dont_initialize();
    }
};

SC_MODULE(VAM16Ctrl) {
    sc_in<bool> clk, rst, start;
    sc_in<bool> ready8;
    sc_out<bool> start8, ready;

    enum State {IDLE, EXTRACT, WAIT8, COLLECT, DONE};
    sc_signal<State> state, next_state;

    void fsm() {
        start8.write(false);
        ready.write(false);

        switch(state.read()) {
            case IDLE:
                next_state = start.read() ? EXTRACT : IDLE;
                break;
            case EXTRACT:
                start8.write(true);
                next_state = WAIT8;
                break;
            case WAIT8:
                next_state = ready8.read() ? COLLECT : WAIT8;
                break;
            case COLLECT:
                ready.write(true);
                next_state = DONE;
                break;
            case DONE:
                next_state = IDLE;
                break;
        }
    }

    void reg_state() {
        state = rst.read() ? IDLE : next_state;
    }

    SC_CTOR(VAM16Ctrl) {
        SC_METHOD(fsm);
        sensitive << state << start << ready8;
        SC_METHOD(reg_state);
        sensitive << clk.pos();
        dont_initialize();
    }
};

SC_MODULE(Mult8) {
    sc_in<bool> clk, rst, start;
    sc_in<sc_uint<W8>> A, B;
    sc_out<bool> ready;
    sc_out<sc_uint<W16>> P;

    sc_signal<bool> load_sig, add_sig, shift_sig;
    sc_signal<sc_uint<CNT_WIDTH>> cnt_sig;

    Mult8Datapath dp;
    Mult8Ctrl ctrl;

    SC_CTOR(Mult8): dp("dp"), ctrl("ctrl") {
        dp.clk(clk);
        dp.A(A);
        dp.B(B);
        dp.load(load_sig);
        dp.add(add_sig);
        dp.shift(shift_sig);
        dp.P_out(P);
        dp.cnt_out(cnt_sig);

        ctrl.clk(clk);
        ctrl.rst(rst);
        ctrl.start(start);
        ctrl.cnt(cnt_sig);
        ctrl.load(load_sig);
        ctrl.add(add_sig);
        ctrl.shift(shift_sig);
        ctrl.ready(ready);
    }
};

SC_MODULE(VAM16) {
    sc_in<bool> clk, rst, start;
    sc_in<sc_uint<W16>> A, B;
    sc_out<bool> ready;
    sc_out<sc_uint<W16>> P;

    sc_signal<bool> start8_sig, ready8_sig;
    sc_signal<sc_uint<W8>> a_s;
    sc_signal<sc_uint<W8>> b_s;
    sc_signal<unsigned int> ia_sig, ib_sig;
    sc_signal<sc_uint<W16>> p8_sig;

    Mult8 core8;
    VAM16Datapath dp;
    VAM16Ctrl ctrl;

    SC_CTOR(VAM16)
    : core8("core8"), dp("dp16"), ctrl("ctrl16") {
        core8.clk(clk);
        core8.rst(rst);
        core8.start(start8_sig);
        core8.A(a_s);
        core8.B(b_s);
        core8.ready(ready8_sig);
        core8.P(p8_sig);

        dp.clk(clk);
        dp.rst(rst);
        dp.start(start);
        dp.A(A);
        dp.B(B);
        dp.start8(start8_sig);
        dp.a_s(a_s);
        dp.b_s(b_s);
        dp.ia(ia_sig);
        dp.ib(ib_sig);
        dp.ready8(ready8_sig);
        dp.p8(p8_sig);
        dp.ready(ready);
        dp.P_out(P);

        ctrl.clk(clk);
        ctrl.rst(rst);
        ctrl.start(start);
        ctrl.ready8(ready8_sig);
        ctrl.start8(start8_sig);
        ctrl.ready(ready);
    }
};
