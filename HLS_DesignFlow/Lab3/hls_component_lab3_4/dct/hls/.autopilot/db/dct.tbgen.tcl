set moduleName dct
set isTopModule 1
set isCombinational 0
set isDatapathOnly 0
set isPipelined 0
set pipeline_type none
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
set C_modelName {dct}
set C_modelType { void 0 }
set ap_memory_interface_dict [dict create]
dict set ap_memory_interface_dict input_r { MEM_WIDTH 16 MEM_SIZE 128 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 1 }
dict set ap_memory_interface_dict output_r { MEM_WIDTH 16 MEM_SIZE 128 MASTER_TYPE BRAM_CTRL MEM_ADDRESS_MODE WORD_ADDRESS PACKAGE_IO port READ_LATENCY 1 }
set C_modelArgList {
	{ input_r int 16 regular {array 64 { 1 3 } 1 1 }  }
	{ output_r int 16 regular {array 64 { 0 3 } 0 1 }  }
}
set hasAXIMCache 0
set l_AXIML2Cache [list]
set AXIMCacheInstDict [dict create]
set C_modelArgMapList {[ 
	{ "Name" : "input_r", "interface" : "memory", "bitwidth" : 16, "direction" : "READONLY"} , 
 	{ "Name" : "output_r", "interface" : "memory", "bitwidth" : 16, "direction" : "WRITEONLY"} ]}
