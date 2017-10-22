This was the first application I got working that actually involves the PS and custom PL talking to each other.

1. Download Avnet's Developing Zynq®-7000 All Programmable SoC Hardware (Vivado 2015.2 and 2016.2) labs at http://zedboard.org/support/trainings-and-videos 
2. Follow Lab 5 and 6, but make the following changes.

* Of course choose MiniZed as your board in Vivado
* "Run block automation" in Vivado 2017.2 will remove the M_AXI_GP0 (and related) ports from the Zynq PS IP. To fix this problem, after you add the AXI and BRAM generator and run connection automation, double-click Zynq PS and under PS-PL interface | AXI Non secure enablement | GP Master Interface | check GP Master 0. Re-run connection automation to make sure your PS and AXI interface get wired up. You can check that you did this step correctly by opening up the Address Editor and checking that AXI BRAM is listed.
* Use the copy of dma_test.c in this repository. It includes some bug fixes.
