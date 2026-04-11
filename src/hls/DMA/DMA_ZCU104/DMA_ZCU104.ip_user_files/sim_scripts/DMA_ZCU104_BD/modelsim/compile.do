vlib modelsim_lib/work
vlib modelsim_lib/msim

vlib modelsim_lib/msim/xilinx_vip
vlib modelsim_lib/msim/xpm
vlib modelsim_lib/msim/axi_infrastructure_v1_1_0
vlib modelsim_lib/msim/axi_vip_v1_1_19
vlib modelsim_lib/msim/zynq_ultra_ps_e_vip_v1_0_19
vlib modelsim_lib/msim/xil_defaultlib
vlib modelsim_lib/msim/axi_bram_ctrl_v4_1_11
vlib modelsim_lib/msim/blk_mem_gen_v8_4_9
vlib modelsim_lib/msim/lib_pkg_v1_0_4
vlib modelsim_lib/msim/lib_srl_fifo_v1_0_4
vlib modelsim_lib/msim/fifo_generator_v13_2_11
vlib modelsim_lib/msim/lib_fifo_v1_0_20
vlib modelsim_lib/msim/lib_cdc_v1_0_3
vlib modelsim_lib/msim/axi_datamover_v5_1_35
vlib modelsim_lib/msim/axi_sg_v4_1_19
vlib modelsim_lib/msim/axi_cdma_v4_1_33
vlib modelsim_lib/msim/xlconstant_v1_1_9
vlib modelsim_lib/msim/proc_sys_reset_v5_0_16
vlib modelsim_lib/msim/smartconnect_v1_0
vlib modelsim_lib/msim/axi_register_slice_v2_1_33

vmap xilinx_vip modelsim_lib/msim/xilinx_vip
vmap xpm modelsim_lib/msim/xpm
vmap axi_infrastructure_v1_1_0 modelsim_lib/msim/axi_infrastructure_v1_1_0
vmap axi_vip_v1_1_19 modelsim_lib/msim/axi_vip_v1_1_19
vmap zynq_ultra_ps_e_vip_v1_0_19 modelsim_lib/msim/zynq_ultra_ps_e_vip_v1_0_19
vmap xil_defaultlib modelsim_lib/msim/xil_defaultlib
vmap axi_bram_ctrl_v4_1_11 modelsim_lib/msim/axi_bram_ctrl_v4_1_11
vmap blk_mem_gen_v8_4_9 modelsim_lib/msim/blk_mem_gen_v8_4_9
vmap lib_pkg_v1_0_4 modelsim_lib/msim/lib_pkg_v1_0_4
vmap lib_srl_fifo_v1_0_4 modelsim_lib/msim/lib_srl_fifo_v1_0_4
vmap fifo_generator_v13_2_11 modelsim_lib/msim/fifo_generator_v13_2_11
vmap lib_fifo_v1_0_20 modelsim_lib/msim/lib_fifo_v1_0_20
vmap lib_cdc_v1_0_3 modelsim_lib/msim/lib_cdc_v1_0_3
vmap axi_datamover_v5_1_35 modelsim_lib/msim/axi_datamover_v5_1_35
vmap axi_sg_v4_1_19 modelsim_lib/msim/axi_sg_v4_1_19
vmap axi_cdma_v4_1_33 modelsim_lib/msim/axi_cdma_v4_1_33
vmap xlconstant_v1_1_9 modelsim_lib/msim/xlconstant_v1_1_9
vmap proc_sys_reset_v5_0_16 modelsim_lib/msim/proc_sys_reset_v5_0_16
vmap smartconnect_v1_0 modelsim_lib/msim/smartconnect_v1_0
vmap axi_register_slice_v2_1_33 modelsim_lib/msim/axi_register_slice_v2_1_33

vlog -work xilinx_vip -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/hdl/axi4stream_vip_axi4streampc.sv" \
"/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/hdl/axi_vip_axi4pc.sv" \
"/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/hdl/xil_common_vip_pkg.sv" \
"/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/hdl/axi4stream_vip_pkg.sv" \
"/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/hdl/axi_vip_pkg.sv" \
"/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/hdl/axi4stream_vip_if.sv" \
"/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/hdl/axi_vip_if.sv" \
"/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/hdl/clk_vip_if.sv" \
"/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/hdl/rst_vip_if.sv" \

