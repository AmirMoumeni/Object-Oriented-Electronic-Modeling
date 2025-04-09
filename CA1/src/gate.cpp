#include "gate.hpp"
#include "wire.hpp"

#include "simulation_context.hpp"

void Or::evl() {
    char a = i1->value();
    char b = i2->value();
    char result;

    if (a == ONE || b == ONE) result = ONE;
    else if (a == UNKNOWN || b == UNKNOWN) result = UNKNOWN;
    else result = ZERO;

    if (result != o1->value()) {
        SimEvent e = {
            SimulationContext::current_time + gateDelay,
            o1,
            result
        };
        SimulationContext::eventQueue.push(e);
    }
}



void And::evl() {
    char a = i1->value();
    char b = i2->value();
    char result;

    if (a == ZERO || b == ZERO) result = ZERO;
    else if (a == UNKNOWN || b == UNKNOWN) result = UNKNOWN;
    else result = ONE;

    if (result != o1->value()) {
        SimEvent e = {
            SimulationContext::current_time + gateDelay,
            o1,
            result
        };
        SimulationContext::eventQueue.push(e);
    }
}



void Not::evl() {
    char a = i1->value();
    char result;

    if (a == UNKNOWN) result = UNKNOWN;
    else result = (a == ONE) ? ZERO : ONE;

    if (result != o1->value()) {
        SimEvent e = {
            SimulationContext::current_time + gateDelay,
            o1,
            result
        };
        SimulationContext::eventQueue.push(e);
    }
}


void Nand::evl() {
    char a = i1->value();
    char b = i2->value();
    char result;

    if (a == ZERO || b == ZERO) result = ONE;
    else if (a == UNKNOWN || b == UNKNOWN) result = UNKNOWN;
    else result = ZERO;

    if (result != o1->value()) {
        SimEvent e = {
            SimulationContext::current_time + gateDelay,
            o1,
            result
        };
        SimulationContext::eventQueue.push(e);
    }
}


void Nor::evl() {
    char a = i1->value();
    char b = i2->value();
    char result;

    if (a == ZERO && b == ZERO) result = ONE;
    else if (a == UNKNOWN || b == UNKNOWN) result = UNKNOWN;
    else result = ZERO;

    if (result != o1->value()) {
        SimEvent e = {
            SimulationContext::current_time + gateDelay,
            o1,
            result
        };
        SimulationContext::eventQueue.push(e);
    }
}


void Xor::evl() {
    char a = i1->value();
    char b = i2->value();
    char result;

    if (a == UNKNOWN || b == UNKNOWN) result = UNKNOWN;
    else result = (a != b) ? ONE : ZERO;

    if (result != o1->value()) {
        SimEvent e = {
            SimulationContext::current_time + gateDelay,
            o1,
            result
        };
        SimulationContext::eventQueue.push(e);
    }
}

void Or_3::evl() {
    char a = i1->value();
    char b = i2->value();
    char c = i3->value();
    char result;

    if (a == ONE || b == ONE || c == ONE) result = ONE;
    else if (a == UNKNOWN || b == UNKNOWN || c == UNKNOWN) result = UNKNOWN;
    else result = ZERO;

    if (result != o1->value()) {
        SimEvent e = {
            SimulationContext::current_time + gateDelay,
            o1,
            result
        };
        SimulationContext::eventQueue.push(e);
    }
}


void Or_4::evl() {
    char a = i1->value();
    char b = i2->value();
    char c = i3->value();
    char d = i4->value();
    char result;

    if (a == ONE || b == ONE || c == ONE || d == ONE) result = ONE;
    else if (a == UNKNOWN || b == UNKNOWN || c == UNKNOWN || d == UNKNOWN) result = UNKNOWN;
    else result = ZERO;

    if (result != o1->value()) {
        SimEvent e = {
            SimulationContext::current_time + gateDelay,
            o1,
            result
        };
        SimulationContext::eventQueue.push(e);
    }
}


void And_3::evl() {
    char a = i1->value();
    char b = i2->value();
    char c = i3->value();
    char result;

    if (a == ZERO || b == ZERO || c == ZERO) result = ZERO;
    else if (a == UNKNOWN || b == UNKNOWN || c == UNKNOWN) result = UNKNOWN;
    else result = ONE;

    if (result != o1->value()) {
        SimEvent e = {
            SimulationContext::current_time + gateDelay,
            o1,
            result
        };
        SimulationContext::eventQueue.push(e);
    }
}


void gates::introduce(){
    cout << name <<"- out: " <<o1->getName() <<" in: " << i1->getName() << " " << i2->getName() << endl;
}

void gates::printValues(){
    cout << name << " : " << i1->getName() << "." << i1->value() 
         << i2->getName() << "." << i2->value()
         << o1->getName() << "." << o1->value() << endl;
}

