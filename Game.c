/* #include "Battle.h"*/
#include "Game.h"
#include "Screen.h"
#include <stdio.h>
#include <windows.h> // Apagar no Linux

// Tela de loading, porque sim
void loading_screen(){	
	char bar[8][11] = {"[.]", "[..]", "[...]", "[....]", "[.....]", "[......]", "[.......]", "[........]"};
	int i;
	for(i = 0; i < 8; i++){
		printf("\n\t");
		printf("%s", bar[i]);
		Sleep(500);
		// sleep(2); -> Pra Linux
		clear_screen();	
	}
	intro();
}

// Introdu��o da hist�ria
void intro(){
	print_screen("BEM VINDO.txt");
	clear_screen();
	print_screen("BLOCO A.txt");
}

/*
// Come�a o jogo
void start_game(){
	
}

// Chama batalha... ou n�o
void isThereABattle(int quest){
	// int random = 1-100;
	// if(random < 61) battle();
}

// Fim de jogo
void end_game(Mage* hero){ // Provis�rio at� fazer struct do mago
	
}
*/
