#include "xparameters.h"
#include "stdio.h"
#include "xil_testmem.h"
#include "xil_exception.h"
#include "xil_types.h"
#include "xil_cache.h"

#include "xuartps.h"	// if PS uart is used
#include "xscutimer.h"  // if PS Timer is used
#include "xdmaps.h"		// if PS DMA is used
#include "xscugic.h" 	// if PS GIC is used
#include "xil_exception.h"	// if interrupt is used
#include "xil_printf.h"

//These BRAM parameters are currently set incorrectly in SDK 2014.4 xparameters.h
//This is a known bug that is scheduled to be fixed in 2015.1
//See http://forums.xilinx.com/t5/Zynq-All-Programmable-SoC/Bram-parameters-in-Xparameters-h/m-p/555182/highlight/true#M5042
#undef XPAR_AXI_BRAM_CTRL_0_BASEADDR
#undef XPAR_AXI_BRAM_CTRL_0_HIGHADDR
#define XPAR_AXI_BRAM_CTRL_0_BASEADDR 0x40000000
#define XPAR_AXI_BRAM_CTRL_0_HIGHADDR 0x40001FFF

#define BRAM_MEMORY XPAR_AXI_BRAM_CTRL_0_BASEADDR

#define RESET_LOOP_COUNT	10	// Number of times to check reset is done
#define LENGTH 8192 // source and destination buffers lengths in number of words
#define DDR_MEMORY XPAR_PS7_DDR_0_S_AXI_BASEADDR+0x00020000 // pass all code and data sections
#define TIMER_DEVICE_ID	XPAR_SCUTIMER_DEVICE_ID
#define TIMER_LOAD_VALUE 0xFFFFFFFF
#define DMA0_ID XPAR_XDMAPS_1_DEVICE_ID
#define INTC_DEVICE_INT_ID XPAR_SCUGIC_SINGLE_DEVICE_ID

volatile static int Done = 0;	/* Dma transfer is done */
volatile static int Error = 0;	/* Dma Bus Error occurs */

XUartPs Uart_PS;		/* Instance of the UART Device */
XScuTimer Timer;		/* Cortex A9 SCU Private Timer Instance */
XDmaPs Dma;				/* PS DMA */
XScuGic Gic;			/* PS GIC */

int getNumber (){

	u8 byte;
	u8 uartBuffer[16];
	u8 validNumber;
	int digitIndex;
	int digit, number, sign;
	int c;

	while(1){
		byte = 0x00;
		digit = 0;
		digitIndex = 0;
		number = 0;
		validNumber = TRUE;

		//get bytes from uart until RETURN is entered
		while(byte != 0x0d){
			while (!XUartPs_IsReceiveData(STDIN_BASEADDRESS));
			byte = XUartPs_ReadReg(STDIN_BASEADDRESS,
								    XUARTPS_FIFO_OFFSET);
			uartBuffer[digitIndex] = byte;
			XUartPs_Send(&Uart_PS, &byte, 1);
			digitIndex++;
		}

		//calculate number from string of digits

		for(c = 0; c < (digitIndex - 1); c++){
			if(c == 0){
				//check if first byte is a "-"
				if(uartBuffer[c] == 0x2D){
					sign = -1;
					digit = 0;
				}
				//check if first byte is a digit
				else if((uartBuffer[c] >> 4) == 0x03){
					sign = 1;
					digit = (uartBuffer[c] & 0x0F);
				}
				else
					validNumber = FALSE;
			}
			else{
				//check byte is a digit
				if((uartBuffer[c] >> 4) == 0x03){
					digit = (uartBuffer[c] & 0x0F);
				}
				else
					validNumber = FALSE;
			}
			number = (number * 10) + digit;
		}
		number *= sign;
		if(validNumber == TRUE){
			print("\r\n");
			return number;
		}
		print("This is not a valid number.\n\r");
	}
}

void DmaDoneHandler(unsigned int Channel,
		    XDmaPs_Cmd *DmaCmd,
		    void *CallbackRef)
{
	/* done handler */
  	Done = 1;
}

void DmaFaultHandler(unsigned int Channel,
		     XDmaPs_Cmd *DmaCmd,
		     void *CallbackRef)
{
	/* fault handler */

	Error = 1;
}

int SetupIntrSystem(XScuGic *GicPtr, XDmaPs *DmaPtr)
{
	int Status;

	Xil_ExceptionInit();

	// Connect the interrupt controller interrupt handler to the hardware
	// interrupt handling logic in the processor.
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT,
			     (Xil_ExceptionHandler)XScuGic_InterruptHandler,
			     GicPtr);

	// Connect a device driver handler that will be called when an interrupt
	// for the device occurs, the device driver handler performs the specific
	// interrupt processing for the device

	// Connect the Fault ISR
	Status = XScuGic_Connect(GicPtr,
				 XPAR_XDMAPS_0_FAULT_INTR,
				 (Xil_InterruptHandler)XDmaPs_FaultISR,
				 (void *)DmaPtr);
	if (Status != XST_SUCCESS)
		return XST_FAILURE;

	// Connect the Done ISR for channel 0 of DMA 0
	Status = XScuGic_Connect(GicPtr,
				 XPAR_XDMAPS_0_DONE_INTR_0,
				 (Xil_InterruptHandler)XDmaPs_DoneISR_0,
				 (void *)DmaPtr);

	if (Status != XST_SUCCESS)
		return XST_FAILURE;

	// Enable the interrupt for the device
	XScuGic_Enable(GicPtr, XPAR_XDMAPS_0_DONE_INTR_0);

	return XST_SUCCESS;
}

