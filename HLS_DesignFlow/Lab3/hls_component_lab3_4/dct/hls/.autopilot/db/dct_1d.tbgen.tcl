set moduleName dct_1d
set isTopModule 0
set isCombinational 0
set isDatapathOnly 0
set isPipelined 1
set pipeline_type function
set FunctionProtocol ap_ctrl_hs
set isOneStateSeq 0
set ProfileFlag 0
set StallSigGenFlag 0
set isEnableWaveformDebug 1
set hasInterrupt 0
set DLRegFirstOffset 0
set DLRegItemOffset 0
set svuvm_can_support 1
set cdfgNum 10
set C_modelName {dct_1d}
set C_modelType { void 0 }
set ap_memory_interface_dict [dict create]
dict set ap_memory_interface_dict dst { MEM_WIDTH 16 MEM_SIZE 128 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 0 }
set C_modelArgList {
	{ src_0_val int 16 regular  }
	{ src_1_val int 16 regular  }
	{ src_2_val int 16 regular  }
	{ src_3_val int 16 regular  }
	{ src_4_val int 16 regular  }
	{ src_5_val int 16 regular  }
	{ src_6_val int 16 regular  }
	{ src_7_val int 16 regular  }
	{ dst int 16 regular {array 64 { 0 0 } 0 1 }  }
	{ dst_offset int 3 regular  }
}
set hasAXIMCache 0
set l_AXIML2Cache [list]
set AXIMCacheInstDict [dict create]
set C_modelArgMapList {[ 
	{ "Name" : "src_0_val", "interface" : "wire", "bitwidth" : 16, "direction" : "READONLY"} , 
 	{ "Name" : "src_1_val", "interface" : "wire", "bitwidth" : 16, "direction" : "READONLY"} , 
 	{ "Name" : "src_2_val", "interface" : "wire", "bitwidth" : 16, "direction" : "READONLY"} , 
 	{ "Name" : "src_3_val", "interface" : "wire", "bitwidth" : 16, "direction" : "READONLY"} , 
 	{ "Name" : "src_4_val", "interface" : "wire", "bitwidth" : 16, "direction" : "READONLY"} , 
 	{ "Name" : "src_5_val", "interface" : "wire", "bitwidth" : 16, "direction" : "READONLY"} , 
 	{ "Name" : "src_6_val", "interface" : "wire", "bitwidth" : 16, "direction" : "READONLY"} , 
 	{ "Name" : "src_7_val", "interface" : "wire", "bitwidth" : 16, "direction" : "READONLY"} , 
 	{ "Name" : "dst", "interface" : "memory", "bitwidth" : 16, "direction" : "WRITEONLY"} , 
 	{ "Name" : "dst_offset", "interface" : "wire", "bitwidth" : 3, "direction" : "READONLY"} ]}
