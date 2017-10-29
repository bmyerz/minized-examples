`timescale 1ns / 1ps

module Counter #(parameter WIDTH=32) (
    input areset,//active low
    input user_reset,
    input clk,
    output reg [WIDTH-1:0] dataout
    );
    
    always @(posedge clk) begin
        if (areset == 1'b0) dataout <= {WIDTH{1'b0}};
        else if (user_reset == 1'b1) dataout <= {WIDTH{1'b0}};
        else if (dataout != {WIDTH{1'b1}}) dataout <= dataout + 1; // saturate or increment
    end
endmodule
