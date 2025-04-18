#include "circuit.hpp"
#include "simulation_context.hpp"

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
        input_gates.push_back(new And(*gate_wires[1], *gate_wires[2], *gate_wires[0], delay));
    }
    else if (gate_name == "OR") {
        input_gates.push_back(new Or(*gate_wires[1], *gate_wires[2], *gate_wires[0], delay));
    }
    else if (gate_name == "XOR") {
        input_gates.push_back(new Xor(*gate_wires[1], *gate_wires[2], *gate_wires[0], delay));
    }
    else if (gate_name == "NOR") {
        input_gates.push_back(new Nor(*gate_wires[1], *gate_wires[2], *gate_wires[0], delay));
    }
    else if (gate_name == "NAND") {
        input_gates.push_back(new Nand(*gate_wires[1], *gate_wires[2], *gate_wires[0], delay));
    }
    
}

void Circuit::simulate() {
    for (const Event& event : testBench) {
        SimulationContext::current_time += event.time_delay;

        for (size_t i = 0; i < event.transitions.size(); i++) {
            if (i < input_wires.size()) {
                Wire* w = input_wires[i];
                SimEvent e = {SimulationContext::current_time, w, event.transitions[i]};
                SimulationContext::eventQueue.push(e);
            }
        }

        while (!SimulationContext::eventQueue.empty()) {
            SimEvent e = SimulationContext::eventQueue.top();
            SimulationContext::eventQueue.pop();

            SimulationContext::current_time = e.time;
            e.target->setValue(e.newValue);

            for (gates* g : e.target->getConnectedGates()) {
                g->evl();  
            }
        }

        cout << "Time: " << SimulationContext::current_time << endl;
        for (Wire* wire : input_wires) {
            cout << wire->getName() << ":" << wire->value() << endl;
        }
        for (Wire* wire : output_wires) {
            cout << wire->getName() << ":" << wire->value() << endl;
        }
        cout << "-------------------" << endl;
    }
}




void Circuit::checkConnections() {
    cout << "\n=== Circuit Connections ===" << endl;

    cout << "\nInputs:" << endl;
    for (auto wire : input_wires) {
        cout << "  " << wire->getName() << endl;
    }

    cout << "\nOutputs:" << endl;
    for (auto wire : output_wires) {
        cout << "  " << wire->getName() << endl;
    }

    cout << "\nGates:" << endl;
    for (auto& gate : input_gates) {
        gate->introduce();
    }
}


void Circuit::showTestBench(){
    cout << endl << "TestBench :" << endl;
    for (size_t i =0 ; i< testBench.size() ; i++){
        if (i != 0) cout << endl;
        for(size_t j =0 ; j <testBench[i].transitions.size() ; j++){
            cout << testBench[i].transitions[j] << " ";
        }
    }
    cout << endl;
}

void Circuit::setIOWires(vector<string> inputs, vector<string> outputs) {
    input_wires = changeStringToWire(inputs);
    output_wires = changeStringToWire(outputs);
}
