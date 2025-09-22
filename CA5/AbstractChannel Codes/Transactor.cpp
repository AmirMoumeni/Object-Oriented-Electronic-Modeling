
// ---------------------- Transactor.cpp ----------------------
#include "Transactor.h"

//template<int N>
void Transactor::transactor_process() {
    while (true) {
        wait();

        i_ready.write(SC_LOGIC_0);

        //cout << "[Transactor] writeMem =  " << i_wr.read() << "readMem = " << i_rd.read() << endl;

        if (i_wr.read() == SC_LOGIC_1 || i_rd.read() == SC_LOGIC_1) {

            cout << "[Transactor] Sending masterMMreq: addrMem = " << i_addr.read() << ", dataMem = " <<  i_out.read() << ", writeMem =  " << i_wr.read() << ", readMem = " << i_rd.read()<<endl;

            bus->masterMMreq(i_addr.read(), i_out.read(), i_in.read(), masterID, i_rd.read(), i_wr.read());

            cout << "[Transactor] End of masterMMreq." << endl;
            
            i_ready.write(SC_LOGIC_1);
        }
    }
}

// Explicit instantiation
//template class Transactor<16>;
// because of
// template<int N>
// SC_MODULE(Transactor) {