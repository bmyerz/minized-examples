# Tips and troubleshooting

## Vivado

### Extraneous bus interface when packaging IP

When packaging IP, you might see a signal (such as the clock) with a bus interface symbol (looks like the same as the AXI symbol). Just right click and say “remove interface”

### Run block automation removes ports I wanted

double click the IP core and go find the port

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
