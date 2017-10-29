In this example, we'll create an AXI peripheral IP, test in behavioral simulation everything except the AXI interface, then use it in a Zynq PS block design and test the system in behavioral.

Our AXI peripheral IP for this example is a user-resettable saturating counter. I picked this component because it is a simple sequential logic circuit that will provide both read and write interaction over AXI.

1. Create a new AXI4 peripheral IP. For sources you can add this file https://github.com/bmyerz/minized-examples/blob/master/axi_counter/axi_counter_ip_sources/Counter.v. Notice that it has an active low system reset and a user reset.

2. Add https://github.com/bmyerz/minized-examples/blob/master/axi_counter/axi_counter_ip_sources/testbench.v as a simulation source. This testbench only tests the Counter's functionality, not the AXI interface. I figure that instead of dealing with the AXI Verification IP, in the next step I'll just rely on the Zynq Verification IP that includes the AXI one.

3. Edit the \*S00_AXI.v file that Vivado generated for you to instantiate the Counter. I decided on the following register usage:

* slv_reg0: write 0x000000001 to assert user_reset, write 0x00000000 to de-assert user_reset
* slv_reg1: read to get the counter's value

See lines to edit:

https://github.com/bmyerz/minized-examples/blob/71babf5fec09c134f7f5f251685b97ca94e831db/axi_counter/axi_counter_ip_sources/Counter_v1_0_S00_AXI.v#L374

https://github.com/bmyerz/minized-examples/blob/71babf5fec09c134f7f5f251685b97ca94e831db/axi_counter/axi_counter_ip_sources/Counter_v1_0_S00_AXI.v#L400

4. Package the IP and close the Project

5. Create a new project. The block design should include Zynq PS. To add our Counter IP, right-click within the block diagram editor | IP Settings | IP | repositories. Navigate to the location of your IP. The default seems to be a folder inside of ip_repo/. Now when you click Add IP, you'll be able to see your Counter IP. 

6. Run connection automation. 

7. Add a simulation source. You can use this file https://github.com/bmyerz/minized-examples/blob/master/axi_counter/block_design_sources/tb.v. The testbench [issues a FPGA reset](https://github.com/bmyerz/minized-examples/blob/2bb6fc6a8dbd9806e405e38b4eb6df97918b1a9d/axi_counter/block_design_sources/tb.v#L48), then [reads the counter a couple times](https://github.com/bmyerz/minized-examples/blob/2bb6fc6a8dbd9806e405e38b4eb6df97918b1a9d/axi_counter/block_design_sources/tb.v#L53), then [does a user reset on the counter](https://github.com/bmyerz/minized-examples/blob/2bb6fc6a8dbd9806e405e38b4eb6df97918b1a9d/axi_counter/block_design_sources/tb.v#L72), then [reads it one more time](https://github.com/bmyerz/minized-examples/blob/2bb6fc6a8dbd9806e405e38b4eb6df97918b1a9d/axi_counter/block_design_sources/tb.v#L80).

