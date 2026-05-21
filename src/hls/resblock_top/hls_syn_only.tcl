open_project prj
set_top resblock_top
add_files gen/resblock_top.cpp -cflags "-Igen -DENABLE_MINI_TEST=0"
open_solution "solution1" -flow_target vitis
set_part {xczu7ev-ffvc1156-2-e}
create_clock -period 3.333 -name default
csynth_design
exit
