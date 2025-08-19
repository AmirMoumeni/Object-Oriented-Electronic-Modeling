#include "systemc.h"
#include <fstream>
#include <iostream>
#include <array>
#include <sstream>
#include <vector>
#include <map>

SC_MODULE(CoreUnit) {

    sc_in<sc_logic> clk_trigger;
    sc_out<sc_logic> is_ready;

    std::array<sc_lv<8>, 24> weight_bank;
    std::array<sc_lv<18>, 8> result_vec;
    std::array<int, 3> input_set;

    void evl() {
        is_ready.write(sc_logic_1);
        while (true) {
            wait();
            if (clk_trigger.read() == sc_logic_1) {

                is_ready.write(sc_logic_0);
                wait(SC_ZERO_TIME);

                std::ifstream fin("data.txt");
                for (int &v : input_set) fin >> v;
                fin.close();

                std::ifstream win("weights.txt");
                int temp;
                for (auto &w : weight_bank) {
                    win >> temp;
                    w = sc_lv<8>(temp);
                }
                win.close();

                for (int neuron = 0; neuron < 8; ++neuron) {
                    sc_int<18> accumulator = 0;
                    for (int feat = 0; feat < 3; ++feat) {
                        int w_idx = feat * 8 + neuron;
                        accumulator += input_set[feat] * weight_bank[w_idx].to_int();
                    }
                    result_vec[neuron] = accumulator;
                }


                std::ofstream fout("MLP_out.txt");
                for (const auto &val : result_vec)
                    fout << val.to_int() << "\n";
                fout.close();

                is_ready.write(sc_logic_1);
            }
        }
    }

    SC_CTOR(CoreUnit) {
        SC_THREAD(evl);
        sensitive << clk_trigger.pos();
    }
};
