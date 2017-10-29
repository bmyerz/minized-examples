`timescale 1ns / 1ps

module tb;
    reg tb_ACLK;
    reg tb_ARESETn;
   
    wire temp_clk;
    wire temp_rstn; 
   
    reg [31:0] read_data;
    reg [31:0] last_read;
    reg resp;
    

    initial 
    begin       
        tb_ACLK = 1'b0;
    end
    
    //------------------------------------------------------------------------
    // Simple Clock Generator
    //------------------------------------------------------------------------
    
    always #10 tb_ACLK = !tb_ACLK;
	
	initial 
    begin
	`ifndef XILINX_SIMULATOR
           tb.zynq_sys.zynq_counter_i.processing_system7_0.inst.M_AXI_GP0.master.IF.PC.fatal_to_warnings=1;
           #40;
           tb.zynq_sys.zynq_counter_i.processing_system7_0.inst.M_AXI_GP0.master.IF.PC.fatal_to_warnings=0;
	 `endif 		
    end
       
    initial
    begin
    
        $display ("running the tb");
        
        tb_ARESETn = 1'b0;
        repeat(2)@(posedge tb_ACLK);        
        tb_ARESETn = 1'b1;
        @(posedge tb_ACLK);
        
        repeat(5) @(posedge tb_ACLK);
          
        //Reset the PL
        tb.zynq_sys.zynq_counter_i.processing_system7_0.inst.fpga_soft_reset(32'h1);
        tb.zynq_sys.zynq_counter_i.processing_system7_0.inst.fpga_soft_reset(32'h0);

       repeat(5) @(posedge tb_ACLK);
       
        tb.zynq_sys.zynq_counter_i.processing_system7_0.inst.read_data(32'h43C00004,4,read_data,resp);
        // expect something larger than 5
        $display ("%t, running the testbench, data read from Counter was 32'h%x",$time, read_data);
        if (read_data < 5) begin
             $display("FAILED: expected data to be >= 5");
             $stop;
        end
        last_read = read_data;
        
        repeat(30) @(posedge tb_ACLK);
        tb.zynq_sys.zynq_counter_i.processing_system7_0.inst.read_data(32'h43C00004,4,read_data,resp);
        // expect previous + 30
        $display ("%t, running the testbench, data read from Counter was 32'h%x",$time, read_data);
        if (read_data <= last_read) begin
            $display("FAILED: expected data to be > %d", last_read);
            $stop;
        end
        last_read = read_data;

        // user counter reset
        $display("User resets the counter to 0");
       tb.zynq_sys.zynq_counter_i.processing_system7_0.inst.write_data(32'h43C00000,4, 32'h00000001, resp);
       repeat(5) @(posedge tb_ACLK);
       tb.zynq_sys.zynq_counter_i.processing_system7_0.inst.write_data(32'h43C00000,4, 32'h00000000, resp);
       
       repeat(5) @(posedge tb_ACLK);

        tb.zynq_sys.zynq_counter_i.processing_system7_0.inst.read_data(32'h43C00004,4,read_data,resp);
        // expect something smaller
        $display ("%t, running the testbench, data read from Counter was 32'h%x",$time, read_data);
        if (read_data >= last_read) begin
              $display("FAILED: expected data to be < %d", last_read);
              $stop;
        end

        $finish;
    end

    assign temp_clk = tb_ACLK;
    assign temp_rstn = tb_ARESETn;
   
zynq_counter_wrapper zynq_sys(
    .DDR_addr(),
    .DDR_ba(),
    .DDR_cas_n(),
    .DDR_ck_n(),
    .DDR_ck_p(),
    .DDR_cke(),
    .DDR_cs_n(),
    .DDR_dm(),
    .DDR_dq(),
    .DDR_dqs_n(),
    .DDR_dqs_p(),
    .DDR_odt(),
    .DDR_ras_n(),
    .DDR_reset_n(),
    .DDR_we_n(),
    .FIXED_IO_ddr_vrn(),
    .FIXED_IO_ddr_vrp(),
    .FIXED_IO_mio(),
    .FIXED_IO_ps_clk(temp_clk),
    .FIXED_IO_ps_porb(temp_rstn),
    .FIXED_IO_ps_srstb(temp_rstn)
    );

endmodule