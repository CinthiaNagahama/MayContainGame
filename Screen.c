#include <stdio.h>
#include "Screen.h"

// Imprime um bloco de texto no terminal
void print_screen(char *fileName){
	FILE *file = fopen(fileName, "r");;
	char line[100]; // Tamanho da linha a ser impressa

	if (file == NULL) // Se houve erro na abertura
	{
		printf("Arquivo não encontrado.\n");
		return ;
	}
	
	while (fgets(line, 100, file) != NULL) // Checa se chegou ao fim do arquivo
	{
		printf("%s", line);
	}
	printf("\n\n");
	system("pause");
	fclose(file);
}

// Limpa o terminal
void clear_screen()
{
	system("cls||clear");
}
