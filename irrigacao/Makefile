# comentario -> para executar digite make 

all: compile
	
compile:
	clear
	gcc -o 1 irrigacao_seq.c
	gcc -o 2 irrigacao_pthread.c -O3 -lpthread
	gcc -o 3 irrigacao_openmp.c -O3 -fopenmp

	@echo " "
	@echo "Compilaçao concluida"
	@echo " "

clean:
	rm -rf *.out

