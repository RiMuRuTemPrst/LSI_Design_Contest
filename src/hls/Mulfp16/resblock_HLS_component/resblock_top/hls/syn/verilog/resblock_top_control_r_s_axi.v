// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2024.2 (64-bit)
// Tool Version Limit: 2024.11
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
`timescale 1ns/1ps
module resblock_top_control_r_s_axi
#(parameter
    C_S_AXI_ADDR_WIDTH = 8,
    C_S_AXI_DATA_WIDTH = 32
)(
    input  wire                          ACLK,
    input  wire                          ARESET,
    input  wire                          ACLK_EN,
    input  wire [C_S_AXI_ADDR_WIDTH-1:0] AWADDR,
    input  wire                          AWVALID,
    output wire                          AWREADY,
    input  wire [C_S_AXI_DATA_WIDTH-1:0] WDATA,
    input  wire [C_S_AXI_DATA_WIDTH/8-1:0] WSTRB,
    input  wire                          WVALID,
    output wire                          WREADY,
    output wire [1:0]                    BRESP,
    output wire                          BVALID,
    input  wire                          BREADY,
    input  wire [C_S_AXI_ADDR_WIDTH-1:0] ARADDR,
    input  wire                          ARVALID,
    output wire                          ARREADY,
    output wire [C_S_AXI_DATA_WIDTH-1:0] RDATA,
    output wire [1:0]                    RRESP,
    output wire                          RVALID,
    input  wire                          RREADY,
    output wire [63:0]                   X,
    output wire [63:0]                   W1,
    output wire [63:0]                   B1,
    output wire [63:0]                   G1,
    output wire [63:0]                   BE1,
    output wire [63:0]                   W2,
    output wire [63:0]                   B2,
    output wire [63:0]                   G2,
    output wire [63:0]                   BE2,
    output wire [63:0]                   Y
);
//------------------------Address Info-------------------
// Protocol Used: ap_ctrl_none
//
// 0x00 : reserved
// 0x04 : reserved
// 0x08 : reserved
// 0x0c : reserved
// 0x10 : Data signal of X
//        bit 31~0 - X[31:0] (Read/Write)
// 0x14 : Data signal of X
//        bit 31~0 - X[63:32] (Read/Write)
// 0x18 : reserved
// 0x1c : Data signal of W1
//        bit 31~0 - W1[31:0] (Read/Write)
// 0x20 : Data signal of W1
//        bit 31~0 - W1[63:32] (Read/Write)
// 0x24 : reserved
// 0x28 : Data signal of B1
//        bit 31~0 - B1[31:0] (Read/Write)
// 0x2c : Data signal of B1
//        bit 31~0 - B1[63:32] (Read/Write)
// 0x30 : reserved
// 0x34 : Data signal of G1
//        bit 31~0 - G1[31:0] (Read/Write)
// 0x38 : Data signal of G1
//        bit 31~0 - G1[63:32] (Read/Write)
// 0x3c : reserved
// 0x40 : Data signal of BE1
//        bit 31~0 - BE1[31:0] (Read/Write)
// 0x44 : Data signal of BE1
//        bit 31~0 - BE1[63:32] (Read/Write)
// 0x48 : reserved
// 0x4c : Data signal of W2
//        bit 31~0 - W2[31:0] (Read/Write)
// 0x50 : Data signal of W2
//        bit 31~0 - W2[63:32] (Read/Write)
// 0x54 : reserved
// 0x58 : Data signal of B2
//        bit 31~0 - B2[31:0] (Read/Write)
// 0x5c : Data signal of B2
//        bit 31~0 - B2[63:32] (Read/Write)
// 0x60 : reserved
// 0x64 : Data signal of G2
//        bit 31~0 - G2[31:0] (Read/Write)
// 0x68 : Data signal of G2
//        bit 31~0 - G2[63:32] (Read/Write)
// 0x6c : reserved
// 0x70 : Data signal of BE2
//        bit 31~0 - BE2[31:0] (Read/Write)
// 0x74 : Data signal of BE2
//        bit 31~0 - BE2[63:32] (Read/Write)
// 0x78 : reserved
// 0x7c : Data signal of Y
//        bit 31~0 - Y[31:0] (Read/Write)
// 0x80 : Data signal of Y
//        bit 31~0 - Y[63:32] (Read/Write)
// 0x84 : reserved
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

//------------------------Parameter----------------------
localparam
    ADDR_X_DATA_0   = 8'h10,
    ADDR_X_DATA_1   = 8'h14,
    ADDR_X_CTRL     = 8'h18,
    ADDR_W1_DATA_0  = 8'h1c,
    ADDR_W1_DATA_1  = 8'h20,
    ADDR_W1_CTRL    = 8'h24,
    ADDR_B1_DATA_0  = 8'h28,
    ADDR_B1_DATA_1  = 8'h2c,
    ADDR_B1_CTRL    = 8'h30,
    ADDR_G1_DATA_0  = 8'h34,
    ADDR_G1_DATA_1  = 8'h38,
    ADDR_G1_CTRL    = 8'h3c,
    ADDR_BE1_DATA_0 = 8'h40,
    ADDR_BE1_DATA_1 = 8'h44,
    ADDR_BE1_CTRL   = 8'h48,
    ADDR_W2_DATA_0  = 8'h4c,
    ADDR_W2_DATA_1  = 8'h50,
    ADDR_W2_CTRL    = 8'h54,
    ADDR_B2_DATA_0  = 8'h58,
    ADDR_B2_DATA_1  = 8'h5c,
    ADDR_B2_CTRL    = 8'h60,
    ADDR_G2_DATA_0  = 8'h64,
    ADDR_G2_DATA_1  = 8'h68,
    ADDR_G2_CTRL    = 8'h6c,
    ADDR_BE2_DATA_0 = 8'h70,
    ADDR_BE2_DATA_1 = 8'h74,
    ADDR_BE2_CTRL   = 8'h78,
    ADDR_Y_DATA_0   = 8'h7c,
    ADDR_Y_DATA_1   = 8'h80,
    ADDR_Y_CTRL     = 8'h84,
    WRIDLE          = 2'd0,
    WRDATA          = 2'd1,
    WRRESP          = 2'd2,
    WRRESET         = 2'd3,
    RDIDLE          = 2'd0,
    RDDATA          = 2'd1,
    RDRESET         = 2'd2,
    ADDR_BITS                = 8;

//------------------------Local signal-------------------
    reg  [1:0]                    wstate = WRRESET;
    reg  [1:0]                    wnext;
    reg  [ADDR_BITS-1:0]          waddr;
    wire [C_S_AXI_DATA_WIDTH-1:0] wmask;
    wire                          aw_hs;
    wire                          w_hs;
    reg  [1:0]                    rstate = RDRESET;
    reg  [1:0]                    rnext;
    reg  [C_S_AXI_DATA_WIDTH-1:0] rdata;
    wire                          ar_hs;
    wire [ADDR_BITS-1:0]          raddr;
    // internal registers
    reg  [63:0]                   int_X = 'b0;
    reg  [63:0]                   int_W1 = 'b0;
    reg  [63:0]                   int_B1 = 'b0;
    reg  [63:0]                   int_G1 = 'b0;
    reg  [63:0]                   int_BE1 = 'b0;
    reg  [63:0]                   int_W2 = 'b0;
    reg  [63:0]                   int_B2 = 'b0;
    reg  [63:0]                   int_G2 = 'b0;
    reg  [63:0]                   int_BE2 = 'b0;
    reg  [63:0]                   int_Y = 'b0;

//------------------------Instantiation------------------


//------------------------AXI write fsm------------------
assign AWREADY = (wstate == WRIDLE);
assign WREADY  = (wstate == WRDATA);
assign BRESP   = 2'b00;  // OKAY
assign BVALID  = (wstate == WRRESP);
assign wmask   = { {8{WSTRB[3]}}, {8{WSTRB[2]}}, {8{WSTRB[1]}}, {8{WSTRB[0]}} };
assign aw_hs   = AWVALID & AWREADY;
assign w_hs    = WVALID & WREADY;

// wstate
always @(posedge ACLK) begin
    if (ARESET)
        wstate <= WRRESET;
    else if (ACLK_EN)
        wstate <= wnext;
end

// wnext
always @(*) begin
    case (wstate)
        WRIDLE:
            if (AWVALID)
                wnext = WRDATA;
            else
                wnext = WRIDLE;
        WRDATA:
            if (WVALID)
                wnext = WRRESP;
            else
                wnext = WRDATA;
        WRRESP:
            if (BREADY)
                wnext = WRIDLE;
            else
                wnext = WRRESP;
        default:
            wnext = WRIDLE;
    endcase
end

// waddr
always @(posedge ACLK) begin
    if (ACLK_EN) begin
        if (aw_hs)
            waddr <= {AWADDR[ADDR_BITS-1:2], {2{1'b0}}};
    end
end

//------------------------AXI read fsm-------------------
assign ARREADY = (rstate == RDIDLE);
assign RDATA   = rdata;
assign RRESP   = 2'b00;  // OKAY
assign RVALID  = (rstate == RDDATA);
assign ar_hs   = ARVALID & ARREADY;
assign raddr   = ARADDR[ADDR_BITS-1:0];

// rstate
always @(posedge ACLK) begin
    if (ARESET)
        rstate <= RDRESET;
    else if (ACLK_EN)
        rstate <= rnext;
end

// rnext
always @(*) begin
    case (rstate)
        RDIDLE:
            if (ARVALID)
                rnext = RDDATA;
            else
                rnext = RDIDLE;
        RDDATA:
            if (RREADY & RVALID)
                rnext = RDIDLE;
            else
                rnext = RDDATA;
        default:
            rnext = RDIDLE;
    endcase
end

// rdata
always @(posedge ACLK) begin
    if (ACLK_EN) begin
        if (ar_hs) begin
            rdata <= 'b0;
            case (raddr)
                ADDR_X_DATA_0: begin
                    rdata <= int_X[31:0];
                end
                ADDR_X_DATA_1: begin
                    rdata <= int_X[63:32];
                end
                ADDR_W1_DATA_0: begin
                    rdata <= int_W1[31:0];
                end
                ADDR_W1_DATA_1: begin
                    rdata <= int_W1[63:32];
                end
                ADDR_B1_DATA_0: begin
                    rdata <= int_B1[31:0];
                end
                ADDR_B1_DATA_1: begin
                    rdata <= int_B1[63:32];
                end
                ADDR_G1_DATA_0: begin
                    rdata <= int_G1[31:0];
                end
                ADDR_G1_DATA_1: begin
                    rdata <= int_G1[63:32];
                end
                ADDR_BE1_DATA_0: begin
                    rdata <= int_BE1[31:0];
                end
                ADDR_BE1_DATA_1: begin
                    rdata <= int_BE1[63:32];
                end
                ADDR_W2_DATA_0: begin
                    rdata <= int_W2[31:0];
                end
                ADDR_W2_DATA_1: begin
                    rdata <= int_W2[63:32];
                end
                ADDR_B2_DATA_0: begin
                    rdata <= int_B2[31:0];
                end
                ADDR_B2_DATA_1: begin
                    rdata <= int_B2[63:32];
                end
                ADDR_G2_DATA_0: begin
                    rdata <= int_G2[31:0];
                end
                ADDR_G2_DATA_1: begin
                    rdata <= int_G2[63:32];
                end
                ADDR_BE2_DATA_0: begin
                    rdata <= int_BE2[31:0];
                end
                ADDR_BE2_DATA_1: begin
                    rdata <= int_BE2[63:32];
                end
                ADDR_Y_DATA_0: begin
                    rdata <= int_Y[31:0];
                end
                ADDR_Y_DATA_1: begin
                    rdata <= int_Y[63:32];
                end
            endcase
        end
    end
end


//------------------------Register logic-----------------
assign X   = int_X;
assign W1  = int_W1;
assign B1  = int_B1;
assign G1  = int_G1;
assign BE1 = int_BE1;
assign W2  = int_W2;
assign B2  = int_B2;
assign G2  = int_G2;
assign BE2 = int_BE2;
assign Y   = int_Y;
// int_X[31:0]
always @(posedge ACLK) begin
    if (ARESET)
        int_X[31:0] <= 0;
    else if (ACLK_EN) begin
        if (w_hs && waddr == ADDR_X_DATA_0)
            int_X[31:0] <= (WDATA[31:0] & wmask) | (int_X[31:0] & ~wmask);
    end
end

// int_X[63:32]
always @(posedge ACLK) begin
    if (ARESET)
        int_X[63:32] <= 0;
    else if (ACLK_EN) begin
        if (w_hs && waddr == ADDR_X_DATA_1)
            int_X[63:32] <= (WDATA[31:0] & wmask) | (int_X[63:32] & ~wmask);
    end
end

// int_W1[31:0]
always @(posedge ACLK) begin
    if (ARESET)
        int_W1[31:0] <= 0;
    else if (ACLK_EN) begin
        if (w_hs && waddr == ADDR_W1_DATA_0)
            int_W1[31:0] <= (WDATA[31:0] & wmask) | (int_W1[31:0] & ~wmask);
    end
end

// int_W1[63:32]
always @(posedge ACLK) begin
    if (ARESET)
        int_W1[63:32] <= 0;
    else if (ACLK_EN) begin
        if (w_hs && waddr == ADDR_W1_DATA_1)
            int_W1[63:32] <= (WDATA[31:0] & wmask) | (int_W1[63:32] & ~wmask);
    end
end

// int_B1[31:0]
always @(posedge ACLK) begin
    if (ARESET)
        int_B1[31:0] <= 0;
    else if (ACLK_EN) begin
        if (w_hs && waddr == ADDR_B1_DATA_0)
            int_B1[31:0] <= (WDATA[31:0] & wmask) | (int_B1[31:0] & ~wmask);
    end
end

// int_B1[63:32]
always @(posedge ACLK) begin
    if (ARESET)
        int_B1[63:32] <= 0;
    else if (ACLK_EN) begin
        if (w_hs && waddr == ADDR_B1_DATA_1)
            int_B1[63:32] <= (WDATA[31:0] & wmask) | (int_B1[63:32] & ~wmask);
    end
end

// int_G1[31:0]
always @(posedge ACLK) begin
    if (ARESET)
        int_G1[31:0] <= 0;
    else if (ACLK_EN) begin
        if (w_hs && waddr == ADDR_G1_DATA_0)
            int_G1[31:0] <= (WDATA[31:0] & wmask) | (int_G1[31:0] & ~wmask);
    end
end

// int_G1[63:32]
always @(posedge ACLK) begin
    if (ARESET)
        int_G1[63:32] <= 0;
    else if (ACLK_EN) begin
        if (w_hs && waddr == ADDR_G1_DATA_1)
            int_G1[63:32] <= (WDATA[31:0] & wmask) | (int_G1[63:32] & ~wmask);
    end
end

// int_BE1[31:0]
always @(posedge ACLK) begin
    if (ARESET)
        int_BE1[31:0] <= 0;
    else if (ACLK_EN) begin
        if (w_hs && waddr == ADDR_BE1_DATA_0)
            int_BE1[31:0] <= (WDATA[31:0] & wmask) | (int_BE1[31:0] & ~wmask);
    end
end

// int_BE1[63:32]
always @(posedge ACLK) begin
    if (ARESET)
        int_BE1[63:32] <= 0;
    else if (ACLK_EN) begin
        if (w_hs && waddr == ADDR_BE1_DATA_1)
            int_BE1[63:32] <= (WDATA[31:0] & wmask) | (int_BE1[63:32] & ~wmask);
    end
end

// int_W2[31:0]
always @(posedge ACLK) begin
    if (ARESET)
        int_W2[31:0] <= 0;
    else if (ACLK_EN) begin
        if (w_hs && waddr == ADDR_W2_DATA_0)
            int_W2[31:0] <= (WDATA[31:0] & wmask) | (int_W2[31:0] & ~wmask);
    end
end

// int_W2[63:32]
always @(posedge ACLK) begin
    if (ARESET)
        int_W2[63:32] <= 0;
    else if (ACLK_EN) begin
        if (w_hs && waddr == ADDR_W2_DATA_1)
            int_W2[63:32] <= (WDATA[31:0] & wmask) | (int_W2[63:32] & ~wmask);
    end
end

// int_B2[31:0]
always @(posedge ACLK) begin
    if (ARESET)
        int_B2[31:0] <= 0;
    else if (ACLK_EN) begin
        if (w_hs && waddr == ADDR_B2_DATA_0)
            int_B2[31:0] <= (WDATA[31:0] & wmask) | (int_B2[31:0] & ~wmask);
    end
end

// int_B2[63:32]
always @(posedge ACLK) begin
    if (ARESET)
        int_B2[63:32] <= 0;
    else if (ACLK_EN) begin
        if (w_hs && waddr == ADDR_B2_DATA_1)
            int_B2[63:32] <= (WDATA[31:0] & wmask) | (int_B2[63:32] & ~wmask);
    end
end

// int_G2[31:0]
always @(posedge ACLK) begin
    if (ARESET)
        int_G2[31:0] <= 0;
    else if (ACLK_EN) begin
        if (w_hs && waddr == ADDR_G2_DATA_0)
            int_G2[31:0] <= (WDATA[31:0] & wmask) | (int_G2[31:0] & ~wmask);
    end
end

// int_G2[63:32]
always @(posedge ACLK) begin
    if (ARESET)
        int_G2[63:32] <= 0;
    else if (ACLK_EN) begin
        if (w_hs && waddr == ADDR_G2_DATA_1)
            int_G2[63:32] <= (WDATA[31:0] & wmask) | (int_G2[63:32] & ~wmask);
    end
end

// int_BE2[31:0]
always @(posedge ACLK) begin
    if (ARESET)
        int_BE2[31:0] <= 0;
    else if (ACLK_EN) begin
        if (w_hs && waddr == ADDR_BE2_DATA_0)
            int_BE2[31:0] <= (WDATA[31:0] & wmask) | (int_BE2[31:0] & ~wmask);
    end
end

// int_BE2[63:32]
always @(posedge ACLK) begin
    if (ARESET)
        int_BE2[63:32] <= 0;
    else if (ACLK_EN) begin
        if (w_hs && waddr == ADDR_BE2_DATA_1)
            int_BE2[63:32] <= (WDATA[31:0] & wmask) | (int_BE2[63:32] & ~wmask);
    end
end

// int_Y[31:0]
always @(posedge ACLK) begin
    if (ARESET)
        int_Y[31:0] <= 0;
    else if (ACLK_EN) begin
        if (w_hs && waddr == ADDR_Y_DATA_0)
            int_Y[31:0] <= (WDATA[31:0] & wmask) | (int_Y[31:0] & ~wmask);
    end
end

// int_Y[63:32]
always @(posedge ACLK) begin
    if (ARESET)
        int_Y[63:32] <= 0;
    else if (ACLK_EN) begin
        if (w_hs && waddr == ADDR_Y_DATA_1)
            int_Y[63:32] <= (WDATA[31:0] & wmask) | (int_Y[63:32] & ~wmask);
    end
end


//------------------------Memory logic-------------------

endmodule
