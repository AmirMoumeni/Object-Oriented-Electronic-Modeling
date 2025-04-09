#ifndef VCD_GENERATOR_HPP
#define VCD_GENERATOR_HPP

#include <vector>
#include <string>
#include <map>

class VCDGenerator {
public:
    VCDGenerator();
    ~VCDGenerator();

    void addWire(const std::string& wireName);
    void addValueToWire(const std::string& wireName, char value);
    void generateVCDFile(const std::string& filename);
    void generateVCDFileFromData();

private:
    std::vector<std::string> wires;
    std::map<std::string, char> values;
};

#endif
