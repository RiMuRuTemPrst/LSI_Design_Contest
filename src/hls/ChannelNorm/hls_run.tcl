open_project prj
set_top ChannelNorm_IP
add_files gen/ChannelNorm.cpp -cflags "-I../../resblock_top/gen"
add_files -tb test.cpp -cflags "-I../../resblock_top/gen"
open_solution "solution1" -flow_target vitis
set_part {xczu7ev-ffvc1156-2-e}
create_clock -period 3.333 -name default
csynth_design
exit
