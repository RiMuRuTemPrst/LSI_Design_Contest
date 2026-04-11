import numpy as np
import os

# ==========================================
# CẤU HÌNH KÍCH THƯỚC (PHẢI KHỚP VỚI MODEL)
# ==========================================
INPUT_H = 32   # Chiều cao ảnh đầu vào
INPUT_W = 32   # Chiều rộng ảnh đầu vào
BATCH_SIZE = 1

# Tạo thư mục chứa data nếu chưa có
OUTPUT_DIR = "block_weight"
if not os.path.exists(OUTPUT_DIR):
    os.makedirs(OUTPUT_DIR)

print(f"--- Đang sinh dữ liệu vào thư mục '{OUTPUT_DIR}' ---\n")

def generate_and_save(filename, shape, permute_order=None, init_type='normal'):
    """
    Sinh mảng numpy theo shape và lưu vào file txt.
    Dữ liệu được làm phẳng (flatten) thành 1 cột hoặc 1 dòng dài.
    """
    filepath = os.path.join(OUTPUT_DIR, filename)
    
    # 1. Sinh dữ liệu giả lập
    if init_type == 'normal':
        # Phân phối chuẩn (Gaussian) - thường dùng cho Weight
        data = np.random.randn(*shape).astype(np.float32)
    elif init_type == 'ones':
        # Toàn số 1 - thường dùng khởi tạo Scale (Gamma) để không làm mất tín hiệu
        data = np.ones(shape, dtype=np.float32)
    elif init_type == 'zeros':
        # Toàn số 0 - thường dùng cho Bias
        data = np.zeros(shape, dtype=np.float32)
    else:
        data = np.random.rand(*shape).astype(np.float32)

    if permute_order is not None:
        # data đang là shape gốc, chuyển sang shape mong muốn
        # VD: data (1, 120, 32, 32) --transpose(0,2,3,1)--> (1, 32, 32, 120)
        data_to_save = np.transpose(data, permute_order)
        print(f"File: {filename:<20} | PyTorch Shape: {str(shape):<18} -> Saved Shape: {str(data_to_save.shape)}")
    else:
        data_to_save = data
        print(f"File: {filename:<20} | Shape: {str(shape)}")

    # 2. Lưu file txt
    # fmt='%.6f': Lưu 6 chữ số thập phân
    # data.flatten(): Duỗi thẳng tensor nhiều chiều thành 1 chuỗi số liên tiếp
    np.savetxt(filepath, data_to_save.flatten(), fmt='%.6f')
    
    print(f"[OK] {filename:<20} | Shape gốc: {str(shape):<20} | Số lượng phần tử: {data.size}")

# ==========================================
# 1. INPUT
# ==========================================
# Shape: (Batch, In_Channels, H, W)
input_shape = (BATCH_SIZE, 120, INPUT_H, INPUT_W)
generate_and_save("input_data.txt", input_shape, (0, 2, 3, 1), init_type='normal')

# ==========================================
# 2. CONV TRANSPOSE LAYER
# ==========================================
# PyTorch ConvTranspose Weight Shape: (in_channels, out_channels, kernel_h, kernel_w)
# Theo ảnh: W <120x60x3x3>
generate_and_save("conv_transpose_w.txt", (120, 60, 3, 3), (1, 2, 3, 0), init_type='normal')

# Bias: <60>
generate_and_save("conv_transpose_b.txt", (60,), init_type='normal')

# ==========================================
# 3. INSTANCE NORMALIZATION (AFFINE PARAMS)
# ==========================================
# Trong ảnh là Node Mul (A) và Add (B) sau khi chuẩn hóa
# Scale (A) thường khởi tạo là 1 (để giữ nguyên giá trị ban đầu)
generate_and_save("inst_norm_scale.txt", (60,), init_type='ones') 

# Bias (B) thường khởi tạo là 0
generate_and_save("inst_norm_bias.txt", (60,), init_type='ones')

# ==========================================
# 4. CONV LAYER
# ==========================================
# PyTorch Conv2d Weight Shape: (out_channels, in_channels, kernel_h, kernel_w)
# Theo ảnh: W <3x60x7x7>, input vào lớp này là 60 channels, output ra 3 channels
generate_and_save("conv_w.txt", (3, 60, 7, 7), (0, 2, 3, 1), init_type='normal')

# Bias: <3>
generate_and_save("conv_b.txt", (3,), init_type='normal')

print("\n--- Hoàn tất! Bạn hãy copy đường dẫn các file trong thư mục weights_data vào code chính để chạy ---")