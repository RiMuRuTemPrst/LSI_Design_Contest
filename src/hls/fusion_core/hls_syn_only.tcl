open_project prj
set_top fusion_core_top
add_files gen/fusion_core_top.cpp -cflags "-Igen"
open_solution "solution1" -flow_target vitis
set_part {xczu7ev-ffvc1156-2-e}
create_clock -period 3.333 -name default
csynth_design
exit
