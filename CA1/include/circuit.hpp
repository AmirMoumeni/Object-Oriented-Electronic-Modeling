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
    void addTestBench(vector<Event> testbench) {events = testbench;}
    void simulate();
    vector<Wire*> getWires() {return wires;}
private:
    vector<Event> events;
    vector<Wire*> changeStringToWire(vector<string> string_wires);
    vector<gates> input_gates;
    map<string, Wire*> wires_map;
    vector<Wire*> wires;
};

#endif