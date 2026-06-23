open_project prj_csim
set_top conv_only_top
add_files gen/conv_only.cpp -cflags "-Igen -DENABLE_MINI_TEST=0"
add_files -tb test.cpp -cflags "-Igen -Inon_gen -DENABLE_MINI_TEST=0"
open_solution "sol_csim" -flow_target vitis
set_part {xczu7ev-ffvc1156-2-e}
create_clock -period 3.333 -name default
csim_design -O
exit
