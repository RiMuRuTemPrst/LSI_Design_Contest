// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2024.2 (64-bit)
// Tool Version Limit: 2024.11
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
/***************************** Include Files *********************************/
#include "xmul32_hls.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XMul32_hls_CfgInitialize(XMul32_hls *InstancePtr, XMul32_hls_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Ctrl_BaseAddress = ConfigPtr->Ctrl_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XMul32_hls_Start(XMul32_hls *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XMul32_hls_ReadReg(InstancePtr->Ctrl_BaseAddress, XMUL32_HLS_CTRL_ADDR_AP_CTRL) & 0x80;
    XMul32_hls_WriteReg(InstancePtr->Ctrl_BaseAddress, XMUL32_HLS_CTRL_ADDR_AP_CTRL, Data | 0x01);
}

u32 XMul32_hls_IsDone(XMul32_hls *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XMul32_hls_ReadReg(InstancePtr->Ctrl_BaseAddress, XMUL32_HLS_CTRL_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XMul32_hls_IsIdle(XMul32_hls *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XMul32_hls_ReadReg(InstancePtr->Ctrl_BaseAddress, XMUL32_HLS_CTRL_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XMul32_hls_IsReady(XMul32_hls *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XMul32_hls_ReadReg(InstancePtr->Ctrl_BaseAddress, XMUL32_HLS_CTRL_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XMul32_hls_EnableAutoRestart(XMul32_hls *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XMul32_hls_WriteReg(InstancePtr->Ctrl_BaseAddress, XMUL32_HLS_CTRL_ADDR_AP_CTRL, 0x80);
}

void XMul32_hls_DisableAutoRestart(XMul32_hls *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XMul32_hls_WriteReg(InstancePtr->Ctrl_BaseAddress, XMUL32_HLS_CTRL_ADDR_AP_CTRL, 0);
}

void XMul32_hls_Set_a(XMul32_hls *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XMul32_hls_WriteReg(InstancePtr->Ctrl_BaseAddress, XMUL32_HLS_CTRL_ADDR_A_DATA, Data);
}

u32 XMul32_hls_Get_a(XMul32_hls *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XMul32_hls_ReadReg(InstancePtr->Ctrl_BaseAddress, XMUL32_HLS_CTRL_ADDR_A_DATA);
    return Data;
}

void XMul32_hls_Set_b(XMul32_hls *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XMul32_hls_WriteReg(InstancePtr->Ctrl_BaseAddress, XMUL32_HLS_CTRL_ADDR_B_DATA, Data);
}

u32 XMul32_hls_Get_b(XMul32_hls *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XMul32_hls_ReadReg(InstancePtr->Ctrl_BaseAddress, XMUL32_HLS_CTRL_ADDR_B_DATA);
    return Data;
}

u64 XMul32_hls_Get_p(XMul32_hls *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XMul32_hls_ReadReg(InstancePtr->Ctrl_BaseAddress, XMUL32_HLS_CTRL_ADDR_P_DATA);
    Data += (u64)XMul32_hls_ReadReg(InstancePtr->Ctrl_BaseAddress, XMUL32_HLS_CTRL_ADDR_P_DATA + 4) << 32;
    return Data;
}

u32 XMul32_hls_Get_p_vld(XMul32_hls *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XMul32_hls_ReadReg(InstancePtr->Ctrl_BaseAddress, XMUL32_HLS_CTRL_ADDR_P_CTRL);
    return Data & 0x1;
}

void XMul32_hls_InterruptGlobalEnable(XMul32_hls *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XMul32_hls_WriteReg(InstancePtr->Ctrl_BaseAddress, XMUL32_HLS_CTRL_ADDR_GIE, 1);
}

void XMul32_hls_InterruptGlobalDisable(XMul32_hls *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XMul32_hls_WriteReg(InstancePtr->Ctrl_BaseAddress, XMUL32_HLS_CTRL_ADDR_GIE, 0);
}

void XMul32_hls_InterruptEnable(XMul32_hls *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XMul32_hls_ReadReg(InstancePtr->Ctrl_BaseAddress, XMUL32_HLS_CTRL_ADDR_IER);
    XMul32_hls_WriteReg(InstancePtr->Ctrl_BaseAddress, XMUL32_HLS_CTRL_ADDR_IER, Register | Mask);
}

void XMul32_hls_InterruptDisable(XMul32_hls *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XMul32_hls_ReadReg(InstancePtr->Ctrl_BaseAddress, XMUL32_HLS_CTRL_ADDR_IER);
    XMul32_hls_WriteReg(InstancePtr->Ctrl_BaseAddress, XMUL32_HLS_CTRL_ADDR_IER, Register & (~Mask));
}

void XMul32_hls_InterruptClear(XMul32_hls *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XMul32_hls_WriteReg(InstancePtr->Ctrl_BaseAddress, XMUL32_HLS_CTRL_ADDR_ISR, Mask);
}

u32 XMul32_hls_InterruptGetEnabled(XMul32_hls *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XMul32_hls_ReadReg(InstancePtr->Ctrl_BaseAddress, XMUL32_HLS_CTRL_ADDR_IER);
}

u32 XMul32_hls_InterruptGetStatus(XMul32_hls *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XMul32_hls_ReadReg(InstancePtr->Ctrl_BaseAddress, XMUL32_HLS_CTRL_ADDR_ISR);
}

