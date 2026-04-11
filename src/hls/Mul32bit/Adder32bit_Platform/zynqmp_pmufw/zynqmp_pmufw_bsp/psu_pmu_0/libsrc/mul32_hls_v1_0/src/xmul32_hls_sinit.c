// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2024.2 (64-bit)
// Tool Version Limit: 2024.11
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
#ifndef __linux__

#include "xstatus.h"
#ifdef SDT
#include "xparameters.h"
#endif
#include "xmul32_hls.h"

extern XMul32_hls_Config XMul32_hls_ConfigTable[];

#ifdef SDT
XMul32_hls_Config *XMul32_hls_LookupConfig(UINTPTR BaseAddress) {
	XMul32_hls_Config *ConfigPtr = NULL;

	int Index;

	for (Index = (u32)0x0; XMul32_hls_ConfigTable[Index].Name != NULL; Index++) {
		if (!BaseAddress || XMul32_hls_ConfigTable[Index].Ctrl_BaseAddress == BaseAddress) {
			ConfigPtr = &XMul32_hls_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XMul32_hls_Initialize(XMul32_hls *InstancePtr, UINTPTR BaseAddress) {
	XMul32_hls_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XMul32_hls_LookupConfig(BaseAddress);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XMul32_hls_CfgInitialize(InstancePtr, ConfigPtr);
}
#else
XMul32_hls_Config *XMul32_hls_LookupConfig(u16 DeviceId) {
	XMul32_hls_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XMUL32_HLS_NUM_INSTANCES; Index++) {
		if (XMul32_hls_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XMul32_hls_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XMul32_hls_Initialize(XMul32_hls *InstancePtr, u16 DeviceId) {
	XMul32_hls_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XMul32_hls_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XMul32_hls_CfgInitialize(InstancePtr, ConfigPtr);
}
#endif

#endif

