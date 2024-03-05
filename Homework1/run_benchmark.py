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



# get benchmark name
name = sys.argv[1]

if name == "ex2c":
    def flops(n):
        return n
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
        def get_flops(n):
            return n
        for line in file:
            # Run benchmark
            tokens = line.split()
            n = int(tokens[0])
            flops = get_flops(n)
            
            tokens = tokens[1:]
            # Calculate median and deviation of the rest of the tokens
            # Map tokens to float
            tokens = list(map(lambda token: flops/float(token), tokens))
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
            # Calculate median and deviation of the rest of the tokens
            # Map tokens to float
            tokens = list(map(lambda token: flops/float(token), tokens))
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
            # Calculate median and deviation of the rest of the tokens
            # Map tokens to float
            tokens = list(map(lambda token: flops/float(token), tokens))
            tokens.sort()
            median = tokens[(len(tokens) // 2)]
            deviation = 0
            for token in tokens:
                deviation += (float(token) - median) ** 2
            x3.append(n)
            y3.append(median)
            z3.append(deviation)
    # Plot graph x,y NO ERROBAR
    color1 = "#BF4E30"
    color2 = "#FF916A"
    color3 = "#4E937A"
    plt.figure(figsize=(20, 6), facecolor="white") # Making the plot wider
    line1, = plt.plot(x1, y1, '^-', color=color1)  # Added data point dots
    line2, = plt.plot(x2, y2, 'o-', color=color2)  # Added data point dots
    line3, = plt.plot(x3, y3, 's-', color=color3)  # Added data point dots
    plt.gca().set_facecolor("#E4E4E4")
    plt.gca().set_yticks(np.arange(min(y1+y2+y3), max(y1+y2+y3), (max(y1+y2+y3)-min(y1+y2+y3))/10), minor=False)
    plt.gca().set_xticks(np.arange(min(x1), max(x1)+100, 100), minor=False)
    plt.gca().yaxis.grid(True, which='major', color='w', linestyle='-', linewidth=1.5)
    plt.xlabel("Size n of the square matrix", fontsize=18, rotation=0, labelpad=10)
    plt.ylabel("flops/cycles", fontsize=15, rotation=0, labelpad=20, ha='right')
    # Labeling the lines at custom positions
    plt.text(x1[-3], y1[-2], "i) No optimizations", fontsize=12, va='center', ha='right', color=color1, fontweight='bold')
    plt.text(x2[-2], y2[-1], "ii) -O3, no vectorization", fontsize=12, va='center', ha='right', color=color2, fontweight='bold')
    plt.text(x3[-1], y3[9], "iii) All major optimizations", fontsize=12, va='top', ha='right', color=color3, fontweight='bold')
    # Save plot as pdf
    plt.savefig(f"out/ex2c.pdf")
    plt.show()






