#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deck.h"

// TODO:
// Struct Carta                                         OK
// Struct Deck                                          OK
// Struct Mao                                           OK
// Struct Database                                      OK
// Funcao de inicializar deck                           OK
// Funcao de embaralhar deck                            OK
// Funcao de inserir carta do deck                      OK
// Funcao de remover carta do deck                      OK
// Funcao de comprar carta do deck                      OK
// Funcao de inserir carta da mao                       OK
// Funcao de remover carta da mao                       OK
// Funcao de usar carta da mao                          OK
// Funcao de esvaziar o deck para nova batalha          OK
// Funcao de resetar o database para nova batalha       OK

void cria_db_carta(){
    // aloca a memória para o array:
    carta_db = (DatabaseCarta*) malloc(sizeof(DatabaseCarta) * 16);

    // registra as informações das cartas no mesmo:

    // Cartas de Cura

    carta_db[0].carta_info.ID_carta = 0;
    strcpy(carta_db[0].carta_info.nome_carta, "Luz Divina");
    strcpy(carta_db[0].carta_info.elemento_carta, "Cura");
    carta_db[0].carta_info.multiplicador_carta = 115;
    strcpy(carta_db[0].carta_info.efeito, "Cura");
    strcpy(carta_db[0].carta_info.descricao, "Cura - Nível 1:\nRegenera seu HP muito pouco. Agora voce entende quando dizem \"melhor prevenir do que remediar\", não é?");
    carta_db[0].qtdefault = 4;
    carta_db[0].qtleft = 4;
    strcpy(carta_db[0].carta_info.path, "Sprites/Cartas/00.png");
    carta_db[0].carta_info.range = 35;
    carta_db[0].carta_info.rangemin = 11;

    carta_db[1].carta_info.ID_carta = 1;
    strcpy(carta_db[1].carta_info.nome_carta, "Benção Divina");
    strcpy(carta_db[1].carta_info.elemento_carta, "Cura");
    carta_db[1].carta_info.multiplicador_carta = 220;
    strcpy(carta_db[1].carta_info.efeito, "Cura");
    strcpy(carta_db[1].carta_info.descricao, "Cura - Nível 2:\nRegenera seu HP. Mas não tem nada de divino aqui. Devo lembró-lo de que você é um mago e não um clérigo?");
    carta_db[1].qtdefault = 2;
    carta_db[1].qtleft = 2;
    strcpy(carta_db[1].carta_info.path, "Sprites/Cartas/01.png");
    carta_db[1].carta_info.range = 27;
    carta_db[1].carta_info.rangemin = 30;

    carta_db[2].carta_info.ID_carta = 2;
    strcpy(carta_db[2].carta_info.nome_carta, "Relaxa, Tô Full Life");
    strcpy(carta_db[2].carta_info.elemento_carta, "Cura");
    carta_db[2].carta_info.multiplicador_carta = 9999900;
    strcpy(carta_db[2].carta_info.efeito, "Cura");
    strcpy(carta_db[2].carta_info.descricao, "Cura - Nível 3:\nCura completamente. Só pode ser usada uma vez por batalha! Nada melhor do que um dia após o outro, não é mesmo?");
    carta_db[2].qtdefault = 1;
    carta_db[2].qtleft = 1;
    strcpy(carta_db[2].carta_info.path, "Sprites/Cartas/02.png");
    carta_db[2].carta_info.range = 0;
    carta_db[2].carta_info.rangemin = 1;

    // Cartas de Fogo

    carta_db[3].carta_info.ID_carta = 3;
    strcpy(carta_db[3].carta_info.nome_carta, "Labareda");
    strcpy(carta_db[3].carta_info.elemento_carta, "Fogo");
    carta_db[3].carta_info.multiplicador_carta = 115;
    strcpy(carta_db[3].carta_info.efeito, "Ataque");
    strcpy(carta_db[3].carta_info.descricao, "Fogo - Nível 1:\nSério que voce precisa de uma explicação pra uma magia com esse nome? Conjura uma labareda. Uau.");
    carta_db[3].qtdefault = 3;
    carta_db[3].qtleft = 3;
    strcpy(carta_db[3].carta_info.path, "Sprites/Cartas/03.png");
    carta_db[3].carta_info.range = 40;
    carta_db[3].carta_info.rangemin = 80;

    carta_db[4].carta_info.ID_carta = 4;
    strcpy(carta_db[4].carta_info.nome_carta, "Fogo Amigo");
    strcpy(carta_db[4].carta_info.elemento_carta, "Fogo");
    carta_db[4].carta_info.multiplicador_carta = 150;
    strcpy(carta_db[4].carta_info.efeito, "Ataque");
    strcpy(carta_db[4].carta_info.descricao, "Fogo - Nível 2:\nConjura um redemoinho de fogo que prende o alvo. Isso que é um abraço caloroso!");
    carta_db[4].qtdefault = 2;
    carta_db[4].qtleft = 2;
    strcpy(carta_db[4].carta_info.path, "Sprites/Cartas/04.png");
    carta_db[4].carta_info.range = 40;
    carta_db[4].carta_info.rangemin = 80;

    carta_db[5].carta_info.ID_carta = 5;
    strcpy(carta_db[5].carta_info.nome_carta, "Presente dos Céus");
    strcpy(carta_db[5].carta_info.elemento_carta, "Fogo");
    carta_db[5].carta_info.multiplicador_carta = 200;
    strcpy(carta_db[5].carta_info.efeito, "Ataque");
    strcpy(carta_db[5].carta_info.descricao, "Fogo - Nível 3:\nConjura um meteoro acima da cabeça do alvo. Indicado para amaciar cabeças-duras!");
    carta_db[5].qtdefault = 1;
    carta_db[5].qtleft = 1;
    strcpy(carta_db[5].carta_info.path, "Sprites/Cartas/05.png");
    carta_db[5].carta_info.range = 40;
    carta_db[5].carta_info.rangemin = 80;

    // Cartas de Gelo

    carta_db[6].carta_info.ID_carta = 6;
    strcpy(carta_db[6].carta_info.nome_carta, "Fica Frio Aí");
    strcpy(carta_db[6].carta_info.elemento_carta, "Gelo");
    carta_db[6].carta_info.multiplicador_carta = 115;
    strcpy(carta_db[6].carta_info.efeito, "Ataque");
    strcpy(carta_db[6].carta_info.descricao, "Gelo - Nível 1:\nConjura um campo mágico de baixa temperatura em volta do alvo, capaz de congelar toda a umidade do ar dentro dele. ");
    carta_db[6].qtdefault = 3;
    carta_db[6].qtleft = 3;
    strcpy(carta_db[6].carta_info.path, "Sprites/Cartas/06.png");
    carta_db[6].carta_info.range = 40;
    carta_db[6].carta_info.rangemin = 80;

    carta_db[7].carta_info.ID_carta = 7;
    strcpy(carta_db[7].carta_info.nome_carta, "Rajada Congelante");
    strcpy(carta_db[7].carta_info.elemento_carta, "Gelo");
    carta_db[7].carta_info.multiplicador_carta = 150;
    strcpy(carta_db[7].carta_info.efeito, "Ataque");
    strcpy(carta_db[7].carta_info.descricao, "Gelo - Nível 2:\nConjura uma rajada de gelo contra o alvo. Congelamento vendido separadamente.");
    carta_db[7].qtdefault = 2;
    carta_db[7].qtleft = 2;
    strcpy(carta_db[7].carta_info.path, "Sprites/Cartas/07.png");
    carta_db[7].carta_info.range = 40;
    carta_db[7].carta_info.rangemin = 80;

    carta_db[8].carta_info.ID_carta = 8;
    strcpy(carta_db[8].carta_info.nome_carta, "Tempestade de Neve");
    strcpy(carta_db[8].carta_info.elemento_carta, "Gelo");
    carta_db[8].carta_info.multiplicador_carta = 200;
    strcpy(carta_db[8].carta_info.efeito, "Ataque");
    strcpy(carta_db[8].carta_info.descricao, "Gelo - Nivel 3:\nConjura uma nevasca sob o alvo. Melhor vestir um casaco!");
    carta_db[8].qtdefault = 1;
    carta_db[8].qtleft = 1;
    strcpy(carta_db[8].carta_info.path, "Sprites/Cartas/08.png");
    carta_db[8].carta_info.range = 40;
    carta_db[8].carta_info.rangemin = 80;

    // Cartas de Eletricidade

    carta_db[9].carta_info.ID_carta = 9;
    strcpy(carta_db[9].carta_info.nome_carta, "Choque");
    strcpy(carta_db[9].carta_info.elemento_carta, "Elec");
    carta_db[9].carta_info.multiplicador_carta = 115;
    strcpy(carta_db[9].carta_info.efeito, "Ataque");
    strcpy(carta_db[9].carta_info.descricao, "Elétrico - Nível 1:\nTão potente quanto enfiar o dedo na tomada.");
    carta_db[9].qtdefault = 3;
    carta_db[9].qtleft = 3;
    strcpy(carta_db[9].carta_info.path, "Sprites/Cartas/09.png");
    carta_db[9].carta_info.range = 40;
    carta_db[9].carta_info.rangemin = 80;

    carta_db[10].carta_info.ID_carta = 10;
    strcpy(carta_db[10].carta_info.nome_carta, "Desfibrilar");
    strcpy(carta_db[10].carta_info.elemento_carta, "Elec");
    carta_db[10].carta_info.multiplicador_carta = 150;
    strcpy(carta_db[10].carta_info.efeito, "Ataque");
    strcpy(carta_db[10].carta_info.descricao, "Elétrico - Nível 2:\nConjura um relâmpago diretamente sob o alvo. Churrasco instantâneo!");
    carta_db[10].qtdefault = 2;
    carta_db[10].qtleft = 2;
    strcpy(carta_db[10].carta_info.path, "Sprites/Cartas/10.png");
    carta_db[10].carta_info.range = 40;
    carta_db[10].carta_info.rangemin = 80;

    carta_db[11].carta_info.ID_carta = 11;
    strcpy(carta_db[11].carta_info.nome_carta, "Fúria dos Céus");
    strcpy(carta_db[11].carta_info.elemento_carta, "Elec");
    carta_db[11].carta_info.multiplicador_carta = 200;
    strcpy(carta_db[11].carta_info.efeito, "Ataque");
    strcpy(carta_db[11].carta_info.descricao, "Elétrico - Nível 3:\nConjura uma tempestade de relâmpagos diretamente sob o alvo. Espero que não tenha medo de trovões!");
    carta_db[11].qtdefault = 1;
    carta_db[11].qtleft = 1;
    strcpy(carta_db[11].carta_info.path, "Sprites/Cartas/11.png");
    carta_db[11].carta_info.range = 40;
    carta_db[11].carta_info.rangemin = 80;

    // Cartas de Suporte

    carta_db[12].carta_info.ID_carta = 12;
    strcpy(carta_db[12].carta_info.nome_carta, "Escudo Mágico");
    strcpy(carta_db[12].carta_info.elemento_carta, "Supp");
    carta_db[12].carta_info.multiplicador_carta = 0;
    strcpy(carta_db[12].carta_info.efeito, "Escudo");
    strcpy(carta_db[12].carta_info.descricao, "Suporte:\nConjura um escudo que te protege por 2 turnos. Bem útil.");
    carta_db[12].qtdefault = 2;
    carta_db[12].qtleft = 2;
    strcpy(carta_db[12].carta_info.path, "Sprites/Cartas/12.png");
    carta_db[12].carta_info.range = 0;
    carta_db[12].carta_info.rangemin = 0;

    carta_db[13].carta_info.ID_carta = 13;
    strcpy(carta_db[13].carta_info.nome_carta, "Truque do Coelho na Manga");
    strcpy(carta_db[13].carta_info.elemento_carta, "Supp");
    carta_db[13].carta_info.multiplicador_carta = 0;
    strcpy(carta_db[13].carta_info.efeito, "Saque");
    strcpy(carta_db[13].carta_info.descricao, "Suporte:\nLhe permite sacar até duas cartas. O quê? Cartola? Não, eu acho que você está equivocado.");
    carta_db[13].qtdefault = 2;
    carta_db[13].qtleft = 2;
    strcpy(carta_db[13].carta_info.path, "Sprites/Cartas/13.png");
    carta_db[13].carta_info.range = 0;
    carta_db[13].carta_info.rangemin = 0;

    carta_db[14].carta_info.ID_carta = 14;
    strcpy(carta_db[14].carta_info.nome_carta, "Punho Firme");
    strcpy(carta_db[14].carta_info.elemento_carta, "Supp");
    carta_db[14].carta_info.multiplicador_carta = 0;
    strcpy(carta_db[14].carta_info.efeito, "Prev");
    strcpy(carta_db[14].carta_info.descricao, "Suporte:\nPrevine que cartas sejam descartadas por 4 turnos. Afinal de contas, como eles te obrigam a fazer isso!?");
    carta_db[14].qtdefault = 1;
    carta_db[14].qtleft = 1;
    strcpy(carta_db[14].carta_info.path, "Sprites/Cartas/14.png");
    carta_db[14].carta_info.range = 0;
    carta_db[14].carta_info.rangemin = 0;

    // Espaço vazio [Carta nula precisa ter o maior ID pois a
    // organização das cartas da mao é feita em ordem crescente de IDs]

    carta_db[15].carta_info.ID_carta = 15;
    strcpy(carta_db[15].carta_info.nome_carta, "----------");
    strcpy(carta_db[15].carta_info.elemento_carta, "Nulo");
    carta_db[15].carta_info.multiplicador_carta = 100;
    strcpy(carta_db[15].carta_info.efeito, "Nulo");
    strcpy(carta_db[15].carta_info.descricao, "---");
    carta_db[15].qtdefault = 0;
    carta_db[15].qtleft = 0;
    strcpy(carta_db[15].carta_info.path, "Sprites/Cartas/15.png");
    carta_db[15].carta_info.range = 50;
    carta_db[15].carta_info.rangemin = 75;

}

