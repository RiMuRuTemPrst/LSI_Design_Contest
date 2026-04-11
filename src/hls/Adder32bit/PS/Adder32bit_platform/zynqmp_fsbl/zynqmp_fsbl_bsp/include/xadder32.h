// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2024.2 (64-bit)
// Tool Version Limit: 2024.11
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
#ifndef XADDER32_H
#define XADDER32_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#ifndef __linux__
#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"
#include "xil_io.h"
#else
#include <stdint.h>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>
#endif
#include "xadder32_hw.h"

/**************************** Type Definitions ******************************/
#ifdef __linux__
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
#else
typedef struct {
#ifdef SDT
    char *Name;
#else
    u16 DeviceId;
#endif
    u64 Ctrl_BaseAddress;
} XAdder32_Config;
#endif

typedef struct {
    u64 Ctrl_BaseAddress;
    u32 IsReady;
} XAdder32;

typedef u32 word_type;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XAdder32_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XAdder32_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XAdder32_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XAdder32_ReadReg(BaseAddress, RegOffset) \
    *(volatile u32*)((BaseAddress) + (RegOffset))

#define Xil_AssertVoid(expr)    assert(expr)
#define Xil_AssertNonvoid(expr) assert(expr)

#define XST_SUCCESS             0
#define XST_DEVICE_NOT_FOUND    2
#define XST_OPEN_DEVICE_FAILED  3
#define XIL_COMPONENT_IS_READY  1
#endif

/************************** Function Prototypes *****************************/
#ifndef __linux__
#ifdef SDT
int XAdder32_Initialize(XAdder32 *InstancePtr, UINTPTR BaseAddress);
XAdder32_Config* XAdder32_LookupConfig(UINTPTR BaseAddress);
#else
int XAdder32_Initialize(XAdder32 *InstancePtr, u16 DeviceId);
XAdder32_Config* XAdder32_LookupConfig(u16 DeviceId);
#endif
int XAdder32_CfgInitialize(XAdder32 *InstancePtr, XAdder32_Config *ConfigPtr);
#else
int XAdder32_Initialize(XAdder32 *InstancePtr, const char* InstanceName);
int XAdder32_Release(XAdder32 *InstancePtr);
#endif

void XAdder32_Start(XAdder32 *InstancePtr);
u32 XAdder32_IsDone(XAdder32 *InstancePtr);
u32 XAdder32_IsIdle(XAdder32 *InstancePtr);
u32 XAdder32_IsReady(XAdder32 *InstancePtr);
void XAdder32_EnableAutoRestart(XAdder32 *InstancePtr);
void XAdder32_DisableAutoRestart(XAdder32 *InstancePtr);

void XAdder32_Set_a(XAdder32 *InstancePtr, u32 Data);
u32 XAdder32_Get_a(XAdder32 *InstancePtr);
void XAdder32_Set_b(XAdder32 *InstancePtr, u32 Data);
u32 XAdder32_Get_b(XAdder32 *InstancePtr);
u32 XAdder32_Get_sum(XAdder32 *InstancePtr);
u32 XAdder32_Get_sum_vld(XAdder32 *InstancePtr);

void XAdder32_InterruptGlobalEnable(XAdder32 *InstancePtr);
void XAdder32_InterruptGlobalDisable(XAdder32 *InstancePtr);
void XAdder32_InterruptEnable(XAdder32 *InstancePtr, u32 Mask);
void XAdder32_InterruptDisable(XAdder32 *InstancePtr, u32 Mask);
void XAdder32_InterruptClear(XAdder32 *InstancePtr, u32 Mask);
u32 XAdder32_InterruptGetEnabled(XAdder32 *InstancePtr);
u32 XAdder32_InterruptGetStatus(XAdder32 *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
