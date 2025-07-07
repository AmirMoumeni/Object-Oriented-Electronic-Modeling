#include "systemc.h"
#include <fstream>
#include <array>
#include <string>

SC_MODULE(FusedMatVec) {

    sc_in<sc_logic> trigger;
    sc_out<sc_logic> status;

    static constexpr int VEC_SIZE = 3;
    static constexpr int OUT_SIZE = 2;

    std::array<sc_lv<8>, VEC_SIZE> vec_in;
    std::array<sc_lv<8>, VEC_SIZE * OUT_SIZE> mat_flat;
    std::array<sc_lv<18>, OUT_SIZE> vec_out;

    std::string vec_path = "vec_mma.txt";
    std::string mat_path = "mat_mma.txt";
    std::string out_path = "res_mma.txt";

    void evl() {
        status.write(sc_logic_1);
        while (true) {
            wait();
            if (trigger.read() == sc_logic_1) {
                status.write(sc_logic_0);
                wait(SC_ZERO_TIME);

                std::ifstream vec_file(vec_path);
                for (auto& x : vec_in) {
                    int temp;
                    vec_file >> temp;
                    x = temp;
                }
                vec_file.close();

                std::ifstream mat_file(mat_path);
                for (auto& x : mat_flat) {
                    int temp;
                    mat_file >> temp;
                    x = temp;
                }
                mat_file.close();

                for (int out_idx = 0; out_idx < OUT_SIZE; ++out_idx) {
                    sc_int<18> dot = 0;
                    for (int in_idx = 0; in_idx < VEC_SIZE; ++in_idx) {
                        int pos = in_idx * OUT_SIZE + out_idx;
                        dot += vec_in[in_idx].to_int() * mat_flat[pos].to_int();
                    }
                    vec_out[out_idx] = dot;
                }

                std::ofstream out_file(out_path);
                for (const auto& x : vec_out) {
                    out_file << x.to_int() << '\n';
                }
                out_file.close();

                status.write(sc_logic_1);
            }
        }
    }

    SC_CTOR(FusedMatVec) {
        SC_THREAD(evl);
        sensitive << trigger.pos();
    }
};