void libera_db_carta(){
    free(carta_db);
}

Deck* cria_deck(){
    // aloca memória para a fila
    Deck* fi = (Deck*) malloc (sizeof(Deck));
    if (fi == NULL){ // código de erro para falha na alocação
        printf("ERRO MEMDECK: low memo\n");
        system("pause");
        exit(1);
    }
    // se ok, inicializa a mesma
    fi->inicio = NULL;
    fi->final = NULL;
    fi->qtd = 0;
    return fi;
}

void libera_deck(Deck* fi){
    if (fi != NULL){
        Elem* no;
        while (fi->inicio != NULL){
            no = fi->inicio;
            fi->inicio = fi->inicio->prox;
            free(no);
        }
        free(fi);
    }
}

int deck_cheio(Deck* fi){
    if (fi == NULL)
        return -1;
    return (fi->qtd == 30);
}

int deck_vazio(Deck* fi){
    if (fi == NULL)
        return -1;
    return (fi->qtd == 0);
}

int insere_deck(Deck* fi, int idcarta){
    if (fi == NULL)
        return -1;
    if (fi->qtd == 30) // se deck cheio, não realiza operação
        return 0;
    Elem* p = (Elem*) malloc(sizeof(Elem));
    if (p == NULL){
        printf("Deck element creation failed: Low memo.\n");
        return 0;
    }
    p->info = idcarta; // armazena ID da carta para consulta no array DB [MÉTODO INT]
    p->prox = NULL;
    if(fi->final == NULL)
        fi->inicio = p;
    else
        fi->final->prox = p;
    fi->final = p;
    fi->qtd++; // incrementa o contador quantidade
    return 1;
}

