open_project prj_csim
set_top conv_only_top
add_files gen/conv_only.cpp -cflags "-Igen -DENABLE_MINI_TEST=0"
set data_path [file normalize "../../../assets/test_data"]
set hdr_path  [file normalize "non_gen/csim_datapath.h"]
set fh [open $hdr_path w]
puts $fh "#pragma once"
puts $fh "#define DATA_PATH \"${data_path}/\""
close $fh
add_files -tb test.cpp -cflags "-Igen -Inon_gen -include ${hdr_path} -DENABLE_MINI_TEST=0"
open_solution "sol_csim" -flow_target vitis
set_part {xczu7ev-ffvc1156-2-e}
create_clock -period 3.333 -name default
csim_design -O
exit
