#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Inimigo.h"

void cria_db_inimigo(){
    inimigo_db = (Inimigo*) malloc(sizeof(Inimigo) * 5);

    strcpy(inimigo_db[0].nome, "Pinguim");
    inimigo_db[0].HP = 500;
    inimigo_db[0].ATK = 100;
    inimigo_db[0].DEF = 1;
    inimigo_db[0].buffdefault = 2;
    inimigo_db[0].RElemento[0]=0.75; // fogo
    inimigo_db[0].RElemento[1]=0.5; // gelo
    inimigo_db[0].RElemento[2]=1.5; // elec
    strcpy(inimigo_db[0].path, "Sprites/Inimigos/Pinguim.png");
    inimigo_db[0].w = 197;
    inimigo_db[0].h = 233;
    inimigo_db[0].chance[ATTACK].min = 0;
    inimigo_db[0].chance[ATTACK].max = 0;
    inimigo_db[0].chance[MUG].min = 0;
    inimigo_db[0].chance[MUG].max = 10;
    inimigo_db[0].chance[FLEE].min = 30;
    inimigo_db[0].chance[FLEE].max = 60;
    inimigo_db[0].chance[SPECIAL_MOVE].min = 10;
    inimigo_db[0].chance[SPECIAL_MOVE].max = 30;
    inimigo_db[0].chance[DEFEND].min = 60;
    inimigo_db[0].chance[DEFEND].max = 100; 
    inimigo_db[0].comportamento.mult_atk = 0;
    inimigo_db[0].comportamento.discard_num = 5;
    inimigo_db[0].comportamento.mult_special = -1;
    inimigo_db[0].comportamento.flee_chance = 30;
    inimigo_db[0].comportamento.mult_defend = 2;
    strcpy(inimigo_db[4].comportamento.nome_special, "          Um Presente");

    strcpy(inimigo_db[1].nome, "Goblin");
    inimigo_db[1].HP = 120;
    inimigo_db[1].ATK = 3;
    inimigo_db[1].DEF = 10;
    inimigo_db[1].buffdefault = 0;
    inimigo_db[1].RElemento[0] = 1.5; // fogo
    inimigo_db[1].RElemento[1] = 1; // gelo
    inimigo_db[1].RElemento[2] = 1.25; // elec
    strcpy(inimigo_db[1].path, "Sprites/Inimigos/Goblin.png");
    inimigo_db[1].w = 250;
    inimigo_db[1].h = 292;
    inimigo_db[1].chance[ATTACK].min = 20;
    inimigo_db[1].chance[ATTACK].max = 100;
    inimigo_db[1].chance[MUG].min = 0;
    inimigo_db[1].chance[MUG].max = 5;
    inimigo_db[1].chance[FLEE].min = 0;
    inimigo_db[1].chance[FLEE].max = 5;
    inimigo_db[1].chance[SPECIAL_MOVE].min = 10;
    inimigo_db[1].chance[SPECIAL_MOVE].max = 20;
    inimigo_db[1].chance[DEFEND].min = 0;
    inimigo_db[1].chance[DEFEND].max = 0; 
    inimigo_db[1].comportamento.mult_atk = 1;
    inimigo_db[1].comportamento.discard_num = 1;
    inimigo_db[1].comportamento.mult_special = 0;
    inimigo_db[1].comportamento.flee_chance = 5;
    inimigo_db[1].comportamento.mult_defend = 0;
    strcpy(inimigo_db[1].comportamento.nome_special, "         ... Ugah");

    strcpy(inimigo_db[2].nome, "Ladrão");
    inimigo_db[2].HP = 200;
    inimigo_db[2].ATK = 5;
    inimigo_db[2].DEF = 20;
    inimigo_db[2].buffdefault = 0;
    inimigo_db[2].RElemento[0] = 1.75; // fogo
    inimigo_db[2].RElemento[1] = 1.25; // gelo
    inimigo_db[2].RElemento[2] = 1.0; // elec
    strcpy(inimigo_db[2].path, "Sprites/Inimigos/Ladrao.png");
    inimigo_db[2].w = 200;
    inimigo_db[2].h = 234;
    inimigo_db[2].chance[ATTACK].min = 90;
    inimigo_db[2].chance[ATTACK].max = 100;
    inimigo_db[2].chance[MUG].min = 50;
    inimigo_db[2].chance[MUG].max = 90;
    inimigo_db[2].chance[FLEE].min = 10;
    inimigo_db[2].chance[FLEE].max = 50;
    inimigo_db[2].chance[SPECIAL_MOVE].min = 0;
    inimigo_db[2].chance[SPECIAL_MOVE].max = 10;
    inimigo_db[2].chance[DEFEND].min = 0;
    inimigo_db[2].chance[DEFEND].max = 0; 
    inimigo_db[2].comportamento.mult_atk = 1.15;
    inimigo_db[2].comportamento.discard_num = 2;
    inimigo_db[2].comportamento.mult_special = 1.20;
    inimigo_db[2].comportamento.flee_chance = 30;
    inimigo_db[2].comportamento.mult_defend = 0;
    strcpy(inimigo_db[2].comportamento.nome_special, "               Agilidade");
    
    strcpy(inimigo_db[3].nome, "Pinguim");
    inimigo_db[3].HP = 250;
    inimigo_db[3].ATK = 5;
    inimigo_db[3].DEF = 30;
    inimigo_db[3].buffdefault = 1;
    inimigo_db[3].RElemento[0] = 0.5; // fogo
    inimigo_db[3].RElemento[1] = 0; // gelo
    inimigo_db[3].RElemento[2] = 1; // elec
    strcpy(inimigo_db[3].path, "Sprites/Inimigos/Pinguim_Bolado.png");
    inimigo_db[3].w = 197;
    inimigo_db[3].h = 233;
    inimigo_db[3].chance[ATTACK].min = 50;
    inimigo_db[3].chance[ATTACK].max = 100;
    inimigo_db[3].chance[MUG].min = 0;
    inimigo_db[3].chance[MUG].max = 10;
    inimigo_db[3].chance[FLEE].min = 0;
    inimigo_db[3].chance[FLEE].max = 0;
    inimigo_db[3].chance[SPECIAL_MOVE].min = 10;
    inimigo_db[3].chance[SPECIAL_MOVE].max = 30;
    inimigo_db[3].chance[DEFEND].min = 30;
    inimigo_db[3].chance[DEFEND].max = 50; 
    inimigo_db[3].comportamento.mult_atk = 1.15;
    inimigo_db[3].comportamento.discard_num = 2;
    inimigo_db[3].comportamento.mult_special = 1.25;
    inimigo_db[3].comportamento.flee_chance = 30;
    inimigo_db[3].comportamento.mult_defend = 0.75;
    strcpy(inimigo_db[3].comportamento.nome_special, "                Toda Ira");

    strcpy(inimigo_db[4].nome, "Abu");
    inimigo_db[4].HP = 1000;
    inimigo_db[4].ATK = 50;
    inimigo_db[4].DEF = 50;
    inimigo_db[4].buffdefault = 1;
    inimigo_db[4].RElemento[0]=1; // fogo
    inimigo_db[4].RElemento[1]=1; // gelo
    inimigo_db[4].RElemento[2]=1; // elec
    strcpy(inimigo_db[4].path, "Sprites/Inimigos/Abu.png");
    inimigo_db[4].w = 575; // 677;
    inimigo_db[4].h = 400; // 471;
    inimigo_db[4].chance[ATTACK].min = 30;
    inimigo_db[4].chance[ATTACK].max = 100;
    inimigo_db[4].chance[MUG].min = 5;
    inimigo_db[4].chance[MUG].max = 8;
    inimigo_db[4].chance[FLEE].min = 0;
    inimigo_db[4].chance[FLEE].max = 0;
    inimigo_db[4].chance[SPECIAL_MOVE].min = 0;
    inimigo_db[4].chance[SPECIAL_MOVE].max = 5;
    inimigo_db[4].chance[DEFEND].min = 8; 
    inimigo_db[4].chance[DEFEND].max = 30; 
    inimigo_db[4].comportamento.mult_atk = 1.15;
    inimigo_db[4].comportamento.discard_num = 3;
    inimigo_db[4].comportamento.mult_special = 1.5;
    inimigo_db[4].comportamento.flee_chance = 0.0;
    inimigo_db[4].comportamento.mult_defend = 1.25;
    strcpy(inimigo_db[4].comportamento.nome_special, "                  Mal Encarnado");
}

void libera_db_inimigo(){
    free(inimigo_db);
}

void imprime_inimigo(){
    int i, size=4;
    for(i = 0; i < size; i++){
        printf("Nome: %s \n", inimigo_db[i].nome);
        printf("HP: %.2f \n", inimigo_db[i].HP);
        printf("ATK: %d \n", inimigo_db[i].ATK);
        printf("DEF: %d \n", inimigo_db[i].DEF);
        printf("Gelo: %.2f \n", inimigo_db[i].RElemento[0]);
        printf("Raio: %.2f \n", inimigo_db[i].RElemento[1]);
        printf("Fogo: %.2f \n\n", inimigo_db[i].RElemento[2]);
    }
}