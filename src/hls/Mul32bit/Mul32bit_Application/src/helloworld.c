#include <stdio.h>
#include <stdint.h>

#include "platform.h"     // init_platform(), cleanup_platform()
#include "xil_printf.h"   // xil_printf
#include "xil_io.h"       // Xil_In32, Xil_Out32
#include "sleep.h"

// ======================================================
// Base address (từ Address Editor bạn gửi)
// ======================================================
#define MUL32_BASEADDR   0xA0000000

// ======================================================
// AXI-Lite register offsets (chuẩn HLS)
// ======================================================
#define REG_CTRL    0x00
#define REG_A       0x10
#define REG_B       0x18
#define REG_P_LO    0x20
#define REG_P_HI    0x24

// ======================================================
// AXI access helpers
// ======================================================
static inline void mul32_write(uint32_t offset, uint32_t value)
{
    Xil_Out32(MUL32_BASEADDR + offset, value);
}

static inline uint32_t mul32_read(uint32_t offset)
{
    return Xil_In32(MUL32_BASEADDR + offset);
}

// ======================================================
// MAIN
// ======================================================
int main()
{
    uint32_t a = 123456789;
    uint32_t b = 1000;

    uint32_t p_lo, p_hi;
    uint64_t result;

    // ⚠️ BẮT BUỘC: init platform (UART ở đây)
    init_platform();

    xil_printf("\r\n==============================\r\n");
    xil_printf(" MUL32 HLS AXI-Lite DEMO\r\n");
    xil_printf("==============================\r\n");

    xil_printf("A = %lu\r\n", (unsigned long)a);
    xil_printf("B = %lu\r\n", (unsigned long)b);

    // --------------------------------------------------
    // Ghi dữ liệu vào IP
    // --------------------------------------------------
    mul32_write(REG_A, a);
    mul32_write(REG_B, b);

    // --------------------------------------------------
    // Start IP (ap_start = bit0)
    // --------------------------------------------------
    mul32_write(REG_CTRL, 0x01);

    // --------------------------------------------------
    // Poll ap_done (bit1)
    // --------------------------------------------------
    while ((mul32_read(REG_CTRL) & 0x2) == 0);

    // --------------------------------------------------
    // Đọc kết quả
    // --------------------------------------------------
    p_lo = mul32_read(REG_P_LO);
    p_hi = mul32_read(REG_P_HI);

    result = ((uint64_t)p_hi << 32) | p_lo;

    xil_printf("Result = %llu\r\n",
               (unsigned long long)result);

    xil_printf("DONE ✅\r\n");

    // Không bắt buộc, nhưng giữ cho đúng chuẩn BSP
    cleanup_platform();

    while (1);
    return 0;
}
