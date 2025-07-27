#include <systemc.h>
#include <array>
#include "mlp.hpp"
#include "channels.hpp"
#include "mma2.cpp"

SC_MODULE(MLPSystemTop) {
    sc_clock clk;
    sc_signal<bool> done;

    MatrixKernel mma_kernel;
    packet_stream chan_proc_to_mma;
    packet_stream chan_mma_to_proc;

    std::array<sc_lv<8>, 3 * 8> full_weights;
    std::array<sc_lv<8>, 3> input_vector;
    std::array<sc_lv<18>, 8> final_output;

    SC_CTOR(MLPSystemTop)
        : clk("clk", 10, SC_NS),
          mma_kernel("mma_kernel"),
          chan_proc_to_mma("proc_to_mma"),
          chan_mma_to_proc("mma_to_proc")
    {
        SC_THREAD(run_processor);
        sensitive << clk.posedge_event();

        SC_THREAD(run_accelerator);
    }

    void run_processor() {
        for (int i = 0; i < 3 * 8; ++i)
            full_weights[i] = i + 1;
        for (int i = 0; i < 3; ++i)
            input_vector[i] = 1;

        for (int seg = 0; seg < 4; ++seg) {
            data_packet pkt_in;
            for (int i = 0; i < 3; ++i)
                pkt_in[i] = input_vector[i];
            for (int i = 0; i < 6; ++i)
                pkt_in[3 + i] = full_weights[seg * 6 + i];

            chan_proc_to_mma.send(pkt_in);
            mma_kernel.start_mma.notify();

            wait(mma_kernel.intr_mma);

            data_packet pkt_out;
            chan_mma_to_proc.receive(pkt_out);
            for (int i = 0; i < 2; ++i) {
                final_output[seg * 2 + i] = pkt_out[i].to_int();
            }
        }

        std::ofstream fout("mlp_final_out.txt");
        for (const auto& val : final_output) {
            fout << val.to_int() << "\n";
        }
        fout.close();

        done.write(true);
        sc_stop();
    }

    void run_accelerator() {
        while (true) {
            wait(mma_kernel.start_mma);

            data_packet pkt;
            chan_proc_to_mma.receive(pkt);

            for (int i = 0; i < 3; ++i)
                mma_kernel.vec_input[i] = pkt[i];
            for (int i = 0; i < 6; ++i)
                mma_kernel.mat_weights[i] = pkt[3 + i];

            for (int col = 0; col < 2; ++col) {
                sc_int<18> acc = 0;
                for (int row = 0; row < 3; ++row) {
                    int index = row * 2 + col;
                    acc += mma_kernel.vec_input[row].to_int() * mma_kernel.mat_weights[index].to_int();
                }
                mma_kernel.vec_output[col] = acc;
            }

            data_packet pkt_out;
            for (int i = 0; i < 2; ++i)
                pkt_out[i] = mma_kernel.vec_output[i];

            chan_mma_to_proc.send(pkt_out);
            mma_kernel.intr_mma.notify();
        }
    }
};

int sc_main(int argc, char* argv[]) {
    MLPSystemTop top("top");
    sc_start();
    return 0;
}
