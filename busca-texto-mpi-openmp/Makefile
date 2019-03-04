all: compile
	
compile:
	gcc -o 1 1criar.c
	gcc -o 2 2pesquisar.c
	gcc -o 3 3posicionar.c 
	mpicc -o 4 mpiopenmp.c -O3 -fopenmp

clean:
	rm -rf ?
	rm -rf *.txt
	rm -rf *.out