vlog -work xpm -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/ec67/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/6f8f/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/f0b6/hdl/verilog" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/0127/hdl/verilog" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"/home/rimurutempest/Tool/Vivado/2024.2/data/ip/xpm/xpm_cdc/hdl/xpm_cdc.sv" \
"/home/rimurutempest/Tool/Vivado/2024.2/data/ip/xpm/xpm_fifo/hdl/xpm_fifo.sv" \
"/home/rimurutempest/Tool/Vivado/2024.2/data/ip/xpm/xpm_memory/hdl/xpm_memory.sv" \

vcom -work xpm -64 -93  \
"/home/rimurutempest/Tool/Vivado/2024.2/data/ip/xpm/xpm_VCOMP.vhd" \

vlog -work axi_infrastructure_v1_1_0 -64 -incr -mfcu  "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/ec67/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/6f8f/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/f0b6/hdl/verilog" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/0127/hdl/verilog" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/ec67/hdl/axi_infrastructure_v1_1_vl_rfs.v" \

vlog -work axi_vip_v1_1_19 -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/ec67/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/6f8f/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/f0b6/hdl/verilog" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/0127/hdl/verilog" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/8c45/hdl/axi_vip_v1_1_vl_rfs.sv" \

vlog -work zynq_ultra_ps_e_vip_v1_0_19 -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/ec67/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/6f8f/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/f0b6/hdl/verilog" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/0127/hdl/verilog" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/6f8f/hdl/zynq_ultra_ps_e_vip_v1_0_vl_rfs.sv" \

vlog -work xil_defaultlib -64 -incr -mfcu  "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/ec67/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/6f8f/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/f0b6/hdl/verilog" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/0127/hdl/verilog" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_zynq_ultra_ps_e_0_0/sim/DMA_ZCU104_BD_zynq_ultra_ps_e_0_0_vip_wrapper.v" \

vcom -work axi_bram_ctrl_v4_1_11 -64 -93  \
"../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/df79/hdl/axi_bram_ctrl_v4_1_rfs.vhd" \

vcom -work xil_defaultlib -64 -93  \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_bram_ctrl_0_0/sim/DMA_ZCU104_BD_axi_bram_ctrl_0_0.vhd" \

vlog -work blk_mem_gen_v8_4_9 -64 -incr -mfcu  "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/ec67/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/6f8f/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/f0b6/hdl/verilog" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/0127/hdl/verilog" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/5ec1/simulation/blk_mem_gen_v8_4.v" \

vlog -work xil_defaultlib -64 -incr -mfcu  "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/ec67/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/6f8f/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/f0b6/hdl/verilog" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/0127/hdl/verilog" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_blk_mem_gen_0_0/sim/DMA_ZCU104_BD_blk_mem_gen_0_0.v" \

vcom -work lib_pkg_v1_0_4 -64 -93  \
"../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/8c68/hdl/lib_pkg_v1_0_rfs.vhd" \

vcom -work lib_srl_fifo_v1_0_4 -64 -93  \
"../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/1e5a/hdl/lib_srl_fifo_v1_0_rfs.vhd" \

vlog -work fifo_generator_v13_2_11 -64 -incr -mfcu  "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/ec67/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/6f8f/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/f0b6/hdl/verilog" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/0127/hdl/verilog" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/6080/simulation/fifo_generator_vlog_beh.v" \

vcom -work fifo_generator_v13_2_11 -64 -93  \
"../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/6080/hdl/fifo_generator_v13_2_rfs.vhd" \

vlog -work fifo_generator_v13_2_11 -64 -incr -mfcu  "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/ec67/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/6f8f/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/f0b6/hdl/verilog" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/0127/hdl/verilog" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/6080/hdl/fifo_generator_v13_2_rfs.v" \

vcom -work lib_fifo_v1_0_20 -64 -93  \
"../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/e160/hdl/lib_fifo_v1_0_rfs.vhd" \

vcom -work lib_cdc_v1_0_3 -64 -93  \
"../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/2a4f/hdl/lib_cdc_v1_0_rfs.vhd" \

vcom -work axi_datamover_v5_1_35 -64 -93  \
"../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/4277/hdl/axi_datamover_v5_1_vh_rfs.vhd" \

