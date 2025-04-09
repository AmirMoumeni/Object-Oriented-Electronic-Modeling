#include "io.hpp"
#include "vcd_generator.hpp"

void Interface::readVerilogNetlist() {
    ifstream file("files/circuit.txt");

    string line;
    vector<string> input_wires;
    vector<string> output_wires;

    if (getline(file, line)) {
        istringstream ss(line);
        string word;
        bool is_input = false, is_output = false;

        while (ss >> word) {
            if (word == "input") {
                is_input = true;
                is_output = false;
                continue;
            } else if (word == "output") {
                is_input = false;
                is_output = true;
                continue;
            }

            stringstream wire_stream(word);
            string wire;
            while (getline(wire_stream, wire, ',')) {
                if (is_input)
                    input_wires.push_back(wire);
                else if (is_output)
                    output_wires.push_back(wire);
            }
        }
    }

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

        circuit.addGate(gate_name, delay, wires);
    }

    circuit.setIOWires(input_wires, output_wires);

    // circuit.checkConnections();
    file.close();
}


void Interface::readTestBench() {
    ifstream file("files/testbench.txt");

    vector<Event> testbench;
    string line;

    while (getline(file, line)) {
        if (line.empty() || line[0] != '#') continue; 

        istringstream ss(line.substr(1)); 
        Event event;
        ss >> event.time_delay;

        int temp;
        while (ss >> temp) {
            event.transitions.push_back(temp + '0'); 
        }

        testbench.push_back(event);
    }

    file.close();
    circuit.addTestBench(testbench);
    // circuit.showTestBench();
}



void Interface::showOutput() {
    vector<Wire*> output_wires = circuit.getWires();
    cout << "Wires Value end of Simulation : " << endl;
    for (Wire* wire : output_wires) {
        cout << wire->getName() << " : " << wire->value() << endl;
    }
}

void Interface::run() {
    VCDGenerator VCD;

    readVerilogNetlist();
    readTestBench();
    circuit.simulate();
    showOutput();
    VCD.generateVCDFileFromData();

}
