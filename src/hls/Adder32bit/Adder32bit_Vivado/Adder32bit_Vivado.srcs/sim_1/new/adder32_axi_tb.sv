`timescale 1ns/1ps

module adder32_axi_tb;

  // ------------------------------------------------------------
  // Clock & Reset
  // ------------------------------------------------------------
  logic clk = 0;
  logic reset = 1;   // ACTIVE HIGH

  always #5 clk = ~clk;   // 100 MHz

  initial begin
    reset = 1;
    #100;
    reset = 0;
  end

  // ------------------------------------------------------------
  // DUT
  // ------------------------------------------------------------
  Adder32bit_sim dut (
    .clk_100MHz(clk),
    .reset(reset)
  );

  // ------------------------------------------------------------
  // AXI VIP
  // ------------------------------------------------------------
  import axi_vip_pkg::*;
  import Adder32bit_sim_axi_vip_0_0_pkg::*;

  Adder32bit_sim_axi_vip_0_0_mst_t axi_mst;

  // ------------------------------------------------------------
  // Test variables (PHẢI KHAI BÁO Ở ĐÂY)
  // ------------------------------------------------------------
  logic [31:0] ctrl;
  logic [31:0] sum;

  // ------------------------------------------------------------
  // Test sequence
  // ------------------------------------------------------------
  initial begin
    // Chờ reset nhả
    wait (reset == 0);
    #20;

    // Init AXI master
    axi_mst = new(
      "axi_master",
      dut.Adder32bit_sim_axi_vip_0_0.inst.IF
    );
    axi_mst.start_master();

    // Write inputs
    axi_mst.write(32'h10, 32'd123);   // a
    axi_mst.write(32'h18, 32'd456);   // b

    // ap_start = 1
    axi_mst.write(32'h00, 32'h00000001);

    // Poll ap_done
    do begin
      axi_mst.read(32'h00, ctrl);
    end while (ctrl[1] == 1'b0);

    // Read result
    axi_mst.read(32'h20, sum);

    $display("=================================");
    $display("SIM RESULT: SUM = %0d", sum);
    $display("=================================");

    #100;
    $finish;
  end

endmodule
