#!/usr/bin/env python3
# Generates UCB_Timing_path_current.svg — corrected UCB timing diagram
# matching the synthesized full_generator design (PEs_U=8, post-[A]/[B]/[C-pipe], 2026-06-01).
# All cycle numbers are deterministic, derived from per-loop csynth iteration latency x real trip counts.

# ---- deterministic numbers (UCB_0: 16x16x960 -> 32x32x480) ----
PEs, CI, CWO, WIN, WOUT, COUT = 8, 60, 30, 16, 32, 480
NT       = COUT // PEs                 # 60 tiles
load     = WIN*CI + 3                  # 963   LOAD IFM ROW
reset    = WOUT*CWO + 2                # 962   RESET_ROW_ACC (II=1)
wflat    = 9*CI + 3                    # 543   W_FLAT per PE
preload  = PEs*wflat                   # 4344  PRELOAD_W per tile
mac      = 3*WIN*CI + 116              # 2996  KW_LOOP_FLAT_LOOP per valid kh (II=1, iter-lat 116)
lp       = 162                         # LOAD_PARAMS (II=3)
pstats   = WOUT*COUT + 179             # 15539 PIXEL_STATS (II=1)
pnorm    = WOUT*CWO + 93               # 1053  PIXEL_NORM  (II=1)
tile_e   = preload + 1*mac             # 7340
tile_o   = preload + 2*mac             # 10336
tloop_o  = NT*tile_o                   # 620160
norm     = pstats + pnorm              # 16592
row_e    = reset + NT*tile_e + lp + norm   # 458116
row_o    = reset + NT*tile_o + lp + norm   # 637876
total    = 17*row_e + 15*row_o             # 17,356,112
ms       = total/300e6*1000

import unicodedata
def asc(s):
    s=str(s).replace("đ","d").replace("Đ","D")
    s=unicodedata.normalize("NFD",s)
    s="".join(c for c in s if unicodedata.category(c)!="Mn")
    for a,b in [("→","->"),("×","x"),("≈","~"),("—","-"),("·","."),("Σ","Sum "),("⚠","(!)")]:
        s=s.replace(a,b)
    return s

C = dict(load="#9DC3E6", comp="#F4B183", compE="#FBE5D6", reset="#BFBFBF",
         pre="#9DC3E6", mac="#F4B183", lp="#FFD966", ps="#A9D08E", pn="#C6E0B4",
         hl="#C00000", ink="#222222", brk="#404040")

S = []
def r(x,y,w,h,fill,stroke="#333",sw=1,rx=3):
    S.append(f'<rect x="{x:.1f}" y="{y:.1f}" width="{w:.1f}" height="{h:.1f}" rx="{rx}" '
             f'fill="{fill}" stroke="{stroke}" stroke-width="{sw}"/>')
def t(x,y,s,sz=13,anc="middle",fill="#222",w="normal",sty="normal"):
    s=asc(s).replace("&","&amp;").replace("<","&lt;").replace(">","&gt;")
    S.append(f'<text x="{x:.1f}" y="{y:.1f}" font-size="{sz}" text-anchor="{anc}" '
             f'fill="{fill}" font-weight="{w}" font-style="{sty}">{s}</text>')
def tlines(x,y,lines,sz=12,anc="middle",fill="#222",lh=14,w="normal"):
    for i,ln in enumerate(lines): t(x,y+i*lh,ln,sz,anc,fill,w)
def line(x1,y1,x2,y2,col="#404040",sw=1.4,dash=""):
    d=f' stroke-dasharray="{dash}"' if dash else ""
    S.append(f'<line x1="{x1:.1f}" y1="{y1:.1f}" x2="{x2:.1f}" y2="{y2:.1f}" stroke="{col}" stroke-width="{sw}"{d}/>')
def brace(x1,x2,y,txt,col="#404040",sz=12,up=True,tcol="#222"):
    line(x1,y,x2,y,col,1.4); tick=6 if up else -6
    line(x1,y,x1,y+(tick if up else -tick),col,1.4); line(x2,y,x2,y+(tick if up else -tick),col,1.4)
    t((x1+x2)/2, y-6 if up else y+16, txt, sz, "middle", tcol, "bold")

W,H = 1760, 1260
S.append(f'<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="{H}" viewBox="0 0 {W} {H}" font-family="Segoe UI, Arial, sans-serif">')
r(0,0,W,H,"#FFFFFF","#FFFFFF")
t(W/2,34,"UCB Timing Path — trạng thái HIỆN TẠI (full_generator, PEs_U=8, post-[A]/[B]/[C-pipe])",19,"middle","#1F3864","bold")
t(W/2,55,"Số cycle = deterministic (iteration-latency × trip-count thật từ csynth). Khung minh hoạ — KHÔNG theo tỉ lệ.",12.5,"middle","#666")

