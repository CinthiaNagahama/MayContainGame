#ifndef STRUCTS_H
#define STRUCTS_H
#include "../src/main.h"

const int SCR_WID = 1280;
const int SCR_HEI = 750;

enum ICON_NUM {
    SHIELD_ICON,
    PREV_ICON
};

typedef struct player{
    int hp;
    int hpmax;
    int atk;
    int def;
    int turno;
    int gameOver;
} Player;

typedef struct seletor{
    int ativoc;
    int ativoe;
    int indexc;
    int indexe;
    int escudo;
    int prev;
    int tutorial;
    int animation;
    int dano;
} Seletor;

typedef struct animation{
    int alpha;
    int r;
    int g;
    int b;
    SDL_Rect clip;
    SDL_Rect size;
} Animation;

// window
SDL_Window *gWindow = NULL;
// window surface
SDL_Surface *gSurface = NULL;
// imagem atual
SDL_Surface *gCurrentSurface = NULL;
// Renderer
SDL_Renderer *gRenderer = NULL;
// Texturas
SDL_Texture *gTexture = NULL;
SDL_Texture *gBGTexture = NULL;
SDL_Texture **gEnemyTexture = NULL;
SDL_Texture *gBoxTexture = NULL;
SDL_Texture *gNameBoxTexture = NULL;
SDL_Texture *gMenuBoxTexture = NULL;
SDL_Texture *gBattleBoxTexture = NULL;
SDL_Texture **gIconTexture = NULL;
SDL_Texture **gCardTexture = NULL;
SDL_Texture *gModTexture = NULL;
SDL_Texture *gExpressionBoxTexture = NULL;
// Fonte
TTF_Font *gFont = NULL;
// rendered texture
SDL_Texture *gFontTexture = NULL;;
// Viewports
    const SDL_Rect battleBGVP = {0, 0, SCR_WID, 3 * SCR_HEI / 4 + 20};
    const SDL_Rect menuVP = {0, 3 * SCR_HEI / 4, SCR_WID, SCR_HEI/4};
    const SDL_Rect statsVP = {0, 3 * SCR_HEI / 4, SCR_WID/6, SCR_HEI/4};
    const SDL_Rect stockVP = {SCR_WID / 6 + 35, 3 * SCR_HEI / 4, 5 * SCR_WID/6 - 10, SCR_HEI/4};
    const SDL_Rect enemyAreaVP = {120, 30, 1040, 3 * SCR_HEI / 4 - 30};
    const SDL_Rect msgAreaTopVP = {190, 30, 900, 160};
    const SDL_Rect msgAreaBottomVP = {190, 570, 900, 160};
// Cores
    const SDL_Color color = {200,200,200};
    const SDL_Color black = {0,0,0};
    const SDL_Color blue = {20,20,204};
    const SDL_Color green = {0,128,0};
    const SDL_Color red = {204,0,0};
    const SDL_Color yellow = {204,204,0};

#endif