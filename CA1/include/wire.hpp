#ifndef WIRE_INCLUDE
#define WIRE_INCLUDE

#include "global.hpp"

class gates;

class Wire
{
private:
    char val;
    string name;

public:
    Wire(string assign_name = "") {name = assign_name;};
    char value() {return val;}
    void setValue(char v) {val = v;}
    string getName() {return name;}
    bool stage;
    vector<gates*> connectedGates;

    void attachGate(gates* g) {
        connectedGates.push_back(g);
    }

    const vector<gates*>& getConnectedGates() const {
        return connectedGates;
    }

};

#endif