
#include "SlaveTransactor.h"

void SlaveTransactor::slave_process() {
    cout << "[SlaveTransactor] Listening on Slave ID " << slaveID << endl;

    while (true) {
        // منتظر فعال‌سازی از سوی master
        t_cs.write(SC_LOGIC_0);
        bus_port->slaveMMcollection(slaveID, addr, data, r_en, w_en);
        //cout << "[SlaveTransactor] Received request:" << endl;
       // cout << "  addr = " << addr << endl;
        //cout << "  data = " << data << endl;
        //cout << "  r_en = " << r_en << ", w_en = " << w_en << endl;
        // ارسال مقادیر به سمت حافظه

        t_addr.write(addr);
        t_in.write(data);
        t_rd.write(r_en);
        t_wr.write(w_en);
        t_cs.write(SC_LOGIC_1);
        wait(SC_ZERO_TIME);
        cout << "[slaveTransactor] Receiving slaveMMcollection: addrMem = " << addr << ", dataMem = " << data << ", writeMem =  " << w_en << ", readMem = " << r_en << ", csMem = " << SC_LOGIC_1 << endl;
        //cout << "  cs = " << t_cs.read() <<  endl;
    }
}

void SlaveTransactor::on_mem_ready() {
    if (t_ready.read() == SC_LOGIC_1) {
        cout << "[SlaveTransactor] memReady = 1 detected, calling slaveMMresponse() from : " << slaveID << endl;
        
        sc_lv<16> read_value = t_out.read();

        cout << "[SlaveTransactor] data from memory = " << read_value << endl;

        bus_port->slaveMMresponse(read_value);
    }
}

