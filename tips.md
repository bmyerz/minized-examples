# Tips and troubleshooting

## Vivado

### Extraneous bus interface when packaging IP

When packaging IP, see the clock with a weird symbol (looks like the AXI symbol; I think it means “bus interface”). Just right click and say “remove interface”

### Run block automation removes ports I wanted

double click the IP core and go find the port

## SDK

### How to reset the board without physical access or ability to power cycle the USB port

The following seems to be robust so far. Create a "Run configuration..." where you check all the steps, including System Reset and Program FPGA.

### Stdin/out choices

* coresight: uses the debugging console. I haven't used it enough to know how reliable it is
* ps7_uart1: uses the serial (COM) port. This has been the most reliable and predictable. If you don't want to use an external terminal program, use the SDK terminal.
