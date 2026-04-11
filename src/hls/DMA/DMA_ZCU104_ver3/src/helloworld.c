#include "xaxicdma.h"
#include "xparameters.h"
#include "xtime_l.h"
#include "xil_cache.h"
#include "xil_printf.h"
#include "platform.h"
#include <stdio.h>

#define DDR_SRC_ADDR   0x10000000U
#define BRAM_DST_ADDR  0xA0000000U

/* CẤU HÌNH BENCHMARK */
#define DMA_CHUNK_SIZE 3840  // Kích thước 1 gói (bytes)
#define NUM_PACKETS    4320  // Số lượng gói muốn nạp (Ví dụ: nạp 1000 lần)
// => Tổng dữ liệu = 3840 * 1000 = 3.84 MB

XAxiCdma Cdma;

int main()
{
    init_platform();
    xil_printf("\n=== ZCU104 DMA BENCHMARK (%d Packets x %d Bytes) ===\n",
               NUM_PACKETS, DMA_CHUNK_SIZE);

    /* 1. Invalidate Cache vùng nguồn để nhận dữ liệu từ XSCT (nếu có nạp mới) */
    Xil_DCacheInvalidateRange(DDR_SRC_ADDR, DMA_CHUNK_SIZE * NUM_PACKETS);

    /* Init CDMA */
    XAxiCdma_Config *Cfg = XAxiCdma_LookupConfig(XPAR_AXICDMA_0_DEVICE_ID);
    if (!Cfg) { xil_printf("CDMA lookup failed\n"); return -1; }

    if (XAxiCdma_CfgInitialize(&Cdma, Cfg, Cfg->BaseAddress) != XST_SUCCESS) {
        xil_printf("CDMA init failed\n"); return -1;
    }

    /* Tắt ngắt (Interrupt) để đo cho chính xác, ta dùng Polling */
    XAxiCdma_IntrDisable(&Cdma, XAXICDMA_XR_IRQ_ALL_MASK);

    xil_printf("Init OK. Flushing Cache Source...\n");
    /* Flush toàn bộ dữ liệu nguồn từ Cache xuống RAM trước khi đo
       để không tính thời gian Flush vào thời gian DMA */
    Xil_DCacheFlushRange(DDR_SRC_ADDR, DMA_CHUNK_SIZE * NUM_PACKETS);

    xil_printf("Starting Benchmark Loop...\n");

    /* ===== BẮT ĐẦU ĐO THỜI GIAN ===== */
    XTime t0, t1;
    XTime_GetTime(&t0);

    for (int i = 0; i < NUM_PACKETS; i++) {

        // Tính địa chỉ nguồn: Nhảy cóc từng chunk (0, 3840, 7680...)
        UINTPTR src_curr = DDR_SRC_ADDR + (i * DMA_CHUNK_SIZE);

        // Địa chỉ đích: Giữ nguyên (nạp đè vào buffer BRAM để xử lý)
        // Nếu bạn muốn copy nối tiếp trên BRAM thì sửa thành: BRAM_DST_ADDR + (i * DMA_CHUNK_SIZE)
        UINTPTR dst_curr = BRAM_DST_ADDR;

        int status = XAxiCdma_SimpleTransfer(
                &Cdma,
                src_curr,
                dst_curr,
                DMA_CHUNK_SIZE,
                NULL,
                NULL);

        if (status != XST_SUCCESS) {
            xil_printf("DMA Failed at packet %d\n", i);
            break;
        }

        // Chờ DMA xong (Polling mode)
        while (XAxiCdma_IsBusy(&Cdma));

        /* Lưu ý: Nếu muốn CPU đọc data ngay trong vòng lặp thì cần thêm
           Xil_DCacheInvalidateRange(dst_curr, DMA_CHUNK_SIZE);
           Nhưng để đo Max Throughput của DMA thì ta bỏ qua bước này.
        */
    }

    XTime_GetTime(&t1);
    /* ===== KẾT THÚC ĐO ===== */

    /* Tính toán kết quả */
    double total_time_us = (double)(t1 - t0) * 1e6 / (double)COUNTS_PER_SECOND;
    double total_bytes   = (double)DMA_CHUNK_SIZE * NUM_PACKETS;
    double throughput_MBs = (total_bytes / 1024.0 / 1024.0) / (total_time_us / 1e6);

    xil_printf("\n=== RESULT ===\n");
    xil_printf("Total Data Transferred: %d bytes\n", (int)total_bytes);
    xil_printf("Total Time            : %d us\n", (int)total_time_us);
    xil_printf("Average Time per Chunk: %.2f us\n", total_time_us / NUM_PACKETS);
    xil_printf("Throughput            : %.2f MB/s\n", throughput_MBs);

    xil_printf("=== DONE ===\n");
    while (1);
}
