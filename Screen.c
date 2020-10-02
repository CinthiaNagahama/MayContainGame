#include <stdio.h>
#include <stdlib.h>
#include "Screen.h"
#include <unistd.h>

// Imprime um bloco de texto no terminal (letra_a_letra tipo booleano)
void print_screen(char *fileName, int letra_a_letra){
	int i, tempo = 10000; // microsegundos
	FILE *file = fopen(fileName, "r");;
	char line[159]; // Tamanho da linha a ser impressa

	if (file == NULL) // Se houve erro na abertura
	{
		printf("Arquivo não encontrado.\n");
		return ;
	}
	
	while (fgets(line, 159, file) != NULL) // Checa se chegou ao fim do arquivo
	{
		if(letra_a_letra){
			for(i = 0; line[i] != '\0'; i++){	// -
				printf("%c", line[i]);		 	//  |
				usleep(tempo);					//   ]> Imprime letra a letra
				fflush(stdout);					//  |
			}								 	// -
		}
		else
			printf("%s", line); // imprime de uma vez
	}
	printf("\n\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t▼");
	fflush(stdin); // necessario para evitar problemas com o getchar
	getchar();
	fclose(file);
}

// Limpa o terminal
void clear_screen()
{
	system("cls||clear");
}
