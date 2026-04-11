// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2024.2 (64-bit)
// Tool Version Limit: 2024.11
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
/***************************** Include Files *********************************/
#include "xadder32.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XAdder32_CfgInitialize(XAdder32 *InstancePtr, XAdder32_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Ctrl_BaseAddress = ConfigPtr->Ctrl_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XAdder32_Start(XAdder32 *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XAdder32_ReadReg(InstancePtr->Ctrl_BaseAddress, XADDER32_CTRL_ADDR_AP_CTRL) & 0x80;
    XAdder32_WriteReg(InstancePtr->Ctrl_BaseAddress, XADDER32_CTRL_ADDR_AP_CTRL, Data | 0x01);
}

u32 XAdder32_IsDone(XAdder32 *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XAdder32_ReadReg(InstancePtr->Ctrl_BaseAddress, XADDER32_CTRL_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XAdder32_IsIdle(XAdder32 *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XAdder32_ReadReg(InstancePtr->Ctrl_BaseAddress, XADDER32_CTRL_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XAdder32_IsReady(XAdder32 *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XAdder32_ReadReg(InstancePtr->Ctrl_BaseAddress, XADDER32_CTRL_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XAdder32_EnableAutoRestart(XAdder32 *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XAdder32_WriteReg(InstancePtr->Ctrl_BaseAddress, XADDER32_CTRL_ADDR_AP_CTRL, 0x80);
}

void XAdder32_DisableAutoRestart(XAdder32 *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XAdder32_WriteReg(InstancePtr->Ctrl_BaseAddress, XADDER32_CTRL_ADDR_AP_CTRL, 0);
}

void XAdder32_Set_a(XAdder32 *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XAdder32_WriteReg(InstancePtr->Ctrl_BaseAddress, XADDER32_CTRL_ADDR_A_DATA, Data);
}

u32 XAdder32_Get_a(XAdder32 *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XAdder32_ReadReg(InstancePtr->Ctrl_BaseAddress, XADDER32_CTRL_ADDR_A_DATA);
    return Data;
}

void XAdder32_Set_b(XAdder32 *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XAdder32_WriteReg(InstancePtr->Ctrl_BaseAddress, XADDER32_CTRL_ADDR_B_DATA, Data);
}

u32 XAdder32_Get_b(XAdder32 *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XAdder32_ReadReg(InstancePtr->Ctrl_BaseAddress, XADDER32_CTRL_ADDR_B_DATA);
    return Data;
}

u32 XAdder32_Get_sum(XAdder32 *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XAdder32_ReadReg(InstancePtr->Ctrl_BaseAddress, XADDER32_CTRL_ADDR_SUM_DATA);
    return Data;
}

u32 XAdder32_Get_sum_vld(XAdder32 *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XAdder32_ReadReg(InstancePtr->Ctrl_BaseAddress, XADDER32_CTRL_ADDR_SUM_CTRL);
    return Data & 0x1;
}

void XAdder32_InterruptGlobalEnable(XAdder32 *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XAdder32_WriteReg(InstancePtr->Ctrl_BaseAddress, XADDER32_CTRL_ADDR_GIE, 1);
}

void XAdder32_InterruptGlobalDisable(XAdder32 *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XAdder32_WriteReg(InstancePtr->Ctrl_BaseAddress, XADDER32_CTRL_ADDR_GIE, 0);
}

void XAdder32_InterruptEnable(XAdder32 *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XAdder32_ReadReg(InstancePtr->Ctrl_BaseAddress, XADDER32_CTRL_ADDR_IER);
    XAdder32_WriteReg(InstancePtr->Ctrl_BaseAddress, XADDER32_CTRL_ADDR_IER, Register | Mask);
}

void XAdder32_InterruptDisable(XAdder32 *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XAdder32_ReadReg(InstancePtr->Ctrl_BaseAddress, XADDER32_CTRL_ADDR_IER);
    XAdder32_WriteReg(InstancePtr->Ctrl_BaseAddress, XADDER32_CTRL_ADDR_IER, Register & (~Mask));
}

void XAdder32_InterruptClear(XAdder32 *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XAdder32_WriteReg(InstancePtr->Ctrl_BaseAddress, XADDER32_CTRL_ADDR_ISR, Mask);
}

u32 XAdder32_InterruptGetEnabled(XAdder32 *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XAdder32_ReadReg(InstancePtr->Ctrl_BaseAddress, XADDER32_CTRL_ADDR_IER);
}

u32 XAdder32_InterruptGetStatus(XAdder32 *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XAdder32_ReadReg(InstancePtr->Ctrl_BaseAddress, XADDER32_CTRL_ADDR_ISR);
}

