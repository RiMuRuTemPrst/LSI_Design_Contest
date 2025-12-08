import os
import numpy as np

BASE_DIR = os.path.dirname(os.path.abspath(__file__))

golden_dir = os.path.join(BASE_DIR, "data_test", "golden_output")
output_dir = os.path.join(BASE_DIR, "data_test", "output")

def read_txt(path):
    with open(path, "r") as f:
        data = [float(x.strip()) for x in f.readlines()]
    return np.array(data, dtype=np.float32)

def compare_arrays(name, golden, out, atol=1e-5, rtol=1e-5):
    if golden.shape != out.shape:
        print(f"[FAIL] {name}: shape mismatch {golden.shape} vs {out.shape}")
        return False

    diff = np.abs(golden - out)
    max_diff = diff.max() if diff.size > 0 else 0.0
    rel_diff = np.max(np.abs(diff / (golden + 1e-12)))

    ok = np.allclose(golden, out, atol=atol, rtol=rtol)
    if ok:
        print(f"[PASS] {name}: max_diff={max_diff:.6f}, rel_diff={rel_diff:.6f}")
    else:
        print(f"[FAIL] {name}: max_diff={max_diff:.6f}, rel_diff={rel_diff:.6f}")

    return ok


def main():
    print("=== COMPARING C++ OUTPUT WITH GOLDEN OUTPUT ===\n")

    files = sorted(os.listdir(golden_dir))
    passed = 0
    failed = 0

    for fname in files:
        golden_path = os.path.join(golden_dir, fname)
        out_path = os.path.join(output_dir, fname)

        if not os.path.exists(out_path):
            print(f"[MISSING] {fname} → no output file")
            failed += 1
            continue

        golden = read_txt(golden_path)
        out = read_txt(out_path)

        if compare_arrays(fname, golden, out):
            passed += 1
        else:
            failed += 1

    print("\n============================================")
    print(f"TOTAL PASS : {passed}")
    print(f"TOTAL FAIL : {failed}")
    print("============================================\n")

    if failed == 0:
        print("🎉 ALL TESTCASES PASSED!")
    else:
        print("❌ SOME TESTS FAILED. CHECK LOG ABOVE.")


if __name__ == "__main__":
    main()
