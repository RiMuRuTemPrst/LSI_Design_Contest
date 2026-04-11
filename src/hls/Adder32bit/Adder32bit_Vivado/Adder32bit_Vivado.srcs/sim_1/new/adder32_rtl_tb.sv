`timescale 1ns/1ps

module adder32_rtl_tb;

  // --------------------------------------------------
  // Clock & Reset
  // --------------------------------------------------
  logic clk = 0;
  logic rst_n = 0;

  always #5 clk = ~clk;   // 100 MHz

  initial begin
    rst_n = 0;
    #50;
    rst_n = 1;
    $display("[TB] Reset released at %0t", $time);
  end

  // --------------------------------------------------
  // AXI-Lite signals
  // --------------------------------------------------
  logic [5:0]  awaddr;
  logic        awvalid;
  logic        awready;

  logic [31:0] wdata;
  logic [3:0]  wstrb;
  logic        wvalid;
  logic        wready;

  logic [1:0]  bresp;
  logic        bvalid;
  logic        bready;

  logic [5:0]  araddr;
  logic        arvalid;
  logic        arready;

  logic [31:0] rdata;
  logic [1:0]  rresp;
  logic        rvalid;
  logic        rready;

  // --------------------------------------------------
  // Test variables (PHẢI KHAI BÁO Ở CẤP MODULE)
  // --------------------------------------------------
  logic [31:0] ctrl;
  logic [31:0] sum;

  // --------------------------------------------------
  // DUT : HLS IP
  // --------------------------------------------------
  adder32 dut (
    .ap_clk(clk),
    .ap_rst_n(rst_n),

    .s_axi_CTRL_AWADDR (awaddr),
    .s_axi_CTRL_AWVALID(awvalid),
    .s_axi_CTRL_AWREADY(awready),

    .s_axi_CTRL_WDATA  (wdata),
    .s_axi_CTRL_WSTRB  (wstrb),
    .s_axi_CTRL_WVALID (wvalid),
    .s_axi_CTRL_WREADY (wready),

    .s_axi_CTRL_BRESP  (bresp),
    .s_axi_CTRL_BVALID (bvalid),
    .s_axi_CTRL_BREADY (bready),

    .s_axi_CTRL_ARADDR (araddr),
    .s_axi_CTRL_ARVALID(arvalid),
    .s_axi_CTRL_ARREADY(arready),

    .s_axi_CTRL_RDATA  (rdata),
    .s_axi_CTRL_RRESP  (rresp),
    .s_axi_CTRL_RVALID (rvalid),
    .s_axi_CTRL_RREADY (rready)
  );

  // --------------------------------------------------
  // Test sequence
  // --------------------------------------------------
  initial begin
    // default values
    awvalid = 0;
    wvalid  = 0;
    bready  = 1;
    arvalid = 0;
    rready  = 1;
    wstrb   = 4'hF;

    // wait reset release
    wait (rst_n == 1);
    #20;

    // ------------------------------------------------
    // Write operands
    // ------------------------------------------------
    axi_write(6'h10, 32'd10);   // a = 10
    $display("[TB] Write A done");

    axi_write(6'h18, 32'd20);   // b = 20
    $display("[TB] Write B done");

    // ------------------------------------------------
    // Start IP
    // ------------------------------------------------
    axi_write(6'h00, 32'h00000001); // ap_start
    $display("[TB] ap_start written");

    // ------------------------------------------------
    // Poll ap_done
    // ------------------------------------------------
    do begin
      axi_read(6'h00, ctrl);
      $display("[TB] CTRL = %h at %0t", ctrl, $time);
    end while (ctrl[1] == 1'b0);   // bit1 = ap_done

    // ------------------------------------------------
    // Read result
    // ------------------------------------------------
    axi_read(6'h20, sum);

    $display("=================================");
    $display("SIM RESULT: SUM = %0d", sum); // expect 30
    $display("=================================");

    #100;
    $finish;
  end

  // --------------------------------------------------
  // AXI-Lite helper tasks (HANDSHAKE ĐÚNG)
  // --------------------------------------------------
  task axi_write(input [5:0] addr, input [31:0] data);
    begin
      // Address channel
      awaddr  = addr;
      awvalid = 1;
      wait (awready);
      awvalid = 0;

      // Write data channel
      wdata   = data;
      wvalid  = 1;
      wait (wready);
      wvalid  = 0;

      // Write response
      wait (bvalid);
    end
  endtask

  task axi_read(input [5:0] addr, output [31:0] data);
    begin
      araddr  = addr;
      arvalid = 1;
      wait (arready);
      arvalid = 0;

      wait (rvalid);
      data = rdata;
    end
  endtask

endmodule
