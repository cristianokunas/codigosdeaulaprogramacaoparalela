all: compile
	
compile:
	gcc -o 1 criartxt.c
	gcc -o 2 busca_seq.c -O3
	mpicc -o 3 busca_mpi.c -O3

clean:
	rm -rf ?
	rm -rf *.txt
	rm -rf *.out
