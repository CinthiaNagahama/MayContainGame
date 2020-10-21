#ifndef INIMIGO_H
#define INIMIGO_H

enum MOVES{
    ATTACK,
    MUG,
    FLEE,
    SPECIAL_MOVE,
    DEFEND,
};

typedef struct moveset{
    int mult_atk;
    int discard_num;
    int mult_special;
    int flee_chance;
    int mult_defend;
    char nome_special[35];
} Moveset;

typedef struct chance{
    int min;
    int max;
} Chance;

typedef struct inimigo{
    char nome[15];
    int HP;
    int ATK;
    int DEF;
    int buffdefault;
    int RElemento[3];
    char path[35];
    int w;
    int h;
    Chance chance[5];
    Moveset comportamento;
} Inimigo;

typedef struct inimigoBatalha{
    int hp;
    int id;
    int buff;
    int buffCount;
    int bolado;
    int turno;
    int derrotado;
    int alpha;
    int fleeCount;
} InimigoBatalha;

Inimigo* inimigo_db;

void cria_db_inimigo();

void libera_db_inimigo();

void imprime_inimigo();
#endif