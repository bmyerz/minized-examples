# Tips and troubleshooting

## Vivado

### Inferred bus interface when packaging IP

When packaging IP, you might see a signal (such as the clock) with a bus interface symbol. This should be fine. If you don't see it, to mark clk as a clock input, you can go to auto-infer bus interface and find clock_rtl.

### IP Packager Reset active/high low

If you want your IP's reset signal to be marked as active high (helps with IP Integrator automation) then add a Parameter POLARITY with value ACTIVE_HIGH.

### IP Packager: [IP_Flow 19-3153] Bus Interface 'clk': ASSOCIATED_BUSIF bus parameter is missing.

If your IP is not an AXI peripheral, then you can ignore this. Just make sure when you wire up the IP in your design, that it gets the expected clock signal (I use FCLK_CLK0 by default).

If you want to see an example of how the ASSOCIATED_BUSIF parameter is set, you can create a new AXI4 Peripheral IP, then go to the IP packager, right click on clock or reset, IP Settings, Parameters.

### IP Packager: File Groups: source file is not relative to IP project

Right click on the source file in the sources window and choose "copy into project". The IP Packager's File Groups tab will detect the change.

### Where is my custom IP?

Right click in blank space of the block diagram editor | IP Settings | IP | Repository. Navigate to where your IP is. You can verify you got the right location because the pop-up (title is "Add repository") will show that it detected an IP.

### Run block automation removes ports I wanted

double click the IP core and go find the port

### Behavioral simulation error: NAME not found while processing module instance FILE_INSTANTIATING NAME

It seems that my IP didn't generate behavioral simulation output products. I haven't figured out how to get it to do that, but instead you could jump ahead to Functional post-synth simulation.


## SDK

### How to reset the board without physical access or ability to power cycle the USB port

The following seems to be robust so far. Create a "Run configuration..." where you check all the steps, including System Reset and Program FPGA.

### Stdin/out choices

Make your selection in the board support package (BSP) settings

* coresight: uses the debugging console. It appears to work fine for stdout; I haven't yet tried it for stdin.
* ps7_uart1: uses the serial (COM) port. I like that it is less magical than coresight, since serial is such a common interface. If you don't want to use an external terminal program like Tera Term, use the "SDK terminal".

### Reflect changes when you modify the HDF

I don't know if this is the best way to do it, but it works for me. 

I like to open SDK from within Vivado after exporting hardware. If your SDK project for the HDF already exists, SDK will automatically open your existing application projects. To use the new HDF, right click | refresh on the hardware platform folder. Then right-click your BSP and Re-generate BSP sources.

### Already connected to port COMx

You probably have a second SDK window open with a serial connection.
