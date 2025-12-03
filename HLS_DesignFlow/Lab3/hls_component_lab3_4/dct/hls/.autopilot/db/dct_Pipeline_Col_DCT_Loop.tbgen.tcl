set moduleName dct_Pipeline_Col_DCT_Loop
set isTopModule 0
set isCombinational 0
set isDatapathOnly 0
set isPipelined 1
set pipeline_type loop_auto_rewind
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
set C_modelName {dct_Pipeline_Col_DCT_Loop}
set C_modelType { void 0 }
set ap_memory_interface_dict [dict create]
dict set ap_memory_interface_dict col_inbuf { MEM_WIDTH 16 MEM_SIZE 16 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 1 }
dict set ap_memory_interface_dict col_inbuf_1 { MEM_WIDTH 16 MEM_SIZE 16 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 1 }
dict set ap_memory_interface_dict col_inbuf_2 { MEM_WIDTH 16 MEM_SIZE 16 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 1 }
dict set ap_memory_interface_dict col_inbuf_3 { MEM_WIDTH 16 MEM_SIZE 16 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 1 }
dict set ap_memory_interface_dict col_inbuf_4 { MEM_WIDTH 16 MEM_SIZE 16 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 1 }
dict set ap_memory_interface_dict col_inbuf_5 { MEM_WIDTH 16 MEM_SIZE 16 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 1 }
dict set ap_memory_interface_dict col_inbuf_6 { MEM_WIDTH 16 MEM_SIZE 16 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 1 }
dict set ap_memory_interface_dict col_inbuf_7 { MEM_WIDTH 16 MEM_SIZE 16 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 1 }
dict set ap_memory_interface_dict col_outbuf { MEM_WIDTH 16 MEM_SIZE 128 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 1 }
set C_modelArgList {
	{ col_inbuf int 16 regular {array 8 { 1 3 } 1 1 }  }
	{ col_inbuf_1 int 16 regular {array 8 { 1 3 } 1 1 }  }
	{ col_inbuf_2 int 16 regular {array 8 { 1 3 } 1 1 }  }
	{ col_inbuf_3 int 16 regular {array 8 { 1 3 } 1 1 }  }
	{ col_inbuf_4 int 16 regular {array 8 { 1 3 } 1 1 }  }
	{ col_inbuf_5 int 16 regular {array 8 { 1 3 } 1 1 }  }
	{ col_inbuf_6 int 16 regular {array 8 { 1 3 } 1 1 }  }
	{ col_inbuf_7 int 16 regular {array 8 { 1 3 } 1 1 }  }
	{ col_outbuf int 16 regular {array 64 { 0 0 } 0 1 }  }
}
set hasAXIMCache 0
set l_AXIML2Cache [list]
set AXIMCacheInstDict [dict create]
set C_modelArgMapList {[ 
	{ "Name" : "col_inbuf", "interface" : "memory", "bitwidth" : 16, "direction" : "READONLY"} , 
 	{ "Name" : "col_inbuf_1", "interface" : "memory", "bitwidth" : 16, "direction" : "READONLY"} , 
 	{ "Name" : "col_inbuf_2", "interface" : "memory", "bitwidth" : 16, "direction" : "READONLY"} , 
 	{ "Name" : "col_inbuf_3", "interface" : "memory", "bitwidth" : 16, "direction" : "READONLY"} , 
 	{ "Name" : "col_inbuf_4", "interface" : "memory", "bitwidth" : 16, "direction" : "READONLY"} , 
 	{ "Name" : "col_inbuf_5", "interface" : "memory", "bitwidth" : 16, "direction" : "READONLY"} , 
 	{ "Name" : "col_inbuf_6", "interface" : "memory", "bitwidth" : 16, "direction" : "READONLY"} , 
 	{ "Name" : "col_inbuf_7", "interface" : "memory", "bitwidth" : 16, "direction" : "READONLY"} , 
 	{ "Name" : "col_outbuf", "interface" : "memory", "bitwidth" : 16, "direction" : "WRITEONLY"} ]}
