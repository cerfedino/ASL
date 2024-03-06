all: out/ ex1 ex2 ex4 ex2benchmark ex4benchmark

clean:
	rm -rf ex1/*.s ex2/*.o ex4/*.o *.o out/ 
out/:
	mkdir -p out



ex1: ex1/fp_instruction_set.s
ex1/fp_instruction_set.s: ex1/fp_instruction_set.c
	gcc -S -O3 -o ex1/fp_instruction_set.s ex1/fp_instruction_set.c



# EXERCISE 2
ex2: ex2/ex2c_1.o ex2/ex2c_2.o ex2/ex2c_3.o
ex2/ex2c_1.o: ex2/lu.c 
	gcc -c -DEXTERNAL -O0 ex2/lu.c  -o ex2/ex2c_1.o
ex2/ex2c_2.o: ex2/lu.c
	gcc -c -DEXTERNAL -O3 -fno-tree-vectorize -fno-slp-vectorize ex2/lu.c -o ex2/ex2c_2.o
ex2/ex2c_3.o: ex2/lu.c
	gcc -c -DEXTERNAL -O3 -ffast-math -mcpu=apple-m1 ex2/lu.c -o ex2/ex2c_3.o

## BENCHMARK
ex2benchmark: ex2 out/ex2c_b1.txt out/ex2c_b2.txt out/ex2c_b3.txt out/ex2c.pdf
out/ex2c_b1.txt: ex2/ex2c_1.o benchmark.c
	gcc ex2/ex2c_1.o benchmark.c -o run_benchmark.o && ./run_benchmark.o ex2c_b1 out/ex2c_b1.txt
out/ex2c_b2.txt: ex2/ex2c_2.o benchmark.c
	gcc ex2/ex2c_2.o benchmark.c -o run_benchmark.o && ./run_benchmark.o ex2c_b2 out/ex2c_b2.txt
out/ex2c_b3.txt: ex2/ex2c_3.o benchmark.c
	gcc ex2/ex2c_3.o benchmark.c -o run_benchmark.o && ./run_benchmark.o ex2c_b3 out/ex2c_b3.txt

out/ex2c.pdf: out/ex2c_b1.txt out/ex2c_b2.txt out/ex2c_b3.txt run_benchmark.py
	python3 run_benchmark.py ex2c --no-show


# EXERCISE 4
ex4: ex4/ex4a_1.o ex4/ex4c_1.o
ex4/ex4a_1.o: ex4/lu4.c
	gcc -c -DEXTERNAL -O3 -ffast-math -mcpu=apple-m1 -fno-tree-vectorize -fno-slp-vectorize -ffp-contract=fast ex4/lu4.c -o ex4/ex4a_1.o
ex4/ex4c_1.o: ex4/lu4_optimized.c
	gcc -c -DEXTERNAL -O3 -ffast-math -mcpu=apple-m1 -fno-tree-vectorize -fno-slp-vectorize -ffp-contract=fast ex4/lu4_optimized.c -o ex4/ex4c_1.o

## BENCHMARK
ex4benchmark: out/ex4a_b1.txt out/ex4c_b1.txt out/ex4.pdf
out/ex4a_b1.txt: ex4/ex4a_1.o benchmark.c
	gcc ex4/ex4a_1.o benchmark.c -o run_benchmark.o && ./run_benchmark.o ex4a_b1 out/ex4a_b1.txt
out/ex4c_b1.txt: ex4/ex4c_1.o benchmark.c 
	gcc ex4/ex4c_1.o benchmark.c -o run_benchmark.o && ./run_benchmark.o ex4c_b1 out/ex4c_b1.txt
out/ex4.pdf: out/ex4a_b1.txt out/ex4c_b1.txt run_benchmark.py
	python3 run_benchmark.py ex4 --no-show