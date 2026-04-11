//Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
//Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2024.2 (lin64) Build 5239630 Fri Nov 08 22:34:34 MST 2024
//Date        : Mon Jan 12 19:40:18 2026
//Host        : RimuruLenovo running 64-bit Ubuntu 24.04.3 LTS
//Command     : generate_target Adder32bit_sim_wrapper.bd
//Design      : Adder32bit_sim_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module Adder32bit_sim_wrapper
   (clk_100MHz,
    reset);
  input clk_100MHz;
  input reset;

  wire clk_100MHz;
  wire reset;

  Adder32bit_sim Adder32bit_sim_i
       (.clk_100MHz(clk_100MHz),
        .reset(reset));
endmodule
