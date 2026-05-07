open_project prj
set_top upconv_core_top
add_files gen/upconv_core_top.cpp -cflags "-Igen"
add_files -tb test.cpp -cflags "-Igen"
open_solution "solution1" -flow_target vitis
set_part {xczu7ev-ffvc1156-2-e}
create_clock -period 3.333 -name default
csim_design
exit
