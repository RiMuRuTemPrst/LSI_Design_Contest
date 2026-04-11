# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct /home/rimurutempest/Code/LSI_Design_Contest/Adder32bit_BD_wrapper/platform.tcl
# 
# OR launch xsct and run below command.
# source /home/rimurutempest/Code/LSI_Design_Contest/Adder32bit_BD_wrapper/platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {Adder32bit_BD_wrapper}\
-hw {/home/rimurutempest/Code/LSI_Design_Contest/HLS/Adder32bit/Adder32bit_Vivado/Adder32bit_BD_wrapper.xsa}\
-arch {64-bit} -fsbl-target {psu_cortexa53_0} -out {/home/rimurutempest/Code/LSI_Design_Contest}

platform write
domain create -name {standalone_psu_cortexa53_0} -display-name {standalone_psu_cortexa53_0} -os {standalone} -proc {psu_cortexa53_0} -runtime {cpp} -arch {64-bit} -support-app {hello_world}
platform generate -domains 
platform active {Adder32bit_BD_wrapper}
domain active {zynqmp_fsbl}
domain active {zynqmp_pmufw}
domain active {standalone_psu_cortexa53_0}
platform generate -quick
platform generate
platform generate
