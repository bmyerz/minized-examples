`timescale 1ns / 1ps


module testbench();
parameter HALF_PERIOD = 10;

reg areset, clk;

//  test 
initial begin
 user_reset <= 0;
 areset <= 0; #(HALF_PERIOD*4); areset <= 1;
 
 #(HALF_PERIOD*2 * 3);
 if (counter_dataout != 3) begin
         $display("failed: was %d expected %d", counter_dataout, 3);
         $finish;
     end
  #(HALF_PERIOD*2 * 20);
  if (counter_dataout != 15) begin
    $display("failed: was %d expected %d", counter_dataout, 15);
    $finish;
  end
  user_reset <= 1'b1;
  #(HALF_PERIOD*2 * 2);
  user_reset <= 1'b0;
  if (counter_dataout != 0 ) begin
    $display("failed: was %d expected %d", counter_dataout, 0);
      $finish;
   end
   #(HALF_PERIOD*2 * 7);
   if (counter_dataout != 7 ) begin
    $display("failed: was %d expected %d", counter_dataout, 7);
         $finish;
      end
   $display("success");
   $finish;
end

// generate clock to sequence tests 
initial begin 
 clk = 0;
 forever #(HALF_PERIOD) clk = ~clk; // generate a clock
end


	wire [3:0] counter_dataout;
	reg user_reset;
	
    Counter #(.WIDTH(4)) cntr(.areset(areset),
        .user_reset(user_reset),  // reg0 resets the counter
        .clk(clk),
        .dataout(counter_dataout)
        );
        



endmodule
