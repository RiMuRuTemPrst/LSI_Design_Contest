vlib questa_lib/work
vlib questa_lib/msim

vlib questa_lib/msim/xilinx_vip
vlib questa_lib/msim/xpm
vlib questa_lib/msim/xil_defaultlib
vlib questa_lib/msim/xlconstant_v1_1_9
vlib questa_lib/msim/lib_cdc_v1_0_3
vlib questa_lib/msim/proc_sys_reset_v5_0_16
vlib questa_lib/msim/smartconnect_v1_0
vlib questa_lib/msim/axi_infrastructure_v1_1_0
vlib questa_lib/msim/axi_register_slice_v2_1_33
vlib questa_lib/msim/axi_vip_v1_1_19

vmap xilinx_vip questa_lib/msim/xilinx_vip
vmap xpm questa_lib/msim/xpm
vmap xil_defaultlib questa_lib/msim/xil_defaultlib
vmap xlconstant_v1_1_9 questa_lib/msim/xlconstant_v1_1_9
vmap lib_cdc_v1_0_3 questa_lib/msim/lib_cdc_v1_0_3
vmap proc_sys_reset_v5_0_16 questa_lib/msim/proc_sys_reset_v5_0_16
vmap smartconnect_v1_0 questa_lib/msim/smartconnect_v1_0
vmap axi_infrastructure_v1_1_0 questa_lib/msim/axi_infrastructure_v1_1_0
vmap axi_register_slice_v2_1_33 questa_lib/msim/axi_register_slice_v2_1_33
vmap axi_vip_v1_1_19 questa_lib/msim/axi_vip_v1_1_19

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

vlog -work xpm -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/f0b6/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/0127/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/ec67/hdl" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/3cbc" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ip/Adder32bit_sim_adder32_0_0/drivers/adder32_v1_0/src" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"/home/rimurutempest/Tool/Vivado/2024.2/data/ip/xpm/xpm_fifo/hdl/xpm_fifo.sv" \
"/home/rimurutempest/Tool/Vivado/2024.2/data/ip/xpm/xpm_memory/hdl/xpm_memory.sv" \
"/home/rimurutempest/Tool/Vivado/2024.2/data/ip/xpm/xpm_cdc/hdl/xpm_cdc.sv" \

vcom -work xpm -64 -93  \
"/home/rimurutempest/Tool/Vivado/2024.2/data/ip/xpm/xpm_VCOMP.vhd" \

vlog -work xil_defaultlib -64 -incr -mfcu  "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/f0b6/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/0127/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/ec67/hdl" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/3cbc" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ip/Adder32bit_sim_adder32_0_0/drivers/adder32_v1_0/src" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/2427/hdl/verilog/adder32_CTRL_s_axi.v" \
"../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/2427/hdl/verilog/adder32.v" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_adder32_0_0/sim/Adder32bit_sim_adder32_0_0.v" \

vlog -work xlconstant_v1_1_9 -64 -incr -mfcu  "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/f0b6/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/0127/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/ec67/hdl" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/3cbc" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ip/Adder32bit_sim_adder32_0_0/drivers/adder32_v1_0/src" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/e2d2/hdl/xlconstant_v1_1_vl_rfs.v" \

vlog -work xil_defaultlib -64 -incr -mfcu  "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/f0b6/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/0127/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/ec67/hdl" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/3cbc" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ip/Adder32bit_sim_adder32_0_0/drivers/adder32_v1_0/src" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_axi_smc_0/bd_0/ip/ip_0/sim/bd_7461_one_0.v" \

vcom -work lib_cdc_v1_0_3 -64 -93  \
"../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/2a4f/hdl/lib_cdc_v1_0_rfs.vhd" \

vcom -work proc_sys_reset_v5_0_16 -64 -93  \
"../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/0831/hdl/proc_sys_reset_v5_0_vh_rfs.vhd" \

vcom -work xil_defaultlib -64 -93  \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_axi_smc_0/bd_0/ip/ip_1/sim/bd_7461_psr_aclk_0.vhd" \

vlog -work smartconnect_v1_0 -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/f0b6/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/0127/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/ec67/hdl" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/3cbc" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ip/Adder32bit_sim_adder32_0_0/drivers/adder32_v1_0/src" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/f0b6/hdl/sc_util_v1_0_vl_rfs.sv" \
"../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/3718/hdl/sc_switchboard_v1_0_vl_rfs.sv" \

