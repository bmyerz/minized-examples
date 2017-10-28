# Tips and troubleshooting

## Vivado

### Extraneous bus interface when packaging IP

When packaging IP, you might see a signal (such as the clock) with a bus interface symbol. This should be fine. If you don't see it, to mark clk as a clock input, you can go to auto-infer bus interface and find clock_rtl.

### Run block automation removes ports I wanted

double click the IP core and go find the port

### Behavioral simulation error: NAME not found while processing module instance FILE_INSTANTIATING NAME

It seems that my IP didn't generate behavioral simulation output products. I haven't figured out how to get it to do that, but instead you could jump ahead to Functional post-synth simulation.

### How do I simulate PS + PL?

As far as I know you cannot. However, I think the most likely path is to use the AXI Verification IP. Send commands to your AXI peripheral.
https://www.xilinx.com/products/intellectual-property/axi-vip.html

## SDK

### How to reset the board without physical access or ability to power cycle the USB port

The following seems to be robust so far. Create a "Run configuration..." where you check all the steps, including System Reset and Program FPGA.

### Stdin/out choices

Make your selection in the board support package (BSP) settings

* coresight: uses the debugging console. I haven't used it enough to know how reliable it is
* ps7_uart1: uses the serial (COM) port. This has been the most reliable and predictable. If you don't want to use an external terminal program, use the SDK terminal.

### Reflect changes when you modify the HDF

I don't know if this is the best way to do it, but it works for me. 

I like to open SDK from within Vivado after exporting hardware. If your SDK project for the HDF already exists, SDK will automatically open your existing application projects. To use the new HDF, right click | refresh on the hardware platform folder. Then right-click your BSP and Re-generate BSP sources.

### Already connected to port COMx

You probably have a second SDK window open with a serial connection.