# RTL Port declarations: 
set portNum 38
set portList { 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst sc_in sc_logic 1 reset -1 active_high_sync } 
	{ ap_start sc_in sc_logic 1 start -1 } 
	{ ap_done sc_out sc_logic 1 predone -1 } 
	{ ap_idle sc_out sc_logic 1 done -1 } 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ col_inbuf_address0 sc_out sc_lv 3 signal 0 } 
	{ col_inbuf_ce0 sc_out sc_logic 1 signal 0 } 
	{ col_inbuf_q0 sc_in sc_lv 16 signal 0 } 
	{ col_inbuf_1_address0 sc_out sc_lv 3 signal 1 } 
	{ col_inbuf_1_ce0 sc_out sc_logic 1 signal 1 } 
	{ col_inbuf_1_q0 sc_in sc_lv 16 signal 1 } 
	{ col_inbuf_2_address0 sc_out sc_lv 3 signal 2 } 
	{ col_inbuf_2_ce0 sc_out sc_logic 1 signal 2 } 
	{ col_inbuf_2_q0 sc_in sc_lv 16 signal 2 } 
	{ col_inbuf_3_address0 sc_out sc_lv 3 signal 3 } 
	{ col_inbuf_3_ce0 sc_out sc_logic 1 signal 3 } 
	{ col_inbuf_3_q0 sc_in sc_lv 16 signal 3 } 
	{ col_inbuf_4_address0 sc_out sc_lv 3 signal 4 } 
	{ col_inbuf_4_ce0 sc_out sc_logic 1 signal 4 } 
	{ col_inbuf_4_q0 sc_in sc_lv 16 signal 4 } 
	{ col_inbuf_5_address0 sc_out sc_lv 3 signal 5 } 
	{ col_inbuf_5_ce0 sc_out sc_logic 1 signal 5 } 
	{ col_inbuf_5_q0 sc_in sc_lv 16 signal 5 } 
	{ col_inbuf_6_address0 sc_out sc_lv 3 signal 6 } 
	{ col_inbuf_6_ce0 sc_out sc_logic 1 signal 6 } 
	{ col_inbuf_6_q0 sc_in sc_lv 16 signal 6 } 
	{ col_inbuf_7_address0 sc_out sc_lv 3 signal 7 } 
	{ col_inbuf_7_ce0 sc_out sc_logic 1 signal 7 } 
	{ col_inbuf_7_q0 sc_in sc_lv 16 signal 7 } 
	{ col_outbuf_address0 sc_out sc_lv 6 signal 8 } 
	{ col_outbuf_ce0 sc_out sc_logic 1 signal 8 } 
	{ col_outbuf_we0 sc_out sc_logic 1 signal 8 } 
	{ col_outbuf_d0 sc_out sc_lv 16 signal 8 } 
	{ col_outbuf_address1 sc_out sc_lv 6 signal 8 } 
	{ col_outbuf_ce1 sc_out sc_logic 1 signal 8 } 
	{ col_outbuf_we1 sc_out sc_logic 1 signal 8 } 
	{ col_outbuf_d1 sc_out sc_lv 16 signal 8 } 
}
set NewPortList {[ 
	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst", "role": "default" }} , 
 	{ "name": "ap_start", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "start", "bundle":{"name": "ap_start", "role": "default" }} , 
 	{ "name": "ap_done", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "predone", "bundle":{"name": "ap_done", "role": "default" }} , 
 	{ "name": "ap_idle", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "done", "bundle":{"name": "ap_idle", "role": "default" }} , 
 	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "col_inbuf_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "col_inbuf", "role": "address0" }} , 
 	{ "name": "col_inbuf_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "col_inbuf", "role": "ce0" }} , 
 	{ "name": "col_inbuf_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":16, "type": "signal", "bundle":{"name": "col_inbuf", "role": "q0" }} , 
 	{ "name": "col_inbuf_1_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "col_inbuf_1", "role": "address0" }} , 
 	{ "name": "col_inbuf_1_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "col_inbuf_1", "role": "ce0" }} , 
 	{ "name": "col_inbuf_1_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":16, "type": "signal", "bundle":{"name": "col_inbuf_1", "role": "q0" }} , 
 	{ "name": "col_inbuf_2_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "col_inbuf_2", "role": "address0" }} , 
 	{ "name": "col_inbuf_2_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "col_inbuf_2", "role": "ce0" }} , 
 	{ "name": "col_inbuf_2_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":16, "type": "signal", "bundle":{"name": "col_inbuf_2", "role": "q0" }} , 
 	{ "name": "col_inbuf_3_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "col_inbuf_3", "role": "address0" }} , 
 	{ "name": "col_inbuf_3_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "col_inbuf_3", "role": "ce0" }} , 
 	{ "name": "col_inbuf_3_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":16, "type": "signal", "bundle":{"name": "col_inbuf_3", "role": "q0" }} , 
 	{ "name": "col_inbuf_4_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "col_inbuf_4", "role": "address0" }} , 
 	{ "name": "col_inbuf_4_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "col_inbuf_4", "role": "ce0" }} , 
 	{ "name": "col_inbuf_4_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":16, "type": "signal", "bundle":{"name": "col_inbuf_4", "role": "q0" }} , 
 	{ "name": "col_inbuf_5_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "col_inbuf_5", "role": "address0" }} , 
 	{ "name": "col_inbuf_5_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "col_inbuf_5", "role": "ce0" }} , 
 	{ "name": "col_inbuf_5_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":16, "type": "signal", "bundle":{"name": "col_inbuf_5", "role": "q0" }} , 
 	{ "name": "col_inbuf_6_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "col_inbuf_6", "role": "address0" }} , 
 	{ "name": "col_inbuf_6_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "col_inbuf_6", "role": "ce0" }} , 
 	{ "name": "col_inbuf_6_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":16, "type": "signal", "bundle":{"name": "col_inbuf_6", "role": "q0" }} , 
 	{ "name": "col_inbuf_7_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":3, "type": "signal", "bundle":{"name": "col_inbuf_7", "role": "address0" }} , 
 	{ "name": "col_inbuf_7_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "col_inbuf_7", "role": "ce0" }} , 
 	{ "name": "col_inbuf_7_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":16, "type": "signal", "bundle":{"name": "col_inbuf_7", "role": "q0" }} , 
 	{ "name": "col_outbuf_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":6, "type": "signal", "bundle":{"name": "col_outbuf", "role": "address0" }} , 
 	{ "name": "col_outbuf_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "col_outbuf", "role": "ce0" }} , 
 	{ "name": "col_outbuf_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "col_outbuf", "role": "we0" }} , 
 	{ "name": "col_outbuf_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":16, "type": "signal", "bundle":{"name": "col_outbuf", "role": "d0" }} , 
 	{ "name": "col_outbuf_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":6, "type": "signal", "bundle":{"name": "col_outbuf", "role": "address1" }} , 
 	{ "name": "col_outbuf_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "col_outbuf", "role": "ce1" }} , 
 	{ "name": "col_outbuf_we1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "col_outbuf", "role": "we1" }} , 
 	{ "name": "col_outbuf_d1", "direction": "out", "datatype": "sc_lv", "bitwidth":16, "type": "signal", "bundle":{"name": "col_outbuf", "role": "d1" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1", "30"],
		"CDFG" : "dct_Pipeline_Col_DCT_Loop",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1", "real_start" : "0",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "41", "EstimateLatencyMax" : "41",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "col_inbuf", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "col_inbuf_1", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "col_inbuf_2", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "col_inbuf_3", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "col_inbuf_4", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "col_inbuf_5", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "col_inbuf_6", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "col_inbuf_7", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "col_outbuf", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "1", "SubInstance" : "grp_dct_1d_fu_154", "Port" : "dst", "Inst_start_state" : "3", "Inst_end_state" : "12"}]}],
		"Loop" : [
			{"Name" : "Col_DCT_Loop", "PipelineType" : "UPC",
				"LoopDec" : {"FSMBitwidth" : "4", "FirstState" : "ap_ST_fsm_pp0_stage0", "FirstStateIter" : "ap_enable_reg_pp0_iter0", "FirstStateBlock" : "ap_block_pp0_stage0_subdone", "LastState" : "ap_ST_fsm_pp0_stage3", "LastStateIter" : "ap_enable_reg_pp0_iter2", "LastStateBlock" : "ap_block_pp0_stage3_subdone", "QuitState" : "ap_ST_fsm_pp0_stage3", "QuitStateIter" : "ap_enable_reg_pp0_iter2", "QuitStateBlock" : "ap_block_pp0_stage3_subdone", "OneDepthLoop" : "0", "has_ap_ctrl" : "1", "has_continue" : "0"}}]},
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_dct_1d_fu_154", "Parent" : "0", "Child" : ["2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29"],
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
	{"ID" : "2", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_1d_fu_154.mul_16s_15ns_29_1_1_U10", "Parent" : "1"},
	{"ID" : "3", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_1d_fu_154.mul_16s_15s_29_1_1_U11", "Parent" : "1"},
	{"ID" : "4", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_1d_fu_154.mul_16s_15ns_29_1_1_U12", "Parent" : "1"},
	{"ID" : "5", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_1d_fu_154.mul_16s_15s_29_1_1_U13", "Parent" : "1"},
	{"ID" : "6", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_1d_fu_154.mul_16s_15s_29_1_1_U14", "Parent" : "1"},
	{"ID" : "7", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_1d_fu_154.mul_16s_15s_29_1_1_U15", "Parent" : "1"},
	{"ID" : "8", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_1d_fu_154.mul_16s_15ns_29_1_1_U16", "Parent" : "1"},
	{"ID" : "9", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_1d_fu_154.mul_16s_15s_29_1_1_U17", "Parent" : "1"},
	{"ID" : "10", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_1d_fu_154.mac_muladd_16s_15s_13ns_29_4_1_U18", "Parent" : "1"},
	{"ID" : "11", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_1d_fu_154.ama_submuladd_16s_16s_12ns_29s_29_4_1_U19", "Parent" : "1"},
	{"ID" : "12", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_1d_fu_154.mac_muladd_16s_14ns_29s_29_4_1_U20", "Parent" : "1"},
	{"ID" : "13", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_1d_fu_154.mac_muladd_16s_15s_29s_29_4_1_U21", "Parent" : "1"},
	{"ID" : "14", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_1d_fu_154.mac_muladd_16s_14ns_29s_29_4_1_U22", "Parent" : "1"},
	{"ID" : "15", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_1d_fu_154.mac_muladd_17s_13ns_29s_29_4_1_U23", "Parent" : "1"},
	{"ID" : "16", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_1d_fu_154.mac_muladd_18s_14ns_13ns_29_4_1_U24", "Parent" : "1"},
	{"ID" : "17", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_1d_fu_154.ama_submuladd_16s_16s_13ns_29s_29_4_1_U25", "Parent" : "1"},
	{"ID" : "18", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_1d_fu_154.mac_muladd_17s_12ns_13ns_29_4_1_U26", "Parent" : "1"},
	{"ID" : "19", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_1d_fu_154.mac_muladd_17s_13ns_13ns_29_4_1_U27", "Parent" : "1"},
	{"ID" : "20", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_1d_fu_154.mac_muladd_16s_14ns_29ns_29_4_1_U28", "Parent" : "1"},
	{"ID" : "21", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_1d_fu_154.mac_muladd_17s_12ns_29s_29_4_1_U29", "Parent" : "1"},
	{"ID" : "22", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_1d_fu_154.mac_muladd_18s_13ns_13ns_29_4_1_U30", "Parent" : "1"},
	{"ID" : "23", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_1d_fu_154.mac_muladd_17s_13ns_29s_29_4_1_U31", "Parent" : "1"},
	{"ID" : "24", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_1d_fu_154.mac_muladd_17s_12ns_13ns_29_4_1_U32", "Parent" : "1"},
	{"ID" : "25", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_1d_fu_154.ama_addmuladd_18s_16s_13ns_29ns_29_4_1_U33", "Parent" : "1"},
	{"ID" : "26", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_1d_fu_154.mac_muladd_16s_14ns_29ns_29_4_1_U34", "Parent" : "1"},
	{"ID" : "27", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_1d_fu_154.mac_muladd_16s_15s_29ns_29_4_1_U35", "Parent" : "1"},
	{"ID" : "28", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_1d_fu_154.ama_submuladd_18s_16s_14ns_29ns_29_4_1_U36", "Parent" : "1"},
	{"ID" : "29", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_1d_fu_154.mac_muladd_16s_14ns_29ns_29_4_1_U37", "Parent" : "1"},
	{"ID" : "30", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.flow_control_loop_pipe_sequential_init_U", "Parent" : "0"}]}


set ArgLastReadFirstWriteLatency {
	dct_Pipeline_Col_DCT_Loop {
		col_inbuf {Type I LastRead 0 FirstWrite -1}
		col_inbuf_1 {Type I LastRead 0 FirstWrite -1}
		col_inbuf_2 {Type I LastRead 0 FirstWrite -1}
		col_inbuf_3 {Type I LastRead 0 FirstWrite -1}
		col_inbuf_4 {Type I LastRead 0 FirstWrite -1}
		col_inbuf_5 {Type I LastRead 0 FirstWrite -1}
		col_inbuf_6 {Type I LastRead 0 FirstWrite -1}
		col_inbuf_7 {Type I LastRead 0 FirstWrite -1}
		col_outbuf {Type O LastRead -1 FirstWrite 1}}
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
	{"Name" : "Latency", "Min" : "41", "Max" : "41"}
	, {"Name" : "Interval", "Min" : "41", "Max" : "41"}
]}

set PipelineEnableSignalInfo {[
	{"Pipeline" : "0", "EnableSignal" : "ap_enable_pp0"}
]}

set Spec2ImplPortList { 
	col_inbuf { ap_memory {  { col_inbuf_address0 mem_address 1 3 }  { col_inbuf_ce0 mem_ce 1 1 }  { col_inbuf_q0 mem_dout 0 16 } } }
	col_inbuf_1 { ap_memory {  { col_inbuf_1_address0 mem_address 1 3 }  { col_inbuf_1_ce0 mem_ce 1 1 }  { col_inbuf_1_q0 mem_dout 0 16 } } }
	col_inbuf_2 { ap_memory {  { col_inbuf_2_address0 mem_address 1 3 }  { col_inbuf_2_ce0 mem_ce 1 1 }  { col_inbuf_2_q0 mem_dout 0 16 } } }
	col_inbuf_3 { ap_memory {  { col_inbuf_3_address0 mem_address 1 3 }  { col_inbuf_3_ce0 mem_ce 1 1 }  { col_inbuf_3_q0 mem_dout 0 16 } } }
	col_inbuf_4 { ap_memory {  { col_inbuf_4_address0 mem_address 1 3 }  { col_inbuf_4_ce0 mem_ce 1 1 }  { col_inbuf_4_q0 mem_dout 0 16 } } }
	col_inbuf_5 { ap_memory {  { col_inbuf_5_address0 mem_address 1 3 }  { col_inbuf_5_ce0 mem_ce 1 1 }  { col_inbuf_5_q0 mem_dout 0 16 } } }
	col_inbuf_6 { ap_memory {  { col_inbuf_6_address0 mem_address 1 3 }  { col_inbuf_6_ce0 mem_ce 1 1 }  { col_inbuf_6_q0 mem_dout 0 16 } } }
	col_inbuf_7 { ap_memory {  { col_inbuf_7_address0 mem_address 1 3 }  { col_inbuf_7_ce0 mem_ce 1 1 }  { col_inbuf_7_q0 mem_dout 0 16 } } }
	col_outbuf { ap_memory {  { col_outbuf_address0 mem_address 1 6 }  { col_outbuf_ce0 mem_ce 1 1 }  { col_outbuf_we0 mem_we 1 1 }  { col_outbuf_d0 mem_din 1 16 }  { col_outbuf_address1 MemPortADDR2 1 6 }  { col_outbuf_ce1 MemPortCE2 1 1 }  { col_outbuf_we1 MemPortWE2 1 1 }  { col_outbuf_d1 MemPortDIN2 1 16 } } }
}
