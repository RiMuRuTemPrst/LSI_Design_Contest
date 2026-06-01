# =============================================================================
# Vitis HLS 2024.2 — Conv77 Run Script
# Target: ZCU104 (xczu7ev-ffvc1156-2-e) @ 200 MHz (5 ns period)
#
# Design: fp16 SIMD (SIMD_DEPTH=8) + Spatial-PE (NUM_WIN_PEs=8)
#   - 1 PE performs 8 parallel MACs on 8 depth (channel) elements
#   - 8 PEs process 8 adjacent output windows simultaneously
#   - Data type: ap_fixed<16,8> (fp16 via HLS ap library)
#   - Estimated latency: ~305 ms @ 200 MHz
#
# Usage:
#   vitis_hls -f hls_run.tcl
#
# Flow: csim → csynth → export_ip
# =============================================================================

set TOP_FUNC "HW_Conv7x7"
set PART     "xczu7ev-ffvc1156-2-e"
set CLOCK_NS 3.333
set SOLUTION "solution"

open_project -reset Conv77_HLS
set_top $TOP_FUNC

add_files "gen/Hard_op_3.cpp" -cflags "-std=c++17 -I."
add_files -tb test.cpp         -cflags "-std=c++17 -I."
add_files -tb io_params/Gen_ucb4_Relu_output_0.txt
add_files -tb io_params/output_numbers.txt
add_files -tb model_params/Gen_cbo_weight.txt
add_files -tb model_params/Gen_cbo_bias.txt

open_solution -reset $SOLUTION -flow_target vivado
set_part $PART
create_clock -period $CLOCK_NS -name default

puts ">>> Running C Simulation..."
csim_design -clean
puts ">>> csim DONE"

puts ">>> Running HLS Synthesis..."
csynth_design
puts ">>> csynth DONE"

# --- Co-simulation (optional — uncomment to run RTL sim) --------------------
# cosim_design -rtl verilog -trace_level all

puts ">>> Exporting IP Catalog..."
export_design -format ip_catalog -output ./Conv77_HLS_fp16_ip
puts ">>> Done. Report: Conv77_HLS/$SOLUTION/syn/report/csynth.rpt"
exit
