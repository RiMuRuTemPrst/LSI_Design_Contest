import torch
import torch.nn as nn
import numpy as np
import os

# Tên thư mục chứa file txt
DATA_DIR = "block_weight" 

# ==========================================
# 1. HÀM ĐỌC VÀ CHUYỂN ĐỔI DỮ LIỆU
# ==========================================
def load_tensor_with_permute(filename, source_shape, permute_order=None):
    """
    Hàm tiện ích load weight từ file txt và permute về đúng shape PyTorch.
    """
    file_path = os.path.join(DATA_DIR, filename)
    if not os.path.exists(file_path):
        raise FileNotFoundError(f"Không tìm thấy file: {file_path}")
    
    try:
        data_np = np.loadtxt(file_path, dtype=np.float32)
    except Exception as e:
        raise ValueError(f"Lỗi đọc file {filename}: {e}")

    expected_numel = np.prod(source_shape)
    if data_np.size != expected_numel:
        raise ValueError(f"Shape không khớp: File có {data_np.size} phần tử, cần {expected_numel}.")

    tensor = torch.from_numpy(data_np).view(source_shape)
    
    if permute_order:
        tensor = tensor.permute(permute_order)
        
    return tensor

# ==========================================
# 2. ĐỊNH NGHĨA MODEL VỚI INSTANCE NORM THỦ CÔNG
# ==========================================
class ComplexBlock(nn.Module):
    def __init__(self):
        super(ComplexBlock, self).__init__()
        
        # --- 1. ConvTranspose ---
        self.conv_transpose = nn.ConvTranspose2d(
            in_channels=120, 
            out_channels=60, 
            kernel_size=3, 
            stride=2, 
            padding=1, 
            output_padding=1
        )
        
        # --- 2. Manual Instance Normalization Params ---
        # Thay vì dùng nn.InstanceNorm2d, ta khai báo Parameter để chứa Scale và Bias
        # Shape gốc là (60,), ta sẽ reshape khi tính toán
        self.inst_norm_scale = nn.Parameter(torch.ones(60))
        self.inst_norm_bias = nn.Parameter(torch.zeros(60))
        self.epsilon = 1e-3 # Giá trị epsilon lấy từ ảnh ONNX (0.001)
        
        # --- 3. ReLU ---
        self.relu = nn.ReLU()
        
        # --- 4. Padding (Reflect) ---
        self.pad = nn.ReflectionPad2d(3) 
        
        # --- 5. Conv ---
        self.conv = nn.Conv2d(60, 3, 7)

    def forward(self, x):
        # 1. ConvTranspose
        x = self.conv_transpose(x)
        
        # ====================================================
        # 2. MANUAL INSTANCE NORMALIZATION (Từng bước)
        # ====================================================
        
        # Bước 2.1: Tính Mean (ReduceMean)
        # Tính trung bình trên chiều H (2) và W (3), giữ nguyên số chiều (keepdim=True)
        # Tương ứng node: ReduceMean trong ảnh ONNX
        mean = torch.mean(x, dim=(2, 3), keepdim=True)
        
        # Bước 2.2: Trừ Mean (Centering)
        # Tương ứng node: Sub (Input - Mean)
        x_centered = x - mean
        
        # Bước 2.3: Tính Variance
        # Var = Mean((x - mean)^2). Lưu ý không dùng hàm var() của torch vì nó là unbiased (chia N-1).
        # ONNX dùng biased (chia N). Ta dùng mean(pow(x, 2)) để khớp.
        var = torch.mean(x_centered ** 2, dim=(2, 3), keepdim=True)
        
        # Bước 2.4: Cộng Epsilon và Căn bậc hai (Standard Deviation)
        # Tương ứng node: Add (Var + eps) -> Sqrt
        std = torch.sqrt(var + self.epsilon)
        
        # Bước 2.5: Chia (Normalize)
        # Tương ứng node: Div
        x_norm = x_centered / std
        
        # Bước 2.6: Affine Transform (Nhân Scale và Cộng Bias)
        # Reshape Scale/Bias từ (60) thành (1, 60, 1, 1) để broadcast
        scale = self.inst_norm_scale.view(1, -1, 1, 1)
        bias = self.inst_norm_bias.view(1, -1, 1, 1)
        
        # Tương ứng node: Mul (với Scale) -> Add (với Bias)
        x = x_norm * scale + bias
        
        # ====================================================
        
        # 3. Activation (ReLU)
        x = self.relu(x)
        
        # 4. Pad 3
        x = self.pad(x)
        
        # 5. Conv
        x = self.conv(x)
        
        # 6. Clip (Output Activation) [0, 1]
        # x = torch.clamp(x, min=0.0, max=1.0)
        
        return x

# ==========================================
# 3. LOAD DATA & WEIGHTS
# ==========================================

model = ComplexBlock()

print(f"--- Đang đọc dữ liệu từ thư mục: {DATA_DIR} ---\n")

# --- Load Input ---
input_tensor = load_tensor_with_permute("input_data.txt", (1, 32, 32, 120), (0, 3, 1, 2))

with torch.no_grad():
    # --- Load ConvTranspose ---
    model.conv_transpose.weight.data = load_tensor_with_permute("conv_transpose_w.txt", (60, 3, 3, 120), (3, 0, 1, 2))
    model.conv_transpose.bias.data   = load_tensor_with_permute("conv_transpose_b.txt", (60,))

    # --- Load Manual Instance Norm Weights ---
    # Load vào các biến Parameter tự định nghĩa thay vì layer có sẵn
    scale_data = load_tensor_with_permute("inst_norm_scale.txt", (60,))
    bias_data  = load_tensor_with_permute("inst_norm_bias.txt", (60,))
    
    model.inst_norm_scale.data = scale_data
    model.inst_norm_bias.data  = bias_data

    # --- Load Conv ---
    model.conv.weight.data = load_tensor_with_permute("conv_w.txt", (3, 7, 7, 60), (0, 3, 1, 2))
    model.conv.bias.data   = load_tensor_with_permute("conv_b.txt", (3,))

print("Weights loaded successfully!")

# Forward Pass
output = model(input_tensor)
print(f"Output shape: {output.shape}")

# Lưu kết quả
output_nhwc = output.permute(0, 2, 3, 1) 
np.savetxt(os.path.join(DATA_DIR, "output_data.txt"), output_nhwc.detach().cpu().numpy().flatten(), fmt='%.6f')
print(f"Đã lưu kết quả tại: {os.path.join(DATA_DIR, 'output_data.txt')}")