import sys
import os
import subprocess
import time
import matplotlib.pyplot as plt
import numpy as np

# read first CLI arg
if len(sys.argv) < 2:
    print("Usage: python run_benchmark.py <plot name / exercise >")
    exit(1)

show_plt = "--no-show" not in sys.argv[2:]


# get benchmark name
name = sys.argv[1]


if name == "ex2":
    def get_flops():
        return 11 + 50*(7 + 16*(42) + 1*(28))
    def percentage_peak_performance(perf):
        return perf/ 2
    # Open txt file and read line by line
    y = [0, 0, 0, 0]
    ydeviation = [0, 0, 0, 0]
    flopscycles = 0
    with open(f"out/ex2b.txt", "r") as file:
        for line in file:
            # Run benchmark
            tokens = line.split()
            n = int(tokens[0])
            flops = get_flops()
            
            tokens = tokens[1:]
            tokens = list(map(float, tokens))
            median = tokens[(len(tokens) // 2)]
            deviation = 0
            for token in tokens:
                deviation += (float(token) - median) ** 2
            deviation = (deviation / len(tokens)) ** 0.5
            y[n] = median
            ydeviation[n] = deviation
            if n == 3:
                flopscycles = flops/median
    
    # Bar graph
    color0 = "#C2C2C2"
    color1 = "#880000"
    color2 = "#D94700"
    color3 = "#4E937A"

    plt.figure(figsize=(15, 8), facecolor="white") # Making the plot wider
    bars = []
    bars.append(plt.bar(1, y[0], color=color0, capsize=5))
    bars.append(plt.bar(2, y[1], color=color1, capsize=5))
    bars.append(plt.bar(3, y[2], color=color2, capsize=5))
    bars.append(plt.bar(4, y[3], color=color3, capsize=5))
    for b in bars:
        for bar in b:
            yval = bar.get_height()
            plt.text(bar.get_x() + bar.get_width()/2, yval, f"{int(yval)//1000}'{int(yval)%1000:03d}", ha='center', va='bottom', fontsize=18, fontweight='bold')
    plt.gca().set_facecolor("#E4E4E4")
    plt.gca().yaxis.grid(True, which='major', color='w', linestyle='-', linewidth=1.5)
    yticks = np.arange(0, max(y)+200000, 100000)
    plt.ylabel("Runtime\n(cycles)", fontsize=18, rotation=0, labelpad=18, ha='right')
    plt.gca().set_yticks(yticks, minor=False)
    plt.gca().set_yticklabels([f"{int(i/1000)} K" for i in yticks])
    plt.xlabel("Function implementations", fontsize=18, rotation=0, labelpad=18, ha='center')
    plt.gca().set_xticks(np.arange(0, 5, 1), minor=False)
    plt.gca().set_xticklabels(["", "Baseline", "V1", "V2", "V3"], fontweight='bold', fontsize=15)
    plt.gca().set_axisbelow(True)
    with open(f"out/ex2_a_baseline.txt", "w") as file:
        file.write(f"{y[0]}")
    with open(f"out/ex2_a_v1.txt", "w") as file:
        file.write(f"{y[1]}")
    with open(f"out/ex2_a_v2.txt", "w") as file:
        file.write(f"{y[2]}")
    with open(f"out/ex2_a_v3.txt", "w") as file:
        file.write(f"{y[3]}")
    with open(f"out/ex2_b_flops.txt", "w") as file:
        file.write(f"{get_flops()}")
    with open(f"out/ex2_b.txt", "w") as file:
        file.write(f"{flopscycles:.3f}")
    with open(f"out/ex2_c.txt", "w") as file:
        file.write(f"{percentage_peak_performance(flopscycles)*100:.2f}")

    # Save plot as pdf
    plt.savefig(f"out/ex2_a.pdf")
    if show_plt:
        plt.show()






