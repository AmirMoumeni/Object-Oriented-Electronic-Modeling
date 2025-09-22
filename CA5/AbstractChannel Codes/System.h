#include <systemc.h>
#include <iostream>
//#include "Bus.h"
#include "Memory.h"
#include "DMA.cpp"
#include "MatMulAcc.cpp"
#include "DummyProcessor.h"
#include "PICWrapper.h"
//#include "SayacInterface.h"

//Maryam
#include "busChannel.h"
#include "Transactor.h"
#include "SlaveTransactor.h"

#define NumberOfTargets 4
#define NumberOfInitiators 2
template <int N>
SC_MODULE(EmbeddedSystem)
{
public:
    sc_in<sc_logic> clk;

    //Bus<N, NumberOfInitiators, NumberOfTargets> *bus;
    MatMulAcc<N> *mma;
    DMA *dma;
    Memory<N> *memory;
    DummyProcessor<N> *dp;
    PICWrapper* pic;

    Transactor* cpuTransactor;
    Transactor* dmaMasterTransactor;
    busChannel* bus;
    SlaveTransactor* memTransactor;
    SlaveTransactor* mmaTransactor;
    SlaveTransactor* dmaSlaveTransactor;
    SlaveTransactor* picTransactor;
   // SayacInterface* cpu;


    sc_signal<sc_lv<N>> i_addr[NumberOfInitiators];
    sc_signal<sc_lv<N>> i_in[NumberOfInitiators];
    sc_signal<sc_lv<N>> i_out[NumberOfInitiators];
    sc_signal<sc_logic> i_wr[NumberOfInitiators];
    sc_signal<sc_logic> i_rd[NumberOfInitiators];
    sc_signal<sc_logic> i_ready[NumberOfInitiators];

    sc_signal<sc_logic> t_cs[NumberOfTargets];
    sc_signal<sc_lv<N>> t_addr[NumberOfTargets];
    sc_signal<sc_lv<N>> t_in[NumberOfTargets];
    sc_signal<sc_lv<N>> t_out[NumberOfTargets];
    sc_signal<sc_logic> t_wr[NumberOfTargets];
    sc_signal<sc_logic> t_rd[NumberOfTargets];
    sc_signal<sc_logic> t_ready[NumberOfTargets];

    // MMA to DMA signals
    sc_signal<sc_lv<N>> dmammaAddr;
    sc_signal<sc_lv<N>> dmammaIn;
    sc_signal<sc_lv<N>> dmammaOut;
    sc_signal<sc_logic> dmammaWR;
    sc_signal<sc_logic> dmammaRD;

    // PIC signals
    sc_signal<sc_logic> dmaInterrupt;
    sc_signal<sc_logic> mmaInterrupt;
    sc_signal<sc_logic> INTA_bar;
    sc_signal<sc_logic> INT;
   
    //Maryam
        // Master-side signals
    sc_signal<sc_lv<16>> addr_sig, data_in_sig, data_out_sig;
    sc_signal<sc_logic> readMem_sig, writeMem_sig, memReady_sig;

    // Slave-side (memory interface) signals
    sc_signal<sc_lv<16>> slave_addr_sig, slave_writeData_sig, slave_dataBus_sig;
    sc_signal<sc_logic> slave_readMem_sig, slave_writeMem_sig, slave_memReady_sig;


    SC_CTOR(EmbeddedSystem)
    {
        //Maryam
        bus = new busChannel("bus_channel");
        
        // Instantiate modules
        // 0 -> cpu

        // dp = new DummyProcessor<N>("CPU");
        // dp->clk(clk);
        // dp->i_addr(i_addr[0]);
        // dp->i_in(i_in[0]);
        // dp->i_out(i_out[0]);
        // dp->i_wr(i_wr[0]);
        // dp->i_rd(i_rd[0]);
        // dp->i_ready(i_ready[0]);

        cpu = new SayacInterface("CPU");
        cpu->clk(clk);
        cpu->i_addr(i_addr[0]);
        cpu->i_in(i_in[0]);
        cpu->i_out(i_out[0]);
        cpu->i_wr(i_wr[0]);
        cpu->i_rd(i_rd[0]);
        cpu->i_ready(i_ready[0]);
        cpu->INT(INT);
        cpu->INTA_bar(INTA_bar);


        //cpu transactor
        cpuTransactor = new Transactor("cpuTransactor");
        cpuTransactor->bus(*bus);
        cpuTransactor->i_addr(i_addr[0]);
        cpuTransactor->i_out(i_out[0]);
        cpuTransactor->i_in(i_in[0]);
        cpuTransactor->i_rd(i_rd[0]);
        cpuTransactor->i_wr(i_wr[0]);
        cpuTransactor->i_ready(i_ready[0]);
        cpuTransactor->masterID = 0;


        //mem -> #2
        memory = new Memory<N>("mem");
        memory->clk(clk);
        memory->t_cs(t_cs[2]);
        memory->t_addr(t_addr[2]);
        memory->t_in(t_in[2]);
        memory->t_out(t_out[2]);
        memory->t_wr(t_wr[2]);
        memory->t_rd(t_rd[2]);
        memory->t_ready(t_ready[2]);
        //mem transactor
        memTransactor = new SlaveTransactor("memTransactor");
        memTransactor->bus_port(*bus);
        memTransactor->t_cs(t_cs[2]);
        memTransactor->t_addr(t_addr[2]);
        memTransactor->t_in(t_in[2]);
        memTransactor->t_rd(t_rd[2]);
        memTransactor->t_wr(t_wr[2]);
        memTransactor->t_out(t_out[2]);
        memTransactor->t_ready(t_ready[2]);
        memTransactor->slaveID = 2;



        // MMA -> 0
        mma = new MatMulAcc<N>("mma");
        mma->clk(clk);
        mma->dmaAddr(dmammaAddr);
        mma->dmaIn(dmammaIn);
        mma->dmaOut(dmammaOut);
        mma->dmaWR(dmammaWR);
        mma->dmaRD(dmammaRD);
        mma->interrupt(mmaInterrupt);

        mma->t_cs(t_cs[0]);
        mma->t_addr(t_addr[0]);
        mma->t_in(t_in[0]);
        mma->t_out(t_out[0]);
        mma->t_wr(t_wr[0]);
        mma->t_rd(t_rd[0]);
        mma->t_ready(t_ready[0]);

        //mma transactor
        mmaTransactor = new SlaveTransactor("mmaTransactor");
        mmaTransactor->bus_port(*bus);
        mmaTransactor->t_cs(t_cs[0]);
        mmaTransactor->t_addr(t_addr[0]);
        mmaTransactor->t_in(t_in[0]);
        mmaTransactor->t_rd(t_rd[0]);
        mmaTransactor->t_wr(t_wr[0]);
        mmaTransactor->t_out(t_out[0]);
        mmaTransactor->t_ready(t_ready[0]);
        mmaTransactor->slaveID = 0;


        // DMA -> 1
        dma = new DMA("dma");
        dma->clk(clk);
        dma->mmaAddr(dmammaAddr);
        dma->mmaIn(dmammaIn);
        dma->mmaOut(dmammaOut);
        dma->mmaWR(dmammaWR);
        dma->mmaRD(dmammaRD);
        dma->interrupt(dmaInterrupt);


        dma->i_addr(i_addr[1]);
        dma->i_in(i_in[1]);
        dma->i_out(i_out[1]);
        dma->i_wr(i_wr[1]);
        dma->i_rd(i_rd[1]);
        dma->i_ready(i_ready[1]);

        dma->t_cs(t_cs[1]);
        dma->t_addr(t_addr[1]);
        dma->t_in(t_in[1]);
        dma->t_out(t_out[1]);
        dma->t_wr(t_wr[1]);
        dma->t_rd(t_rd[1]);
        dma->t_ready(t_ready[1]);

        //mma master side transactor
        dmaMasterTransactor = new Transactor("dmaMasterTransactor");
        dmaMasterTransactor->bus(*bus);
        dmaMasterTransactor->i_addr(i_addr[1]);
        dmaMasterTransactor->i_out(i_out[1]);
        dmaMasterTransactor->i_in(i_in[1]);
        dmaMasterTransactor->i_rd(i_rd[1]);
        dmaMasterTransactor->i_wr(i_wr[1]);
        dmaMasterTransactor->i_ready(i_ready[1]);
        dmaMasterTransactor->masterID = 1;

        //mma slave side transactor
        dmaSlaveTransactor = new SlaveTransactor("dmaSlaveTransactor");
        dmaSlaveTransactor->bus_port(*bus);
        dmaSlaveTransactor->t_cs(t_cs[1]);
        dmaSlaveTransactor->t_addr(t_addr[1]);
        dmaSlaveTransactor->t_in(t_in[1]);
        dmaSlaveTransactor->t_rd(t_rd[1]);
        dmaSlaveTransactor->t_wr(t_wr[1]);
        dmaSlaveTransactor->t_out(t_out[1]);
        dmaSlaveTransactor->t_ready(t_ready[1]);
        dmaSlaveTransactor->slaveID = 1;


        //pic -> 3
        pic = new PICWrapper("picWrapper");
        pic->clk(clk);
        pic->INTA_bar(INTA_bar);
        pic->INT(INT);
        pic->dmaInterrupt(dmaInterrupt);
        pic->mmaInterrupt(mmaInterrupt);

        pic->t_cs(t_cs[3]);
        pic->t_addr(t_addr[3]);
        pic->t_in(t_in[3]);
        pic->t_out(t_out[3]);
        pic->t_wr(t_wr[3]);
        pic->t_rd(t_rd[3]);
        pic->t_ready(t_ready[3]);

        //pic transactor
        picTransactor = new SlaveTransactor("picTransactor");
        picTransactor->bus_port(*bus);
        picTransactor->t_cs(t_cs[3]);
        picTransactor->t_addr(t_addr[3]);
        picTransactor->t_in(t_in[3]);
        picTransactor->t_rd(t_rd[3]);
        picTransactor->t_wr(t_wr[3]);
        picTransactor->t_out(t_out[3]);
        picTransactor->t_ready(t_ready[3]);
        picTransactor->slaveID = 3;

    }
};
