open_project prj
set_top full_generator_top
add_files gen/generator_top.cpp -cflags "-Igen -I../fusion_core/gen -I../upconv_core/gen -I../Conv77/gen"
add_files -tb gen/test_full.cpp  -cflags "-std=c++17 -O3 -Igen -I../fusion_core/gen -I../upconv_core/gen -I../Conv77/gen"
open_solution "solution1" -flow_target vitis
set_part {xczu7ev-ffvc1156-2-e}
create_clock -period 3.333 -name default
config_cosim -disable_binary_tv
puts ">>> Running Co-Simulation (full_generator_top)..."
cosim_design -trace_level none
puts ">>> DONE"
exit