vlog -work xil_defaultlib -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/f0b6/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/0127/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/ec67/hdl" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/3cbc" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ip/Adder32bit_sim_adder32_0_0/drivers/adder32_v1_0/src" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_axi_smc_0/bd_0/ip/ip_2/sim/bd_7461_arinsw_0.sv" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_axi_smc_0/bd_0/ip/ip_3/sim/bd_7461_rinsw_0.sv" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_axi_smc_0/bd_0/ip/ip_4/sim/bd_7461_awinsw_0.sv" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_axi_smc_0/bd_0/ip/ip_5/sim/bd_7461_winsw_0.sv" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_axi_smc_0/bd_0/ip/ip_6/sim/bd_7461_binsw_0.sv" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_axi_smc_0/bd_0/ip/ip_7/sim/bd_7461_aroutsw_0.sv" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_axi_smc_0/bd_0/ip/ip_8/sim/bd_7461_routsw_0.sv" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_axi_smc_0/bd_0/ip/ip_9/sim/bd_7461_awoutsw_0.sv" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_axi_smc_0/bd_0/ip/ip_10/sim/bd_7461_woutsw_0.sv" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_axi_smc_0/bd_0/ip/ip_11/sim/bd_7461_boutsw_0.sv" \

vlog -work smartconnect_v1_0 -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/f0b6/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/0127/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/ec67/hdl" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/3cbc" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ip/Adder32bit_sim_adder32_0_0/drivers/adder32_v1_0/src" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/0127/hdl/sc_node_v1_0_vl_rfs.sv" \

vlog -work xil_defaultlib -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/f0b6/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/0127/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/ec67/hdl" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/3cbc" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ip/Adder32bit_sim_adder32_0_0/drivers/adder32_v1_0/src" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_axi_smc_0/bd_0/ip/ip_12/sim/bd_7461_arni_0.sv" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_axi_smc_0/bd_0/ip/ip_13/sim/bd_7461_rni_0.sv" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_axi_smc_0/bd_0/ip/ip_14/sim/bd_7461_awni_0.sv" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_axi_smc_0/bd_0/ip/ip_15/sim/bd_7461_wni_0.sv" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_axi_smc_0/bd_0/ip/ip_16/sim/bd_7461_bni_0.sv" \

vlog -work smartconnect_v1_0 -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/f0b6/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/0127/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/ec67/hdl" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/3cbc" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ip/Adder32bit_sim_adder32_0_0/drivers/adder32_v1_0/src" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/f49a/hdl/sc_mmu_v1_0_vl_rfs.sv" \

vlog -work xil_defaultlib -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/f0b6/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/0127/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/ec67/hdl" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/3cbc" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ip/Adder32bit_sim_adder32_0_0/drivers/adder32_v1_0/src" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_axi_smc_0/bd_0/ip/ip_17/sim/bd_7461_s00mmu_0.sv" \

vlog -work smartconnect_v1_0 -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/f0b6/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/0127/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/ec67/hdl" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/3cbc" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ip/Adder32bit_sim_adder32_0_0/drivers/adder32_v1_0/src" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/2da8/hdl/sc_transaction_regulator_v1_0_vl_rfs.sv" \

vlog -work xil_defaultlib -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/f0b6/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/0127/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/ec67/hdl" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/3cbc" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ip/Adder32bit_sim_adder32_0_0/drivers/adder32_v1_0/src" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_axi_smc_0/bd_0/ip/ip_18/sim/bd_7461_s00tr_0.sv" \

vlog -work smartconnect_v1_0 -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/f0b6/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/0127/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/ec67/hdl" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/3cbc" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ip/Adder32bit_sim_adder32_0_0/drivers/adder32_v1_0/src" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/63ed/hdl/sc_si_converter_v1_0_vl_rfs.sv" \

vlog -work xil_defaultlib -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/f0b6/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/0127/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/ec67/hdl" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/3cbc" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ip/Adder32bit_sim_adder32_0_0/drivers/adder32_v1_0/src" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_axi_smc_0/bd_0/ip/ip_19/sim/bd_7461_s00sic_0.sv" \

vlog -work smartconnect_v1_0 -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/f0b6/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/0127/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/ec67/hdl" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/3cbc" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ip/Adder32bit_sim_adder32_0_0/drivers/adder32_v1_0/src" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/cef3/hdl/sc_axi2sc_v1_0_vl_rfs.sv" \

vlog -work xil_defaultlib -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/f0b6/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/0127/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/ec67/hdl" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/3cbc" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ip/Adder32bit_sim_adder32_0_0/drivers/adder32_v1_0/src" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_axi_smc_0/bd_0/ip/ip_20/sim/bd_7461_s00a2s_0.sv" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_axi_smc_0/bd_0/ip/ip_21/sim/bd_7461_sarn_0.sv" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_axi_smc_0/bd_0/ip/ip_22/sim/bd_7461_srn_0.sv" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_axi_smc_0/bd_0/ip/ip_23/sim/bd_7461_sawn_0.sv" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_axi_smc_0/bd_0/ip/ip_24/sim/bd_7461_swn_0.sv" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_axi_smc_0/bd_0/ip/ip_25/sim/bd_7461_sbn_0.sv" \

vlog -work smartconnect_v1_0 -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/f0b6/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/0127/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/ec67/hdl" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/3cbc" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ip/Adder32bit_sim_adder32_0_0/drivers/adder32_v1_0/src" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/7f4f/hdl/sc_sc2axi_v1_0_vl_rfs.sv" \

