set ModuleHierarchy {[{
"Name" : "dct","ID" : "0","Type" : "sequential",
"SubInsts" : [
	{"Name" : "grp_dct_Pipeline_RD_Loop_Row_RD_Loop_Col_fu_54","ID" : "1","Type" : "sequential",
		"SubLoops" : [
		{"Name" : "RD_Loop_Row_RD_Loop_Col","ID" : "2","Type" : "pipeline"},]},
	{"Name" : "grp_dct_Pipeline_Row_DCT_Loop_fu_62","ID" : "3","Type" : "sequential",
		"SubLoops" : [
		{"Name" : "Row_DCT_Loop","ID" : "4","Type" : "pipeline",
		"SubInsts" : [
		{"Name" : "grp_dct_1d_fu_34","ID" : "5","Type" : "pipeline"},]},]},
	{"Name" : "grp_dct_Pipeline_Xpose_Row_Outer_Loop_Xpose_Row_Inner_Loop_fu_68","ID" : "6","Type" : "sequential",
		"SubLoops" : [
		{"Name" : "Xpose_Row_Outer_Loop_Xpose_Row_Inner_Loop","ID" : "7","Type" : "pipeline"},]},
	{"Name" : "grp_dct_Pipeline_Col_DCT_Loop_fu_74","ID" : "8","Type" : "sequential",
		"SubLoops" : [
		{"Name" : "Col_DCT_Loop","ID" : "9","Type" : "pipeline",
		"SubInsts" : [
		{"Name" : "grp_dct_1d_fu_34","ID" : "10","Type" : "pipeline"},]},]},
	{"Name" : "grp_dct_Pipeline_Xpose_Col_Outer_Loop_Xpose_Col_Inner_Loop_fu_80","ID" : "11","Type" : "sequential",
		"SubLoops" : [
		{"Name" : "Xpose_Col_Outer_Loop_Xpose_Col_Inner_Loop","ID" : "12","Type" : "pipeline"},]},
	{"Name" : "grp_dct_Pipeline_WR_Loop_Row_WR_Loop_Col_fu_86","ID" : "13","Type" : "sequential",
		"SubLoops" : [
		{"Name" : "WR_Loop_Row_WR_Loop_Col","ID" : "14","Type" : "pipeline"},]},]
}]}