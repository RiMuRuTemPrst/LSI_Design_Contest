# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct /home/rimurutempest/Code/LSI_Design_Contest/HLS/DMA/DMA_ZCU104_Platform/platform.tcl
# 
# OR launch xsct and run below command.
# source /home/rimurutempest/Code/LSI_Design_Contest/HLS/DMA/DMA_ZCU104_Platform/platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {DMA_ZCU104_Platform}\
-hw {/home/rimurutempest/Code/LSI_Design_Contest/HLS/DMA/DMA_ZCU104/DMA_ZCU104_BD_wrapper.xsa}\
-arch {64-bit} -fsbl-target {psu_cortexa53_0} -out {/home/rimurutempest/Code/LSI_Design_Contest/HLS/DMA}

platform write
domain create -name {standalone_psu_cortexa53_0} -display-name {standalone_psu_cortexa53_0} -os {standalone} -proc {psu_cortexa53_0} -runtime {cpp} -arch {64-bit} -support-app {hello_world}
platform generate -domains 
platform active {DMA_ZCU104_Platform}
domain active {zynqmp_fsbl}
domain active {zynqmp_pmufw}
domain active {standalone_psu_cortexa53_0}
platform generate -quick
platform generate
platform config -updatehw {/home/rimurutempest/Code/LSI_Design_Contest/HLS/DMA/DMA_ZCU104/DMA_ZCU104_BD_wrapper.xsa}
platform config -updatehw {/home/rimurutempest/Code/LSI_Design_Contest/HLS/DMA/DMA_ZCU104/DMA_ZCU104_BD_wrapper.xsa}
platform active {DMA_ZCU104_Platform}
platform config -updatehw {/home/rimurutempest/Code/LSI_Design_Contest/HLS/DMA/DMA_ZCU104/DMA_ZCU104_BD_wrapper.xsa}
platform write
platform active {DMA_ZCU104_Platform}
platform config -updatehw {/home/rimurutempest/Code/LSI_Design_Contest/HLS/DMA/DMA_ZCU104/DMA_ZCU104_BD_wrapper_ver3.xsa}
platform generate -domains 
platform config -updatehw {/home/rimurutempest/Code/LSI_Design_Contest/HLS/DMA/DMA_ZCU104/DMA_ZCU104_BD_wrapper_ver3.xsa}
