open_project prj_mini
set_top conv_only_top
add_files gen/conv_only.cpp -cflags "-Igen -DENABLE_MINI_TEST=1"
add_files -tb test.cpp -cflags "-Igen -Inon_gen -DENABLE_MINI_TEST=1"
open_solution "sol_mini" -flow_target vitis
set_part {xczu7ev-ffvc1156-2-e}
create_clock -period 3.333 -name default
csim_design -O
exit
