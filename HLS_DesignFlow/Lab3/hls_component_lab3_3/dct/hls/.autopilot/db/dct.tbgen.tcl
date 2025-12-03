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
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1", "2", "3", "4", "5", "6", "8", "39", "41", "72", "74"],
		"CDFG" : "dct",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1", "real_start" : "0",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "377", "EstimateLatencyMax" : "377",
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
					{"ID" : "6", "SubInstance" : "grp_dct_Pipeline_RD_Loop_Row_RD_Loop_Col_fu_54", "Port" : "input_r", "Inst_start_state" : "1", "Inst_end_state" : "2"}]},
			{"Name" : "output_r", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "74", "SubInstance" : "grp_dct_Pipeline_WR_Loop_Row_WR_Loop_Col_fu_86", "Port" : "output_r", "Inst_start_state" : "11", "Inst_end_state" : "12"}]}]},
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.row_outbuf_U", "Parent" : "0"},
	{"ID" : "2", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.col_outbuf_U", "Parent" : "0"},
	{"ID" : "3", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.col_inbuf_U", "Parent" : "0"},
	{"ID" : "4", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.buf_2d_in_U", "Parent" : "0"},
	{"ID" : "5", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.buf_2d_out_U", "Parent" : "0"},
	{"ID" : "6", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_RD_Loop_Row_RD_Loop_Col_fu_54", "Parent" : "0", "Child" : ["7"],
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
			{"Name" : "input_r", "Type" : "Memory", "Direction" : "I"}],
		"Loop" : [
			{"Name" : "RD_Loop_Row_RD_Loop_Col", "PipelineType" : "UPC",
				"LoopDec" : {"FSMBitwidth" : "1", "FirstState" : "ap_ST_fsm_pp0_stage0", "FirstStateIter" : "ap_enable_reg_pp0_iter0", "FirstStateBlock" : "ap_block_pp0_stage0_subdone", "LastState" : "ap_ST_fsm_pp0_stage0", "LastStateIter" : "ap_enable_reg_pp0_iter3", "LastStateBlock" : "ap_block_pp0_stage0_subdone", "QuitState" : "ap_ST_fsm_pp0_stage0", "QuitStateIter" : "ap_enable_reg_pp0_iter3", "QuitStateBlock" : "ap_block_pp0_stage0_subdone", "OneDepthLoop" : "0", "has_ap_ctrl" : "1", "has_continue" : "0"}}]},
	{"ID" : "7", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_RD_Loop_Row_RD_Loop_Col_fu_54.flow_control_loop_pipe_sequential_init_U", "Parent" : "6"},
	{"ID" : "8", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_62", "Parent" : "0", "Child" : ["9", "38"],
		"CDFG" : "dct_Pipeline_Row_DCT_Loop",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1", "real_start" : "0",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "47", "EstimateLatencyMax" : "47",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "buf_2d_in", "Type" : "Memory", "Direction" : "I",
				"SubConnect" : [
					{"ID" : "9", "SubInstance" : "grp_dct_1d_fu_34", "Port" : "src", "Inst_start_state" : "2", "Inst_end_state" : "18"}]},
			{"Name" : "row_outbuf", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "9", "SubInstance" : "grp_dct_1d_fu_34", "Port" : "dst", "Inst_start_state" : "2", "Inst_end_state" : "18"}]}],
		"Loop" : [
			{"Name" : "Row_DCT_Loop", "PipelineType" : "UPC",
				"LoopDec" : {"FSMBitwidth" : "4", "FirstState" : "ap_ST_fsm_pp0_stage0", "FirstStateIter" : "ap_enable_reg_pp0_iter0", "FirstStateBlock" : "ap_block_pp0_stage0_subdone", "LastState" : "ap_ST_fsm_pp0_stage1", "LastStateIter" : "ap_enable_reg_pp0_iter4", "LastStateBlock" : "ap_block_pp0_stage1_subdone", "QuitState" : "ap_ST_fsm_pp0_stage1", "QuitStateIter" : "ap_enable_reg_pp0_iter4", "QuitStateBlock" : "ap_block_pp0_stage1_subdone", "OneDepthLoop" : "0", "has_ap_ctrl" : "1", "has_continue" : "0"}}]},
	{"ID" : "9", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_62.grp_dct_1d_fu_34", "Parent" : "8", "Child" : ["10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36", "37"],
		"CDFG" : "dct_1d",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1", "real_start" : "0",
		"Pipeline" : "Aligned", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "4",
		"VariableLatency" : "0", "ExactLatency" : "16", "EstimateLatencyMin" : "16", "EstimateLatencyMax" : "16",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "src", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "src_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "dst", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "dst_offset", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "10", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_62.grp_dct_1d_fu_34.mul_16s_15ns_29_1_1_U3", "Parent" : "9"},
	{"ID" : "11", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_62.grp_dct_1d_fu_34.mul_16s_15s_29_1_1_U4", "Parent" : "9"},
	{"ID" : "12", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_62.grp_dct_1d_fu_34.mul_16s_15ns_29_1_1_U5", "Parent" : "9"},
	{"ID" : "13", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_62.grp_dct_1d_fu_34.mul_16s_15s_29_1_1_U6", "Parent" : "9"},
	{"ID" : "14", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_62.grp_dct_1d_fu_34.mul_17s_14ns_29_1_1_U7", "Parent" : "9"},
	{"ID" : "15", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_62.grp_dct_1d_fu_34.mul_16s_15ns_29_1_1_U8", "Parent" : "9"},
	{"ID" : "16", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_62.grp_dct_1d_fu_34.mul_17s_13ns_29_1_1_U9", "Parent" : "9"},
	{"ID" : "17", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_62.grp_dct_1d_fu_34.mul_16s_15s_29_1_1_U10", "Parent" : "9"},
	{"ID" : "18", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_62.grp_dct_1d_fu_34.mac_muladd_16s_15s_13ns_29_4_1_U11", "Parent" : "9"},
	{"ID" : "19", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_62.grp_dct_1d_fu_34.ama_submuladd_16s_16s_12ns_29s_29_4_1_U12", "Parent" : "9"},
	{"ID" : "20", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_62.grp_dct_1d_fu_34.ama_submuladd_16s_16s_13ns_29s_29_4_1_U13", "Parent" : "9"},
	{"ID" : "21", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_62.grp_dct_1d_fu_34.mac_muladd_16s_15s_29s_29_4_1_U14", "Parent" : "9"},
	{"ID" : "22", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_62.grp_dct_1d_fu_34.mac_muladd_16s_14ns_29s_29_4_1_U15", "Parent" : "9"},
	{"ID" : "23", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_62.grp_dct_1d_fu_34.mac_muladd_16s_15s_29s_29_4_1_U16", "Parent" : "9"},
	{"ID" : "24", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_62.grp_dct_1d_fu_34.mac_muladd_16s_15s_13ns_29_4_1_U17", "Parent" : "9"},
	{"ID" : "25", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_62.grp_dct_1d_fu_34.mac_muladd_17s_12ns_29s_29_4_1_U18", "Parent" : "9"},
	{"ID" : "26", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_62.grp_dct_1d_fu_34.mac_muladd_17s_13ns_13ns_29_4_1_U19", "Parent" : "9"},
	{"ID" : "27", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_62.grp_dct_1d_fu_34.mac_muladd_16s_14ns_29ns_29_4_1_U20", "Parent" : "9"},
	{"ID" : "28", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_62.grp_dct_1d_fu_34.mac_muladd_18s_14ns_13ns_29_4_1_U21", "Parent" : "9"},
	{"ID" : "29", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_62.grp_dct_1d_fu_34.mac_muladd_16s_14ns_29ns_29_4_1_U22", "Parent" : "9"},
	{"ID" : "30", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_62.grp_dct_1d_fu_34.mac_muladd_16s_14ns_29s_29_4_1_U23", "Parent" : "9"},
	{"ID" : "31", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_62.grp_dct_1d_fu_34.mac_muladd_17s_12ns_13ns_29_4_1_U24", "Parent" : "9"},
	{"ID" : "32", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_62.grp_dct_1d_fu_34.ama_addmuladd_18s_16s_13ns_29ns_29_4_1_U25", "Parent" : "9"},
	{"ID" : "33", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_62.grp_dct_1d_fu_34.mac_muladd_16s_15s_29ns_29_4_1_U26", "Parent" : "9"},
	{"ID" : "34", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_62.grp_dct_1d_fu_34.mac_muladd_18s_13ns_13ns_29_4_1_U27", "Parent" : "9"},
	{"ID" : "35", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_62.grp_dct_1d_fu_34.mac_muladd_16s_14ns_29ns_29_4_1_U28", "Parent" : "9"},
	{"ID" : "36", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_62.grp_dct_1d_fu_34.mac_muladd_17s_13ns_29s_29_4_1_U29", "Parent" : "9"},
	{"ID" : "37", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_62.grp_dct_1d_fu_34.ama_submuladd_18s_16s_14ns_29ns_29_4_1_U30", "Parent" : "9"},
	{"ID" : "38", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Row_DCT_Loop_fu_62.flow_control_loop_pipe_sequential_init_U", "Parent" : "8"},
	{"ID" : "39", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Xpose_Row_Outer_Loop_Xpose_Row_Inner_Loop_fu_68", "Parent" : "0", "Child" : ["40"],
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
			{"Name" : "row_outbuf", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "col_inbuf", "Type" : "Memory", "Direction" : "O"}],
		"Loop" : [
			{"Name" : "Xpose_Row_Outer_Loop_Xpose_Row_Inner_Loop", "PipelineType" : "UPC",
				"LoopDec" : {"FSMBitwidth" : "1", "FirstState" : "ap_ST_fsm_pp0_stage0", "FirstStateIter" : "ap_enable_reg_pp0_iter0", "FirstStateBlock" : "ap_block_pp0_stage0_subdone", "LastState" : "ap_ST_fsm_pp0_stage0", "LastStateIter" : "ap_enable_reg_pp0_iter3", "LastStateBlock" : "ap_block_pp0_stage0_subdone", "QuitState" : "ap_ST_fsm_pp0_stage0", "QuitStateIter" : "ap_enable_reg_pp0_iter3", "QuitStateBlock" : "ap_block_pp0_stage0_subdone", "OneDepthLoop" : "0", "has_ap_ctrl" : "1", "has_continue" : "0"}}]},
	{"ID" : "40", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Xpose_Row_Outer_Loop_Xpose_Row_Inner_Loop_fu_68.flow_control_loop_pipe_sequential_init_U", "Parent" : "39"},
	{"ID" : "41", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_74", "Parent" : "0", "Child" : ["42", "71"],
		"CDFG" : "dct_Pipeline_Col_DCT_Loop",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1", "real_start" : "0",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "47", "EstimateLatencyMax" : "47",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "col_inbuf", "Type" : "Memory", "Direction" : "I",
				"SubConnect" : [
					{"ID" : "42", "SubInstance" : "grp_dct_1d_fu_34", "Port" : "src", "Inst_start_state" : "2", "Inst_end_state" : "18"}]},
			{"Name" : "col_outbuf", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "42", "SubInstance" : "grp_dct_1d_fu_34", "Port" : "dst", "Inst_start_state" : "2", "Inst_end_state" : "18"}]}],
		"Loop" : [
			{"Name" : "Col_DCT_Loop", "PipelineType" : "UPC",
				"LoopDec" : {"FSMBitwidth" : "4", "FirstState" : "ap_ST_fsm_pp0_stage0", "FirstStateIter" : "ap_enable_reg_pp0_iter0", "FirstStateBlock" : "ap_block_pp0_stage0_subdone", "LastState" : "ap_ST_fsm_pp0_stage1", "LastStateIter" : "ap_enable_reg_pp0_iter4", "LastStateBlock" : "ap_block_pp0_stage1_subdone", "QuitState" : "ap_ST_fsm_pp0_stage1", "QuitStateIter" : "ap_enable_reg_pp0_iter4", "QuitStateBlock" : "ap_block_pp0_stage1_subdone", "OneDepthLoop" : "0", "has_ap_ctrl" : "1", "has_continue" : "0"}}]},
	{"ID" : "42", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_74.grp_dct_1d_fu_34", "Parent" : "41", "Child" : ["43", "44", "45", "46", "47", "48", "49", "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "70"],
		"CDFG" : "dct_1d",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1", "real_start" : "0",
		"Pipeline" : "Aligned", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "4",
		"VariableLatency" : "0", "ExactLatency" : "16", "EstimateLatencyMin" : "16", "EstimateLatencyMax" : "16",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "src", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "src_offset", "Type" : "None", "Direction" : "I"},
			{"Name" : "dst", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "dst_offset", "Type" : "None", "Direction" : "I"}]},
	{"ID" : "43", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_74.grp_dct_1d_fu_34.mul_16s_15ns_29_1_1_U3", "Parent" : "42"},
	{"ID" : "44", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_74.grp_dct_1d_fu_34.mul_16s_15s_29_1_1_U4", "Parent" : "42"},
	{"ID" : "45", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_74.grp_dct_1d_fu_34.mul_16s_15ns_29_1_1_U5", "Parent" : "42"},
	{"ID" : "46", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_74.grp_dct_1d_fu_34.mul_16s_15s_29_1_1_U6", "Parent" : "42"},
	{"ID" : "47", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_74.grp_dct_1d_fu_34.mul_17s_14ns_29_1_1_U7", "Parent" : "42"},
	{"ID" : "48", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_74.grp_dct_1d_fu_34.mul_16s_15ns_29_1_1_U8", "Parent" : "42"},
	{"ID" : "49", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_74.grp_dct_1d_fu_34.mul_17s_13ns_29_1_1_U9", "Parent" : "42"},
	{"ID" : "50", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_74.grp_dct_1d_fu_34.mul_16s_15s_29_1_1_U10", "Parent" : "42"},
	{"ID" : "51", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_74.grp_dct_1d_fu_34.mac_muladd_16s_15s_13ns_29_4_1_U11", "Parent" : "42"},
	{"ID" : "52", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_74.grp_dct_1d_fu_34.ama_submuladd_16s_16s_12ns_29s_29_4_1_U12", "Parent" : "42"},
	{"ID" : "53", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_74.grp_dct_1d_fu_34.ama_submuladd_16s_16s_13ns_29s_29_4_1_U13", "Parent" : "42"},
	{"ID" : "54", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_74.grp_dct_1d_fu_34.mac_muladd_16s_15s_29s_29_4_1_U14", "Parent" : "42"},
	{"ID" : "55", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_74.grp_dct_1d_fu_34.mac_muladd_16s_14ns_29s_29_4_1_U15", "Parent" : "42"},
	{"ID" : "56", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_74.grp_dct_1d_fu_34.mac_muladd_16s_15s_29s_29_4_1_U16", "Parent" : "42"},
	{"ID" : "57", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_74.grp_dct_1d_fu_34.mac_muladd_16s_15s_13ns_29_4_1_U17", "Parent" : "42"},
	{"ID" : "58", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_74.grp_dct_1d_fu_34.mac_muladd_17s_12ns_29s_29_4_1_U18", "Parent" : "42"},
	{"ID" : "59", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_74.grp_dct_1d_fu_34.mac_muladd_17s_13ns_13ns_29_4_1_U19", "Parent" : "42"},
	{"ID" : "60", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_74.grp_dct_1d_fu_34.mac_muladd_16s_14ns_29ns_29_4_1_U20", "Parent" : "42"},
	{"ID" : "61", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_74.grp_dct_1d_fu_34.mac_muladd_18s_14ns_13ns_29_4_1_U21", "Parent" : "42"},
	{"ID" : "62", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_74.grp_dct_1d_fu_34.mac_muladd_16s_14ns_29ns_29_4_1_U22", "Parent" : "42"},
	{"ID" : "63", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_74.grp_dct_1d_fu_34.mac_muladd_16s_14ns_29s_29_4_1_U23", "Parent" : "42"},
	{"ID" : "64", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_74.grp_dct_1d_fu_34.mac_muladd_17s_12ns_13ns_29_4_1_U24", "Parent" : "42"},
	{"ID" : "65", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_74.grp_dct_1d_fu_34.ama_addmuladd_18s_16s_13ns_29ns_29_4_1_U25", "Parent" : "42"},
	{"ID" : "66", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_74.grp_dct_1d_fu_34.mac_muladd_16s_15s_29ns_29_4_1_U26", "Parent" : "42"},
	{"ID" : "67", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_74.grp_dct_1d_fu_34.mac_muladd_18s_13ns_13ns_29_4_1_U27", "Parent" : "42"},
	{"ID" : "68", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_74.grp_dct_1d_fu_34.mac_muladd_16s_14ns_29ns_29_4_1_U28", "Parent" : "42"},
	{"ID" : "69", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_74.grp_dct_1d_fu_34.mac_muladd_17s_13ns_29s_29_4_1_U29", "Parent" : "42"},
	{"ID" : "70", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_74.grp_dct_1d_fu_34.ama_submuladd_18s_16s_14ns_29ns_29_4_1_U30", "Parent" : "42"},
	{"ID" : "71", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Col_DCT_Loop_fu_74.flow_control_loop_pipe_sequential_init_U", "Parent" : "41"},
	{"ID" : "72", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Xpose_Col_Outer_Loop_Xpose_Col_Inner_Loop_fu_80", "Parent" : "0", "Child" : ["73"],
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
	{"ID" : "73", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_Xpose_Col_Outer_Loop_Xpose_Col_Inner_Loop_fu_80.flow_control_loop_pipe_sequential_init_U", "Parent" : "72"},
	{"ID" : "74", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_WR_Loop_Row_WR_Loop_Col_fu_86", "Parent" : "0", "Child" : ["75"],
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
	{"ID" : "75", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.grp_dct_Pipeline_WR_Loop_Row_WR_Loop_Col_fu_86.flow_control_loop_pipe_sequential_init_U", "Parent" : "74"}]}


set ArgLastReadFirstWriteLatency {
	dct {
		input_r {Type I LastRead 2 FirstWrite -1}
		output_r {Type O LastRead -1 FirstWrite 3}}
	dct_Pipeline_RD_Loop_Row_RD_Loop_Col {
		buf_2d_in {Type O LastRead -1 FirstWrite 3}
		input_r {Type I LastRead 2 FirstWrite -1}}
	dct_Pipeline_Row_DCT_Loop {
		buf_2d_in {Type I LastRead 4 FirstWrite -1}
		row_outbuf {Type O LastRead -1 FirstWrite 5}}
	dct_1d {
		src {Type I LastRead 4 FirstWrite -1}
		src_offset {Type I LastRead 0 FirstWrite -1}
		dst {Type O LastRead -1 FirstWrite 5}
		dst_offset {Type I LastRead 3 FirstWrite -1}}
	dct_Pipeline_Xpose_Row_Outer_Loop_Xpose_Row_Inner_Loop {
		row_outbuf {Type I LastRead 2 FirstWrite -1}
		col_inbuf {Type O LastRead -1 FirstWrite 3}}
	dct_Pipeline_Col_DCT_Loop {
		col_inbuf {Type I LastRead 4 FirstWrite -1}
		col_outbuf {Type O LastRead -1 FirstWrite 5}}
	dct_1d {
		src {Type I LastRead 4 FirstWrite -1}
		src_offset {Type I LastRead 0 FirstWrite -1}
		dst {Type O LastRead -1 FirstWrite 5}
		dst_offset {Type I LastRead 3 FirstWrite -1}}
	dct_Pipeline_Xpose_Col_Outer_Loop_Xpose_Col_Inner_Loop {
		col_outbuf {Type I LastRead 2 FirstWrite -1}
		buf_2d_out {Type O LastRead -1 FirstWrite 3}}
	dct_Pipeline_WR_Loop_Row_WR_Loop_Col {
		buf_2d_out {Type I LastRead 2 FirstWrite -1}
		output_r {Type O LastRead -1 FirstWrite 3}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "377", "Max" : "377"}
	, {"Name" : "Interval", "Min" : "378", "Max" : "378"}
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
