import os

ips = [
    ("Relu", "src/hls/Relu/prj/solution1/syn/report/Relu_IP_csynth.rpt"),
    ("Add", "src/hls/Add/prj/solution1/syn/report/Add_IP_csynth.rpt"),
    ("ChannelNorm", "src/hls/ChannelNorm/prj/solution1/syn/report/ChannelNorm_IP_csynth.rpt"),
    ("Conv3x3", "src/hls/Conv3x3/prj/solution1/syn/report/Conv3x3_IP_csynth.rpt")
]

for name, path in ips:
    if not os.path.exists(path):
        print(f"IP {name}: Report not found")
        continue
        
    latency, timing, bram, dsp, ff, lut, uram = "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A"
    
    with open(path, "r") as f:
        lines = f.readlines()
        
    for i, line in enumerate(lines):
        if "|ap_clk" in line:
            parts = line.split('|')
            if len(parts) >= 4: timing = parts[3].strip()
        if "|Latency (cycles)" in line:
            # Look at 4 lines below for the summary
            if i + 4 < len(lines):
                parts = lines[i+4].split('|')
                if len(parts) >= 3: latency = f"{parts[1].strip()} ~ {parts[2].strip()}"
        if "|Total" in line and "|Instance" not in lines[i-1] and "|DSP" not in lines[i-1]:
            # This is tricky because there are multiple "Total" rows. 
            # The one we want is in the "Utilization Estimates" -> "Summary" section.
            # Usually it has 5-6 numerical columns.
            parts = [p.strip() for p in line.split('|')]
            if len(parts) >= 7 and parts[1] == "Total":
                bram, dsp, ff, lut, uram = parts[2], parts[3], parts[4], parts[5], parts[6]

    print(f"--- IP: {name} ---")
    print(f"Latency: {latency} cycles")
    print(f"Timing (Estimated): {timing}")
    print(f"Utilization -> BRAM: {bram}, DSP: {dsp}, FF: {ff}, LUT: {lut}, URAM: {uram}\n")
