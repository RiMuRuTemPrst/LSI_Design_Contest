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
exit
