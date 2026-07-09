#include "xresblock_top.h"
#include "xparameters.h"
#include "xil_cache.h"
#include "xil_printf.h" 
#include "xil_io.h"   
#include "xpseudo_asm.h"  // Thư viện hỗ trợ hàm dsb() cho ARMv8 64-bit
#include "ff.h"           // Thư viện FatFs của Xilinx để đọc SD Card
#include <string.h> 
#include <stdio.h>        // Sử dụng cho hàm sprintf ghép chuỗi đường dẫn

// ====================================================================
// --- QUY HOẠCH ĐỊA CHỈ DDR4 ---
// ====================================================================
#define DDR4_SAFE_BASE       0x10000000ULL 
#define ADDR_BUFFER_A        (DDR4_SAFE_BASE + 0x00000000ULL) 
#define ADDR_BUFFER_B        (DDR4_SAFE_BASE + 0x00100000ULL) 
#define ADDR_WEIGHTS_BASE    (DDR4_SAFE_BASE + 0x00200000ULL) 
#define ADDR_PARAMS_BASE     (ADDR_WEIGHTS_BASE + (u64)9 * STRIDE_BLOCK)

#define STRIDE_BLOCK         0x02400000ULL 
#define STRIDE_PARAMS        0x00010000ULL 
#define NUM_RESBLOCKS        9  // Xử lý chuỗi 9 block liên tục (rb0 -> rb8)

static FATFS fatfs;
XResblock_top IpInstance;

// Macro hỗ trợ ghi 64-bit an toàn vào thanh ghi điều khiển AXI
#define WRITE_REG_64(BASE_ADDR, OFFSET, DATA64) \
    XResblock_top_WriteReg(BASE_ADDR, OFFSET, (u32)(DATA64)); \
    XResblock_top_WriteReg(BASE_ADDR, OFFSET + 4, (u32)((DATA64) >> 32))

// ====================================================================
// --- HÀM TỰ ĐỘNG ĐỌC KÍCH THƯỚC FILE VÀ NẠP DDR4 ---
// ====================================================================
int load_file_to_ddr4(const char *filepath, u64 ddr4_target_addr) {
    FIL fil;       
    FRESULT res;   
    UINT br;       

    // Mở file từ thẻ nhớ ở chế độ chỉ đọc
    res = f_open(&fil, filepath, FA_READ);
    if (res != FR_OK) {
        xil_printf("  -> [ERR] Khong the mo file: %s (Ma loi: %d)\r\n", filepath, res);
        return -1;
    }

    // Tự động lấy kích thước file thực tế từ FAT32
    FSIZE_t file_size = f_size(&fil); 

    // Đọc toàn bộ dữ liệu file đổ thẳng vào vùng nhớ DDR4 target
    res = f_read(&fil, (void*)ddr4_target_addr, file_size, &br);
    if (res != FR_OK) {
        xil_printf("  -> [ERR] Doc loi file: %s\r\n", filepath);
        f_close(&fil);
        return -1;
    }

    f_close(&fil);

    // Chí mạng: Ép dữ liệu từ Cache dội xuống RAM để phần cứng IP HLS nhìn thấy
    Xil_DCacheFlushRange(ddr4_target_addr, file_size);
    dsb(); // Hàng rào bộ nhớ chuẩn ARMv8 cho Vitis 2025.2

    return 0;
}

