import matplotlib.pyplot as plt
from mpl_toolkits.axisartist.axislines import SubplotZero
import numpy as np

XMIN = 0.03125
XMAX = 2**7

fig = plt.figure(figsize=(15, 6), facecolor="white")
plt.gca().set_facecolor("#E4E4E4")


SIMD_LEN = 256
GB = 10**9
FREQ = 2 * (10**9)
MEM_BW = 30 * (10**9)

INT32 = 32

#### Exercise 3a
###  No SIMD
PEAK_scalar = 3
MEM_BW = MEM_BW / FREQ

#  Memory bound
x = np.linspace(XMIN, XMAX, 100)
y_mem_scalar = [(MEM_BW * i) for i in x ]
plt.plot(x, y_mem_scalar, color='#55575C', linestyle='--', linewidth=1.4)
#  Compute bound
y_comp_scalar = [PEAK_scalar for i in x]
plt.plot(x, y_comp_scalar, color='#55575C', linestyle='--', linewidth=1.4)
plt.text(0.04,y_comp_scalar[0], f"Peak $\pi$ scalar ({y_comp_scalar[0]} iops/cycle)", fontsize=12, va='bottom', ha='left', color='#55575C') 

plt.text(0.035, 0.6, f"Read $\\beta$ ({MEM_BW} Bytes/Cycle)", fontsize=12, va='bottom', ha='left', color='#55575C', rotation=33)

# plt.plot(x, [min(y_mem_scalar[i], y_comp_scalar[i]) for i in range(len(x))], color='black', linestyle='-', linewidth=1.4)

### With SIMD
PEAK_simd = 3 * (SIMD_LEN/INT32)
y_comp_simd = [PEAK_simd for i in x]
# Compute bound
plt.plot(x, y_comp_simd, color='#55575C', linestyle='--', linewidth=1.4)
plt.text(0.04,y_comp_simd[0], f"Peak $\pi$ SIMD ({y_comp_simd[0]} iops/cycle)", fontsize=12, va='bottom', ha='left', color='#55575C')


#  Roofline plot

# color1 = "#BF4E30"
# color2 = "#FF916A"
# color3 = "#4E937A"
# ax = SubplotZero(fig, 111)
# fig.add_subplot(ax)
# for direction in ["xzero", "yzero"]:
#     ax.axis[direction].set_axisline_style("-|>", size=2)
#     ax.axis[direction].set_visible(True)
# for direction in ["left", "right", "bottom", "top"]:
#     ax.axis[direction].set_visible(False)
# line1, = plt.plot(x1, y1, '^-', color=color1)  # Added data point dots
# line2, = plt.plot(x2, y2, 'o-', color=color2)  # Added data point dots
# line3, = plt.plot(x3, y3, 's-', color=color3)  # Added data point dots

plt.gca().yaxis.grid(True, which='major', color='w', linestyle='-', linewidth=1.5)
plt.xlabel("Operational Intensity I(n) [iops/byte]", fontsize=14, rotation=0, labelpad=5, ha='center')
plt.ylabel("Performance P(n) [iops/cycle]", fontsize=14, rotation=0, ha='left', labelpad=-15, position=(1,1))

# Plot bounds
plt.ylim(np.min(y_mem_scalar), 2**6)
plt.xlim(np.min(x), XMAX)
plt.xscale("log", base=2)
plt.yscale("log", base=2)

# plt.text(1500, 1.5, "i) No optimizations", fontsize=12, va='center', ha='right', color=color1, fontweight='bold')
# plt.text(1800, 3, "ii) -O3, no vectorization", fontsize=12, va='center', ha='right', color=color2, fontweight='bold')
# plt.text(2000, 6.5, "iii) All major optimizations", fontsize=12, va='top', ha='right', color=color3, fontweight='bold')
# plt.text(2000, 8, f"Best: {bestY}", fontsize=25, va='center', ha='right', color="black", fontweight='bold')


#### Exercise 3b
comp1_PEAK_scalar = 2
comp1_I = 3/16


#  Memory bound
comp1_P_scalar = min(comp1_PEAK_scalar, MEM_BW * comp1_I)
print("comp1_P_scalar", comp1_P_scalar)
# Plot point
plt.plot(comp1_I, comp1_P_scalar, 'o', color='black', label="comp1")
plt.text(comp1_I, comp1_P_scalar, "\ncomp1", fontsize=8, ha='center', va='top')
#  Compute bound
y_comp_scalar = [comp1_PEAK_scalar for i in x]
plt.plot(x, y_comp_scalar, color='#55575C', linestyle='--', linewidth=1.4)
plt.text(0.04,y_comp_scalar[0], f"Peak $\pi$ scalar ({y_comp_scalar[0]} iops/cycle)", fontsize=12, va='bottom', ha='left', color='#55575C') 







comp2_PEAK_scalar = 3
comp2_I = 3/16

#  Memory bound
comp2_P_scalar = min(comp2_PEAK_scalar, MEM_BW * comp2_I)
print("comp2_P_scalar", comp2_P_scalar)
# Plot point
plt.plot(comp2_I, comp2_P_scalar, 'o', color='black', label="comp2")
plt.text(comp2_I, comp2_P_scalar, "\ncomp2", fontsize=8, ha='center', va='top')
#  Compute bound
# y_comp_scalar = [comp2_PEAK_scalar for i in x]
# plt.plot(x, y_comp_scalar, color='#55575C', linestyle='--', linewidth=1.4)
# plt.text(0.04,y_comp_scalar[0], f"Peak $\pi$ scalar ({y_comp_scalar[0]} iops/cycle)", fontsize=12, va='bottom', ha='left', color='#55575C')


#### Exercise 3c


comp1_peak_simd = comp1_PEAK_scalar * (SIMD_LEN/INT32)
# Memory bound
comp1_P_simd = min(comp1_peak_simd, MEM_BW * comp1_I)
comp1_speedup = comp1_P_simd / comp1_P_scalar
print("comp1_speedup", comp1_speedup)

comp2_peak_simd = comp2_PEAK_scalar * (SIMD_LEN/INT32)
# Memory bound
comp2_P_simd = min(comp2_peak_simd, MEM_BW * comp2_I)
comp2_speedup = comp2_P_simd / comp2_P_scalar
print("comp2_speedup", comp1_speedup)



plt.legend(loc='upper right', fontsize=12)

with open("out/ex3c_comp1_speedup.tex", "w") as f:
    f.write(f"{comp1_speedup}")
with open("out/ex3c_comp2_speedup.tex", "w") as f:
    f.write(f"{comp2_speedup}")


plt.savefig("out/ex3a.pdf")
plt.show()