# RTL Port declarations: 
set portNum 23
set portList { 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst sc_in sc_logic 1 reset -1 active_high_sync } 
	{ ap_start sc_in sc_logic 1 start -1 } 
	{ ap_done sc_out sc_logic 1 predone -1 } 
	{ ap_idle sc_out sc_logic 1 done -1 } 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ src_0_val sc_in sc_lv 16 signal 0 } 
	{ src_1_val sc_in sc_lv 16 signal 1 } 
	{ src_2_val sc_in sc_lv 16 signal 2 } 
	{ src_3_val sc_in sc_lv 16 signal 3 } 
	{ src_4_val sc_in sc_lv 16 signal 4 } 
	{ src_5_val sc_in sc_lv 16 signal 5 } 
	{ src_6_val sc_in sc_lv 16 signal 6 } 
	{ src_7_val sc_in sc_lv 16 signal 7 } 
	{ dst_address0 sc_out sc_lv 6 signal 8 } 
	{ dst_ce0 sc_out sc_logic 1 signal 8 } 
	{ dst_we0 sc_out sc_logic 1 signal 8 } 
	{ dst_d0 sc_out sc_lv 16 signal 8 } 
	{ dst_address1 sc_out sc_lv 6 signal 8 } 
	{ dst_ce1 sc_out sc_logic 1 signal 8 } 
	{ dst_we1 sc_out sc_logic 1 signal 8 } 
	{ dst_d1 sc_out sc_lv 16 signal 8 } 
	{ dst_offset sc_in sc_lv 3 signal 9 } 
}
set NewPortList {[ 
	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst", "role": "default" }} , 
 	{ "name": "ap_start", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "start", "bundle":{"name": "ap_start", "role": "default" }} , 
 	{ "name": "ap_done", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "predone", "bundle":{"name": "ap_done", "role": "default" }} , 
 	{ "name": "ap_idle", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "done", "bundle":{"name": "ap_idle", "role": "default" }} , 
 	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "src_0_val", "direction": "in", "datatype": "sc_lv", "bitwidth":16, "type": "signal", "bundle":{"name": "src_0_val", "role": "default" }} , 
 	{ "name": "src_1_val", "direction": "in", "datatype": "sc_lv", "bitwidth":16, "type": "signal", "bundle":{"name": "src_1_val", "role": "default" }} , 
 	{ "name": "src_2_val", "direction": "in", "datatype": "sc_lv", "bitwidth":16, "type": "signal", "bundle":{"name": "src_2_val", "role": "default" }} , 
 	{ "name": "src_3_val", "direction": "in", "datatype": "sc_lv", "bitwidth":16, "type": "signal", "bundle":{"name": "src_3_val", "role": "default" }} , 
 	{ "name": "src_4_val", "direction": "in", "datatype": "sc_lv", "bitwidth":16, "type": "signal", "bundle":{"name": "src_4_val", "role": "default" }} , 
 	{ "name": "src_5_val", "direction": "in", "datatype": "sc_lv", "bitwidth":16, "type": "signal", "bundle":{"name": "src_5_val", "role": "default" }} , 
 	{ "name": "src_6_val", "direction": "in", "datatype": "sc_lv", "bitwidth":16, "type": "signal", "bundle":{"name": "src_6_val", "role": "default" }} , 
 	{ "name": "src_7_val", "direction": "in", "datatype": "sc_lv", "bitwidth":16, "type": "signal", "bundle":{"name": "src_7_val", "role": "default" }} , 
 	{ "name": "dst_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":6, "type": "signal", "bundle":{"name": "dst", "role": "address0" }} , 
 	{ "name": "dst_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "dst", "role": "ce0" }} , 
 	{ "name": "dst_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "dst", "role": "we0" }} , 
 	{ "name": "dst_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":16, "type": "signal", "bundle":{"name": "dst", "role": "d0" }} , 
 	{ "name": "dst_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":6, "type": "signal", "bundle":{"name": "dst", "role": "address1" }} , 
 	{ "name": "dst_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "dst", "role": "ce1" }} , 
 	{ "name": "dst_we1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "dst", "role": "we1" }} , 
 	{ "name": "dst_d1", "direction": "out", "datatype": "sc_lv", "bitwidth":16, "type": "signal", "bundle":{"name": "dst", "role": "d1" }} , 
 	{ "name": "dst_offset", "direction": "in", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "dst_offset", "role": "default" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28"],
		"CDFG" : "dct_1d",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1", "real_start" : "0",
		"Pipeline" : "Aligned", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "4",
		"VariableLatency" : "0", "ExactLatency" : "9", "EstimateLatencyMin" : "9", "EstimateLatencyMax" : "9",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "src_0_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "src_1_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "src_2_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "src_3_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "src_4_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "src_5_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "src_6_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "src_7_val", "Type" : "None", "Direction" : "I"},
			{"Name" : "dst", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "dst_offset", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.mul_16s_15ns_29_1_1_U10", "Parent" : "0"},
	{"ID" : "2", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.mul_16s_15s_29_1_1_U11", "Parent" : "0"},
	{"ID" : "3", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.mul_16s_15ns_29_1_1_U12", "Parent" : "0"},
	{"ID" : "4", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.mul_16s_15s_29_1_1_U13", "Parent" : "0"},
	{"ID" : "5", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.mul_16s_15s_29_1_1_U14", "Parent" : "0"},
	{"ID" : "6", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.mul_16s_15s_29_1_1_U15", "Parent" : "0"},
	{"ID" : "7", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.mul_16s_15ns_29_1_1_U16", "Parent" : "0"},
	{"ID" : "8", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.mul_16s_15s_29_1_1_U17", "Parent" : "0"},
	{"ID" : "9", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.mac_muladd_16s_15s_13ns_29_4_1_U18", "Parent" : "0"},
	{"ID" : "10", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.ama_submuladd_16s_16s_12ns_29s_29_4_1_U19", "Parent" : "0"},
	{"ID" : "11", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.mac_muladd_16s_14ns_29s_29_4_1_U20", "Parent" : "0"},
	{"ID" : "12", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.mac_muladd_16s_15s_29s_29_4_1_U21", "Parent" : "0"},
	{"ID" : "13", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.mac_muladd_16s_14ns_29s_29_4_1_U22", "Parent" : "0"},
	{"ID" : "14", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.mac_muladd_17s_13ns_29s_29_4_1_U23", "Parent" : "0"},
	{"ID" : "15", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.mac_muladd_18s_14ns_13ns_29_4_1_U24", "Parent" : "0"},
	{"ID" : "16", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.ama_submuladd_16s_16s_13ns_29s_29_4_1_U25", "Parent" : "0"},
	{"ID" : "17", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.mac_muladd_17s_12ns_13ns_29_4_1_U26", "Parent" : "0"},
	{"ID" : "18", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.mac_muladd_17s_13ns_13ns_29_4_1_U27", "Parent" : "0"},
	{"ID" : "19", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.mac_muladd_16s_14ns_29ns_29_4_1_U28", "Parent" : "0"},
	{"ID" : "20", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.mac_muladd_17s_12ns_29s_29_4_1_U29", "Parent" : "0"},
	{"ID" : "21", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.mac_muladd_18s_13ns_13ns_29_4_1_U30", "Parent" : "0"},
	{"ID" : "22", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.mac_muladd_17s_13ns_29s_29_4_1_U31", "Parent" : "0"},
	{"ID" : "23", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.mac_muladd_17s_12ns_13ns_29_4_1_U32", "Parent" : "0"},
	{"ID" : "24", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.ama_addmuladd_18s_16s_13ns_29ns_29_4_1_U33", "Parent" : "0"},
	{"ID" : "25", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.mac_muladd_16s_14ns_29ns_29_4_1_U34", "Parent" : "0"},
	{"ID" : "26", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.mac_muladd_16s_15s_29ns_29_4_1_U35", "Parent" : "0"},
	{"ID" : "27", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.ama_submuladd_18s_16s_14ns_29ns_29_4_1_U36", "Parent" : "0"},
	{"ID" : "28", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.mac_muladd_16s_14ns_29ns_29_4_1_U37", "Parent" : "0"}]}


set ArgLastReadFirstWriteLatency {
	dct_1d {
		src_0_val {Type I LastRead 0 FirstWrite -1}
		src_1_val {Type I LastRead 0 FirstWrite -1}
		src_2_val {Type I LastRead 0 FirstWrite -1}
		src_3_val {Type I LastRead 0 FirstWrite -1}
		src_4_val {Type I LastRead 0 FirstWrite -1}
		src_5_val {Type I LastRead 0 FirstWrite -1}
		src_6_val {Type I LastRead 0 FirstWrite -1}
		src_7_val {Type I LastRead 0 FirstWrite -1}
		dst {Type O LastRead -1 FirstWrite 1}
		dst_offset {Type I LastRead 1 FirstWrite -1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "9", "Max" : "9"}
	, {"Name" : "Interval", "Min" : "4", "Max" : "4"}
]}

set PipelineEnableSignalInfo {[
	{"Pipeline" : "0", "EnableSignal" : "ap_enable_pp0"}
]}

set Spec2ImplPortList { 
	src_0_val { ap_none {  { src_0_val in_data 0 16 } } }
	src_1_val { ap_none {  { src_1_val in_data 0 16 } } }
	src_2_val { ap_none {  { src_2_val in_data 0 16 } } }
	src_3_val { ap_none {  { src_3_val in_data 0 16 } } }
	src_4_val { ap_none {  { src_4_val in_data 0 16 } } }
	src_5_val { ap_none {  { src_5_val in_data 0 16 } } }
	src_6_val { ap_none {  { src_6_val in_data 0 16 } } }
	src_7_val { ap_none {  { src_7_val in_data 0 16 } } }
	dst { ap_memory {  { dst_address0 mem_address 1 6 }  { dst_ce0 mem_ce 1 1 }  { dst_we0 mem_we 1 1 }  { dst_d0 mem_din 1 16 }  { dst_address1 MemPortADDR2 1 6 }  { dst_ce1 MemPortCE2 1 1 }  { dst_we1 MemPortWE2 1 1 }  { dst_d1 MemPortDIN2 1 16 } } }
	dst_offset { ap_none {  { dst_offset in_data 0 3 } } }
}
