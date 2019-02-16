#include <stdio.h>
#include <string.h>
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
void propagacao();
void evapora();
void move_matriz();

int main(int argc, char *argv[])
{

	int i;
	tempo1();

	printf(" gerando sistema ... \n");
	inicializa_cubo();

	printf(" irrigando ... \n");
	
 	for (i = 0; i< ITER; i++) {
		irriga();
		propagacao();
		evapora();
		move_matriz();
	}
	tempo2();
	//mostra_cubo();
	printf("%4.25f", a[0][C/2][P/2]);

	tempoFinal("segundos", argv[0], 0);
}


void propagacao()

{	
	int i, j, k, s, t;
	
	for (i = 1; i < L-1; i++) {
		for (j = 1; j < C-1; j++){
			for (k = 1; k < P-1; k++) 			        
		     b[i][j][k] = ((a[i][j][k] + a[i-1][j][k] + a[i][j-1][k] + a[i][j][k-1] + a[i+1][j][k] + a[i][j+1][k] +a[i][j][k+1] ) / 7);
		}
	}
	
	//superficie
	for (s = C/2; s > -1 ; s--){
		for (t = 0; t < L ; t++) 
	     b[0][s][t] =  ((a[0][s][t] +  a[0][s-1][t] + a[0][s][t-1] + a[1][s][t] + a[0][s+1][t] + a[0][s][t+1] ) / 6);
	}

	for (s = (C/2)+1; s < C; s++){
		for (t = 0; t < L ; t++)
	     b[0][s][t] =  ((a[0][s][t] +  a[0][s-1][t] + a[0][s][t-1] + a[1][s][t] + a[0][s+1][t] + a[0][s][t+1] ) / 6);
	}

}


void evapora()
{	
	int l, p;

	for (l = 0; l < L; l++){
		for (p = 0; p < P; p++)
	     b[0][l][p] =  (b[0][l][p]  * 0.778);   
	}
}


void mostra_cubo()
{
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


void irriga()
{
		a[0][C/2][P/2] = a[0][C/2][P/2] + 2; 
}


void move_matriz()
{
        int z, v, x;
       
        for (z=0; z < L; z ++)
			for (v=0; v < C; v++){
				for (x=0; x < P; x++){
				a[z][v][x] = b[z][v][x];
				}			
			}
}

void inicializa_cubo()
{
        int z, v, x;
        printf("\n inicializando cubo ...\n");
        
        for (z=0; z < L; z ++)
			for (v=0; v < C; v++)
				for (x=0; x < P; x++){
					a[z][v][x] = 0.0;
					b[z][v][x] = 0.0;
				}
}



