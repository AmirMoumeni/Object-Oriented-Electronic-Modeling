// ---------------------- busChannel.cpp ----------------------
#include "busChannel.h"

// Return the master ID with the highest priority that is currently requesting the bus
int busChannel::getPriorityMaster() {
    if (requesting[0]) return 0;  // Master 0 has highest priority
    if (requesting[1]) return 1;  // Master 1 has lower priority
    return -1;                    // No master is requesting
}

// Master sends memory request to the bus
void busChannel::masterMMreq(sc_lv<16> addr, sc_lv<16> writeData, sc_lv<16> raeadData, int masterID,
    sc_logic readMem, sc_logic writeMem) {
    requesting[masterID] = true; // Mark this master as requesting access

    // Wait until this master becomes the highest priority requesting master
    while (getPriorityMaster() != masterID) {
        wait(slaveOprCompleted); // Wait until another master finishes
    }

    // Now this master is allowed to access the bus
  //  cout << "-------------------  [busChannel] Starting a Transaction ------------------- " << endl;
    busBusy.lock();              // Lock the bus
    requesting[masterID] = false; // Clear the request flag

    // Set data on the shared bus
    addrOut = addr;
    writeDataOut = writeData;
    readMemOut = readMem;
    writeMemOut = writeMem;
    masterIDOut = masterID;

    // Decode target slave based on address
    goingToSlave = decodeSlave(addr.to_uint());

    if (goingToSlave >= 0 && goingToSlave <= 3) {
        cout << "[busChannel :: masterMMreq] goingToSlave " << goingToSlave << endl;
        requestMM[goingToSlave].notify(SC_ZERO_TIME); // Notify the selected slave

        // Wait for slave to respond
        wait(slaveOprCompleted);
        raeadData = readDataIn; // Return read data back to master
        cout << "[busChannel :: masterMMreq] slaveOprCompleted notified (memory ready). raeadData = " << raeadData << endl;
    }
    else {
        cout << "[busChannel] Invalid slave ID, skipping wait." << endl;
    }

    // Release the bus 
    busBusy.unlock();
    //slaveOprCompleted.notify(SC_ZERO_TIME);
}

// Slave retrieves the request sent by the master
void busChannel::slaveMMcollection(int slaveID,
    sc_lv<16>& addr,
    sc_lv<16>& data,
    sc_logic& readEnable,
    sc_logic& writeEnable) {
    cout << "[busChannel :: slaveMMcollection] waiting for slave ID = " << slaveID  << endl;
    if (slaveID != goingToSlave) {
        wait(requestMM[slaveID]); // Wait for request notification
    }
    // Read request details from the bus
    addr = addrOut.read();
    data = writeDataOut.read();
    readEnable = readMemOut.read();
    writeEnable = writeMemOut.read();
    goingToSlave = -1;
    
}

// Map address to a specific slave (address decoding logic)
int busChannel::decodeSlave(sc_uint<16> addrInt) {
    const int A1 = 0x0000, A2 = A1 + 0x0FFF;
    const int A3 = 0x1000, A4 = A3 + 0x0FFF;
    const int A5 = 0x2000, A6 = A5 + 0x0FFF;
    const int A7 = 0x3000, A8 = A7 + 0x0FFF;

    if (addrInt >= A1 && addrInt <= A2) {
        cout << "[busChannel :: decodeSlave] Address 0x" << hex << addrInt
            << " => Slave 0" << endl;
        return 0;
    }
    if (addrInt >= A3 && addrInt <= A4) {
        cout << "[busChannel :: decodeSlave] Address 0x" << hex << addrInt
            << " => Slave 1" << endl;
        return 1;
    }
    if (addrInt >= A5 && addrInt <= A6) {
        cout << "[busChannel :: decodeSlave] Address 0x" << hex << addrInt
            << " => Slave 2" << endl;
        return 2;
    }
    if (addrInt >= A7 && addrInt <= A8) {
        cout << "[busChannel :: decodeSlave] Address 0x" << hex << addrInt
            << " => Slave 3" << endl;
        return 3;
    }

        cout << "[decodeSlave] Address 0x" << hex << addrInt
        << " → Invalid (no slave match)" << endl;
    return -1;
}

// Slave sends read data back to master
void busChannel::slaveMMresponse(sc_lv<16>& readData) {
    cout << "[busChannel::slaveMMresponse] Notify master waiting for the response, Read Data = " << readData << endl;

    readDataIn = readData;               // Save data into bus channel
    slaveOprCompleted.notify(SC_ZERO_TIME);  // Notify master waiting for the response
    
}
