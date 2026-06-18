# csynth từng top riêng (1 op/kernel) trên xczu7ev @300MHz, flow vivado (cổng scalar, không AXI)
set tops {
    mul_fp32 mul_fp16 mul_fp8 mul_int16 mul_int8
    add_fp32 add_fp16 add_int32 add_int16
    mac_fp32 mac_fp16 mac_fp8 mac_int16 mac_int8
}
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