int remove_deck(Deck* fi){
    if (fi == NULL)
        return -1;
    if (fi->qtd == 0) // se deck vazio, então impossível
        return 0;
    Elem* p = fi->inicio;
    fi->inicio = fi->inicio->prox;
    if(fi->inicio == NULL)
        fi->final = NULL;
    free(p);
    fi->qtd--;
    return 1;
}

int consulta_deck(Deck* fi, int *idcarta){
    if (fi == NULL)
        return -1;
    if (fi->qtd == 0) // se deck vazio, então impossível
        return 0;
    *idcarta = fi->inicio->info; // puxa ID da primeira carta da fila por referência [MÉTODO INT]
    return 1;
}

int qtd_deck(Deck* fi){
    if(fi == NULL)
        return -1;
    return fi->qtd;
}

int inicializa_deck(Deck* fi){
    if (fi == NULL)
        return -1;
    int counter = 30, index;
    while (counter > 0){                    // gera número aleatório como índice
        index = rand()%15;                  // e puxa info da carta do array
        if (carta_db[index].qtleft > 0){    // se o qtleft dela ainda for > 0
            insere_deck(fi, index);     // se não, refaz o processo até que
            carta_db[index].qtleft--;       // todas as 30 cartas tenham sido
            counter--;                      // guardadas no deck
        }
    }
    return 1;    
}

