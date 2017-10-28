`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/27/2017 06:53:08 PM
// Design Name: 
// Module Name: Multiplier
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

// THIS IS NOT A MULTIPLIER
// I am too lazy to go change the name.

module Multiplier(
    input [15:0] a,
    input [15:0] b,
    output [31:0] c
    );
    
    // add two 16bit numbers and extend with 1's
    assign c = {16'hffff, a + b}; 
endmodule
