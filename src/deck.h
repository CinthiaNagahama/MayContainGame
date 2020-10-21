#ifndef DECK_H
#define DECK_H

typedef struct carta {
    int ID_carta; // id utilizado para procurar cartas no array carta_db; ID = index da carta no array
    char nome_carta[31]; // nome da carta, para display
    char elemento_carta[5]; // elemento da carta, para o cálculo de dano
    int multiplicador_carta; // valor do multiplicador para o cálculo de dano
    char efeito[8]; // tipo da carta, usado para definir ação no combate
    char descricao[151]; // descrição da carta, para 
    char path[35]; // caminho da textura
    int range;
    int rangemin;
} Carta; // estrutura da carta

typedef struct databasecarta{
    int qtleft; // qtd restante da carta a ser embaralhada
    int qtdefault; // qtd padrao para resetar 
    Carta carta_info; // dados da carta
} DatabaseCarta; // estrutura do array global que servirá de banco de dados das cartas

typedef struct elemento{
    int info;
    struct elemento* prox;
} Elem;

struct fila_deck {
    int qtd;
    Elem* inicio;
    Elem* final;
}; // estrutura da fila (Deck)

struct mao {
    Carta info[5];
    int qtd_cartas;
}; // estrutura da lista (Mao)

typedef struct fila_deck Deck; // ponteiro de fila estática para o Deck
typedef struct mao Mao; // ponteiro de lista estática para a Mao

DatabaseCarta* carta_db; // variável global que será usada como database das cartas

// cria o db de cartas
void cria_db_carta();

// libera o db de cartas
void libera_db_carta();

// cria o ponteiro para o deck
Deck* cria_deck();

// libera o ponteiro;
void libera_deck(Deck* fi);

// testa se deck está cheio
int deck_cheio(Deck* fi);

// testa se deck está vazio
int deck_vazio(Deck* fi);

// insere carta no deck
int insere_deck(Deck* fi, int idcarta);

// remove carta do deck
int remove_deck(Deck* fi);

// consulta o valor da primeira carta no deck e o salva por referência
int consulta_deck(Deck* fi, int *idcarta);

// retorna quantidade de cartas no deck
int qtd_deck(Deck* fi);

// prepara e embaralha o deck a primeira vez na luta
int inicializa_deck(Deck* fi);

// embaralha o deck à partir da condição atual do mesmo
int embaralha_deck(Deck* fi);

// esvazia o deck após a batalha para uma nova inicialização
// (pode ser usado o libera_deck, porém, será necessário utilizar cria_deck novamente)
int esvazia_deck(Deck* fi);

// reseta o array banco de dados de cartas para uma nova inicialização
void reseta_db();

// cria o ponteiro para a mão
Mao* cria_mao();

// libera o ponteiro da mão
void libera_mao(Mao* mi);

// testa se mão está cheia
int mao_cheia(Mao* mi);

// testa se mão está vazia
int mao_vazia(Mao* mi);

// remove carta da mao
int descarta_carta(Mao* mi, int index);

// adiciona carta à mão
int insere_mao(Mao* mi, int idcarta);

// consulta a carta de índice = index da mão
int consulta_mao(Mao* mi, int index, int *idconsulta);

// retorna quantidade de cartas na mao
int qtd_mao(Mao* mi);

// adiciona carta à mão e remove do deck
int compra_carta(Deck* f1, Mao* m1);

// utiliza a carta de índice = index e a remove da mão
// (copia o id para idconsulta para extrair info do array carta_db)
int usa_carta(Mao* mi, int index, int *idconsulta);

// imprime mao (desnecessario in-game; só para testes mesmo)
void imprime_mao(Mao* mi);

// imprime deck (desnecessario in-game; só para testes mesmo)
void imprime_deck(Deck* fi);

#endif