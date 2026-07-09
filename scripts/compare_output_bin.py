import numpy as np

golden_txt = "/home/dung/Documents/GAN_HLS/res_block_param/rb8/output.txt"
hw_bin     = "/home/dung/Documents/GAN_HLS/output_relu_hardware.bin"

H, W, C    = 16, 16, 960
THRESHOLD  = 0.5

print("Loading golden (float32 from output.txt)...")
golden = np.fromstring(open(golden_txt).read(), sep="\n", dtype=np.float32)[:H*W*C]

print("Loading hardware output (fp16 binary)...")
hw = np.fromfile(hw_bin, dtype=np.float16).astype(np.float32)[:H*W*C]

print(f"Elements: {len(golden)}")

diff       = np.abs(golden - hw)
max_error  = float(diff.max())
mean_error = float(diff.mean())
mismatches = int((diff > THRESHOLD).sum())
worst_idx  = int(diff.argmax())
wh, ww, wc = worst_idx//(W*C), (worst_idx//C)%W, worst_idx%C

print("-" * 50)
print(f"Max Absolute Error  : {max_error:.6f}  at [h={wh}, w={ww}, c={wc}]")
print(f"  golden = {golden[worst_idx]:.6f}")
print(f"  hw     = {hw[worst_idx]:.6f}")
print(f"Mean Absolute Error : {mean_error:.6f}")
print(f"Mismatches (>{THRESHOLD}) : {mismatches} / {H*W*C}")
print("-" * 50)
print("First 10 values:")
print(f"  golden  : {golden[:10]}")
print(f"  hardware: {hw[:10]}")

if mismatches == 0 and max_error < THRESHOLD:
    print(f"\n[PASS] Hardware matches golden (max_err={max_error:.4f} < {THRESHOLD})")
else:
    print(f"\n[FAIL] {mismatches} mismatches, max_err={max_error:.4f}")
