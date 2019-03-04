#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define TAM 10

void main(void)
{


   FILE *fp;
   char data[TAM];
   fp = fopen ("texto.txt","r");

   // le 10 char e coloca na var data
   fgets ( data, TAM, fp );
   printf("\n--primeiros 10 bytes do arquivo:\n %s\n", data); 



 
   // avançar ponteiro para 50' byte no arquivo a partir do inicio
   fseek(fp, 50, SEEK_SET);
   fgets ( data, TAM, fp );
   printf("\n--10 bytes apos posicionar no byte 50: \n %s\n", data);
 


   // voltar ponteiro 10 byte no arquivo a partir da posicao atual 
   fseek(fp, -10, SEEK_CUR);
   fgets ( data, TAM, fp );
   printf("\n--voltando 10 bytes da posicao atual: \n %s\n", data);
 


   // posicionao o ponteiro no 30 byte antes do final do arquivo 
   fseek(fp, -30, SEEK_END); 
   fgets ( data, TAM, fp );
   printf("\n--30 bytes antes do final do arquivo: \n %s \n\n ", data);


 
   fclose(fp);

}



//SEEK_SET – It moves file pointer position to the beginning of the file.
//SEEK_CUR – It moves file pointer position to given location.
//SEEK_END – It moves file pointer position to the end of file

//fseek(fp, 0, SEEK_SET);  - // posicionao o ponteiro no inicio do arquivo

