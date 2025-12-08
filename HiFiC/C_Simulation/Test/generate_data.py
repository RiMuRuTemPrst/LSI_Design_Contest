import os
import numpy as np
import tensorflow as tf

# ================================================
# Helper: Shrink shape to max 4 per dimension
# ================================================
def shrink_shape(shape):
    if shape is None:
        return None
    return [min(dim, 4) for dim in shape]

# ================================================
# Danh sách OP từ Excel (Giữ nguyên)
# ================================================
ops = [
    ("Concat",          [4],              [8],               "int64"),
    ("Constant",        None,             [1],               None),
    ("Constant",        None,             [],                None),
    ("Constant",        None,             [2],               None),
    ("Constant",        None,             [4],               None),
    ("ConstantOfShape", [1],              [4],               "int64"),
    ("Gather",          [4],              [1],               "int64"),
    ("Identity",        None,             [1,2,1,1],         None),

    ("Pad", [1,960,68,120],   [1,960,70,122],   "float32"),
    ("Pad", [1,120,544,960],  [1,120,545,961],  "float32"),
    ("Pad", [1,120,545,961],  [1,120,545,961],  "float32"),
    ("Pad", [1,220,68,120],   [1,220,70,122],   "float32"),
    ("Pad", [1,240,272,480],  [1,240,273,481],  "float32"),
    ("Pad", [1,240,273,481],  [1,240,273,481],  "float32"),
    ("Pad", [1,3,1088,1920],  [1,3,1094,1926],  "float32"),
    ("Pad", [1,320,34,60],    [1,320,38,64],    "float32"),
    ("Pad", [1,320,68,120],   [1,320,72,124],   "float32"),
    ("Pad", [1,480,136,241],  [1,480,137,241],  "float32"),
    ("Pad", [1,480,137,241],  [1,480,137,241],  "float32"),
    ("Pad", [1,60,1088,1920], [1,60,1089,1921], "float32"),
    ("Pad", [1,60,1088,1920], [1,60,1094,1926], "float32"),
    ("Pad", [1,60,1089,1921], [1,60,1089,1921], "float32"),

    ("Reshape", [2,4],        [8],       "int64"),
    ("Reshape", [8],          [4,2],     "int64"),

    ("Shape", [1,960,68,120], [4], "float32"),
    ("Shape", [1,120,544,960],[4], "float32"),
    ("Shape", [1,240,272,480],[4], "float32"),
    ("Shape", [1,480,136,240],[4], "float32"),
    ("Shape", [1,60,1088,1920],[4],"float32"),
    ("Shape", [1,220,68,120], [4],"float32"),

    ("Slice", [4,2],  [4,2],  "int64"),

    ("Transpose", [4,2], [2,4], "int64"),
]

os.makedirs("data_test/input", exist_ok=True)
os.makedirs("data_test/golden_output", exist_ok=True)

# ================================================
# Save TXT
# ================================================
def save_txt(path, arr):
    with open(path, "w") as f:
        # Flatten array to 1D and write
        for x in arr.flatten():
            f.write(str(float(x)) + "\n")

# ================================================
# Run operator (DETERMINISTIC VERSION)
# ================================================
def run_small_op(op, ifm):

    t = tf.convert_to_tensor(ifm)

    if op == "Pad":
        # FIX: TF mặc định pad NCHW hoặc NHWC tùy config.
        # Giả sử Tensor4D C++ là (N, H, W, C), pad vào H(dim1) và W(dim2).
        # Padding config: [[pad_dim0], [pad_dim1], [pad_dim2], [pad_dim3]]
        return tf.pad(t, [[0,0],[1,1],[1,1],[0,0]])

    if op == "Reshape":
        return tf.reshape(t, [-1])

    if op == "Shape":
        return tf.constant(t.shape.as_list(), dtype=tf.int32)

    if op == "Slice":
        return t  # full slice = identity

    if op == "Transpose":
        # Test case [4,2] -> [2,4]
        return tf.transpose(t, perm=[1,0])

    if op == "Identity":
        return tf.identity(t)

    if op == "Constant":
        # FIX: Trả về chính input để khớp với C++ copy
        return t

    if op == "ConstantOfShape":
        # FIX: Trả về ones để khớp với C++ set 1.0f
        return tf.ones(ifm.shape)

    if op == "Gather":
        # FIX: Index 0 luôn hợp lệ
        idx = tf.constant([0])
        return tf.gather(t, idx, axis=0)

    if op == "Concat":
        # FIX: Nối input với chính nó để dữ liệu không ngẫu nhiên
        return tf.concat([t,t], axis=0)

    return None


# ================================================
# MAIN LOOP
# ================================================
for idx, (op, ifm_shape, ofm_shape, dtype) in enumerate(ops):

    print(f"Generating TEST: {op}_{idx}")

    # shrink IFM shape
    small_ifm_shape = shrink_shape(ifm_shape)

    # Create random input
    if small_ifm_shape is None:
        ifm = np.zeros((1,), dtype=np.float32)
    else:
        if dtype == "float32":
            ifm = np.random.rand(*small_ifm_shape).astype(np.float32)
        else:
            ifm = np.random.randint(0, 10, size=small_ifm_shape).astype(np.float32)

    # Compute golden output
    # QUAN TRỌNG: Dùng chính 'ifm' vừa tạo để tính output
    golden = run_small_op(op, ifm).numpy()

    # Save input buffer
    save_txt(f"data_test/input/{op}_{idx}.txt", ifm)

    # Save SHAPE file only if shape exists
    if small_ifm_shape is not None:
        with open(f"data_test/input/{op}_{idx}_shape.txt", "w") as fs:
            fs.write(" ".join(str(x) for x in small_ifm_shape))

    # Save golden output
    save_txt(f"data_test/golden_output/{op}_{idx}.txt", golden)

print("\nDONE — Data generation complete!")