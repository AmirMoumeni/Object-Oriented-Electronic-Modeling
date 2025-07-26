#include "systemc.h"
#include "mlp.hpp"

SC_MODULE(CoreUnit_TB) {
    sc_clock clk;
    sc_signal<sc_logic> signal_exec;
    sc_signal<sc_logic> signal_ready;

    CoreUnit* uut;

    SC_CTOR(CoreUnit_TB) : clk("clk", 10, SC_NS) {
        uut = new CoreUnit("NeuralBlock");
        uut->clk_trigger(signal_exec);
        uut->is_ready(signal_ready);

        SC_THREAD(run_test);
        sensitive << clk.posedge_event();
    }

    

    void run_test() {
        signal_exec.write(sc_logic_1);
        wait(SC_ZERO_TIME);
        signal_exec.write(sc_logic_0);
        wait(100, SC_NS);
        sc_stop();
    }
};

int sc_main(int argc, char* argv[]) {
    sc_set_time_resolution(1, SC_NS);

    CoreUnit_TB testbench("testbench");

    sc_trace_file* tf = sc_create_vcd_trace_file("CoreUnit_Waves");
    sc_trace(tf, testbench.clk, "clk");
    sc_trace(tf, testbench.signal_exec, "exec_signal");
    sc_trace(tf, testbench.signal_ready, "ready_signal");

    sc_start();
    sc_close_vcd_trace_file(tf);
    return 0;
}
