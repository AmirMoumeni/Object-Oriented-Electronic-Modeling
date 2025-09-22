#include <iostream>
#include <systemc.h>
#include <bits/stdc++.h>
#include "System.h"

SC_MODULE(SystemTester)
{
	sc_signal <sc_logic> clk;


	EmbeddedSystem<16> *systemModule;

	SC_CTOR(SystemTester)
	{
		systemModule = new EmbeddedSystem<16>("systemModule");
		(*systemModule)(clk);

		// Amir: configure DMA burst before starting any simulation
		systemModule->dma->configureDMA(0x0002, 0x1002, 4); // copy 4 words per burst
		systemModule->dma->byteCount = 16;                  // total number of transfers
		systemModule->dma->controlReg = "011";              // Start + WR (Mem -> MMA)
		// bit0 = Start, bit1 = WR, bit2 = RD

		SC_THREAD (clocking);

	}

	void clocking();
};
void SystemTester::clocking()
{
	clk = SC_LOGIC_0;
	while (true)
	{
		
		wait(2.5,SC_NS);
		clk = SC_LOGIC_0;
		wait(2.5,SC_NS);
		clk = SC_LOGIC_1;
	}
}

int sc_main(int argc, char* argv[])
{
    // Silence SystemC warnings about sc_logic
    sc_report_handler::set_actions(SC_ID_VECTOR_CONTAINS_LOGIC_VALUE_, SC_DO_NOTHING);
    sc_report_handler::set_actions(SC_WARNING, SC_DO_NOTHING);

    // Instantiate the top-level tester
    SystemTester* TOP = new SystemTester("systemTester");

    // Create VCD trace file
    sc_trace_file* VCDFile;
    VCDFile = sc_create_vcd_trace_file("wave");

    // Trace clock
    sc_trace(VCDFile, TOP->clk, "clk");

    // Trace some key DMA + MMA signals for burst testing
    sc_trace(VCDFile, TOP->systemModule->dma->i_addr, "dma_i_addr");
    sc_trace(VCDFile, TOP->systemModule->dma->i_out, "dma_i_out");
    sc_trace(VCDFile, TOP->systemModule->dma->i_in, "dma_i_in");
    sc_trace(VCDFile, TOP->systemModule->dma->i_rd, "dma_i_rd");
    sc_trace(VCDFile, TOP->systemModule->dma->i_wr, "dma_i_wr");
    sc_trace(VCDFile, TOP->systemModule->dma->i_ready, "dma_i_ready");

    sc_trace(VCDFile, TOP->systemModule->mma->dmaIn, "mma_dmaIn");
    sc_trace(VCDFile, TOP->systemModule->mma->dmaOut, "mma_dmaOut");
    sc_trace(VCDFile, TOP->systemModule->mma->controlReg, "mma_controlReg");

    // Run the simulation
    sc_start(10000, SC_NS);

    sc_close_vcd_trace_file(VCDFile);
    return 0;
}


// int sc_main(int argc, char *argv[])
// {
	
// 	sc_report_handler::set_actions (SC_ID_VECTOR_CONTAINS_LOGIC_VALUE_,
//                                 SC_DO_NOTHING);
// 	sc_report_handler::set_actions (SC_WARNING, SC_DO_NOTHING);


// 	systemTest * TOP = new systemTest ("systemTest_TB");
// 	sc_trace_file* VCDFile;
// 	VCDFile = sc_create_vcd_trace_file("wave");
// 	sc_trace(VCDFile, TOP -> clk, "clk");
// 	sc_trace(VCDFile, TOP->systemModule->mma->controlReg, "mmaControlReg");
// 	sc_trace(VCDFile, TOP->systemModule->mma->dmaOut, "mmaDMAOut");
// 	sc_trace(VCDFile, TOP->systemModule->mma->dmaIn, "mmaDMAIn");

// 	sc_trace(VCDFile, TOP->systemModule->bus->req, "busReq");

// 	sc_trace(VCDFile, TOP->systemModule->dma->t_cs, "dmaConfigPortCS");
// 	sc_trace(VCDFile, TOP->systemModule->dma->t_in, "dmaConfigPortIn");
// 	sc_trace(VCDFile, TOP->systemModule->dma->t_out, "dmaConfigPortOut");
// 	sc_trace(VCDFile, TOP->systemModule->dma->t_addr, " dmaConfigPortAddr");
// 	sc_trace(VCDFile, TOP->systemModule->dma->t_rd, "dmaConfigPortRD");
// 	sc_trace(VCDFile, TOP->systemModule->dma->t_wr, "dmaConfigPortWR");
// 	sc_trace(VCDFile, TOP->systemModule->dma->t_ready, "dmaConfigPortReady");


