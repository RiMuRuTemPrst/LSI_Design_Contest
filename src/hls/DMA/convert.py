#!/usr/bin/env python3
from pathlib import Path

BIN_PATH = Path("input.bin")         # file binary bạn nạp XSCT
TXT_PATH = Path("input_bin.txt")     # file text gốc (16-bit 0/1 mỗi dòng)
N = 16

def u16_to_bin16(x: int) -> str:
    return format(x & 0xFFFF, "016b")

def swap16(x: int) -> int:
    return ((x & 0x00FF) << 8) | ((x & 0xFF00) >> 8)

def read_first_u16_list_from_bin(path: Path, n: int):
    data = path.read_bytes()
    if len(data) < 2*n:
        raise ValueError(f"{path} too small: {len(data)} bytes, need at least {2*n}")
    out = []
    for i in range(n):
        lo = data[2*i]
        hi = data[2*i + 1]
        out.append(lo | (hi << 8))   # interpret as little-endian u16
    return out

def read_first_lines_from_txt(path: Path, n: int):
    lines = []
    with path.open("r", newline=None) as f:
        for line in f:
            s = line.strip()
            if not s:
                continue
            lines.append(s)
            if len(lines) >= n:
                break
    return lines

def main():
    u16s = read_first_u16_list_from_bin(BIN_PATH, N)
    txt = read_first_lines_from_txt(TXT_PATH, N)

    print(f"== Compare first {N} fp16 patterns ==")
    print(f"BIN: {BIN_PATH}  |  TXT: {TXT_PATH}\n")

    # Header
    print(f"{'idx':>3}  {'txt':16}  {'bin_le':16}  {'bin_be(swapped)':16}  match_le  match_be")
    print("-"*3 + "  " + "-"*16 + "  " + "-"*16 + "  " + "-"*16 + "  " + "-"*8 + "  " + "-"*8)

    for i in range(N):
        txt_i = txt[i] if i < len(txt) else "(missing)"
        le = u16_to_bin16(u16s[i])
        be = u16_to_bin16(swap16(u16s[i]))

        match_le = (txt_i == le)
        match_be = (txt_i == be)

        print(f"{i:>3}  {txt_i:16}  {le:16}  {be:16}  {str(match_le):>8}  {str(match_be):>8}")

    print("\nTip:")
    print("- Nếu cột match_le True => input.bin đúng endian so với PS (little-endian).")
    print("- Nếu cột match_be True => file đang bị đảo byte, cần swap khi đọc (hoặc sửa convert).")

if __name__ == "__main__":
    main()
