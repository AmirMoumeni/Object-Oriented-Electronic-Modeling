#pragma once

#include "interfaceClasses.h"

//template <int N>
class busChannel : public sc_channel,
    public masterSide_if<16>,
    public slaveSide_if<16> {
public:
    sc_mutex busBusy;
    int goingToSlave =-1;
    bool requesting[2] = { false, false }; 

    sc_signal<sc_lv<16>> addrOut;
    sc_signal<sc_lv<16>> writeDataOut;
    sc_signal<sc_lv<16>> readDataIn;
    sc_signal<sc_logic> readMemOut;
    sc_signal<sc_logic> writeMemOut;
    sc_signal<int> masterIDOut;
    sc_signal<sc_logic> memReadyOut;



    sc_event slaveOprCompleted;
    sc_event requestMM[4];

    //SC_HAS_PROCESS(busChannel);
    public:
        busChannel(sc_module_name NAME) : sc_channel(NAME) {};

    void masterMMreq(sc_lv<16> addr, sc_lv<16> writeData, sc_lv<16> raeadData, int masterID,
        sc_logic readMem, sc_logic writeMem) override;

    void slaveMMcollection(int slaveID,
        sc_lv<16>& addr,
        sc_lv<16>& data,
        sc_logic& readEnable,
        sc_logic& writeEnable);

    void slaveMMresponse(sc_lv<16>& readData) override;


    int decodeSlave(sc_uint<16> addrInt);
    int getPriorityMaster();

};
