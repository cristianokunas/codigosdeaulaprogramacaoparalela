#include "mpi.h"
#include "tempo.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define INPUT_FILE "texto.txt"
#define MASTER 0

#define QUERY "DHGK"

int master(int numProc);
void slave();
int checkTextForPatternAtPosition(int textIndex, int textLength, int lengthToCheck, char query[], int queryLength);
char *string_reverse(char *dst, const char *src);

int main(int argc,char *argv[])
{
    int procId, numProc;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &procId);

    if (numProc < 2)
    {
        printf("\nDefina pelo menos 2 processos. Mestre não faz trabalho duro\n");
    }
    else if (procId == MASTER)
    {
        tempo1();
        int totalFound = master(numProc);
        tempo2();
        printf("Total: %d| Tempo: ", totalFound);
        tempoFinal("mili segundos", argv[0], MSGLOG);
	printf("\n");
    }
    else
    {
        slave();
    }
    
    MPI_Finalize();
}

int master(int numProc)
{
    // Mover o ponteiro do arquivo para o final do fluxo e obter o tamanho do texto
    FILE *fp;
    fp = fopen (INPUT_FILE,"r");
    fseek(fp, 0L, SEEK_END);
    const int textLength = ftell(fp);
    fclose(fp);

    // Mestre não fará trabalho duro
    const int slaves = numProc - 1;

    // Work division
    int count = textLength / slaves;
    int remainder = textLength % slaves;
    int start, stop;
    for (int slaveId = 0; slaveId < slaves; slaveId++)
    {
        if (slaveId < remainder) {
            // O primeiro escravo 'restante' recebe cada uma das tarefas 'contar + 1'
            start = slaveId * (count + 1);
            stop = start + count;
        } else {
            // Os escravos remanescentes de 'tamanho - restante' recebem as tarefas de 'contagem' cada
            start = slaveId * count + remainder;
            stop = start + (count - 1);
        }
        
        // Prepara os dados para enviar através do MPI
        int control[] = {start, textLength, stop - start};

        MPI_Send(control, 3, MPI_INT, slaveId + 1, 4, MPI_COMM_WORLD);
    }

    MPI_Status status;
    int totalFound = 0, found;
    for (int procId = 1; procId < numProc; procId++)
    {
        MPI_Recv(&found, 1, MPI_INT, procId, 4, MPI_COMM_WORLD, &status);
        totalFound += found;
    }

    return totalFound;
}

void slave(char *argv[])
{
    int control[3];
    MPI_Status status;
    MPI_Recv(control, 3, MPI_INT, 0, 4, MPI_COMM_WORLD, &status);
  

    const int start = control[0];
    const int textLength = control[1];
    const int lengthToCheck = control[2];
    const int queryLength = strlen(QUERY);
    char reversedQuery[queryLength];
    string_reverse(reversedQuery, QUERY);

    // pesquisar a consulta e sua versão invertida
    int found = checkTextForPatternAtPosition(start, textLength, lengthToCheck, QUERY, queryLength) + 
                checkTextForPatternAtPosition(start, textLength, lengthToCheck, reversedQuery, queryLength);
    
    MPI_Send(&found, 1, MPI_INT, 0, 4, MPI_COMM_WORLD);
  
}

// Encontra o padrão na posição do texto e as posições seguintes para lengthToCheck
int checkTextForPatternAtPosition(int textIndex, int textLength, int lengthToCheck, char query[], int queryLength)
{
    FILE *fp;

    // Abrir arquivo (somente leitura)
    fp = fopen (INPUT_FILE, "r");

	int i;
	int found = 0;

	for (i = 0; i < lengthToCheck; i++)
	{
		if (textLength - (textIndex + i) < queryLength) break;

		int k = textIndex + i;
		int j = 0;
			
		while (fseek(fp, k, SEEK_SET) == 0 && getc(fp) == query[j] && j < queryLength)
		{
			k++;
			j++;
		}
		
		if (j == queryLength) found++;
	}

    // Solte o arquivo
    fclose(fp);
		
	return found;
}

char *string_reverse(char *dst, const char *src)
{
    if (src == NULL) return NULL;

    const char *src_start = src;
    char *dst_end = dst + strlen(src);
    *dst_end = '\0';

    while ((*--dst_end = *src_start++)) { ; }

    return dst;
}