// 	sc_trace(VCDFile, TOP->systemModule->memory->t_cs, " memPortCS");
// 	sc_trace(VCDFile, TOP->systemModule->memory->t_in, " memPortIn");
// 	sc_trace(VCDFile, TOP->systemModule->memory->t_out, " memPortOut");
// 	sc_trace(VCDFile, TOP->systemModule->memory->t_addr, " memPortAddr");
// 	sc_trace(VCDFile, TOP->systemModule->memory->t_rd, " memPortRD");
// 	sc_trace(VCDFile, TOP->systemModule->memory->t_wr, " memPortWR");
// 	sc_trace(VCDFile, TOP->systemModule->memory->t_ready, " memPortReady");


// 	// sc_trace(VCDFile, TOP->systemModule->cpu->i_in, "cpuInitiatorIn");
// 	// sc_trace(VCDFile, TOP->systemModule->cpu->i_addr, "cpuInitiatorAddr");
// 	// sc_trace(VCDFile, TOP->systemModule->cpu->i_out, "cpuInitiatorOut");
// 	// sc_trace(VCDFile, TOP->systemModule->cpu->i_rd, "cpuInitiatorRD");
// 	// sc_trace(VCDFile, TOP->systemModule->cpu->i_wr, "cpuInitiatorWR");
// 	// sc_trace(VCDFile, TOP->systemModule->cpu->i_ready, "cpuInitiatorReady");

// 	sc_trace(VCDFile, TOP->systemModule->dp->i_in, "cpuInitiatorIn");
// 	sc_trace(VCDFile, TOP->systemModule->dp->i_addr, "cpuInitiatorAddr");
// 	sc_trace(VCDFile, TOP->systemModule->dp->i_out, "cpuInitiatorOut");
// 	sc_trace(VCDFile, TOP->systemModule->dp->i_rd, "cpuInitiatorRD");
// 	sc_trace(VCDFile, TOP->systemModule->dp->i_wr, "cpuInitiatorWR");
// 	sc_trace(VCDFile, TOP->systemModule->dp->i_ready, "cpuInitiatorReady");

// 	sc_trace(VCDFile, TOP->systemModule->dma->interrupt, "dmaInt");
// 	sc_trace(VCDFile, TOP->systemModule->dma->i_in, "dmaInitiatorIn");
// 	sc_trace(VCDFile, TOP->systemModule->dma->i_addr, "dmaInitiatorAddr");
// 	sc_trace(VCDFile, TOP->systemModule->dma->i_out, "dmaInitiatorOut");
// 	sc_trace(VCDFile, TOP->systemModule->dma->i_rd, "dmaInitiatorRD");
// 	sc_trace(VCDFile, TOP->systemModule->dma->i_wr, "dmaInitiatorWR");
// 	sc_trace(VCDFile, TOP->systemModule->dma->i_ready, "dmaInitiatorReady");

// 	sc_trace(VCDFile, TOP->systemModule->pic->INT, "picINT");
// 	sc_trace(VCDFile, TOP->systemModule->pic->PIC_output, "picOUT");
// 	sc_trace(VCDFile, TOP->systemModule->pic->IR, "picIR");

// 	for(int i = 0;i<2;i++){
// 		sc_trace(VCDFile, TOP->systemModule->bus->req[i], "busInitiatorReq"+std::to_string(i));
// 		sc_trace(VCDFile, TOP->systemModule->bus->i_in[i], "busInitiatorIn"+std::to_string(i));
// 		sc_trace(VCDFile, TOP->systemModule->bus->i_addr[i], "busInitiatorAddr"+std::to_string(i));
// 		sc_trace(VCDFile, TOP->systemModule->bus->i_out[i], "busInitiatorOut"+std::to_string(i));
// 		sc_trace(VCDFile, TOP->systemModule->bus->i_rd[i], "busInitiatorRD"+std::to_string(i));
// 		sc_trace(VCDFile, TOP->systemModule->bus->i_wr[i], "busInitiatorWR"+std::to_string(i));
// 		sc_trace(VCDFile, TOP->systemModule->bus->i_ready[i], "busInitiatorReady"+std::to_string(i));
// 	}

// 	sc_trace(VCDFile, TOP->systemModule->bus->addressBus, "busAddressBus");
// 	sc_trace(VCDFile, TOP->systemModule->bus->dataBus ,"busDataBus");
// 	sc_trace(VCDFile, TOP->systemModule->bus->targetReadyBus, "busTargetReadyBus");
//     sc_start(10000,SC_NS);
	
// 	return 0;
// }