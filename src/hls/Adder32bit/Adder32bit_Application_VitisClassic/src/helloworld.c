#include "xil_printf.h"
#include "xil_io.h"
#include "xparameters.h"
#include "sleep.h"


#define ADDER_BASE_ADDR   0xA0000000


#define ADDER_CTRL        0x00
#define ADDER_IN_A        0x10
#define ADDER_IN_B        0x18
#define ADDER_OUT_SUM     0x20

int main()
{
	init_platform();
    u32 a = 123;
    u32 b = 456;
    u32 sum;

    xil_printf("=== PS controls HLS 32-bit Adder ===\r\n");

    /* Ghi dữ liệu đầu vào */
    Xil_Out32(ADDER_BASE_ADDR + ADDER_IN_A, a);
    Xil_Out32(ADDER_BASE_ADDR + ADDER_IN_B, b);

    /* Start IP (ap_start = bit[0]) */
    Xil_Out32(ADDER_BASE_ADDR + ADDER_CTRL, 0x01);

    /* Chờ IP xử lý xong (ap_done = bit[1]) */
    while ((Xil_In32(ADDER_BASE_ADDR + ADDER_CTRL) & 0x2) == 0);

    /* Đọc kết quả */
    sum = Xil_In32(ADDER_BASE_ADDR + ADDER_OUT_SUM);

    xil_printf("A   = %d\r\n", a);
    xil_printf("B   = %d\r\n", b);
    xil_printf("SUM = %d\r\n", sum);

    while (1);
    return 0;
}
