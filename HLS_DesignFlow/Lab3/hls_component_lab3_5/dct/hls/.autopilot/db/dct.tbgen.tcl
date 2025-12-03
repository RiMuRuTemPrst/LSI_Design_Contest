set moduleName dct
set isTopModule 1
set isCombinational 0
set isDatapathOnly 0
set isPipelined 1
set pipeline_type dataflow
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
set portNum 26
set portList { 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst sc_in sc_logic 1 reset -1 active_high_sync } 
	{ input_r_address0 sc_out sc_lv 6 signal 0 } 
	{ input_r_ce0 sc_out sc_logic 1 signal 0 } 
	{ input_r_d0 sc_out sc_lv 16 signal 0 } 
	{ input_r_q0 sc_in sc_lv 16 signal 0 } 
	{ input_r_we0 sc_out sc_logic 1 signal 0 } 
	{ input_r_address1 sc_out sc_lv 6 signal 0 } 
	{ input_r_ce1 sc_out sc_logic 1 signal 0 } 
	{ input_r_d1 sc_out sc_lv 16 signal 0 } 
	{ input_r_q1 sc_in sc_lv 16 signal 0 } 
	{ input_r_we1 sc_out sc_logic 1 signal 0 } 
	{ output_r_address0 sc_out sc_lv 6 signal 1 } 
	{ output_r_ce0 sc_out sc_logic 1 signal 1 } 
	{ output_r_d0 sc_out sc_lv 16 signal 1 } 
	{ output_r_q0 sc_in sc_lv 16 signal 1 } 
	{ output_r_we0 sc_out sc_logic 1 signal 1 } 
	{ output_r_address1 sc_out sc_lv 6 signal 1 } 
	{ output_r_ce1 sc_out sc_logic 1 signal 1 } 
	{ output_r_d1 sc_out sc_lv 16 signal 1 } 
	{ output_r_q1 sc_in sc_lv 16 signal 1 } 
	{ output_r_we1 sc_out sc_logic 1 signal 1 } 
	{ ap_start sc_in sc_logic 1 start -1 } 
	{ ap_done sc_out sc_logic 1 predone -1 } 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ ap_idle sc_out sc_logic 1 done -1 } 
}
set NewPortList {[ 
	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst", "role": "default" }} , 
 	{ "name": "input_r_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":6, "type": "signal", "bundle":{"name": "input_r", "role": "address0" }} , 
 	{ "name": "input_r_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "input_r", "role": "ce0" }} , 
 	{ "name": "input_r_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":16, "type": "signal", "bundle":{"name": "input_r", "role": "d0" }} , 
 	{ "name": "input_r_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":16, "type": "signal", "bundle":{"name": "input_r", "role": "q0" }} , 
 	{ "name": "input_r_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "input_r", "role": "we0" }} , 
 	{ "name": "input_r_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":6, "type": "signal", "bundle":{"name": "input_r", "role": "address1" }} , 
 	{ "name": "input_r_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "input_r", "role": "ce1" }} , 
 	{ "name": "input_r_d1", "direction": "out", "datatype": "sc_lv", "bitwidth":16, "type": "signal", "bundle":{"name": "input_r", "role": "d1" }} , 
 	{ "name": "input_r_q1", "direction": "in", "datatype": "sc_lv", "bitwidth":16, "type": "signal", "bundle":{"name": "input_r", "role": "q1" }} , 
 	{ "name": "input_r_we1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "input_r", "role": "we1" }} , 
 	{ "name": "output_r_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":6, "type": "signal", "bundle":{"name": "output_r", "role": "address0" }} , 
 	{ "name": "output_r_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "output_r", "role": "ce0" }} , 
 	{ "name": "output_r_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":16, "type": "signal", "bundle":{"name": "output_r", "role": "d0" }} , 
 	{ "name": "output_r_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":16, "type": "signal", "bundle":{"name": "output_r", "role": "q0" }} , 
 	{ "name": "output_r_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "output_r", "role": "we0" }} , 
 	{ "name": "output_r_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":6, "type": "signal", "bundle":{"name": "output_r", "role": "address1" }} , 
 	{ "name": "output_r_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "output_r", "role": "ce1" }} , 
 	{ "name": "output_r_d1", "direction": "out", "datatype": "sc_lv", "bitwidth":16, "type": "signal", "bundle":{"name": "output_r", "role": "d1" }} , 
 	{ "name": "output_r_q1", "direction": "in", "datatype": "sc_lv", "bitwidth":16, "type": "signal", "bundle":{"name": "output_r", "role": "q1" }} , 
 	{ "name": "output_r_we1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "output_r", "role": "we1" }} , 
 	{ "name": "ap_start", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "start", "bundle":{"name": "ap_start", "role": "default" }} , 
 	{ "name": "ap_done", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "predone", "bundle":{"name": "ap_done", "role": "default" }} , 
 	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "ap_idle", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "done", "bundle":{"name": "ap_idle", "role": "default" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "12", "55"],
		"CDFG" : "dct",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1", "real_start" : "0",
		"Pipeline" : "Dataflow", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "1",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "425", "EstimateLatencyMax" : "425",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "1",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"InputProcess" : [
			{"ID" : "10", "Name" : "read_data_U0"}],
		"OutputProcess" : [
			{"ID" : "55", "Name" : "write_data_U0"}],
		"Port" : [
			{"Name" : "input_r", "Type" : "Memory", "Direction" : "I",
				"SubConnect" : [
					{"ID" : "10", "SubInstance" : "read_data_U0", "Port" : "input_r"}]},
			{"Name" : "output_r", "Type" : "Memory", "Direction" : "O",
				"SubConnect" : [
					{"ID" : "55", "SubInstance" : "write_data_U0", "Port" : "output_r"}]},
			{"Name" : "dct_1d_dct_coeff_table_0", "Type" : "Memory", "Direction" : "I",
				"SubConnect" : [
					{"ID" : "12", "SubInstance" : "dct_2d_U0", "Port" : "dct_1d_dct_coeff_table_0"}]},
			{"Name" : "dct_1d_dct_coeff_table_1", "Type" : "Memory", "Direction" : "I",
				"SubConnect" : [
					{"ID" : "12", "SubInstance" : "dct_2d_U0", "Port" : "dct_1d_dct_coeff_table_1"}]},
			{"Name" : "dct_1d_dct_coeff_table_2", "Type" : "Memory", "Direction" : "I",
				"SubConnect" : [
					{"ID" : "12", "SubInstance" : "dct_2d_U0", "Port" : "dct_1d_dct_coeff_table_2"}]},
			{"Name" : "dct_1d_dct_coeff_table_3", "Type" : "Memory", "Direction" : "I",
				"SubConnect" : [
					{"ID" : "12", "SubInstance" : "dct_2d_U0", "Port" : "dct_1d_dct_coeff_table_3"}]},
			{"Name" : "dct_1d_dct_coeff_table_4", "Type" : "Memory", "Direction" : "I",
				"SubConnect" : [
					{"ID" : "12", "SubInstance" : "dct_2d_U0", "Port" : "dct_1d_dct_coeff_table_4"}]},
			{"Name" : "dct_1d_dct_coeff_table_5", "Type" : "Memory", "Direction" : "I",
				"SubConnect" : [
					{"ID" : "12", "SubInstance" : "dct_2d_U0", "Port" : "dct_1d_dct_coeff_table_5"}]},
			{"Name" : "dct_1d_dct_coeff_table_6", "Type" : "Memory", "Direction" : "I",
				"SubConnect" : [
					{"ID" : "12", "SubInstance" : "dct_2d_U0", "Port" : "dct_1d_dct_coeff_table_6"}]},
			{"Name" : "dct_1d_dct_coeff_table_7", "Type" : "Memory", "Direction" : "I",
				"SubConnect" : [
					{"ID" : "12", "SubInstance" : "dct_2d_U0", "Port" : "dct_1d_dct_coeff_table_7"}]}]},
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.buf_2d_in_U", "Parent" : "0"},
	{"ID" : "2", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.buf_2d_in_1_U", "Parent" : "0"},
	{"ID" : "3", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.buf_2d_in_2_U", "Parent" : "0"},
	{"ID" : "4", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.buf_2d_in_3_U", "Parent" : "0"},
	{"ID" : "5", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.buf_2d_in_4_U", "Parent" : "0"},
	{"ID" : "6", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.buf_2d_in_5_U", "Parent" : "0"},
	{"ID" : "7", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.buf_2d_in_6_U", "Parent" : "0"},
	{"ID" : "8", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.buf_2d_in_7_U", "Parent" : "0"},
	{"ID" : "9", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.buf_2d_out_U", "Parent" : "0"},
	{"ID" : "10", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.read_data_U0", "Parent" : "0", "Child" : ["11"],
		"CDFG" : "read_data",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "1", "ap_idle" : "1", "real_start" : "0",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "67", "EstimateLatencyMax" : "67",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "1",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "input_r", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "buf_0", "Type" : "Memory", "Direction" : "O", "DependentProc" : ["12"], "DependentChan" : "1"},
			{"Name" : "buf_1", "Type" : "Memory", "Direction" : "O", "DependentProc" : ["12"], "DependentChan" : "2"},
			{"Name" : "buf_2", "Type" : "Memory", "Direction" : "O", "DependentProc" : ["12"], "DependentChan" : "3"},
			{"Name" : "buf_3", "Type" : "Memory", "Direction" : "O", "DependentProc" : ["12"], "DependentChan" : "4"},
			{"Name" : "buf_4", "Type" : "Memory", "Direction" : "O", "DependentProc" : ["12"], "DependentChan" : "5"},
			{"Name" : "buf_5", "Type" : "Memory", "Direction" : "O", "DependentProc" : ["12"], "DependentChan" : "6"},
			{"Name" : "buf_6", "Type" : "Memory", "Direction" : "O", "DependentProc" : ["12"], "DependentChan" : "7"},
			{"Name" : "buf_7", "Type" : "Memory", "Direction" : "O", "DependentProc" : ["12"], "DependentChan" : "8"}],
		"Loop" : [
			{"Name" : "RD_Loop_Row_RD_Loop_Col", "PipelineType" : "UPC",
				"LoopDec" : {"FSMBitwidth" : "1", "FirstState" : "ap_ST_fsm_pp0_stage0", "FirstStateIter" : "ap_enable_reg_pp0_iter0", "FirstStateBlock" : "ap_block_pp0_stage0_subdone", "LastState" : "ap_ST_fsm_pp0_stage0", "LastStateIter" : "ap_enable_reg_pp0_iter2", "LastStateBlock" : "ap_block_pp0_stage0_subdone", "QuitState" : "ap_ST_fsm_pp0_stage0", "QuitStateIter" : "ap_enable_reg_pp0_iter2", "QuitStateBlock" : "ap_block_pp0_stage0_subdone", "OneDepthLoop" : "0", "has_ap_ctrl" : "1", "has_continue" : "1"}}]},
	{"ID" : "11", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.read_data_U0.flow_control_loop_delay_pipe_U", "Parent" : "10"},
	{"ID" : "12", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0", "Parent" : "0", "Child" : ["13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "41", "43", "53"],
		"CDFG" : "dct_2d",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "1", "ap_idle" : "1", "real_start" : "0",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "289", "EstimateLatencyMax" : "289",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "1",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "in_block_0", "Type" : "Memory", "Direction" : "I", "DependentProc" : ["10"], "DependentChan" : "1",
				"SubConnect" : [
					{"ID" : "31", "SubInstance" : "grp_dct_2d_Pipeline_Row_DCT_Loop_DCT_Outer_Loop_fu_84", "Port" : "in_block_0", "Inst_start_state" : "1", "Inst_end_state" : "2"}]},
			{"Name" : "in_block_1", "Type" : "Memory", "Direction" : "I", "DependentProc" : ["10"], "DependentChan" : "2",
				"SubConnect" : [
					{"ID" : "31", "SubInstance" : "grp_dct_2d_Pipeline_Row_DCT_Loop_DCT_Outer_Loop_fu_84", "Port" : "in_block_1", "Inst_start_state" : "1", "Inst_end_state" : "2"}]},
			{"Name" : "in_block_2", "Type" : "Memory", "Direction" : "I", "DependentProc" : ["10"], "DependentChan" : "3",
				"SubConnect" : [
					{"ID" : "31", "SubInstance" : "grp_dct_2d_Pipeline_Row_DCT_Loop_DCT_Outer_Loop_fu_84", "Port" : "in_block_2", "Inst_start_state" : "1", "Inst_end_state" : "2"}]},
			{"Name" : "in_block_3", "Type" : "Memory", "Direction" : "I", "DependentProc" : ["10"], "DependentChan" : "4",
				"SubConnect" : [
					{"ID" : "31", "SubInstance" : "grp_dct_2d_Pipeline_Row_DCT_Loop_DCT_Outer_Loop_fu_84", "Port" : "in_block_3", "Inst_start_state" : "1", "Inst_end_state" : "2"}]},
			{"Name" : "in_block_4", "Type" : "Memory", "Direction" : "I", "DependentProc" : ["10"], "DependentChan" : "5",
				"SubConnect" : [
					{"ID" : "31", "SubInstance" : "grp_dct_2d_Pipeline_Row_DCT_Loop_DCT_Outer_Loop_fu_84", "Port" : "in_block_4", "Inst_start_state" : "1", "Inst_end_state" : "2"}]},
			{"Name" : "in_block_5", "Type" : "Memory", "Direction" : "I", "DependentProc" : ["10"], "DependentChan" : "6",
				"SubConnect" : [
					{"ID" : "31", "SubInstance" : "grp_dct_2d_Pipeline_Row_DCT_Loop_DCT_Outer_Loop_fu_84", "Port" : "in_block_5", "Inst_start_state" : "1", "Inst_end_state" : "2"}]},
			{"Name" : "in_block_6", "Type" : "Memory", "Direction" : "I", "DependentProc" : ["10"], "DependentChan" : "7",
				"SubConnect" : [
					{"ID" : "31", "SubInstance" : "grp_dct_2d_Pipeline_Row_DCT_Loop_DCT_Outer_Loop_fu_84", "Port" : "in_block_6", "Inst_start_state" : "1", "Inst_end_state" : "2"}]},
			{"Name" : "in_block_7", "Type" : "Memory", "Direction" : "I", "DependentProc" : ["10"], "DependentChan" : "8",
				"SubConnect" : [
					{"ID" : "31", "SubInstance" : "grp_dct_2d_Pipeline_Row_DCT_Loop_DCT_Outer_Loop_fu_84", "Port" : "in_block_7", "Inst_start_state" : "1", "Inst_end_state" : "2"}]},
			{"Name" : "out_block", "Type" : "Memory", "Direction" : "O", "DependentProc" : ["55"], "DependentChan" : "9",
				"SubConnect" : [
					{"ID" : "53", "SubInstance" : "grp_dct_2d_Pipeline_Xpose_Col_Outer_Loop_Xpose_Col_Inner_Loop_fu_164", "Port" : "out_block", "Inst_start_state" : "7", "Inst_end_state" : "8"}]},
			{"Name" : "dct_1d_dct_coeff_table_0", "Type" : "Memory", "Direction" : "I",
				"SubConnect" : [
					{"ID" : "31", "SubInstance" : "grp_dct_2d_Pipeline_Row_DCT_Loop_DCT_Outer_Loop_fu_84", "Port" : "dct_1d_dct_coeff_table_0", "Inst_start_state" : "1", "Inst_end_state" : "2"},
					{"ID" : "43", "SubInstance" : "grp_dct_2d_Pipeline_Col_DCT_Loop_DCT_Outer_Loop_fu_135", "Port" : "dct_1d_dct_coeff_table_0", "Inst_start_state" : "5", "Inst_end_state" : "6"}]},
			{"Name" : "dct_1d_dct_coeff_table_1", "Type" : "Memory", "Direction" : "I",
				"SubConnect" : [
					{"ID" : "31", "SubInstance" : "grp_dct_2d_Pipeline_Row_DCT_Loop_DCT_Outer_Loop_fu_84", "Port" : "dct_1d_dct_coeff_table_1", "Inst_start_state" : "1", "Inst_end_state" : "2"},
					{"ID" : "43", "SubInstance" : "grp_dct_2d_Pipeline_Col_DCT_Loop_DCT_Outer_Loop_fu_135", "Port" : "dct_1d_dct_coeff_table_1", "Inst_start_state" : "5", "Inst_end_state" : "6"}]},
			{"Name" : "dct_1d_dct_coeff_table_2", "Type" : "Memory", "Direction" : "I",
				"SubConnect" : [
					{"ID" : "31", "SubInstance" : "grp_dct_2d_Pipeline_Row_DCT_Loop_DCT_Outer_Loop_fu_84", "Port" : "dct_1d_dct_coeff_table_2", "Inst_start_state" : "1", "Inst_end_state" : "2"},
					{"ID" : "43", "SubInstance" : "grp_dct_2d_Pipeline_Col_DCT_Loop_DCT_Outer_Loop_fu_135", "Port" : "dct_1d_dct_coeff_table_2", "Inst_start_state" : "5", "Inst_end_state" : "6"}]},
			{"Name" : "dct_1d_dct_coeff_table_3", "Type" : "Memory", "Direction" : "I",
				"SubConnect" : [
					{"ID" : "31", "SubInstance" : "grp_dct_2d_Pipeline_Row_DCT_Loop_DCT_Outer_Loop_fu_84", "Port" : "dct_1d_dct_coeff_table_3", "Inst_start_state" : "1", "Inst_end_state" : "2"},
					{"ID" : "43", "SubInstance" : "grp_dct_2d_Pipeline_Col_DCT_Loop_DCT_Outer_Loop_fu_135", "Port" : "dct_1d_dct_coeff_table_3", "Inst_start_state" : "5", "Inst_end_state" : "6"}]},
			{"Name" : "dct_1d_dct_coeff_table_4", "Type" : "Memory", "Direction" : "I",
				"SubConnect" : [
					{"ID" : "31", "SubInstance" : "grp_dct_2d_Pipeline_Row_DCT_Loop_DCT_Outer_Loop_fu_84", "Port" : "dct_1d_dct_coeff_table_4", "Inst_start_state" : "1", "Inst_end_state" : "2"},
					{"ID" : "43", "SubInstance" : "grp_dct_2d_Pipeline_Col_DCT_Loop_DCT_Outer_Loop_fu_135", "Port" : "dct_1d_dct_coeff_table_4", "Inst_start_state" : "5", "Inst_end_state" : "6"}]},
			{"Name" : "dct_1d_dct_coeff_table_5", "Type" : "Memory", "Direction" : "I",
				"SubConnect" : [
					{"ID" : "31", "SubInstance" : "grp_dct_2d_Pipeline_Row_DCT_Loop_DCT_Outer_Loop_fu_84", "Port" : "dct_1d_dct_coeff_table_5", "Inst_start_state" : "1", "Inst_end_state" : "2"},
					{"ID" : "43", "SubInstance" : "grp_dct_2d_Pipeline_Col_DCT_Loop_DCT_Outer_Loop_fu_135", "Port" : "dct_1d_dct_coeff_table_5", "Inst_start_state" : "5", "Inst_end_state" : "6"}]},
			{"Name" : "dct_1d_dct_coeff_table_6", "Type" : "Memory", "Direction" : "I",
				"SubConnect" : [
					{"ID" : "31", "SubInstance" : "grp_dct_2d_Pipeline_Row_DCT_Loop_DCT_Outer_Loop_fu_84", "Port" : "dct_1d_dct_coeff_table_6", "Inst_start_state" : "1", "Inst_end_state" : "2"},
					{"ID" : "43", "SubInstance" : "grp_dct_2d_Pipeline_Col_DCT_Loop_DCT_Outer_Loop_fu_135", "Port" : "dct_1d_dct_coeff_table_6", "Inst_start_state" : "5", "Inst_end_state" : "6"}]},
			{"Name" : "dct_1d_dct_coeff_table_7", "Type" : "Memory", "Direction" : "I",
				"SubConnect" : [
					{"ID" : "31", "SubInstance" : "grp_dct_2d_Pipeline_Row_DCT_Loop_DCT_Outer_Loop_fu_84", "Port" : "dct_1d_dct_coeff_table_7", "Inst_start_state" : "1", "Inst_end_state" : "2"},
					{"ID" : "43", "SubInstance" : "grp_dct_2d_Pipeline_Col_DCT_Loop_DCT_Outer_Loop_fu_135", "Port" : "dct_1d_dct_coeff_table_7", "Inst_start_state" : "5", "Inst_end_state" : "6"}]}]},
	{"ID" : "13", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.dct_1d_dct_coeff_table_0_U", "Parent" : "12"},
	{"ID" : "14", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.dct_1d_dct_coeff_table_1_U", "Parent" : "12"},
	{"ID" : "15", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.dct_1d_dct_coeff_table_2_U", "Parent" : "12"},
	{"ID" : "16", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.dct_1d_dct_coeff_table_3_U", "Parent" : "12"},
	{"ID" : "17", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.dct_1d_dct_coeff_table_4_U", "Parent" : "12"},
	{"ID" : "18", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.dct_1d_dct_coeff_table_5_U", "Parent" : "12"},
	{"ID" : "19", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.dct_1d_dct_coeff_table_6_U", "Parent" : "12"},
	{"ID" : "20", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.dct_1d_dct_coeff_table_7_U", "Parent" : "12"},
	{"ID" : "21", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.row_outbuf_U", "Parent" : "12"},
	{"ID" : "22", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.col_outbuf_U", "Parent" : "12"},
	{"ID" : "23", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.col_inbuf_U", "Parent" : "12"},
	{"ID" : "24", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.col_inbuf_1_U", "Parent" : "12"},
	{"ID" : "25", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.col_inbuf_2_U", "Parent" : "12"},
	{"ID" : "26", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.col_inbuf_3_U", "Parent" : "12"},
	{"ID" : "27", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.col_inbuf_4_U", "Parent" : "12"},
	{"ID" : "28", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.col_inbuf_5_U", "Parent" : "12"},
	{"ID" : "29", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.col_inbuf_6_U", "Parent" : "12"},
	{"ID" : "30", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.col_inbuf_7_U", "Parent" : "12"},
	{"ID" : "31", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.grp_dct_2d_Pipeline_Row_DCT_Loop_DCT_Outer_Loop_fu_84", "Parent" : "12", "Child" : ["32", "33", "34", "35", "36", "37", "38", "39", "40"],
		"CDFG" : "dct_2d_Pipeline_Row_DCT_Loop_DCT_Outer_Loop",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1", "real_start" : "0",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "73", "EstimateLatencyMax" : "73",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "in_block_0", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "in_block_1", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "in_block_2", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "in_block_3", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "in_block_4", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "in_block_5", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "in_block_6", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "in_block_7", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "row_outbuf", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "dct_1d_dct_coeff_table_0", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "dct_1d_dct_coeff_table_1", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "dct_1d_dct_coeff_table_2", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "dct_1d_dct_coeff_table_3", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "dct_1d_dct_coeff_table_4", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "dct_1d_dct_coeff_table_5", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "dct_1d_dct_coeff_table_6", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "dct_1d_dct_coeff_table_7", "Type" : "Memory", "Direction" : "I"}],
		"Loop" : [
			{"Name" : "Row_DCT_Loop_DCT_Outer_Loop", "PipelineType" : "UPC",
				"LoopDec" : {"FSMBitwidth" : "1", "FirstState" : "ap_ST_fsm_pp0_stage0", "FirstStateIter" : "ap_enable_reg_pp0_iter0", "FirstStateBlock" : "ap_block_pp0_stage0_subdone", "LastState" : "ap_ST_fsm_pp0_stage0", "LastStateIter" : "ap_enable_reg_pp0_iter8", "LastStateBlock" : "ap_block_pp0_stage0_subdone", "QuitState" : "ap_ST_fsm_pp0_stage0", "QuitStateIter" : "ap_enable_reg_pp0_iter8", "QuitStateBlock" : "ap_block_pp0_stage0_subdone", "OneDepthLoop" : "0", "has_ap_ctrl" : "1", "has_continue" : "0"}}]},
	{"ID" : "32", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.grp_dct_2d_Pipeline_Row_DCT_Loop_DCT_Outer_Loop_fu_84.mul_16s_15s_29_1_1_U10", "Parent" : "31"},
	{"ID" : "33", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.grp_dct_2d_Pipeline_Row_DCT_Loop_DCT_Outer_Loop_fu_84.mul_16s_15s_29_1_1_U11", "Parent" : "31"},
	{"ID" : "34", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.grp_dct_2d_Pipeline_Row_DCT_Loop_DCT_Outer_Loop_fu_84.mul_16s_14ns_29_1_1_U12", "Parent" : "31"},
	{"ID" : "35", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.grp_dct_2d_Pipeline_Row_DCT_Loop_DCT_Outer_Loop_fu_84.mac_muladd_16s_15s_13ns_29_4_1_U13", "Parent" : "31"},
	{"ID" : "36", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.grp_dct_2d_Pipeline_Row_DCT_Loop_DCT_Outer_Loop_fu_84.mac_muladd_16s_15s_29s_29_4_1_U14", "Parent" : "31"},
	{"ID" : "37", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.grp_dct_2d_Pipeline_Row_DCT_Loop_DCT_Outer_Loop_fu_84.mac_muladd_16s_15s_29ns_29_4_1_U15", "Parent" : "31"},
	{"ID" : "38", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.grp_dct_2d_Pipeline_Row_DCT_Loop_DCT_Outer_Loop_fu_84.mac_muladd_16s_15s_29s_29_4_1_U16", "Parent" : "31"},
	{"ID" : "39", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.grp_dct_2d_Pipeline_Row_DCT_Loop_DCT_Outer_Loop_fu_84.mac_muladd_16s_15s_29s_29_4_1_U17", "Parent" : "31"},
	{"ID" : "40", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.grp_dct_2d_Pipeline_Row_DCT_Loop_DCT_Outer_Loop_fu_84.flow_control_loop_pipe_sequential_init_U", "Parent" : "31"},
	{"ID" : "41", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.grp_dct_2d_Pipeline_Xpose_Row_Outer_Loop_Xpose_Row_Inner_Loop_fu_122", "Parent" : "12", "Child" : ["42"],
		"CDFG" : "dct_2d_Pipeline_Xpose_Row_Outer_Loop_Xpose_Row_Inner_Loop",
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
	{"ID" : "42", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.grp_dct_2d_Pipeline_Xpose_Row_Outer_Loop_Xpose_Row_Inner_Loop_fu_122.flow_control_loop_pipe_sequential_init_U", "Parent" : "41"},
	{"ID" : "43", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.grp_dct_2d_Pipeline_Col_DCT_Loop_DCT_Outer_Loop_fu_135", "Parent" : "12", "Child" : ["44", "45", "46", "47", "48", "49", "50", "51", "52"],
		"CDFG" : "dct_2d_Pipeline_Col_DCT_Loop_DCT_Outer_Loop",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1", "real_start" : "0",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "73", "EstimateLatencyMax" : "73",
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
			{"Name" : "col_outbuf", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "dct_1d_dct_coeff_table_0", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "dct_1d_dct_coeff_table_1", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "dct_1d_dct_coeff_table_2", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "dct_1d_dct_coeff_table_3", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "dct_1d_dct_coeff_table_4", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "dct_1d_dct_coeff_table_5", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "dct_1d_dct_coeff_table_6", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "dct_1d_dct_coeff_table_7", "Type" : "Memory", "Direction" : "I"}],
		"Loop" : [
			{"Name" : "Col_DCT_Loop_DCT_Outer_Loop", "PipelineType" : "UPC",
				"LoopDec" : {"FSMBitwidth" : "1", "FirstState" : "ap_ST_fsm_pp0_stage0", "FirstStateIter" : "ap_enable_reg_pp0_iter0", "FirstStateBlock" : "ap_block_pp0_stage0_subdone", "LastState" : "ap_ST_fsm_pp0_stage0", "LastStateIter" : "ap_enable_reg_pp0_iter8", "LastStateBlock" : "ap_block_pp0_stage0_subdone", "QuitState" : "ap_ST_fsm_pp0_stage0", "QuitStateIter" : "ap_enable_reg_pp0_iter8", "QuitStateBlock" : "ap_block_pp0_stage0_subdone", "OneDepthLoop" : "0", "has_ap_ctrl" : "1", "has_continue" : "0"}}]},
	{"ID" : "44", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.grp_dct_2d_Pipeline_Col_DCT_Loop_DCT_Outer_Loop_fu_135.mul_16s_15s_29_1_1_U49", "Parent" : "43"},
	{"ID" : "45", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.grp_dct_2d_Pipeline_Col_DCT_Loop_DCT_Outer_Loop_fu_135.mul_16s_15s_29_1_1_U50", "Parent" : "43"},
	{"ID" : "46", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.grp_dct_2d_Pipeline_Col_DCT_Loop_DCT_Outer_Loop_fu_135.mul_16s_14ns_29_1_1_U51", "Parent" : "43"},
	{"ID" : "47", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.grp_dct_2d_Pipeline_Col_DCT_Loop_DCT_Outer_Loop_fu_135.mac_muladd_16s_15s_13ns_29_4_1_U52", "Parent" : "43"},
	{"ID" : "48", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.grp_dct_2d_Pipeline_Col_DCT_Loop_DCT_Outer_Loop_fu_135.mac_muladd_16s_15s_29s_29_4_1_U53", "Parent" : "43"},
	{"ID" : "49", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.grp_dct_2d_Pipeline_Col_DCT_Loop_DCT_Outer_Loop_fu_135.mac_muladd_16s_15s_29ns_29_4_1_U54", "Parent" : "43"},
	{"ID" : "50", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.grp_dct_2d_Pipeline_Col_DCT_Loop_DCT_Outer_Loop_fu_135.mac_muladd_16s_15s_29s_29_4_1_U55", "Parent" : "43"},
	{"ID" : "51", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.grp_dct_2d_Pipeline_Col_DCT_Loop_DCT_Outer_Loop_fu_135.mac_muladd_16s_15s_29s_29_4_1_U56", "Parent" : "43"},
	{"ID" : "52", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.grp_dct_2d_Pipeline_Col_DCT_Loop_DCT_Outer_Loop_fu_135.flow_control_loop_pipe_sequential_init_U", "Parent" : "43"},
	{"ID" : "53", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.grp_dct_2d_Pipeline_Xpose_Col_Outer_Loop_Xpose_Col_Inner_Loop_fu_164", "Parent" : "12", "Child" : ["54"],
		"CDFG" : "dct_2d_Pipeline_Xpose_Col_Outer_Loop_Xpose_Col_Inner_Loop",
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
			{"Name" : "out_block", "Type" : "Memory", "Direction" : "O"},
			{"Name" : "col_outbuf", "Type" : "Memory", "Direction" : "I"}],
		"Loop" : [
			{"Name" : "Xpose_Col_Outer_Loop_Xpose_Col_Inner_Loop", "PipelineType" : "UPC",
				"LoopDec" : {"FSMBitwidth" : "1", "FirstState" : "ap_ST_fsm_pp0_stage0", "FirstStateIter" : "ap_enable_reg_pp0_iter0", "FirstStateBlock" : "ap_block_pp0_stage0_subdone", "LastState" : "ap_ST_fsm_pp0_stage0", "LastStateIter" : "ap_enable_reg_pp0_iter3", "LastStateBlock" : "ap_block_pp0_stage0_subdone", "QuitState" : "ap_ST_fsm_pp0_stage0", "QuitStateIter" : "ap_enable_reg_pp0_iter3", "QuitStateBlock" : "ap_block_pp0_stage0_subdone", "OneDepthLoop" : "0", "has_ap_ctrl" : "1", "has_continue" : "0"}}]},
	{"ID" : "54", "Level" : "3", "Path" : "`AUTOTB_DUT_INST.dct_2d_U0.grp_dct_2d_Pipeline_Xpose_Col_Outer_Loop_Xpose_Col_Inner_Loop_fu_164.flow_control_loop_pipe_sequential_init_U", "Parent" : "53"},
	{"ID" : "55", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.write_data_U0", "Parent" : "0", "Child" : ["56"],
		"CDFG" : "write_data",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "1", "ap_idle" : "1", "real_start" : "0",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "67", "EstimateLatencyMax" : "67",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "1",
		"HasNonBlockingOperation" : "0",
		"IsBlackBox" : "0",
		"Port" : [
			{"Name" : "buf_r", "Type" : "Memory", "Direction" : "I", "DependentProc" : ["12"], "DependentChan" : "9"},
			{"Name" : "output_r", "Type" : "Memory", "Direction" : "O"}],
		"Loop" : [
			{"Name" : "WR_Loop_Row_WR_Loop_Col", "PipelineType" : "UPC",
				"LoopDec" : {"FSMBitwidth" : "1", "FirstState" : "ap_ST_fsm_pp0_stage0", "FirstStateIter" : "ap_enable_reg_pp0_iter0", "FirstStateBlock" : "ap_block_pp0_stage0_subdone", "LastState" : "ap_ST_fsm_pp0_stage0", "LastStateIter" : "ap_enable_reg_pp0_iter2", "LastStateBlock" : "ap_block_pp0_stage0_subdone", "QuitState" : "ap_ST_fsm_pp0_stage0", "QuitStateIter" : "ap_enable_reg_pp0_iter2", "QuitStateBlock" : "ap_block_pp0_stage0_subdone", "OneDepthLoop" : "0", "has_ap_ctrl" : "1", "has_continue" : "1"}}]},
	{"ID" : "56", "Level" : "2", "Path" : "`AUTOTB_DUT_INST.write_data_U0.flow_control_loop_delay_pipe_U", "Parent" : "55"}]}


set ArgLastReadFirstWriteLatency {
	dct {
		input_r {Type I LastRead 1 FirstWrite -1}
		output_r {Type O LastRead -1 FirstWrite 2}
		dct_1d_dct_coeff_table_0 {Type I LastRead -1 FirstWrite -1}
		dct_1d_dct_coeff_table_1 {Type I LastRead -1 FirstWrite -1}
		dct_1d_dct_coeff_table_2 {Type I LastRead -1 FirstWrite -1}
		dct_1d_dct_coeff_table_3 {Type I LastRead -1 FirstWrite -1}
		dct_1d_dct_coeff_table_4 {Type I LastRead -1 FirstWrite -1}
		dct_1d_dct_coeff_table_5 {Type I LastRead -1 FirstWrite -1}
		dct_1d_dct_coeff_table_6 {Type I LastRead -1 FirstWrite -1}
		dct_1d_dct_coeff_table_7 {Type I LastRead -1 FirstWrite -1}}
	read_data {
		input_r {Type I LastRead 1 FirstWrite -1}
		buf_0 {Type O LastRead -1 FirstWrite 2}
		buf_1 {Type O LastRead -1 FirstWrite 2}
		buf_2 {Type O LastRead -1 FirstWrite 2}
		buf_3 {Type O LastRead -1 FirstWrite 2}
		buf_4 {Type O LastRead -1 FirstWrite 2}
		buf_5 {Type O LastRead -1 FirstWrite 2}
		buf_6 {Type O LastRead -1 FirstWrite 2}
		buf_7 {Type O LastRead -1 FirstWrite 2}}
	dct_2d {
		in_block_0 {Type I LastRead 3 FirstWrite -1}
		in_block_1 {Type I LastRead 3 FirstWrite -1}
		in_block_2 {Type I LastRead 3 FirstWrite -1}
		in_block_3 {Type I LastRead 3 FirstWrite -1}
		in_block_4 {Type I LastRead 2 FirstWrite -1}
		in_block_5 {Type I LastRead 2 FirstWrite -1}
		in_block_6 {Type I LastRead 2 FirstWrite -1}
		in_block_7 {Type I LastRead 1 FirstWrite -1}
		out_block {Type O LastRead -1 FirstWrite 3}
		dct_1d_dct_coeff_table_0 {Type I LastRead -1 FirstWrite -1}
		dct_1d_dct_coeff_table_1 {Type I LastRead -1 FirstWrite -1}
		dct_1d_dct_coeff_table_2 {Type I LastRead -1 FirstWrite -1}
		dct_1d_dct_coeff_table_3 {Type I LastRead -1 FirstWrite -1}
		dct_1d_dct_coeff_table_4 {Type I LastRead -1 FirstWrite -1}
		dct_1d_dct_coeff_table_5 {Type I LastRead -1 FirstWrite -1}
		dct_1d_dct_coeff_table_6 {Type I LastRead -1 FirstWrite -1}
		dct_1d_dct_coeff_table_7 {Type I LastRead -1 FirstWrite -1}}
	dct_2d_Pipeline_Row_DCT_Loop_DCT_Outer_Loop {
		in_block_0 {Type I LastRead 3 FirstWrite -1}
		in_block_1 {Type I LastRead 3 FirstWrite -1}
		in_block_2 {Type I LastRead 3 FirstWrite -1}
		in_block_3 {Type I LastRead 3 FirstWrite -1}
		in_block_4 {Type I LastRead 2 FirstWrite -1}
		in_block_5 {Type I LastRead 2 FirstWrite -1}
		in_block_6 {Type I LastRead 2 FirstWrite -1}
		in_block_7 {Type I LastRead 1 FirstWrite -1}
		row_outbuf {Type O LastRead -1 FirstWrite 8}
		dct_1d_dct_coeff_table_0 {Type I LastRead 2 FirstWrite -1}
		dct_1d_dct_coeff_table_1 {Type I LastRead 2 FirstWrite -1}
		dct_1d_dct_coeff_table_2 {Type I LastRead 2 FirstWrite -1}
		dct_1d_dct_coeff_table_3 {Type I LastRead 2 FirstWrite -1}
		dct_1d_dct_coeff_table_4 {Type I LastRead 2 FirstWrite -1}
		dct_1d_dct_coeff_table_5 {Type I LastRead 2 FirstWrite -1}
		dct_1d_dct_coeff_table_6 {Type I LastRead 2 FirstWrite -1}
		dct_1d_dct_coeff_table_7 {Type I LastRead 1 FirstWrite -1}}
	dct_2d_Pipeline_Xpose_Row_Outer_Loop_Xpose_Row_Inner_Loop {
		col_inbuf {Type O LastRead -1 FirstWrite 3}
		col_inbuf_1 {Type O LastRead -1 FirstWrite 3}
		col_inbuf_2 {Type O LastRead -1 FirstWrite 3}
		col_inbuf_3 {Type O LastRead -1 FirstWrite 3}
		col_inbuf_4 {Type O LastRead -1 FirstWrite 3}
		col_inbuf_5 {Type O LastRead -1 FirstWrite 3}
		col_inbuf_6 {Type O LastRead -1 FirstWrite 3}
		col_inbuf_7 {Type O LastRead -1 FirstWrite 3}
		row_outbuf {Type I LastRead 2 FirstWrite -1}}
	dct_2d_Pipeline_Col_DCT_Loop_DCT_Outer_Loop {
		col_inbuf {Type I LastRead 3 FirstWrite -1}
		col_inbuf_1 {Type I LastRead 3 FirstWrite -1}
		col_inbuf_2 {Type I LastRead 3 FirstWrite -1}
		col_inbuf_3 {Type I LastRead 3 FirstWrite -1}
		col_inbuf_4 {Type I LastRead 2 FirstWrite -1}
		col_inbuf_5 {Type I LastRead 2 FirstWrite -1}
		col_inbuf_6 {Type I LastRead 2 FirstWrite -1}
		col_inbuf_7 {Type I LastRead 1 FirstWrite -1}
		col_outbuf {Type O LastRead -1 FirstWrite 8}
		dct_1d_dct_coeff_table_0 {Type I LastRead 2 FirstWrite -1}
		dct_1d_dct_coeff_table_1 {Type I LastRead 2 FirstWrite -1}
		dct_1d_dct_coeff_table_2 {Type I LastRead 2 FirstWrite -1}
		dct_1d_dct_coeff_table_3 {Type I LastRead 2 FirstWrite -1}
		dct_1d_dct_coeff_table_4 {Type I LastRead 2 FirstWrite -1}
		dct_1d_dct_coeff_table_5 {Type I LastRead 2 FirstWrite -1}
		dct_1d_dct_coeff_table_6 {Type I LastRead 2 FirstWrite -1}
		dct_1d_dct_coeff_table_7 {Type I LastRead 1 FirstWrite -1}}
	dct_2d_Pipeline_Xpose_Col_Outer_Loop_Xpose_Col_Inner_Loop {
		out_block {Type O LastRead -1 FirstWrite 3}
		col_outbuf {Type I LastRead 2 FirstWrite -1}}
	write_data {
		buf_r {Type I LastRead 1 FirstWrite -1}
		output_r {Type O LastRead -1 FirstWrite 2}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "425", "Max" : "425"}
	, {"Name" : "Interval", "Min" : "290", "Max" : "290"}
]}

set PipelineEnableSignalInfo {[
]}

set Spec2ImplPortList { 
	input_r { ap_memory {  { input_r_address0 mem_address 1 6 }  { input_r_ce0 mem_ce 1 1 }  { input_r_d0 mem_din 1 16 }  { input_r_q0 mem_dout 0 16 }  { input_r_we0 mem_we 1 1 }  { input_r_address1 mem_address 1 6 }  { input_r_ce1 mem_ce 1 1 }  { input_r_d1 mem_din 1 16 }  { input_r_q1 mem_dout 0 16 }  { input_r_we1 mem_we 1 1 } } }
	output_r { ap_memory {  { output_r_address0 mem_address 1 6 }  { output_r_ce0 mem_ce 1 1 }  { output_r_d0 mem_din 1 16 }  { output_r_q0 mem_dout 0 16 }  { output_r_we0 mem_we 1 1 }  { output_r_address1 mem_address 1 6 }  { output_r_ce1 mem_ce 1 1 }  { output_r_d1 mem_din 1 16 }  { output_r_q1 mem_dout 0 16 }  { output_r_we1 mem_we 1 1 } } }
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