int embaralha_deck(Deck* fi){
    if (fi == NULL)
        return -1;
    int i, index, tam = fi->qtd;
    // aloca um vetor auxiliar de tamanho tam para armazenar as cartas atuais no deck
    int* aux = (int*) malloc(sizeof(int) * tam);
    for(i = 0; i < tam; i++){               // este loop salva o ID da primeira carta
        consulta_deck(fi, &aux[i]);     // em aux e a remove do deck; repete até
        remove_deck(fi);                // que o deck esteja vazio
    }
    while (tam > 0){                            // este loop gera um índice aleatório
        index = rand()%tam;                     // de módulo tam e insere a carta
        insere_deck(fi, aux[index]);        // de volta no deck
        if (index != tam - 1){
            for (i = index; i < tam - 1; i++) // este loop reorganiza aux para remover
                aux[i] = aux[i + 1];          // espaços vazios do meio do array e
        }                                     // garantir que o último dado não se perca
        tam--;
    }
    free(aux);
    return 1;
}

int esvazia_deck(Deck* fi){
    while(fi->qtd > 0)
        remove_deck(fi); // esvazia o deck por completo
    return 1;
}

void reseta_db(){
    int i;
    for (i = 0; i < 15; i++)
        carta_db[i].qtleft = carta_db[i].qtdefault; // retorna os valores default para qtleft
    return;
}

