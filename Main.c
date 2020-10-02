#include <stdio.h>
#include <locale.h>
#include "Screen.h"

int main(void){
	setlocale(LC_ALL, "Portuguese");
	print_screen("Historia/DIA 1/BLOCO A.txt", 1);
	clear_screen();
	return 0;
}