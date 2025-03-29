#include "io.hpp"

void Interface::readVerilogNetlist() {
    ifstream file("files/circuit.txt");

    string line;
    while (getline(file, line)) {
        istringstream ss(line);
        string gate_name;
        int delay;
        vector<string> wires;

        ss >> gate_name >> delay;
        string wire;
        while (ss >> wire) {
            wires.push_back(wire);
        }
        cout << gate_name << " " << delay << endl;
        circuit.addGate(gate_name, delay, wires);
    }

    file.close();
}

void Interface::readTestBench() {
    ifstream file("files/testbench.txt");

    vector<Event> testbench;
    string line;
    while (getline(file, line)) {
        istringstream ss(line);
        Event event;
        ss >> event.time_delay;

        char value;
        while (ss >> value) {
            event.events.push_back(value);
        }

        testbench.push_back(event);
    }

    file.close();
    circuit.addTestBench(testbench);
}

void Interface::showOutput() {
    vector<Wire*> output_wires = circuit.getWires();
    for (Wire* wire : output_wires) {
        cout << wire->getName() << " : " << wire->value() << endl;
    }
}

void Interface::run() {
    readVerilogNetlist();
    readTestBench();
    circuit.simulate();
    showOutput();
}