vcom -work axi_sg_v4_1_19 -64 -93  \
"../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/fc5d/hdl/axi_sg_v4_1_rfs.vhd" \

vcom -work axi_cdma_v4_1_33 -64 -93  \
"../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/9d4d/hdl/axi_cdma_v4_1_vh_rfs.vhd" \

vcom -work xil_defaultlib -64 -93  \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_cdma_0_0/sim/DMA_ZCU104_BD_axi_cdma_0_0.vhd" \

vlog -work xlconstant_v1_1_9 -64 -incr -mfcu  "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/ec67/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/6f8f/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/f0b6/hdl/verilog" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/0127/hdl/verilog" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/e2d2/hdl/xlconstant_v1_1_vl_rfs.v" \

vlog -work xil_defaultlib -64 -incr -mfcu  "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/ec67/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/6f8f/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/f0b6/hdl/verilog" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/0127/hdl/verilog" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_0/sim/bd_b811_one_0.v" \

vcom -work proc_sys_reset_v5_0_16 -64 -93  \
"../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/0831/hdl/proc_sys_reset_v5_0_vh_rfs.vhd" \

vcom -work xil_defaultlib -64 -93  \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_1/sim/bd_b811_psr_aclk_0.vhd" \

vlog -work smartconnect_v1_0 -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/ec67/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/6f8f/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/f0b6/hdl/verilog" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/0127/hdl/verilog" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/f0b6/hdl/sc_util_v1_0_vl_rfs.sv" \
"../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/3718/hdl/sc_switchboard_v1_0_vl_rfs.sv" \

vlog -work xil_defaultlib -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/ec67/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/6f8f/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/f0b6/hdl/verilog" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/0127/hdl/verilog" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_2/sim/bd_b811_arsw_0.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_3/sim/bd_b811_rsw_0.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_4/sim/bd_b811_awsw_0.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_5/sim/bd_b811_wsw_0.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_6/sim/bd_b811_bsw_0.sv" \

vlog -work smartconnect_v1_0 -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/ec67/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/6f8f/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/f0b6/hdl/verilog" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/0127/hdl/verilog" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/f49a/hdl/sc_mmu_v1_0_vl_rfs.sv" \

vlog -work xil_defaultlib -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/ec67/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/6f8f/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/f0b6/hdl/verilog" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/0127/hdl/verilog" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_7/sim/bd_b811_s00mmu_0.sv" \

vlog -work smartconnect_v1_0 -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/ec67/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/6f8f/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/f0b6/hdl/verilog" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/0127/hdl/verilog" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/2da8/hdl/sc_transaction_regulator_v1_0_vl_rfs.sv" \

vlog -work xil_defaultlib -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/ec67/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/6f8f/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/f0b6/hdl/verilog" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/0127/hdl/verilog" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_8/sim/bd_b811_s00tr_0.sv" \

vlog -work smartconnect_v1_0 -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/ec67/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/6f8f/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/f0b6/hdl/verilog" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/0127/hdl/verilog" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/63ed/hdl/sc_si_converter_v1_0_vl_rfs.sv" \

vlog -work xil_defaultlib -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/ec67/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/6f8f/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/f0b6/hdl/verilog" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/0127/hdl/verilog" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_9/sim/bd_b811_s00sic_0.sv" \

vlog -work smartconnect_v1_0 -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/ec67/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/6f8f/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/f0b6/hdl/verilog" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/0127/hdl/verilog" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/cef3/hdl/sc_axi2sc_v1_0_vl_rfs.sv" \

vlog -work xil_defaultlib -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/ec67/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/6f8f/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/f0b6/hdl/verilog" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/0127/hdl/verilog" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_10/sim/bd_b811_s00a2s_0.sv" \

vlog -work smartconnect_v1_0 -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/ec67/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/6f8f/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/f0b6/hdl/verilog" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/0127/hdl/verilog" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/0127/hdl/sc_node_v1_0_vl_rfs.sv" \