# arrowhead marker
S.append('<defs><marker id="ar" markerWidth="9" markerHeight="9" refX="7" refY="3" orient="auto">'
         '<path d="M0,0 L7,3 L0,6 Z" fill="#404040"/></marker></defs>')

# ============================================================== SECTION 1
y0=90
t(40,y0,"1.  Top Module — OFM 32×32×480 (UCB_0)",16,"start","#1F3864","bold")
lane_load, lane_comp = y0+44, y0+118
t(40,lane_load+20,"LOAD IFM ROW",12.5,"start","#333","bold"); t(40,lane_load+36,"[1×16×960]",11,"start","#777")
t(40,lane_comp+20,"Compute OFM ROW",12.5,"start","#333","bold"); t(40,lane_comp+36,"[1×32×480]",11,"start","#777")

# sequential sequence: L0,C0,L1,C1,C2,L2,C3,C4, ... ,L15,C29,C30,C31
seq=[("L","0"),("C","0","e"),("L","1"),("C","1","o"),("C","2","e"),
     ("L","2"),("C","3","o"),("C","4","e"),("...",),
     ("L","15"),("C","29","o"),("C","30","e"),("C","31","e","biên: kh=1")]
x=240; LW,CW,gap=66,104,10; lh=46
first_pair=[]
for el in seq:
    if el[0]=="...":
        t(x+22,(lane_load+lane_comp)/2+20,"·  ·  ·",20,"middle","#555","bold"); x+=70; continue
    if el[0]=="L":
        r(x,lane_load,LW,lh,C["load"]); tlines(x+LW/2,lane_load+19,[f"LOAD {el[1]}","963"],11.5)
        if el[1]=="1": first_pair.append(x)
        x+=LW+gap
    else:
        cls=el[2]; note=el[3] if len(el)>3 else ""
        fill=C["comp"] if cls=="o" else C["compE"]
        r(x,lane_comp,CW,lh,fill)
        cyc = "637,876" if cls=="o" else "458,116"
        tlines(x+CW/2,lane_comp+16,[f"Compute {el[1]}",cyc+" cy"],11.5)
        if note: t(x+CW/2,lane_comp+lh-5,note,8.5,"middle",C["hl"])
        if el[1] in("1","2"): first_pair.append(x+CW)
        x+=CW+gap
xend=x
# bracket "1 input -> 2 output"
bx1=first_pair[0]-LW-gap
brace(bx1, first_pair[-1], lane_comp+lh+22, "1 input row  →  2 output rows  (ConvT stride-2)", C["brk"],12,False)
# sequential note
t(xend+12,lane_load+8,"TUẦN TỰ:",12,"start",C["hl"],"bold")
tlines(xend+12,lane_load+26,["ROW_LOOP không PIPELINE/","DATAFLOW → LOAD và","COMPUTE KHÔNG chồng lấn"],11.5,"start","#333",15)
# total banner
r(240,lane_comp+lh+40,xend-240,30,"#FCE4D6","#C00000",1.4)
t((240+xend)/2,lane_comp+lh+60,f"Σ UCB_0  =  17 × {row_e:,} (ho chẵn)  +  15 × {row_o:,} (ho lẻ)  =  {total:,} cycles  =  {ms:.2f} ms @ 300 MHz",
  13,"middle","#833C00","bold")

# ============================================================== SECTION 2
y1=lane_comp+lh+110
line(40,y1-22,W-40,y1-22,"#CCC",1)
t(40,y1,"2.  Compute Row  =  1 lần gọi UpConv_Fused_Row  (ví dụ ho lẻ, valid_kh = 2)",16,"start","#1F3864","bold")
ly=y1+40; bh=52
# stage boxes sequential (schematic widths)
stages=[("RESET_ROW_ACC", f"{reset:,} cy","II=1", C["reset"],120),
        ("TILE_LOOP ×60", "", "", None, 560),
        ("LOAD_PARAMS", f"{lp} cy","II=3", C["lp"],150),
        ("PIXEL_STATS", f"{pstats:,} cy","II=1·pass1", C["ps"],150),
        ("PIXEL_NORM", f"{pnorm:,} cy","II=1·pass2", C["pn"],140)]
