#include "circuit.hpp"
#include <algorithm>

vector<Wire*> Circuit::changeStringToWire(vector<string> string_wires) {
    vector<Wire*> gate_wires;
    for (string wire_name : string_wires) {
        if (wires_map.find(wire_name) == wires_map.end()) {
            wires_map[wire_name] = new Wire(wire_name);
            wires.push_back(wires_map[wire_name]);
        }
        gate_wires.push_back(wires_map[wire_name]);
    }
    return gate_wires;
}

void Circuit::addGate(string gate_name, int delay, vector<string> string_wires) {
    vector<Wire*> gate_wires = changeStringToWire(string_wires);

    if (gate_name == "AND") {
        input_gates.push_back(And(*gate_wires[0], *gate_wires[1], *gate_wires[2], delay));
    }
    else if (gate_name == "OR") {
        input_gates.push_back(Or(*gate_wires[0], *gate_wires[1], *gate_wires[2], delay));
    }
    else if (gate_name == "XOR") {
        input_gates.push_back(Xor(*gate_wires[0], *gate_wires[1], *gate_wires[2], delay));
    }
    else if (gate_name == "NOR") {
        input_gates.push_back(Nor(*gate_wires[0], *gate_wires[1], *gate_wires[2], delay));
    }
    else if (gate_name == "NAND") {
        input_gates.push_back(Nand(*gate_wires[0], *gate_wires[1], *gate_wires[2], delay));
    }
    for (auto gate : input_gates){
        gate.showWires();
    }
}

void Circuit::simulate() {

    for (const Event& event : events) {
        for (size_t i = 0; i < event.events.size(); i++) {
            if (i < wires.size()) {
                wires[i]->setValue(event.events[i]);
            }
        }

        bool changed;
        do {
            changed = false;
            for (auto& gate : input_gates) {
                char prev_output = gate.out();
                
                gate.evl();
                
                if (gate.out() != prev_output) {
                    changed = true;
                }
            }
        } while (changed);

        cout << "Time: " << event.time_delay << endl;
        for (Wire* wire : wires) {
            cout << wire->getName() << ": " << wire->value() << endl;
        }
        cout << "-------------------" << endl;
    }
}

