#include "vcd_generator.hpp"
#include <fstream>
#include <iostream>

VCDGenerator::VCDGenerator() {}

VCDGenerator::~VCDGenerator() {}

void VCDGenerator::addWire(const std::string& wireName) {
    wires.push_back(wireName);
}

void VCDGenerator::addValueToWire(const std::string& wireName, char value) {
    values[wireName] = value;
}

void VCDGenerator::generateVCDFile(const std::string& filename) {
    std::ofstream file(filename);

    if (file.is_open()) {
        file << "$date today $end" << std::endl;
        file << "$timescale 1 ns $end" << std::endl;

        for (const auto& wire : wires) {
            file << "$var wire 1 " << wire << " $end" << std::endl;
        }

        file << "#0" << std::endl;

        for (const auto& wire : wires) {
            char value = values.count(wire) ? values.at(wire) : 'Z';
            file << value << " " << wire << std::endl;
        }

        file.close();
        std::cout << "VCD file has been generated: " << filename << std::endl;
    } else {
        std::cerr << "Error opening file!" << std::endl;
    }
}

void VCDGenerator::generateVCDFileFromData() {
    addWire("wire1");
    addWire("wire2");

    addValueToWire("wire1", '1');
    addValueToWire("wire2", '0');

    generateVCDFile("output.vcd");
}
