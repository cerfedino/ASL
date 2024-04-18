ex: ex1 ex2 plots
clean:
	rm -r *.out out/


ex1: ex1.out
	./ex1.out
ex1.out: ex1.c cache.c cache.h
	gcc ex1.c cache.c -o ex1.out

ex2: ex2.out
	./ex2.out
ex2.out: ex2.c cache.c cache.h
	gcc ex2.c cache.c -o ex2.out

plots: out/
	python3 ex3.py


out/:
	mkdir -p out/