vlog -work xil_defaultlib -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/f0b6/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/0127/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/ec67/hdl" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/3cbc" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ip/Adder32bit_sim_adder32_0_0/drivers/adder32_v1_0/src" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_axi_smc_0/bd_0/ip/ip_26/sim/bd_7461_m00s2a_0.sv" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_axi_smc_0/bd_0/ip/ip_27/sim/bd_7461_m00arn_0.sv" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_axi_smc_0/bd_0/ip/ip_28/sim/bd_7461_m00rn_0.sv" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_axi_smc_0/bd_0/ip/ip_29/sim/bd_7461_m00awn_0.sv" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_axi_smc_0/bd_0/ip/ip_30/sim/bd_7461_m00wn_0.sv" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_axi_smc_0/bd_0/ip/ip_31/sim/bd_7461_m00bn_0.sv" \

vlog -work smartconnect_v1_0 -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/f0b6/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/0127/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/ec67/hdl" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/3cbc" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ip/Adder32bit_sim_adder32_0_0/drivers/adder32_v1_0/src" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/37bc/hdl/sc_exit_v1_0_vl_rfs.sv" \

vlog -work xil_defaultlib -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/f0b6/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/0127/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/ec67/hdl" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/3cbc" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ip/Adder32bit_sim_adder32_0_0/drivers/adder32_v1_0/src" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_axi_smc_0/bd_0/ip/ip_32/sim/bd_7461_m00e_0.sv" \

vlog -work xil_defaultlib -64 -incr -mfcu  "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/f0b6/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/0127/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/ec67/hdl" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/3cbc" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ip/Adder32bit_sim_adder32_0_0/drivers/adder32_v1_0/src" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_axi_smc_0/bd_0/sim/bd_7461.v" \

vlog -work axi_infrastructure_v1_1_0 -64 -incr -mfcu  "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/f0b6/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/0127/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/ec67/hdl" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/3cbc" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ip/Adder32bit_sim_adder32_0_0/drivers/adder32_v1_0/src" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/ec67/hdl/axi_infrastructure_v1_1_vl_rfs.v" \

vlog -work axi_register_slice_v2_1_33 -64 -incr -mfcu  "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/f0b6/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/0127/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/ec67/hdl" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/3cbc" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ip/Adder32bit_sim_adder32_0_0/drivers/adder32_v1_0/src" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/3ee4/hdl/axi_register_slice_v2_1_vl_rfs.v" \

vlog -work axi_vip_v1_1_19 -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/f0b6/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/0127/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/ec67/hdl" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/3cbc" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ip/Adder32bit_sim_adder32_0_0/drivers/adder32_v1_0/src" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/8c45/hdl/axi_vip_v1_1_vl_rfs.sv" \

vlog -work xil_defaultlib -64 -incr -mfcu  "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/f0b6/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/0127/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/ec67/hdl" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/3cbc" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ip/Adder32bit_sim_adder32_0_0/drivers/adder32_v1_0/src" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_axi_smc_0/sim/Adder32bit_sim_axi_smc_0.v" \

vlog -work xil_defaultlib -64 -incr -mfcu  -sv -L axi_vip_v1_1_19 -L smartconnect_v1_0 -L zynq_ultra_ps_e_vip_v1_0_19 -L xilinx_vip "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/f0b6/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/0127/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/ec67/hdl" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/3cbc" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ip/Adder32bit_sim_adder32_0_0/drivers/adder32_v1_0/src" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_axi_vip_0_0/sim/Adder32bit_sim_axi_vip_0_0_pkg.sv" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_axi_vip_0_0/sim/Adder32bit_sim_axi_vip_0_0.sv" \

vlog -work xil_defaultlib -64 -incr -mfcu  "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/f0b6/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/0127/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/ec67/hdl" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/3cbc" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ip/Adder32bit_sim_adder32_0_0/drivers/adder32_v1_0/src" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_clk_wiz_0_0/Adder32bit_sim_clk_wiz_0_0_clk_wiz.v" \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_clk_wiz_0_0/Adder32bit_sim_clk_wiz_0_0.v" \

vcom -work xil_defaultlib -64 -93  \
"../../../bd/Adder32bit_sim/ip/Adder32bit_sim_proc_sys_reset_0_0/sim/Adder32bit_sim_proc_sys_reset_0_0.vhd" \

vlog -work xil_defaultlib -64 -incr -mfcu  "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/f0b6/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/0127/hdl/verilog" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/ec67/hdl" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ipshared/3cbc" "+incdir+../../../../Adder32bit_Vivado.gen/sources_1/bd/Adder32bit_sim/ip/Adder32bit_sim_adder32_0_0/drivers/adder32_v1_0/src" "+incdir+/home/rimurutempest/Tool/Vivado/2024.2/data/xilinx_vip/include" \
"../../../bd/Adder32bit_sim/sim/Adder32bit_sim.v" \

vlog -work xil_defaultlib \
"glbl.v"

