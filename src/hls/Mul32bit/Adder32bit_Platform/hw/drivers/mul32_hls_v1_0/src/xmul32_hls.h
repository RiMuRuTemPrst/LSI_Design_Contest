// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2024.2 (64-bit)
// Tool Version Limit: 2024.11
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
#ifndef XMUL32_HLS_H
#define XMUL32_HLS_H

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
#include "xmul32_hls_hw.h"

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
} XMul32_hls_Config;
#endif

typedef struct {
    u64 Ctrl_BaseAddress;
    u32 IsReady;
} XMul32_hls;

typedef u32 word_type;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XMul32_hls_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XMul32_hls_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XMul32_hls_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XMul32_hls_ReadReg(BaseAddress, RegOffset) \
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
int XMul32_hls_Initialize(XMul32_hls *InstancePtr, UINTPTR BaseAddress);
XMul32_hls_Config* XMul32_hls_LookupConfig(UINTPTR BaseAddress);
#else
int XMul32_hls_Initialize(XMul32_hls *InstancePtr, u16 DeviceId);
XMul32_hls_Config* XMul32_hls_LookupConfig(u16 DeviceId);
#endif
int XMul32_hls_CfgInitialize(XMul32_hls *InstancePtr, XMul32_hls_Config *ConfigPtr);
#else
int XMul32_hls_Initialize(XMul32_hls *InstancePtr, const char* InstanceName);
int XMul32_hls_Release(XMul32_hls *InstancePtr);
#endif

void XMul32_hls_Start(XMul32_hls *InstancePtr);
u32 XMul32_hls_IsDone(XMul32_hls *InstancePtr);
u32 XMul32_hls_IsIdle(XMul32_hls *InstancePtr);
u32 XMul32_hls_IsReady(XMul32_hls *InstancePtr);
void XMul32_hls_EnableAutoRestart(XMul32_hls *InstancePtr);
void XMul32_hls_DisableAutoRestart(XMul32_hls *InstancePtr);

void XMul32_hls_Set_a(XMul32_hls *InstancePtr, u32 Data);
u32 XMul32_hls_Get_a(XMul32_hls *InstancePtr);
void XMul32_hls_Set_b(XMul32_hls *InstancePtr, u32 Data);
u32 XMul32_hls_Get_b(XMul32_hls *InstancePtr);
u64 XMul32_hls_Get_p(XMul32_hls *InstancePtr);
u32 XMul32_hls_Get_p_vld(XMul32_hls *InstancePtr);

void XMul32_hls_InterruptGlobalEnable(XMul32_hls *InstancePtr);
void XMul32_hls_InterruptGlobalDisable(XMul32_hls *InstancePtr);
void XMul32_hls_InterruptEnable(XMul32_hls *InstancePtr, u32 Mask);
void XMul32_hls_InterruptDisable(XMul32_hls *InstancePtr, u32 Mask);
void XMul32_hls_InterruptClear(XMul32_hls *InstancePtr, u32 Mask);
u32 XMul32_hls_InterruptGetEnabled(XMul32_hls *InstancePtr);
u32 XMul32_hls_InterruptGetStatus(XMul32_hls *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
