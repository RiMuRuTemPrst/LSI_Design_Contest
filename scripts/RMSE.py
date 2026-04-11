import numpy as np

# --------------------------------------------------
# Config
# --------------------------------------------------
SHAPE = (16, 16, 960)

gold_file = "gold_test.txt"
test1_file = "test_1.txt"
test2_file = "test_1_ver2.txt"

# --------------------------------------------------
# Helper: load text file -> numpy array
# --------------------------------------------------
def load_txt(path):
    data = np.loadtxt(path, dtype=np.float32)
    expected_size = np.prod(SHAPE)
    assert data.size == expected_size, \
        f"{path}: size {data.size} != expected {expected_size}"
    return data.reshape(SHAPE)

# --------------------------------------------------
# Metrics
# --------------------------------------------------
def error_metrics(test, gold):
    diff = test - gold
    abs_diff = np.abs(diff)

    return {
        "max_error": float(np.max(abs_diff)),
        "min_error": float(np.min(abs_diff)),
        "mean_error": float(np.mean(abs_diff)),
        "var_error": float(np.var(diff)),
        "rmse": float(np.sqrt(np.mean(diff ** 2))),
    }

# --------------------------------------------------
# Main
# --------------------------------------------------
gold = load_txt(gold_file)
test1 = load_txt(test1_file)
test2 = load_txt(test2_file)

m1 = error_metrics(test1, gold)
m2 = error_metrics(test2, gold)

print("===== TEST_1 vs GOLD =====")
for k, v in m1.items():
    print(f"{k:12s}: {v:e}")

print("\n===== TEST_1_VER2 vs GOLD =====")
for k, v in m2.items():
    print(f"{k:12s}: {v:e}")
