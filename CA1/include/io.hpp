#ifndef IO_INCLUDE
#define IO_INCLUDE

#include "global.hpp"
#include "gate.hpp"
#include "circuit.hpp"

class Interface {
public:
    Interface() {};
    ~Interface() {};
    void run();
    void showOutput();
private:
    Circuit circuit;
    vector<gates> input_gates;
    void readVerilogNetlist();
    void readTestBench();
};

#endif