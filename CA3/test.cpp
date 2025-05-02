#include <systemc.h>

int sc_main(int argc, char* argv[]) {
    sc_start(10, SC_NS);
    std::cout << "Simulation done at " << sc_time_stamp() << std::endl;
    return 0;
}