For information on these commands, take a look at the [Zynq VIP documentation](https://www.xilinx.com/support/documentation/ip_documentation/processing_system7_vip/v1_0/ds940-zynq-vip.pdf).


8. Run behavioral simulation. The output looks something like the following (make sure you run it until completion).

```
XilinxAXIVIP: Found at Path: tb.zynq_sys.zynq_counter_i.processing_system7_0.inst.M_AXI_GP0.master
[0] : *ZYNQ_BFM_INFO : M_AXI_GP0 : Port is ENABLED.
XilinxAXIVIP: Found at Path: tb.zynq_sys.zynq_counter_i.processing_system7_0.inst.M_AXI_GP1.master
[0] : *ZYNQ_BFM_INFO : M_AXI_GP1 : Port is DISABLED.
XilinxAXIVIP: Found at Path: tb.zynq_sys.zynq_counter_i.processing_system7_0.inst.S_AXI_GP0.slave
[0] : *ZYNQ_BFM_INFO : S_AXI_GP0 : Port is DISABLED.
XilinxAXIVIP: Found at Path: tb.zynq_sys.zynq_counter_i.processing_system7_0.inst.S_AXI_GP1.slave
[0] : *ZYNQ_BFM_INFO : S_AXI_GP1 : Port is DISABLED.
XilinxAXIVIP: Found at Path: tb.zynq_sys.zynq_counter_i.processing_system7_0.inst.S_AXI_HP0.slave
[0] : *ZYNQ_BFM_INFO : S_AXI_HP0 : Port is DISABLED.
XilinxAXIVIP: Found at Path: tb.zynq_sys.zynq_counter_i.processing_system7_0.inst.S_AXI_HP1.slave
[0] : *ZYNQ_BFM_INFO : S_AXI_HP1 : Port is DISABLED.
XilinxAXIVIP: Found at Path: tb.zynq_sys.zynq_counter_i.processing_system7_0.inst.S_AXI_HP2.slave
[0] : *ZYNQ_BFM_INFO : S_AXI_HP2 : Port is DISABLED.
XilinxAXIVIP: Found at Path: tb.zynq_sys.zynq_counter_i.processing_system7_0.inst.S_AXI_HP3.slave
[0] : *ZYNQ_BFM_INFO : S_AXI_HP3 : Port is DISABLED.
XilinxAXIVIP: Found at Path: tb.zynq_sys.zynq_counter_i.processing_system7_0.inst.S_AXI_ACP.slave
[0] : *ZYNQ_BFM_INFO : S_AXI_ACP : Port is DISABLED.
running the tb
[150] : *ZYNQ_BFM_INFO : FPGA Soft Reset called for 0x1
[150] : *ZYNQ_BFM_INFO : FPGA Soft Reset called for 0x0
[250] : M_AXI_GP0 : *ZYNQ_BFM_INFO : Starting Address(0x43c00004) -> AXI Read -> 4 bytes
[1430] : M_AXI_GP0 : *ZYNQ_BFM_INFO : Done AXI Read for Starting Address(0x43c00004) with Response 'OKAY'
             1430000, running the testbench, data read from Counter was 32'h00000005
[2030] : M_AXI_GP0 : *ZYNQ_BFM_INFO : Starting Address(0x43c00004) -> AXI Read -> 4 bytes
[2210] : M_AXI_GP0 : *ZYNQ_BFM_INFO : Done AXI Read for Starting Address(0x43c00004) with Response 'OKAY'
             2210000, running the testbench, data read from Counter was 32'h0000002a
User resets the counter to 0
[2210] : M_AXI_GP0 : *ZYNQ_BFM_INFO : Starting Address(0x43c00000) -> AXI Write -> 4 bytes
[2770] : M_AXI_GP0 : *ZYNQ_BFM_INFO : Done AXI Write for Starting Address(0x43c00000) with Response 'OKAY'
[2870] : M_AXI_GP0 : *ZYNQ_BFM_INFO : Starting Address(0x43c00000) -> AXI Write -> 4 bytes
[3310] : M_AXI_GP0 : *ZYNQ_BFM_INFO : Done AXI Write for Starting Address(0x43c00000) with Response 'OKAY'
[3410] : M_AXI_GP0 : *ZYNQ_BFM_INFO : Starting Address(0x43c00004) -> AXI Read -> 4 bytes
[3590] : M_AXI_GP0 : *ZYNQ_BFM_INFO : Done AXI Read for Starting Address(0x43c00004) with Response 'OKAY'
             3590000, running the testbench, data read from Counter was 32'h0000000e
$finish called at time : 3590 ns : File "C:/Users/bdmyers/ZynqWithSaturatingCounter/ZynqWithSaturatingCounter.srcs/sim_1/new/tb.v" Line 88
```

9. From here you can Generate a bitstream and try it on the board with SDK.

TODO: add source for counter C code.
