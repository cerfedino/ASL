ex: ex1 ex2
clean:
	rm -f *.out


ex1: ex1.out
	./ex1.out
ex1.out: ex1.c cache.c cache.h
	gcc ex1.c cache.c -o ex1.out

ex2: ex2.out
	./ex2.out
ex2.out: ex2a.c cache.c cache.h
	gcc ex2a.c cache.c -o ex2.out