#include "xaxicdma.h"
#include "xparameters.h"
#include "xtime_l.h"
#include "xil_cache.h"
#include "xil_printf.h"
#include "platform.h"
#include <stdio.h>

#define DDR_SRC_ADDR   0x10000000U
#define DDR_TMP_ADDR   0x11000000U   // DDR trung gian
#define BRAM_DST_ADDR  0xA0000000U

#define DMA_BYTES      3840
#define PRINT_COUNT    10

XAxiCdma Cdma;

/* ================= fp16 -> float ================= */
static float fp16_to_float(uint16_t h)
{
    uint16_t h_exp = h & 0x7C00u;
    uint16_t h_sig = h & 0x03FFu;
    uint32_t f_sgn = ((uint32_t)h & 0x8000u) << 16;
    uint32_t f_exp, f_sig;

    if (h_exp == 0) {
        if (h_sig == 0) {
            uint32_t f = f_sgn;
            float out; *((uint32_t*)&out) = f;
            return out;
        }
        int shift = 0;
        while ((h_sig & 0x0400u) == 0) {
            h_sig <<= 1;
            shift++;
        }
        h_sig &= 0x03FFu;
        f_exp = (127 - 15 - shift) << 23;
        f_sig = (uint32_t)h_sig << 13;
    } else {
        f_exp = ((h_exp >> 10) + (127 - 15)) << 23;
        f_sig = (uint32_t)h_sig << 13;
    }

    uint32_t f = f_sgn | f_exp | f_sig;
    float out; *((uint32_t*)&out) = f;
    return out;
}

/* ============ print binary 16-bit ============ */
static void print_bin16(uint16_t v)
{
    for (int b = 15; b >= 0; b--) {
        xil_printf("%d", (v >> b) & 1);
    }
}

int main()
{
    init_platform();
    xil_printf("\n=== ZCU104 DMA TEST (DDR -> DDR -> BRAM) ===\n");

    /* Init CDMA */
    XAxiCdma_Config *Cfg = XAxiCdma_LookupConfig(XPAR_AXICDMA_0_DEVICE_ID);
    if (!Cfg) { xil_printf("CDMA lookup failed\n"); return -1; }

    if (XAxiCdma_CfgInitialize(&Cdma, Cfg, Cfg->BaseAddress) != XST_SUCCESS) {
        xil_printf("CDMA init failed\n"); return -1;
    }
    xil_printf("CDMA init OK\n");

    volatile uint16_t *ddr_src = (uint16_t*)DDR_SRC_ADDR;
    volatile uint16_t *ddr_tmp = (uint16_t*)DDR_TMP_ADDR;
    volatile uint16_t *bram    = (uint16_t*)BRAM_DST_ADDR;

    /* Dump DDR source */
    xil_printf("\nDDR SRC first %d:\n", PRINT_COUNT);
    for (int i = 0; i < PRINT_COUNT; i++)
        xil_printf("[%d] 0x%04x\n", i, ddr_src[i]);

    /* Flush source cache */
    Xil_DCacheFlushRange(DDR_SRC_ADDR, DMA_BYTES);

    /* ===== DMA: DDR_SRC -> DDR_TMP ===== */
    xil_printf("\nDMA: DDR_SRC -> DDR_TMP\n");

    if (XAxiCdma_SimpleTransfer(
            &Cdma,
            DDR_SRC_ADDR,
            DDR_TMP_ADDR,
            DMA_BYTES,
            NULL,
            NULL) != XST_SUCCESS) {
        xil_printf("DMA transfer failed\n");
        return -1;
    }

    while (XAxiCdma_IsBusy(&Cdma));

    u32 err = XAxiCdma_GetError(&Cdma);
    xil_printf("CDMA error reg = 0x%08x\n", err);

    if (err) {
        xil_printf("CDMA ERROR, abort\n");
        return -1;
    }

    /* Invalidate DDR tmp cache */
    Xil_DCacheInvalidateRange(DDR_TMP_ADDR, DMA_BYTES);

    /* Dump DDR tmp */
    xil_printf("\nDDR TMP first %d:\n", PRINT_COUNT);
    for (int i = 0; i < PRINT_COUNT; i++)
        xil_printf("[%d] 0x%04x\n", i, ddr_tmp[i]);

    /* ===== CPU COPY: DDR_TMP -> BRAM ===== */
    xil_printf("\nCPU copy DDR_TMP -> BRAM\n");
    for (int i = 0; i < DMA_BYTES/2; i++)
        bram[i] = ddr_tmp[i];

    /* Dump BRAM */
    xil_printf("\nBRAM first %d:\n", PRINT_COUNT);
    for (int i = 0; i < PRINT_COUNT; i++) {
        uint16_t raw = bram[i];
        xil_printf("[%d] 0x%04x  ", i, raw);
        print_bin16(raw);
        xil_printf("\n");
    }

    /* Convert */
    xil_printf("\nConverted to float:\n");
    for (int i = 0; i < PRINT_COUNT; i++) {
        float f = fp16_to_float(bram[i]);
        printf("[%d] %f\n", i, f);
    }

    xil_printf("=== DONE ===\n");
    while (1);
}
