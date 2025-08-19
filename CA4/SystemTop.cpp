#include <systemc.h>
#include <array>
#include <queue>
#include <fstream>
#include <iostream>
#include <mutex>

using namespace std;

class chan_if : virtual public sc_interface {
public:
    virtual void push_data(const array<sc_lv<8>, 9>& pkt) = 0;
    virtual void pop_data(array<sc_lv<8>, 9>& pkt) = 0;
};

class data_bus : public sc_channel, public chan_if {
private:
    queue<array<sc_lv<8>, 9>> qbuf;
    sc_event ev_ready;
    mutex qmtx;

public:
    data_bus(sc_module_name nm) : sc_channel(nm) {}

    void push_data(const array<sc_lv<8>, 9>& pkt) override {
        lock_guard<mutex> lk(qmtx);
        qbuf.push(pkt);
        ev_ready.notify();
    }

    void pop_data(array<sc_lv<8>, 9>& pkt) override {
        lock_guard<mutex> lk(qmtx);
        if (qbuf.empty()) {
            wait(ev_ready);
        }
        pkt = qbuf.front();
        qbuf.pop();
    }
};

SC_MODULE(MatUnit) {
    sc_in<bool> trigMUL;
    sc_out<bool> irqMUL;
    sc_port<chan_if> bus;

    void exec() {
        while (true) {
            wait(trigMUL.posedge_event());

            array<sc_lv<8>, 9> seg_in;
            bus->pop_data(seg_in);
            sc_int<8> vecA[3] = {
                seg_in[0].to_int(),
                seg_in[1].to_int(),
                seg_in[2].to_int()
            };

            array<sc_lv<8>, 9> seg_w;
            bus->pop_data(seg_w);
            sc_int<8> matB[3][2];
            for (int r = 0; r < 3; r++) {
                for (int c = 0; c < 2; c++) {
                    matB[r][c] = seg_w[r * 2 + c].to_int();
                }
            }

            sc_int<18> vecO[2] = {0, 0};
            for (int c = 0; c < 2; c++) {
                for (int r = 0; r < 3; r++) {
                    vecO[c] += vecA[r] * matB[r][c];
                }
            }

            array<sc_lv<8>, 9> seg_out;
            seg_out[0] = vecO[0];
            seg_out[1] = vecO[1];
            for (int i = 2; i < 9; i++) {
                seg_out[i] = 0;
            }
            bus->push_data(seg_out);

            irqMUL.write(true);
            wait(SC_ZERO_TIME);
            irqMUL.write(false);
        }
    }

    SC_CTOR(MatUnit) {
        SC_THREAD(exec);
        sensitive << trigMUL;
    }
};

SC_MODULE(CPU) {
    sc_port<chan_if> bus;
    sc_out<bool> trigMUL;
    sc_in<bool> irqMUL;

    array<sc_lv<8>, 3> vec_in;
    array<array<sc_lv<8>, 8>, 3> mat_w;
    array<sc_lv<18>, 8> vec_out;

    void main_proc() {
        read_inputs();
        read_weights();

        calc_layer();

        dump_outputs();
        sc_stop();
    }

    void calc_layer() {
        for (int blk = 0; blk < 4; blk++) {
            array<sc_lv<8>, 9> seg_in;
            for (int i = 0; i < 3; i++) {
                seg_in[i] = vec_in[i];
            }
            for (int i = 3; i < 9; i++) {
                seg_in[i] = 0;
            }

            array<sc_lv<8>, 9> seg_w;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 2; j++) {
                    seg_w[i * 2 + j] = mat_w[i][blk * 2 + j];
                }
            }
            for (int i = 6; i < 9; i++) {
                seg_w[i] = 0;
            }

            bus->push_data(seg_in);
            bus->push_data(seg_w);

            trigMUL.write(true);
            wait(SC_ZERO_TIME);
            trigMUL.write(false);

            wait(irqMUL.posedge_event());

            array<sc_lv<8>, 9> seg_out;
            bus->pop_data(seg_out);

            vec_out[blk * 2] = seg_out[0];
            vec_out[blk * 2 + 1] = seg_out[1];
        }
    }

    void read_inputs() {
        ifstream fin("data.txt");
        if (!fin) {
            cerr << "Error opening inputs.txt" << endl;
            sc_stop();
            return;
        }

        for (int i = 0; i < 3; i++) {
            int v;
            fin >> v;
            vec_in[i] = v;
        }
        fin.close();
    }

    void read_weights() {
        ifstream fin("weights.txt");
        if (!fin) {
            cerr << "Error opening weights.txt" << endl;
            sc_stop();
            return;
        }

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 8; j++) {
                int v;
                fin >> v;
                mat_w[i][j] = v;
            }
        }
        fin.close();
    }

    void dump_outputs() {
        ofstream fout("SystemTop_outputs.txt");
        if (!fout) {
            cerr << "Error creating outputs.txt" << endl;
            return;
        }

        for (int i = 0; i < 8; i++) {
            fout << vec_out[i].to_int() << endl;
        }
        fout.close();
    }

    SC_CTOR(CPU) {
        SC_THREAD(main_proc);
    }
};

int sc_main(int argc, char* argv[]) {
    data_bus bus("bus");
    MatUnit accel("accel");
    CPU cpu("cpu");

    sc_signal<bool> sig_start, sig_irq;

    cpu.bus(bus);
    cpu.trigMUL(sig_start);
    cpu.irqMUL(sig_irq);

    accel.bus(bus);
    accel.trigMUL(sig_start);
    accel.irqMUL(sig_irq);

    sc_start();
    return 0;
}
