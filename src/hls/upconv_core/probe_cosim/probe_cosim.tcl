open_project prj_probe
set_top probe_top
add_files probe_top.cpp -cflags "-I../gen"
add_files -tb probe_tb.cpp -cflags "-I../gen"
open_solution "solution1" -flow_target vitis
set_part {xczu7ev-ffvc1156-2-e}
create_clock -period 3.333 -name default
csim_design
csynth_design
cosim_design
exit
