open_project -reset prj_csim_full
set_top full_generator_top
add_files gen/generator_top.cpp -cflags "-std=c++17 -Igen -I../fusion_core/gen -I../upconv_core/gen -I../Conv77/gen"
add_files -tb gen/test_full.cpp  -cflags "-std=c++17 -Igen -I../fusion_core/gen -I../upconv_core/gen -I../Conv77/gen"
open_solution -reset "solution_csim_full" -flow_target vitis
set_part {xczu7ev-ffvc1156-2-e}
create_clock -period 3.333 -name default
puts ">>> Running Full E2E CSIM (full_generator_top)..."
csim_design -clean
puts ">>> DONE"
exit
