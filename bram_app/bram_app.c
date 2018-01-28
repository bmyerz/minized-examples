#include "xparameters.h"
#include "stdio.h"
#include "xil_testmem.h"
#include "xil_exception.h"
#include "xil_types.h"
#include "xil_cache.h"

#include "xil_printf.h"

#define BRAM_MEMORY XPAR_AXI_BRAM_CTRL_0_S_AXI_BASEADDR
#define BRAM_MEMORY_LAST_WORD (XPAR_AXI_BRAM_CTRL_0_S_AXI_HIGHADDR-3)

#define LENGTH 4096 // source and destination buffers lengths in number of words

// you want to avoid writing over our  program
// since in the standalone/baremetal mode there is no memory protection
#define DDR_MEMORY XPAR_PS7_DDR_0_S_AXI_BASEADDR+0x00020000 // pass all code and data sections


int main (void) {

	int i;
    int num;

    u32 * source, * destination;

    int Status;


   	    // The only difference
   	    // between the transfer with different types is the memory addresses!
    	switch(select)
		{
			case '1' :
				source = (u32 *)BRAM_MEMORY;
				destination = (u32 *)(BRAM_MEMORY+LENGTH);
				print("BRAM to BRAM transfer\r\n");
				if ((int)(destination+num-1) > BRAM_MEMORY_LAST_WORD) {
						  print("too big of transfer for BRAM destination\r\n");
						  continue;
				}
				break;
			case '2' :
				source = (u32 *)BRAM_MEMORY;
				destination = (u32 *)DDR_MEMORY;
				print("BRAM to DDR3 transfer\r\n");
				if ((int)(source+num-1) > BRAM_MEMORY_LAST_WORD) {
										  print("too big of transfer for BRAM source\r\n");
										  continue;
								}
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

		// Initialize src memory
		print("initialize source memory\r\n");
		for (i=0; i<num; i++) {
			//xil_printf("clearing %x of %x\r\n", (int)(source+i), num);
			*(source+i) = num-i;
		}
		print("initialized\r\n");


		print("transferring\r\n");
		for (i=0; i<num; i++) {
			//xil_printf("transferring %d\r\n",i);
			*(destination+i) = *(source+i);
		}
		print("transferred\r\n");

		xil_printf("x improvement \r\n");

    return 0;
}