Mao* cria_mao(){
    // aloca a memória para a lista da mão
    Mao* mi = (Mao*) malloc(sizeof(Mao));
    if (mi == NULL){
        printf("ERRO MEMMAO: low memo\n");
        system("pause");
        exit(1);
    }
    // se ok, inicializa a mão com cartas nulas para evitar data junk
    mi->qtd_cartas = 0;
    for(int i = 0; i < 5; i++)
        mi->info[i] = carta_db[15].carta_info;
    return mi;
}

void libera_mao(Mao* mi){
    free(mi);
    return;
}

int mao_cheia(Mao* mi){
    if(mi == NULL)
        return -1;
    return (mi->qtd_cartas == 5); // se mão cheia, retorna 1, se não, 0
}

int mao_vazia(Mao* mi){
    if(mi == NULL)
        return -1;
    return (mi->qtd_cartas == 0); // se mão vazia, retorna 1, se não, 0
}

int descarta_carta(Mao* mi, int index){
    if(mi == NULL)
        return 0;
    if(mao_vazia(mi))
        return 0;
    int i = index;
    if(index != 4){                                          // se índice nao for 4,
        for(i = index; i < mi->qtd_cartas - 1; i++)          // reorganiza a mao
            mi->info[i] = mi->info[i + 1];
    }
    mi->info[i] = carta_db[15].carta_info; // coloca espaço vazio no último slot
    mi->qtd_cartas--; // decrementa contador quantidade
    return 1;
}

int insere_mao(Mao* mi, int idcarta){
    if(mi == NULL)
        return 0;
    if(mao_cheia(mi)) // se mão cheia, então impossível
        return 0;
    int j, i;
    for(i = 0; i < mi->qtd_cartas; i++){        // este loop procura o slot
        if (idcarta < mi->info[i].ID_carta)     // onde a carta a ser inserida
            break;                              // tem id menor que as outras
    }
    for(j = mi->qtd_cartas - 1; j >= i; j--)    // este loop puxa as cartas para frente
        mi->info[j + 1] = mi->info[j];          // (abre o espaço)
    mi->info[i] = carta_db[idcarta].carta_info; // para então inserir a carta atual
    mi->qtd_cartas++; // incrementa contador quantidade
    return 1;
}

int consulta_mao(Mao* mi, int index, int *idconsulta){
    if (mi == NULL)
        return -1;
    if (mao_vazia(mi) || index >= mi->qtd_cartas) // se mão vazia ou index > tam da lista, então, impossível
        return 0;
    *idconsulta = mi->info[index].ID_carta; // copia o id da carta solicitada por referência
    return 1;
}

int qtd_mao(Mao* mi){
    if(mi == NULL)
        return -1;
    return mi->qtd_cartas;
}

int compra_carta(Deck* f1, Mao* m1){
    if (f1 == NULL || m1 == NULL)
        return -1;
    if (deck_vazio(f1)){
        return 0;
    }
    if (mao_cheia(m1)){
        return 0;
    }
    int carta;
    consulta_deck(f1, &carta); // pega id da carta
    insere_mao(m1, carta); // insere na mão
    remove_deck(f1); // remove do deck
    return 1;
}

int usa_carta(Mao* mi, int index, int *idconsulta){
    if(mi == NULL)
        return -1;
    if (mao_vazia(mi)) // se mão vazia, impossível
        return 0;
    if (index >= mi->qtd_cartas) // se index fora do limite, retorna erro
        return -1;
    consulta_mao(mi, index, idconsulta); // passa id da carta selecionada por referência
    descarta_carta(mi, index); // descarta a carta
    return 1;
}

void imprime_deck(Deck* fi){
    Elem* p = fi->inicio;
    int i = 1;
    while(p != NULL){
        printf("Carta %2i: %2i | ", i, p->info);
        p = p->prox;
        i++;
    }
    printf("\n");
}

void imprime_mao(Mao* mi){
    int i, tam = 5;
    printf("Mao:\n");
    for(i = 0; i < tam; i++)
        printf("ID: %2i | Nome: %s | Elem: %s | Efeito: %s | Mult: %i\nDesc:\n%s\n", mi->info[i].ID_carta, mi->info[i].nome_carta, mi->info[i].elemento_carta, mi->info[i].efeito, mi->info[i].multiplicador_carta, mi->info[i].descricao);
}