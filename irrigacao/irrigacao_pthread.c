#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "tempo.h"

#define L 216
#define C L
#define P L
#define ITER 300

double a[C][L][P];
double b[C][L][P];

void inicializa_cubo();
void mostra_cubo();
void irriga();
void *propagacao(void *arga);
void evapora();
void move_matriz();

int NUMTHREAD;

struct divisao
    {
        int inicio;
        int final;
    };

int main(int argc, char *argv[]){
	//Recebe numero de threads
	NUMTHREAD = atoi(argv[1]);
	int d, i,c, div, fim, resto, inicio[NUMTHREAD+1];
	//solicita numero de threads caso seja digitado numero menor que 1
	if (NUMTHREAD < 1){
		printf("Digite ./nome do executavel + numero de thread");
		return 0;
	}

	// div recebe a quantidade de linhas que cada thread vai trabalha
    div = ((L - 1)/NUMTHREAD);

    //cria struct divisao de tamanho NUMTHREAD
	struct divisao divisao[NUMTHREAD];		

	 //divisão do trabalho
    for(c=0; c < NUMTHREAD; c++){
         //se c ==0 
        if(c == 0){
        	//struct divisao recebe casting {inicio 1, final div+1}
	        divisao[c] = (struct divisao){ 1, (div + 1)};
	        //fim recebe div+1
			fim = (div + 1);
        }else{
        	if(c == (NUMTHREAD - 1)){
         	resto = (L - fim);
            divisao[c] = (struct divisao){ fim, ((fim + resto) - 1)};
        	}else{
        	//senão struct divisao recebe casting {inicio = fim, final (div+fim)}
        	divisao[c] = (struct divisao){ fim, (fim + div)};
        	//novo fim recebe novo fim+div
        	fim = (fim + div);
        }
        } 
    }

    //cria vetor de threads de tamanho NUMTHREAD
	pthread_t  pidt[NUMTHREAD];
	
	tempo1();
	printf(" gerando sistema ... \n");
	inicializa_cubo();

	printf(" irrigando ... \n");

	inicio[0]= 0;
 	for (i = 0; i< ITER; i++) {
		irriga();

		for (d = 0; d < NUMTHREAD; d++)
		{
			//cria thread chamando a função propagacao, passando a divisao[d] como parametro 
			pthread_create(&pidt[d], NULL, propagacao, &divisao[d]);
		}

		for (d=0; d< NUMTHREAD; d++){
			//junta as threads
			pthread_join(pidt[d], NULL);
		}
		//propagacao();
		evapora();
		move_matriz();
	}
	tempo2();
	//mostra_cubo();
	printf("%4.25f", a[0][C/2][P/2]);

	tempoFinal("segundos", argv[0], 0);
}

void * propagacao(void *arga){	
	int i, j, k, s, t;

	//superficie
	for (s = C/2; s > -1 ; s--){
		for (t = 0; t < L ; t++) 
	     b[0][s][t] =  ((a[0][s][t] +  a[0][s-1][t] + a[0][s][t-1] + a[1][s][t] + a[0][s+1][t] + a[0][s][t+1] ) / 6);
	}

	for (s = (C/2)+1; s < C; s++){
		for (t = 0; t < L ; t++)
	     b[0][s][t] =  ((a[0][s][t] +  a[0][s-1][t] + a[0][s][t-1] + a[1][s][t] + a[0][s+1][t] + a[0][s][t+1] ) / 6);
	}

	//recebe no ponteiro o argumento
	struct divisao *divisao = (int *)arga;

	//cada thread percore do inicio ao final das suas linhas
	for (i = (divisao->inicio); i < (divisao->final); i++) {
		for (j = 1; j < C-1; j++){
			for (k = 1; k < P-1; k++) 			        
		     b[i][j][k] = ((a[i][j][k] + a[i-1][j][k] + a[i][j-1][k] + a[i][j][k-1] + a[i+1][j][k] + a[i][j+1][k] +a[i][j][k+1] ) / 7);
		}
	}
}


void evapora(){	
	int l, p;
	for (l = 0; l < L; l++){
		for (p = 0; p < P; p++)
	     b[0][l][p] =  (b[0][l][p]  * 0.778);   
	}
}

void mostra_cubo(){
	int z, v, x;
        printf("\n  CUBO \n");
        for (z=0; z < L; z ++){
			for (v=0; v < C; v++) {
				for (x=0; x < P; x++){
        	        printf(" %4.5f", a[z][v][x]);
				}
				printf("\n");
			}
			printf("\n");
		}
		printf("\n\n\n");
}

void irriga(){
		a[0][C/2][P/2] = a[0][C/2][P/2] + 2; 
}

void move_matriz(){
        int z, v, x;       
        for (z=0; z < L; z ++)
			for (v=0; v < C; v++){
				for (x=0; x < P; x++){
				a[z][v][x] = b[z][v][x];
				}			
			}
}

void inicializa_cubo(){
        int z, v, x;
        printf("\n inicializando cubo ...\n");        
        for (z=0; z < L; z ++)
			for (v=0; v < C; v++)
				for (x=0; x < P; x++){
					a[z][v][x] = 0.0;
					b[z][v][x] = 0.0;
				}
}



