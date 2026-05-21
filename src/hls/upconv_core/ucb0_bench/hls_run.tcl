open_project prj
set_top ucb0_bench_top
add_files bench_top.cpp -cflags "-I../gen"
open_solution "solution1" -flow_target vitis
set_part {xczu7ev-ffvc1156-2-e}
create_clock -period 3.333 -name default
csynth_design
exit
