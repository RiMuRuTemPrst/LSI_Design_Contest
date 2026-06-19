# Re-csynth chỉ các top fp16 vừa sửa (mul_fp16, mac_fp16) -> product ra float32
set tops { mul_fp16 mac_fp16 }
foreach t $tops {
    open_project -reset prj_$t
    set_top $t
    add_files mac_probe.cpp
    open_solution -reset sol -flow_target vivado
    set_part {xczu7ev-ffvc1156-2-e}
    create_clock -period 3.333 -name default
    csynth_design
    close_project
}
exit