// ====================================================================
// --- CHƯƠNG TRÌNH CHÍNH ---
// ====================================================================
int main() {
    // Kích hoạt Cache hệ thống để CPU Cortex-A53 chạy tăng tốc
    Xil_DCacheEnable(); 
    Xil_ICacheEnable();

    xil_printf("\r\n===================================================\r\n");
    xil_printf("--- AI BARE-METAL ZCU104 - VITIS 2025.2 SYSTEM  ---\r\n");
    xil_printf("===================================================\r\n");

    // 1. MOUNT SD CARD
    FRESULT res = f_mount(&fatfs, "0:/", 1);
    if (res != FR_OK) {
        xil_printf("[ERROR] Mount the nho that bai! Ma loi: %d\r\n", res);
        return -1;
    }
    xil_printf("[SYSTEM] Da mount SD Card thanh cong!\r\n");

    // 2. KHỞI TẠO HARDWARE ACCELERATOR (IP HLS)
    // Sửa thành _BASEADDR theo chuẩn System Device Tree mới
    XResblock_top_Config *CfgPtr = XResblock_top_LookupConfig(XPAR_XRESBLOCK_TOP_0_BASEADDR); 
    if (!CfgPtr) {
        xil_printf("[ERROR] Khong tim thay cau hinh IP Resblock tai Base Address!\r\n");
        return -1;
    }
    XResblock_top_CfgInitialize(&IpInstance, CfgPtr);
    u64 base_ctrl = IpInstance.Control_BaseAddress; 

    // Cấu hình con trỏ vùng đệm Ping-Pong dữ liệu
    u64 current_in  = ADDR_BUFFER_A;
    u64 current_out = ADDR_BUFFER_B;
    char filepath[128]; 

    // Nạp file Input tổng đầu tiên vào bộ đệm A (Nằm trong thư mục rb0 trên SD Card)
    xil_printf("\r\n[SYSTEM] Dang nap file input.bin dau chuoi...\r\n");
    if (load_file_to_ddr4("rb0/input.bin", current_in) != 0) return -1;

    // 3. VÒNG LẶP TỰ ĐỘNG DUYỆT QUA 9 THƯ MỤC BLOCK (rb0 -> rb8)
    for (int i = 0; i < NUM_RESBLOCKS; i++) {
        xil_printf("\r\n>>> TIẾN TRÌNH XỬ LÝ BLOCK: rb%d <<<\r\n", i);
        
        u64 w_addr = ADDR_WEIGHTS_BASE + (i * STRIDE_BLOCK);
        u64 p_addr = ADDR_PARAMS_BASE + (i * STRIDE_PARAMS);

        xil_printf("[BLOCK %d] 1. Dang load Weights & Params tu SD Card vao DDR4...\r\n", i);

        // Tự động quét và nạp file Weights 1 & 2
        sprintf(filepath, "rb%d/Gen_rb%d_weight_1.bin", i, i);
        load_file_to_ddr4(filepath, w_addr);
        
        sprintf(filepath, "rb%d/Gen_rb%d_weight_2.bin", i, i);
        load_file_to_ddr4(filepath, w_addr + 0x01200000ULL);

        // Tự động quét và nạp nhóm tham số Params (bias -> B, gamma -> G, beta -> BE)
        sprintf(filepath, "rb%d/Gen_rb%d_bias_1.bin", i, i);
        load_file_to_ddr4(filepath, p_addr);                   // B1
        
        sprintf(filepath, "rb%d/Gen_rb%d_bias_2.bin", i, i);
        load_file_to_ddr4(filepath, p_addr + 0x1000ULL);       // B2

        sprintf(filepath, "rb%d/Gen_rb%d_gamma_1.bin", i, i);
        load_file_to_ddr4(filepath, p_addr + 0x2000ULL);       // G1
        
        sprintf(filepath, "rb%d/Gen_rb%d_beta_1.bin", i, i);
        load_file_to_ddr4(filepath, p_addr + 0x3000ULL);       // BE1

        sprintf(filepath, "rb%d/Gen_rb%d_gamma_2.bin", i, i);
        load_file_to_ddr4(filepath, p_addr + 0x4000ULL);       // G2
        
        sprintf(filepath, "rb%d/Gen_rb%d_beta_2.bin", i, i);
        load_file_to_ddr4(filepath, p_addr + 0x5000ULL);       // BE2

        xil_printf("[BLOCK %d] 2. Dang cau hinh cac thanh ghi dia chi IP...\r\n", i);

        // Ghi địa chỉ luồng dữ liệu Input và Output vào IP thông qua cổng điều khiển AXI-Lite
        WRITE_REG_64(base_ctrl, XRESBLOCK_TOP_CONTROL_ADDR_X_DATA, current_in);
        WRITE_REG_64(base_ctrl, XRESBLOCK_TOP_CONTROL_ADDR_Y_DATA, current_out);
        
        // Ghi địa chỉ tham số mô hình vào IP
        WRITE_REG_64(base_ctrl, XRESBLOCK_TOP_CONTROL_ADDR_W1_DATA, w_addr);
        WRITE_REG_64(base_ctrl, XRESBLOCK_TOP_CONTROL_ADDR_W2_DATA, (w_addr + 0x01200000ULL));
        WRITE_REG_64(base_ctrl, XRESBLOCK_TOP_CONTROL_ADDR_B1_DATA,  p_addr);
        WRITE_REG_64(base_ctrl, XRESBLOCK_TOP_CONTROL_ADDR_B2_DATA,  (p_addr + 0x1000ULL));
        WRITE_REG_64(base_ctrl, XRESBLOCK_TOP_CONTROL_ADDR_G1_DATA,  (p_addr + 0x2000ULL));
        WRITE_REG_64(base_ctrl, XRESBLOCK_TOP_CONTROL_ADDR_BE1_DATA, (p_addr + 0x3000ULL));
        WRITE_REG_64(base_ctrl, XRESBLOCK_TOP_CONTROL_ADDR_G2_DATA,  (p_addr + 0x4000ULL));
        WRITE_REG_64(base_ctrl, XRESBLOCK_TOP_CONTROL_ADDR_BE2_DATA, (p_addr + 0x5000ULL));
        
        // Ghi tham số Epsilon mã hóa FP16 (0x00A8 tương đương 1e-5)
        XResblock_top_WriteReg(base_ctrl, XRESBLOCK_TOP_CONTROL_ADDR_EPSILON_DATA, 0x00A8);

        // Dọn dẹp Cache vùng ngõ ra để CPU không lấy nhầm dữ liệu cũ rác
        Xil_DCacheInvalidateRange(current_out, 0x00078000);
        dsb(); // Ép hoàn thành dọn cache trước khi ra lệnh start phần cứng

        // KÍCH HOẠT PHẦN CỨNG THỰC THI
        xil_printf("[BLOCK %d] 3. Khoi chay Hardware Accelerator...", i);
        XResblock_top_Start(&IpInstance);
        
        // Polling liên tục chờ cờ DONE từ phần cứng báo về
        while (!XResblock_top_IsDone(&IpInstance));
        XResblock_top_Continue(&IpInstance); // ap_ctrl_chain: clear ap_done before next block

        // Đồng bộ dữ liệu mới tính toán từ DDR4 ngược lên lại Cache CPU để xử lý tiếp theo
        dsb();
        Xil_DCacheInvalidateRange(current_out, 0x00078000);
        xil_printf(" HOÀN THÀNH!\r\n");

        // Hoán đổi cơ chế Ping-Pong bộ nhớ: Đầu ra block này làm đầu vào block sau
        u64 temp = current_in;
        current_in = current_out;
        current_out = temp;
    }

    xil_printf("\r\n===================================================\r\n");
    xil_printf("[SUCCESS] DA CHAY XONG TOAN BO CHUOI 9 RESBLOCKS!\r\n");
    xil_printf("===================================================\r\n");
    
    // Tắt Cache an toàn trước khi kết thúc chương trình Bare-metal
    Xil_DCacheDisable();
    Xil_ICacheDisable();
    return 0;
}