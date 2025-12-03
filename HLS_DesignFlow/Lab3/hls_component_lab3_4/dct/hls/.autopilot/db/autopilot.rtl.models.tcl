set SynModuleInfo {
  {SRCNAME dct_Pipeline_RD_Loop_Row_RD_Loop_Col MODELNAME dct_Pipeline_RD_Loop_Row_RD_Loop_Col RTLNAME dct_dct_Pipeline_RD_Loop_Row_RD_Loop_Col
    SUBMODULES {
      {MODELNAME dct_flow_control_loop_pipe_sequential_init RTLNAME dct_flow_control_loop_pipe_sequential_init BINDTYPE interface TYPE internal_upc_flow_control INSTNAME dct_flow_control_loop_pipe_sequential_init_U}
    }
  }
  {SRCNAME dct_1d MODELNAME dct_1d RTLNAME dct_dct_1d
    SUBMODULES {
      {MODELNAME dct_mul_16s_15ns_29_1_1 RTLNAME dct_mul_16s_15ns_29_1_1 BINDTYPE op TYPE mul IMPL auto LATENCY 0 ALLOW_PRAGMA 1}
      {MODELNAME dct_mul_16s_15s_29_1_1 RTLNAME dct_mul_16s_15s_29_1_1 BINDTYPE op TYPE mul IMPL auto LATENCY 0 ALLOW_PRAGMA 1}
      {MODELNAME dct_mac_muladd_16s_15s_13ns_29_4_1 RTLNAME dct_mac_muladd_16s_15s_13ns_29_4_1 BINDTYPE op TYPE all IMPL dsp_slice LATENCY 3}
      {MODELNAME dct_ama_submuladd_16s_16s_12ns_29s_29_4_1 RTLNAME dct_ama_submuladd_16s_16s_12ns_29s_29_4_1 BINDTYPE op TYPE all IMPL dsp_slice LATENCY 3}
      {MODELNAME dct_mac_muladd_16s_14ns_29s_29_4_1 RTLNAME dct_mac_muladd_16s_14ns_29s_29_4_1 BINDTYPE op TYPE all IMPL dsp_slice LATENCY 3}
      {MODELNAME dct_mac_muladd_16s_15s_29s_29_4_1 RTLNAME dct_mac_muladd_16s_15s_29s_29_4_1 BINDTYPE op TYPE all IMPL dsp_slice LATENCY 3}
      {MODELNAME dct_mac_muladd_17s_13ns_29s_29_4_1 RTLNAME dct_mac_muladd_17s_13ns_29s_29_4_1 BINDTYPE op TYPE all IMPL dsp_slice LATENCY 3}
      {MODELNAME dct_mac_muladd_18s_14ns_13ns_29_4_1 RTLNAME dct_mac_muladd_18s_14ns_13ns_29_4_1 BINDTYPE op TYPE all IMPL dsp_slice LATENCY 3}
      {MODELNAME dct_ama_submuladd_16s_16s_13ns_29s_29_4_1 RTLNAME dct_ama_submuladd_16s_16s_13ns_29s_29_4_1 BINDTYPE op TYPE all IMPL dsp_slice LATENCY 3}
      {MODELNAME dct_mac_muladd_17s_12ns_13ns_29_4_1 RTLNAME dct_mac_muladd_17s_12ns_13ns_29_4_1 BINDTYPE op TYPE all IMPL dsp_slice LATENCY 3}
      {MODELNAME dct_mac_muladd_17s_13ns_13ns_29_4_1 RTLNAME dct_mac_muladd_17s_13ns_13ns_29_4_1 BINDTYPE op TYPE all IMPL dsp_slice LATENCY 3}
      {MODELNAME dct_mac_muladd_16s_14ns_29ns_29_4_1 RTLNAME dct_mac_muladd_16s_14ns_29ns_29_4_1 BINDTYPE op TYPE all IMPL dsp_slice LATENCY 3}
      {MODELNAME dct_mac_muladd_17s_12ns_29s_29_4_1 RTLNAME dct_mac_muladd_17s_12ns_29s_29_4_1 BINDTYPE op TYPE all IMPL dsp_slice LATENCY 3}
      {MODELNAME dct_mac_muladd_18s_13ns_13ns_29_4_1 RTLNAME dct_mac_muladd_18s_13ns_13ns_29_4_1 BINDTYPE op TYPE all IMPL dsp_slice LATENCY 3}
      {MODELNAME dct_ama_addmuladd_18s_16s_13ns_29ns_29_4_1 RTLNAME dct_ama_addmuladd_18s_16s_13ns_29ns_29_4_1 BINDTYPE op TYPE all IMPL dsp_slice LATENCY 3}
      {MODELNAME dct_mac_muladd_16s_15s_29ns_29_4_1 RTLNAME dct_mac_muladd_16s_15s_29ns_29_4_1 BINDTYPE op TYPE all IMPL dsp_slice LATENCY 3}
      {MODELNAME dct_ama_submuladd_18s_16s_14ns_29ns_29_4_1 RTLNAME dct_ama_submuladd_18s_16s_14ns_29ns_29_4_1 BINDTYPE op TYPE all IMPL dsp_slice LATENCY 3}
    }
  }
  {SRCNAME dct_Pipeline_Row_DCT_Loop MODELNAME dct_Pipeline_Row_DCT_Loop RTLNAME dct_dct_Pipeline_Row_DCT_Loop}
  {SRCNAME dct_Pipeline_Xpose_Row_Outer_Loop_Xpose_Row_Inner_Loop MODELNAME dct_Pipeline_Xpose_Row_Outer_Loop_Xpose_Row_Inner_Loop RTLNAME dct_dct_Pipeline_Xpose_Row_Outer_Loop_Xpose_Row_Inner_Loop}
  {SRCNAME dct_Pipeline_Col_DCT_Loop MODELNAME dct_Pipeline_Col_DCT_Loop RTLNAME dct_dct_Pipeline_Col_DCT_Loop}
  {SRCNAME dct_Pipeline_Xpose_Col_Outer_Loop_Xpose_Col_Inner_Loop MODELNAME dct_Pipeline_Xpose_Col_Outer_Loop_Xpose_Col_Inner_Loop RTLNAME dct_dct_Pipeline_Xpose_Col_Outer_Loop_Xpose_Col_Inner_Loop}
  {SRCNAME dct_Pipeline_WR_Loop_Row_WR_Loop_Col MODELNAME dct_Pipeline_WR_Loop_Row_WR_Loop_Col RTLNAME dct_dct_Pipeline_WR_Loop_Row_WR_Loop_Col}
  {SRCNAME dct MODELNAME dct RTLNAME dct IS_TOP 1
    SUBMODULES {
      {MODELNAME dct_row_outbuf_RAM_AUTO_1R1W RTLNAME dct_row_outbuf_RAM_AUTO_1R1W BINDTYPE storage TYPE ram IMPL auto LATENCY 2 ALLOW_PRAGMA 1}
      {MODELNAME dct_col_inbuf_RAM_AUTO_1R1W RTLNAME dct_col_inbuf_RAM_AUTO_1R1W BINDTYPE storage TYPE ram IMPL auto LATENCY 2 ALLOW_PRAGMA 1}
      {MODELNAME dct_buf_2d_out_RAM_AUTO_1R1W RTLNAME dct_buf_2d_out_RAM_AUTO_1R1W BINDTYPE storage TYPE ram IMPL auto LATENCY 2 ALLOW_PRAGMA 1}
    }
  }
}
