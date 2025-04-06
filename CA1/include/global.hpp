// in God we trust

#ifndef GLOBAL_INCLUDE
#define GLOBAL_INCLUDE

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

const char ONE = '1';
const char ZERO = '0';
const char HIGH_IMPEDANCE = 'Z';
const char UNKNOWN = 'X';

struct Event {
    int time_delay;
    vector<char> transitions;
};


#endif


