#ifndef SIMULATION_CONTEXT_INCLUDE
#define SIMULATION_CONTEXT_INCLUDE

#include <queue>

class Wire;
class gates;

struct SimEvent {
    int time;
    Wire* target;
    char newValue;

    bool operator<(const SimEvent& other) const {
        return time > other.time; 
    }
};

class SimulationContext {
public:
    static int current_time;
    static std::priority_queue<SimEvent> eventQueue;
};

#endif