vlog -work xil_defaultlib -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/ec67/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/6f8f/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/f0b6/hdl/verilog" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/0127/hdl/verilog" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_11/sim/bd_b811_sarn_0.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_12/sim/bd_b811_srn_0.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_13/sim/bd_b811_sawn_0.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_14/sim/bd_b811_swn_0.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_15/sim/bd_b811_sbn_0.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_16/sim/bd_b811_s01mmu_0.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_17/sim/bd_b811_s01tr_0.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_18/sim/bd_b811_s01sic_0.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_19/sim/bd_b811_s01a2s_0.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_20/sim/bd_b811_sarn_1.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_21/sim/bd_b811_srn_1.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_22/sim/bd_b811_sawn_1.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_23/sim/bd_b811_swn_1.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_24/sim/bd_b811_sbn_1.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_25/sim/bd_b811_s03mmu_0.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_26/sim/bd_b811_s03tr_0.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_27/sim/bd_b811_s03sic_0.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_28/sim/bd_b811_s03a2s_0.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_29/sim/bd_b811_sarn_2.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_30/sim/bd_b811_srn_2.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_31/sim/bd_b811_sawn_2.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_32/sim/bd_b811_swn_2.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_33/sim/bd_b811_sbn_2.sv" \

vlog -work smartconnect_v1_0 -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/ec67/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/6f8f/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/f0b6/hdl/verilog" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/0127/hdl/verilog" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/7f4f/hdl/sc_sc2axi_v1_0_vl_rfs.sv" \

vlog -work xil_defaultlib -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/ec67/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/6f8f/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/f0b6/hdl/verilog" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/0127/hdl/verilog" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_34/sim/bd_b811_m00s2a_0.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_35/sim/bd_b811_m00arn_0.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_36/sim/bd_b811_m00rn_0.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_37/sim/bd_b811_m00awn_0.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_38/sim/bd_b811_m00wn_0.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_39/sim/bd_b811_m00bn_0.sv" \

vlog -work smartconnect_v1_0 -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/ec67/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/6f8f/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/f0b6/hdl/verilog" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/0127/hdl/verilog" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/37bc/hdl/sc_exit_v1_0_vl_rfs.sv" \

vlog -work xil_defaultlib -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/ec67/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/6f8f/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/f0b6/hdl/verilog" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/0127/hdl/verilog" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_40/sim/bd_b811_m00e_0.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_41/sim/bd_b811_m01s2a_0.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_42/sim/bd_b811_m01arn_0.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_43/sim/bd_b811_m01rn_0.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_44/sim/bd_b811_m01awn_0.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_45/sim/bd_b811_m01wn_0.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_46/sim/bd_b811_m01bn_0.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_47/sim/bd_b811_m01e_0.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_48/sim/bd_b811_m02s2a_0.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_49/sim/bd_b811_m02arn_0.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_50/sim/bd_b811_m02rn_0.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_51/sim/bd_b811_m02awn_0.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_52/sim/bd_b811_m02wn_0.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_53/sim/bd_b811_m02bn_0.sv" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/ip/ip_54/sim/bd_b811_m02e_0.sv" \

vlog -work xil_defaultlib -64 -incr -mfcu  "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/ec67/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/6f8f/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/f0b6/hdl/verilog" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/0127/hdl/verilog" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/bd_0/sim/bd_b811.v" \

vlog -work axi_register_slice_v2_1_33 -64 -incr -mfcu  "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/ec67/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/6f8f/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/f0b6/hdl/verilog" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/0127/hdl/verilog" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/3ee4/hdl/axi_register_slice_v2_1_vl_rfs.v" \

vlog -work xil_defaultlib -64 -incr -mfcu  "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/ec67/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/6f8f/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/f0b6/hdl/verilog" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/0127/hdl/verilog" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_axi_smc_0/sim/DMA_ZCU104_BD_axi_smc_0.v" \

vcom -work xil_defaultlib -64 -93  \
"../../../bd/DMA_ZCU104_BD/ip/DMA_ZCU104_BD_rst_ps8_0_100M_0/sim/DMA_ZCU104_BD_rst_ps8_0_100M_0.vhd" \

vlog -work xil_defaultlib -64 -incr -mfcu  "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/ec67/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/6f8f/hdl" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/f0b6/hdl/verilog" "+incdir+../../../../DMA_ZCU104.gen/sources_1/bd/DMA_ZCU104_BD/ipshared/0127/hdl/verilog" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../bd/DMA_ZCU104_BD/sim/DMA_ZCU104_BD.v" \

vlog -work xil_defaultlib \
"glbl.v"

