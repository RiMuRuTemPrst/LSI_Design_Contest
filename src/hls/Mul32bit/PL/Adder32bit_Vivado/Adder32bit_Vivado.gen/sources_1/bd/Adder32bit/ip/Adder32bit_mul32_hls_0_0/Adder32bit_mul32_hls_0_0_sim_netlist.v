// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2024.2 (lin64) Build 5239630 Fri Nov 08 22:34:34 MST 2024
// Date        : Wed Jan 14 18:22:34 2026
// Host        : RimuruLenovo running 64-bit Ubuntu 24.04.3 LTS
// Command     : write_verilog -force -mode funcsim
//               /home/rimurutempest/Code/LSI_Design_Contest/HLS/Mul32bit/PL/Adder32bit_Vivado/Adder32bit_Vivado.gen/sources_1/bd/Adder32bit/ip/Adder32bit_mul32_hls_0_0/Adder32bit_mul32_hls_0_0_sim_netlist.v
// Design      : Adder32bit_mul32_hls_0_0
// Purpose     : This verilog netlist is a functional simulation representation of the design and should not be modified
//               or synthesized. This netlist cannot be used for SDF annotated simulation.
// Device      : xczu7ev-ffvc1156-2-e
// --------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

(* CHECK_LICENSE_TYPE = "Adder32bit_mul32_hls_0_0,mul32_hls,{}" *) (* DowngradeIPIdentifiedWarnings = "yes" *) (* IP_DEFINITION_SOURCE = "HLS" *) 
(* X_CORE_INFO = "mul32_hls,Vivado 2024.2" *) (* hls_module = "yes" *) 
(* NotValidForBitStream *)
module Adder32bit_mul32_hls_0_0
   (s_axi_CTRL_ARADDR,
    s_axi_CTRL_ARREADY,
    s_axi_CTRL_ARVALID,
    s_axi_CTRL_AWADDR,
    s_axi_CTRL_AWREADY,
    s_axi_CTRL_AWVALID,
    s_axi_CTRL_BREADY,
    s_axi_CTRL_BRESP,
    s_axi_CTRL_BVALID,
    s_axi_CTRL_RDATA,
    s_axi_CTRL_RREADY,
    s_axi_CTRL_RRESP,
    s_axi_CTRL_RVALID,
    s_axi_CTRL_WDATA,
    s_axi_CTRL_WREADY,
    s_axi_CTRL_WSTRB,
    s_axi_CTRL_WVALID,
    ap_clk,
    ap_rst_n,
    interrupt);
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 s_axi_CTRL ARADDR" *) (* X_INTERFACE_MODE = "slave" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME s_axi_CTRL, ADDR_WIDTH 6, DATA_WIDTH 32, PROTOCOL AXI4LITE, READ_WRITE_MODE READ_WRITE, FREQ_HZ 100000000, ID_WIDTH 0, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, HAS_BURST 0, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 1, NUM_WRITE_OUTSTANDING 1, MAX_BURST_LENGTH 1, PHASE 0.0, CLK_DOMAIN Adder32bit_zynq_ultra_ps_e_0_0_pl_clk0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *) input [5:0]s_axi_CTRL_ARADDR;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 s_axi_CTRL ARREADY" *) output s_axi_CTRL_ARREADY;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 s_axi_CTRL ARVALID" *) input s_axi_CTRL_ARVALID;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 s_axi_CTRL AWADDR" *) input [5:0]s_axi_CTRL_AWADDR;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 s_axi_CTRL AWREADY" *) output s_axi_CTRL_AWREADY;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 s_axi_CTRL AWVALID" *) input s_axi_CTRL_AWVALID;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 s_axi_CTRL BREADY" *) input s_axi_CTRL_BREADY;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 s_axi_CTRL BRESP" *) output [1:0]s_axi_CTRL_BRESP;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 s_axi_CTRL BVALID" *) output s_axi_CTRL_BVALID;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 s_axi_CTRL RDATA" *) output [31:0]s_axi_CTRL_RDATA;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 s_axi_CTRL RREADY" *) input s_axi_CTRL_RREADY;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 s_axi_CTRL RRESP" *) output [1:0]s_axi_CTRL_RRESP;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 s_axi_CTRL RVALID" *) output s_axi_CTRL_RVALID;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 s_axi_CTRL WDATA" *) input [31:0]s_axi_CTRL_WDATA;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 s_axi_CTRL WREADY" *) output s_axi_CTRL_WREADY;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 s_axi_CTRL WSTRB" *) input [3:0]s_axi_CTRL_WSTRB;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 s_axi_CTRL WVALID" *) input s_axi_CTRL_WVALID;
  (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 ap_clk CLK" *) (* X_INTERFACE_MODE = "slave" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME ap_clk, ASSOCIATED_BUSIF s_axi_CTRL, ASSOCIATED_RESET ap_rst_n, FREQ_HZ 100000000, FREQ_TOLERANCE_HZ 0, PHASE 0.0, CLK_DOMAIN Adder32bit_zynq_ultra_ps_e_0_0_pl_clk0, INSERT_VIP 0" *) input ap_clk;
  (* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 ap_rst_n RST" *) (* X_INTERFACE_MODE = "slave" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME ap_rst_n, POLARITY ACTIVE_LOW, INSERT_VIP 0" *) input ap_rst_n;
  (* X_INTERFACE_INFO = "xilinx.com:signal:interrupt:1.0 interrupt INTERRUPT" *) (* X_INTERFACE_MODE = "master" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME interrupt, SENSITIVITY LEVEL_HIGH, PortWidth 1" *) output interrupt;

  wire \<const0> ;
  wire ap_clk;
  wire ap_rst_n;
  wire interrupt;
  wire [5:0]s_axi_CTRL_ARADDR;
  wire s_axi_CTRL_ARREADY;
  wire s_axi_CTRL_ARVALID;
  wire [5:0]s_axi_CTRL_AWADDR;
  wire s_axi_CTRL_AWREADY;
  wire s_axi_CTRL_AWVALID;
  wire s_axi_CTRL_BREADY;
  wire s_axi_CTRL_BVALID;
  wire [31:0]s_axi_CTRL_RDATA;
  wire s_axi_CTRL_RREADY;
  wire s_axi_CTRL_RVALID;
  wire [31:0]s_axi_CTRL_WDATA;
  wire s_axi_CTRL_WREADY;
  wire [3:0]s_axi_CTRL_WSTRB;
  wire s_axi_CTRL_WVALID;
  wire [1:0]NLW_inst_s_axi_CTRL_BRESP_UNCONNECTED;
  wire [1:0]NLW_inst_s_axi_CTRL_RRESP_UNCONNECTED;

  assign s_axi_CTRL_BRESP[1] = \<const0> ;
  assign s_axi_CTRL_BRESP[0] = \<const0> ;
  assign s_axi_CTRL_RRESP[1] = \<const0> ;
  assign s_axi_CTRL_RRESP[0] = \<const0> ;
  GND GND
       (.G(\<const0> ));
  (* C_S_AXI_CTRL_ADDR_WIDTH = "6" *) 
  (* C_S_AXI_CTRL_DATA_WIDTH = "32" *) 
  (* C_S_AXI_CTRL_WSTRB_WIDTH = "4" *) 
  (* C_S_AXI_DATA_WIDTH = "32" *) 
  (* C_S_AXI_WSTRB_WIDTH = "4" *) 
  (* SDX_KERNEL = "true" *) 
  (* SDX_KERNEL_SYNTH_INST = "inst" *) 
  (* SDX_KERNEL_TYPE = "hls" *) 
  Adder32bit_mul32_hls_0_0_mul32_hls inst
       (.ap_clk(ap_clk),
        .ap_rst_n(ap_rst_n),
        .interrupt(interrupt),
        .s_axi_CTRL_ARADDR(s_axi_CTRL_ARADDR),
        .s_axi_CTRL_ARREADY(s_axi_CTRL_ARREADY),
        .s_axi_CTRL_ARVALID(s_axi_CTRL_ARVALID),
        .s_axi_CTRL_AWADDR({s_axi_CTRL_AWADDR[5:2],1'b0,1'b0}),
        .s_axi_CTRL_AWREADY(s_axi_CTRL_AWREADY),
        .s_axi_CTRL_AWVALID(s_axi_CTRL_AWVALID),
        .s_axi_CTRL_BREADY(s_axi_CTRL_BREADY),
        .s_axi_CTRL_BRESP(NLW_inst_s_axi_CTRL_BRESP_UNCONNECTED[1:0]),
        .s_axi_CTRL_BVALID(s_axi_CTRL_BVALID),
        .s_axi_CTRL_RDATA(s_axi_CTRL_RDATA),
        .s_axi_CTRL_RREADY(s_axi_CTRL_RREADY),
        .s_axi_CTRL_RRESP(NLW_inst_s_axi_CTRL_RRESP_UNCONNECTED[1:0]),
        .s_axi_CTRL_RVALID(s_axi_CTRL_RVALID),
        .s_axi_CTRL_WDATA(s_axi_CTRL_WDATA),
        .s_axi_CTRL_WREADY(s_axi_CTRL_WREADY),
        .s_axi_CTRL_WSTRB(s_axi_CTRL_WSTRB),
        .s_axi_CTRL_WVALID(s_axi_CTRL_WVALID));
endmodule

(* C_S_AXI_CTRL_ADDR_WIDTH = "6" *) (* C_S_AXI_CTRL_DATA_WIDTH = "32" *) (* C_S_AXI_CTRL_WSTRB_WIDTH = "4" *) 
(* C_S_AXI_DATA_WIDTH = "32" *) (* C_S_AXI_WSTRB_WIDTH = "4" *) (* ORIG_REF_NAME = "mul32_hls" *) 
(* hls_module = "yes" *) 
module Adder32bit_mul32_hls_0_0_mul32_hls
   (s_axi_CTRL_AWVALID,
    s_axi_CTRL_AWREADY,
    s_axi_CTRL_AWADDR,
    s_axi_CTRL_WVALID,
    s_axi_CTRL_WREADY,
    s_axi_CTRL_WDATA,
    s_axi_CTRL_WSTRB,
    s_axi_CTRL_ARVALID,
    s_axi_CTRL_ARREADY,
    s_axi_CTRL_ARADDR,
    s_axi_CTRL_RVALID,
    s_axi_CTRL_RREADY,
    s_axi_CTRL_RDATA,
    s_axi_CTRL_RRESP,
    s_axi_CTRL_BVALID,
    s_axi_CTRL_BREADY,
    s_axi_CTRL_BRESP,
    ap_clk,
    ap_rst_n,
    interrupt);
  input s_axi_CTRL_AWVALID;
  output s_axi_CTRL_AWREADY;
  input [5:0]s_axi_CTRL_AWADDR;
  input s_axi_CTRL_WVALID;
  output s_axi_CTRL_WREADY;
  input [31:0]s_axi_CTRL_WDATA;
  input [3:0]s_axi_CTRL_WSTRB;
  input s_axi_CTRL_ARVALID;
  output s_axi_CTRL_ARREADY;
  input [5:0]s_axi_CTRL_ARADDR;
  output s_axi_CTRL_RVALID;
  input s_axi_CTRL_RREADY;
  output [31:0]s_axi_CTRL_RDATA;
  output [1:0]s_axi_CTRL_RRESP;
  output s_axi_CTRL_BVALID;
  input s_axi_CTRL_BREADY;
  output [1:0]s_axi_CTRL_BRESP;
  input ap_clk;
  input ap_rst_n;
  output interrupt;

  wire \<const0> ;
  wire CTRL_s_axi_U_n_102;
  wire CTRL_s_axi_U_n_103;
  wire CTRL_s_axi_U_n_104;
  wire CTRL_s_axi_U_n_105;
  wire CTRL_s_axi_U_n_106;
  wire CTRL_s_axi_U_n_107;
  wire CTRL_s_axi_U_n_108;
  wire CTRL_s_axi_U_n_109;
  wire CTRL_s_axi_U_n_112;
  wire CTRL_s_axi_U_n_113;
  wire CTRL_s_axi_U_n_114;
  wire CTRL_s_axi_U_n_115;
  wire CTRL_s_axi_U_n_116;
  wire CTRL_s_axi_U_n_117;
  wire CTRL_s_axi_U_n_118;
  wire CTRL_s_axi_U_n_119;
  wire CTRL_s_axi_U_n_120;
  wire CTRL_s_axi_U_n_121;
  wire CTRL_s_axi_U_n_122;
  wire CTRL_s_axi_U_n_123;
  wire CTRL_s_axi_U_n_124;
  wire CTRL_s_axi_U_n_125;
  wire CTRL_s_axi_U_n_126;
  wire CTRL_s_axi_U_n_127;
  wire CTRL_s_axi_U_n_128;
  wire CTRL_s_axi_U_n_129;
  wire CTRL_s_axi_U_n_130;
  wire CTRL_s_axi_U_n_131;
  wire CTRL_s_axi_U_n_132;
  wire CTRL_s_axi_U_n_133;
  wire CTRL_s_axi_U_n_134;
  wire CTRL_s_axi_U_n_135;
  wire CTRL_s_axi_U_n_136;
  wire CTRL_s_axi_U_n_137;
  wire CTRL_s_axi_U_n_138;
  wire CTRL_s_axi_U_n_139;
  wire CTRL_s_axi_U_n_140;
  wire CTRL_s_axi_U_n_141;
  wire CTRL_s_axi_U_n_142;
  wire CTRL_s_axi_U_n_143;
  wire CTRL_s_axi_U_n_144;
  wire CTRL_s_axi_U_n_145;
  wire CTRL_s_axi_U_n_146;
  wire CTRL_s_axi_U_n_147;
  wire CTRL_s_axi_U_n_148;
  wire CTRL_s_axi_U_n_149;
  wire CTRL_s_axi_U_n_150;
  wire CTRL_s_axi_U_n_151;
  wire CTRL_s_axi_U_n_152;
  wire CTRL_s_axi_U_n_153;
  wire CTRL_s_axi_U_n_154;
  wire CTRL_s_axi_U_n_155;
  wire CTRL_s_axi_U_n_156;
  wire CTRL_s_axi_U_n_157;
  wire CTRL_s_axi_U_n_158;
  wire CTRL_s_axi_U_n_159;
  wire CTRL_s_axi_U_n_160;
  wire CTRL_s_axi_U_n_161;
  wire CTRL_s_axi_U_n_162;
  wire CTRL_s_axi_U_n_163;
  wire CTRL_s_axi_U_n_164;
  wire CTRL_s_axi_U_n_165;
  wire CTRL_s_axi_U_n_166;
  wire CTRL_s_axi_U_n_2;
  wire CTRL_s_axi_U_n_3;
  wire CTRL_s_axi_U_n_53;
  wire CTRL_s_axi_U_n_54;
  wire CTRL_s_axi_U_n_55;
  wire CTRL_s_axi_U_n_56;
  wire CTRL_s_axi_U_n_57;
  wire CTRL_s_axi_U_n_58;
  wire CTRL_s_axi_U_n_59;
  wire CTRL_s_axi_U_n_60;
  wire CTRL_s_axi_U_n_61;
  wire CTRL_s_axi_U_n_62;
  wire CTRL_s_axi_U_n_63;
  wire CTRL_s_axi_U_n_64;
  wire CTRL_s_axi_U_n_65;
  wire CTRL_s_axi_U_n_66;
  wire CTRL_s_axi_U_n_67;
  wire CTRL_s_axi_U_n_68;
  wire CTRL_s_axi_U_n_69;
  wire CTRL_s_axi_U_n_70;
  wire CTRL_s_axi_U_n_71;
  wire CTRL_s_axi_U_n_72;
  wire CTRL_s_axi_U_n_73;
  wire CTRL_s_axi_U_n_74;
  wire CTRL_s_axi_U_n_75;
  wire CTRL_s_axi_U_n_76;
  wire CTRL_s_axi_U_n_77;
  wire CTRL_s_axi_U_n_78;
  wire CTRL_s_axi_U_n_79;
  wire CTRL_s_axi_U_n_80;
  wire CTRL_s_axi_U_n_81;
  wire CTRL_s_axi_U_n_82;
  wire CTRL_s_axi_U_n_83;
  wire CTRL_s_axi_U_n_84;
  wire CTRL_s_axi_U_n_85;
  wire CTRL_s_axi_U_n_86;
  wire CTRL_s_axi_U_n_87;
  wire CTRL_s_axi_U_n_88;
  wire CTRL_s_axi_U_n_89;
  wire CTRL_s_axi_U_n_90;
  wire CTRL_s_axi_U_n_91;
  wire CTRL_s_axi_U_n_92;
  wire CTRL_s_axi_U_n_93;
  wire CTRL_s_axi_U_n_94;
  wire CTRL_s_axi_U_n_95;
  wire CTRL_s_axi_U_n_96;
  wire CTRL_s_axi_U_n_97;
  wire CTRL_s_axi_U_n_98;
  wire ap_clk;
  wire ap_rst_n;
  wire ap_rst_n_inv;
  wire [31:0]data7;
  wire [16:0]int_a0;
  wire [31:0]int_b0;
  wire interrupt;
  wire mul_32ns_32ns_64_1_1_U1_n_0;
  wire mul_32ns_32ns_64_1_1_U1_n_1;
  wire mul_32ns_32ns_64_1_1_U1_n_10;
  wire mul_32ns_32ns_64_1_1_U1_n_100;
  wire mul_32ns_32ns_64_1_1_U1_n_101;
  wire mul_32ns_32ns_64_1_1_U1_n_102;
  wire mul_32ns_32ns_64_1_1_U1_n_103;
  wire mul_32ns_32ns_64_1_1_U1_n_104;
  wire mul_32ns_32ns_64_1_1_U1_n_105;
  wire mul_32ns_32ns_64_1_1_U1_n_106;
  wire mul_32ns_32ns_64_1_1_U1_n_107;
  wire mul_32ns_32ns_64_1_1_U1_n_108;
  wire mul_32ns_32ns_64_1_1_U1_n_109;
  wire mul_32ns_32ns_64_1_1_U1_n_11;
  wire mul_32ns_32ns_64_1_1_U1_n_110;
  wire mul_32ns_32ns_64_1_1_U1_n_111;
  wire mul_32ns_32ns_64_1_1_U1_n_112;
  wire mul_32ns_32ns_64_1_1_U1_n_113;
  wire mul_32ns_32ns_64_1_1_U1_n_114;
  wire mul_32ns_32ns_64_1_1_U1_n_115;
  wire mul_32ns_32ns_64_1_1_U1_n_116;
  wire mul_32ns_32ns_64_1_1_U1_n_117;
  wire mul_32ns_32ns_64_1_1_U1_n_118;
  wire mul_32ns_32ns_64_1_1_U1_n_119;
  wire mul_32ns_32ns_64_1_1_U1_n_12;
  wire mul_32ns_32ns_64_1_1_U1_n_120;
  wire mul_32ns_32ns_64_1_1_U1_n_121;
  wire mul_32ns_32ns_64_1_1_U1_n_122;
  wire mul_32ns_32ns_64_1_1_U1_n_123;
  wire mul_32ns_32ns_64_1_1_U1_n_124;
  wire mul_32ns_32ns_64_1_1_U1_n_125;
  wire mul_32ns_32ns_64_1_1_U1_n_126;
  wire mul_32ns_32ns_64_1_1_U1_n_127;
  wire mul_32ns_32ns_64_1_1_U1_n_128;
  wire mul_32ns_32ns_64_1_1_U1_n_129;
  wire mul_32ns_32ns_64_1_1_U1_n_13;
  wire mul_32ns_32ns_64_1_1_U1_n_130;
  wire mul_32ns_32ns_64_1_1_U1_n_131;
  wire mul_32ns_32ns_64_1_1_U1_n_132;
  wire mul_32ns_32ns_64_1_1_U1_n_133;
  wire mul_32ns_32ns_64_1_1_U1_n_134;
  wire mul_32ns_32ns_64_1_1_U1_n_135;
  wire mul_32ns_32ns_64_1_1_U1_n_136;
  wire mul_32ns_32ns_64_1_1_U1_n_137;
  wire mul_32ns_32ns_64_1_1_U1_n_138;
  wire mul_32ns_32ns_64_1_1_U1_n_139;
  wire mul_32ns_32ns_64_1_1_U1_n_14;
  wire mul_32ns_32ns_64_1_1_U1_n_140;
  wire mul_32ns_32ns_64_1_1_U1_n_141;
  wire mul_32ns_32ns_64_1_1_U1_n_142;
  wire mul_32ns_32ns_64_1_1_U1_n_143;
  wire mul_32ns_32ns_64_1_1_U1_n_144;
  wire mul_32ns_32ns_64_1_1_U1_n_145;
  wire mul_32ns_32ns_64_1_1_U1_n_15;
  wire mul_32ns_32ns_64_1_1_U1_n_155;
  wire mul_32ns_32ns_64_1_1_U1_n_156;
  wire mul_32ns_32ns_64_1_1_U1_n_157;
  wire mul_32ns_32ns_64_1_1_U1_n_158;
  wire mul_32ns_32ns_64_1_1_U1_n_159;
  wire mul_32ns_32ns_64_1_1_U1_n_16;
  wire mul_32ns_32ns_64_1_1_U1_n_160;
  wire mul_32ns_32ns_64_1_1_U1_n_161;
  wire mul_32ns_32ns_64_1_1_U1_n_162;
  wire mul_32ns_32ns_64_1_1_U1_n_163;
  wire mul_32ns_32ns_64_1_1_U1_n_164;
  wire mul_32ns_32ns_64_1_1_U1_n_165;
  wire mul_32ns_32ns_64_1_1_U1_n_166;
  wire mul_32ns_32ns_64_1_1_U1_n_167;
  wire mul_32ns_32ns_64_1_1_U1_n_168;
  wire mul_32ns_32ns_64_1_1_U1_n_169;
  wire mul_32ns_32ns_64_1_1_U1_n_17;
  wire mul_32ns_32ns_64_1_1_U1_n_170;
  wire mul_32ns_32ns_64_1_1_U1_n_171;
  wire mul_32ns_32ns_64_1_1_U1_n_172;
  wire mul_32ns_32ns_64_1_1_U1_n_173;
  wire mul_32ns_32ns_64_1_1_U1_n_174;
  wire mul_32ns_32ns_64_1_1_U1_n_175;
  wire mul_32ns_32ns_64_1_1_U1_n_176;
  wire mul_32ns_32ns_64_1_1_U1_n_177;
  wire mul_32ns_32ns_64_1_1_U1_n_18;
  wire mul_32ns_32ns_64_1_1_U1_n_19;
  wire mul_32ns_32ns_64_1_1_U1_n_2;
  wire mul_32ns_32ns_64_1_1_U1_n_20;
  wire mul_32ns_32ns_64_1_1_U1_n_21;
  wire mul_32ns_32ns_64_1_1_U1_n_22;
  wire mul_32ns_32ns_64_1_1_U1_n_23;
  wire mul_32ns_32ns_64_1_1_U1_n_24;
  wire mul_32ns_32ns_64_1_1_U1_n_25;
  wire mul_32ns_32ns_64_1_1_U1_n_26;
  wire mul_32ns_32ns_64_1_1_U1_n_27;
  wire mul_32ns_32ns_64_1_1_U1_n_28;
  wire mul_32ns_32ns_64_1_1_U1_n_29;
  wire mul_32ns_32ns_64_1_1_U1_n_3;
  wire mul_32ns_32ns_64_1_1_U1_n_30;
  wire mul_32ns_32ns_64_1_1_U1_n_31;
  wire mul_32ns_32ns_64_1_1_U1_n_32;
  wire mul_32ns_32ns_64_1_1_U1_n_33;
  wire mul_32ns_32ns_64_1_1_U1_n_34;
  wire mul_32ns_32ns_64_1_1_U1_n_35;
  wire mul_32ns_32ns_64_1_1_U1_n_36;
  wire mul_32ns_32ns_64_1_1_U1_n_37;
  wire mul_32ns_32ns_64_1_1_U1_n_38;
  wire mul_32ns_32ns_64_1_1_U1_n_39;
  wire mul_32ns_32ns_64_1_1_U1_n_4;
  wire mul_32ns_32ns_64_1_1_U1_n_40;
  wire mul_32ns_32ns_64_1_1_U1_n_41;
  wire mul_32ns_32ns_64_1_1_U1_n_42;
  wire mul_32ns_32ns_64_1_1_U1_n_43;
  wire mul_32ns_32ns_64_1_1_U1_n_44;
  wire mul_32ns_32ns_64_1_1_U1_n_45;
  wire mul_32ns_32ns_64_1_1_U1_n_46;
  wire mul_32ns_32ns_64_1_1_U1_n_47;
  wire mul_32ns_32ns_64_1_1_U1_n_48;
  wire mul_32ns_32ns_64_1_1_U1_n_49;
  wire mul_32ns_32ns_64_1_1_U1_n_5;
  wire mul_32ns_32ns_64_1_1_U1_n_50;
  wire mul_32ns_32ns_64_1_1_U1_n_51;
  wire mul_32ns_32ns_64_1_1_U1_n_52;
  wire mul_32ns_32ns_64_1_1_U1_n_53;
  wire mul_32ns_32ns_64_1_1_U1_n_54;
  wire mul_32ns_32ns_64_1_1_U1_n_55;
  wire mul_32ns_32ns_64_1_1_U1_n_56;
  wire mul_32ns_32ns_64_1_1_U1_n_57;
  wire mul_32ns_32ns_64_1_1_U1_n_58;
  wire mul_32ns_32ns_64_1_1_U1_n_59;
  wire mul_32ns_32ns_64_1_1_U1_n_6;
  wire mul_32ns_32ns_64_1_1_U1_n_60;
  wire mul_32ns_32ns_64_1_1_U1_n_61;
  wire mul_32ns_32ns_64_1_1_U1_n_62;
  wire mul_32ns_32ns_64_1_1_U1_n_63;
  wire mul_32ns_32ns_64_1_1_U1_n_64;
  wire mul_32ns_32ns_64_1_1_U1_n_65;
  wire mul_32ns_32ns_64_1_1_U1_n_66;
  wire mul_32ns_32ns_64_1_1_U1_n_67;
  wire mul_32ns_32ns_64_1_1_U1_n_68;
  wire mul_32ns_32ns_64_1_1_U1_n_69;
  wire mul_32ns_32ns_64_1_1_U1_n_7;
  wire mul_32ns_32ns_64_1_1_U1_n_70;
  wire mul_32ns_32ns_64_1_1_U1_n_71;
  wire mul_32ns_32ns_64_1_1_U1_n_72;
  wire mul_32ns_32ns_64_1_1_U1_n_73;
  wire mul_32ns_32ns_64_1_1_U1_n_74;
  wire mul_32ns_32ns_64_1_1_U1_n_75;
  wire mul_32ns_32ns_64_1_1_U1_n_76;
  wire mul_32ns_32ns_64_1_1_U1_n_77;
  wire mul_32ns_32ns_64_1_1_U1_n_78;
  wire mul_32ns_32ns_64_1_1_U1_n_79;
  wire mul_32ns_32ns_64_1_1_U1_n_8;
  wire mul_32ns_32ns_64_1_1_U1_n_80;
  wire mul_32ns_32ns_64_1_1_U1_n_81;
  wire mul_32ns_32ns_64_1_1_U1_n_82;
  wire mul_32ns_32ns_64_1_1_U1_n_83;
  wire mul_32ns_32ns_64_1_1_U1_n_84;
  wire mul_32ns_32ns_64_1_1_U1_n_85;
  wire mul_32ns_32ns_64_1_1_U1_n_86;
  wire mul_32ns_32ns_64_1_1_U1_n_87;
  wire mul_32ns_32ns_64_1_1_U1_n_88;
  wire mul_32ns_32ns_64_1_1_U1_n_89;
  wire mul_32ns_32ns_64_1_1_U1_n_9;
  wire mul_32ns_32ns_64_1_1_U1_n_90;
  wire mul_32ns_32ns_64_1_1_U1_n_91;
  wire mul_32ns_32ns_64_1_1_U1_n_92;
  wire mul_32ns_32ns_64_1_1_U1_n_93;
  wire mul_32ns_32ns_64_1_1_U1_n_94;
  wire mul_32ns_32ns_64_1_1_U1_n_95;
  wire mul_32ns_32ns_64_1_1_U1_n_96;
  wire mul_32ns_32ns_64_1_1_U1_n_97;
  wire mul_32ns_32ns_64_1_1_U1_n_98;
  wire mul_32ns_32ns_64_1_1_U1_n_99;
  wire [5:0]s_axi_CTRL_ARADDR;
  wire s_axi_CTRL_ARREADY;
  wire s_axi_CTRL_ARVALID;
  wire [5:0]s_axi_CTRL_AWADDR;
  wire s_axi_CTRL_AWREADY;
  wire s_axi_CTRL_AWVALID;
  wire s_axi_CTRL_BREADY;
  wire s_axi_CTRL_BVALID;
  wire [31:0]s_axi_CTRL_RDATA;
  wire s_axi_CTRL_RREADY;
  wire s_axi_CTRL_RVALID;
  wire [31:0]s_axi_CTRL_WDATA;
  wire s_axi_CTRL_WREADY;
  wire [3:0]s_axi_CTRL_WSTRB;
  wire s_axi_CTRL_WVALID;

  assign s_axi_CTRL_BRESP[1] = \<const0> ;
  assign s_axi_CTRL_BRESP[0] = \<const0> ;
  assign s_axi_CTRL_RRESP[1] = \<const0> ;
  assign s_axi_CTRL_RRESP[0] = \<const0> ;
  Adder32bit_mul32_hls_0_0_mul32_hls_CTRL_s_axi CTRL_s_axi_U
       (.CEB2(CTRL_s_axi_U_n_3),
        .D(int_a0),
        .DSP_OUTPUT_INST({mul_32ns_32ns_64_1_1_U1_n_82,mul_32ns_32ns_64_1_1_U1_n_83,mul_32ns_32ns_64_1_1_U1_n_84,mul_32ns_32ns_64_1_1_U1_n_85,mul_32ns_32ns_64_1_1_U1_n_86,mul_32ns_32ns_64_1_1_U1_n_87,mul_32ns_32ns_64_1_1_U1_n_88,mul_32ns_32ns_64_1_1_U1_n_89,mul_32ns_32ns_64_1_1_U1_n_90,mul_32ns_32ns_64_1_1_U1_n_91,mul_32ns_32ns_64_1_1_U1_n_92,mul_32ns_32ns_64_1_1_U1_n_93,mul_32ns_32ns_64_1_1_U1_n_94,mul_32ns_32ns_64_1_1_U1_n_95,mul_32ns_32ns_64_1_1_U1_n_96,mul_32ns_32ns_64_1_1_U1_n_97,mul_32ns_32ns_64_1_1_U1_n_98,mul_32ns_32ns_64_1_1_U1_n_99,mul_32ns_32ns_64_1_1_U1_n_100,mul_32ns_32ns_64_1_1_U1_n_101,mul_32ns_32ns_64_1_1_U1_n_102,mul_32ns_32ns_64_1_1_U1_n_103,mul_32ns_32ns_64_1_1_U1_n_104,mul_32ns_32ns_64_1_1_U1_n_105,mul_32ns_32ns_64_1_1_U1_n_106,mul_32ns_32ns_64_1_1_U1_n_107,mul_32ns_32ns_64_1_1_U1_n_108,mul_32ns_32ns_64_1_1_U1_n_109,mul_32ns_32ns_64_1_1_U1_n_110,mul_32ns_32ns_64_1_1_U1_n_111,mul_32ns_32ns_64_1_1_U1_n_112,mul_32ns_32ns_64_1_1_U1_n_113,mul_32ns_32ns_64_1_1_U1_n_114,mul_32ns_32ns_64_1_1_U1_n_115,mul_32ns_32ns_64_1_1_U1_n_116,mul_32ns_32ns_64_1_1_U1_n_117,mul_32ns_32ns_64_1_1_U1_n_118,mul_32ns_32ns_64_1_1_U1_n_119,mul_32ns_32ns_64_1_1_U1_n_120,mul_32ns_32ns_64_1_1_U1_n_121,mul_32ns_32ns_64_1_1_U1_n_122,mul_32ns_32ns_64_1_1_U1_n_123,mul_32ns_32ns_64_1_1_U1_n_124,mul_32ns_32ns_64_1_1_U1_n_125,mul_32ns_32ns_64_1_1_U1_n_126,mul_32ns_32ns_64_1_1_U1_n_127,mul_32ns_32ns_64_1_1_U1_n_128,mul_32ns_32ns_64_1_1_U1_n_129}),
        .E(CTRL_s_axi_U_n_2),
        .\FSM_onehot_rstate_reg[1]_0 (s_axi_CTRL_ARREADY),
        .\FSM_onehot_wstate_reg[1]_0 (s_axi_CTRL_AWREADY),
        .\FSM_onehot_wstate_reg[2]_0 (s_axi_CTRL_WREADY),
        .O({mul_32ns_32ns_64_1_1_U1_n_130,mul_32ns_32ns_64_1_1_U1_n_131,mul_32ns_32ns_64_1_1_U1_n_132,mul_32ns_32ns_64_1_1_U1_n_133,mul_32ns_32ns_64_1_1_U1_n_134,mul_32ns_32ns_64_1_1_U1_n_135,mul_32ns_32ns_64_1_1_U1_n_136,mul_32ns_32ns_64_1_1_U1_n_137}),
        .P({CTRL_s_axi_U_n_53,CTRL_s_axi_U_n_54,CTRL_s_axi_U_n_55,CTRL_s_axi_U_n_56,CTRL_s_axi_U_n_57,CTRL_s_axi_U_n_58,CTRL_s_axi_U_n_59,CTRL_s_axi_U_n_60,CTRL_s_axi_U_n_61,CTRL_s_axi_U_n_62,CTRL_s_axi_U_n_63,CTRL_s_axi_U_n_64,CTRL_s_axi_U_n_65,CTRL_s_axi_U_n_66,CTRL_s_axi_U_n_67,CTRL_s_axi_U_n_68,CTRL_s_axi_U_n_69,CTRL_s_axi_U_n_70,CTRL_s_axi_U_n_71,CTRL_s_axi_U_n_72,CTRL_s_axi_U_n_73,CTRL_s_axi_U_n_74,CTRL_s_axi_U_n_75,CTRL_s_axi_U_n_76,CTRL_s_axi_U_n_77,CTRL_s_axi_U_n_78,CTRL_s_axi_U_n_79,CTRL_s_axi_U_n_80,CTRL_s_axi_U_n_81,CTRL_s_axi_U_n_82,CTRL_s_axi_U_n_83,CTRL_s_axi_U_n_84,CTRL_s_axi_U_n_85,CTRL_s_axi_U_n_86,CTRL_s_axi_U_n_87,CTRL_s_axi_U_n_88,CTRL_s_axi_U_n_89,CTRL_s_axi_U_n_90,CTRL_s_axi_U_n_91,CTRL_s_axi_U_n_92,CTRL_s_axi_U_n_93,CTRL_s_axi_U_n_94,CTRL_s_axi_U_n_95,CTRL_s_axi_U_n_96,CTRL_s_axi_U_n_97,CTRL_s_axi_U_n_98}),
        .PCOUT({mul_32ns_32ns_64_1_1_U1_n_17,mul_32ns_32ns_64_1_1_U1_n_18,mul_32ns_32ns_64_1_1_U1_n_19,mul_32ns_32ns_64_1_1_U1_n_20,mul_32ns_32ns_64_1_1_U1_n_21,mul_32ns_32ns_64_1_1_U1_n_22,mul_32ns_32ns_64_1_1_U1_n_23,mul_32ns_32ns_64_1_1_U1_n_24,mul_32ns_32ns_64_1_1_U1_n_25,mul_32ns_32ns_64_1_1_U1_n_26,mul_32ns_32ns_64_1_1_U1_n_27,mul_32ns_32ns_64_1_1_U1_n_28,mul_32ns_32ns_64_1_1_U1_n_29,mul_32ns_32ns_64_1_1_U1_n_30,mul_32ns_32ns_64_1_1_U1_n_31,mul_32ns_32ns_64_1_1_U1_n_32,mul_32ns_32ns_64_1_1_U1_n_33,mul_32ns_32ns_64_1_1_U1_n_34,mul_32ns_32ns_64_1_1_U1_n_35,mul_32ns_32ns_64_1_1_U1_n_36,mul_32ns_32ns_64_1_1_U1_n_37,mul_32ns_32ns_64_1_1_U1_n_38,mul_32ns_32ns_64_1_1_U1_n_39,mul_32ns_32ns_64_1_1_U1_n_40,mul_32ns_32ns_64_1_1_U1_n_41,mul_32ns_32ns_64_1_1_U1_n_42,mul_32ns_32ns_64_1_1_U1_n_43,mul_32ns_32ns_64_1_1_U1_n_44,mul_32ns_32ns_64_1_1_U1_n_45,mul_32ns_32ns_64_1_1_U1_n_46,mul_32ns_32ns_64_1_1_U1_n_47,mul_32ns_32ns_64_1_1_U1_n_48,mul_32ns_32ns_64_1_1_U1_n_49,mul_32ns_32ns_64_1_1_U1_n_50,mul_32ns_32ns_64_1_1_U1_n_51,mul_32ns_32ns_64_1_1_U1_n_52,mul_32ns_32ns_64_1_1_U1_n_53,mul_32ns_32ns_64_1_1_U1_n_54,mul_32ns_32ns_64_1_1_U1_n_55,mul_32ns_32ns_64_1_1_U1_n_56,mul_32ns_32ns_64_1_1_U1_n_57,mul_32ns_32ns_64_1_1_U1_n_58,mul_32ns_32ns_64_1_1_U1_n_59,mul_32ns_32ns_64_1_1_U1_n_60,mul_32ns_32ns_64_1_1_U1_n_61,mul_32ns_32ns_64_1_1_U1_n_62,mul_32ns_32ns_64_1_1_U1_n_63,mul_32ns_32ns_64_1_1_U1_n_64}),
        .RSTB(ap_rst_n_inv),
        .S({CTRL_s_axi_U_n_102,CTRL_s_axi_U_n_103,CTRL_s_axi_U_n_104,CTRL_s_axi_U_n_105,CTRL_s_axi_U_n_106,CTRL_s_axi_U_n_107,CTRL_s_axi_U_n_108,CTRL_s_axi_U_n_109}),
        .ap_clk(ap_clk),
        .ap_rst_n(ap_rst_n),
        .data7({data7[31:24],data7[0]}),
        .int_ap_start_reg_0({CTRL_s_axi_U_n_143,CTRL_s_axi_U_n_144,CTRL_s_axi_U_n_145,CTRL_s_axi_U_n_146,CTRL_s_axi_U_n_147,CTRL_s_axi_U_n_148,CTRL_s_axi_U_n_149,CTRL_s_axi_U_n_150}),
        .int_ap_start_reg_1({CTRL_s_axi_U_n_151,CTRL_s_axi_U_n_152,CTRL_s_axi_U_n_153,CTRL_s_axi_U_n_154,CTRL_s_axi_U_n_155,CTRL_s_axi_U_n_156,CTRL_s_axi_U_n_157,CTRL_s_axi_U_n_158}),
        .int_ap_start_reg_2({CTRL_s_axi_U_n_159,CTRL_s_axi_U_n_160,CTRL_s_axi_U_n_161,CTRL_s_axi_U_n_162,CTRL_s_axi_U_n_163,CTRL_s_axi_U_n_164,CTRL_s_axi_U_n_165,CTRL_s_axi_U_n_166}),
        .\int_b_reg[10]_0 (CTRL_s_axi_U_n_121),
        .\int_b_reg[11]_0 (CTRL_s_axi_U_n_122),
        .\int_b_reg[12]_0 (CTRL_s_axi_U_n_123),
        .\int_b_reg[13]_0 (CTRL_s_axi_U_n_124),
        .\int_b_reg[14]_0 (CTRL_s_axi_U_n_125),
        .\int_b_reg[15]_0 (CTRL_s_axi_U_n_126),
        .\int_b_reg[4]_0 (CTRL_s_axi_U_n_115),
        .\int_b_reg[5]_0 (CTRL_s_axi_U_n_116),
        .\int_b_reg[6]_0 (CTRL_s_axi_U_n_117),
        .\int_b_reg[8]_0 (CTRL_s_axi_U_n_119),
        .\int_p_reg[14]_0 ({CTRL_s_axi_U_n_135,CTRL_s_axi_U_n_136,CTRL_s_axi_U_n_137,CTRL_s_axi_U_n_138,CTRL_s_axi_U_n_139,CTRL_s_axi_U_n_140,CTRL_s_axi_U_n_141,CTRL_s_axi_U_n_142}),
        .\int_p_reg[16]_0 ({mul_32ns_32ns_64_1_1_U1_n_0,mul_32ns_32ns_64_1_1_U1_n_1,mul_32ns_32ns_64_1_1_U1_n_2,mul_32ns_32ns_64_1_1_U1_n_3,mul_32ns_32ns_64_1_1_U1_n_4,mul_32ns_32ns_64_1_1_U1_n_5,mul_32ns_32ns_64_1_1_U1_n_6,mul_32ns_32ns_64_1_1_U1_n_7,mul_32ns_32ns_64_1_1_U1_n_8,mul_32ns_32ns_64_1_1_U1_n_9,mul_32ns_32ns_64_1_1_U1_n_10,mul_32ns_32ns_64_1_1_U1_n_11,mul_32ns_32ns_64_1_1_U1_n_12,mul_32ns_32ns_64_1_1_U1_n_13,mul_32ns_32ns_64_1_1_U1_n_14,mul_32ns_32ns_64_1_1_U1_n_15,mul_32ns_32ns_64_1_1_U1_n_16}),
        .\int_p_reg[16]__0_0 ({mul_32ns_32ns_64_1_1_U1_n_65,mul_32ns_32ns_64_1_1_U1_n_66,mul_32ns_32ns_64_1_1_U1_n_67,mul_32ns_32ns_64_1_1_U1_n_68,mul_32ns_32ns_64_1_1_U1_n_69,mul_32ns_32ns_64_1_1_U1_n_70,mul_32ns_32ns_64_1_1_U1_n_71,mul_32ns_32ns_64_1_1_U1_n_72,mul_32ns_32ns_64_1_1_U1_n_73,mul_32ns_32ns_64_1_1_U1_n_74,mul_32ns_32ns_64_1_1_U1_n_75,mul_32ns_32ns_64_1_1_U1_n_76,mul_32ns_32ns_64_1_1_U1_n_77,mul_32ns_32ns_64_1_1_U1_n_78,mul_32ns_32ns_64_1_1_U1_n_79,mul_32ns_32ns_64_1_1_U1_n_80,mul_32ns_32ns_64_1_1_U1_n_81}),
        .\int_p_reg[6]_0 ({CTRL_s_axi_U_n_127,CTRL_s_axi_U_n_128,CTRL_s_axi_U_n_129,CTRL_s_axi_U_n_130,CTRL_s_axi_U_n_131,CTRL_s_axi_U_n_132,CTRL_s_axi_U_n_133,CTRL_s_axi_U_n_134}),
        .interrupt(interrupt),
        .\rdata_reg[10]_0 (mul_32ns_32ns_64_1_1_U1_n_164),
        .\rdata_reg[11]_0 (mul_32ns_32ns_64_1_1_U1_n_165),
        .\rdata_reg[12]_0 (mul_32ns_32ns_64_1_1_U1_n_166),
        .\rdata_reg[13]_0 (mul_32ns_32ns_64_1_1_U1_n_167),
        .\rdata_reg[14]_0 (mul_32ns_32ns_64_1_1_U1_n_168),
        .\rdata_reg[15]_0 (mul_32ns_32ns_64_1_1_U1_n_169),
        .\rdata_reg[16]_0 (mul_32ns_32ns_64_1_1_U1_n_170),
        .\rdata_reg[17]_0 (mul_32ns_32ns_64_1_1_U1_n_171),
        .\rdata_reg[18]_0 (mul_32ns_32ns_64_1_1_U1_n_172),
        .\rdata_reg[19]_0 (mul_32ns_32ns_64_1_1_U1_n_173),
        .\rdata_reg[1]_0 (mul_32ns_32ns_64_1_1_U1_n_155),
        .\rdata_reg[20]_0 (mul_32ns_32ns_64_1_1_U1_n_174),
        .\rdata_reg[21]_0 (mul_32ns_32ns_64_1_1_U1_n_175),
        .\rdata_reg[22]_0 (mul_32ns_32ns_64_1_1_U1_n_176),
        .\rdata_reg[23]_0 (mul_32ns_32ns_64_1_1_U1_n_177),
        .\rdata_reg[2]_0 (mul_32ns_32ns_64_1_1_U1_n_156),
        .\rdata_reg[31]_0 ({mul_32ns_32ns_64_1_1_U1_n_138,mul_32ns_32ns_64_1_1_U1_n_139,mul_32ns_32ns_64_1_1_U1_n_140,mul_32ns_32ns_64_1_1_U1_n_141,mul_32ns_32ns_64_1_1_U1_n_142,mul_32ns_32ns_64_1_1_U1_n_143,mul_32ns_32ns_64_1_1_U1_n_144,mul_32ns_32ns_64_1_1_U1_n_145}),
        .\rdata_reg[3]_0 (mul_32ns_32ns_64_1_1_U1_n_157),
        .\rdata_reg[4]_0 (mul_32ns_32ns_64_1_1_U1_n_158),
        .\rdata_reg[5]_0 (mul_32ns_32ns_64_1_1_U1_n_159),
        .\rdata_reg[6]_0 (mul_32ns_32ns_64_1_1_U1_n_160),
        .\rdata_reg[7]_0 (mul_32ns_32ns_64_1_1_U1_n_161),
        .\rdata_reg[8]_0 (mul_32ns_32ns_64_1_1_U1_n_162),
        .\rdata_reg[9]_0 (mul_32ns_32ns_64_1_1_U1_n_163),
        .s_axi_CTRL_ARADDR(s_axi_CTRL_ARADDR),
        .\s_axi_CTRL_ARADDR[5]_0 (CTRL_s_axi_U_n_113),
        .\s_axi_CTRL_ARADDR[5]_1 (CTRL_s_axi_U_n_114),
        .\s_axi_CTRL_ARADDR[5]_2 (CTRL_s_axi_U_n_118),
        .\s_axi_CTRL_ARADDR[5]_3 (CTRL_s_axi_U_n_120),
        .s_axi_CTRL_ARADDR_5_sp_1(CTRL_s_axi_U_n_112),
        .s_axi_CTRL_ARVALID(s_axi_CTRL_ARVALID),
        .s_axi_CTRL_AWADDR(s_axi_CTRL_AWADDR[5:2]),
        .s_axi_CTRL_AWVALID(s_axi_CTRL_AWVALID),
        .s_axi_CTRL_BREADY(s_axi_CTRL_BREADY),
        .s_axi_CTRL_BVALID(s_axi_CTRL_BVALID),
        .s_axi_CTRL_RDATA(s_axi_CTRL_RDATA),
        .s_axi_CTRL_RREADY(s_axi_CTRL_RREADY),
        .s_axi_CTRL_RVALID(s_axi_CTRL_RVALID),
        .s_axi_CTRL_WDATA(s_axi_CTRL_WDATA),
        .\s_axi_CTRL_WDATA[31] (int_b0),
        .s_axi_CTRL_WSTRB(s_axi_CTRL_WSTRB),
        .s_axi_CTRL_WVALID(s_axi_CTRL_WVALID));
  GND GND
       (.G(\<const0> ));
  Adder32bit_mul32_hls_0_0_mul32_hls_mul_32ns_32ns_64_1_1 mul_32ns_32ns_64_1_1_U1
       (.CEB2(CTRL_s_axi_U_n_3),
        .D(int_a0),
        .DSP_ALU_INST(int_b0),
        .E(CTRL_s_axi_U_n_2),
        .O({mul_32ns_32ns_64_1_1_U1_n_130,mul_32ns_32ns_64_1_1_U1_n_131,mul_32ns_32ns_64_1_1_U1_n_132,mul_32ns_32ns_64_1_1_U1_n_133,mul_32ns_32ns_64_1_1_U1_n_134,mul_32ns_32ns_64_1_1_U1_n_135,mul_32ns_32ns_64_1_1_U1_n_136,mul_32ns_32ns_64_1_1_U1_n_137}),
        .P({mul_32ns_32ns_64_1_1_U1_n_0,mul_32ns_32ns_64_1_1_U1_n_1,mul_32ns_32ns_64_1_1_U1_n_2,mul_32ns_32ns_64_1_1_U1_n_3,mul_32ns_32ns_64_1_1_U1_n_4,mul_32ns_32ns_64_1_1_U1_n_5,mul_32ns_32ns_64_1_1_U1_n_6,mul_32ns_32ns_64_1_1_U1_n_7,mul_32ns_32ns_64_1_1_U1_n_8,mul_32ns_32ns_64_1_1_U1_n_9,mul_32ns_32ns_64_1_1_U1_n_10,mul_32ns_32ns_64_1_1_U1_n_11,mul_32ns_32ns_64_1_1_U1_n_12,mul_32ns_32ns_64_1_1_U1_n_13,mul_32ns_32ns_64_1_1_U1_n_14,mul_32ns_32ns_64_1_1_U1_n_15,mul_32ns_32ns_64_1_1_U1_n_16}),
        .PCOUT({mul_32ns_32ns_64_1_1_U1_n_17,mul_32ns_32ns_64_1_1_U1_n_18,mul_32ns_32ns_64_1_1_U1_n_19,mul_32ns_32ns_64_1_1_U1_n_20,mul_32ns_32ns_64_1_1_U1_n_21,mul_32ns_32ns_64_1_1_U1_n_22,mul_32ns_32ns_64_1_1_U1_n_23,mul_32ns_32ns_64_1_1_U1_n_24,mul_32ns_32ns_64_1_1_U1_n_25,mul_32ns_32ns_64_1_1_U1_n_26,mul_32ns_32ns_64_1_1_U1_n_27,mul_32ns_32ns_64_1_1_U1_n_28,mul_32ns_32ns_64_1_1_U1_n_29,mul_32ns_32ns_64_1_1_U1_n_30,mul_32ns_32ns_64_1_1_U1_n_31,mul_32ns_32ns_64_1_1_U1_n_32,mul_32ns_32ns_64_1_1_U1_n_33,mul_32ns_32ns_64_1_1_U1_n_34,mul_32ns_32ns_64_1_1_U1_n_35,mul_32ns_32ns_64_1_1_U1_n_36,mul_32ns_32ns_64_1_1_U1_n_37,mul_32ns_32ns_64_1_1_U1_n_38,mul_32ns_32ns_64_1_1_U1_n_39,mul_32ns_32ns_64_1_1_U1_n_40,mul_32ns_32ns_64_1_1_U1_n_41,mul_32ns_32ns_64_1_1_U1_n_42,mul_32ns_32ns_64_1_1_U1_n_43,mul_32ns_32ns_64_1_1_U1_n_44,mul_32ns_32ns_64_1_1_U1_n_45,mul_32ns_32ns_64_1_1_U1_n_46,mul_32ns_32ns_64_1_1_U1_n_47,mul_32ns_32ns_64_1_1_U1_n_48,mul_32ns_32ns_64_1_1_U1_n_49,mul_32ns_32ns_64_1_1_U1_n_50,mul_32ns_32ns_64_1_1_U1_n_51,mul_32ns_32ns_64_1_1_U1_n_52,mul_32ns_32ns_64_1_1_U1_n_53,mul_32ns_32ns_64_1_1_U1_n_54,mul_32ns_32ns_64_1_1_U1_n_55,mul_32ns_32ns_64_1_1_U1_n_56,mul_32ns_32ns_64_1_1_U1_n_57,mul_32ns_32ns_64_1_1_U1_n_58,mul_32ns_32ns_64_1_1_U1_n_59,mul_32ns_32ns_64_1_1_U1_n_60,mul_32ns_32ns_64_1_1_U1_n_61,mul_32ns_32ns_64_1_1_U1_n_62,mul_32ns_32ns_64_1_1_U1_n_63,mul_32ns_32ns_64_1_1_U1_n_64}),
        .RSTB(ap_rst_n_inv),
        .S({CTRL_s_axi_U_n_102,CTRL_s_axi_U_n_103,CTRL_s_axi_U_n_104,CTRL_s_axi_U_n_105,CTRL_s_axi_U_n_106,CTRL_s_axi_U_n_107,CTRL_s_axi_U_n_108,CTRL_s_axi_U_n_109}),
        .ap_clk(ap_clk),
        .ap_clk_0({mul_32ns_32ns_64_1_1_U1_n_65,mul_32ns_32ns_64_1_1_U1_n_66,mul_32ns_32ns_64_1_1_U1_n_67,mul_32ns_32ns_64_1_1_U1_n_68,mul_32ns_32ns_64_1_1_U1_n_69,mul_32ns_32ns_64_1_1_U1_n_70,mul_32ns_32ns_64_1_1_U1_n_71,mul_32ns_32ns_64_1_1_U1_n_72,mul_32ns_32ns_64_1_1_U1_n_73,mul_32ns_32ns_64_1_1_U1_n_74,mul_32ns_32ns_64_1_1_U1_n_75,mul_32ns_32ns_64_1_1_U1_n_76,mul_32ns_32ns_64_1_1_U1_n_77,mul_32ns_32ns_64_1_1_U1_n_78,mul_32ns_32ns_64_1_1_U1_n_79,mul_32ns_32ns_64_1_1_U1_n_80,mul_32ns_32ns_64_1_1_U1_n_81}),
        .ap_clk_1({mul_32ns_32ns_64_1_1_U1_n_82,mul_32ns_32ns_64_1_1_U1_n_83,mul_32ns_32ns_64_1_1_U1_n_84,mul_32ns_32ns_64_1_1_U1_n_85,mul_32ns_32ns_64_1_1_U1_n_86,mul_32ns_32ns_64_1_1_U1_n_87,mul_32ns_32ns_64_1_1_U1_n_88,mul_32ns_32ns_64_1_1_U1_n_89,mul_32ns_32ns_64_1_1_U1_n_90,mul_32ns_32ns_64_1_1_U1_n_91,mul_32ns_32ns_64_1_1_U1_n_92,mul_32ns_32ns_64_1_1_U1_n_93,mul_32ns_32ns_64_1_1_U1_n_94,mul_32ns_32ns_64_1_1_U1_n_95,mul_32ns_32ns_64_1_1_U1_n_96,mul_32ns_32ns_64_1_1_U1_n_97,mul_32ns_32ns_64_1_1_U1_n_98,mul_32ns_32ns_64_1_1_U1_n_99,mul_32ns_32ns_64_1_1_U1_n_100,mul_32ns_32ns_64_1_1_U1_n_101,mul_32ns_32ns_64_1_1_U1_n_102,mul_32ns_32ns_64_1_1_U1_n_103,mul_32ns_32ns_64_1_1_U1_n_104,mul_32ns_32ns_64_1_1_U1_n_105,mul_32ns_32ns_64_1_1_U1_n_106,mul_32ns_32ns_64_1_1_U1_n_107,mul_32ns_32ns_64_1_1_U1_n_108,mul_32ns_32ns_64_1_1_U1_n_109,mul_32ns_32ns_64_1_1_U1_n_110,mul_32ns_32ns_64_1_1_U1_n_111,mul_32ns_32ns_64_1_1_U1_n_112,mul_32ns_32ns_64_1_1_U1_n_113,mul_32ns_32ns_64_1_1_U1_n_114,mul_32ns_32ns_64_1_1_U1_n_115,mul_32ns_32ns_64_1_1_U1_n_116,mul_32ns_32ns_64_1_1_U1_n_117,mul_32ns_32ns_64_1_1_U1_n_118,mul_32ns_32ns_64_1_1_U1_n_119,mul_32ns_32ns_64_1_1_U1_n_120,mul_32ns_32ns_64_1_1_U1_n_121,mul_32ns_32ns_64_1_1_U1_n_122,mul_32ns_32ns_64_1_1_U1_n_123,mul_32ns_32ns_64_1_1_U1_n_124,mul_32ns_32ns_64_1_1_U1_n_125,mul_32ns_32ns_64_1_1_U1_n_126,mul_32ns_32ns_64_1_1_U1_n_127,mul_32ns_32ns_64_1_1_U1_n_128,mul_32ns_32ns_64_1_1_U1_n_129}),
        .int_ap_start_reg({data7[31:24],data7[0]}),
        .\int_p_reg[16]__0 ({mul_32ns_32ns_64_1_1_U1_n_138,mul_32ns_32ns_64_1_1_U1_n_139,mul_32ns_32ns_64_1_1_U1_n_140,mul_32ns_32ns_64_1_1_U1_n_141,mul_32ns_32ns_64_1_1_U1_n_142,mul_32ns_32ns_64_1_1_U1_n_143,mul_32ns_32ns_64_1_1_U1_n_144,mul_32ns_32ns_64_1_1_U1_n_145}),
        .\rdata[16]_i_2 ({CTRL_s_axi_U_n_127,CTRL_s_axi_U_n_128,CTRL_s_axi_U_n_129,CTRL_s_axi_U_n_130,CTRL_s_axi_U_n_131,CTRL_s_axi_U_n_132,CTRL_s_axi_U_n_133,CTRL_s_axi_U_n_134}),
        .\rdata[16]_i_3_0 ({CTRL_s_axi_U_n_159,CTRL_s_axi_U_n_160,CTRL_s_axi_U_n_161,CTRL_s_axi_U_n_162,CTRL_s_axi_U_n_163,CTRL_s_axi_U_n_164,CTRL_s_axi_U_n_165,CTRL_s_axi_U_n_166}),
        .\rdata[24]_i_2 ({CTRL_s_axi_U_n_135,CTRL_s_axi_U_n_136,CTRL_s_axi_U_n_137,CTRL_s_axi_U_n_138,CTRL_s_axi_U_n_139,CTRL_s_axi_U_n_140,CTRL_s_axi_U_n_141,CTRL_s_axi_U_n_142}),
        .\rdata[24]_i_3 ({CTRL_s_axi_U_n_53,CTRL_s_axi_U_n_54,CTRL_s_axi_U_n_55,CTRL_s_axi_U_n_56,CTRL_s_axi_U_n_57,CTRL_s_axi_U_n_58,CTRL_s_axi_U_n_59,CTRL_s_axi_U_n_60,CTRL_s_axi_U_n_61,CTRL_s_axi_U_n_62,CTRL_s_axi_U_n_63,CTRL_s_axi_U_n_64,CTRL_s_axi_U_n_65,CTRL_s_axi_U_n_66,CTRL_s_axi_U_n_67,CTRL_s_axi_U_n_68,CTRL_s_axi_U_n_69,CTRL_s_axi_U_n_70,CTRL_s_axi_U_n_71,CTRL_s_axi_U_n_72,CTRL_s_axi_U_n_73,CTRL_s_axi_U_n_74,CTRL_s_axi_U_n_75,CTRL_s_axi_U_n_76,CTRL_s_axi_U_n_77,CTRL_s_axi_U_n_78,CTRL_s_axi_U_n_79,CTRL_s_axi_U_n_80,CTRL_s_axi_U_n_81,CTRL_s_axi_U_n_82,CTRL_s_axi_U_n_83,CTRL_s_axi_U_n_84,CTRL_s_axi_U_n_85,CTRL_s_axi_U_n_86,CTRL_s_axi_U_n_87,CTRL_s_axi_U_n_88,CTRL_s_axi_U_n_89,CTRL_s_axi_U_n_90,CTRL_s_axi_U_n_91,CTRL_s_axi_U_n_92,CTRL_s_axi_U_n_93,CTRL_s_axi_U_n_94,CTRL_s_axi_U_n_95,CTRL_s_axi_U_n_96,CTRL_s_axi_U_n_97,CTRL_s_axi_U_n_98}),
        .\rdata_reg[10] (CTRL_s_axi_U_n_121),
        .\rdata_reg[11] (CTRL_s_axi_U_n_122),
        .\rdata_reg[12] (CTRL_s_axi_U_n_123),
        .\rdata_reg[13] (CTRL_s_axi_U_n_124),
        .\rdata_reg[14] (CTRL_s_axi_U_n_125),
        .\rdata_reg[15] ({CTRL_s_axi_U_n_151,CTRL_s_axi_U_n_152,CTRL_s_axi_U_n_153,CTRL_s_axi_U_n_154,CTRL_s_axi_U_n_155,CTRL_s_axi_U_n_156,CTRL_s_axi_U_n_157,CTRL_s_axi_U_n_158}),
        .\rdata_reg[15]_0 (CTRL_s_axi_U_n_126),
        .\rdata_reg[1] (CTRL_s_axi_U_n_112),
        .\rdata_reg[2] (CTRL_s_axi_U_n_113),
        .\rdata_reg[3] (CTRL_s_axi_U_n_114),
        .\rdata_reg[4] (CTRL_s_axi_U_n_115),
        .\rdata_reg[5] (CTRL_s_axi_U_n_116),
        .\rdata_reg[6] (CTRL_s_axi_U_n_117),
        .\rdata_reg[7] ({CTRL_s_axi_U_n_143,CTRL_s_axi_U_n_144,CTRL_s_axi_U_n_145,CTRL_s_axi_U_n_146,CTRL_s_axi_U_n_147,CTRL_s_axi_U_n_148,CTRL_s_axi_U_n_149,CTRL_s_axi_U_n_150}),
        .\rdata_reg[7]_0 (CTRL_s_axi_U_n_118),
        .\rdata_reg[8] (CTRL_s_axi_U_n_119),
        .\rdata_reg[9] (CTRL_s_axi_U_n_120),
        .s_axi_CTRL_ARADDR(s_axi_CTRL_ARADDR[5:2]),
        .\s_axi_CTRL_ARADDR[3]_0 (mul_32ns_32ns_64_1_1_U1_n_156),
        .\s_axi_CTRL_ARADDR[3]_1 (mul_32ns_32ns_64_1_1_U1_n_157),
        .\s_axi_CTRL_ARADDR[3]_10 (mul_32ns_32ns_64_1_1_U1_n_166),
        .\s_axi_CTRL_ARADDR[3]_11 (mul_32ns_32ns_64_1_1_U1_n_167),
        .\s_axi_CTRL_ARADDR[3]_12 (mul_32ns_32ns_64_1_1_U1_n_168),
        .\s_axi_CTRL_ARADDR[3]_13 (mul_32ns_32ns_64_1_1_U1_n_169),
        .\s_axi_CTRL_ARADDR[3]_14 (mul_32ns_32ns_64_1_1_U1_n_170),
        .\s_axi_CTRL_ARADDR[3]_15 (mul_32ns_32ns_64_1_1_U1_n_171),
        .\s_axi_CTRL_ARADDR[3]_16 (mul_32ns_32ns_64_1_1_U1_n_172),
        .\s_axi_CTRL_ARADDR[3]_17 (mul_32ns_32ns_64_1_1_U1_n_173),
        .\s_axi_CTRL_ARADDR[3]_18 (mul_32ns_32ns_64_1_1_U1_n_174),
        .\s_axi_CTRL_ARADDR[3]_19 (mul_32ns_32ns_64_1_1_U1_n_175),
        .\s_axi_CTRL_ARADDR[3]_2 (mul_32ns_32ns_64_1_1_U1_n_158),
        .\s_axi_CTRL_ARADDR[3]_20 (mul_32ns_32ns_64_1_1_U1_n_176),
        .\s_axi_CTRL_ARADDR[3]_21 (mul_32ns_32ns_64_1_1_U1_n_177),
        .\s_axi_CTRL_ARADDR[3]_3 (mul_32ns_32ns_64_1_1_U1_n_159),
        .\s_axi_CTRL_ARADDR[3]_4 (mul_32ns_32ns_64_1_1_U1_n_160),
        .\s_axi_CTRL_ARADDR[3]_5 (mul_32ns_32ns_64_1_1_U1_n_161),
        .\s_axi_CTRL_ARADDR[3]_6 (mul_32ns_32ns_64_1_1_U1_n_162),
        .\s_axi_CTRL_ARADDR[3]_7 (mul_32ns_32ns_64_1_1_U1_n_163),
        .\s_axi_CTRL_ARADDR[3]_8 (mul_32ns_32ns_64_1_1_U1_n_164),
        .\s_axi_CTRL_ARADDR[3]_9 (mul_32ns_32ns_64_1_1_U1_n_165),
        .s_axi_CTRL_ARADDR_3_sp_1(mul_32ns_32ns_64_1_1_U1_n_155));
endmodule

(* ORIG_REF_NAME = "mul32_hls_CTRL_s_axi" *) 
module Adder32bit_mul32_hls_0_0_mul32_hls_CTRL_s_axi
   (RSTB,
    interrupt,
    E,
    CEB2,
    D,
    \s_axi_CTRL_WDATA[31] ,
    P,
    \FSM_onehot_rstate_reg[1]_0 ,
    s_axi_CTRL_RVALID,
    \FSM_onehot_wstate_reg[1]_0 ,
    S,
    \FSM_onehot_wstate_reg[2]_0 ,
    s_axi_CTRL_BVALID,
    s_axi_CTRL_ARADDR_5_sp_1,
    \s_axi_CTRL_ARADDR[5]_0 ,
    \s_axi_CTRL_ARADDR[5]_1 ,
    \int_b_reg[4]_0 ,
    \int_b_reg[5]_0 ,
    \int_b_reg[6]_0 ,
    \s_axi_CTRL_ARADDR[5]_2 ,
    \int_b_reg[8]_0 ,
    \s_axi_CTRL_ARADDR[5]_3 ,
    \int_b_reg[10]_0 ,
    \int_b_reg[11]_0 ,
    \int_b_reg[12]_0 ,
    \int_b_reg[13]_0 ,
    \int_b_reg[14]_0 ,
    \int_b_reg[15]_0 ,
    \int_p_reg[6]_0 ,
    \int_p_reg[14]_0 ,
    int_ap_start_reg_0,
    int_ap_start_reg_1,
    int_ap_start_reg_2,
    s_axi_CTRL_RDATA,
    ap_clk,
    PCOUT,
    DSP_OUTPUT_INST,
    s_axi_CTRL_ARVALID,
    s_axi_CTRL_RREADY,
    s_axi_CTRL_ARADDR,
    s_axi_CTRL_AWVALID,
    s_axi_CTRL_WVALID,
    s_axi_CTRL_BREADY,
    s_axi_CTRL_WDATA,
    s_axi_CTRL_WSTRB,
    data7,
    \rdata_reg[16]_0 ,
    O,
    \rdata_reg[17]_0 ,
    \rdata_reg[18]_0 ,
    \rdata_reg[19]_0 ,
    \rdata_reg[20]_0 ,
    \rdata_reg[21]_0 ,
    \rdata_reg[22]_0 ,
    \rdata_reg[23]_0 ,
    \rdata_reg[31]_0 ,
    ap_rst_n,
    s_axi_CTRL_AWADDR,
    \int_p_reg[16]_0 ,
    \int_p_reg[16]__0_0 ,
    \rdata_reg[15]_0 ,
    \rdata_reg[14]_0 ,
    \rdata_reg[13]_0 ,
    \rdata_reg[12]_0 ,
    \rdata_reg[11]_0 ,
    \rdata_reg[10]_0 ,
    \rdata_reg[9]_0 ,
    \rdata_reg[8]_0 ,
    \rdata_reg[7]_0 ,
    \rdata_reg[6]_0 ,
    \rdata_reg[5]_0 ,
    \rdata_reg[4]_0 ,
    \rdata_reg[3]_0 ,
    \rdata_reg[2]_0 ,
    \rdata_reg[1]_0 );
  output RSTB;
  output interrupt;
  output [0:0]E;
  output CEB2;
  output [16:0]D;
  output [31:0]\s_axi_CTRL_WDATA[31] ;
  output [45:0]P;
  output \FSM_onehot_rstate_reg[1]_0 ;
  output s_axi_CTRL_RVALID;
  output \FSM_onehot_wstate_reg[1]_0 ;
  output [7:0]S;
  output \FSM_onehot_wstate_reg[2]_0 ;
  output s_axi_CTRL_BVALID;
  output s_axi_CTRL_ARADDR_5_sp_1;
  output \s_axi_CTRL_ARADDR[5]_0 ;
  output \s_axi_CTRL_ARADDR[5]_1 ;
  output \int_b_reg[4]_0 ;
  output \int_b_reg[5]_0 ;
  output \int_b_reg[6]_0 ;
  output \s_axi_CTRL_ARADDR[5]_2 ;
  output \int_b_reg[8]_0 ;
  output \s_axi_CTRL_ARADDR[5]_3 ;
  output \int_b_reg[10]_0 ;
  output \int_b_reg[11]_0 ;
  output \int_b_reg[12]_0 ;
  output \int_b_reg[13]_0 ;
  output \int_b_reg[14]_0 ;
  output \int_b_reg[15]_0 ;
  output [7:0]\int_p_reg[6]_0 ;
  output [7:0]\int_p_reg[14]_0 ;
  output [7:0]int_ap_start_reg_0;
  output [7:0]int_ap_start_reg_1;
  output [7:0]int_ap_start_reg_2;
  output [31:0]s_axi_CTRL_RDATA;
  input ap_clk;
  input [47:0]PCOUT;
  input [47:0]DSP_OUTPUT_INST;
  input s_axi_CTRL_ARVALID;
  input s_axi_CTRL_RREADY;
  input [5:0]s_axi_CTRL_ARADDR;
  input s_axi_CTRL_AWVALID;
  input s_axi_CTRL_WVALID;
  input s_axi_CTRL_BREADY;
  input [31:0]s_axi_CTRL_WDATA;
  input [3:0]s_axi_CTRL_WSTRB;
  input [8:0]data7;
  input \rdata_reg[16]_0 ;
  input [7:0]O;
  input \rdata_reg[17]_0 ;
  input \rdata_reg[18]_0 ;
  input \rdata_reg[19]_0 ;
  input \rdata_reg[20]_0 ;
  input \rdata_reg[21]_0 ;
  input \rdata_reg[22]_0 ;
  input \rdata_reg[23]_0 ;
  input [7:0]\rdata_reg[31]_0 ;
  input ap_rst_n;
  input [3:0]s_axi_CTRL_AWADDR;
  input [16:0]\int_p_reg[16]_0 ;
  input [16:0]\int_p_reg[16]__0_0 ;
  input \rdata_reg[15]_0 ;
  input \rdata_reg[14]_0 ;
  input \rdata_reg[13]_0 ;
  input \rdata_reg[12]_0 ;
  input \rdata_reg[11]_0 ;
  input \rdata_reg[10]_0 ;
  input \rdata_reg[9]_0 ;
  input \rdata_reg[8]_0 ;
  input \rdata_reg[7]_0 ;
  input \rdata_reg[6]_0 ;
  input \rdata_reg[5]_0 ;
  input \rdata_reg[4]_0 ;
  input \rdata_reg[3]_0 ;
  input \rdata_reg[2]_0 ;
  input \rdata_reg[1]_0 ;

  wire CEB2;
  wire [16:0]D;
  wire [47:0]DSP_OUTPUT_INST;
  wire [0:0]E;
  wire \FSM_onehot_rstate[1]_i_1_n_0 ;
  wire \FSM_onehot_rstate[2]_i_1_n_0 ;
  wire \FSM_onehot_rstate_reg[1]_0 ;
  wire \FSM_onehot_wstate[1]_i_2_n_0 ;
  wire \FSM_onehot_wstate[2]_i_1_n_0 ;
  wire \FSM_onehot_wstate[3]_i_1_n_0 ;
  wire \FSM_onehot_wstate_reg[1]_0 ;
  wire \FSM_onehot_wstate_reg[2]_0 ;
  wire [7:0]O;
  wire [45:0]P;
  wire [47:0]PCOUT;
  wire RSTB;
  wire [7:0]S;
  wire ap_clk;
  wire ap_done;
  wire ap_rst_n;
  wire ar_hs;
  wire auto_restart_status_reg_n_0;
  wire [8:0]data7;
  wire [31:17]int_a0;
  wire int_ap_ready;
  wire int_ap_ready_i_1_n_0;
  wire int_ap_start_i_1_n_0;
  wire int_ap_start_i_2_n_0;
  wire [7:0]int_ap_start_reg_0;
  wire [7:0]int_ap_start_reg_1;
  wire [7:0]int_ap_start_reg_2;
  wire int_auto_restart_i_1_n_0;
  wire \int_b_reg[10]_0 ;
  wire \int_b_reg[11]_0 ;
  wire \int_b_reg[12]_0 ;
  wire \int_b_reg[13]_0 ;
  wire \int_b_reg[14]_0 ;
  wire \int_b_reg[15]_0 ;
  wire \int_b_reg[4]_0 ;
  wire \int_b_reg[5]_0 ;
  wire \int_b_reg[6]_0 ;
  wire \int_b_reg[8]_0 ;
  wire int_gie_i_1_n_0;
  wire int_gie_reg_n_0;
  wire int_ier;
  wire int_ier_i_1_n_0;
  wire int_interrupt0;
  wire int_interrupt1;
  wire int_isr6_out;
  wire int_isr_i_1_n_0;
  wire int_isr_i_2_n_0;
  wire int_p_ap_vld;
  wire int_p_ap_vld1;
  wire int_p_ap_vld_i_1_n_0;
  wire \int_p_reg[0]__0_n_0 ;
  wire \int_p_reg[10]__0_n_0 ;
  wire \int_p_reg[11]__0_n_0 ;
  wire \int_p_reg[12]__0_n_0 ;
  wire \int_p_reg[13]__0_n_0 ;
  wire [7:0]\int_p_reg[14]_0 ;
  wire \int_p_reg[14]__0_n_0 ;
  wire \int_p_reg[15]__0_n_0 ;
  wire [16:0]\int_p_reg[16]_0 ;
  wire [16:0]\int_p_reg[16]__0_0 ;
  wire \int_p_reg[1]__0_n_0 ;
  wire \int_p_reg[2]__0_n_0 ;
  wire \int_p_reg[3]__0_n_0 ;
  wire \int_p_reg[4]__0_n_0 ;
  wire \int_p_reg[5]__0_n_0 ;
  wire [7:0]\int_p_reg[6]_0 ;
  wire \int_p_reg[6]__0_n_0 ;
  wire \int_p_reg[7]__0_n_0 ;
  wire \int_p_reg[8]__0_n_0 ;
  wire \int_p_reg[9]__0_n_0 ;
  wire int_p_reg__0_n_58;
  wire int_p_reg__0_n_59;
  wire \int_p_reg_n_0_[0] ;
  wire \int_p_reg_n_0_[10] ;
  wire \int_p_reg_n_0_[11] ;
  wire \int_p_reg_n_0_[12] ;
  wire \int_p_reg_n_0_[13] ;
  wire \int_p_reg_n_0_[14] ;
  wire \int_p_reg_n_0_[15] ;
  wire \int_p_reg_n_0_[16] ;
  wire \int_p_reg_n_0_[1] ;
  wire \int_p_reg_n_0_[2] ;
  wire \int_p_reg_n_0_[3] ;
  wire \int_p_reg_n_0_[4] ;
  wire \int_p_reg_n_0_[5] ;
  wire \int_p_reg_n_0_[6] ;
  wire \int_p_reg_n_0_[7] ;
  wire \int_p_reg_n_0_[8] ;
  wire \int_p_reg_n_0_[9] ;
  wire int_p_reg_n_100;
  wire int_p_reg_n_101;
  wire int_p_reg_n_102;
  wire int_p_reg_n_103;
  wire int_p_reg_n_104;
  wire int_p_reg_n_105;
  wire int_p_reg_n_58;
  wire int_p_reg_n_59;
  wire int_p_reg_n_60;
  wire int_p_reg_n_61;
  wire int_p_reg_n_62;
  wire int_p_reg_n_63;
  wire int_p_reg_n_64;
  wire int_p_reg_n_65;
  wire int_p_reg_n_66;
  wire int_p_reg_n_67;
  wire int_p_reg_n_68;
  wire int_p_reg_n_69;
  wire int_p_reg_n_70;
  wire int_p_reg_n_71;
  wire int_p_reg_n_72;
  wire int_p_reg_n_73;
  wire int_p_reg_n_74;
  wire int_p_reg_n_75;
  wire int_p_reg_n_76;
  wire int_p_reg_n_77;
  wire int_p_reg_n_78;
  wire int_p_reg_n_79;
  wire int_p_reg_n_80;
  wire int_p_reg_n_81;
  wire int_p_reg_n_82;
  wire int_p_reg_n_83;
  wire int_p_reg_n_84;
  wire int_p_reg_n_85;
  wire int_p_reg_n_86;
  wire int_p_reg_n_87;
  wire int_p_reg_n_88;
  wire int_p_reg_n_89;
  wire int_p_reg_n_90;
  wire int_p_reg_n_91;
  wire int_p_reg_n_92;
  wire int_p_reg_n_93;
  wire int_p_reg_n_94;
  wire int_p_reg_n_95;
  wire int_p_reg_n_96;
  wire int_p_reg_n_97;
  wire int_p_reg_n_98;
  wire int_p_reg_n_99;
  wire int_task_ap_done;
  wire int_task_ap_done0__4;
  wire int_task_ap_done_i_1_n_0;
  wire int_task_ap_done_i_3_n_0;
  wire interrupt;
  wire [7:2]p_4_in;
  wire \rdata[0]_i_1_n_0 ;
  wire \rdata[0]_i_2_n_0 ;
  wire \rdata[0]_i_4_n_0 ;
  wire \rdata[0]_i_5_n_0 ;
  wire \rdata[16]_i_2_n_0 ;
  wire \rdata[17]_i_2_n_0 ;
  wire \rdata[18]_i_2_n_0 ;
  wire \rdata[19]_i_2_n_0 ;
  wire \rdata[1]_i_3_n_0 ;
  wire \rdata[20]_i_2_n_0 ;
  wire \rdata[21]_i_2_n_0 ;
  wire \rdata[22]_i_2_n_0 ;
  wire \rdata[23]_i_2_n_0 ;
  wire \rdata[24]_i_2_n_0 ;
  wire \rdata[24]_i_3_n_0 ;
  wire \rdata[25]_i_2_n_0 ;
  wire \rdata[25]_i_3_n_0 ;
  wire \rdata[26]_i_2_n_0 ;
  wire \rdata[26]_i_3_n_0 ;
  wire \rdata[27]_i_2_n_0 ;
  wire \rdata[27]_i_3_n_0 ;
  wire \rdata[28]_i_2_n_0 ;
  wire \rdata[28]_i_3_n_0 ;
  wire \rdata[29]_i_2_n_0 ;
  wire \rdata[29]_i_3_n_0 ;
  wire \rdata[2]_i_3_n_0 ;
  wire \rdata[30]_i_2_n_0 ;
  wire \rdata[30]_i_3_n_0 ;
  wire \rdata[31]_i_1_n_0 ;
  wire \rdata[31]_i_4_n_0 ;
  wire \rdata[31]_i_5_n_0 ;
  wire \rdata[3]_i_3_n_0 ;
  wire \rdata[7]_i_3_n_0 ;
  wire \rdata[9]_i_3_n_0 ;
  wire \rdata_reg[0]_i_3_n_0 ;
  wire \rdata_reg[10]_0 ;
  wire \rdata_reg[11]_0 ;
  wire \rdata_reg[12]_0 ;
  wire \rdata_reg[13]_0 ;
  wire \rdata_reg[14]_0 ;
  wire \rdata_reg[15]_0 ;
  wire \rdata_reg[16]_0 ;
  wire \rdata_reg[16]_i_1_n_0 ;
  wire \rdata_reg[17]_0 ;
  wire \rdata_reg[17]_i_1_n_0 ;
  wire \rdata_reg[18]_0 ;
  wire \rdata_reg[18]_i_1_n_0 ;
  wire \rdata_reg[19]_0 ;
  wire \rdata_reg[19]_i_1_n_0 ;
  wire \rdata_reg[1]_0 ;
  wire \rdata_reg[20]_0 ;
  wire \rdata_reg[20]_i_1_n_0 ;
  wire \rdata_reg[21]_0 ;
  wire \rdata_reg[21]_i_1_n_0 ;
  wire \rdata_reg[22]_0 ;
  wire \rdata_reg[22]_i_1_n_0 ;
  wire \rdata_reg[23]_0 ;
  wire \rdata_reg[23]_i_1_n_0 ;
  wire \rdata_reg[24]_i_1_n_0 ;
  wire \rdata_reg[25]_i_1_n_0 ;
  wire \rdata_reg[26]_i_1_n_0 ;
  wire \rdata_reg[27]_i_1_n_0 ;
  wire \rdata_reg[28]_i_1_n_0 ;
  wire \rdata_reg[29]_i_1_n_0 ;
  wire \rdata_reg[2]_0 ;
  wire \rdata_reg[30]_i_1_n_0 ;
  wire [7:0]\rdata_reg[31]_0 ;
  wire \rdata_reg[31]_i_3_n_0 ;
  wire \rdata_reg[3]_0 ;
  wire \rdata_reg[4]_0 ;
  wire \rdata_reg[5]_0 ;
  wire \rdata_reg[6]_0 ;
  wire \rdata_reg[7]_0 ;
  wire \rdata_reg[8]_0 ;
  wire \rdata_reg[9]_0 ;
  wire [5:0]s_axi_CTRL_ARADDR;
  wire \s_axi_CTRL_ARADDR[5]_0 ;
  wire \s_axi_CTRL_ARADDR[5]_1 ;
  wire \s_axi_CTRL_ARADDR[5]_2 ;
  wire \s_axi_CTRL_ARADDR[5]_3 ;
  wire s_axi_CTRL_ARADDR_5_sn_1;
  wire s_axi_CTRL_ARVALID;
  wire [3:0]s_axi_CTRL_AWADDR;
  wire s_axi_CTRL_AWVALID;
  wire s_axi_CTRL_BREADY;
  wire s_axi_CTRL_BVALID;
  wire [31:0]s_axi_CTRL_RDATA;
  wire s_axi_CTRL_RREADY;
  wire s_axi_CTRL_RVALID;
  wire [31:0]s_axi_CTRL_WDATA;
  wire [31:0]\s_axi_CTRL_WDATA[31] ;
  wire [3:0]s_axi_CTRL_WSTRB;
  wire s_axi_CTRL_WVALID;
  wire [31:0]vp_fu_61_p00;
  wire [31:0]vp_fu_61_p10;
  wire waddr;
  wire \waddr_reg_n_0_[2] ;
  wire \waddr_reg_n_0_[3] ;
  wire \waddr_reg_n_0_[4] ;
  wire \waddr_reg_n_0_[5] ;
  wire NLW_int_p_reg_CARRYCASCOUT_UNCONNECTED;
  wire NLW_int_p_reg_MULTSIGNOUT_UNCONNECTED;
  wire NLW_int_p_reg_OVERFLOW_UNCONNECTED;
  wire NLW_int_p_reg_PATTERNBDETECT_UNCONNECTED;
  wire NLW_int_p_reg_PATTERNDETECT_UNCONNECTED;
  wire NLW_int_p_reg_UNDERFLOW_UNCONNECTED;
  wire [29:0]NLW_int_p_reg_ACOUT_UNCONNECTED;
  wire [17:0]NLW_int_p_reg_BCOUT_UNCONNECTED;
  wire [3:0]NLW_int_p_reg_CARRYOUT_UNCONNECTED;
  wire [47:0]NLW_int_p_reg_PCOUT_UNCONNECTED;
  wire [7:0]NLW_int_p_reg_XOROUT_UNCONNECTED;
  wire NLW_int_p_reg__0_CARRYCASCOUT_UNCONNECTED;
  wire NLW_int_p_reg__0_MULTSIGNOUT_UNCONNECTED;
  wire NLW_int_p_reg__0_OVERFLOW_UNCONNECTED;
  wire NLW_int_p_reg__0_PATTERNBDETECT_UNCONNECTED;
  wire NLW_int_p_reg__0_PATTERNDETECT_UNCONNECTED;
  wire NLW_int_p_reg__0_UNDERFLOW_UNCONNECTED;
  wire [29:0]NLW_int_p_reg__0_ACOUT_UNCONNECTED;
  wire [17:0]NLW_int_p_reg__0_BCOUT_UNCONNECTED;
  wire [3:0]NLW_int_p_reg__0_CARRYOUT_UNCONNECTED;
  wire [47:0]NLW_int_p_reg__0_PCOUT_UNCONNECTED;
  wire [7:0]NLW_int_p_reg__0_XOROUT_UNCONNECTED;

  assign s_axi_CTRL_ARADDR_5_sp_1 = s_axi_CTRL_ARADDR_5_sn_1;
  (* SOFT_HLUTNM = "soft_lutpair1" *) 
  LUT4 #(
    .INIT(16'hF747)) 
    \FSM_onehot_rstate[1]_i_1 
       (.I0(s_axi_CTRL_ARVALID),
        .I1(\FSM_onehot_rstate_reg[1]_0 ),
        .I2(s_axi_CTRL_RVALID),
        .I3(s_axi_CTRL_RREADY),
        .O(\FSM_onehot_rstate[1]_i_1_n_0 ));
  (* SOFT_HLUTNM = "soft_lutpair1" *) 
  LUT4 #(
    .INIT(16'h88F8)) 
    \FSM_onehot_rstate[2]_i_1 
       (.I0(s_axi_CTRL_ARVALID),
        .I1(\FSM_onehot_rstate_reg[1]_0 ),
        .I2(s_axi_CTRL_RVALID),
        .I3(s_axi_CTRL_RREADY),
        .O(\FSM_onehot_rstate[2]_i_1_n_0 ));
  (* FSM_ENCODED_STATES = "RDIDLE:010,RDDATA:100,iSTATE:001" *) 
  FDRE #(
    .INIT(1'b0)) 
    \FSM_onehot_rstate_reg[1] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\FSM_onehot_rstate[1]_i_1_n_0 ),
        .Q(\FSM_onehot_rstate_reg[1]_0 ),
        .R(RSTB));
  (* FSM_ENCODED_STATES = "RDIDLE:010,RDDATA:100,iSTATE:001" *) 
  FDRE #(
    .INIT(1'b0)) 
    \FSM_onehot_rstate_reg[2] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\FSM_onehot_rstate[2]_i_1_n_0 ),
        .Q(s_axi_CTRL_RVALID),
        .R(RSTB));
  LUT1 #(
    .INIT(2'h1)) 
    \FSM_onehot_wstate[1]_i_1 
       (.I0(ap_rst_n),
        .O(RSTB));
  LUT5 #(
    .INIT(32'h888BFF8B)) 
    \FSM_onehot_wstate[1]_i_2 
       (.I0(s_axi_CTRL_BREADY),
        .I1(s_axi_CTRL_BVALID),
        .I2(\FSM_onehot_wstate_reg[2]_0 ),
        .I3(\FSM_onehot_wstate_reg[1]_0 ),
        .I4(s_axi_CTRL_AWVALID),
        .O(\FSM_onehot_wstate[1]_i_2_n_0 ));
  LUT4 #(
    .INIT(16'h8F88)) 
    \FSM_onehot_wstate[2]_i_1 
       (.I0(s_axi_CTRL_AWVALID),
        .I1(\FSM_onehot_wstate_reg[1]_0 ),
        .I2(s_axi_CTRL_WVALID),
        .I3(\FSM_onehot_wstate_reg[2]_0 ),
        .O(\FSM_onehot_wstate[2]_i_1_n_0 ));
  LUT4 #(
    .INIT(16'h8F88)) 
    \FSM_onehot_wstate[3]_i_1 
       (.I0(s_axi_CTRL_WVALID),
        .I1(\FSM_onehot_wstate_reg[2]_0 ),
        .I2(s_axi_CTRL_BREADY),
        .I3(s_axi_CTRL_BVALID),
        .O(\FSM_onehot_wstate[3]_i_1_n_0 ));
  (* FSM_ENCODED_STATES = "WRDATA:0100,WRRESP:1000,WRIDLE:0010,iSTATE:0001" *) 
  FDRE #(
    .INIT(1'b0)) 
    \FSM_onehot_wstate_reg[1] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\FSM_onehot_wstate[1]_i_2_n_0 ),
        .Q(\FSM_onehot_wstate_reg[1]_0 ),
        .R(RSTB));
  (* FSM_ENCODED_STATES = "WRDATA:0100,WRRESP:1000,WRIDLE:0010,iSTATE:0001" *) 
  FDRE #(
    .INIT(1'b0)) 
    \FSM_onehot_wstate_reg[2] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\FSM_onehot_wstate[2]_i_1_n_0 ),
        .Q(\FSM_onehot_wstate_reg[2]_0 ),
        .R(RSTB));
  (* FSM_ENCODED_STATES = "WRDATA:0100,WRRESP:1000,WRIDLE:0010,iSTATE:0001" *) 
  FDRE #(
    .INIT(1'b0)) 
    \FSM_onehot_wstate_reg[3] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\FSM_onehot_wstate[3]_i_1_n_0 ),
        .Q(s_axi_CTRL_BVALID),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    auto_restart_status_reg
       (.C(ap_clk),
        .CE(1'b1),
        .D(p_4_in[7]),
        .Q(auto_restart_status_reg_n_0),
        .R(RSTB));
  (* SOFT_HLUTNM = "soft_lutpair4" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_a[0]_i_1 
       (.I0(s_axi_CTRL_WDATA[0]),
        .I1(s_axi_CTRL_WSTRB[0]),
        .I2(vp_fu_61_p10[0]),
        .O(D[0]));
  (* SOFT_HLUTNM = "soft_lutpair14" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_a[10]_i_1 
       (.I0(s_axi_CTRL_WDATA[10]),
        .I1(s_axi_CTRL_WSTRB[1]),
        .I2(vp_fu_61_p10[10]),
        .O(D[10]));
  (* SOFT_HLUTNM = "soft_lutpair15" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_a[11]_i_1 
       (.I0(s_axi_CTRL_WDATA[11]),
        .I1(s_axi_CTRL_WSTRB[1]),
        .I2(vp_fu_61_p10[11]),
        .O(D[11]));
  (* SOFT_HLUTNM = "soft_lutpair16" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_a[12]_i_1 
       (.I0(s_axi_CTRL_WDATA[12]),
        .I1(s_axi_CTRL_WSTRB[1]),
        .I2(vp_fu_61_p10[12]),
        .O(D[12]));
  (* SOFT_HLUTNM = "soft_lutpair17" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_a[13]_i_1 
       (.I0(s_axi_CTRL_WDATA[13]),
        .I1(s_axi_CTRL_WSTRB[1]),
        .I2(vp_fu_61_p10[13]),
        .O(D[13]));
  (* SOFT_HLUTNM = "soft_lutpair18" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_a[14]_i_1 
       (.I0(s_axi_CTRL_WDATA[14]),
        .I1(s_axi_CTRL_WSTRB[1]),
        .I2(vp_fu_61_p10[14]),
        .O(D[14]));
  (* SOFT_HLUTNM = "soft_lutpair19" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_a[15]_i_1 
       (.I0(s_axi_CTRL_WDATA[15]),
        .I1(s_axi_CTRL_WSTRB[1]),
        .I2(vp_fu_61_p10[15]),
        .O(D[15]));
  (* SOFT_HLUTNM = "soft_lutpair20" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_a[16]_i_1 
       (.I0(s_axi_CTRL_WDATA[16]),
        .I1(s_axi_CTRL_WSTRB[2]),
        .I2(vp_fu_61_p10[16]),
        .O(D[16]));
  (* SOFT_HLUTNM = "soft_lutpair21" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_a[17]_i_1 
       (.I0(s_axi_CTRL_WDATA[17]),
        .I1(s_axi_CTRL_WSTRB[2]),
        .I2(vp_fu_61_p10[17]),
        .O(int_a0[17]));
  (* SOFT_HLUTNM = "soft_lutpair22" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_a[18]_i_1 
       (.I0(s_axi_CTRL_WDATA[18]),
        .I1(s_axi_CTRL_WSTRB[2]),
        .I2(vp_fu_61_p10[18]),
        .O(int_a0[18]));
  (* SOFT_HLUTNM = "soft_lutpair23" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_a[19]_i_1 
       (.I0(s_axi_CTRL_WDATA[19]),
        .I1(s_axi_CTRL_WSTRB[2]),
        .I2(vp_fu_61_p10[19]),
        .O(int_a0[19]));
  (* SOFT_HLUTNM = "soft_lutpair5" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_a[1]_i_1 
       (.I0(s_axi_CTRL_WDATA[1]),
        .I1(s_axi_CTRL_WSTRB[0]),
        .I2(vp_fu_61_p10[1]),
        .O(D[1]));
  (* SOFT_HLUTNM = "soft_lutpair24" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_a[20]_i_1 
       (.I0(s_axi_CTRL_WDATA[20]),
        .I1(s_axi_CTRL_WSTRB[2]),
        .I2(vp_fu_61_p10[20]),
        .O(int_a0[20]));
  (* SOFT_HLUTNM = "soft_lutpair25" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_a[21]_i_1 
       (.I0(s_axi_CTRL_WDATA[21]),
        .I1(s_axi_CTRL_WSTRB[2]),
        .I2(vp_fu_61_p10[21]),
        .O(int_a0[21]));
  (* SOFT_HLUTNM = "soft_lutpair26" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_a[22]_i_1 
       (.I0(s_axi_CTRL_WDATA[22]),
        .I1(s_axi_CTRL_WSTRB[2]),
        .I2(vp_fu_61_p10[22]),
        .O(int_a0[22]));
  (* SOFT_HLUTNM = "soft_lutpair27" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_a[23]_i_1 
       (.I0(s_axi_CTRL_WDATA[23]),
        .I1(s_axi_CTRL_WSTRB[2]),
        .I2(vp_fu_61_p10[23]),
        .O(int_a0[23]));
  (* SOFT_HLUTNM = "soft_lutpair28" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_a[24]_i_1 
       (.I0(s_axi_CTRL_WDATA[24]),
        .I1(s_axi_CTRL_WSTRB[3]),
        .I2(vp_fu_61_p10[24]),
        .O(int_a0[24]));
  (* SOFT_HLUTNM = "soft_lutpair29" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_a[25]_i_1 
       (.I0(s_axi_CTRL_WDATA[25]),
        .I1(s_axi_CTRL_WSTRB[3]),
        .I2(vp_fu_61_p10[25]),
        .O(int_a0[25]));
  (* SOFT_HLUTNM = "soft_lutpair30" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_a[26]_i_1 
       (.I0(s_axi_CTRL_WDATA[26]),
        .I1(s_axi_CTRL_WSTRB[3]),
        .I2(vp_fu_61_p10[26]),
        .O(int_a0[26]));
  (* SOFT_HLUTNM = "soft_lutpair31" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_a[27]_i_1 
       (.I0(s_axi_CTRL_WDATA[27]),
        .I1(s_axi_CTRL_WSTRB[3]),
        .I2(vp_fu_61_p10[27]),
        .O(int_a0[27]));
  (* SOFT_HLUTNM = "soft_lutpair32" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_a[28]_i_1 
       (.I0(s_axi_CTRL_WDATA[28]),
        .I1(s_axi_CTRL_WSTRB[3]),
        .I2(vp_fu_61_p10[28]),
        .O(int_a0[28]));
  (* SOFT_HLUTNM = "soft_lutpair33" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_a[29]_i_1 
       (.I0(s_axi_CTRL_WDATA[29]),
        .I1(s_axi_CTRL_WSTRB[3]),
        .I2(vp_fu_61_p10[29]),
        .O(int_a0[29]));
  (* SOFT_HLUTNM = "soft_lutpair6" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_a[2]_i_1 
       (.I0(s_axi_CTRL_WDATA[2]),
        .I1(s_axi_CTRL_WSTRB[0]),
        .I2(vp_fu_61_p10[2]),
        .O(D[2]));
  (* SOFT_HLUTNM = "soft_lutpair34" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_a[30]_i_1 
       (.I0(s_axi_CTRL_WDATA[30]),
        .I1(s_axi_CTRL_WSTRB[3]),
        .I2(vp_fu_61_p10[30]),
        .O(int_a0[30]));
  LUT6 #(
    .INIT(64'h0000000000002000)) 
    \int_a[31]_i_1 
       (.I0(\waddr_reg_n_0_[4] ),
        .I1(\waddr_reg_n_0_[5] ),
        .I2(\FSM_onehot_wstate_reg[2]_0 ),
        .I3(s_axi_CTRL_WVALID),
        .I4(\waddr_reg_n_0_[2] ),
        .I5(\waddr_reg_n_0_[3] ),
        .O(CEB2));
  (* SOFT_HLUTNM = "soft_lutpair35" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_a[31]_i_2 
       (.I0(s_axi_CTRL_WDATA[31]),
        .I1(s_axi_CTRL_WSTRB[3]),
        .I2(vp_fu_61_p10[31]),
        .O(int_a0[31]));
  (* SOFT_HLUTNM = "soft_lutpair7" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_a[3]_i_1 
       (.I0(s_axi_CTRL_WDATA[3]),
        .I1(s_axi_CTRL_WSTRB[0]),
        .I2(vp_fu_61_p10[3]),
        .O(D[3]));
  (* SOFT_HLUTNM = "soft_lutpair8" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_a[4]_i_1 
       (.I0(s_axi_CTRL_WDATA[4]),
        .I1(s_axi_CTRL_WSTRB[0]),
        .I2(vp_fu_61_p10[4]),
        .O(D[4]));
  (* SOFT_HLUTNM = "soft_lutpair9" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_a[5]_i_1 
       (.I0(s_axi_CTRL_WDATA[5]),
        .I1(s_axi_CTRL_WSTRB[0]),
        .I2(vp_fu_61_p10[5]),
        .O(D[5]));
  (* SOFT_HLUTNM = "soft_lutpair10" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_a[6]_i_1 
       (.I0(s_axi_CTRL_WDATA[6]),
        .I1(s_axi_CTRL_WSTRB[0]),
        .I2(vp_fu_61_p10[6]),
        .O(D[6]));
  (* SOFT_HLUTNM = "soft_lutpair11" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_a[7]_i_1 
       (.I0(s_axi_CTRL_WDATA[7]),
        .I1(s_axi_CTRL_WSTRB[0]),
        .I2(vp_fu_61_p10[7]),
        .O(D[7]));
  (* SOFT_HLUTNM = "soft_lutpair12" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_a[8]_i_1 
       (.I0(s_axi_CTRL_WDATA[8]),
        .I1(s_axi_CTRL_WSTRB[1]),
        .I2(vp_fu_61_p10[8]),
        .O(D[8]));
  (* SOFT_HLUTNM = "soft_lutpair13" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_a[9]_i_1 
       (.I0(s_axi_CTRL_WDATA[9]),
        .I1(s_axi_CTRL_WSTRB[1]),
        .I2(vp_fu_61_p10[9]),
        .O(D[9]));
  FDRE #(
    .INIT(1'b0)) 
    \int_a_reg[0] 
       (.C(ap_clk),
        .CE(CEB2),
        .D(D[0]),
        .Q(vp_fu_61_p10[0]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_a_reg[10] 
       (.C(ap_clk),
        .CE(CEB2),
        .D(D[10]),
        .Q(vp_fu_61_p10[10]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_a_reg[11] 
       (.C(ap_clk),
        .CE(CEB2),
        .D(D[11]),
        .Q(vp_fu_61_p10[11]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_a_reg[12] 
       (.C(ap_clk),
        .CE(CEB2),
        .D(D[12]),
        .Q(vp_fu_61_p10[12]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_a_reg[13] 
       (.C(ap_clk),
        .CE(CEB2),
        .D(D[13]),
        .Q(vp_fu_61_p10[13]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_a_reg[14] 
       (.C(ap_clk),
        .CE(CEB2),
        .D(D[14]),
        .Q(vp_fu_61_p10[14]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_a_reg[15] 
       (.C(ap_clk),
        .CE(CEB2),
        .D(D[15]),
        .Q(vp_fu_61_p10[15]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_a_reg[16] 
       (.C(ap_clk),
        .CE(CEB2),
        .D(D[16]),
        .Q(vp_fu_61_p10[16]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_a_reg[17] 
       (.C(ap_clk),
        .CE(CEB2),
        .D(int_a0[17]),
        .Q(vp_fu_61_p10[17]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_a_reg[18] 
       (.C(ap_clk),
        .CE(CEB2),
        .D(int_a0[18]),
        .Q(vp_fu_61_p10[18]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_a_reg[19] 
       (.C(ap_clk),
        .CE(CEB2),
        .D(int_a0[19]),
        .Q(vp_fu_61_p10[19]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_a_reg[1] 
       (.C(ap_clk),
        .CE(CEB2),
        .D(D[1]),
        .Q(vp_fu_61_p10[1]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_a_reg[20] 
       (.C(ap_clk),
        .CE(CEB2),
        .D(int_a0[20]),
        .Q(vp_fu_61_p10[20]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_a_reg[21] 
       (.C(ap_clk),
        .CE(CEB2),
        .D(int_a0[21]),
        .Q(vp_fu_61_p10[21]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_a_reg[22] 
       (.C(ap_clk),
        .CE(CEB2),
        .D(int_a0[22]),
        .Q(vp_fu_61_p10[22]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_a_reg[23] 
       (.C(ap_clk),
        .CE(CEB2),
        .D(int_a0[23]),
        .Q(vp_fu_61_p10[23]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_a_reg[24] 
       (.C(ap_clk),
        .CE(CEB2),
        .D(int_a0[24]),
        .Q(vp_fu_61_p10[24]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_a_reg[25] 
       (.C(ap_clk),
        .CE(CEB2),
        .D(int_a0[25]),
        .Q(vp_fu_61_p10[25]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_a_reg[26] 
       (.C(ap_clk),
        .CE(CEB2),
        .D(int_a0[26]),
        .Q(vp_fu_61_p10[26]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_a_reg[27] 
       (.C(ap_clk),
        .CE(CEB2),
        .D(int_a0[27]),
        .Q(vp_fu_61_p10[27]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_a_reg[28] 
       (.C(ap_clk),
        .CE(CEB2),
        .D(int_a0[28]),
        .Q(vp_fu_61_p10[28]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_a_reg[29] 
       (.C(ap_clk),
        .CE(CEB2),
        .D(int_a0[29]),
        .Q(vp_fu_61_p10[29]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_a_reg[2] 
       (.C(ap_clk),
        .CE(CEB2),
        .D(D[2]),
        .Q(vp_fu_61_p10[2]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_a_reg[30] 
       (.C(ap_clk),
        .CE(CEB2),
        .D(int_a0[30]),
        .Q(vp_fu_61_p10[30]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_a_reg[31] 
       (.C(ap_clk),
        .CE(CEB2),
        .D(int_a0[31]),
        .Q(vp_fu_61_p10[31]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_a_reg[3] 
       (.C(ap_clk),
        .CE(CEB2),
        .D(D[3]),
        .Q(vp_fu_61_p10[3]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_a_reg[4] 
       (.C(ap_clk),
        .CE(CEB2),
        .D(D[4]),
        .Q(vp_fu_61_p10[4]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_a_reg[5] 
       (.C(ap_clk),
        .CE(CEB2),
        .D(D[5]),
        .Q(vp_fu_61_p10[5]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_a_reg[6] 
       (.C(ap_clk),
        .CE(CEB2),
        .D(D[6]),
        .Q(vp_fu_61_p10[6]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_a_reg[7] 
       (.C(ap_clk),
        .CE(CEB2),
        .D(D[7]),
        .Q(vp_fu_61_p10[7]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_a_reg[8] 
       (.C(ap_clk),
        .CE(CEB2),
        .D(D[8]),
        .Q(vp_fu_61_p10[8]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_a_reg[9] 
       (.C(ap_clk),
        .CE(CEB2),
        .D(D[9]),
        .Q(vp_fu_61_p10[9]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    int_ap_idle_reg
       (.C(ap_clk),
        .CE(1'b1),
        .D(1'b1),
        .Q(p_4_in[2]),
        .R(RSTB));
  (* SOFT_HLUTNM = "soft_lutpair3" *) 
  LUT4 #(
    .INIT(16'h4F44)) 
    int_ap_ready_i_1
       (.I0(p_4_in[7]),
        .I1(ap_done),
        .I2(int_task_ap_done0__4),
        .I3(int_ap_ready),
        .O(int_ap_ready_i_1_n_0));
  FDRE #(
    .INIT(1'b0)) 
    int_ap_ready_reg
       (.C(ap_clk),
        .CE(1'b1),
        .D(int_ap_ready_i_1_n_0),
        .Q(int_ap_ready),
        .R(RSTB));
  LUT6 #(
    .INIT(64'h88F8888888888888)) 
    int_ap_start_i_1
       (.I0(p_4_in[7]),
        .I1(ap_done),
        .I2(s_axi_CTRL_WSTRB[0]),
        .I3(\waddr_reg_n_0_[3] ),
        .I4(int_ap_start_i_2_n_0),
        .I5(s_axi_CTRL_WDATA[0]),
        .O(int_ap_start_i_1_n_0));
  (* SOFT_HLUTNM = "soft_lutpair0" *) 
  LUT5 #(
    .INIT(32'h00000040)) 
    int_ap_start_i_2
       (.I0(\waddr_reg_n_0_[4] ),
        .I1(s_axi_CTRL_WVALID),
        .I2(\FSM_onehot_wstate_reg[2]_0 ),
        .I3(\waddr_reg_n_0_[5] ),
        .I4(\waddr_reg_n_0_[2] ),
        .O(int_ap_start_i_2_n_0));
  FDRE #(
    .INIT(1'b0)) 
    int_ap_start_reg
       (.C(ap_clk),
        .CE(1'b1),
        .D(int_ap_start_i_1_n_0),
        .Q(ap_done),
        .R(RSTB));
  LUT5 #(
    .INIT(32'hFBFF0800)) 
    int_auto_restart_i_1
       (.I0(s_axi_CTRL_WDATA[7]),
        .I1(s_axi_CTRL_WSTRB[0]),
        .I2(\waddr_reg_n_0_[3] ),
        .I3(int_ap_start_i_2_n_0),
        .I4(p_4_in[7]),
        .O(int_auto_restart_i_1_n_0));
  FDRE #(
    .INIT(1'b0)) 
    int_auto_restart_reg
       (.C(ap_clk),
        .CE(1'b1),
        .D(int_auto_restart_i_1_n_0),
        .Q(p_4_in[7]),
        .R(RSTB));
  (* SOFT_HLUTNM = "soft_lutpair4" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_b[0]_i_1 
       (.I0(s_axi_CTRL_WDATA[0]),
        .I1(s_axi_CTRL_WSTRB[0]),
        .I2(vp_fu_61_p00[0]),
        .O(\s_axi_CTRL_WDATA[31] [0]));
  (* SOFT_HLUTNM = "soft_lutpair14" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_b[10]_i_1 
       (.I0(s_axi_CTRL_WDATA[10]),
        .I1(s_axi_CTRL_WSTRB[1]),
        .I2(vp_fu_61_p00[10]),
        .O(\s_axi_CTRL_WDATA[31] [10]));
  (* SOFT_HLUTNM = "soft_lutpair15" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_b[11]_i_1 
       (.I0(s_axi_CTRL_WDATA[11]),
        .I1(s_axi_CTRL_WSTRB[1]),
        .I2(vp_fu_61_p00[11]),
        .O(\s_axi_CTRL_WDATA[31] [11]));
  (* SOFT_HLUTNM = "soft_lutpair16" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_b[12]_i_1 
       (.I0(s_axi_CTRL_WDATA[12]),
        .I1(s_axi_CTRL_WSTRB[1]),
        .I2(vp_fu_61_p00[12]),
        .O(\s_axi_CTRL_WDATA[31] [12]));
  (* SOFT_HLUTNM = "soft_lutpair17" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_b[13]_i_1 
       (.I0(s_axi_CTRL_WDATA[13]),
        .I1(s_axi_CTRL_WSTRB[1]),
        .I2(vp_fu_61_p00[13]),
        .O(\s_axi_CTRL_WDATA[31] [13]));
  (* SOFT_HLUTNM = "soft_lutpair18" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_b[14]_i_1 
       (.I0(s_axi_CTRL_WDATA[14]),
        .I1(s_axi_CTRL_WSTRB[1]),
        .I2(vp_fu_61_p00[14]),
        .O(\s_axi_CTRL_WDATA[31] [14]));
  (* SOFT_HLUTNM = "soft_lutpair19" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_b[15]_i_1 
       (.I0(s_axi_CTRL_WDATA[15]),
        .I1(s_axi_CTRL_WSTRB[1]),
        .I2(vp_fu_61_p00[15]),
        .O(\s_axi_CTRL_WDATA[31] [15]));
  (* SOFT_HLUTNM = "soft_lutpair20" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_b[16]_i_1 
       (.I0(s_axi_CTRL_WDATA[16]),
        .I1(s_axi_CTRL_WSTRB[2]),
        .I2(vp_fu_61_p00[16]),
        .O(\s_axi_CTRL_WDATA[31] [16]));
  (* SOFT_HLUTNM = "soft_lutpair21" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_b[17]_i_1 
       (.I0(s_axi_CTRL_WDATA[17]),
        .I1(s_axi_CTRL_WSTRB[2]),
        .I2(vp_fu_61_p00[17]),
        .O(\s_axi_CTRL_WDATA[31] [17]));
  (* SOFT_HLUTNM = "soft_lutpair22" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_b[18]_i_1 
       (.I0(s_axi_CTRL_WDATA[18]),
        .I1(s_axi_CTRL_WSTRB[2]),
        .I2(vp_fu_61_p00[18]),
        .O(\s_axi_CTRL_WDATA[31] [18]));
  (* SOFT_HLUTNM = "soft_lutpair23" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_b[19]_i_1 
       (.I0(s_axi_CTRL_WDATA[19]),
        .I1(s_axi_CTRL_WSTRB[2]),
        .I2(vp_fu_61_p00[19]),
        .O(\s_axi_CTRL_WDATA[31] [19]));
  (* SOFT_HLUTNM = "soft_lutpair5" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_b[1]_i_1 
       (.I0(s_axi_CTRL_WDATA[1]),
        .I1(s_axi_CTRL_WSTRB[0]),
        .I2(vp_fu_61_p00[1]),
        .O(\s_axi_CTRL_WDATA[31] [1]));
  (* SOFT_HLUTNM = "soft_lutpair24" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_b[20]_i_1 
       (.I0(s_axi_CTRL_WDATA[20]),
        .I1(s_axi_CTRL_WSTRB[2]),
        .I2(vp_fu_61_p00[20]),
        .O(\s_axi_CTRL_WDATA[31] [20]));
  (* SOFT_HLUTNM = "soft_lutpair25" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_b[21]_i_1 
       (.I0(s_axi_CTRL_WDATA[21]),
        .I1(s_axi_CTRL_WSTRB[2]),
        .I2(vp_fu_61_p00[21]),
        .O(\s_axi_CTRL_WDATA[31] [21]));
  (* SOFT_HLUTNM = "soft_lutpair26" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_b[22]_i_1 
       (.I0(s_axi_CTRL_WDATA[22]),
        .I1(s_axi_CTRL_WSTRB[2]),
        .I2(vp_fu_61_p00[22]),
        .O(\s_axi_CTRL_WDATA[31] [22]));
  (* SOFT_HLUTNM = "soft_lutpair27" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_b[23]_i_1 
       (.I0(s_axi_CTRL_WDATA[23]),
        .I1(s_axi_CTRL_WSTRB[2]),
        .I2(vp_fu_61_p00[23]),
        .O(\s_axi_CTRL_WDATA[31] [23]));
  (* SOFT_HLUTNM = "soft_lutpair28" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_b[24]_i_1 
       (.I0(s_axi_CTRL_WDATA[24]),
        .I1(s_axi_CTRL_WSTRB[3]),
        .I2(vp_fu_61_p00[24]),
        .O(\s_axi_CTRL_WDATA[31] [24]));
  (* SOFT_HLUTNM = "soft_lutpair29" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_b[25]_i_1 
       (.I0(s_axi_CTRL_WDATA[25]),
        .I1(s_axi_CTRL_WSTRB[3]),
        .I2(vp_fu_61_p00[25]),
        .O(\s_axi_CTRL_WDATA[31] [25]));
  (* SOFT_HLUTNM = "soft_lutpair30" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_b[26]_i_1 
       (.I0(s_axi_CTRL_WDATA[26]),
        .I1(s_axi_CTRL_WSTRB[3]),
        .I2(vp_fu_61_p00[26]),
        .O(\s_axi_CTRL_WDATA[31] [26]));
  (* SOFT_HLUTNM = "soft_lutpair31" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_b[27]_i_1 
       (.I0(s_axi_CTRL_WDATA[27]),
        .I1(s_axi_CTRL_WSTRB[3]),
        .I2(vp_fu_61_p00[27]),
        .O(\s_axi_CTRL_WDATA[31] [27]));
  (* SOFT_HLUTNM = "soft_lutpair32" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_b[28]_i_1 
       (.I0(s_axi_CTRL_WDATA[28]),
        .I1(s_axi_CTRL_WSTRB[3]),
        .I2(vp_fu_61_p00[28]),
        .O(\s_axi_CTRL_WDATA[31] [28]));
  (* SOFT_HLUTNM = "soft_lutpair33" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_b[29]_i_1 
       (.I0(s_axi_CTRL_WDATA[29]),
        .I1(s_axi_CTRL_WSTRB[3]),
        .I2(vp_fu_61_p00[29]),
        .O(\s_axi_CTRL_WDATA[31] [29]));
  (* SOFT_HLUTNM = "soft_lutpair6" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_b[2]_i_1 
       (.I0(s_axi_CTRL_WDATA[2]),
        .I1(s_axi_CTRL_WSTRB[0]),
        .I2(vp_fu_61_p00[2]),
        .O(\s_axi_CTRL_WDATA[31] [2]));
  (* SOFT_HLUTNM = "soft_lutpair34" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_b[30]_i_1 
       (.I0(s_axi_CTRL_WDATA[30]),
        .I1(s_axi_CTRL_WSTRB[3]),
        .I2(vp_fu_61_p00[30]),
        .O(\s_axi_CTRL_WDATA[31] [30]));
  LUT6 #(
    .INIT(64'h0000000008000000)) 
    \int_b[31]_i_1 
       (.I0(\waddr_reg_n_0_[3] ),
        .I1(\waddr_reg_n_0_[4] ),
        .I2(\waddr_reg_n_0_[5] ),
        .I3(\FSM_onehot_wstate_reg[2]_0 ),
        .I4(s_axi_CTRL_WVALID),
        .I5(\waddr_reg_n_0_[2] ),
        .O(E));
  (* SOFT_HLUTNM = "soft_lutpair35" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_b[31]_i_2 
       (.I0(s_axi_CTRL_WDATA[31]),
        .I1(s_axi_CTRL_WSTRB[3]),
        .I2(vp_fu_61_p00[31]),
        .O(\s_axi_CTRL_WDATA[31] [31]));
  (* SOFT_HLUTNM = "soft_lutpair7" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_b[3]_i_1 
       (.I0(s_axi_CTRL_WDATA[3]),
        .I1(s_axi_CTRL_WSTRB[0]),
        .I2(vp_fu_61_p00[3]),
        .O(\s_axi_CTRL_WDATA[31] [3]));
  (* SOFT_HLUTNM = "soft_lutpair8" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_b[4]_i_1 
       (.I0(s_axi_CTRL_WDATA[4]),
        .I1(s_axi_CTRL_WSTRB[0]),
        .I2(vp_fu_61_p00[4]),
        .O(\s_axi_CTRL_WDATA[31] [4]));
  (* SOFT_HLUTNM = "soft_lutpair9" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_b[5]_i_1 
       (.I0(s_axi_CTRL_WDATA[5]),
        .I1(s_axi_CTRL_WSTRB[0]),
        .I2(vp_fu_61_p00[5]),
        .O(\s_axi_CTRL_WDATA[31] [5]));
  (* SOFT_HLUTNM = "soft_lutpair10" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_b[6]_i_1 
       (.I0(s_axi_CTRL_WDATA[6]),
        .I1(s_axi_CTRL_WSTRB[0]),
        .I2(vp_fu_61_p00[6]),
        .O(\s_axi_CTRL_WDATA[31] [6]));
  (* SOFT_HLUTNM = "soft_lutpair11" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_b[7]_i_1 
       (.I0(s_axi_CTRL_WDATA[7]),
        .I1(s_axi_CTRL_WSTRB[0]),
        .I2(vp_fu_61_p00[7]),
        .O(\s_axi_CTRL_WDATA[31] [7]));
  (* SOFT_HLUTNM = "soft_lutpair12" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_b[8]_i_1 
       (.I0(s_axi_CTRL_WDATA[8]),
        .I1(s_axi_CTRL_WSTRB[1]),
        .I2(vp_fu_61_p00[8]),
        .O(\s_axi_CTRL_WDATA[31] [8]));
  (* SOFT_HLUTNM = "soft_lutpair13" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \int_b[9]_i_1 
       (.I0(s_axi_CTRL_WDATA[9]),
        .I1(s_axi_CTRL_WSTRB[1]),
        .I2(vp_fu_61_p00[9]),
        .O(\s_axi_CTRL_WDATA[31] [9]));
  FDRE #(
    .INIT(1'b0)) 
    \int_b_reg[0] 
       (.C(ap_clk),
        .CE(E),
        .D(\s_axi_CTRL_WDATA[31] [0]),
        .Q(vp_fu_61_p00[0]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_b_reg[10] 
       (.C(ap_clk),
        .CE(E),
        .D(\s_axi_CTRL_WDATA[31] [10]),
        .Q(vp_fu_61_p00[10]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_b_reg[11] 
       (.C(ap_clk),
        .CE(E),
        .D(\s_axi_CTRL_WDATA[31] [11]),
        .Q(vp_fu_61_p00[11]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_b_reg[12] 
       (.C(ap_clk),
        .CE(E),
        .D(\s_axi_CTRL_WDATA[31] [12]),
        .Q(vp_fu_61_p00[12]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_b_reg[13] 
       (.C(ap_clk),
        .CE(E),
        .D(\s_axi_CTRL_WDATA[31] [13]),
        .Q(vp_fu_61_p00[13]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_b_reg[14] 
       (.C(ap_clk),
        .CE(E),
        .D(\s_axi_CTRL_WDATA[31] [14]),
        .Q(vp_fu_61_p00[14]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_b_reg[15] 
       (.C(ap_clk),
        .CE(E),
        .D(\s_axi_CTRL_WDATA[31] [15]),
        .Q(vp_fu_61_p00[15]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_b_reg[16] 
       (.C(ap_clk),
        .CE(E),
        .D(\s_axi_CTRL_WDATA[31] [16]),
        .Q(vp_fu_61_p00[16]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_b_reg[17] 
       (.C(ap_clk),
        .CE(E),
        .D(\s_axi_CTRL_WDATA[31] [17]),
        .Q(vp_fu_61_p00[17]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_b_reg[18] 
       (.C(ap_clk),
        .CE(E),
        .D(\s_axi_CTRL_WDATA[31] [18]),
        .Q(vp_fu_61_p00[18]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_b_reg[19] 
       (.C(ap_clk),
        .CE(E),
        .D(\s_axi_CTRL_WDATA[31] [19]),
        .Q(vp_fu_61_p00[19]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_b_reg[1] 
       (.C(ap_clk),
        .CE(E),
        .D(\s_axi_CTRL_WDATA[31] [1]),
        .Q(vp_fu_61_p00[1]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_b_reg[20] 
       (.C(ap_clk),
        .CE(E),
        .D(\s_axi_CTRL_WDATA[31] [20]),
        .Q(vp_fu_61_p00[20]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_b_reg[21] 
       (.C(ap_clk),
        .CE(E),
        .D(\s_axi_CTRL_WDATA[31] [21]),
        .Q(vp_fu_61_p00[21]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_b_reg[22] 
       (.C(ap_clk),
        .CE(E),
        .D(\s_axi_CTRL_WDATA[31] [22]),
        .Q(vp_fu_61_p00[22]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_b_reg[23] 
       (.C(ap_clk),
        .CE(E),
        .D(\s_axi_CTRL_WDATA[31] [23]),
        .Q(vp_fu_61_p00[23]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_b_reg[24] 
       (.C(ap_clk),
        .CE(E),
        .D(\s_axi_CTRL_WDATA[31] [24]),
        .Q(vp_fu_61_p00[24]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_b_reg[25] 
       (.C(ap_clk),
        .CE(E),
        .D(\s_axi_CTRL_WDATA[31] [25]),
        .Q(vp_fu_61_p00[25]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_b_reg[26] 
       (.C(ap_clk),
        .CE(E),
        .D(\s_axi_CTRL_WDATA[31] [26]),
        .Q(vp_fu_61_p00[26]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_b_reg[27] 
       (.C(ap_clk),
        .CE(E),
        .D(\s_axi_CTRL_WDATA[31] [27]),
        .Q(vp_fu_61_p00[27]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_b_reg[28] 
       (.C(ap_clk),
        .CE(E),
        .D(\s_axi_CTRL_WDATA[31] [28]),
        .Q(vp_fu_61_p00[28]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_b_reg[29] 
       (.C(ap_clk),
        .CE(E),
        .D(\s_axi_CTRL_WDATA[31] [29]),
        .Q(vp_fu_61_p00[29]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_b_reg[2] 
       (.C(ap_clk),
        .CE(E),
        .D(\s_axi_CTRL_WDATA[31] [2]),
        .Q(vp_fu_61_p00[2]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_b_reg[30] 
       (.C(ap_clk),
        .CE(E),
        .D(\s_axi_CTRL_WDATA[31] [30]),
        .Q(vp_fu_61_p00[30]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_b_reg[31] 
       (.C(ap_clk),
        .CE(E),
        .D(\s_axi_CTRL_WDATA[31] [31]),
        .Q(vp_fu_61_p00[31]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_b_reg[3] 
       (.C(ap_clk),
        .CE(E),
        .D(\s_axi_CTRL_WDATA[31] [3]),
        .Q(vp_fu_61_p00[3]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_b_reg[4] 
       (.C(ap_clk),
        .CE(E),
        .D(\s_axi_CTRL_WDATA[31] [4]),
        .Q(vp_fu_61_p00[4]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_b_reg[5] 
       (.C(ap_clk),
        .CE(E),
        .D(\s_axi_CTRL_WDATA[31] [5]),
        .Q(vp_fu_61_p00[5]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_b_reg[6] 
       (.C(ap_clk),
        .CE(E),
        .D(\s_axi_CTRL_WDATA[31] [6]),
        .Q(vp_fu_61_p00[6]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_b_reg[7] 
       (.C(ap_clk),
        .CE(E),
        .D(\s_axi_CTRL_WDATA[31] [7]),
        .Q(vp_fu_61_p00[7]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_b_reg[8] 
       (.C(ap_clk),
        .CE(E),
        .D(\s_axi_CTRL_WDATA[31] [8]),
        .Q(vp_fu_61_p00[8]),
        .R(RSTB));
  FDRE #(
    .INIT(1'b0)) 
    \int_b_reg[9] 
       (.C(ap_clk),
        .CE(E),
        .D(\s_axi_CTRL_WDATA[31] [9]),
        .Q(vp_fu_61_p00[9]),
        .R(RSTB));
  LUT5 #(
    .INIT(32'hFBFF0800)) 
    int_gie_i_1
       (.I0(s_axi_CTRL_WDATA[0]),
        .I1(s_axi_CTRL_WSTRB[0]),
        .I2(\waddr_reg_n_0_[3] ),
        .I3(int_isr_i_2_n_0),
        .I4(int_gie_reg_n_0),
        .O(int_gie_i_1_n_0));
  FDRE #(
    .INIT(1'b0)) 
    int_gie_reg
       (.C(ap_clk),
        .CE(1'b1),
        .D(int_gie_i_1_n_0),
        .Q(int_gie_reg_n_0),
        .R(RSTB));
  LUT5 #(
    .INIT(32'hBFFF8000)) 
    int_ier_i_1
       (.I0(s_axi_CTRL_WDATA[0]),
        .I1(s_axi_CTRL_WSTRB[0]),
        .I2(int_ap_start_i_2_n_0),
        .I3(\waddr_reg_n_0_[3] ),
        .I4(int_ier),
        .O(int_ier_i_1_n_0));
  FDRE #(
    .INIT(1'b0)) 
    int_ier_reg
       (.C(ap_clk),
        .CE(1'b1),
        .D(int_ier_i_1_n_0),
        .Q(int_ier),
        .R(RSTB));
  (* SOFT_HLUTNM = "soft_lutpair2" *) 
  LUT2 #(
    .INIT(4'h8)) 
    int_interrupt_i_1
       (.I0(int_interrupt1),
        .I1(int_gie_reg_n_0),
        .O(int_interrupt0));
  FDRE #(
    .INIT(1'b0)) 
    int_interrupt_reg
       (.C(ap_clk),
        .CE(1'b1),
        .D(int_interrupt0),
        .Q(interrupt),
        .R(RSTB));
  LUT6 #(
    .INIT(64'hFFFF7FFFFFFF8000)) 
    int_isr_i_1
       (.I0(s_axi_CTRL_WDATA[0]),
        .I1(\waddr_reg_n_0_[3] ),
        .I2(int_isr_i_2_n_0),
        .I3(s_axi_CTRL_WSTRB[0]),
        .I4(int_isr6_out),
        .I5(int_interrupt1),
        .O(int_isr_i_1_n_0));
  (* SOFT_HLUTNM = "soft_lutpair0" *) 
  LUT5 #(
    .INIT(32'h00002000)) 
    int_isr_i_2
       (.I0(\waddr_reg_n_0_[2] ),
        .I1(\waddr_reg_n_0_[4] ),
        .I2(s_axi_CTRL_WVALID),
        .I3(\FSM_onehot_wstate_reg[2]_0 ),
        .I4(\waddr_reg_n_0_[5] ),
        .O(int_isr_i_2_n_0));
  (* SOFT_HLUTNM = "soft_lutpair3" *) 
  LUT2 #(
    .INIT(4'h8)) 
    int_isr_i_3
       (.I0(ap_done),
        .I1(int_ier),
        .O(int_isr6_out));
  FDRE #(
    .INIT(1'b0)) 
    int_isr_reg
       (.C(ap_clk),
        .CE(1'b1),
        .D(int_isr_i_1_n_0),
        .Q(int_interrupt1),
        .R(RSTB));
  LUT5 #(
    .INIT(32'hBFFFAAAA)) 
    int_p_ap_vld_i_1
       (.I0(ap_done),
        .I1(int_p_ap_vld1),
        .I2(\FSM_onehot_rstate_reg[1]_0 ),
        .I3(s_axi_CTRL_ARVALID),
        .I4(int_p_ap_vld),
        .O(int_p_ap_vld_i_1_n_0));
  LUT6 #(
    .INIT(64'h0000000000001000)) 
    int_p_ap_vld_i_2
       (.I0(s_axi_CTRL_ARADDR[1]),
        .I1(s_axi_CTRL_ARADDR[4]),
        .I2(s_axi_CTRL_ARADDR[5]),
        .I3(s_axi_CTRL_ARADDR[3]),
        .I4(s_axi_CTRL_ARADDR[0]),
        .I5(s_axi_CTRL_ARADDR[2]),
        .O(int_p_ap_vld1));
  FDRE int_p_ap_vld_reg
       (.C(ap_clk),
        .CE(1'b1),
        .D(int_p_ap_vld_i_1_n_0),
        .Q(int_p_ap_vld),
        .R(RSTB));
  (* KEEP_HIERARCHY = "yes" *) 
  (* METHODOLOGY_DRC_VIOS = "{SYNTH-10 {cell *THIS*} {string 16x16 4}}" *) 
  DSP48E2 #(
    .ACASCREG(1),
    .ADREG(1),
    .ALUMODEREG(0),
    .AMULTSEL("A"),
    .AREG(1),
    .AUTORESET_PATDET("NO_RESET"),
    .AUTORESET_PRIORITY("RESET"),
    .A_INPUT("DIRECT"),
    .BCASCREG(1),
    .BMULTSEL("B"),
    .BREG(1),
    .B_INPUT("DIRECT"),
    .CARRYINREG(0),
    .CARRYINSELREG(0),
    .CREG(1),
    .DREG(1),
    .INMODEREG(0),
    .MASK(48'h3FFFFFFFFFFF),
    .MREG(0),
    .OPMODEREG(0),
    .PATTERN(48'h000000000000),
    .PREADDINSEL("A"),
    .PREG(1),
    .RND(48'h000000000000),
    .SEL_MASK("MASK"),
    .SEL_PATTERN("PATTERN"),
    .USE_MULT("MULTIPLY"),
    .USE_PATTERN_DETECT("NO_PATDET"),
    .USE_SIMD("ONE48"),
    .USE_WIDEXOR("FALSE"),
    .XORSIMD("XOR24_48_96")) 
    int_p_reg
       (.A({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,\s_axi_CTRL_WDATA[31] [31:17]}),
        .ACIN({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .ACOUT(NLW_int_p_reg_ACOUT_UNCONNECTED[29:0]),
        .ALUMODE({1'b0,1'b0,1'b0,1'b0}),
        .B({1'b0,1'b0,1'b0,int_a0}),
        .BCIN({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .BCOUT(NLW_int_p_reg_BCOUT_UNCONNECTED[17:0]),
        .C({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .CARRYCASCIN(1'b0),
        .CARRYCASCOUT(NLW_int_p_reg_CARRYCASCOUT_UNCONNECTED),
        .CARRYIN(1'b0),
        .CARRYINSEL({1'b0,1'b0,1'b0}),
        .CARRYOUT(NLW_int_p_reg_CARRYOUT_UNCONNECTED[3:0]),
        .CEA1(1'b0),
        .CEA2(E),
        .CEAD(1'b0),
        .CEALUMODE(1'b0),
        .CEB1(1'b0),
        .CEB2(CEB2),
        .CEC(1'b0),
        .CECARRYIN(1'b0),
        .CECTRL(1'b0),
        .CED(1'b0),
        .CEINMODE(1'b0),
        .CEM(1'b0),
        .CEP(ap_done),
        .CLK(ap_clk),
        .D({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .INMODE({1'b0,1'b0,1'b0,1'b0,1'b0}),
        .MULTSIGNIN(1'b0),
        .MULTSIGNOUT(NLW_int_p_reg_MULTSIGNOUT_UNCONNECTED),
        .OPMODE({1'b0,1'b0,1'b1,1'b0,1'b1,1'b0,1'b1,1'b0,1'b1}),
        .OVERFLOW(NLW_int_p_reg_OVERFLOW_UNCONNECTED),
        .P({int_p_reg_n_58,int_p_reg_n_59,int_p_reg_n_60,int_p_reg_n_61,int_p_reg_n_62,int_p_reg_n_63,int_p_reg_n_64,int_p_reg_n_65,int_p_reg_n_66,int_p_reg_n_67,int_p_reg_n_68,int_p_reg_n_69,int_p_reg_n_70,int_p_reg_n_71,int_p_reg_n_72,int_p_reg_n_73,int_p_reg_n_74,int_p_reg_n_75,int_p_reg_n_76,int_p_reg_n_77,int_p_reg_n_78,int_p_reg_n_79,int_p_reg_n_80,int_p_reg_n_81,int_p_reg_n_82,int_p_reg_n_83,int_p_reg_n_84,int_p_reg_n_85,int_p_reg_n_86,int_p_reg_n_87,int_p_reg_n_88,int_p_reg_n_89,int_p_reg_n_90,int_p_reg_n_91,int_p_reg_n_92,int_p_reg_n_93,int_p_reg_n_94,int_p_reg_n_95,int_p_reg_n_96,int_p_reg_n_97,int_p_reg_n_98,int_p_reg_n_99,int_p_reg_n_100,int_p_reg_n_101,int_p_reg_n_102,int_p_reg_n_103,int_p_reg_n_104,int_p_reg_n_105}),
        .PATTERNBDETECT(NLW_int_p_reg_PATTERNBDETECT_UNCONNECTED),
        .PATTERNDETECT(NLW_int_p_reg_PATTERNDETECT_UNCONNECTED),
        .PCIN(PCOUT),
        .PCOUT(NLW_int_p_reg_PCOUT_UNCONNECTED[47:0]),
        .RSTA(RSTB),
        .RSTALLCARRYIN(1'b0),
        .RSTALUMODE(1'b0),
        .RSTB(RSTB),
        .RSTC(1'b0),
        .RSTCTRL(1'b0),
        .RSTD(1'b0),
        .RSTINMODE(1'b0),
        .RSTM(1'b0),
        .RSTP(RSTB),
        .UNDERFLOW(NLW_int_p_reg_UNDERFLOW_UNCONNECTED),
        .XOROUT(NLW_int_p_reg_XOROUT_UNCONNECTED[7:0]));
  FDRE \int_p_reg[0] 
       (.C(ap_clk),
        .CE(ap_done),
        .D(\int_p_reg[16]_0 [0]),
        .Q(\int_p_reg_n_0_[0] ),
        .R(RSTB));
  FDRE \int_p_reg[0]__0 
       (.C(ap_clk),
        .CE(ap_done),
        .D(\int_p_reg[16]__0_0 [0]),
        .Q(\int_p_reg[0]__0_n_0 ),
        .R(RSTB));
  FDRE \int_p_reg[10] 
       (.C(ap_clk),
        .CE(ap_done),
        .D(\int_p_reg[16]_0 [10]),
        .Q(\int_p_reg_n_0_[10] ),
        .R(RSTB));
  FDRE \int_p_reg[10]__0 
       (.C(ap_clk),
        .CE(ap_done),
        .D(\int_p_reg[16]__0_0 [10]),
        .Q(\int_p_reg[10]__0_n_0 ),
        .R(RSTB));
  FDRE \int_p_reg[11] 
       (.C(ap_clk),
        .CE(ap_done),
        .D(\int_p_reg[16]_0 [11]),
        .Q(\int_p_reg_n_0_[11] ),
        .R(RSTB));
  FDRE \int_p_reg[11]__0 
       (.C(ap_clk),
        .CE(ap_done),
        .D(\int_p_reg[16]__0_0 [11]),
        .Q(\int_p_reg[11]__0_n_0 ),
        .R(RSTB));
  FDRE \int_p_reg[12] 
       (.C(ap_clk),
        .CE(ap_done),
        .D(\int_p_reg[16]_0 [12]),
        .Q(\int_p_reg_n_0_[12] ),
        .R(RSTB));
  FDRE \int_p_reg[12]__0 
       (.C(ap_clk),
        .CE(ap_done),
        .D(\int_p_reg[16]__0_0 [12]),
        .Q(\int_p_reg[12]__0_n_0 ),
        .R(RSTB));
  FDRE \int_p_reg[13] 
       (.C(ap_clk),
        .CE(ap_done),
        .D(\int_p_reg[16]_0 [13]),
        .Q(\int_p_reg_n_0_[13] ),
        .R(RSTB));
  FDRE \int_p_reg[13]__0 
       (.C(ap_clk),
        .CE(ap_done),
        .D(\int_p_reg[16]__0_0 [13]),
        .Q(\int_p_reg[13]__0_n_0 ),
        .R(RSTB));
  FDRE \int_p_reg[14] 
       (.C(ap_clk),
        .CE(ap_done),
        .D(\int_p_reg[16]_0 [14]),
        .Q(\int_p_reg_n_0_[14] ),
        .R(RSTB));
  FDRE \int_p_reg[14]__0 
       (.C(ap_clk),
        .CE(ap_done),
        .D(\int_p_reg[16]__0_0 [14]),
        .Q(\int_p_reg[14]__0_n_0 ),
        .R(RSTB));
  FDRE \int_p_reg[15] 
       (.C(ap_clk),
        .CE(ap_done),
        .D(\int_p_reg[16]_0 [15]),
        .Q(\int_p_reg_n_0_[15] ),
        .R(RSTB));
  FDRE \int_p_reg[15]__0 
       (.C(ap_clk),
        .CE(ap_done),
        .D(\int_p_reg[16]__0_0 [15]),
        .Q(\int_p_reg[15]__0_n_0 ),
        .R(RSTB));
  FDRE \int_p_reg[16] 
       (.C(ap_clk),
        .CE(ap_done),
        .D(\int_p_reg[16]_0 [16]),
        .Q(\int_p_reg_n_0_[16] ),
        .R(RSTB));
  FDRE \int_p_reg[16]__0 
       (.C(ap_clk),
        .CE(ap_done),
        .D(\int_p_reg[16]__0_0 [16]),
        .Q(\int_p_reg[6]_0 [0]),
        .R(RSTB));
  FDRE \int_p_reg[1] 
       (.C(ap_clk),
        .CE(ap_done),
        .D(\int_p_reg[16]_0 [1]),
        .Q(\int_p_reg_n_0_[1] ),
        .R(RSTB));
  FDRE \int_p_reg[1]__0 
       (.C(ap_clk),
        .CE(ap_done),
        .D(\int_p_reg[16]__0_0 [1]),
        .Q(\int_p_reg[1]__0_n_0 ),
        .R(RSTB));
  FDRE \int_p_reg[2] 
       (.C(ap_clk),
        .CE(ap_done),
        .D(\int_p_reg[16]_0 [2]),
        .Q(\int_p_reg_n_0_[2] ),
        .R(RSTB));
  FDRE \int_p_reg[2]__0 
       (.C(ap_clk),
        .CE(ap_done),
        .D(\int_p_reg[16]__0_0 [2]),
        .Q(\int_p_reg[2]__0_n_0 ),
        .R(RSTB));
  FDRE \int_p_reg[3] 
       (.C(ap_clk),
        .CE(ap_done),
        .D(\int_p_reg[16]_0 [3]),
        .Q(\int_p_reg_n_0_[3] ),
        .R(RSTB));
  FDRE \int_p_reg[3]__0 
       (.C(ap_clk),
        .CE(ap_done),
        .D(\int_p_reg[16]__0_0 [3]),
        .Q(\int_p_reg[3]__0_n_0 ),
        .R(RSTB));
  FDRE \int_p_reg[4] 
       (.C(ap_clk),
        .CE(ap_done),
        .D(\int_p_reg[16]_0 [4]),
        .Q(\int_p_reg_n_0_[4] ),
        .R(RSTB));
  FDRE \int_p_reg[4]__0 
       (.C(ap_clk),
        .CE(ap_done),
        .D(\int_p_reg[16]__0_0 [4]),
        .Q(\int_p_reg[4]__0_n_0 ),
        .R(RSTB));
  FDRE \int_p_reg[5] 
       (.C(ap_clk),
        .CE(ap_done),
        .D(\int_p_reg[16]_0 [5]),
        .Q(\int_p_reg_n_0_[5] ),
        .R(RSTB));
  FDRE \int_p_reg[5]__0 
       (.C(ap_clk),
        .CE(ap_done),
        .D(\int_p_reg[16]__0_0 [5]),
        .Q(\int_p_reg[5]__0_n_0 ),
        .R(RSTB));
  FDRE \int_p_reg[6] 
       (.C(ap_clk),
        .CE(ap_done),
        .D(\int_p_reg[16]_0 [6]),
        .Q(\int_p_reg_n_0_[6] ),
        .R(RSTB));
  FDRE \int_p_reg[6]__0 
       (.C(ap_clk),
        .CE(ap_done),
        .D(\int_p_reg[16]__0_0 [6]),
        .Q(\int_p_reg[6]__0_n_0 ),
        .R(RSTB));
  FDRE \int_p_reg[7] 
       (.C(ap_clk),
        .CE(ap_done),
        .D(\int_p_reg[16]_0 [7]),
        .Q(\int_p_reg_n_0_[7] ),
        .R(RSTB));
  FDRE \int_p_reg[7]__0 
       (.C(ap_clk),
        .CE(ap_done),
        .D(\int_p_reg[16]__0_0 [7]),
        .Q(\int_p_reg[7]__0_n_0 ),
        .R(RSTB));
  FDRE \int_p_reg[8] 
       (.C(ap_clk),
        .CE(ap_done),
        .D(\int_p_reg[16]_0 [8]),
        .Q(\int_p_reg_n_0_[8] ),
        .R(RSTB));
  FDRE \int_p_reg[8]__0 
       (.C(ap_clk),
        .CE(ap_done),
        .D(\int_p_reg[16]__0_0 [8]),
        .Q(\int_p_reg[8]__0_n_0 ),
        .R(RSTB));
  FDRE \int_p_reg[9] 
       (.C(ap_clk),
        .CE(ap_done),
        .D(\int_p_reg[16]_0 [9]),
        .Q(\int_p_reg_n_0_[9] ),
        .R(RSTB));
  FDRE \int_p_reg[9]__0 
       (.C(ap_clk),
        .CE(ap_done),
        .D(\int_p_reg[16]__0_0 [9]),
        .Q(\int_p_reg[9]__0_n_0 ),
        .R(RSTB));
  (* KEEP_HIERARCHY = "yes" *) 
  (* METHODOLOGY_DRC_VIOS = "{SYNTH-10 {cell *THIS*} {string 18x16 4}}" *) 
  DSP48E2 #(
    .ACASCREG(1),
    .ADREG(1),
    .ALUMODEREG(0),
    .AMULTSEL("A"),
    .AREG(1),
    .AUTORESET_PATDET("NO_RESET"),
    .AUTORESET_PRIORITY("RESET"),
    .A_INPUT("DIRECT"),
    .BCASCREG(1),
    .BMULTSEL("B"),
    .BREG(1),
    .B_INPUT("DIRECT"),
    .CARRYINREG(0),
    .CARRYINSELREG(0),
    .CREG(1),
    .DREG(1),
    .INMODEREG(0),
    .MASK(48'h3FFFFFFFFFFF),
    .MREG(0),
    .OPMODEREG(0),
    .PATTERN(48'h000000000000),
    .PREADDINSEL("A"),
    .PREG(1),
    .RND(48'h000000000000),
    .SEL_MASK("MASK"),
    .SEL_PATTERN("PATTERN"),
    .USE_MULT("MULTIPLY"),
    .USE_PATTERN_DETECT("NO_PATDET"),
    .USE_SIMD("ONE48"),
    .USE_WIDEXOR("FALSE"),
    .XORSIMD("XOR24_48_96")) 
    int_p_reg__0
       (.A({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,\s_axi_CTRL_WDATA[31] [16:0]}),
        .ACIN({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .ACOUT(NLW_int_p_reg__0_ACOUT_UNCONNECTED[29:0]),
        .ALUMODE({1'b0,1'b0,1'b0,1'b0}),
        .B({1'b0,1'b0,1'b0,int_a0}),
        .BCIN({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .BCOUT(NLW_int_p_reg__0_BCOUT_UNCONNECTED[17:0]),
        .C({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .CARRYCASCIN(1'b0),
        .CARRYCASCOUT(NLW_int_p_reg__0_CARRYCASCOUT_UNCONNECTED),
        .CARRYIN(1'b0),
        .CARRYINSEL({1'b0,1'b0,1'b0}),
        .CARRYOUT(NLW_int_p_reg__0_CARRYOUT_UNCONNECTED[3:0]),
        .CEA1(1'b0),
        .CEA2(E),
        .CEAD(1'b0),
        .CEALUMODE(1'b0),
        .CEB1(1'b0),
        .CEB2(CEB2),
        .CEC(1'b0),
        .CECARRYIN(1'b0),
        .CECTRL(1'b0),
        .CED(1'b0),
        .CEINMODE(1'b0),
        .CEM(1'b0),
        .CEP(ap_done),
        .CLK(ap_clk),
        .D({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .INMODE({1'b0,1'b0,1'b0,1'b0,1'b0}),
        .MULTSIGNIN(1'b0),
        .MULTSIGNOUT(NLW_int_p_reg__0_MULTSIGNOUT_UNCONNECTED),
        .OPMODE({1'b0,1'b0,1'b1,1'b0,1'b1,1'b0,1'b1,1'b0,1'b1}),
        .OVERFLOW(NLW_int_p_reg__0_OVERFLOW_UNCONNECTED),
        .P({int_p_reg__0_n_58,int_p_reg__0_n_59,P}),
        .PATTERNBDETECT(NLW_int_p_reg__0_PATTERNBDETECT_UNCONNECTED),
        .PATTERNDETECT(NLW_int_p_reg__0_PATTERNDETECT_UNCONNECTED),
        .PCIN(DSP_OUTPUT_INST),
        .PCOUT(NLW_int_p_reg__0_PCOUT_UNCONNECTED[47:0]),
        .RSTA(RSTB),
        .RSTALLCARRYIN(1'b0),
        .RSTALUMODE(1'b0),
        .RSTB(RSTB),
        .RSTC(1'b0),
        .RSTCTRL(1'b0),
        .RSTD(1'b0),
        .RSTINMODE(1'b0),
        .RSTM(1'b0),
        .RSTP(RSTB),
        .UNDERFLOW(NLW_int_p_reg__0_UNDERFLOW_UNCONNECTED),
        .XOROUT(NLW_int_p_reg__0_XOROUT_UNCONNECTED[7:0]));
  LUT5 #(
    .INIT(32'h72FF7272)) 
    int_task_ap_done_i_1
       (.I0(auto_restart_status_reg_n_0),
        .I1(p_4_in[2]),
        .I2(ap_done),
        .I3(int_task_ap_done0__4),
        .I4(int_task_ap_done),
        .O(int_task_ap_done_i_1_n_0));
  LUT6 #(
    .INIT(64'h0000000000000200)) 
    int_task_ap_done_i_2
       (.I0(ar_hs),
        .I1(s_axi_CTRL_ARADDR[2]),
        .I2(s_axi_CTRL_ARADDR[3]),
        .I3(int_task_ap_done_i_3_n_0),
        .I4(s_axi_CTRL_ARADDR[4]),
        .I5(s_axi_CTRL_ARADDR[5]),
        .O(int_task_ap_done0__4));
  LUT2 #(
    .INIT(4'h1)) 
    int_task_ap_done_i_3
       (.I0(s_axi_CTRL_ARADDR[1]),
        .I1(s_axi_CTRL_ARADDR[0]),
        .O(int_task_ap_done_i_3_n_0));
  FDRE #(
    .INIT(1'b0)) 
    int_task_ap_done_reg
       (.C(ap_clk),
        .CE(1'b1),
        .D(int_task_ap_done_i_1_n_0),
        .Q(int_task_ap_done),
        .R(RSTB));
  LUT6 #(
    .INIT(64'h0D00FFFF0D000000)) 
    \rdata[0]_i_1 
       (.I0(s_axi_CTRL_ARADDR[5]),
        .I1(data7[0]),
        .I2(s_axi_CTRL_ARADDR[4]),
        .I3(\rdata[0]_i_2_n_0 ),
        .I4(s_axi_CTRL_ARADDR[2]),
        .I5(\rdata_reg[0]_i_3_n_0 ),
        .O(\rdata[0]_i_1_n_0 ));
  (* SOFT_HLUTNM = "soft_lutpair2" *) 
  LUT4 #(
    .INIT(16'h3B38)) 
    \rdata[0]_i_2 
       (.I0(int_interrupt1),
        .I1(s_axi_CTRL_ARADDR[3]),
        .I2(s_axi_CTRL_ARADDR[5]),
        .I3(int_gie_reg_n_0),
        .O(\rdata[0]_i_2_n_0 ));
  LUT5 #(
    .INIT(32'h30BB3088)) 
    \rdata[0]_i_4 
       (.I0(vp_fu_61_p10[0]),
        .I1(s_axi_CTRL_ARADDR[4]),
        .I2(\int_p_reg[0]__0_n_0 ),
        .I3(s_axi_CTRL_ARADDR[5]),
        .I4(ap_done),
        .O(\rdata[0]_i_4_n_0 ));
  LUT5 #(
    .INIT(32'h30BB3088)) 
    \rdata[0]_i_5 
       (.I0(vp_fu_61_p00[0]),
        .I1(s_axi_CTRL_ARADDR[4]),
        .I2(int_p_ap_vld),
        .I3(s_axi_CTRL_ARADDR[5]),
        .I4(int_ier),
        .O(\rdata[0]_i_5_n_0 ));
  LUT6 #(
    .INIT(64'h0033B8000000B800)) 
    \rdata[10]_i_2 
       (.I0(vp_fu_61_p00[10]),
        .I1(s_axi_CTRL_ARADDR[3]),
        .I2(vp_fu_61_p10[10]),
        .I3(s_axi_CTRL_ARADDR[4]),
        .I4(s_axi_CTRL_ARADDR[5]),
        .I5(\int_p_reg[10]__0_n_0 ),
        .O(\int_b_reg[10]_0 ));
  LUT6 #(
    .INIT(64'h0033B8000000B800)) 
    \rdata[11]_i_2 
       (.I0(vp_fu_61_p00[11]),
        .I1(s_axi_CTRL_ARADDR[3]),
        .I2(vp_fu_61_p10[11]),
        .I3(s_axi_CTRL_ARADDR[4]),
        .I4(s_axi_CTRL_ARADDR[5]),
        .I5(\int_p_reg[11]__0_n_0 ),
        .O(\int_b_reg[11]_0 ));
  LUT6 #(
    .INIT(64'h0033B8000000B800)) 
    \rdata[12]_i_2 
       (.I0(vp_fu_61_p00[12]),
        .I1(s_axi_CTRL_ARADDR[3]),
        .I2(vp_fu_61_p10[12]),
        .I3(s_axi_CTRL_ARADDR[4]),
        .I4(s_axi_CTRL_ARADDR[5]),
        .I5(\int_p_reg[12]__0_n_0 ),
        .O(\int_b_reg[12]_0 ));
  LUT6 #(
    .INIT(64'h0033B8000000B800)) 
    \rdata[13]_i_2 
       (.I0(vp_fu_61_p00[13]),
        .I1(s_axi_CTRL_ARADDR[3]),
        .I2(vp_fu_61_p10[13]),
        .I3(s_axi_CTRL_ARADDR[4]),
        .I4(s_axi_CTRL_ARADDR[5]),
        .I5(\int_p_reg[13]__0_n_0 ),
        .O(\int_b_reg[13]_0 ));
  LUT6 #(
    .INIT(64'h0033B8000000B800)) 
    \rdata[14]_i_2 
       (.I0(vp_fu_61_p00[14]),
        .I1(s_axi_CTRL_ARADDR[3]),
        .I2(vp_fu_61_p10[14]),
        .I3(s_axi_CTRL_ARADDR[4]),
        .I4(s_axi_CTRL_ARADDR[5]),
        .I5(\int_p_reg[14]__0_n_0 ),
        .O(\int_b_reg[14]_0 ));
  LUT6 #(
    .INIT(64'h0033B8000000B800)) 
    \rdata[15]_i_2 
       (.I0(vp_fu_61_p00[15]),
        .I1(s_axi_CTRL_ARADDR[3]),
        .I2(vp_fu_61_p10[15]),
        .I3(s_axi_CTRL_ARADDR[4]),
        .I4(s_axi_CTRL_ARADDR[5]),
        .I5(\int_p_reg[15]__0_n_0 ),
        .O(\int_b_reg[15]_0 ));
  LUT6 #(
    .INIT(64'h0033B8000000B800)) 
    \rdata[16]_i_2 
       (.I0(vp_fu_61_p00[16]),
        .I1(s_axi_CTRL_ARADDR[3]),
        .I2(vp_fu_61_p10[16]),
        .I3(s_axi_CTRL_ARADDR[4]),
        .I4(s_axi_CTRL_ARADDR[5]),
        .I5(O[0]),
        .O(\rdata[16]_i_2_n_0 ));
  LUT6 #(
    .INIT(64'h0033B8000000B800)) 
    \rdata[17]_i_2 
       (.I0(vp_fu_61_p00[17]),
        .I1(s_axi_CTRL_ARADDR[3]),
        .I2(vp_fu_61_p10[17]),
        .I3(s_axi_CTRL_ARADDR[4]),
        .I4(s_axi_CTRL_ARADDR[5]),
        .I5(O[1]),
        .O(\rdata[17]_i_2_n_0 ));
  LUT6 #(
    .INIT(64'h0033B8000000B800)) 
    \rdata[18]_i_2 
       (.I0(vp_fu_61_p00[18]),
        .I1(s_axi_CTRL_ARADDR[3]),
        .I2(vp_fu_61_p10[18]),
        .I3(s_axi_CTRL_ARADDR[4]),
        .I4(s_axi_CTRL_ARADDR[5]),
        .I5(O[2]),
        .O(\rdata[18]_i_2_n_0 ));
  LUT6 #(
    .INIT(64'h0033B8000000B800)) 
    \rdata[19]_i_2 
       (.I0(vp_fu_61_p00[19]),
        .I1(s_axi_CTRL_ARADDR[3]),
        .I2(vp_fu_61_p10[19]),
        .I3(s_axi_CTRL_ARADDR[4]),
        .I4(s_axi_CTRL_ARADDR[5]),
        .I5(O[3]),
        .O(\rdata[19]_i_2_n_0 ));
  LUT5 #(
    .INIT(32'h40FF4000)) 
    \rdata[1]_i_2 
       (.I0(s_axi_CTRL_ARADDR[5]),
        .I1(s_axi_CTRL_ARADDR[4]),
        .I2(vp_fu_61_p00[1]),
        .I3(s_axi_CTRL_ARADDR[3]),
        .I4(\rdata[1]_i_3_n_0 ),
        .O(s_axi_CTRL_ARADDR_5_sn_1));
  LUT5 #(
    .INIT(32'h30BB3088)) 
    \rdata[1]_i_3 
       (.I0(vp_fu_61_p10[1]),
        .I1(s_axi_CTRL_ARADDR[4]),
        .I2(\int_p_reg[1]__0_n_0 ),
        .I3(s_axi_CTRL_ARADDR[5]),
        .I4(int_task_ap_done),
        .O(\rdata[1]_i_3_n_0 ));
  LUT6 #(
    .INIT(64'h0033B8000000B800)) 
    \rdata[20]_i_2 
       (.I0(vp_fu_61_p00[20]),
        .I1(s_axi_CTRL_ARADDR[3]),
        .I2(vp_fu_61_p10[20]),
        .I3(s_axi_CTRL_ARADDR[4]),
        .I4(s_axi_CTRL_ARADDR[5]),
        .I5(O[4]),
        .O(\rdata[20]_i_2_n_0 ));
  LUT6 #(
    .INIT(64'h0033B8000000B800)) 
    \rdata[21]_i_2 
       (.I0(vp_fu_61_p00[21]),
        .I1(s_axi_CTRL_ARADDR[3]),
        .I2(vp_fu_61_p10[21]),
        .I3(s_axi_CTRL_ARADDR[4]),
        .I4(s_axi_CTRL_ARADDR[5]),
        .I5(O[5]),
        .O(\rdata[21]_i_2_n_0 ));
  LUT6 #(
    .INIT(64'h0033B8000000B800)) 
    \rdata[22]_i_2 
       (.I0(vp_fu_61_p00[22]),
        .I1(s_axi_CTRL_ARADDR[3]),
        .I2(vp_fu_61_p10[22]),
        .I3(s_axi_CTRL_ARADDR[4]),
        .I4(s_axi_CTRL_ARADDR[5]),
        .I5(O[6]),
        .O(\rdata[22]_i_2_n_0 ));
  LUT6 #(
    .INIT(64'h0033B8000000B800)) 
    \rdata[23]_i_2 
       (.I0(vp_fu_61_p00[23]),
        .I1(s_axi_CTRL_ARADDR[3]),
        .I2(vp_fu_61_p10[23]),
        .I3(s_axi_CTRL_ARADDR[4]),
        .I4(s_axi_CTRL_ARADDR[5]),
        .I5(O[7]),
        .O(\rdata[23]_i_2_n_0 ));
  LUT6 #(
    .INIT(64'h0033B8000000B800)) 
    \rdata[24]_i_2 
       (.I0(vp_fu_61_p00[24]),
        .I1(s_axi_CTRL_ARADDR[3]),
        .I2(vp_fu_61_p10[24]),
        .I3(s_axi_CTRL_ARADDR[4]),
        .I4(s_axi_CTRL_ARADDR[5]),
        .I5(\rdata_reg[31]_0 [0]),
        .O(\rdata[24]_i_2_n_0 ));
  LUT4 #(
    .INIT(16'h1000)) 
    \rdata[24]_i_3 
       (.I0(s_axi_CTRL_ARADDR[3]),
        .I1(s_axi_CTRL_ARADDR[4]),
        .I2(data7[1]),
        .I3(s_axi_CTRL_ARADDR[5]),
        .O(\rdata[24]_i_3_n_0 ));
  LUT6 #(
    .INIT(64'h0033B8000000B800)) 
    \rdata[25]_i_2 
       (.I0(vp_fu_61_p00[25]),
        .I1(s_axi_CTRL_ARADDR[3]),
        .I2(vp_fu_61_p10[25]),
        .I3(s_axi_CTRL_ARADDR[4]),
        .I4(s_axi_CTRL_ARADDR[5]),
        .I5(\rdata_reg[31]_0 [1]),
        .O(\rdata[25]_i_2_n_0 ));
  LUT4 #(
    .INIT(16'h1000)) 
    \rdata[25]_i_3 
       (.I0(s_axi_CTRL_ARADDR[3]),
        .I1(s_axi_CTRL_ARADDR[4]),
        .I2(data7[2]),
        .I3(s_axi_CTRL_ARADDR[5]),
        .O(\rdata[25]_i_3_n_0 ));
  LUT6 #(
    .INIT(64'h0033B8000000B800)) 
    \rdata[26]_i_2 
       (.I0(vp_fu_61_p00[26]),
        .I1(s_axi_CTRL_ARADDR[3]),
        .I2(vp_fu_61_p10[26]),
        .I3(s_axi_CTRL_ARADDR[4]),
        .I4(s_axi_CTRL_ARADDR[5]),
        .I5(\rdata_reg[31]_0 [2]),
        .O(\rdata[26]_i_2_n_0 ));
  LUT4 #(
    .INIT(16'h1000)) 
    \rdata[26]_i_3 
       (.I0(s_axi_CTRL_ARADDR[3]),
        .I1(s_axi_CTRL_ARADDR[4]),
        .I2(data7[3]),
        .I3(s_axi_CTRL_ARADDR[5]),
        .O(\rdata[26]_i_3_n_0 ));
  LUT6 #(
    .INIT(64'h0033B8000000B800)) 
    \rdata[27]_i_2 
       (.I0(vp_fu_61_p00[27]),
        .I1(s_axi_CTRL_ARADDR[3]),
        .I2(vp_fu_61_p10[27]),
        .I3(s_axi_CTRL_ARADDR[4]),
        .I4(s_axi_CTRL_ARADDR[5]),
        .I5(\rdata_reg[31]_0 [3]),
        .O(\rdata[27]_i_2_n_0 ));
  LUT4 #(
    .INIT(16'h1000)) 
    \rdata[27]_i_3 
       (.I0(s_axi_CTRL_ARADDR[3]),
        .I1(s_axi_CTRL_ARADDR[4]),
        .I2(data7[4]),
        .I3(s_axi_CTRL_ARADDR[5]),
        .O(\rdata[27]_i_3_n_0 ));
  LUT6 #(
    .INIT(64'h0033B8000000B800)) 
    \rdata[28]_i_2 
       (.I0(vp_fu_61_p00[28]),
        .I1(s_axi_CTRL_ARADDR[3]),
        .I2(vp_fu_61_p10[28]),
        .I3(s_axi_CTRL_ARADDR[4]),
        .I4(s_axi_CTRL_ARADDR[5]),
        .I5(\rdata_reg[31]_0 [4]),
        .O(\rdata[28]_i_2_n_0 ));
  LUT4 #(
    .INIT(16'h1000)) 
    \rdata[28]_i_3 
       (.I0(s_axi_CTRL_ARADDR[3]),
        .I1(s_axi_CTRL_ARADDR[4]),
        .I2(data7[5]),
        .I3(s_axi_CTRL_ARADDR[5]),
        .O(\rdata[28]_i_3_n_0 ));
  LUT6 #(
    .INIT(64'h0033B8000000B800)) 
    \rdata[29]_i_2 
       (.I0(vp_fu_61_p00[29]),
        .I1(s_axi_CTRL_ARADDR[3]),
        .I2(vp_fu_61_p10[29]),
        .I3(s_axi_CTRL_ARADDR[4]),
        .I4(s_axi_CTRL_ARADDR[5]),
        .I5(\rdata_reg[31]_0 [5]),
        .O(\rdata[29]_i_2_n_0 ));
  LUT4 #(
    .INIT(16'h1000)) 
    \rdata[29]_i_3 
       (.I0(s_axi_CTRL_ARADDR[3]),
        .I1(s_axi_CTRL_ARADDR[4]),
        .I2(data7[6]),
        .I3(s_axi_CTRL_ARADDR[5]),
        .O(\rdata[29]_i_3_n_0 ));
  LUT5 #(
    .INIT(32'h40FF4000)) 
    \rdata[2]_i_2 
       (.I0(s_axi_CTRL_ARADDR[5]),
        .I1(s_axi_CTRL_ARADDR[4]),
        .I2(vp_fu_61_p00[2]),
        .I3(s_axi_CTRL_ARADDR[3]),
        .I4(\rdata[2]_i_3_n_0 ),
        .O(\s_axi_CTRL_ARADDR[5]_0 ));
  LUT5 #(
    .INIT(32'h30BB3088)) 
    \rdata[2]_i_3 
       (.I0(vp_fu_61_p10[2]),
        .I1(s_axi_CTRL_ARADDR[4]),
        .I2(\int_p_reg[2]__0_n_0 ),
        .I3(s_axi_CTRL_ARADDR[5]),
        .I4(p_4_in[2]),
        .O(\rdata[2]_i_3_n_0 ));
  LUT6 #(
    .INIT(64'h0033B8000000B800)) 
    \rdata[30]_i_2 
       (.I0(vp_fu_61_p00[30]),
        .I1(s_axi_CTRL_ARADDR[3]),
        .I2(vp_fu_61_p10[30]),
        .I3(s_axi_CTRL_ARADDR[4]),
        .I4(s_axi_CTRL_ARADDR[5]),
        .I5(\rdata_reg[31]_0 [6]),
        .O(\rdata[30]_i_2_n_0 ));
  LUT4 #(
    .INIT(16'h1000)) 
    \rdata[30]_i_3 
       (.I0(s_axi_CTRL_ARADDR[3]),
        .I1(s_axi_CTRL_ARADDR[4]),
        .I2(data7[7]),
        .I3(s_axi_CTRL_ARADDR[5]),
        .O(\rdata[30]_i_3_n_0 ));
  LUT4 #(
    .INIT(16'h8880)) 
    \rdata[31]_i_1 
       (.I0(s_axi_CTRL_ARVALID),
        .I1(\FSM_onehot_rstate_reg[1]_0 ),
        .I2(s_axi_CTRL_ARADDR[0]),
        .I3(s_axi_CTRL_ARADDR[1]),
        .O(\rdata[31]_i_1_n_0 ));
  LUT2 #(
    .INIT(4'h8)) 
    \rdata[31]_i_2 
       (.I0(\FSM_onehot_rstate_reg[1]_0 ),
        .I1(s_axi_CTRL_ARVALID),
        .O(ar_hs));
  LUT6 #(
    .INIT(64'h0033B8000000B800)) 
    \rdata[31]_i_4 
       (.I0(vp_fu_61_p00[31]),
        .I1(s_axi_CTRL_ARADDR[3]),
        .I2(vp_fu_61_p10[31]),
        .I3(s_axi_CTRL_ARADDR[4]),
        .I4(s_axi_CTRL_ARADDR[5]),
        .I5(\rdata_reg[31]_0 [7]),
        .O(\rdata[31]_i_4_n_0 ));
  LUT4 #(
    .INIT(16'h1000)) 
    \rdata[31]_i_5 
       (.I0(s_axi_CTRL_ARADDR[3]),
        .I1(s_axi_CTRL_ARADDR[4]),
        .I2(data7[8]),
        .I3(s_axi_CTRL_ARADDR[5]),
        .O(\rdata[31]_i_5_n_0 ));
  LUT5 #(
    .INIT(32'h40FF4000)) 
    \rdata[3]_i_2 
       (.I0(s_axi_CTRL_ARADDR[5]),
        .I1(s_axi_CTRL_ARADDR[4]),
        .I2(vp_fu_61_p00[3]),
        .I3(s_axi_CTRL_ARADDR[3]),
        .I4(\rdata[3]_i_3_n_0 ),
        .O(\s_axi_CTRL_ARADDR[5]_1 ));
  LUT5 #(
    .INIT(32'h30BB3088)) 
    \rdata[3]_i_3 
       (.I0(vp_fu_61_p10[3]),
        .I1(s_axi_CTRL_ARADDR[4]),
        .I2(\int_p_reg[3]__0_n_0 ),
        .I3(s_axi_CTRL_ARADDR[5]),
        .I4(int_ap_ready),
        .O(\rdata[3]_i_3_n_0 ));
  LUT6 #(
    .INIT(64'h0033B8000000B800)) 
    \rdata[4]_i_2 
       (.I0(vp_fu_61_p00[4]),
        .I1(s_axi_CTRL_ARADDR[3]),
        .I2(vp_fu_61_p10[4]),
        .I3(s_axi_CTRL_ARADDR[4]),
        .I4(s_axi_CTRL_ARADDR[5]),
        .I5(\int_p_reg[4]__0_n_0 ),
        .O(\int_b_reg[4]_0 ));
  LUT6 #(
    .INIT(64'h0033B8000000B800)) 
    \rdata[5]_i_2 
       (.I0(vp_fu_61_p00[5]),
        .I1(s_axi_CTRL_ARADDR[3]),
        .I2(vp_fu_61_p10[5]),
        .I3(s_axi_CTRL_ARADDR[4]),
        .I4(s_axi_CTRL_ARADDR[5]),
        .I5(\int_p_reg[5]__0_n_0 ),
        .O(\int_b_reg[5]_0 ));
  LUT6 #(
    .INIT(64'h0033B8000000B800)) 
    \rdata[6]_i_2 
       (.I0(vp_fu_61_p00[6]),
        .I1(s_axi_CTRL_ARADDR[3]),
        .I2(vp_fu_61_p10[6]),
        .I3(s_axi_CTRL_ARADDR[4]),
        .I4(s_axi_CTRL_ARADDR[5]),
        .I5(\int_p_reg[6]__0_n_0 ),
        .O(\int_b_reg[6]_0 ));
  LUT5 #(
    .INIT(32'h40FF4000)) 
    \rdata[7]_i_2 
       (.I0(s_axi_CTRL_ARADDR[5]),
        .I1(s_axi_CTRL_ARADDR[4]),
        .I2(vp_fu_61_p00[7]),
        .I3(s_axi_CTRL_ARADDR[3]),
        .I4(\rdata[7]_i_3_n_0 ),
        .O(\s_axi_CTRL_ARADDR[5]_2 ));
  LUT5 #(
    .INIT(32'h30BB3088)) 
    \rdata[7]_i_3 
       (.I0(vp_fu_61_p10[7]),
        .I1(s_axi_CTRL_ARADDR[4]),
        .I2(\int_p_reg[7]__0_n_0 ),
        .I3(s_axi_CTRL_ARADDR[5]),
        .I4(p_4_in[7]),
        .O(\rdata[7]_i_3_n_0 ));
  LUT6 #(
    .INIT(64'h0033B8000000B800)) 
    \rdata[8]_i_2 
       (.I0(vp_fu_61_p00[8]),
        .I1(s_axi_CTRL_ARADDR[3]),
        .I2(vp_fu_61_p10[8]),
        .I3(s_axi_CTRL_ARADDR[4]),
        .I4(s_axi_CTRL_ARADDR[5]),
        .I5(\int_p_reg[8]__0_n_0 ),
        .O(\int_b_reg[8]_0 ));
  LUT5 #(
    .INIT(32'h40FF4000)) 
    \rdata[9]_i_2 
       (.I0(s_axi_CTRL_ARADDR[5]),
        .I1(s_axi_CTRL_ARADDR[4]),
        .I2(vp_fu_61_p00[9]),
        .I3(s_axi_CTRL_ARADDR[3]),
        .I4(\rdata[9]_i_3_n_0 ),
        .O(\s_axi_CTRL_ARADDR[5]_3 ));
  LUT5 #(
    .INIT(32'h30BB3088)) 
    \rdata[9]_i_3 
       (.I0(vp_fu_61_p10[9]),
        .I1(s_axi_CTRL_ARADDR[4]),
        .I2(\int_p_reg[9]__0_n_0 ),
        .I3(s_axi_CTRL_ARADDR[5]),
        .I4(interrupt),
        .O(\rdata[9]_i_3_n_0 ));
  FDRE \rdata_reg[0] 
       (.C(ap_clk),
        .CE(ar_hs),
        .D(\rdata[0]_i_1_n_0 ),
        .Q(s_axi_CTRL_RDATA[0]),
        .R(\rdata[31]_i_1_n_0 ));
  MUXF7 \rdata_reg[0]_i_3 
       (.I0(\rdata[0]_i_4_n_0 ),
        .I1(\rdata[0]_i_5_n_0 ),
        .O(\rdata_reg[0]_i_3_n_0 ),
        .S(s_axi_CTRL_ARADDR[3]));
  FDRE \rdata_reg[10] 
       (.C(ap_clk),
        .CE(ar_hs),
        .D(\rdata_reg[10]_0 ),
        .Q(s_axi_CTRL_RDATA[10]),
        .R(\rdata[31]_i_1_n_0 ));
  FDRE \rdata_reg[11] 
       (.C(ap_clk),
        .CE(ar_hs),
        .D(\rdata_reg[11]_0 ),
        .Q(s_axi_CTRL_RDATA[11]),
        .R(\rdata[31]_i_1_n_0 ));
  FDRE \rdata_reg[12] 
       (.C(ap_clk),
        .CE(ar_hs),
        .D(\rdata_reg[12]_0 ),
        .Q(s_axi_CTRL_RDATA[12]),
        .R(\rdata[31]_i_1_n_0 ));
  FDRE \rdata_reg[13] 
       (.C(ap_clk),
        .CE(ar_hs),
        .D(\rdata_reg[13]_0 ),
        .Q(s_axi_CTRL_RDATA[13]),
        .R(\rdata[31]_i_1_n_0 ));
  FDRE \rdata_reg[14] 
       (.C(ap_clk),
        .CE(ar_hs),
        .D(\rdata_reg[14]_0 ),
        .Q(s_axi_CTRL_RDATA[14]),
        .R(\rdata[31]_i_1_n_0 ));
  FDRE \rdata_reg[15] 
       (.C(ap_clk),
        .CE(ar_hs),
        .D(\rdata_reg[15]_0 ),
        .Q(s_axi_CTRL_RDATA[15]),
        .R(\rdata[31]_i_1_n_0 ));
  FDRE \rdata_reg[16] 
       (.C(ap_clk),
        .CE(ar_hs),
        .D(\rdata_reg[16]_i_1_n_0 ),
        .Q(s_axi_CTRL_RDATA[16]),
        .R(\rdata[31]_i_1_n_0 ));
  MUXF7 \rdata_reg[16]_i_1 
       (.I0(\rdata[16]_i_2_n_0 ),
        .I1(\rdata_reg[16]_0 ),
        .O(\rdata_reg[16]_i_1_n_0 ),
        .S(s_axi_CTRL_ARADDR[2]));
  FDRE \rdata_reg[17] 
       (.C(ap_clk),
        .CE(ar_hs),
        .D(\rdata_reg[17]_i_1_n_0 ),
        .Q(s_axi_CTRL_RDATA[17]),
        .R(\rdata[31]_i_1_n_0 ));
  MUXF7 \rdata_reg[17]_i_1 
       (.I0(\rdata[17]_i_2_n_0 ),
        .I1(\rdata_reg[17]_0 ),
        .O(\rdata_reg[17]_i_1_n_0 ),
        .S(s_axi_CTRL_ARADDR[2]));
  FDRE \rdata_reg[18] 
       (.C(ap_clk),
        .CE(ar_hs),
        .D(\rdata_reg[18]_i_1_n_0 ),
        .Q(s_axi_CTRL_RDATA[18]),
        .R(\rdata[31]_i_1_n_0 ));
  MUXF7 \rdata_reg[18]_i_1 
       (.I0(\rdata[18]_i_2_n_0 ),
        .I1(\rdata_reg[18]_0 ),
        .O(\rdata_reg[18]_i_1_n_0 ),
        .S(s_axi_CTRL_ARADDR[2]));
  FDRE \rdata_reg[19] 
       (.C(ap_clk),
        .CE(ar_hs),
        .D(\rdata_reg[19]_i_1_n_0 ),
        .Q(s_axi_CTRL_RDATA[19]),
        .R(\rdata[31]_i_1_n_0 ));
  MUXF7 \rdata_reg[19]_i_1 
       (.I0(\rdata[19]_i_2_n_0 ),
        .I1(\rdata_reg[19]_0 ),
        .O(\rdata_reg[19]_i_1_n_0 ),
        .S(s_axi_CTRL_ARADDR[2]));
  FDRE \rdata_reg[1] 
       (.C(ap_clk),
        .CE(ar_hs),
        .D(\rdata_reg[1]_0 ),
        .Q(s_axi_CTRL_RDATA[1]),
        .R(\rdata[31]_i_1_n_0 ));
  FDRE \rdata_reg[20] 
       (.C(ap_clk),
        .CE(ar_hs),
        .D(\rdata_reg[20]_i_1_n_0 ),
        .Q(s_axi_CTRL_RDATA[20]),
        .R(\rdata[31]_i_1_n_0 ));
  MUXF7 \rdata_reg[20]_i_1 
       (.I0(\rdata[20]_i_2_n_0 ),
        .I1(\rdata_reg[20]_0 ),
        .O(\rdata_reg[20]_i_1_n_0 ),
        .S(s_axi_CTRL_ARADDR[2]));
  FDRE \rdata_reg[21] 
       (.C(ap_clk),
        .CE(ar_hs),
        .D(\rdata_reg[21]_i_1_n_0 ),
        .Q(s_axi_CTRL_RDATA[21]),
        .R(\rdata[31]_i_1_n_0 ));
  MUXF7 \rdata_reg[21]_i_1 
       (.I0(\rdata[21]_i_2_n_0 ),
        .I1(\rdata_reg[21]_0 ),
        .O(\rdata_reg[21]_i_1_n_0 ),
        .S(s_axi_CTRL_ARADDR[2]));
  FDRE \rdata_reg[22] 
       (.C(ap_clk),
        .CE(ar_hs),
        .D(\rdata_reg[22]_i_1_n_0 ),
        .Q(s_axi_CTRL_RDATA[22]),
        .R(\rdata[31]_i_1_n_0 ));
  MUXF7 \rdata_reg[22]_i_1 
       (.I0(\rdata[22]_i_2_n_0 ),
        .I1(\rdata_reg[22]_0 ),
        .O(\rdata_reg[22]_i_1_n_0 ),
        .S(s_axi_CTRL_ARADDR[2]));
  FDRE \rdata_reg[23] 
       (.C(ap_clk),
        .CE(ar_hs),
        .D(\rdata_reg[23]_i_1_n_0 ),
        .Q(s_axi_CTRL_RDATA[23]),
        .R(\rdata[31]_i_1_n_0 ));
  MUXF7 \rdata_reg[23]_i_1 
       (.I0(\rdata[23]_i_2_n_0 ),
        .I1(\rdata_reg[23]_0 ),
        .O(\rdata_reg[23]_i_1_n_0 ),
        .S(s_axi_CTRL_ARADDR[2]));
  FDRE \rdata_reg[24] 
       (.C(ap_clk),
        .CE(ar_hs),
        .D(\rdata_reg[24]_i_1_n_0 ),
        .Q(s_axi_CTRL_RDATA[24]),
        .R(\rdata[31]_i_1_n_0 ));
  MUXF7 \rdata_reg[24]_i_1 
       (.I0(\rdata[24]_i_2_n_0 ),
        .I1(\rdata[24]_i_3_n_0 ),
        .O(\rdata_reg[24]_i_1_n_0 ),
        .S(s_axi_CTRL_ARADDR[2]));
  FDRE \rdata_reg[25] 
       (.C(ap_clk),
        .CE(ar_hs),
        .D(\rdata_reg[25]_i_1_n_0 ),
        .Q(s_axi_CTRL_RDATA[25]),
        .R(\rdata[31]_i_1_n_0 ));
  MUXF7 \rdata_reg[25]_i_1 
       (.I0(\rdata[25]_i_2_n_0 ),
        .I1(\rdata[25]_i_3_n_0 ),
        .O(\rdata_reg[25]_i_1_n_0 ),
        .S(s_axi_CTRL_ARADDR[2]));
  FDRE \rdata_reg[26] 
       (.C(ap_clk),
        .CE(ar_hs),
        .D(\rdata_reg[26]_i_1_n_0 ),
        .Q(s_axi_CTRL_RDATA[26]),
        .R(\rdata[31]_i_1_n_0 ));
  MUXF7 \rdata_reg[26]_i_1 
       (.I0(\rdata[26]_i_2_n_0 ),
        .I1(\rdata[26]_i_3_n_0 ),
        .O(\rdata_reg[26]_i_1_n_0 ),
        .S(s_axi_CTRL_ARADDR[2]));
  FDRE \rdata_reg[27] 
       (.C(ap_clk),
        .CE(ar_hs),
        .D(\rdata_reg[27]_i_1_n_0 ),
        .Q(s_axi_CTRL_RDATA[27]),
        .R(\rdata[31]_i_1_n_0 ));
  MUXF7 \rdata_reg[27]_i_1 
       (.I0(\rdata[27]_i_2_n_0 ),
        .I1(\rdata[27]_i_3_n_0 ),
        .O(\rdata_reg[27]_i_1_n_0 ),
        .S(s_axi_CTRL_ARADDR[2]));
  FDRE \rdata_reg[28] 
       (.C(ap_clk),
        .CE(ar_hs),
        .D(\rdata_reg[28]_i_1_n_0 ),
        .Q(s_axi_CTRL_RDATA[28]),
        .R(\rdata[31]_i_1_n_0 ));
  MUXF7 \rdata_reg[28]_i_1 
       (.I0(\rdata[28]_i_2_n_0 ),
        .I1(\rdata[28]_i_3_n_0 ),
        .O(\rdata_reg[28]_i_1_n_0 ),
        .S(s_axi_CTRL_ARADDR[2]));
  FDRE \rdata_reg[29] 
       (.C(ap_clk),
        .CE(ar_hs),
        .D(\rdata_reg[29]_i_1_n_0 ),
        .Q(s_axi_CTRL_RDATA[29]),
        .R(\rdata[31]_i_1_n_0 ));
  MUXF7 \rdata_reg[29]_i_1 
       (.I0(\rdata[29]_i_2_n_0 ),
        .I1(\rdata[29]_i_3_n_0 ),
        .O(\rdata_reg[29]_i_1_n_0 ),
        .S(s_axi_CTRL_ARADDR[2]));
  FDRE \rdata_reg[2] 
       (.C(ap_clk),
        .CE(ar_hs),
        .D(\rdata_reg[2]_0 ),
        .Q(s_axi_CTRL_RDATA[2]),
        .R(\rdata[31]_i_1_n_0 ));
  FDRE \rdata_reg[30] 
       (.C(ap_clk),
        .CE(ar_hs),
        .D(\rdata_reg[30]_i_1_n_0 ),
        .Q(s_axi_CTRL_RDATA[30]),
        .R(\rdata[31]_i_1_n_0 ));
  MUXF7 \rdata_reg[30]_i_1 
       (.I0(\rdata[30]_i_2_n_0 ),
        .I1(\rdata[30]_i_3_n_0 ),
        .O(\rdata_reg[30]_i_1_n_0 ),
        .S(s_axi_CTRL_ARADDR[2]));
  FDRE \rdata_reg[31] 
       (.C(ap_clk),
        .CE(ar_hs),
        .D(\rdata_reg[31]_i_3_n_0 ),
        .Q(s_axi_CTRL_RDATA[31]),
        .R(\rdata[31]_i_1_n_0 ));
  MUXF7 \rdata_reg[31]_i_3 
       (.I0(\rdata[31]_i_4_n_0 ),
        .I1(\rdata[31]_i_5_n_0 ),
        .O(\rdata_reg[31]_i_3_n_0 ),
        .S(s_axi_CTRL_ARADDR[2]));
  FDRE \rdata_reg[3] 
       (.C(ap_clk),
        .CE(ar_hs),
        .D(\rdata_reg[3]_0 ),
        .Q(s_axi_CTRL_RDATA[3]),
        .R(\rdata[31]_i_1_n_0 ));
  FDRE \rdata_reg[4] 
       (.C(ap_clk),
        .CE(ar_hs),
        .D(\rdata_reg[4]_0 ),
        .Q(s_axi_CTRL_RDATA[4]),
        .R(\rdata[31]_i_1_n_0 ));
  FDRE \rdata_reg[5] 
       (.C(ap_clk),
        .CE(ar_hs),
        .D(\rdata_reg[5]_0 ),
        .Q(s_axi_CTRL_RDATA[5]),
        .R(\rdata[31]_i_1_n_0 ));
  FDRE \rdata_reg[6] 
       (.C(ap_clk),
        .CE(ar_hs),
        .D(\rdata_reg[6]_0 ),
        .Q(s_axi_CTRL_RDATA[6]),
        .R(\rdata[31]_i_1_n_0 ));
  FDRE \rdata_reg[7] 
       (.C(ap_clk),
        .CE(ar_hs),
        .D(\rdata_reg[7]_0 ),
        .Q(s_axi_CTRL_RDATA[7]),
        .R(\rdata[31]_i_1_n_0 ));
  FDRE \rdata_reg[8] 
       (.C(ap_clk),
        .CE(ar_hs),
        .D(\rdata_reg[8]_0 ),
        .Q(s_axi_CTRL_RDATA[8]),
        .R(\rdata[31]_i_1_n_0 ));
  FDRE \rdata_reg[9] 
       (.C(ap_clk),
        .CE(ar_hs),
        .D(\rdata_reg[9]_0 ),
        .Q(s_axi_CTRL_RDATA[9]),
        .R(\rdata[31]_i_1_n_0 ));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__0_i_1
       (.I0(P[14]),
        .I1(\int_p_reg_n_0_[14] ),
        .O(\int_p_reg[14]_0 [7]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__0_i_2
       (.I0(P[13]),
        .I1(\int_p_reg_n_0_[13] ),
        .O(\int_p_reg[14]_0 [6]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__0_i_3
       (.I0(P[12]),
        .I1(\int_p_reg_n_0_[12] ),
        .O(\int_p_reg[14]_0 [5]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__0_i_4
       (.I0(P[11]),
        .I1(\int_p_reg_n_0_[11] ),
        .O(\int_p_reg[14]_0 [4]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__0_i_5
       (.I0(P[10]),
        .I1(\int_p_reg_n_0_[10] ),
        .O(\int_p_reg[14]_0 [3]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__0_i_6
       (.I0(P[9]),
        .I1(\int_p_reg_n_0_[9] ),
        .O(\int_p_reg[14]_0 [2]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__0_i_7
       (.I0(P[8]),
        .I1(\int_p_reg_n_0_[8] ),
        .O(\int_p_reg[14]_0 [1]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__0_i_8
       (.I0(P[7]),
        .I1(\int_p_reg_n_0_[7] ),
        .O(\int_p_reg[14]_0 [0]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__1_i_1
       (.I0(P[22]),
        .I1(int_p_reg_n_100),
        .O(int_ap_start_reg_0[7]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__1_i_2
       (.I0(P[21]),
        .I1(int_p_reg_n_101),
        .O(int_ap_start_reg_0[6]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__1_i_3
       (.I0(P[20]),
        .I1(int_p_reg_n_102),
        .O(int_ap_start_reg_0[5]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__1_i_4
       (.I0(P[19]),
        .I1(int_p_reg_n_103),
        .O(int_ap_start_reg_0[4]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__1_i_5
       (.I0(P[18]),
        .I1(int_p_reg_n_104),
        .O(int_ap_start_reg_0[3]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__1_i_6
       (.I0(P[17]),
        .I1(int_p_reg_n_105),
        .O(int_ap_start_reg_0[2]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__1_i_7
       (.I0(P[16]),
        .I1(\int_p_reg_n_0_[16] ),
        .O(int_ap_start_reg_0[1]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__1_i_8
       (.I0(P[15]),
        .I1(\int_p_reg_n_0_[15] ),
        .O(int_ap_start_reg_0[0]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__2_i_1
       (.I0(P[30]),
        .I1(int_p_reg_n_92),
        .O(int_ap_start_reg_1[7]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__2_i_2
       (.I0(P[29]),
        .I1(int_p_reg_n_93),
        .O(int_ap_start_reg_1[6]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__2_i_3
       (.I0(P[28]),
        .I1(int_p_reg_n_94),
        .O(int_ap_start_reg_1[5]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__2_i_4
       (.I0(P[27]),
        .I1(int_p_reg_n_95),
        .O(int_ap_start_reg_1[4]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__2_i_5
       (.I0(P[26]),
        .I1(int_p_reg_n_96),
        .O(int_ap_start_reg_1[3]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__2_i_6
       (.I0(P[25]),
        .I1(int_p_reg_n_97),
        .O(int_ap_start_reg_1[2]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__2_i_7
       (.I0(P[24]),
        .I1(int_p_reg_n_98),
        .O(int_ap_start_reg_1[1]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__2_i_8
       (.I0(P[23]),
        .I1(int_p_reg_n_99),
        .O(int_ap_start_reg_1[0]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__3_i_1
       (.I0(P[38]),
        .I1(int_p_reg_n_84),
        .O(int_ap_start_reg_2[7]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__3_i_2
       (.I0(P[37]),
        .I1(int_p_reg_n_85),
        .O(int_ap_start_reg_2[6]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__3_i_3
       (.I0(P[36]),
        .I1(int_p_reg_n_86),
        .O(int_ap_start_reg_2[5]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__3_i_4
       (.I0(P[35]),
        .I1(int_p_reg_n_87),
        .O(int_ap_start_reg_2[4]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__3_i_5
       (.I0(P[34]),
        .I1(int_p_reg_n_88),
        .O(int_ap_start_reg_2[3]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__3_i_6
       (.I0(P[33]),
        .I1(int_p_reg_n_89),
        .O(int_ap_start_reg_2[2]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__3_i_7
       (.I0(P[32]),
        .I1(int_p_reg_n_90),
        .O(int_ap_start_reg_2[1]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__3_i_8
       (.I0(P[31]),
        .I1(int_p_reg_n_91),
        .O(int_ap_start_reg_2[0]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__4_i_1
       (.I0(int_p_reg__0_n_59),
        .I1(int_p_reg_n_76),
        .O(S[7]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__4_i_2
       (.I0(P[45]),
        .I1(int_p_reg_n_77),
        .O(S[6]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__4_i_3
       (.I0(P[44]),
        .I1(int_p_reg_n_78),
        .O(S[5]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__4_i_4
       (.I0(P[43]),
        .I1(int_p_reg_n_79),
        .O(S[4]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__4_i_5
       (.I0(P[42]),
        .I1(int_p_reg_n_80),
        .O(S[3]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__4_i_6
       (.I0(P[41]),
        .I1(int_p_reg_n_81),
        .O(S[2]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__4_i_7
       (.I0(P[40]),
        .I1(int_p_reg_n_82),
        .O(S[1]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry__4_i_8
       (.I0(P[39]),
        .I1(int_p_reg_n_83),
        .O(S[0]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry_i_1
       (.I0(P[6]),
        .I1(\int_p_reg_n_0_[6] ),
        .O(\int_p_reg[6]_0 [7]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry_i_2
       (.I0(P[5]),
        .I1(\int_p_reg_n_0_[5] ),
        .O(\int_p_reg[6]_0 [6]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry_i_3
       (.I0(P[4]),
        .I1(\int_p_reg_n_0_[4] ),
        .O(\int_p_reg[6]_0 [5]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry_i_4
       (.I0(P[3]),
        .I1(\int_p_reg_n_0_[3] ),
        .O(\int_p_reg[6]_0 [4]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry_i_5
       (.I0(P[2]),
        .I1(\int_p_reg_n_0_[2] ),
        .O(\int_p_reg[6]_0 [3]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry_i_6
       (.I0(P[1]),
        .I1(\int_p_reg_n_0_[1] ),
        .O(\int_p_reg[6]_0 [2]));
  LUT2 #(
    .INIT(4'h6)) 
    tmp_product_carry_i_7
       (.I0(P[0]),
        .I1(\int_p_reg_n_0_[0] ),
        .O(\int_p_reg[6]_0 [1]));
  LUT2 #(
    .INIT(4'h8)) 
    \waddr[5]_i_1 
       (.I0(s_axi_CTRL_AWVALID),
        .I1(\FSM_onehot_wstate_reg[1]_0 ),
        .O(waddr));
  FDRE \waddr_reg[2] 
       (.C(ap_clk),
        .CE(waddr),
        .D(s_axi_CTRL_AWADDR[0]),
        .Q(\waddr_reg_n_0_[2] ),
        .R(1'b0));
  FDRE \waddr_reg[3] 
       (.C(ap_clk),
        .CE(waddr),
        .D(s_axi_CTRL_AWADDR[1]),
        .Q(\waddr_reg_n_0_[3] ),
        .R(1'b0));
  FDRE \waddr_reg[4] 
       (.C(ap_clk),
        .CE(waddr),
        .D(s_axi_CTRL_AWADDR[2]),
        .Q(\waddr_reg_n_0_[4] ),
        .R(1'b0));
  FDRE \waddr_reg[5] 
       (.C(ap_clk),
        .CE(waddr),
        .D(s_axi_CTRL_AWADDR[3]),
        .Q(\waddr_reg_n_0_[5] ),
        .R(1'b0));
endmodule

(* ORIG_REF_NAME = "mul32_hls_mul_32ns_32ns_64_1_1" *) 
module Adder32bit_mul32_hls_0_0_mul32_hls_mul_32ns_32ns_64_1_1
   (P,
    PCOUT,
    ap_clk_0,
    ap_clk_1,
    O,
    \int_p_reg[16]__0 ,
    int_ap_start_reg,
    s_axi_CTRL_ARADDR_3_sp_1,
    \s_axi_CTRL_ARADDR[3]_0 ,
    \s_axi_CTRL_ARADDR[3]_1 ,
    \s_axi_CTRL_ARADDR[3]_2 ,
    \s_axi_CTRL_ARADDR[3]_3 ,
    \s_axi_CTRL_ARADDR[3]_4 ,
    \s_axi_CTRL_ARADDR[3]_5 ,
    \s_axi_CTRL_ARADDR[3]_6 ,
    \s_axi_CTRL_ARADDR[3]_7 ,
    \s_axi_CTRL_ARADDR[3]_8 ,
    \s_axi_CTRL_ARADDR[3]_9 ,
    \s_axi_CTRL_ARADDR[3]_10 ,
    \s_axi_CTRL_ARADDR[3]_11 ,
    \s_axi_CTRL_ARADDR[3]_12 ,
    \s_axi_CTRL_ARADDR[3]_13 ,
    \s_axi_CTRL_ARADDR[3]_14 ,
    \s_axi_CTRL_ARADDR[3]_15 ,
    \s_axi_CTRL_ARADDR[3]_16 ,
    \s_axi_CTRL_ARADDR[3]_17 ,
    \s_axi_CTRL_ARADDR[3]_18 ,
    \s_axi_CTRL_ARADDR[3]_19 ,
    \s_axi_CTRL_ARADDR[3]_20 ,
    \s_axi_CTRL_ARADDR[3]_21 ,
    CEB2,
    E,
    ap_clk,
    RSTB,
    DSP_ALU_INST,
    D,
    \rdata[24]_i_3 ,
    \rdata[16]_i_2 ,
    \rdata[24]_i_2 ,
    \rdata_reg[7] ,
    \rdata_reg[15] ,
    \rdata[16]_i_3_0 ,
    S,
    s_axi_CTRL_ARADDR,
    \rdata_reg[1] ,
    \rdata_reg[2] ,
    \rdata_reg[3] ,
    \rdata_reg[4] ,
    \rdata_reg[5] ,
    \rdata_reg[6] ,
    \rdata_reg[7]_0 ,
    \rdata_reg[8] ,
    \rdata_reg[9] ,
    \rdata_reg[10] ,
    \rdata_reg[11] ,
    \rdata_reg[12] ,
    \rdata_reg[13] ,
    \rdata_reg[14] ,
    \rdata_reg[15]_0 );
  output [16:0]P;
  output [47:0]PCOUT;
  output [16:0]ap_clk_0;
  output [47:0]ap_clk_1;
  output [7:0]O;
  output [7:0]\int_p_reg[16]__0 ;
  output [8:0]int_ap_start_reg;
  output s_axi_CTRL_ARADDR_3_sp_1;
  output \s_axi_CTRL_ARADDR[3]_0 ;
  output \s_axi_CTRL_ARADDR[3]_1 ;
  output \s_axi_CTRL_ARADDR[3]_2 ;
  output \s_axi_CTRL_ARADDR[3]_3 ;
  output \s_axi_CTRL_ARADDR[3]_4 ;
  output \s_axi_CTRL_ARADDR[3]_5 ;
  output \s_axi_CTRL_ARADDR[3]_6 ;
  output \s_axi_CTRL_ARADDR[3]_7 ;
  output \s_axi_CTRL_ARADDR[3]_8 ;
  output \s_axi_CTRL_ARADDR[3]_9 ;
  output \s_axi_CTRL_ARADDR[3]_10 ;
  output \s_axi_CTRL_ARADDR[3]_11 ;
  output \s_axi_CTRL_ARADDR[3]_12 ;
  output \s_axi_CTRL_ARADDR[3]_13 ;
  output \s_axi_CTRL_ARADDR[3]_14 ;
  output \s_axi_CTRL_ARADDR[3]_15 ;
  output \s_axi_CTRL_ARADDR[3]_16 ;
  output \s_axi_CTRL_ARADDR[3]_17 ;
  output \s_axi_CTRL_ARADDR[3]_18 ;
  output \s_axi_CTRL_ARADDR[3]_19 ;
  output \s_axi_CTRL_ARADDR[3]_20 ;
  output \s_axi_CTRL_ARADDR[3]_21 ;
  input CEB2;
  input [0:0]E;
  input ap_clk;
  input RSTB;
  input [31:0]DSP_ALU_INST;
  input [16:0]D;
  input [45:0]\rdata[24]_i_3 ;
  input [7:0]\rdata[16]_i_2 ;
  input [7:0]\rdata[24]_i_2 ;
  input [7:0]\rdata_reg[7] ;
  input [7:0]\rdata_reg[15] ;
  input [7:0]\rdata[16]_i_3_0 ;
  input [7:0]S;
  input [3:0]s_axi_CTRL_ARADDR;
  input \rdata_reg[1] ;
  input \rdata_reg[2] ;
  input \rdata_reg[3] ;
  input \rdata_reg[4] ;
  input \rdata_reg[5] ;
  input \rdata_reg[6] ;
  input \rdata_reg[7]_0 ;
  input \rdata_reg[8] ;
  input \rdata_reg[9] ;
  input \rdata_reg[10] ;
  input \rdata_reg[11] ;
  input \rdata_reg[12] ;
  input \rdata_reg[13] ;
  input \rdata_reg[14] ;
  input \rdata_reg[15]_0 ;

  wire CEB2;
  wire [16:0]D;
  wire [31:0]DSP_ALU_INST;
  wire [0:0]E;
  wire [7:0]O;
  wire [16:0]P;
  wire [47:0]PCOUT;
  wire RSTB;
  wire [7:0]S;
  wire ap_clk;
  wire [16:0]ap_clk_0;
  wire [47:0]ap_clk_1;
  wire [23:1]data7;
  wire [8:0]int_ap_start_reg;
  wire [7:0]\int_p_reg[16]__0 ;
  wire [7:0]\rdata[16]_i_2 ;
  wire [7:0]\rdata[16]_i_3_0 ;
  wire [7:0]\rdata[24]_i_2 ;
  wire [45:0]\rdata[24]_i_3 ;
  wire \rdata_reg[10] ;
  wire \rdata_reg[11] ;
  wire \rdata_reg[12] ;
  wire \rdata_reg[13] ;
  wire \rdata_reg[14] ;
  wire [7:0]\rdata_reg[15] ;
  wire \rdata_reg[15]_0 ;
  wire \rdata_reg[1] ;
  wire \rdata_reg[2] ;
  wire \rdata_reg[3] ;
  wire \rdata_reg[4] ;
  wire \rdata_reg[5] ;
  wire \rdata_reg[6] ;
  wire [7:0]\rdata_reg[7] ;
  wire \rdata_reg[7]_0 ;
  wire \rdata_reg[8] ;
  wire \rdata_reg[9] ;
  wire [3:0]s_axi_CTRL_ARADDR;
  wire \s_axi_CTRL_ARADDR[3]_0 ;
  wire \s_axi_CTRL_ARADDR[3]_1 ;
  wire \s_axi_CTRL_ARADDR[3]_10 ;
  wire \s_axi_CTRL_ARADDR[3]_11 ;
  wire \s_axi_CTRL_ARADDR[3]_12 ;
  wire \s_axi_CTRL_ARADDR[3]_13 ;
  wire \s_axi_CTRL_ARADDR[3]_14 ;
  wire \s_axi_CTRL_ARADDR[3]_15 ;
  wire \s_axi_CTRL_ARADDR[3]_16 ;
  wire \s_axi_CTRL_ARADDR[3]_17 ;
  wire \s_axi_CTRL_ARADDR[3]_18 ;
  wire \s_axi_CTRL_ARADDR[3]_19 ;
  wire \s_axi_CTRL_ARADDR[3]_2 ;
  wire \s_axi_CTRL_ARADDR[3]_20 ;
  wire \s_axi_CTRL_ARADDR[3]_21 ;
  wire \s_axi_CTRL_ARADDR[3]_3 ;
  wire \s_axi_CTRL_ARADDR[3]_4 ;
  wire \s_axi_CTRL_ARADDR[3]_5 ;
  wire \s_axi_CTRL_ARADDR[3]_6 ;
  wire \s_axi_CTRL_ARADDR[3]_7 ;
  wire \s_axi_CTRL_ARADDR[3]_8 ;
  wire \s_axi_CTRL_ARADDR[3]_9 ;
  wire s_axi_CTRL_ARADDR_3_sn_1;
  wire tmp_product__0_n_58;
  wire tmp_product__0_n_59;
  wire tmp_product__0_n_60;
  wire tmp_product__0_n_61;
  wire tmp_product__0_n_62;
  wire tmp_product__0_n_63;
  wire tmp_product__0_n_64;
  wire tmp_product__0_n_65;
  wire tmp_product__0_n_66;
  wire tmp_product__0_n_67;
  wire tmp_product__0_n_68;
  wire tmp_product__0_n_69;
  wire tmp_product__0_n_70;
  wire tmp_product__0_n_71;
  wire tmp_product__0_n_72;
  wire tmp_product__0_n_73;
  wire tmp_product__0_n_74;
  wire tmp_product__0_n_75;
  wire tmp_product__0_n_76;
  wire tmp_product__0_n_77;
  wire tmp_product__0_n_78;
  wire tmp_product__0_n_79;
  wire tmp_product__0_n_80;
  wire tmp_product__0_n_81;
  wire tmp_product__0_n_82;
  wire tmp_product__0_n_83;
  wire tmp_product__0_n_84;
  wire tmp_product__0_n_85;
  wire tmp_product__0_n_86;
  wire tmp_product__0_n_87;
  wire tmp_product__0_n_88;
  wire tmp_product_carry__0_n_0;
  wire tmp_product_carry__0_n_1;
  wire tmp_product_carry__0_n_2;
  wire tmp_product_carry__0_n_3;
  wire tmp_product_carry__0_n_4;
  wire tmp_product_carry__0_n_5;
  wire tmp_product_carry__0_n_6;
  wire tmp_product_carry__0_n_7;
  wire tmp_product_carry__1_n_0;
  wire tmp_product_carry__1_n_1;
  wire tmp_product_carry__1_n_2;
  wire tmp_product_carry__1_n_3;
  wire tmp_product_carry__1_n_4;
  wire tmp_product_carry__1_n_5;
  wire tmp_product_carry__1_n_6;
  wire tmp_product_carry__1_n_7;
  wire tmp_product_carry__2_n_0;
  wire tmp_product_carry__2_n_1;
  wire tmp_product_carry__2_n_2;
  wire tmp_product_carry__2_n_3;
  wire tmp_product_carry__2_n_4;
  wire tmp_product_carry__2_n_5;
  wire tmp_product_carry__2_n_6;
  wire tmp_product_carry__2_n_7;
  wire tmp_product_carry__3_n_0;
  wire tmp_product_carry__3_n_1;
  wire tmp_product_carry__3_n_2;
  wire tmp_product_carry__3_n_3;
  wire tmp_product_carry__3_n_4;
  wire tmp_product_carry__3_n_5;
  wire tmp_product_carry__3_n_6;
  wire tmp_product_carry__3_n_7;
  wire tmp_product_carry__4_n_1;
  wire tmp_product_carry__4_n_2;
  wire tmp_product_carry__4_n_3;
  wire tmp_product_carry__4_n_4;
  wire tmp_product_carry__4_n_5;
  wire tmp_product_carry__4_n_6;
  wire tmp_product_carry__4_n_7;
  wire tmp_product_carry_n_0;
  wire tmp_product_carry_n_1;
  wire tmp_product_carry_n_2;
  wire tmp_product_carry_n_3;
  wire tmp_product_carry_n_4;
  wire tmp_product_carry_n_5;
  wire tmp_product_carry_n_6;
  wire tmp_product_carry_n_7;
  wire tmp_product_n_58;
  wire tmp_product_n_59;
  wire tmp_product_n_60;
  wire tmp_product_n_61;
  wire tmp_product_n_62;
  wire tmp_product_n_63;
  wire tmp_product_n_64;
  wire tmp_product_n_65;
  wire tmp_product_n_66;
  wire tmp_product_n_67;
  wire tmp_product_n_68;
  wire tmp_product_n_69;
  wire tmp_product_n_70;
  wire tmp_product_n_71;
  wire tmp_product_n_72;
  wire tmp_product_n_73;
  wire tmp_product_n_74;
  wire tmp_product_n_75;
  wire tmp_product_n_76;
  wire tmp_product_n_77;
  wire tmp_product_n_78;
  wire tmp_product_n_79;
  wire tmp_product_n_80;
  wire tmp_product_n_81;
  wire tmp_product_n_82;
  wire tmp_product_n_83;
  wire tmp_product_n_84;
  wire tmp_product_n_85;
  wire tmp_product_n_86;
  wire tmp_product_n_87;
  wire tmp_product_n_88;
  wire NLW_tmp_product_CARRYCASCOUT_UNCONNECTED;
  wire NLW_tmp_product_MULTSIGNOUT_UNCONNECTED;
  wire NLW_tmp_product_OVERFLOW_UNCONNECTED;
  wire NLW_tmp_product_PATTERNBDETECT_UNCONNECTED;
  wire NLW_tmp_product_PATTERNDETECT_UNCONNECTED;
  wire NLW_tmp_product_UNDERFLOW_UNCONNECTED;
  wire [29:0]NLW_tmp_product_ACOUT_UNCONNECTED;
  wire [17:0]NLW_tmp_product_BCOUT_UNCONNECTED;
  wire [3:0]NLW_tmp_product_CARRYOUT_UNCONNECTED;
  wire [7:0]NLW_tmp_product_XOROUT_UNCONNECTED;
  wire NLW_tmp_product__0_CARRYCASCOUT_UNCONNECTED;
  wire NLW_tmp_product__0_MULTSIGNOUT_UNCONNECTED;
  wire NLW_tmp_product__0_OVERFLOW_UNCONNECTED;
  wire NLW_tmp_product__0_PATTERNBDETECT_UNCONNECTED;
  wire NLW_tmp_product__0_PATTERNDETECT_UNCONNECTED;
  wire NLW_tmp_product__0_UNDERFLOW_UNCONNECTED;
  wire [29:0]NLW_tmp_product__0_ACOUT_UNCONNECTED;
  wire [17:0]NLW_tmp_product__0_BCOUT_UNCONNECTED;
  wire [3:0]NLW_tmp_product__0_CARRYOUT_UNCONNECTED;
  wire [7:0]NLW_tmp_product__0_XOROUT_UNCONNECTED;
  wire [7:7]NLW_tmp_product_carry__4_CO_UNCONNECTED;

  assign s_axi_CTRL_ARADDR_3_sp_1 = s_axi_CTRL_ARADDR_3_sn_1;
  LUT6 #(
    .INIT(64'h1000FFFF10000000)) 
    \rdata[10]_i_1 
       (.I0(s_axi_CTRL_ARADDR[1]),
        .I1(s_axi_CTRL_ARADDR[2]),
        .I2(data7[10]),
        .I3(s_axi_CTRL_ARADDR[3]),
        .I4(s_axi_CTRL_ARADDR[0]),
        .I5(\rdata_reg[10] ),
        .O(\s_axi_CTRL_ARADDR[3]_8 ));
  LUT6 #(
    .INIT(64'h1000FFFF10000000)) 
    \rdata[11]_i_1 
       (.I0(s_axi_CTRL_ARADDR[1]),
        .I1(s_axi_CTRL_ARADDR[2]),
        .I2(data7[11]),
        .I3(s_axi_CTRL_ARADDR[3]),
        .I4(s_axi_CTRL_ARADDR[0]),
        .I5(\rdata_reg[11] ),
        .O(\s_axi_CTRL_ARADDR[3]_9 ));
  LUT6 #(
    .INIT(64'h1000FFFF10000000)) 
    \rdata[12]_i_1 
       (.I0(s_axi_CTRL_ARADDR[1]),
        .I1(s_axi_CTRL_ARADDR[2]),
        .I2(data7[12]),
        .I3(s_axi_CTRL_ARADDR[3]),
        .I4(s_axi_CTRL_ARADDR[0]),
        .I5(\rdata_reg[12] ),
        .O(\s_axi_CTRL_ARADDR[3]_10 ));
  LUT6 #(
    .INIT(64'h1000FFFF10000000)) 
    \rdata[13]_i_1 
       (.I0(s_axi_CTRL_ARADDR[1]),
        .I1(s_axi_CTRL_ARADDR[2]),
        .I2(data7[13]),
        .I3(s_axi_CTRL_ARADDR[3]),
        .I4(s_axi_CTRL_ARADDR[0]),
        .I5(\rdata_reg[13] ),
        .O(\s_axi_CTRL_ARADDR[3]_11 ));
  LUT6 #(
    .INIT(64'h1000FFFF10000000)) 
    \rdata[14]_i_1 
       (.I0(s_axi_CTRL_ARADDR[1]),
        .I1(s_axi_CTRL_ARADDR[2]),
        .I2(data7[14]),
        .I3(s_axi_CTRL_ARADDR[3]),
        .I4(s_axi_CTRL_ARADDR[0]),
        .I5(\rdata_reg[14] ),
        .O(\s_axi_CTRL_ARADDR[3]_12 ));
  LUT6 #(
    .INIT(64'h1000FFFF10000000)) 
    \rdata[15]_i_1 
       (.I0(s_axi_CTRL_ARADDR[1]),
        .I1(s_axi_CTRL_ARADDR[2]),
        .I2(data7[15]),
        .I3(s_axi_CTRL_ARADDR[3]),
        .I4(s_axi_CTRL_ARADDR[0]),
        .I5(\rdata_reg[15]_0 ),
        .O(\s_axi_CTRL_ARADDR[3]_13 ));
  LUT4 #(
    .INIT(16'h1000)) 
    \rdata[16]_i_3 
       (.I0(s_axi_CTRL_ARADDR[1]),
        .I1(s_axi_CTRL_ARADDR[2]),
        .I2(data7[16]),
        .I3(s_axi_CTRL_ARADDR[3]),
        .O(\s_axi_CTRL_ARADDR[3]_14 ));
  LUT4 #(
    .INIT(16'h1000)) 
    \rdata[17]_i_3 
       (.I0(s_axi_CTRL_ARADDR[1]),
        .I1(s_axi_CTRL_ARADDR[2]),
        .I2(data7[17]),
        .I3(s_axi_CTRL_ARADDR[3]),
        .O(\s_axi_CTRL_ARADDR[3]_15 ));
  LUT4 #(
    .INIT(16'h1000)) 
    \rdata[18]_i_3 
       (.I0(s_axi_CTRL_ARADDR[1]),
        .I1(s_axi_CTRL_ARADDR[2]),
        .I2(data7[18]),
        .I3(s_axi_CTRL_ARADDR[3]),
        .O(\s_axi_CTRL_ARADDR[3]_16 ));
  LUT4 #(
    .INIT(16'h1000)) 
    \rdata[19]_i_3 
       (.I0(s_axi_CTRL_ARADDR[1]),
        .I1(s_axi_CTRL_ARADDR[2]),
        .I2(data7[19]),
        .I3(s_axi_CTRL_ARADDR[3]),
        .O(\s_axi_CTRL_ARADDR[3]_17 ));
  LUT6 #(
    .INIT(64'h1000FFFF10000000)) 
    \rdata[1]_i_1 
       (.I0(s_axi_CTRL_ARADDR[1]),
        .I1(s_axi_CTRL_ARADDR[2]),
        .I2(data7[1]),
        .I3(s_axi_CTRL_ARADDR[3]),
        .I4(s_axi_CTRL_ARADDR[0]),
        .I5(\rdata_reg[1] ),
        .O(s_axi_CTRL_ARADDR_3_sn_1));
  LUT4 #(
    .INIT(16'h1000)) 
    \rdata[20]_i_3 
       (.I0(s_axi_CTRL_ARADDR[1]),
        .I1(s_axi_CTRL_ARADDR[2]),
        .I2(data7[20]),
        .I3(s_axi_CTRL_ARADDR[3]),
        .O(\s_axi_CTRL_ARADDR[3]_18 ));
  LUT4 #(
    .INIT(16'h1000)) 
    \rdata[21]_i_3 
       (.I0(s_axi_CTRL_ARADDR[1]),
        .I1(s_axi_CTRL_ARADDR[2]),
        .I2(data7[21]),
        .I3(s_axi_CTRL_ARADDR[3]),
        .O(\s_axi_CTRL_ARADDR[3]_19 ));
  LUT4 #(
    .INIT(16'h1000)) 
    \rdata[22]_i_3 
       (.I0(s_axi_CTRL_ARADDR[1]),
        .I1(s_axi_CTRL_ARADDR[2]),
        .I2(data7[22]),
        .I3(s_axi_CTRL_ARADDR[3]),
        .O(\s_axi_CTRL_ARADDR[3]_20 ));
  LUT4 #(
    .INIT(16'h1000)) 
    \rdata[23]_i_3 
       (.I0(s_axi_CTRL_ARADDR[1]),
        .I1(s_axi_CTRL_ARADDR[2]),
        .I2(data7[23]),
        .I3(s_axi_CTRL_ARADDR[3]),
        .O(\s_axi_CTRL_ARADDR[3]_21 ));
  LUT6 #(
    .INIT(64'h1000FFFF10000000)) 
    \rdata[2]_i_1 
       (.I0(s_axi_CTRL_ARADDR[1]),
        .I1(s_axi_CTRL_ARADDR[2]),
        .I2(data7[2]),
        .I3(s_axi_CTRL_ARADDR[3]),
        .I4(s_axi_CTRL_ARADDR[0]),
        .I5(\rdata_reg[2] ),
        .O(\s_axi_CTRL_ARADDR[3]_0 ));
  LUT6 #(
    .INIT(64'h1000FFFF10000000)) 
    \rdata[3]_i_1 
       (.I0(s_axi_CTRL_ARADDR[1]),
        .I1(s_axi_CTRL_ARADDR[2]),
        .I2(data7[3]),
        .I3(s_axi_CTRL_ARADDR[3]),
        .I4(s_axi_CTRL_ARADDR[0]),
        .I5(\rdata_reg[3] ),
        .O(\s_axi_CTRL_ARADDR[3]_1 ));
  LUT6 #(
    .INIT(64'h1000FFFF10000000)) 
    \rdata[4]_i_1 
       (.I0(s_axi_CTRL_ARADDR[1]),
        .I1(s_axi_CTRL_ARADDR[2]),
        .I2(data7[4]),
        .I3(s_axi_CTRL_ARADDR[3]),
        .I4(s_axi_CTRL_ARADDR[0]),
        .I5(\rdata_reg[4] ),
        .O(\s_axi_CTRL_ARADDR[3]_2 ));
  LUT6 #(
    .INIT(64'h1000FFFF10000000)) 
    \rdata[5]_i_1 
       (.I0(s_axi_CTRL_ARADDR[1]),
        .I1(s_axi_CTRL_ARADDR[2]),
        .I2(data7[5]),
        .I3(s_axi_CTRL_ARADDR[3]),
        .I4(s_axi_CTRL_ARADDR[0]),
        .I5(\rdata_reg[5] ),
        .O(\s_axi_CTRL_ARADDR[3]_3 ));
  LUT6 #(
    .INIT(64'h1000FFFF10000000)) 
    \rdata[6]_i_1 
       (.I0(s_axi_CTRL_ARADDR[1]),
        .I1(s_axi_CTRL_ARADDR[2]),
        .I2(data7[6]),
        .I3(s_axi_CTRL_ARADDR[3]),
        .I4(s_axi_CTRL_ARADDR[0]),
        .I5(\rdata_reg[6] ),
        .O(\s_axi_CTRL_ARADDR[3]_4 ));
  LUT6 #(
    .INIT(64'h1000FFFF10000000)) 
    \rdata[7]_i_1 
       (.I0(s_axi_CTRL_ARADDR[1]),
        .I1(s_axi_CTRL_ARADDR[2]),
        .I2(data7[7]),
        .I3(s_axi_CTRL_ARADDR[3]),
        .I4(s_axi_CTRL_ARADDR[0]),
        .I5(\rdata_reg[7]_0 ),
        .O(\s_axi_CTRL_ARADDR[3]_5 ));
  LUT6 #(
    .INIT(64'h1000FFFF10000000)) 
    \rdata[8]_i_1 
       (.I0(s_axi_CTRL_ARADDR[1]),
        .I1(s_axi_CTRL_ARADDR[2]),
        .I2(data7[8]),
        .I3(s_axi_CTRL_ARADDR[3]),
        .I4(s_axi_CTRL_ARADDR[0]),
        .I5(\rdata_reg[8] ),
        .O(\s_axi_CTRL_ARADDR[3]_6 ));
  LUT6 #(
    .INIT(64'h1000FFFF10000000)) 
    \rdata[9]_i_1 
       (.I0(s_axi_CTRL_ARADDR[1]),
        .I1(s_axi_CTRL_ARADDR[2]),
        .I2(data7[9]),
        .I3(s_axi_CTRL_ARADDR[3]),
        .I4(s_axi_CTRL_ARADDR[0]),
        .I5(\rdata_reg[9] ),
        .O(\s_axi_CTRL_ARADDR[3]_7 ));
  (* KEEP_HIERARCHY = "yes" *) 
  (* METHODOLOGY_DRC_VIOS = "{SYNTH-10 {cell *THIS*} {string 16x18 4}}" *) 
  DSP48E2 #(
    .ACASCREG(1),
    .ADREG(1),
    .ALUMODEREG(0),
    .AMULTSEL("A"),
    .AREG(1),
    .AUTORESET_PATDET("NO_RESET"),
    .AUTORESET_PRIORITY("RESET"),
    .A_INPUT("DIRECT"),
    .BCASCREG(1),
    .BMULTSEL("B"),
    .BREG(1),
    .B_INPUT("DIRECT"),
    .CARRYINREG(0),
    .CARRYINSELREG(0),
    .CREG(1),
    .DREG(1),
    .INMODEREG(0),
    .MASK(48'h3FFFFFFFFFFF),
    .MREG(0),
    .OPMODEREG(0),
    .PATTERN(48'h000000000000),
    .PREADDINSEL("A"),
    .PREG(0),
    .RND(48'h000000000000),
    .SEL_MASK("MASK"),
    .SEL_PATTERN("PATTERN"),
    .USE_MULT("MULTIPLY"),
    .USE_PATTERN_DETECT("NO_PATDET"),
    .USE_SIMD("ONE48"),
    .USE_WIDEXOR("FALSE"),
    .XORSIMD("XOR24_48_96")) 
    tmp_product
       (.A({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,D}),
        .ACIN({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .ACOUT(NLW_tmp_product_ACOUT_UNCONNECTED[29:0]),
        .ALUMODE({1'b0,1'b0,1'b0,1'b0}),
        .B({1'b0,1'b0,1'b0,DSP_ALU_INST[31:17]}),
        .BCIN({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .BCOUT(NLW_tmp_product_BCOUT_UNCONNECTED[17:0]),
        .C({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .CARRYCASCIN(1'b0),
        .CARRYCASCOUT(NLW_tmp_product_CARRYCASCOUT_UNCONNECTED),
        .CARRYIN(1'b0),
        .CARRYINSEL({1'b0,1'b0,1'b0}),
        .CARRYOUT(NLW_tmp_product_CARRYOUT_UNCONNECTED[3:0]),
        .CEA1(1'b0),
        .CEA2(CEB2),
        .CEAD(1'b0),
        .CEALUMODE(1'b0),
        .CEB1(1'b0),
        .CEB2(E),
        .CEC(1'b0),
        .CECARRYIN(1'b0),
        .CECTRL(1'b0),
        .CED(1'b0),
        .CEINMODE(1'b0),
        .CEM(1'b0),
        .CEP(1'b0),
        .CLK(ap_clk),
        .D({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .INMODE({1'b0,1'b0,1'b0,1'b0,1'b0}),
        .MULTSIGNIN(1'b0),
        .MULTSIGNOUT(NLW_tmp_product_MULTSIGNOUT_UNCONNECTED),
        .OPMODE({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b1,1'b0,1'b1}),
        .OVERFLOW(NLW_tmp_product_OVERFLOW_UNCONNECTED),
        .P({tmp_product_n_58,tmp_product_n_59,tmp_product_n_60,tmp_product_n_61,tmp_product_n_62,tmp_product_n_63,tmp_product_n_64,tmp_product_n_65,tmp_product_n_66,tmp_product_n_67,tmp_product_n_68,tmp_product_n_69,tmp_product_n_70,tmp_product_n_71,tmp_product_n_72,tmp_product_n_73,tmp_product_n_74,tmp_product_n_75,tmp_product_n_76,tmp_product_n_77,tmp_product_n_78,tmp_product_n_79,tmp_product_n_80,tmp_product_n_81,tmp_product_n_82,tmp_product_n_83,tmp_product_n_84,tmp_product_n_85,tmp_product_n_86,tmp_product_n_87,tmp_product_n_88,P}),
        .PATTERNBDETECT(NLW_tmp_product_PATTERNBDETECT_UNCONNECTED),
        .PATTERNDETECT(NLW_tmp_product_PATTERNDETECT_UNCONNECTED),
        .PCIN({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .PCOUT(PCOUT),
        .RSTA(RSTB),
        .RSTALLCARRYIN(1'b0),
        .RSTALUMODE(1'b0),
        .RSTB(RSTB),
        .RSTC(1'b0),
        .RSTCTRL(1'b0),
        .RSTD(1'b0),
        .RSTINMODE(1'b0),
        .RSTM(1'b0),
        .RSTP(1'b0),
        .UNDERFLOW(NLW_tmp_product_UNDERFLOW_UNCONNECTED),
        .XOROUT(NLW_tmp_product_XOROUT_UNCONNECTED[7:0]));
  (* KEEP_HIERARCHY = "yes" *) 
  (* METHODOLOGY_DRC_VIOS = "{SYNTH-10 {cell *THIS*} {string 18x18 4}}" *) 
  DSP48E2 #(
    .ACASCREG(1),
    .ADREG(1),
    .ALUMODEREG(0),
    .AMULTSEL("A"),
    .AREG(1),
    .AUTORESET_PATDET("NO_RESET"),
    .AUTORESET_PRIORITY("RESET"),
    .A_INPUT("DIRECT"),
    .BCASCREG(1),
    .BMULTSEL("B"),
    .BREG(1),
    .B_INPUT("DIRECT"),
    .CARRYINREG(0),
    .CARRYINSELREG(0),
    .CREG(1),
    .DREG(1),
    .INMODEREG(0),
    .MASK(48'h3FFFFFFFFFFF),
    .MREG(0),
    .OPMODEREG(0),
    .PATTERN(48'h000000000000),
    .PREADDINSEL("A"),
    .PREG(0),
    .RND(48'h000000000000),
    .SEL_MASK("MASK"),
    .SEL_PATTERN("PATTERN"),
    .USE_MULT("MULTIPLY"),
    .USE_PATTERN_DETECT("NO_PATDET"),
    .USE_SIMD("ONE48"),
    .USE_WIDEXOR("FALSE"),
    .XORSIMD("XOR24_48_96")) 
    tmp_product__0
       (.A({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,DSP_ALU_INST[16:0]}),
        .ACIN({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .ACOUT(NLW_tmp_product__0_ACOUT_UNCONNECTED[29:0]),
        .ALUMODE({1'b0,1'b0,1'b0,1'b0}),
        .B({1'b0,D}),
        .BCIN({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .BCOUT(NLW_tmp_product__0_BCOUT_UNCONNECTED[17:0]),
        .C({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .CARRYCASCIN(1'b0),
        .CARRYCASCOUT(NLW_tmp_product__0_CARRYCASCOUT_UNCONNECTED),
        .CARRYIN(1'b0),
        .CARRYINSEL({1'b0,1'b0,1'b0}),
        .CARRYOUT(NLW_tmp_product__0_CARRYOUT_UNCONNECTED[3:0]),
        .CEA1(1'b0),
        .CEA2(E),
        .CEAD(1'b0),
        .CEALUMODE(1'b0),
        .CEB1(1'b0),
        .CEB2(CEB2),
        .CEC(1'b0),
        .CECARRYIN(1'b0),
        .CECTRL(1'b0),
        .CED(1'b0),
        .CEINMODE(1'b0),
        .CEM(1'b0),
        .CEP(1'b0),
        .CLK(ap_clk),
        .D({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .INMODE({1'b0,1'b0,1'b0,1'b0,1'b0}),
        .MULTSIGNIN(1'b0),
        .MULTSIGNOUT(NLW_tmp_product__0_MULTSIGNOUT_UNCONNECTED),
        .OPMODE({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b1,1'b0,1'b1}),
        .OVERFLOW(NLW_tmp_product__0_OVERFLOW_UNCONNECTED),
        .P({tmp_product__0_n_58,tmp_product__0_n_59,tmp_product__0_n_60,tmp_product__0_n_61,tmp_product__0_n_62,tmp_product__0_n_63,tmp_product__0_n_64,tmp_product__0_n_65,tmp_product__0_n_66,tmp_product__0_n_67,tmp_product__0_n_68,tmp_product__0_n_69,tmp_product__0_n_70,tmp_product__0_n_71,tmp_product__0_n_72,tmp_product__0_n_73,tmp_product__0_n_74,tmp_product__0_n_75,tmp_product__0_n_76,tmp_product__0_n_77,tmp_product__0_n_78,tmp_product__0_n_79,tmp_product__0_n_80,tmp_product__0_n_81,tmp_product__0_n_82,tmp_product__0_n_83,tmp_product__0_n_84,tmp_product__0_n_85,tmp_product__0_n_86,tmp_product__0_n_87,tmp_product__0_n_88,ap_clk_0}),
        .PATTERNBDETECT(NLW_tmp_product__0_PATTERNBDETECT_UNCONNECTED),
        .PATTERNDETECT(NLW_tmp_product__0_PATTERNDETECT_UNCONNECTED),
        .PCIN({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .PCOUT(ap_clk_1),
        .RSTA(RSTB),
        .RSTALLCARRYIN(1'b0),
        .RSTALUMODE(1'b0),
        .RSTB(RSTB),
        .RSTC(1'b0),
        .RSTCTRL(1'b0),
        .RSTD(1'b0),
        .RSTINMODE(1'b0),
        .RSTM(1'b0),
        .RSTP(1'b0),
        .UNDERFLOW(NLW_tmp_product__0_UNDERFLOW_UNCONNECTED),
        .XOROUT(NLW_tmp_product__0_XOROUT_UNCONNECTED[7:0]));
  (* ADDER_THRESHOLD = "35" *) 
  CARRY8 tmp_product_carry
       (.CI(1'b0),
        .CI_TOP(1'b0),
        .CO({tmp_product_carry_n_0,tmp_product_carry_n_1,tmp_product_carry_n_2,tmp_product_carry_n_3,tmp_product_carry_n_4,tmp_product_carry_n_5,tmp_product_carry_n_6,tmp_product_carry_n_7}),
        .DI({\rdata[24]_i_3 [6:0],1'b0}),
        .O(O),
        .S(\rdata[16]_i_2 ));
  (* ADDER_THRESHOLD = "35" *) 
  CARRY8 tmp_product_carry__0
       (.CI(tmp_product_carry_n_0),
        .CI_TOP(1'b0),
        .CO({tmp_product_carry__0_n_0,tmp_product_carry__0_n_1,tmp_product_carry__0_n_2,tmp_product_carry__0_n_3,tmp_product_carry__0_n_4,tmp_product_carry__0_n_5,tmp_product_carry__0_n_6,tmp_product_carry__0_n_7}),
        .DI(\rdata[24]_i_3 [14:7]),
        .O(\int_p_reg[16]__0 ),
        .S(\rdata[24]_i_2 ));
  (* ADDER_THRESHOLD = "35" *) 
  CARRY8 tmp_product_carry__1
       (.CI(tmp_product_carry__0_n_0),
        .CI_TOP(1'b0),
        .CO({tmp_product_carry__1_n_0,tmp_product_carry__1_n_1,tmp_product_carry__1_n_2,tmp_product_carry__1_n_3,tmp_product_carry__1_n_4,tmp_product_carry__1_n_5,tmp_product_carry__1_n_6,tmp_product_carry__1_n_7}),
        .DI(\rdata[24]_i_3 [22:15]),
        .O({data7[7:1],int_ap_start_reg[0]}),
        .S(\rdata_reg[7] ));
  (* ADDER_THRESHOLD = "35" *) 
  CARRY8 tmp_product_carry__2
       (.CI(tmp_product_carry__1_n_0),
        .CI_TOP(1'b0),
        .CO({tmp_product_carry__2_n_0,tmp_product_carry__2_n_1,tmp_product_carry__2_n_2,tmp_product_carry__2_n_3,tmp_product_carry__2_n_4,tmp_product_carry__2_n_5,tmp_product_carry__2_n_6,tmp_product_carry__2_n_7}),
        .DI(\rdata[24]_i_3 [30:23]),
        .O(data7[15:8]),
        .S(\rdata_reg[15] ));
  (* ADDER_THRESHOLD = "35" *) 
  CARRY8 tmp_product_carry__3
       (.CI(tmp_product_carry__2_n_0),
        .CI_TOP(1'b0),
        .CO({tmp_product_carry__3_n_0,tmp_product_carry__3_n_1,tmp_product_carry__3_n_2,tmp_product_carry__3_n_3,tmp_product_carry__3_n_4,tmp_product_carry__3_n_5,tmp_product_carry__3_n_6,tmp_product_carry__3_n_7}),
        .DI(\rdata[24]_i_3 [38:31]),
        .O(data7[23:16]),
        .S(\rdata[16]_i_3_0 ));
  (* ADDER_THRESHOLD = "35" *) 
  CARRY8 tmp_product_carry__4
       (.CI(tmp_product_carry__3_n_0),
        .CI_TOP(1'b0),
        .CO({NLW_tmp_product_carry__4_CO_UNCONNECTED[7],tmp_product_carry__4_n_1,tmp_product_carry__4_n_2,tmp_product_carry__4_n_3,tmp_product_carry__4_n_4,tmp_product_carry__4_n_5,tmp_product_carry__4_n_6,tmp_product_carry__4_n_7}),
        .DI({1'b0,\rdata[24]_i_3 [45:39]}),
        .O(int_ap_start_reg[8:1]),
        .S(S));
endmodule
`ifndef GLBL
`define GLBL
`timescale  1 ps / 1 ps

module glbl ();

    parameter ROC_WIDTH = 100000;
    parameter TOC_WIDTH = 0;
    parameter GRES_WIDTH = 10000;
    parameter GRES_START = 10000;

//--------   STARTUP Globals --------------
    wire GSR;
    wire GTS;
    wire GWE;
    wire PRLD;
    wire GRESTORE;
    tri1 p_up_tmp;
    tri (weak1, strong0) PLL_LOCKG = p_up_tmp;

    wire PROGB_GLBL;
    wire CCLKO_GLBL;
    wire FCSBO_GLBL;
    wire [3:0] DO_GLBL;
    wire [3:0] DI_GLBL;
   
    reg GSR_int;
    reg GTS_int;
    reg PRLD_int;
    reg GRESTORE_int;

//--------   JTAG Globals --------------
    wire JTAG_TDO_GLBL;
    wire JTAG_TCK_GLBL;
    wire JTAG_TDI_GLBL;
    wire JTAG_TMS_GLBL;
    wire JTAG_TRST_GLBL;

    reg JTAG_CAPTURE_GLBL;
    reg JTAG_RESET_GLBL;
    reg JTAG_SHIFT_GLBL;
    reg JTAG_UPDATE_GLBL;
    reg JTAG_RUNTEST_GLBL;

    reg JTAG_SEL1_GLBL = 0;
    reg JTAG_SEL2_GLBL = 0 ;
    reg JTAG_SEL3_GLBL = 0;
    reg JTAG_SEL4_GLBL = 0;

    reg JTAG_USER_TDO1_GLBL = 1'bz;
    reg JTAG_USER_TDO2_GLBL = 1'bz;
    reg JTAG_USER_TDO3_GLBL = 1'bz;
    reg JTAG_USER_TDO4_GLBL = 1'bz;

    assign (strong1, weak0) GSR = GSR_int;
    assign (strong1, weak0) GTS = GTS_int;
    assign (weak1, weak0) PRLD = PRLD_int;
    assign (strong1, weak0) GRESTORE = GRESTORE_int;

    initial begin
	GSR_int = 1'b1;
	PRLD_int = 1'b1;
	#(ROC_WIDTH)
	GSR_int = 1'b0;
	PRLD_int = 1'b0;
    end

    initial begin
	GTS_int = 1'b1;
	#(TOC_WIDTH)
	GTS_int = 1'b0;
    end

    initial begin 
	GRESTORE_int = 1'b0;
	#(GRES_START);
	GRESTORE_int = 1'b1;
	#(GRES_WIDTH);
	GRESTORE_int = 1'b0;
    end

endmodule
`endif
