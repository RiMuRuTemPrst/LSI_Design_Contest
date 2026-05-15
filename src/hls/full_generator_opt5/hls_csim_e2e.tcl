open_project -reset prj_csim_e2e
set_top full_generator_top
add_files gen/generator_top.cpp -cflags "-std=c++17 -Igen -I../fusion_core/gen -I../upconv_core/gen -I../Conv77/gen"
add_files -tb gen/test_e2e.cpp  -cflags "-std=c++17 -Igen -I../fusion_core/gen -I../upconv_core/gen -I../Conv77/gen"
open_solution -reset "solution_csim_e2e" -flow_target vitis
set_part {xczu7ev-ffvc1156-2-e}
create_clock -period 3.333 -name default
puts ">>> Running E2E CSIM (UCB + Conv77, skipping Fusion)..."
csim_design -clean
puts ">>> DONE"
exit
