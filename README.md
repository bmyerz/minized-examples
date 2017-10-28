# Getting started with the Avnet MiniZed

My goal is to use the Avnet MiniZed for small prototypes of hardware accelerators running on the FPGA ("processing logic" or PL) that can talk to the Arm core ("processing system" or PS). Keep that purpose in mind if you choose to follow these steps.

The MiniZed is fairly new (as of starting this repository in Oct 2017), so there is still a limited number of targeted tutorials. Fortunately, there are lots of close-enough tutorials for other Zynq boards including older Avnet boards. 

Since the Zynq combines hardware and software development and MiniZed is an introductory board, I figure there must be other mere mortals like myself using it. This documentation is written from that perspective. If you have tips, please contribute!

1. __Get started with the board, Vivado, and SDK__

Follow Avnet's MiniZed tutorials 1-3. Find it at http://zedboard.org/support/design/18891/146 (Or go to http://zedboard.org/ then go to Support | Reference designs and tutorials | MiniZed).

2. __Get the PS to talk to PL with existing IP__

At this point, you now understand how to create an HDF in Vivado using the block editor and use SDK to create and run applications. Next, get some more interesting PL to talk to the PS. For that, go to https://github.com/bmyerz/minized-examples/tree/master/dma.

3. __Get the PS to talk to PL with your own IP__

At this point, you have worked with an application where the PS talks to the PL, which included an existing AXI peripheral (BRAM). A good next step is to create an application where the software talks to some custom HDL. I adapted this tutorial http://www.fpgadeveloper.com/2014/08/creating-a-custom-ip-block-in-vivado.html. 
