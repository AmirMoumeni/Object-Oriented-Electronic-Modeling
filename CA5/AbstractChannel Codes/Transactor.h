// ---------------------- Transactor.h ----------------------
#include <systemc.h>
#include "busChannel.h"

//template<int N>
SC_MODULE(Transactor) {
    int masterID;
    sc_in<sc_lv<16>> i_addr;
    sc_in<sc_lv<16>> i_out;
    sc_in<sc_logic> i_rd, i_wr;
    sc_out<sc_logic> i_ready;
    sc_out<sc_lv<16>> i_in;

    sc_port<masterSide_if<16>> bus;

    SC_CTOR(Transactor) {
        SC_THREAD(transactor_process);
        sensitive << i_rd.pos() << i_wr.pos();
    }

    void transactor_process();
};