# RTL Port declarations: 
set portNum 13
set portList { 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst sc_in sc_logic 1 reset -1 active_high_sync } 
	{ ap_start sc_in sc_logic 1 start -1 } 
	{ ap_done sc_out sc_logic 1 predone -1 } 
	{ ap_idle sc_out sc_logic 1 done -1 } 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ input_r_address0 sc_out sc_lv 6 signal 0 } 
	{ input_r_ce0 sc_out sc_logic 1 signal 0 } 
	{ input_r_q0 sc_in sc_lv 16 signal 0 } 
	{ output_r_address0 sc_out sc_lv 6 signal 1 } 
	{ output_r_ce0 sc_out sc_logic 1 signal 1 } 
	{ output_r_we0 sc_out sc_logic 1 signal 1 } 
	{ output_r_d0 sc_out sc_lv 16 signal 1 } 
}
set NewPortList {[ 
	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst", "role": "default" }} , 
 	{ "name": "ap_start", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "start", "bundle":{"name": "ap_start", "role": "default" }} , 
 	{ "name": "ap_done", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "predone", "bundle":{"name": "ap_done", "role": "default" }} , 
 	{ "name": "ap_idle", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "done", "bundle":{"name": "ap_idle", "role": "default" }} , 
 	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "input_r_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":6, "type": "signal", "bundle":{"name": "input_r", "role": "address0" }} , 
 	{ "name": "input_r_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "input_r", "role": "ce0" }} , 
 	{ "name": "input_r_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":16, "type": "signal", "bundle":{"name": "input_r", "role": "q0" }} , 
 	{ "name": "output_r_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":6, "type": "signal", "bundle":{"name": "output_r", "role": "address0" }} , 
 	{ "name": "output_r_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "output_r", "role": "ce0" }} , 
 	{ "name": "output_r_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "output_r", "role": "we0" }} , 
 	{ "name": "output_r_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":16, "type": "signal", "bundle":{"name": "output_r", "role": "d0" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "22", "53", "55", "86", "88"],
		"CDFG" : "dct",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1", "real_start" : "0",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "365", "EstimateLatencyMax" : "365",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "input_r", "Type" : "Memory", "Direction" : "I",
				"SubConnect" : [
					{"ID" : "20", "SubInstance" : "grp_dct_Pipeline_RD_Loop_Row_RD_Loop_Col_fu_110", "Port" : "input_r", "Inst_start_state" : "1", "Inst_end_state" : "2"}]},
			{"Name" : "output_r", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "88", "SubInstance" : "grp_dct_Pipeline_WR_Loop_Row_WR_Loop_Col_fu_177", "Port" : "output_r", "Inst_start_state" : "11", "Inst_end_state" : "12"}]}]},
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.row_outbuf_U", "Parent" : "0"},
	{"ID" : "2", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.col_outbuf_U", "Parent" : "0"},
	{"ID" : "3", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.col_inbuf_U", "Parent" : "0"},
	{"ID" : "4", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.col_inbuf_1_U", "Parent" : "0"},
	{"ID" : "5", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.col_inbuf_2_U", "Parent" : "0"},
	{"ID" : "6", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.col_inbuf_3_U", "Parent" : "0"},
	{"ID" : "7", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.col_inbuf_4_U", "Parent" : "0"},
	{"ID" : "8", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.col_inbuf_5_U", "Parent" : "0"},
	{"ID" : "9", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.col_inbuf_6_U", "Parent" : "0"},
	{"ID" : "10", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.col_inbuf_7_U", "Parent" : "0"},
	{"ID" : "11", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.buf_2d_in_U", "Parent" : "0"},
	{"ID" : "12", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.buf_2d_in_8_U", "Parent" : "0"},
	{"ID" : "13", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.buf_2d_in_9_U", "Parent" : "0"},
	{"ID" : "14", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.buf_2d_in_10_U", "Parent" : "0"},
	{"ID" : "15", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.buf_2d_in_11_U", "Parent" : "0"},
	{"ID" : "16", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.buf_2d_in_12_U", "Parent" : "0"},
	{"ID" : "17", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.buf_2d_in_13_U", "Parent" : "0"},
	{"ID" : "18", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.buf_2d_in_14_U", "Parent" : "0"},
	{"ID" : "19", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.buf_2d_out_U", "Parent" : "0"},
	{"ID" : "20", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_RD_Loop_Row_RD_Loop_Col_fu_110", "Parent" : "0", "Child" : ["21"],
		"CDFG" : "dct_Pipeline_RD_Loop_Row_RD_Loop_Col",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1", "real_start" : "0",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "68", "EstimateLatencyMax" : "68",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "buf_2d_in", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buf_2d_in_8", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buf_2d_in_9", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buf_2d_in_10", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buf_2d_in_11", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buf_2d_in_12", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buf_2d_in_13", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "buf_2d_in_14", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "input_r", "Type" : "Memory", "Direction" : "I"}],
		"Loop" : [
			{"Name" : "RD_Loop_Row_RD_Loop_Col", "PipelineType" : "UPC",
				"LoopDec" : {"FSMBitwidth" : "1", "FirstState" : "ap_ST_fsm_pp0_stage0", "FirstStateIter" : "ap_enable_reg_pp0_iter0", "FirstStateBlock" : "ap_block_pp0_stage0_subdone", "LastState" : "ap_ST_fsm_pp0_stage0", "LastStateIter" : "ap_enable_reg_pp0_iter3", "LastStateBlock" : "ap_block_pp0_stage0_subdone", "QuitState" : "ap_ST_fsm_pp0_stage0", "QuitStateIter" : "ap_enable_reg_pp0_iter3", "QuitStateBlock" : "ap_block_pp0_stage0_subdone", "OneDepthLoop" : "0", "has_ap_ctrl" : "1", "has_continue" : "0"}}]},
	{"ID" : "21", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_RD_Loop_Row_RD_Loop_Col_fu_110.flow_control_loop_pipe_sequential_init_U", "Parent" : "20"},
	{"ID" : "22", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_132", "Parent" : "0", "Child" : ["23", "52"],
		"CDFG" : "dct_Pipeline_Row_DCT_Loop",
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
			{"Name" : "buf_2d_in", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "buf_2d_in_8", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "buf_2d_in_9", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "buf_2d_in_10", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "buf_2d_in_11", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "buf_2d_in_12", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "buf_2d_in_13", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "buf_2d_in_14", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "row_outbuf", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "23", "SubInstance" : "grp_dct_1d_fu_154", "Port" : "dst", "Inst_start_state" : "3", "Inst_end_state" : "12"}]}],
		"Loop" : [
			{"Name" : "Row_DCT_Loop", "PipelineType" : "UPC",
				"LoopDec" : {"FSMBitwidth" : "4", "FirstState" : "ap_ST_fsm_pp0_stage0", "FirstStateIter" : "ap_enable_reg_pp0_iter0", "FirstStateBlock" : "ap_block_pp0_stage0_subdone", "LastState" : "ap_ST_fsm_pp0_stage3", "LastStateIter" : "ap_enable_reg_pp0_iter2", "LastStateBlock" : "ap_block_pp0_stage3_subdone", "QuitState" : "ap_ST_fsm_pp0_stage3", "QuitStateIter" : "ap_enable_reg_pp0_iter2", "QuitStateBlock" : "ap_block_pp0_stage3_subdone", "OneDepthLoop" : "0", "has_ap_ctrl" : "1", "has_continue" : "0"}}]},
	{"ID" : "23", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_132.grp_dct_1d_fu_154", "Parent" : "22", "Child" : ["24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "50", "51"],
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
	{"ID" : "24", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_132.grp_dct_1d_fu_154.mul_16s_15ns_29_1_1_U10", "Parent" : "23"},
	{"ID" : "25", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_132.grp_dct_1d_fu_154.mul_16s_15s_29_1_1_U11", "Parent" : "23"},
	{"ID" : "26", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_132.grp_dct_1d_fu_154.mul_16s_15ns_29_1_1_U12", "Parent" : "23"},
	{"ID" : "27", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_132.grp_dct_1d_fu_154.mul_16s_15s_29_1_1_U13", "Parent" : "23"},
	{"ID" : "28", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_132.grp_dct_1d_fu_154.mul_16s_15s_29_1_1_U14", "Parent" : "23"},
	{"ID" : "29", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_132.grp_dct_1d_fu_154.mul_16s_15s_29_1_1_U15", "Parent" : "23"},
	{"ID" : "30", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_132.grp_dct_1d_fu_154.mul_16s_15ns_29_1_1_U16", "Parent" : "23"},
	{"ID" : "31", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_132.grp_dct_1d_fu_154.mul_16s_15s_29_1_1_U17", "Parent" : "23"},
	{"ID" : "32", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_132.grp_dct_1d_fu_154.mac_muladd_16s_15s_13ns_29_4_1_U18", "Parent" : "23"},
	{"ID" : "33", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_132.grp_dct_1d_fu_154.ama_submuladd_16s_16s_12ns_29s_29_4_1_U19", "Parent" : "23"},
	{"ID" : "34", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_132.grp_dct_1d_fu_154.mac_muladd_16s_14ns_29s_29_4_1_U20", "Parent" : "23"},
	{"ID" : "35", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_132.grp_dct_1d_fu_154.mac_muladd_16s_15s_29s_29_4_1_U21", "Parent" : "23"},
	{"ID" : "36", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_132.grp_dct_1d_fu_154.mac_muladd_16s_14ns_29s_29_4_1_U22", "Parent" : "23"},
	{"ID" : "37", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_132.grp_dct_1d_fu_154.mac_muladd_17s_13ns_29s_29_4_1_U23", "Parent" : "23"},
	{"ID" : "38", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_132.grp_dct_1d_fu_154.mac_muladd_18s_14ns_13ns_29_4_1_U24", "Parent" : "23"},
	{"ID" : "39", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_132.grp_dct_1d_fu_154.ama_submuladd_16s_16s_13ns_29s_29_4_1_U25", "Parent" : "23"},
	{"ID" : "40", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_132.grp_dct_1d_fu_154.mac_muladd_17s_12ns_13ns_29_4_1_U26", "Parent" : "23"},
	{"ID" : "41", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_132.grp_dct_1d_fu_154.mac_muladd_17s_13ns_13ns_29_4_1_U27", "Parent" : "23"},
	{"ID" : "42", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_132.grp_dct_1d_fu_154.mac_muladd_16s_14ns_29ns_29_4_1_U28", "Parent" : "23"},
	{"ID" : "43", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_132.grp_dct_1d_fu_154.mac_muladd_17s_12ns_29s_29_4_1_U29", "Parent" : "23"},
	{"ID" : "44", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_132.grp_dct_1d_fu_154.mac_muladd_18s_13ns_13ns_29_4_1_U30", "Parent" : "23"},
	{"ID" : "45", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_132.grp_dct_1d_fu_154.mac_muladd_17s_13ns_29s_29_4_1_U31", "Parent" : "23"},
	{"ID" : "46", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_132.grp_dct_1d_fu_154.mac_muladd_17s_12ns_13ns_29_4_1_U32", "Parent" : "23"},
	{"ID" : "47", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_132.grp_dct_1d_fu_154.ama_addmuladd_18s_16s_13ns_29ns_29_4_1_U33", "Parent" : "23"},
	{"ID" : "48", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_132.grp_dct_1d_fu_154.mac_muladd_16s_14ns_29ns_29_4_1_U34", "Parent" : "23"},
	{"ID" : "49", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_132.grp_dct_1d_fu_154.mac_muladd_16s_15s_29ns_29_4_1_U35", "Parent" : "23"},
	{"ID" : "50", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_132.grp_dct_1d_fu_154.ama_submuladd_18s_16s_14ns_29ns_29_4_1_U36", "Parent" : "23"},
	{"ID" : "51", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_132.grp_dct_1d_fu_154.mac_muladd_16s_14ns_29ns_29_4_1_U37", "Parent" : "23"},
	{"ID" : "52", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_132.flow_control_loop_pipe_sequential_init_U", "Parent" : "22"},
	{"ID" : "53", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Xpose_Row_Outer_Loop_Xpose_Row_Inner_Loop_fu_145", "Parent" : "0", "Child" : ["54"],
		"CDFG" : "dct_Pipeline_Xpose_Row_Outer_Loop_Xpose_Row_Inner_Loop",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1", "real_start" : "0",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "68", "EstimateLatencyMax" : "68",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "col_inbuf", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "col_inbuf_1", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "col_inbuf_2", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "col_inbuf_3", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "col_inbuf_4", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "col_inbuf_5", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "col_inbuf_6", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "col_inbuf_7", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "row_outbuf", "Type" : "Memory", "Direction" : "I"}],
		"Loop" : [
			{"Name" : "Xpose_Row_Outer_Loop_Xpose_Row_Inner_Loop", "PipelineType" : "UPC",
				"LoopDec" : {"FSMBitwidth" : "1", "FirstState" : "ap_ST_fsm_pp0_stage0", "FirstStateIter" : "ap_enable_reg_pp0_iter0", "FirstStateBlock" : "ap_block_pp0_stage0_subdone", "LastState" : "ap_ST_fsm_pp0_stage0", "LastStateIter" : "ap_enable_reg_pp0_iter3", "LastStateBlock" : "ap_block_pp0_stage0_subdone", "QuitState" : "ap_ST_fsm_pp0_stage0", "QuitStateIter" : "ap_enable_reg_pp0_iter3", "QuitStateBlock" : "ap_block_pp0_stage0_subdone", "OneDepthLoop" : "0", "has_ap_ctrl" : "1", "has_continue" : "0"}}]},
	{"ID" : "54", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Xpose_Row_Outer_Loop_Xpose_Row_Inner_Loop_fu_145.flow_control_loop_pipe_sequential_init_U", "Parent" : "53"},
	{"ID" : "55", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_158", "Parent" : "0", "Child" : ["56", "85"],
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
					{"ID" : "56", "SubInstance" : "grp_dct_1d_fu_154", "Port" : "dst", "Inst_start_state" : "3", "Inst_end_state" : "12"}]}],
		"Loop" : [
			{"Name" : "Col_DCT_Loop", "PipelineType" : "UPC",
				"LoopDec" : {"FSMBitwidth" : "4", "FirstState" : "ap_ST_fsm_pp0_stage0", "FirstStateIter" : "ap_enable_reg_pp0_iter0", "FirstStateBlock" : "ap_block_pp0_stage0_subdone", "LastState" : "ap_ST_fsm_pp0_stage3", "LastStateIter" : "ap_enable_reg_pp0_iter2", "LastStateBlock" : "ap_block_pp0_stage3_subdone", "QuitState" : "ap_ST_fsm_pp0_stage3", "QuitStateIter" : "ap_enable_reg_pp0_iter2", "QuitStateBlock" : "ap_block_pp0_stage3_subdone", "OneDepthLoop" : "0", "has_ap_ctrl" : "1", "has_continue" : "0"}}]},
	{"ID" : "56", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_158.grp_dct_1d_fu_154", "Parent" : "55", "Child" : ["57", "58", "59", "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "80", "81", "82", "83", "84"],
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
	{"ID" : "57", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_158.grp_dct_1d_fu_154.mul_16s_15ns_29_1_1_U10", "Parent" : "56"},
	{"ID" : "58", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_158.grp_dct_1d_fu_154.mul_16s_15s_29_1_1_U11", "Parent" : "56"},
	{"ID" : "59", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_158.grp_dct_1d_fu_154.mul_16s_15ns_29_1_1_U12", "Parent" : "56"},
	{"ID" : "60", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_158.grp_dct_1d_fu_154.mul_16s_15s_29_1_1_U13", "Parent" : "56"},
	{"ID" : "61", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_158.grp_dct_1d_fu_154.mul_16s_15s_29_1_1_U14", "Parent" : "56"},
	{"ID" : "62", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_158.grp_dct_1d_fu_154.mul_16s_15s_29_1_1_U15", "Parent" : "56"},
	{"ID" : "63", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_158.grp_dct_1d_fu_154.mul_16s_15ns_29_1_1_U16", "Parent" : "56"},
	{"ID" : "64", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_158.grp_dct_1d_fu_154.mul_16s_15s_29_1_1_U17", "Parent" : "56"},
	{"ID" : "65", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_158.grp_dct_1d_fu_154.mac_muladd_16s_15s_13ns_29_4_1_U18", "Parent" : "56"},
	{"ID" : "66", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_158.grp_dct_1d_fu_154.ama_submuladd_16s_16s_12ns_29s_29_4_1_U19", "Parent" : "56"},
	{"ID" : "67", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_158.grp_dct_1d_fu_154.mac_muladd_16s_14ns_29s_29_4_1_U20", "Parent" : "56"},
	{"ID" : "68", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_158.grp_dct_1d_fu_154.mac_muladd_16s_15s_29s_29_4_1_U21", "Parent" : "56"},
	{"ID" : "69", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_158.grp_dct_1d_fu_154.mac_muladd_16s_14ns_29s_29_4_1_U22", "Parent" : "56"},
	{"ID" : "70", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_158.grp_dct_1d_fu_154.mac_muladd_17s_13ns_29s_29_4_1_U23", "Parent" : "56"},
	{"ID" : "71", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_158.grp_dct_1d_fu_154.mac_muladd_18s_14ns_13ns_29_4_1_U24", "Parent" : "56"},
	{"ID" : "72", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_158.grp_dct_1d_fu_154.ama_submuladd_16s_16s_13ns_29s_29_4_1_U25", "Parent" : "56"},
	{"ID" : "73", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_158.grp_dct_1d_fu_154.mac_muladd_17s_12ns_13ns_29_4_1_U26", "Parent" : "56"},
	{"ID" : "74", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_158.grp_dct_1d_fu_154.mac_muladd_17s_13ns_13ns_29_4_1_U27", "Parent" : "56"},
	{"ID" : "75", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_158.grp_dct_1d_fu_154.mac_muladd_16s_14ns_29ns_29_4_1_U28", "Parent" : "56"},
	{"ID" : "76", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_158.grp_dct_1d_fu_154.mac_muladd_17s_12ns_29s_29_4_1_U29", "Parent" : "56"},
	{"ID" : "77", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_158.grp_dct_1d_fu_154.mac_muladd_18s_13ns_13ns_29_4_1_U30", "Parent" : "56"},
	{"ID" : "78", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_158.grp_dct_1d_fu_154.mac_muladd_17s_13ns_29s_29_4_1_U31", "Parent" : "56"},
	{"ID" : "79", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_158.grp_dct_1d_fu_154.mac_muladd_17s_12ns_13ns_29_4_1_U32", "Parent" : "56"},
	{"ID" : "80", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_158.grp_dct_1d_fu_154.ama_addmuladd_18s_16s_13ns_29ns_29_4_1_U33", "Parent" : "56"},
	{"ID" : "81", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_158.grp_dct_1d_fu_154.mac_muladd_16s_14ns_29ns_29_4_1_U34", "Parent" : "56"},
	{"ID" : "82", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_158.grp_dct_1d_fu_154.mac_muladd_16s_15s_29ns_29_4_1_U35", "Parent" : "56"},
	{"ID" : "83", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_158.grp_dct_1d_fu_154.ama_submuladd_18s_16s_14ns_29ns_29_4_1_U36", "Parent" : "56"},
	{"ID" : "84", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_158.grp_dct_1d_fu_154.mac_muladd_16s_14ns_29ns_29_4_1_U37", "Parent" : "56"},
	{"ID" : "85", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_158.flow_control_loop_pipe_sequential_init_U", "Parent" : "55"},
	{"ID" : "86", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Xpose_Col_Outer_Loop_Xpose_Col_Inner_Loop_fu_171", "Parent" : "0", "Child" : ["87"],
		"CDFG" : "dct_Pipeline_Xpose_Col_Outer_Loop_Xpose_Col_Inner_Loop",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1", "real_start" : "0",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "68", "EstimateLatencyMax" : "68",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "col_outbuf", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "buf_2d_out", "Type" : "Memory", "Direction" : "O"}],
		"Loop" : [
			{"Name" : "Xpose_Col_Outer_Loop_Xpose_Col_Inner_Loop", "PipelineType" : "UPC",
				"LoopDec" : {"FSMBitwidth" : "1", "FirstState" : "ap_ST_fsm_pp0_stage0", "FirstStateIter" : "ap_enable_reg_pp0_iter0", "FirstStateBlock" : "ap_block_pp0_stage0_subdone", "LastState" : "ap_ST_fsm_pp0_stage0", "LastStateIter" : "ap_enable_reg_pp0_iter3", "LastStateBlock" : "ap_block_pp0_stage0_subdone", "QuitState" : "ap_ST_fsm_pp0_stage0", "QuitStateIter" : "ap_enable_reg_pp0_iter3", "QuitStateBlock" : "ap_block_pp0_stage0_subdone", "OneDepthLoop" : "0", "has_ap_ctrl" : "1", "has_continue" : "0"}}]},
	{"ID" : "87", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Xpose_Col_Outer_Loop_Xpose_Col_Inner_Loop_fu_171.flow_control_loop_pipe_sequential_init_U", "Parent" : "86"},
	{"ID" : "88", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_WR_Loop_Row_WR_Loop_Col_fu_177", "Parent" : "0", "Child" : ["89"],
		"CDFG" : "dct_Pipeline_WR_Loop_Row_WR_Loop_Col",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1", "real_start" : "0",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "68", "EstimateLatencyMax" : "68",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "buf_2d_out", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "output_r", "Type" : "Memory", "Direction" : "O"}],
		"Loop" : [
			{"Name" : "WR_Loop_Row_WR_Loop_Col", "PipelineType" : "UPC",
				"LoopDec" : {"FSMBitwidth" : "1", "FirstState" : "ap_ST_fsm_pp0_stage0", "FirstStateIter" : "ap_enable_reg_pp0_iter0", "FirstStateBlock" : "ap_block_pp0_stage0_subdone", "LastState" : "ap_ST_fsm_pp0_stage0", "LastStateIter" : "ap_enable_reg_pp0_iter3", "LastStateBlock" : "ap_block_pp0_stage0_subdone", "QuitState" : "ap_ST_fsm_pp0_stage0", "QuitStateIter" : "ap_enable_reg_pp0_iter3", "QuitStateBlock" : "ap_block_pp0_stage0_subdone", "OneDepthLoop" : "0", "has_ap_ctrl" : "1", "has_continue" : "0"}}]},
	{"ID" : "89", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_WR_Loop_Row_WR_Loop_Col_fu_177.flow_control_loop_pipe_sequential_init_U", "Parent" : "88"}]}


set ArgLastReadFirstWriteLatency {
	dct {
		input_r {Type I LastRead 2 FirstWrite -1}
		output_r {Type O LastRead -1 FirstWrite 3}}
	dct_Pipeline_RD_Loop_Row_RD_Loop_Col {
		buf_2d_in {Type O LastRead -1 FirstWrite 3}
		buf_2d_in_8 {Type O LastRead -1 FirstWrite 3}
		buf_2d_in_9 {Type O LastRead -1 FirstWrite 3}
		buf_2d_in_10 {Type O LastRead -1 FirstWrite 3}
		buf_2d_in_11 {Type O LastRead -1 FirstWrite 3}
		buf_2d_in_12 {Type O LastRead -1 FirstWrite 3}
		buf_2d_in_13 {Type O LastRead -1 FirstWrite 3}
		buf_2d_in_14 {Type O LastRead -1 FirstWrite 3}
		input_r {Type I LastRead 2 FirstWrite -1}}
	dct_Pipeline_Row_DCT_Loop {
		buf_2d_in {Type I LastRead 0 FirstWrite -1}
		buf_2d_in_8 {Type I LastRead 0 FirstWrite -1}
		buf_2d_in_9 {Type I LastRead 0 FirstWrite -1}
		buf_2d_in_10 {Type I LastRead 0 FirstWrite -1}
		buf_2d_in_11 {Type I LastRead 0 FirstWrite -1}
		buf_2d_in_12 {Type I LastRead 0 FirstWrite -1}
		buf_2d_in_13 {Type I LastRead 0 FirstWrite -1}
		buf_2d_in_14 {Type I LastRead 0 FirstWrite -1}
		row_outbuf {Type O LastRead -1 FirstWrite 1}}
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
		dst_offset {Type I LastRead 1 FirstWrite -1}}
	dct_Pipeline_Xpose_Row_Outer_Loop_Xpose_Row_Inner_Loop {
		col_inbuf {Type O LastRead -1 FirstWrite 3}
		col_inbuf_1 {Type O LastRead -1 FirstWrite 3}
		col_inbuf_2 {Type O LastRead -1 FirstWrite 3}
		col_inbuf_3 {Type O LastRead -1 FirstWrite 3}
		col_inbuf_4 {Type O LastRead -1 FirstWrite 3}
		col_inbuf_5 {Type O LastRead -1 FirstWrite 3}
		col_inbuf_6 {Type O LastRead -1 FirstWrite 3}
		col_inbuf_7 {Type O LastRead -1 FirstWrite 3}
		row_outbuf {Type I LastRead 2 FirstWrite -1}}
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
		dst_offset {Type I LastRead 1 FirstWrite -1}}
	dct_Pipeline_Xpose_Col_Outer_Loop_Xpose_Col_Inner_Loop {
		col_outbuf {Type I LastRead 2 FirstWrite -1}
		buf_2d_out {Type O LastRead -1 FirstWrite 3}}
	dct_Pipeline_WR_Loop_Row_WR_Loop_Col {
		buf_2d_out {Type I LastRead 2 FirstWrite -1}
		output_r {Type O LastRead -1 FirstWrite 3}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "365", "Max" : "365"}
	, {"Name" : "Interval", "Min" : "366", "Max" : "366"}
]}

set PipelineEnableSignalInfo {[
]}

set Spec2ImplPortList { 
	input_r { ap_memory {  { input_r_address0 mem_address 1 6 }  { input_r_ce0 mem_ce 1 1 }  { input_r_q0 mem_dout 0 16 } } }
	output_r { ap_memory {  { output_r_address0 mem_address 1 6 }  { output_r_ce0 mem_ce 1 1 }  { output_r_we0 mem_we 1 1 }  { output_r_d0 mem_din 1 16 } } }
}

set maxi_interface_dict [dict create]

# RTL port scheduling information:
set fifoSchedulingInfoList { 
}

# RTL bus port read request latency information:
set busReadReqLatencyList { 
}

# RTL bus port write response latency information:
set busWriteResLatencyList { 
}

# RTL array port load latency information:
set memoryLoadLatencyList { 
}
