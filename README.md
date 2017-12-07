# Getting started with the Avnet MiniZed

My goal is to use the Avnet MiniZed for small prototypes of hardware accelerators running on the FPGA ("processing logic" or PL) that can talk to the Arm core ("processing system" or PS). Keep that purpose in mind if you choose to follow these steps.

The MiniZed is fairly new (as of starting this repository in Oct 2017), so there is still a limited number of targeted tutorials. Fortunately, there are lots of close-enough tutorials for other Zynq boards including older Avnet boards.

Since the Zynq combines hardware and software development and MiniZed is an introductory board, I figure there must be other mere mortals like myself using it. This documentation is written from that perspective. If you have tips, please contribute by issuing a pull request.

If you get stuck at a certain point in these steps or the examples, [create a new issue with the Documentation label](https://github.com/bmyerz/minized-examples/issues/new).

1. __Get started with the board, Vivado, and SDK__

Follow Avnet's [MiniZed tutorials 1-3](http://zedboard.org/support/design/18891/146) (or go to http://zedboard.org/ then go to Support | Reference designs and tutorials | MiniZed).

2. __Get the PS to talk to PL with existing IP__

At this point, you now understand how to create an HDF in Vivado using the block editor and use SDK to create and run applications. Next, get some more interesting PL to talk to the PS. Here is an example I adapted where a [C application reads and writes BRAM and issues DMA transfers](https://github.com/bmyerz/minized-examples/tree/master/dma).

3. __Get the PS to talk to PL with your own IP__

At this point, you have worked with an application where the PS talks to the PL, which included an existing AXI peripheral (BRAM). A good next step is to create an application where the software talks to some custom HDL. I adapted [this tutorial]( http://www.fpgadeveloper.com/2014/08/creating-a-custom-ip-block-in-vivado.html). Here are [my modications](https://github.com/bmyerz/minized-examples/tree/master/customip).

Note that this example is quite simple in that the IP is combinational logic only (other than AXI interface). I was able to get it to work the first time on the board without verification in simulation.

4. __Test the PS + PL in behavioral simulation__

Vivado includes a Zynq Verification IP (VIP). Basically, it will let verify your whole block design. In your testbenches you will issue PS commands to emulate what your PS would be doing. 

First, try out the Vivado example project by [following this video tutorial](https://www.xilinx.com/video/soc/how-to-use-the-zynq-7000-verification-ip-verify-debug-simulation.html).

Then, you can try the same thing with your own IP. I have an [example walkthrough with a resettable saturating counter](https://github.com/bmyerz/minized-examples/tree/master/axi_counter).

Another option to learn: when you create a new AXI4 peripheral, there as an option to "Verify with AXI VIP". This will create an example testbench for a AXI peripheral. I haven't edited the user logic yet.

For more information on the Zynq VIP's API, see its [documentation](https://www.xilinx.com/support/documentation/ip_documentation/processing_system7_vip/v1_0/ds940-zynq-vip.pdf). 


# Switched to Zybo-Z7 10

Here's a VGA pmod demo that besides demonstrating use of pmods also demonstrates using only the PL (so no SDK involvement)
https://reference.digilentinc.com/learn/programmable-logic/tutorials/zybo-z7-pmod-vga-demo/start
- TODO, use Digilent rgb2vga and create a pmod controller ip

PMOD IPs tutorial
https://reference.digilentinc.com/learn/programmable-logic/tutorials/pmod-ips/start
 - note that there is no PMOD VGA IP currently, but there is an rgb to VGA IP, so could make a PMOD vga ip to connect it to
 
 Easy connection of IOs, using the board tab. Rely on the IO constraints already provided by the board definition files
 - create block design
 - click board tab (same window as sources, design,...)
 - right click an IO and Connect board component


