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

CPU_FREQ = 2.53*(10**9)
MAX_FLOPS = 8

if name == "ex2c":
    def get_flops(n):
        return (2*(n**3)) + (n**2)
    # Open txt file and read line by line
    x1 = []
    y1 = []
    z1 = []
    x2 = []
    y2 = []
    z2 = []
    x3 = []
    y3 = []
    z3 = []
    with open(f"out/ex2c_b1.txt", "r") as file:
        for line in file:
            # Run benchmark
            tokens = line.split()
            n = int(tokens[0])
            flops = get_flops(n)
            
            tokens = tokens[1:]
            tokens = list(map(lambda token: flops/(float(token)*CPU_FREQ), tokens))
            tokens.sort()
            median = tokens[(len(tokens) // 2)]
            deviation = 0
            for token in tokens:
                deviation += (float(token) - median) ** 2
            x1.append(n)
            y1.append(median)
            z1.append(deviation)
    with open(f"out/ex2c_b2.txt", "r") as file:
        for line in file:
            # Run benchmark
            tokens = line.split()
            n = int(tokens[0])
            flops = get_flops(n)
            
            tokens = tokens[1:]
            tokens = list(map(lambda token: flops/(float(token)*CPU_FREQ), tokens))
            tokens.sort()
            median = tokens[(len(tokens) // 2)]
            deviation = 0
            for token in tokens:
                deviation += (float(token) - median) ** 2
            x2.append(n)
            y2.append(median)
            z2.append(deviation)
    with open(f"out/ex2c_b3.txt", "r") as file:
        for line in file:
            # Run benchmark
            tokens = line.split()
            n = int(tokens[0])
            flops = get_flops(n)
            
            tokens = tokens[1:]
            tokens = list(map(lambda token: flops/(float(token)*CPU_FREQ), tokens))
            tokens.sort()
            median = tokens[(len(tokens) // 2)]
            deviation = 0
            for token in tokens:
                deviation += (float(token) - median) ** 2
            x3.append(n)
            y3.append(median)
            z3.append(deviation)
    bestY = f"{max(y3):.2f}"
    # Plot graph x,y NO ERROBAR
    color1 = "#BF4E30"
    color2 = "#FF916A"
    color3 = "#4E937A"
    plt.figure(figsize=(15, 6), facecolor="white") # Making the plot wider
    line1, = plt.plot(x1, y1, '^-', color=color1)  # Added data point dots
    line2, = plt.plot(x2, y2, 'o-', color=color2)  # Added data point dots
    line3, = plt.plot(x3, y3, 's-', color=color3)  # Added data point dots
    plt.gca().set_facecolor("#E4E4E4")
    plt.gca().set_yticks(np.arange(0, MAX_FLOPS+1, 1), minor=False)
    plt.gca().set_xticks(np.arange(min(x1), max(x1)+100, 100), minor=False)
    plt.gca().yaxis.grid(True, which='major', color='w', linestyle='-', linewidth=1.5)
    plt.xlabel("Size $n$ of the square matrix", fontsize=18, rotation=0, labelpad=10)
    plt.ylabel("flops/cycles", fontsize=18, rotation=0, labelpad=18, ha='right')
    # Labeling the lines at custom positions
    plt.text(1500, 1.5, "i) No optimizations", fontsize=12, va='center', ha='right', color=color1, fontweight='bold')
    plt.text(1800, 3, "ii) -O3, no vectorization", fontsize=12, va='center', ha='right', color=color2, fontweight='bold')
    plt.text(2000, 6.5, "iii) All major optimizations", fontsize=12, va='top', ha='right', color=color3, fontweight='bold')
    plt.text(2000, 8, f"Best: {bestY}", fontsize=25, va='center', ha='right', color="black", fontweight='bold')
    with open(f"out/ex2c_best.txt", "w") as file:
        file.write(f"{bestY}")

    # Save plot as pdf
    plt.savefig(f"out/ex2c.pdf")
    if show_plt:
        plt.show()

elif name == "ex4":
    def get_flops_1(n):
        return n*2*2
    def get_flops_2(n):
        return n*2*2
    # Open txt file and read line by line
    x1 = []
    y1 = []
    z1 = []
    x2 = []
    y2 = []
    z2 = []
    with open(f"out/ex4a_b1.txt", "r") as file:
        for line in file:
            # Run benchmark
            tokens = line.split()
            n = int(tokens[0])
            flops = get_flops_1(n)
            
            tokens = tokens[1:]
            # Calculate median and deviation of the rest of the tokens
            # Map tokens to float
            tokens = list(map(lambda token: flops/(float(token)*CPU_FREQ), tokens))
            tokens.sort()
            median = tokens[(len(tokens) // 2)]
            deviation = 0
            for token in tokens:
                deviation += (float(token) - median) ** 2
            x1.append(np.log2(n))
            y1.append(median)
            z1.append(deviation)
    with open(f"out/ex4c_b1.txt", "r") as file:
        for line in file:
            # Run benchmark
            tokens = line.split()
            n = int(tokens[0])
            flops = get_flops_2(n)
            
            tokens = tokens[1:]
            # Calculate median and deviation of the rest of the tokens
            # Map tokens to float
            tokens = list(map(lambda token: flops/(float(token)*CPU_FREQ), tokens))
            tokens.sort()
            median = tokens[(len(tokens) // 2)]
            deviation = 0
            for token in tokens:
                deviation += (float(token) - median) ** 2
            x2.append(np.log2(n))
            y2.append(median)
            z2.append(deviation)
    bestY = f"{max(y2):.2f}"
    # Plot graph x,y NO ERROBAR
    color1 = "#BF4E30"
    color2 = "#4E937A"
    plt.figure(figsize=(20, 6), facecolor="white")
    line1, = plt.plot(x1, y1, '^-', color=color1)
    line2, = plt.plot(x2, y2, 'o-', color=color2)
    plt.gca().set_facecolor("#E4E4E4")
    # plt.gca().set_yticks(np.arange(min(y1+y2), max(y1+y2), (max(y1+y2)-min(y1+y2))/10), minor=False)
    plt.gca().set_xticks(np.arange(min(x1), max(x1)+1, 1), minor=False)
    plt.gca().set_yticks(np.arange(0, MAX_FLOPS+1, 1), minor=False)
    # Mapping each xtick label to something different
    plt.gca().set_xticklabels([f"$2^{{{int(i)}}}$" for i in np.arange(min(x1), max(x1)+1, 1)])
    plt.gca().yaxis.grid(True, which='major', color='w', linestyle='-', linewidth=1.5)
    plt.xlabel("Input parameter $n$", fontsize=18, rotation=0, labelpad=10)
    plt.ylabel("flops/cycles", fontsize=15, rotation=0, labelpad=20, ha='right')
    # plt.xscale("log")
    # Labeling the lines at custom positions
    plt.text(23, 2, "Unoptimized function", fontsize=12, va='center', ha='right', color=color1, fontweight='bold')
    plt.text(28, 3, "ILP optimized function", fontsize=12, va='center', ha='right', color=color2, fontweight='bold')
    plt.text(28, 7, f"Best: {bestY}", fontsize=25, va='center', ha='right', color="black", fontweight='bold')
    with open(f"out/ex4_best.txt", "w") as file:
        file.write(f"{bestY}")
    # Save plot as pdf
    plt.savefig(f"out/ex4.pdf")
    if show_plt:
        plt.show()







