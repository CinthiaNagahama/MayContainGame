#include <stdio.h>
#include <locale.h>
#include "Screen.h"

/* Só pro screenshot */
							qtd = mao->qtd_cartas;
                            while(!mao_vazia(mao)){
                                mao_para_deck(deck, mao, 0);
                                (...)
                                SDL_RenderPresent(gRenderer);
                                SDL_Delay(100);
                            }
                            embaralha_deck(deck);
                            while(qtd > 0){
                                compra_carta(deck, mao);
                                (...)
                                SDL_RenderPresent(gRenderer);
                                SDL_Delay(100);
                                qtd--;
                            }
                            inimigos[i].turno = 0;
                            

int main(void){
	setlocale(LC_ALL, "Portuguese");
	print_screen("Historia/DIA 1/BLOCO A.txt", 1);
	clear_screen();
	return 0;
}
