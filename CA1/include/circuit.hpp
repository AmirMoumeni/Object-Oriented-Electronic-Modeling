#ifndef CIRCUIT_INCLUDE
#define CIRCUIT_INCLUDE

#include "global.hpp"
#include "gate.hpp"
#include "wire.hpp"

class Circuit {
public:
    Circuit() {};
    ~Circuit() {};
    void addGate(string gate_name, int delay, vector<string> string_wires);
    void addTestBench(vector<Event> testbench) {testBench = testbench;}
    void simulate();
    vector<Wire*> getWires() {return wires;}
    vector<Wire*> getInputWires() { return input_wires; }
    vector<Wire*> getOutputWires() { return output_wires; }
    void setIOWires(vector<string> inputs, vector<string> outputs);
    void checkConnections();
    void showTestBench();
private:
    vector<Event> testBench;
    vector<gates*> input_gates;
    map<string, Wire*> wires_map;
    vector<Wire*> wires;
    vector<Wire*> input_wires;
    vector<Wire*> output_wires;

    vector<Wire*> changeStringToWire(vector<string> string_wires);
};
#endif
