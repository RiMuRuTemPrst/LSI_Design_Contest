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
#include "xadder32.h"

extern XAdder32_Config XAdder32_ConfigTable[];

#ifdef SDT
XAdder32_Config *XAdder32_LookupConfig(UINTPTR BaseAddress) {
	XAdder32_Config *ConfigPtr = NULL;

	int Index;

	for (Index = (u32)0x0; XAdder32_ConfigTable[Index].Name != NULL; Index++) {
		if (!BaseAddress || XAdder32_ConfigTable[Index].Ctrl_BaseAddress == BaseAddress) {
			ConfigPtr = &XAdder32_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XAdder32_Initialize(XAdder32 *InstancePtr, UINTPTR BaseAddress) {
	XAdder32_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XAdder32_LookupConfig(BaseAddress);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XAdder32_CfgInitialize(InstancePtr, ConfigPtr);
}
#else
XAdder32_Config *XAdder32_LookupConfig(u16 DeviceId) {
	XAdder32_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XADDER32_NUM_INSTANCES; Index++) {
		if (XAdder32_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XAdder32_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XAdder32_Initialize(XAdder32 *InstancePtr, u16 DeviceId) {
	XAdder32_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XAdder32_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XAdder32_CfgInitialize(InstancePtr, ConfigPtr);
}
#endif

#endif

