open_project prj
set_top full_generator_top
add_files gen/generator_top.cpp -cflags "-I../fusion_core/gen -I../upconv_core/gen -I../Conv77/gen"
open_solution "solution1" -flow_target vitis
set_part {xczu7ev-ffvc1156-2-e}
create_clock -period 3.333 -name default
csynth_design
exit
