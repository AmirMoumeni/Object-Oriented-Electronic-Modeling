#include <systemc.h>
#include <fstream>
#include <array>
#include <string>

SC_MODULE(MatrixKernel) {

    sc_event start_mma, intr_mma;

    std::array<sc_lv<8>, 3> vec_input;
    std::array<sc_lv<8>, 6> mat_weights;
    std::array<sc_lv<18>, 2> vec_output;

    void evl() {
        while (true) {
            wait(start_mma);

            std::ifstream vec_file("vec_mma2.txt");
            for (auto& val : vec_input) {
                int x; vec_file >> x; val = x;
            }
            vec_file.close();

            std::ifstream mat_file("mat_mma2.txt");
            for (auto& val : mat_weights) {
                int x; mat_file >> x; val = x;
            }
            mat_file.close();

            for (int col = 0; col < 2; ++col) {
                sc_int<18> acc = 0;
                for (int row = 0; row < 3; ++row) {
                    int index = row * 2 + col;
                    acc += vec_input[row].to_int() * mat_weights[index].to_int();
                }
                vec_output[col] = acc;
            }

            std::ofstream fout("out_mma2.txt");
            for (const auto& val : vec_output) {
                fout << val.to_int() << "\n";
            }
            fout.close();

            intr_mma.notify();
        }
    }

    SC_CTOR(MatrixKernel) {
        SC_THREAD(evl);
    }
};
