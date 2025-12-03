set ModuleHierarchy {[{
"Name" : "dct","ID" : "0","Type" : "dataflow",
"SubInsts" : [
	{"Name" : "read_data_U0","ID" : "1","Type" : "sequential",
		"SubLoops" : [
		{"Name" : "RD_Loop_Row_RD_Loop_Col","ID" : "2","Type" : "pipeline"},]},
	{"Name" : "dct_2d_U0","ID" : "3","Type" : "sequential",
		"SubInsts" : [
		{"Name" : "grp_dct_2d_Pipeline_Row_DCT_Loop_DCT_Outer_Loop_fu_84","ID" : "4","Type" : "sequential",
			"SubLoops" : [
			{"Name" : "Row_DCT_Loop_DCT_Outer_Loop","ID" : "5","Type" : "pipeline"},]},
		{"Name" : "grp_dct_2d_Pipeline_Xpose_Row_Outer_Loop_Xpose_Row_Inner_Loop_fu_122","ID" : "6","Type" : "sequential",
			"SubLoops" : [
			{"Name" : "Xpose_Row_Outer_Loop_Xpose_Row_Inner_Loop","ID" : "7","Type" : "pipeline"},]},
		{"Name" : "grp_dct_2d_Pipeline_Col_DCT_Loop_DCT_Outer_Loop_fu_135","ID" : "8","Type" : "sequential",
			"SubLoops" : [
			{"Name" : "Col_DCT_Loop_DCT_Outer_Loop","ID" : "9","Type" : "pipeline"},]},
		{"Name" : "grp_dct_2d_Pipeline_Xpose_Col_Outer_Loop_Xpose_Col_Inner_Loop_fu_164","ID" : "10","Type" : "sequential",
			"SubLoops" : [
			{"Name" : "Xpose_Col_Outer_Loop_Xpose_Col_Inner_Loop","ID" : "11","Type" : "pipeline"},]},]},
	{"Name" : "write_data_U0","ID" : "12","Type" : "sequential",
		"SubLoops" : [
		{"Name" : "WR_Loop_Row_WR_Loop_Col","ID" : "13","Type" : "pipeline"},]},]
}]}