x=240
tile_x0=tile_x1=0
for name,cyc,ii,fill,w in stages:
    if name=="TILE_LOOP ×60":
        tile_x0=x
        # interleaved P0 M0 P1 M1 ... P59 M59
        sub=[("P","0"),("M","0"),("P","1"),("M","1"),("...",),("P","59"),("M","59")]
        sx=x; pw,mw,sg=42,52,5
        for s in sub:
            if s[0]=="...":
                t(sx+16,ly+bh/2+5,"· · ·",16,"middle","#555","bold"); sx+=46; continue
            if s[0]=="P":
                r(sx,ly,pw,bh,C["pre"]); tlines(sx+pw/2,ly+22,[f"PRE","4,344"],10.5); sx+=pw+sg
            else:
                r(sx,ly,mw,bh,C["mac"]); tlines(sx+mw/2,ly+19,[f"MAC","2×2,996"],10.5); sx+=mw+sg
        tile_x1=sx-sg
        x=sx+14
    else:
        r(x,ly,w,bh,fill);
        lbl=[name,cyc]+([ii] if ii else [])
        tlines(x+w/2,ly+18,lbl,11)
        if name=="LOAD_PARAMS":
            t(x+w/2,ly-14,"⚠ nằm GIỮA (sau 60 tiles),",11,"middle",C["hl"],"bold")
            t(x+w/2,ly-1,"KHÔNG ở đầu",11,"middle",C["hl"],"bold")
        x+=w+14
sec2_end=x
brace(tile_x0,tile_x1,ly-12,f"TILE_LOOP ×60  =  60 × (PRELOAD_W 4,344 + MAC 2×2,996)  =  {tloop_o:,} cy",C["brk"],11.5,True)
# norm bracket
ps_x = sec2_end - (150+14) - (140) - 14
brace(ps_x, sec2_end-14, ly+bh+20, f"NORM & WRITE (2-pass flatten)  =  {norm:,} cy", C["brk"],11.5,False)
r(240,ly+bh+40,sec2_end-14-240,28,"#FFF2CC","#BF8F00",1.3)
t((240+sec2_end-14)/2,ly+bh+59,
  f"Compute Row (ho lẻ)  =  {row_o:,} cy      |      (ho chẵn, valid_kh=1)  =  {row_e:,} cy",13,"middle","#7F6000","bold")

# ============================================================== SECTION 3
y2=ly+bh+120
line(40,y2-24,W-40,y2-24,"#CCC",1)
t(40,y2,"3.  Zoom 1 TILE  (8 output channels — PEs_U=8)",16,"start","#1F3864","bold")
zy=y2+44; zbh=50
t(40,zy+22,"PRELOAD_W",12.5,"start","#333","bold"); t(40,zy+38,"(reload mỗi row)",10.5,"start","#777")
x=240
for pe in range(8):
    r(x,zy,70,zbh,C["pre"]); tlines(x+35,zy+22,[f"PE{pe}","W_FLAT 543"],10.5); x+=70+8
brace(240,x-8,zy-12,f"PRELOAD_W = 8 × (9·CI_WORDS=540, +3)  =  {preload:,} cy",C["brk"],11.5,True)
px_end=x
# MAC lane
my=zy+96
t(40,my+22,"MAC Array",12.5,"start","#333","bold"); t(40,my+38,"(KW_LOOP_FLAT_LOOP)",10.5,"start","#777")
x=240
for i,kh in enumerate(("kh = a","kh = b")):
    r(x,my,250,zbh,C["mac"]); tlines(x+125,my+20,[f"{kh}:  KW×WI×CI = 3×16×60 = 2,880","+ fill 116  →  2,996 cy  · II=1"],10.5); x+=250+12
brace(240,x-12,my-12,f"MAC = valid_kh(2) × 2,996  =  {2*mac:,} cy",C["brk"],11.5,True)
t(x+18,my+22,"opt [A]: FLAT_LOOP gộp KW×WI×CI",11,"start","#333")
t(x+18,my+38,"→ fill 116 trả 1 lần / kh (II=1)",11,"start","#333")
# tile total
r(240,my+zbh+22,px_end-240,26,"#FBE5D6","#C55A11",1.3)
t((240+px_end)/2,my+zbh+40,f"1 TILE (ho lẻ)  =  4,344 + 5,992  =  {tile_o:,} cy        (ho chẵn = {tile_e:,} cy)",12.5,"middle","#833C00","bold")

# footer note
fy=my+zbh+78
line(40,fy-14,W-40,fy-14,"#CCC",1)
t(40,fy,"Khác bản cũ:  (1) MAC/tile = FLAT_LOOP ~6K cy, KHÔNG phải 153   (2) Norm = 2-pass 16,592 cy, KHÔNG phải 113,792   "
        "(3) LOAD_PARAMS nằm giữa   (4) LOAD↔Compute tuần tự.",
  12,"start","#444")
t(40,fy+20,"Toàn UpConv (4 UCB, deterministic post-[A]/[B]/[C-pipe]):  UCB0 57.7 + UCB1 46.2 + UCB2 43.7 + UCB3 58.8  ≈  203.7 ms @ 300 MHz.",
  12,"start","#444")

S.append("</svg>")
open("docs/images/UCB_Timing_path_current.svg","w").write("\n".join(S))
print("wrote docs/images/UCB_Timing_path_current.svg")
print(f"UCB0 total={total:,} cy = {ms:.2f} ms | row_e={row_e:,} row_o={row_o:,}")
