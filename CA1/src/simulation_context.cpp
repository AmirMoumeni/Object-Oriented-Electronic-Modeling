#include "simulation_context.hpp"

int SimulationContext::current_time = 0;
std::priority_queue<SimEvent> SimulationContext::eventQueue;