u8 menu(void)
{
	u8 byte;

	print("Enter 1 for BRAM to BRAM transfer\r\n");
	print("Enter 2 for BRAM to DDR3 transfer\r\n");
	print("Enter 3 for DDR3 to DDR3 transfer\r\n");
	print("Enter 4 to exit\r\n");
	while (!XUartPs_IsReceiveData(STDIN_BASEADDRESS));
	byte = XUartPs_ReadReg(STDIN_BASEADDRESS,
						    XUARTPS_FIFO_OFFSET);
	return(byte);

}

u8 byte_sel(void)
{
	u8 byte;

   	print("Enter number of words you want to transfer:\r\n");
   	print("1=256; 2=512; 3=1024; 4=2048; 5=4096; 6=8192;\r\n");
 	while (!XUartPs_IsReceiveData(STDIN_BASEADDRESS));
	byte = XUartPs_ReadReg(STDIN_BASEADDRESS,
						    XUARTPS_FIFO_OFFSET);
	return(byte);
}

int main (void) {

	u8 select;
	int i;
    int num;
    u8 num_in;
    int dma_improvement;

    u32 * source, * destination;
    int software_cycles, interrupt_cycles;
    int test_done = 0;

	// UART related definitions
    int Status;
	XUartPs_Config *Config;

	// PS Timer related definitions
	volatile u32 CntValue1;
	XScuTimer_Config *ConfigPtr;
	XScuTimer *TimerInstancePtr = &Timer;

	// PS DMA related definitions
	XDmaPs_Config *DmaCfg;
	XDmaPs_Cmd DmaCmd = {
		.ChanCtrl = {
			.SrcBurstSize = 4,
			.SrcBurstLen = 4,
			.SrcInc = 1,		// increment source address
			.DstBurstSize = 4,
			.DstBurstLen = 4,
			.DstInc = 1,		// increment destination address
		},
	};
	unsigned int Channel = 0;

	// PS Interrupt related definitions
	XScuGic_Config *GicConfig;

	// Initialize UART
	// Look up the configuration in the config table, then initialize it.
	Config = XUartPs_LookupConfig(XPAR_XUARTPS_0_DEVICE_ID);
	if (NULL == Config) {
		return XST_FAILURE;
	}

	Status = XUartPs_CfgInitialize(&Uart_PS, Config, Config->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	// Initialize timer counter
	ConfigPtr = XScuTimer_LookupConfig(TIMER_DEVICE_ID);

	Status = XScuTimer_CfgInitialize(TimerInstancePtr, ConfigPtr,
				 ConfigPtr->BaseAddr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	// Initialize GIC
	GicConfig = XScuGic_LookupConfig(INTC_DEVICE_INT_ID);
	if (NULL == GicConfig) {
		xil_printf("XScuGic_LookupConfig(%d) failed\r\n",
				INTC_DEVICE_INT_ID);
		return XST_FAILURE;
	}

	Status = XScuGic_CfgInitialize(&Gic, GicConfig,
				       GicConfig->CpuBaseAddress);
	if (Status != XST_SUCCESS) {
		xil_printf("XScuGic_CfgInitialize failed\r\n");
		return XST_FAILURE;
	}

	// Set options for timer/counter 0
	// Load the timer counter register.
	XScuTimer_LoadTimer(TimerInstancePtr, TIMER_LOAD_VALUE);

	// Start the Scu Private Timer device.
	XScuTimer_Start(TimerInstancePtr);

    print("-- Simple DMA Design Example --\r\n");

	// Get a snapshot of the timer counter value before it's started
	CntValue1 = XScuTimer_GetCounterValue(TimerInstancePtr);

	xil_printf("Above message printing took %d clock cycles\r\n", TIMER_LOAD_VALUE-CntValue1);

	// Setup DMA Controller
	DmaCfg = XDmaPs_LookupConfig(DMA0_ID);
	if (!DmaCfg) {
		xil_printf("Lookup DMAC %d failed\r\n", DMA0_ID);
		return XST_FAILURE;
	}

	Status = XDmaPs_CfgInitialize(&Dma,DmaCfg,DmaCfg->BaseAddress);

	if (Status) {
		xil_printf("XDmaPs_CfgInitialize failed\r\n");
		return XST_FAILURE;
	}

    // Setup Interrupt system here even we don't use it for the poll-DMA mode
	// as required by the PS DMA driver
	Status = SetupIntrSystem(&Gic, &Dma);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

//	Xil_ExceptionEnableMask(XIL_EXCEPTION_IRQ);

    test_done = 0;

    while(test_done==0)
    {

   	   	num_in = byte_sel();

   	   	switch(num_in)
    			{
    				case '1' :
    					num = 256;
    					print("Sending 256 bytes\r\n");
    					break;
    				case '2' :
    					num = 512;
    					print("Sending 512 bytes\r\n");
    					break;
    				case '3' :
    					num = 1024;
    					print("Sending 1024 bytes\r\n");
    					break;
    				case '4' :
    					num = 2048;
    					print("Sending 2048 bytes\r\n");
    				    break;
    				case '5' :
    					num = 4096;
    					print("Sending 4096 bytes\r\n");
    					break;
    				case '6' :
    					num = 8192;
    					print("Sending 8192 bytes\r\n");
    					break;
    				default :
    					num = 256;
    					print("Sending 256 bytes\r\n");
    					break;
    			}
   	   	print("\r\n");
   	    select = menu();

    	switch(select)
		{
			case '1' :
				source = (u32 *)BRAM_MEMORY;
				destination = (u32 *)(BRAM_MEMORY+LENGTH);
				print("BRAM to BRAM transfer\r\n");
				break;
			case '2' :
				source = (u32 *)BRAM_MEMORY;
				destination = (u32 *)DDR_MEMORY;
				print("BRAM to DDR3 transfer\r\n");
				break;
			case '3' :
				source = (u32 *)DDR_MEMORY;
				destination = (u32 *)(DDR_MEMORY+LENGTH);
				print("DDR3 to DDR3 transfer\r\n");
				break;
			case '4' :
				test_done = 1;
			default :
				source = (u32 *)DDR_MEMORY;
				destination = (u32 *)(DDR_MEMORY+LENGTH);
				print("DDR3 to DDR3 transfer\r\n");
				break;
		}



		if(test_done)
			break;



		// Following code is required only if Cache is enabled
		Xil_DCacheFlushRange((u32)&source, LENGTH);
		Xil_DCacheFlushRange((u32)&destination, LENGTH);

		// Initialize src memory
		for (i=0; i<num; i++)
			*(source+i) = num-i;


	// Non-DMA mode
		// reset timer
		XScuTimer_RestartTimer(TimerInstancePtr);

		// start moving data through the processor - no CDMA, no interrupt
		// gives base consumed cycles
		for (i=0; i<num; i++)
			*(destination+i) = *(source+i);

		CntValue1 = XScuTimer_GetCounterValue(TimerInstancePtr);

		xil_printf("Moving data through processor took %d clock cycles\r\n", TIMER_LOAD_VALUE-CntValue1);
		software_cycles = TIMER_LOAD_VALUE-CntValue1;

	// DMA in polling mode
//		print("Starting transfer through DMA in poll mode\r\n");
		DmaCmd.BD.SrcAddr = (u32)source;
		DmaCmd.BD.DstAddr = (u32)destination;
		DmaCmd.BD.Length = num * sizeof(int);

	// setting up for interrupt driven DMA
		// clear destination memory
		for (i=0; i<num; i++)
			*(destination+i) = 0;

		print("Setting up interrupt system\r\n");
		Status = SetupIntrSystem(&Gic, &Dma);
		if (Status != XST_SUCCESS) {
			return XST_FAILURE;
		}

		Xil_ExceptionEnableMask(XIL_EXCEPTION_IRQ);

		XDmaPs_SetDoneHandler(&Dma,0,DmaDoneHandler,0);
		Status = XDmaPs_Start(&Dma, Channel, &DmaCmd, 0);	// release DMA buffer as we are done

		// reset timer
		XScuTimer_RestartTimer(TimerInstancePtr);

		while ((Done==0) & (Error==0));
		if (Error)
			print("Error occurred during DMA transfer\r\n");

		CntValue1 = XScuTimer_GetCounterValue(TimerInstancePtr);

		xil_printf("Moving data through DMA in Interrupt mode took %d clock cycles\r\n", TIMER_LOAD_VALUE-CntValue1);
		interrupt_cycles = TIMER_LOAD_VALUE-CntValue1;

		print("Transfer complete\r\n");
		// Disable the interrupt for the device
		XScuGic_Disable(&Gic, XPAR_XDMAPS_0_DONE_INTR_0);

		for (i = 0; i < num; i++) {
				if ( destination[i] != source[i]) {
					xil_printf("Data match failed at = %d, source data = %d, destination data = %d\n\r",i,source[i],destination[i]);
					print("-- Exiting main() --");
					return XST_FAILURE;
				}
			}
		print("Transfered data verified\r\n");
		dma_improvement = software_cycles/interrupt_cycles;
		xil_printf("Improvement using Interrupt DMA = %2d", dma_improvement);
		xil_printf("x improvement \r\n");
		xil_printf("------------------------------------------------------------------- \r\n\r\n");
		Error = 0;
		Done = 0;
	//	select = menu();
    }
    print("-- Exiting main() --\r\n");
    return 0;
}

