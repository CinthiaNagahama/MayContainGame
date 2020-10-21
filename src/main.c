#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
// #include <SDL2/SDL.h>
// #include <SDL2/SDL_image.h>
// #include <SDL2/SDL_ttf.h>
#include "SDL/SDL2-2.0.12/i686-w64-mingw32/include/SDL2/SDL.h"
#include "SDL/SDL2-2.0.12/i686-w64-mingw32/include/SDL2/SDL_image.h"
#include "SDL/SDL2-2.0.12/i686-w64-mingw32/include/SDL2/SDL_mixer.h"
#include "SDL/SDL2-2.0.12/i686-w64-mingw32/include/SDL2/SDL_ttf.h"
#include "deck.h"
#include "Inimigo.h"

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
    int tutorial;
    int mainMenu;
    int gameOver;
} Player;

typedef struct seletor{
    int ativoc;
    int ativoe;
    int indexc;
    int indexe;
    int escudo;
    int prev;
    int animation;
    int absorb;
    int primeiroTurno;
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
SDL_Texture *gFakeRect = NULL;
SDL_Texture *gCurrentScene = NULL;
// Fonte
TTF_Font *gFont = NULL;
// rendered texture
SDL_Texture *gFontTexture = NULL;
// Viewports
    const SDL_Rect battleBGVP = {0, 0, 1280, 582};
    const SDL_Rect menuVP = {0, 562, 1280, 187};
    const SDL_Rect statsVP = {0, 562, 213, 187};
    const SDL_Rect stockVP = {248, 562, 1056, 187};
    const SDL_Rect enemyAreaVP = {120, 30, 1040, 532};
    const SDL_Rect msgAreaTopVP = {190, 30, 900, 160};
    const SDL_Rect msgAreaBottomVP = {190, 570, 900, 160};
// Cores
    const SDL_Color color = {200,200,200};
    const SDL_Color black = {0,0,0};
    const SDL_Color blue = {20,20,204};
    const SDL_Color green = {0,128,0};
    const SDL_Color red = {204,0,0};
    const SDL_Color yellow = {204,204,0};
    
int init(){
    int success = 1;
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL could not be initialized. SDL_Error %s\n", SDL_GetError());
        success = 0;
    }
    else{
        if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}
        gWindow = SDL_CreateWindow("MayCointainGame", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCR_WID, SCR_HEI, SDL_WINDOW_SHOWN);
        if(gWindow == NULL){
            printf("window could not be created!! SDL_ERROR: %s\n", SDL_GetError());
            success = 0;
        }
        else{
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if(gRenderer == NULL){
                printf("Renderer fail. SDL Error %s\n", SDL_GetError());
                success = 0;
            }
            else{
                SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
                int imgflags = IMG_INIT_PNG;
                if(!(IMG_Init(imgflags) & imgflags)){
                    printf("SDL_Image had some trouble initializing. SDL_Image error: %s\n", IMG_GetError());
                    success = 0;
                }
                if(TTF_Init() == -1){
                    printf("SDL_ttf fail. %s\n", TTF_GetError());
                    success = 0;
                }
                srand(time(0));
            }
        }
    }
    return success;
}

SDL_Texture* loadTexture(char* path){
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path);
    if(loadedSurface == NULL){
        printf("Surface fail %s. IMG Error: %s\n", path, IMG_GetError());
    }
    else{
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if(newTexture == NULL){
            printf("Texture fail from %s. SDL Error %s\n", path, SDL_GetError());
        }
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

int renderTexto(char *string, SDL_Color cor){
    if(gFontTexture != NULL){
        SDL_DestroyTexture(gFontTexture);
        gFontTexture = NULL;
    }

    SDL_Surface *textSurface = TTF_RenderUTF8_Blended(gFont, string, cor);
    if(textSurface == NULL){
        printf("text failed. string: %s\n", string);
    }
    else{
        gFontTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        if(gFontTexture == NULL)
            printf("render text fail.\n");
        SDL_FreeSurface(textSurface);
    }
    return  gFontTexture != NULL;
}

int renderTextoWrapped(char *string, SDL_Color cor){
    if(gFontTexture != NULL){
        SDL_DestroyTexture(gFontTexture);
        gFontTexture = NULL;
    }

    SDL_Surface *textSurface = TTF_RenderUTF8_Blended_Wrapped(gFont, string, cor, 800);
    if(textSurface == NULL){
        printf("text failed w.\n");
    }
    else{
        gFontTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        if(gFontTexture == NULL)
            printf("render text fail w.\n");
        SDL_FreeSurface(textSurface);
    }
    return  gFontTexture != NULL;
}

int loadMediaBasic(){
    int success = 1;
    gBGTexture = loadTexture("Sprites/Misc/bg2.png");
    if(gBGTexture == NULL){
        printf("BGTexture fail. \n");
        success = 0;
    }
    gBattleBoxTexture = loadTexture("Sprites/Misc/textbox1.png");
    if(gBattleBoxTexture == NULL){
        printf("BoxTexture fail. \n");
        success = 0;
    }
    gBoxTexture = loadTexture("Sprites/Misc/textbox2.png");
    if(gBoxTexture == NULL){
        printf("BoxTexture fail. \n");
        success = 0;
    }
    gNameBoxTexture = loadTexture("Sprites/Misc/namebox2.png");
    if(gNameBoxTexture == NULL){
        printf("BoxTexture fail. \n");
        success = 0;
    }
    gMenuBoxTexture = loadTexture("Sprites/Misc/menubox.png");
    if(gMenuBoxTexture == NULL){
        printf("BoxTexture fail. \n");
        success = 0;
    }
    gCardTexture = (SDL_Texture**) malloc(sizeof(SDL_Texture*)*16);
    for(int i = 0; i < 16; i++){
        gCardTexture[i] = loadTexture(carta_db[i].carta_info.path);
        if(gCardTexture == NULL){
            printf("CardTexture fail. \n");
            success = 0;
        }
    }
    gFont = TTF_OpenFont("Ubuntu-C.ttf", 28);
    if(gFont == NULL){
        printf("Font fail. \n");
        success = 0;
    }
    gEnemyTexture = (SDL_Texture**) malloc(sizeof(SDL_Texture*) * 5);
    for(int i = 0; i < 5; i++){
        gEnemyTexture[i] = loadTexture(inimigo_db[i].path);
        if(gEnemyTexture[i] == NULL){
            printf("EnemyTexture fail. \n");
            success = 0;
        }
    }
    gIconTexture = (SDL_Texture**) malloc(sizeof(SDL_Texture*) * 2);
    gIconTexture[SHIELD_ICON] = loadTexture("Sprites/Misc/escudo.png");
    if(gIconTexture[SHIELD_ICON] == NULL){
        printf("ShieldTexture fail. \n");
        success = 0;
    }
    gIconTexture[PREV_ICON] = loadTexture("Sprites/Misc/prev.png");
    if(gIconTexture[PREV_ICON] == NULL){
        printf("PrevTexture fail. \n");
        success = 0;
    }
    gExpressionBoxTexture = loadTexture("Sprites/Misc/twewy.png");
    if(gExpressionBoxTexture == NULL){
        printf("ExpresisonTexture fail. \n");
        success = 0;
    }
    gCurrentScene = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCR_WID, SCR_HEI);
    if(gCurrentScene == NULL){
        printf("CurrentScene fail. \n");
        success = 0;
    }
    return success;
}

void loadMediaCurrentTexture(char *path){
    if(gModTexture != NULL){
        SDL_DestroyTexture(gModTexture);
        gModTexture = NULL;
    }
    gModTexture = loadTexture(path);
    if(gModTexture == NULL){
        printf("ModTexture fail\n");
    }
}

void closeWindow(){
    TTF_CloseFont(gFont);
    gFont = NULL;
    for(int i = 0; i < 16;i++){
        SDL_DestroyTexture(gCardTexture[i]);
    }
    free(gCardTexture);
    gCardTexture = NULL;
    for(int i = 0; i < 5; i++){
        SDL_DestroyTexture(gEnemyTexture[i]);
    }
    free(gEnemyTexture);
    gEnemyTexture = NULL;
    SDL_DestroyTexture(gIconTexture[PREV_ICON]);
    SDL_DestroyTexture(gIconTexture[SHIELD_ICON]);
    free(gIconTexture);
    if(gModTexture != NULL){
        SDL_DestroyTexture(gModTexture);
        gModTexture = NULL;
    }
    if(gFakeRect != NULL){
        SDL_DestroyTexture(gFakeRect);
        gFakeRect = NULL;
    }
    gIconTexture = NULL;
    SDL_DestroyTexture(gBoxTexture);
    gBoxTexture = NULL;
    SDL_DestroyTexture(gNameBoxTexture);
    gNameBoxTexture = NULL;
    SDL_DestroyTexture(gBattleBoxTexture);
    gBattleBoxTexture = NULL;
    SDL_DestroyTexture(gBGTexture);
    gBGTexture = NULL;
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

int mouseOver(SDL_Rect *button, SDL_Event *e){
    int inside = 1;
    int x, y;
    SDL_Rect vp;
    SDL_RenderGetViewport(gRenderer, &vp);
    SDL_GetMouseState(&x, &y);
    if(x < button->x + vp.x){
        inside = 0;
    }
    else if(x > button->x + vp.x + button->w){
        inside = 0;
    }
    else if(y < button->y + vp.y){
        inside = 0;
    }
    else if(y >= button->y + vp.y + button->h + 40){
        inside = 0;
    }
    return inside;
}

int mouseOverClickEnemy(SDL_Rect *button, SDL_Event *e){
    int inside = 1, click = 0;
    int x, y;
    SDL_GetMouseState(&x, &y);
    if(x < button->x + enemyAreaVP.x){
        inside = 0;
    }
    else if(x > button->x + enemyAreaVP.x + button->w){
        inside = 0;
    }
    else if(y < button->y + enemyAreaVP.y){
        inside = 0;
    }
    else if(y >= button->y + enemyAreaVP.y + button->h + 40){
        inside = 0;
    }
    if(inside){
        if(e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT){
            click = 1;
        }
    }
    return click;
}

int mouseOverClickCard(SDL_Rect *button, SDL_Event *e){
    int inside = 1, click = 0;
    int x, y;
    SDL_GetMouseState(&x, &y);
    if(x < button->x + stockVP.x){
        inside = 0;
    }
    else if(x > button->x + stockVP.x + button->w){
        inside = 0;
    }
    else if(y < button->y + stockVP.y){
        inside = 0;
    }
    else if(y >= button->y + stockVP.y + button->h + 40){
        inside = 0;
    }
    if(inside){
        if(e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT){
            click = 1;
        }
    }
    return click;
}

void antiMingwItoa(char *string, int x){
    int c = 0, d = 0, aux = x;
    char atlas[11];
    strcpy(atlas, "0123456789");
    if(x >= 100){
        c = x / 100;
        x -= c * 100;
    }
    if(x >= 10){
        d = x / 10;
        x -= d * 10;
    }
    strcpy(string, "");
    if(aux >= 100){
        string[0] = atlas[c];
        string[1] = atlas[d];
        string[2] = atlas[x];
        string[3] = '\0';
    }
    else if(aux >= 10){
        string[0] = ' ';
        string[1] = atlas[d];
        string[2] = atlas[x];
        string[3] = '\0';
    }
    else{
        string[0] = ' ';
        string[1] = ' ';
        string[2] = atlas[x];
        string[3] = '\0';
    }
}

void renderClear(){
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    SDL_RenderClear(gRenderer);
}

void renderMessageWrapped(char *msg, SDL_Rect *Viewport){
    int i, lines = 1;
    SDL_Rect msgArea;
    msgArea.x = 19; msgArea.y = 19;
    TTF_SizeUTF8(gFont, msg, &msgArea.w, &msgArea.h);
    
    if(msg[0] == '\0')
        return;
    
    for (i = 0; i < strlen(msg); i++){
        if(msg[i] == '\n')
            lines++;
    }
    msgArea.h *= lines * 0.5;
    msgArea.w *= lines * 0.5;
    SDL_RenderSetViewport(gRenderer, Viewport);
    loadMediaCurrentTexture("Sprites/Misc/textbox1.png");
    SDL_SetTextureAlphaMod(gModTexture, 200);
    SDL_RenderCopy(gRenderer, gModTexture, NULL, NULL);

    renderTextoWrapped(msg, black);
    SDL_RenderCopy(gRenderer, gFontTexture, NULL, &msgArea);
    msgArea.x -= 4; msgArea.y -= 4;
    renderTextoWrapped(msg, color);
    SDL_RenderCopy(gRenderer, gFontTexture, NULL, &msgArea);
    
}

void renderPrintOutline(char* msg, const SDL_Color *cor, const SDL_Color *corOutline, int sizeOutline, SDL_Rect *textArea){
    TTF_SetFontOutline(gFont, sizeOutline);
    renderTexto(msg, *corOutline);
    SDL_RenderCopy(gRenderer, gFontTexture, NULL, textArea);
    TTF_SetFontOutline(gFont, 0);
    renderTexto(msg, *cor);
    SDL_RenderCopy(gRenderer, gFontTexture, NULL, textArea);
}

void fadeInOut(int r, int g, int b, int alpha){
    SDL_SetRenderDrawColor(gRenderer, r, g, b, alpha);
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(gRenderer, NULL);
}

void mainMenu(Player *player, SDL_Event *e, int* quit){
    int alpha = 255, tick = 0, pause = 1;
    SDL_Rect logo = {SCR_WID/4, SCR_HEI/4, SCR_WID/2, SCR_HEI/2};
    SDL_Rect pressAny;
    TTF_SizeUTF8(gFont, "Aperte qualquer tecla para continuar.", &pressAny.w, &pressAny.h);
    pressAny.x = SCR_WID / 2 - pressAny.w / 2;
    pressAny.y = 3 * SCR_HEI / 4 - pressAny.h / 2;
    SDL_RenderSetViewport(gRenderer, NULL);
    while(alpha > 0){
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 255);
        SDL_RenderFillRect(gRenderer, NULL);
        renderPrintOutline("LOGO", &yellow, &black, 5, &logo);
        fadeInOut(0xFF, 0xFF, 0xFF, alpha);
        SDL_RenderPresent(gRenderer);
        alpha-=5;
    }
    while(pause)
    {
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 255);
        SDL_RenderFillRect(gRenderer, NULL);
        renderPrintOutline("LOGO", &yellow, &black, 5, &logo);
        if(tick){
            renderPrintOutline("Aperte qualquer tecla para continuar", &color, &black, 2, &pressAny);
        }
        SDL_RenderPresent(gRenderer);
        tick = (tick + 1) % 2;
        SDL_Delay(1000);
        SDL_FlushEvent(SDL_MOUSEBUTTONDOWN);
        SDL_FlushEvent(SDL_MOUSEMOTION);
        SDL_FlushEvent(SDL_KEYDOWN);
        while(SDL_PollEvent(e) != 0){
            if(e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_KEYDOWN){
                player->mainMenu = 0;
                return;
            }
        }
    }
}

void gameOver(SDL_Event *e){
    int alpha = 0, tick = 0, pause = 1;
    SDL_Rect gameover;
    SDL_Rect pressAny;
    TTF_SizeUTF8(gFont, "Game Over", &gameover.w, &gameover.h);
    gameover.w *= 2;
    gameover.h *= 2;
    gameover.x = SCR_WID / 2 - gameover.w / 2;
    gameover.y = SCR_HEI / 2 - gameover.h / 2;
    TTF_SizeUTF8(gFont, "Aperte qualquer tecla para continuar.", &pressAny.w, &pressAny.h);
    pressAny.x = SCR_WID / 2 - pressAny.w / 2;
    pressAny.y = 3 * SCR_HEI / 4 - pressAny.h / 2;
    SDL_RenderSetViewport(gRenderer, NULL);
    while(alpha < 255){
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, alpha);
        SDL_RenderFillRect(gRenderer, NULL);
        SDL_RenderPresent(gRenderer);
        alpha += 5;
    }
    while(alpha > 0){
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 255);
        SDL_RenderFillRect(gRenderer, NULL);
        renderPrintOutline("Game Over", &red, &color, 2, &gameover);
        fadeInOut(0x00, 0x00, 0x00, alpha);
        SDL_RenderPresent(gRenderer);
        alpha-=5;
    }
    while(pause)
    {
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
        SDL_RenderFillRect(gRenderer, NULL);
        renderPrintOutline("Game Over", &red, &color, 5, &gameover);
        if(tick){
            renderPrintOutline("Aperte qualquer tecla para continuar", &color, &black, 2, &pressAny);
        }
        SDL_RenderPresent(gRenderer);
        tick = (tick + 1) % 2;
        SDL_Delay(1000);
        SDL_FlushEvent(SDL_MOUSEBUTTONDOWN);
        SDL_FlushEvent(SDL_MOUSEMOTION);
        SDL_FlushEvent(SDL_KEYDOWN);
        while(SDL_PollEvent(e) != 0){
            if(e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_KEYDOWN){
                return;
            }
        }
    }
}

void renderBattleMessage(char *msg, const SDL_Rect *Viewport, int center){
    SDL_Rect msgArea;
    TTF_SizeUTF8(gFont, msg, &msgArea.w, &msgArea.h);
    if(center){
        msgArea.x = Viewport->w / 2 - msgArea.w / 2;
        msgArea.y = Viewport->h / 2 - msgArea.h / 2;
    }
    else{
        msgArea.x = 25; msgArea.y = 19;
    }

    SDL_RenderSetViewport(gRenderer, Viewport);
    SDL_SetTextureAlphaMod(gBattleBoxTexture, 200);
    SDL_RenderCopy(gRenderer, gBattleBoxTexture, NULL, NULL);

    renderTexto(msg, black);
    SDL_RenderCopy(gRenderer, gFontTexture, NULL, &msgArea);
    msgArea.x -= 4; msgArea.y -= 4;
    renderTexto(msg, color);
    SDL_RenderCopy(gRenderer, gFontTexture, NULL, &msgArea);
}

void renderBattle(Player *player, SDL_Event *e, Deck **deck, Mao **mao, SDL_Rect *enemyVP, SDL_Rect *cardAreaVP, InimigoBatalha* inimigos, int numeroInimigos, Seletor* select){
    int idcarta = 15, i = 0, x = 0, y = 0;
    char a[5];
    SDL_Rect cardText[5];
    SDL_Rect mousehover;
    SDL_Rect texthover;

    // limpa
    renderClear();
    
    // Battle Background
    SDL_RenderSetViewport(gRenderer, &battleBGVP);
    SDL_RenderCopy(gRenderer, gBGTexture, NULL, NULL);

    // Enemy Area
    SDL_RenderSetViewport(gRenderer, &enemyAreaVP);
        
        // Inimigo
        for(i = 0; i <= numeroInimigos; i++){
            if(!inimigos[i].derrotado){
                if(numeroInimigos == 2 && i == 1){
                    if(mouseOver(&enemyVP[i], e))
                        enemyVP[i].y = 3*(3 * SCR_HEI / 4 - 30)/4 - enemyVP[i].h - 15;
                    else
                        enemyVP[i].y = 3*(3 * SCR_HEI / 4 - 30)/4 - enemyVP[i].h;
                }
                else{
                    if(mouseOver(&enemyVP[i], e))
                        enemyVP[i].y = 3*(3 * SCR_HEI / 4 - 30)/4 - enemyVP[i].h + 40;
                    else
                        enemyVP[i].y = 3*(3 * SCR_HEI / 4 - 30)/4 - enemyVP[i].h + 50;
                }
                SDL_SetTextureAlphaMod(gEnemyTexture[inimigos[i].id], inimigos[i].alpha);
                SDL_RenderCopy(gRenderer, gEnemyTexture[inimigos[i].id], NULL, &enemyVP[i]);
            }
        }

        // hover box
        if(!select->ativoe){
            for(i = 0; i <= numeroInimigos; i++){
                if(!inimigos[i].derrotado){
                    if(mouseOver(&enemyVP[i], e)){
                        SDL_GetMouseState(&x, &y);
                        SDL_SetTextureAlphaMod(gBattleBoxTexture, 200);
                        TTF_SizeUTF8(gFont, inimigo_db[inimigos[i].id].nome, &(mousehover.w), &(mousehover.h));
                        mousehover.w += 30; mousehover.h += 30;
                        if(x + mousehover.w >= 1280)
                            mousehover.x = x - mousehover.w;
                        else
                            mousehover.x = x;
                        mousehover.y = y - mousehover.h;
                        texthover.x = 16; texthover.y = 15;

                        SDL_RenderSetViewport(gRenderer, &mousehover);
                        SDL_RenderCopy(gRenderer, gBattleBoxTexture, NULL, NULL);
                        TTF_SizeUTF8(gFont, inimigo_db[inimigos[i].id].nome, &(texthover.w), &(texthover.h));
                        renderTexto(inimigo_db[inimigos[i].id].nome, black);
                        SDL_RenderCopy(gRenderer, gFontTexture, NULL, &texthover);
                        texthover.x -= 4; texthover.y -= 4;
                        renderTexto(inimigo_db[inimigos[i].id].nome, color);
                        SDL_RenderCopy(gRenderer, gFontTexture, NULL, &texthover);
                        
                    }
                }
            }
        }

    // Menu
    SDL_RenderSetViewport(gRenderer, &menuVP);
    SDL_SetTextureAlphaMod(gMenuBoxTexture, 255);
    SDL_RenderCopy(gRenderer, gMenuBoxTexture, NULL, NULL);

        // Stats
        SDL_RenderSetViewport(gRenderer, &statsVP);
                  
            // barra CR
            SDL_Rect aux = {80, 55, 10, 28};
            renderTexto("/", black);
            SDL_RenderCopy(gRenderer, gFontTexture, NULL, &aux);
            aux.x -= 4; aux.y -= 4;
            renderTexto("/", color);
            SDL_RenderCopy(gRenderer, gFontTexture, NULL, &aux);
            
            // barra HP
            aux.x = 80; aux.y = 22; aux.w = 10; aux.h = 28;
            renderTexto("/", black);
            SDL_RenderCopy(gRenderer, gFontTexture, NULL, &aux);
            aux.x -= 4; aux.y -= 4;
            renderTexto("/", color);
            SDL_RenderCopy(gRenderer, gFontTexture, NULL, &aux);

            // CR MAX
            aux.x = 102; aux.y = 54; aux.w = 47*0.75; aux.h = 30;
            renderTexto("30", black);
            SDL_RenderCopy(gRenderer, gFontTexture, NULL, &aux);
            aux.x -= 4; aux.y -= 4;
            renderTexto("30", color);
            SDL_RenderCopy(gRenderer, gFontTexture, NULL, &aux);

            // CR Atual
            aux.x = 35; aux.y = 54; aux.w = 47*0.75; aux.h = 30;
            antiMingwItoa(a, qtd_deck(*deck));
            renderTexto(a, black);
            SDL_RenderCopy(gRenderer, gFontTexture, NULL, &aux);
            aux.x -= 4; aux.y -= 4;
            renderTexto(a, color);
            SDL_RenderCopy(gRenderer, gFontTexture, NULL, &aux);

            // HP
            renderTexto("HP", black);
            SDL_Rect hp = {156, 19, 47, 33};
            SDL_RenderCopy(gRenderer, gFontTexture, NULL, &hp);
            hp.x -= 4; hp.y -= 4;
            renderTexto("HP", color);
            SDL_RenderCopy(gRenderer, gFontTexture, NULL, &hp);

            // HP MAX
            aux.x = 102; aux.y = 21; aux.w = 47*0.75; aux.h = 30;
            renderTexto("250", black);
            SDL_RenderCopy(gRenderer, gFontTexture, NULL, &aux);
            aux.x -= 4; aux.y -= 4;
            renderTexto("250", color);
            SDL_RenderCopy(gRenderer, gFontTexture, NULL, &aux);

            // HP Atual
            aux.x = 35; aux.y = 21; aux.w = 47*0.75; aux.h = 30;
            antiMingwItoa(a, player->hp);
            renderTexto(a, black);
            SDL_RenderCopy(gRenderer, gFontTexture, NULL, &aux);
            aux.x -= 4; aux.y -= 4;
            renderTexto(a, color);
            SDL_RenderCopy(gRenderer, gFontTexture, NULL, &aux);

            // CR
            SDL_Rect deckInfo = {156, 52, 47, 33};
            renderTexto("CR", black);
            SDL_RenderCopy(gRenderer, gFontTexture, NULL, &deckInfo);
            deckInfo.x -= 4; deckInfo.y -= 4;
            renderTexto("CR", color);
            SDL_RenderCopy(gRenderer, gFontTexture, NULL, &deckInfo);

            // SHIELD
            if(select->escudo){
                SDL_Rect escudoArea = { 20, 130, 48, 48};
                SDL_RenderCopy(gRenderer, gIconTexture[SHIELD_ICON], NULL, &escudoArea);
            }

            // PREV
            if(select->prev){
                SDL_Rect prevArea = { 75, 130, 48, 48};
                SDL_RenderCopy(gRenderer, gIconTexture[PREV_ICON], NULL, &prevArea);
            }

    // Cartas
    SDL_RenderSetViewport(gRenderer, &stockVP);
    for (i = 0; i < qtd_mao(*mao); i++){
        if(mouseOver(&(cardAreaVP[i]), e) && !select->ativoc)
            cardAreaVP[i].y = 0;
        else if(select->ativoc && i == select->indexc)
            cardAreaVP[i].y = 0;
        else
            cardAreaVP[i].y = 15;
        cardText[i].x = cardAreaVP[i].x + 10;
        cardText[i].y = cardAreaVP[i].y + 3 * cardAreaVP[0].h/4 + 5;
        cardText[i].w = cardAreaVP[i].w - 20;
        cardText[i].h = 20;
        consulta_mao(*mao, i, &idcarta);
        if(select->ativoc && i == select->indexc){
            loadMediaCurrentTexture(carta_db[idcarta].carta_info.path);
            SDL_SetTextureColorMod(gModTexture, 200, 200, 0);
            SDL_RenderCopy(gRenderer, gModTexture, NULL, &cardAreaVP[i]);
        }
        else
            SDL_RenderCopy(gRenderer, gCardTexture[idcarta], NULL, &cardAreaVP[i]);

        renderTexto(carta_db[idcarta].carta_info.nome_carta, color);
        SDL_RenderCopy(gRenderer, gFontTexture, NULL, &cardText[i]);
    }

    // mouse hover effect
    if(!select->ativoc){
        for (i = 0; i < qtd_mao(*mao); i++){
            if(mouseOver(&cardAreaVP[i], e)){
                SDL_GetMouseState(&x, &y);
                SDL_SetTextureAlphaMod(gBattleBoxTexture, 200);
                mousehover.h = 170;
                mousehover.y = y - mousehover.h;
                mousehover.w = 400;
                if(x + mousehover.w >= 1280)
                    mousehover.x = x - mousehover.w;
                else
                    mousehover.x = x;
                texthover.x = 19; texthover.y = 19;
                consulta_mao(*mao, i, &idcarta);
                SDL_RenderSetViewport(gRenderer, &mousehover);
                SDL_RenderCopy(gRenderer, gBattleBoxTexture, NULL, NULL);
                TTF_SizeUTF8(gFont, carta_db[idcarta].carta_info.nome_carta, &(texthover.w), &(texthover.h));
                renderTexto(carta_db[idcarta].carta_info.nome_carta, black);
                SDL_RenderCopy(gRenderer, gFontTexture, NULL, &texthover);
                texthover.x -= 4; texthover.y -= 4;
                renderTexto(carta_db[idcarta].carta_info.nome_carta, color);
                SDL_RenderCopy(gRenderer, gFontTexture, NULL, &texthover);
                texthover.x += 4; texthover.y += texthover.h + 4;
                TTF_SizeUTF8(gFont, carta_db[idcarta].carta_info.elemento_carta, &(texthover.w), &(texthover.h));
                renderTexto(carta_db[idcarta].carta_info.elemento_carta, black);
                SDL_RenderCopy(gRenderer, gFontTexture, NULL, &texthover);
                texthover.x -= 4; texthover.y -= 4;
                renderTexto(carta_db[idcarta].carta_info.elemento_carta, color);
                SDL_RenderCopy(gRenderer, gFontTexture, NULL, &texthover);
                texthover.x += 4; texthover.y += texthover.h + 4;
                texthover.w = mousehover.w - 30;
                texthover.h *= 2;
                renderTextoWrapped(carta_db[idcarta].carta_info.descricao, black);
                SDL_RenderCopy(gRenderer, gFontTexture, NULL, &texthover);
                texthover.x -= 4; texthover.y -= 4;
                renderTextoWrapped(carta_db[idcarta].carta_info.descricao, color);
                SDL_RenderCopy(gRenderer, gFontTexture, NULL, &texthover);
            }
        }
    }
}


void battleInit(Deck **fi, Mao **li){
    *fi = cria_deck();
    *li = cria_mao();
    inicializa_deck(*fi);
}

void battleEnd(Deck **fi, Mao **li){
    libera_deck(*fi);
    libera_mao(*li);
    reseta_db();
}

int calculoDano(Player *player, int idcarta, InimigoBatalha *inimigos, Seletor *select, int *critical){
    double fatorAleatorio = 0, dano = 0;
    *critical = 0;
    fatorAleatorio = SDL_floor((rand() % (carta_db[idcarta].carta_info.range + 1)) + carta_db[idcarta].carta_info.rangemin);
    dano = SDL_floor(fatorAleatorio / 100 * player->atk * carta_db[idcarta].carta_info.multiplicador_carta/100);
    
    // Detector critical
    if(fatorAleatorio >= carta_db[idcarta].carta_info.rangemin + carta_db[idcarta].carta_info.range)
        *critical = 1;

    // Seletor elemento
    if(strcmp(carta_db[idcarta].carta_info.elemento_carta, "Fogo") == 0)
        dano = SDL_floor(dano * inimigo_db[inimigos[select->indexe].id].RElemento[0]/100);
    else if(strcmp(carta_db[idcarta].carta_info.elemento_carta, "Gelo") == 0)
        dano = SDL_floor(dano * inimigo_db[inimigos[select->indexe].id].RElemento[1]/100);
    else if(strcmp(carta_db[idcarta].carta_info.elemento_carta, "Elec") == 0)
        dano = SDL_floor(dano * inimigo_db[inimigos[select->indexe].id].RElemento[2]/100);
    
    // Deducao DEF
    if(strcmp(carta_db[idcarta].carta_info.elemento_carta, "Cura") != 0){
        if(inimigos[select->indexe].buff)
            dano = SDL_floor(dano - (inimigo_db[inimigos[select->indexe].id].DEF * inimigo_db[inimigos[select->indexe].id].comportamento.mult_defend/100));
        else
            dano = SDL_floor(dano - inimigo_db[inimigos[select->indexe].id].DEF);
    }
    
    // Dmg Caps
    if(dano > 999)
        dano = 999;
    if(dano <= 0)
        dano = 1;

    return dano;                                            
}

int battleOn(Player *player, SDL_Event *e, int *quit, int boss){
    int idcarta, battle = 1, numeroInimigos, inimigosTurno;
    int width, k, i, critical = 0,  roll;
    int escudoCount = 0, prevCount = 0, pause = 1;
    double fatorAleatorio, dano = 0;
    char msg[100];
    Deck *deck = NULL;
    Mao *mao = NULL;
    InimigoBatalha *inimigos = NULL;
    Seletor select = {1, 1, -1, -1, 0, 0, 0, 0, 1};
    SDL_Rect numeroSize;
    Animation animate = {0, 0, 0, 0, {0,0,0,0}, {0,0,0,0}};

    battleInit(&deck, &mao);
    player->turno = 1;
    
    // GERADOR INIMIGOS
    if(boss > 0){
        numeroInimigos = 0;
        inimigos = (InimigoBatalha*) malloc(sizeof(InimigoBatalha) * (numeroInimigos + 1));
        inimigos[0].id = 4;
        inimigos[0].hp = inimigo_db[inimigos[0].id].HP;
        inimigos[0].buff = 0;
        inimigos[0].buffCount = 0;
        inimigos[0].bolado = 0;
        inimigos[0].turno = 0;
        inimigos[0].derrotado = 0;
        inimigos[0].alpha = 0;
        inimigos[0].fleeCount = 0;
    }
    else{
        numeroInimigos = rand() % 3;
        inimigos = (InimigoBatalha*) malloc(sizeof(InimigoBatalha) * (numeroInimigos + 1));
        for(i = 0; i <= numeroInimigos; i++){
            inimigos[i].id = rand() % 3;
            inimigos[i].hp = inimigo_db[inimigos[i].id].HP;
            inimigos[i].buff = 0;
            inimigos[i].buffCount = 0;
            if(inimigos[i].id == 0)
                inimigos[i].bolado = 1;
            else
                inimigos[i].bolado = 0;
            inimigos[i].turno = 0;
            inimigos[i].derrotado = 0;
            inimigos[i].alpha = 0;
            inimigos[i].fleeCount = 0;
        }
    }

    width = 1040 / (numeroInimigos + 2);
    SDL_Rect enemyVP[3];
    for(i = 0;i <= numeroInimigos; i++){
        enemyVP[i].w = inimigo_db[inimigos[i].id].w;
        enemyVP[i].h = inimigo_db[inimigos[i].id].h;
        enemyVP[i].x = (width * (i + 1) - enemyVP[i].w / 2);
        if(numeroInimigos == 2 && i == 1)
            enemyVP[i].y = 3*(3 * SCR_HEI / 4 - 30)/4 - enemyVP[i].h;    
        else
            enemyVP[i].y = 3*(3 * SCR_HEI / 4 - 30)/4 - enemyVP[i].h + 50;
    }
    SDL_Rect cardAreaVP[5];
    for(i = 0; i < 5;i++){
        cardAreaVP[i].x = i * (213 - 15);
        cardAreaVP[i].y = 15;
        cardAreaVP[i].w = 213 - 30;
        cardAreaVP[i].h = SCR_HEI/4 - 35;
    }

    while(battle){
        while(SDL_PollEvent(e) != 0){
            if(e->type == SDL_QUIT){
                *quit = 1;
                return 0;
            }
            else if(e->type == SDL_KEYDOWN){
                switch(e->key.keysym.sym){
                    case SDLK_ESCAPE:
                    *quit = 1;
                    return 0;
                }
            }
        }
        
        if(select.primeiroTurno){
            // Animacao monstros surgindo
            renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
            SDL_RenderPresent(gRenderer);
            while(inimigos[0].alpha < 255){
                for(i = 0; i <= numeroInimigos;i++)
                    inimigos[i].alpha += 5;
                renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                SDL_RenderPresent(gRenderer);
            }

            // Mensagem #1: Quem apareceu?
            renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
            strcpy(msg, inimigo_db[inimigos[0].id].nome);
            for(i = 1; i <= numeroInimigos; i++){
                if(i == numeroInimigos)
                    strcat(msg, " e ");
                else
                    strcat(msg, ", ");
                strcat(msg, inimigo_db[inimigos[i].id].nome);
            }
            if(numeroInimigos == 0)
                strcat(msg, " apareceu!");
            else
                strcat(msg, " apareceram!");
            renderBattleMessage(msg, &msgAreaTopVP, 1);
            SDL_RenderPresent(gRenderer);
            SDL_Delay(1250);

            // Compra cartas iniciais
            for(i = 0; i < 5; i++){
                compra_carta(deck, mao);
                renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                SDL_RenderPresent(gRenderer);
                SDL_Delay(100);
            }

            // Mensagem #2: Inicio da batalha
            strcpy(msg, "É hora do duelo!");
            renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
            renderBattleMessage(msg, &msgAreaTopVP, 1);
            SDL_RenderPresent(gRenderer);
            SDL_Delay(1250);
            renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
            SDL_RenderPresent(gRenderer);
            SDL_Delay(100);

            select.primeiroTurno = 0;
        }
        // else if(deck_vazio(deck)){

        // }
        // Compra carta a cada rodada
        else if(!mao_cheia(mao)){
            compra_carta(deck, mao);
            renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
            SDL_RenderPresent(gRenderer);
            SDL_Delay(100);
        }

        // Mensagem #3: Turno do jogador
        player->turno = 1;
        strcpy(msg, "Sua vez!");
        renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
        renderBattleMessage(msg, &msgAreaTopVP, 1);

        /* IMPRIMIR \/ */

        SDL_RenderPresent(gRenderer);
        SDL_FlushEvent(SDL_MOUSEBUTTONDOWN);
        select.ativoc = 0;
        select.ativoe = 0;
        while(pause){
            SDL_WaitEvent(e);
            if(e->type == SDL_MOUSEBUTTONDOWN)
                break;
            else if(e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_ESCAPE){
                *quit = 1;
                return 0;
            }
        }

        // Turno do jogador
        while(player->turno){
            renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
            SDL_RenderPresent(gRenderer);
            while(SDL_PollEvent(e) != 0){
                if(e->type == SDL_QUIT){
                    battle = 0;
                    *quit = 1;
                    player->turno = 0;
                }
                else if(e->type == SDL_KEYDOWN){
                        switch(e->key.keysym.sym){
                            case SDLK_ESCAPE:
                            *quit = 1;
                            return 0;
                        }
                    }
                else if(e->type == SDL_MOUSEBUTTONDOWN && select.ativoc){
                    if(e->button.button == SDL_BUTTON_RIGHT){
                        select.ativoc = 0;
                        select.indexc = -1;
                        continue;
                    }
                    else if(e->button.button == SDL_BUTTON_LEFT){
                        for(k = 0; k <= numeroInimigos; k++){
                            if(!inimigos[k].derrotado){
                                if(mouseOverClickEnemy(&enemyVP[k], e)){
                                    select.indexe = k;
                                    select.ativoe = 1;
                                    renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                                    SDL_RenderPresent(gRenderer);
                                    // SDL_WaitEventTimeout(e, 150);

                                    // Calculo de dano
                                    dano = calculoDano(player, idcarta, inimigos, &select, &critical);

                                    /* animacao efeito */

                                    // Animacao Dano
                                    numeroSize.x = enemyVP[k].x + enemyVP[k].w / 2 - 47;
                                    numeroSize.y = enemyVP[k].y + enemyVP[k].h / 2 - 33;
                                    numeroSize.w = 94; numeroSize.h = 66;
                                    animate.alpha = 255;
                                    while(animate.alpha > 0){
                                        renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                                        SDL_RenderSetViewport(gRenderer, NULL);

                                        // Animacao critical
                                        if(critical){
                                            TTF_SetFontOutline(gFont, 10);
                                            strcpy(msg, "CRITICAL!");
                                            renderTexto(msg, black);
                                            SDL_SetTextureAlphaMod(gFontTexture, animate.alpha);
                                            numeroSize.y -= 30;
                                            numeroSize.x -= 15;
                                            numeroSize.w += 30;
                                            numeroSize.h -= 20;
                                            SDL_RenderCopy(gRenderer, gFontTexture, NULL, &numeroSize);
                                            TTF_SetFontOutline(gFont, 0);
                                            renderTexto(msg, yellow);
                                            SDL_SetTextureAlphaMod(gFontTexture, animate.alpha);
                                            SDL_RenderCopy(gRenderer, gFontTexture, NULL, &numeroSize);
                                            // SDL_RenderPresent(gRenderer);
                                            numeroSize.y += 30;
                                            numeroSize.x += 15;
                                            numeroSize.w -= 30;
                                            numeroSize.h += 20;
                                        }

                                        // Animacao dano
                                        antiMingwItoa(msg, dano);
                                        TTF_SetFontOutline(gFont, 2);
                                        renderTexto(msg, black);
                                        SDL_SetTextureAlphaMod(gFontTexture, animate.alpha);
                                        SDL_RenderCopy(gRenderer, gFontTexture, NULL, &numeroSize);
                                        TTF_SetFontOutline(gFont, 0);
                                        renderTexto(msg, color);
                                        SDL_SetTextureAlphaMod(gFontTexture, animate.alpha);
                                        SDL_RenderCopy(gRenderer, gFontTexture, NULL, &numeroSize);
                                        SDL_RenderPresent(gRenderer);
                                        numeroSize.y -= 3;
                                        animate.alpha -= 5;
                                    }
                                    critical = 0;

                                    // Deducao HP
                                    inimigos[k].hp -= dano;
                                    if(inimigos[k].bolado){
                                        inimigos[k].bolado = 0;
                                        strcpy(msg, "Você cometeu um grave erro!");
                                        renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                                        renderBattleMessage(msg, &msgAreaTopVP, 1);
                                        SDL_RenderPresent(gRenderer);
                                        SDL_Delay(1250);

                                        inimigos[k].alpha = 255;
                                        while(inimigos[k].alpha > 0){
                                            inimigos[k].alpha -= 5;
                                            renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                                            SDL_RenderPresent(gRenderer);
                                        }

                                        inimigos[k].id = 3;
                                        while(inimigos[k].alpha < 255){
                                            inimigos[k].alpha += 5;
                                            renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                                            SDL_RenderPresent(gRenderer);
                                        }
                                    }
                                    // Detector deadbody
                                    if(inimigos[k].hp <= 0){
                                        inimigos[k].alpha = 255;
                                        while(inimigos[k].alpha > 0){
                                            inimigos[k].alpha -= 5;
                                            renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                                            SDL_RenderPresent(gRenderer);
                                        }
                                        inimigos[k].derrotado = 1;
                                    }
                                    select.ativoc = 0;
                                    select.ativoe = 0;
                                    descarta_carta(mao, select.indexc);
                                    select.indexc = -1;
                                    player->turno = 0;
                                    // SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
                                    break;
                                }
                            }
                        }
                    }
                }
                else if(e->type == SDL_MOUSEBUTTONDOWN && !select.ativoc){
                    if(e->button.button == SDL_BUTTON_LEFT){
                        // Checa se houve clique sobre uma das cartas
                        for(i = 0; i < mao->qtd_cartas; i++){
                            if(mouseOverClickCard(&cardAreaVP[i], e)){
                                select.indexc = i;
                                consulta_mao(mao, select.indexc, &idcarta);
                                select.ativoc = 1;
                                if(strcmp(carta_db[idcarta].carta_info.efeito, "Ataque") == 0){
                                    continue;
                                }
                                else if(strcmp(carta_db[idcarta].carta_info.efeito, "Cura") == 0){
                                    // Calculo cura
                                    dano = calculoDano(player, idcarta, inimigos, &select, &critical);

                                    select.ativoc = 1;
                                    player->hp += dano;
                                    if(player->hp > player->hpmax)
                                        player->hp = player->hpmax;
                                    renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                                    SDL_RenderPresent(gRenderer);
                                    
                                    // Animacao valor
                                    antiMingwItoa(msg, dano);
                                    numeroSize.x = 31; numeroSize.y = 600; numeroSize.w = 94; numeroSize.h = 66;
                                    animate.alpha = 255;
                                    while(animate.alpha > 0){
                                        renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                                        SDL_RenderSetViewport(gRenderer, NULL);
                                        TTF_SetFontOutline(gFont, 2);
                                        renderTexto(msg, black);
                                        SDL_SetTextureAlphaMod(gFontTexture, animate.alpha);
                                        SDL_RenderCopy(gRenderer, gFontTexture, NULL, &numeroSize);
                                        TTF_SetFontOutline(gFont, 0);
                                        renderTexto(msg, green);
                                        SDL_SetTextureAlphaMod(gFontTexture, animate.alpha);
                                        SDL_RenderCopy(gRenderer, gFontTexture, NULL, &numeroSize);
                                        SDL_RenderPresent(gRenderer);
                                        numeroSize.y -= 3;
                                        animate.alpha -= 5;
                                    }

                                    select.ativoc = 0;
                                    player->turno = 0;
                                    descarta_carta(mao, select.indexc);
                                }
                                else if(strcmp(carta_db[idcarta].carta_info.efeito, "Escudo") == 0){
                                    select.escudo = 1;
                                    escudoCount = -1;
                                    select.ativoc = 1;
                                    renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                                    SDL_RenderPresent(gRenderer);

                                    /* ANIMACAO */

                                    select.ativoc = 0;
                                    player->turno = 0;
                                    descarta_carta(mao, select.indexc);
                                }
                                else if(strcmp(carta_db[idcarta].carta_info.efeito, "Prev") == 0){
                                    select.prev = 1;
                                    prevCount = -1;
                                    select.ativoc = 1;
                                    renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                                    SDL_RenderPresent(gRenderer);

                                    /* ANIMACAO */

                                    select.ativoc = 0;
                                    player->turno = 0;
                                    descarta_carta(mao, select.indexc);
                                }
                                else if(strcmp(carta_db[idcarta].carta_info.efeito, "Saque") == 0){
                                    select.ativoc = 1;
                                    renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                                    SDL_RenderPresent(gRenderer);
                                    
                                    descarta_carta(mao, select.indexc);
                                    select.ativoc = 0;
                                    renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                                    SDL_RenderPresent(gRenderer);
                                    SDL_Delay(50);

                                    compra_carta(deck, mao);
                                    renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                                    SDL_RenderPresent(gRenderer);
                                    SDL_Delay(50);
                                    if(!mao_cheia(mao)){
                                        compra_carta(deck, mao);
                                        renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                                        SDL_RenderPresent(gRenderer);
                                        SDL_Delay(50);
                                    }
                                    player->turno = 0;
                                }
                                SDL_Delay(100);
                            }
                        }
                    }
                    if(!player->turno)
                        break;
                    for(i = 0; i <= numeroInimigos; i++){
                        if(!inimigos[i].derrotado){
                            if(mouseOverClickEnemy(&enemyVP[i], e)){
                                select.indexe = i;
                                select.ativoe = 1;
                                idcarta = 15;
                                renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                                SDL_RenderPresent(gRenderer);

                                // Calculo de dano
                                dano = calculoDano(player, idcarta, inimigos, &select, &critical);

                                /* animacao efeito */

                                // Animacao Dano
                                numeroSize.x = enemyVP[i].x + enemyVP[i].w / 2 - 47;
                                numeroSize.y = enemyVP[i].y + enemyVP[i].h / 2 - 33;
                                numeroSize.w = 94; numeroSize.h = 66;
                                animate.alpha = 255;
                                while(animate.alpha > 0){
                                    renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                                    SDL_RenderSetViewport(gRenderer, NULL);

                                    // Animacao critical
                                    if(critical){
                                        TTF_SetFontOutline(gFont, 10);
                                        strcpy(msg, "CRITICAL!");
                                        renderTexto(msg, black);
                                        SDL_SetTextureAlphaMod(gFontTexture, animate.alpha);
                                        numeroSize.y -= 30;
                                        numeroSize.x -= 15;
                                        numeroSize.w += 30;
                                        numeroSize.h -= 20;
                                        SDL_RenderCopy(gRenderer, gFontTexture, NULL, &numeroSize);
                                        TTF_SetFontOutline(gFont, 0);
                                        renderTexto(msg, yellow);
                                        SDL_SetTextureAlphaMod(gFontTexture, animate.alpha);
                                        SDL_RenderCopy(gRenderer, gFontTexture, NULL, &numeroSize);
                                        // SDL_RenderPresent(gRenderer);
                                        numeroSize.y += 30;
                                        numeroSize.x += 15;
                                        numeroSize.w -= 30;
                                        numeroSize.h += 20;
                                    }

                                    // Animacao dano
                                    antiMingwItoa(msg, dano);
                                    TTF_SetFontOutline(gFont, 2);
                                    renderTexto(msg, black);
                                    SDL_SetTextureAlphaMod(gFontTexture, animate.alpha);
                                    SDL_RenderCopy(gRenderer, gFontTexture, NULL, &numeroSize);
                                    TTF_SetFontOutline(gFont, 0);
                                    renderTexto(msg, color);
                                    SDL_SetTextureAlphaMod(gFontTexture, animate.alpha);
                                    SDL_RenderCopy(gRenderer, gFontTexture, NULL, &numeroSize);
                                    SDL_RenderPresent(gRenderer);
                                    numeroSize.y -= 3;
                                    animate.alpha -= 5;
                                }
                                critical = 0;

                                // Deducao HP
                                inimigos[i].hp -= dano;
                                if(inimigos[i].bolado){
                                    inimigos[i].bolado = 0;
                                    strcpy(msg, "Você cometeu um grave erro!");
                                    renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                                    renderBattleMessage(msg, &msgAreaTopVP, 1);
                                    SDL_RenderPresent(gRenderer);
                                    SDL_Delay(1250);

                                    inimigos[i].alpha = 255;
                                    while(inimigos[i].alpha > 0){
                                        inimigos[i].alpha -= 5;
                                        renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                                        SDL_RenderPresent(gRenderer);
                                    }

                                    inimigos[i].id = 3;
                                    while(inimigos[i].alpha < 255){
                                        inimigos[i].alpha += 5;
                                        renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                                        SDL_RenderPresent(gRenderer);
                                    }
                                }
                                // Detector deadbody
                                if(inimigos[i].hp <= 0){
                                    inimigos[i].alpha = 255;
                                    while(inimigos[i].alpha > 0){
                                        inimigos[i].alpha -= 5;
                                        renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                                        SDL_RenderPresent(gRenderer);
                                    }
                                    inimigos[i].derrotado = 1;
                                }
                                select.ativoc = 0;
                                select.ativoe = 0;
                                player->turno = 0;
                                break;
                            }
                        }
                    }
                }
                if(!battle)
                    break;
            }
        }
        if(!battle)
            break;

        select.ativoc = 1;
        select.indexc = -1;
        select.ativoe = 1;
        select.indexe = -1;
        inimigosTurno = 1;
        // Turno Inimigos
        while(inimigosTurno){
            for(i = 0; i <= numeroInimigos; i++){
                inimigos[i].turno = 1;
                while(inimigos[i].turno && !inimigos[i].derrotado){
                    while(SDL_PollEvent(e) != 0){
                        if(e->type == SDL_QUIT){
                            *quit = 1;
                            return 0;
                        }
                    }
                    strcpy(msg, "Vez do ");
                    strcat(msg, inimigo_db[inimigos[i].id].nome);
                    strcat(msg, "!");
                    renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                    renderBattleMessage(msg, &msgAreaTopVP, 1);
                    SDL_RenderPresent(gRenderer);
                    SDL_Delay(1250);
                    if(inimigos[i].fleeCount >= 5 && inimigos[i].id == 0){
                        k = 20;
                        inimigos[i].fleeCount = 100;
                    }
                    else{
                        roll = rand() % 100 + 1;
                        for(k = 0; k < 5; k++){
                            if (roll >= inimigo_db[inimigos[i].id].chance[k].min && roll <= inimigo_db[inimigos[i].id].chance[k].max)
                                break;
                        }
                    }
                    switch(k){
                        case ATTACK:

                            // Calculo de dano inimigo
                            fatorAleatorio = (rand() % 51) + 75;
                            dano = SDL_floor(fatorAleatorio/100 * inimigo_db[inimigos[i].id].ATK * inimigo_db[inimigos[i].id].comportamento.mult_atk/100);
                            dano = dano - player->def;
                            
                            // Dmg Cap
                            if(dano <= 0)
                                dano = 1;
                            if(dano > 999)
                                dano = 999;

                            if(select.escudo && dano > 0){
                                strcpy(msg, inimigo_db[inimigos[i].id].nome);
                                strcat(msg, " tentou atacar, mas Escudo Mágico te protegeu!");
                                renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                                renderBattleMessage(msg, &msgAreaTopVP, 1);
                                SDL_RenderPresent(gRenderer);
                                SDL_Delay(1250);
                                while(pause){
                                    SDL_WaitEvent(e);
                                    if(e->type == SDL_MOUSEBUTTONDOWN)
                                        break;
                                }
                                inimigos[i].turno = 0;
                                // renderMessageWrapped(msg, &msgAreaTopVP);
                                break;
                            }

                            // Animacao dano
                            numeroSize.x = 31; numeroSize.y = 600; numeroSize.w = 94; numeroSize.h = 66;
                            numeroSize.w = 94; numeroSize.h = 66;
                            animate.alpha = 255;
                            while(animate.alpha > 0){
                                renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                                SDL_RenderSetViewport(gRenderer, NULL);
                                if(critical){
                                    TTF_SetFontOutline(gFont, 10);
                                    strcpy(msg, "CRITICAL!");
                                    renderTexto(msg, black);
                                    SDL_SetTextureAlphaMod(gFontTexture, animate.alpha);
                                    numeroSize.y -= 30; numeroSize.x -= 15;
                                    numeroSize.w += 30; numeroSize.h -= 20;
                                    SDL_RenderCopy(gRenderer, gFontTexture, NULL, &numeroSize);
                                    TTF_SetFontOutline(gFont, 0);
                                    renderTexto(msg, yellow);
                                    SDL_SetTextureAlphaMod(gFontTexture, animate.alpha);
                                    SDL_RenderCopy(gRenderer, gFontTexture, NULL, &numeroSize);
                                    numeroSize.y += 30; numeroSize.x += 15;
                                    numeroSize.w -= 30; numeroSize.h += 20;
                                }
                                antiMingwItoa(msg, dano);
                                TTF_SetFontOutline(gFont, 2);
                                renderTexto(msg, black);
                                SDL_SetTextureAlphaMod(gFontTexture, animate.alpha);
                                SDL_RenderCopy(gRenderer, gFontTexture, NULL, &numeroSize);
                                TTF_SetFontOutline(gFont, 0);
                                renderTexto(msg, red);
                                SDL_SetTextureAlphaMod(gFontTexture, animate.alpha);
                                SDL_RenderCopy(gRenderer, gFontTexture, NULL, &numeroSize);
                                SDL_RenderPresent(gRenderer);
                                numeroSize.y -= 3;
                                animate.alpha -= 5;
                            }
                            critical = 0;
                            player->hp -= dano;
                            if(player->hp < 0)
                                player->hp = 0;
                            if(player->hp == 0){
                                battle = 0;
                                player->gameOver = 1;
                                animate.alpha = 0;
                                player->mainMenu = 1;
                                while(animate.alpha < 128){


                                    fadeInOut(200, 100, 100, animate.alpha);
                                    SDL_RenderPresent(gRenderer);
                                    animate.alpha += 1;
                                }
                                SDL_Delay(1000);
                                return 0;
                            }
                        inimigos[i].turno = 0;
                        break;

                        case MUG:
                                strcpy(msg, inimigo_db[inimigos[i].id].nome);
                                strcat(msg, " está tentando te roubar...");
                                renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                                renderBattleMessage(msg, &msgAreaTopVP, 1);
                                SDL_RenderPresent(gRenderer);
                                SDL_Delay(1250);

                            if(select.prev){
                                strcpy(msg, "Punho Firme te protegeu!");
                                renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                                renderBattleMessage(msg, &msgAreaTopVP, 1);
                                SDL_RenderPresent(gRenderer);
                                SDL_Delay(1250);
                                while(pause){
                                    SDL_WaitEvent(e);
                                    if(e->type == SDL_MOUSEBUTTONDOWN)
                                        break;
                                }
                                inimigos[i].turno = 0;
                                break;
                            }

                            k = inimigo_db[inimigos[i].id].comportamento.discard_num;
                            strcpy(msg, inimigo_db[inimigos[i].id].nome);
                            strcat(msg, " te roubou!");
                            renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                            renderBattleMessage(msg, &msgAreaTopVP, 1);
                            SDL_RenderPresent(gRenderer);
                            SDL_Delay(1250);
                            while(k > 0 && !mao_vazia(mao)){
                                roll = rand() % mao->qtd_cartas;
                                consulta_mao(mao, roll, &idcarta);
                                descarta_carta(mao, roll);
                                strcpy(msg, carta_db[idcarta].carta_info.nome_carta);
                                strcat(msg, " foi perdida!");
                                renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                                renderBattleMessage(msg, &msgAreaTopVP, 1);
                                SDL_RenderPresent(gRenderer);
                                SDL_Delay(1250);
                                k--;
                            }
                        inimigos[i].turno = 0;
                        break;

                        case FLEE:
                            roll = rand() % 100 + 1;
                            if(roll <= inimigo_db[inimigos[i].id].comportamento.flee_chance + 5*inimigos[i].fleeCount){
                                strcpy(msg, inimigo_db[inimigos[i].id].nome);
                                strcat(msg, " fugiu!");
                                inimigos[i].alpha = 255;
                                while(inimigos[i].alpha > 0){
                                    inimigos[i].alpha -= 5;
                                    renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                                    renderBattleMessage(msg, &msgAreaTopVP, 1);
                                    SDL_RenderPresent(gRenderer);
                                }
                                inimigos[i].derrotado = 1;
                            }
                            else{
                                strcpy(msg, inimigo_db[inimigos[i].id].nome);
                                strcat(msg, " está tentando fugir...");
                            }
                            renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                            renderBattleMessage(msg, &msgAreaTopVP, 1);
                            SDL_RenderPresent(gRenderer);
                            SDL_Delay(1250);
                        inimigos[i].fleeCount++;
                        inimigos[i].turno = 0;
                        break;

                        case SPECIAL_MOVE:

                            strcpy(msg, inimigo_db[inimigos[i].id].nome);
                            strcat(msg, " irá atacar!!");
                            renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                            renderBattleMessage(msg, &msgAreaTopVP, 1);
                            SDL_RenderPresent(gRenderer);
                            SDL_Delay(1250);
                            strcpy(msg, inimigo_db[inimigos[i].id].comportamento.nome_special);
                            renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                            renderBattleMessage(msg, &msgAreaTopVP, 1);
                            SDL_RenderPresent(gRenderer);
                            SDL_Delay(1250);
                            
                            if(select.escudo && inimigo_db[inimigos[i].id].comportamento.mult_special > 0){
                                strcpy(msg, "Escudo Mágico te protegeu!");
                                renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                                renderBattleMessage(msg, &msgAreaTopVP, 1);
                                SDL_RenderPresent(gRenderer);
                                SDL_Delay(1250);
                                // renderMessageWrapped(msg, &msgAreaTopVP);
                                inimigos[i].turno = 0;
                                break;
                            }

                            // Cálculo de dano SP MOVE
                            fatorAleatorio = (rand() % 51) + 75;
                            dano = SDL_floor(fatorAleatorio/100 * inimigo_db[inimigos[i].id].ATK * inimigo_db[inimigos[i].id].comportamento.mult_special/100);
                            dano = dano - player->def;

                            // SP Cap
                            if(dano > 999)
                                dano = 999;
                            if(dano <= 0 && inimigo_db[inimigos[i].id].comportamento.mult_special >= 0)
                                dano = 1;
                            else if(inimigo_db[inimigos[i].id].comportamento.mult_special < 0){
                                dano *= -1;
                                select.absorb = 1;
                            }
                            antiMingwItoa(msg, dano);
                            numeroSize.x = 31; numeroSize.y = 600; numeroSize.w = 94; numeroSize.h = 66;
                            numeroSize.w = 94; numeroSize.h = 66;
                            animate.alpha = 255;
                            while(animate.alpha > 0){
                                renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                                SDL_RenderSetViewport(gRenderer, NULL);
                                if(critical && !select.absorb){
                                    TTF_SetFontOutline(gFont, 10);
                                    strcpy(msg, "CRITICAL!");
                                    renderTexto(msg, black);
                                    SDL_SetTextureAlphaMod(gFontTexture, animate.alpha);
                                    numeroSize.x -= 15; numeroSize.y -= 30;
                                    numeroSize.w += 30; numeroSize.h -= 20;
                                    SDL_RenderCopy(gRenderer, gFontTexture, NULL, &numeroSize);
                                    TTF_SetFontOutline(gFont, 0);
                                    renderTexto(msg, yellow);
                                    SDL_SetTextureAlphaMod(gFontTexture, animate.alpha);
                                    SDL_RenderCopy(gRenderer, gFontTexture, NULL, &numeroSize);
                                    // SDL_RenderPresent(gRenderer);
                                    numeroSize.x += 15; numeroSize.y += 30;
                                    numeroSize.w -= 30; numeroSize.h += 20;
                                }
                                antiMingwItoa(msg, dano);
                                TTF_SetFontOutline(gFont, 2);
                                renderTexto(msg, black);
                                SDL_SetTextureAlphaMod(gFontTexture, animate.alpha);
                                SDL_RenderCopy(gRenderer, gFontTexture, NULL, &numeroSize);
                                TTF_SetFontOutline(gFont, 0);
                                if(select.absorb){
                                    renderTexto(msg, green);
                                }
                                else
                                    renderTexto(msg, red);
                                SDL_SetTextureAlphaMod(gFontTexture, animate.alpha);
                                SDL_RenderCopy(gRenderer, gFontTexture, NULL, &numeroSize);
                                SDL_RenderPresent(gRenderer);
                                numeroSize.y -= 3;
                                animate.alpha -= 5;
                            }
                            critical = 0;
                            if(select.absorb)
                                dano *= -1;
                            select.absorb = 0;
                            player->hp -= dano;
                            if(player->hp < 0)
                                player->hp = 0;
                            if(player->hp > player->hpmax)
                                player->hp = player->hpmax;
                            if(player->hp == 0){
                                battle = 0;
                                player->gameOver = 1;
                                animate.alpha = 0;
                                player->mainMenu = 1;
                                while(animate.alpha < 128){
                                    
                                    

                                    fadeInOut(200, 100, 100, animate.alpha);
                                    SDL_RenderPresent(gRenderer);
                                    animate.alpha += 1;
                                }
                                SDL_Delay(1000);
                                return 0;
                            }
                        inimigos[i].turno = 0;
                        break;

                        case DEFEND:
                            strcpy(msg, inimigo_db[inimigos[i].id].nome);
                            strcat(msg, " está preparando para se defender.");
                            renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                            renderBattleMessage(msg, &msgAreaTopVP, 1);
                            SDL_RenderPresent(gRenderer);
                            SDL_Delay(1250);
                            inimigos[i].buff = 1;
                            inimigos[i].buffCount = inimigo_db[inimigos[i].id].buffdefault + 1;
                        inimigos[i].turno = 0;
                        break;
                    }
                    if(!battle)
                        break;
                }
                if(!battle)
                    break;
            }
            inimigosTurno = 0;
            if(!battle)
                break;
        }

        // Fim Turno
        if(!battle)
            break;

        battle = 0;
        for(i = 0; i <= numeroInimigos; i++){
            if(!inimigos[i].derrotado){
                battle = 1;
                break;
            }
        }
        
        if(select.escudo)
            escudoCount++;
        if(escudoCount >= 2){
            select.escudo = 0;
            strcpy(msg, "Escudo Mágico se dissipou!");
            renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
            renderBattleMessage(msg, &msgAreaTopVP, 1);
            SDL_Delay(1250);
        }
        if(select.prev)
            prevCount++;
        if(prevCount >= 4){
            select.prev = 0;
            strcpy(msg, "Punho Firme se dissipou!");
            renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
            renderBattleMessage(msg, &msgAreaTopVP, 1);
            SDL_Delay(1250);
        }

        for(i = 0; i <= numeroInimigos; i++){
            if(inimigos[i].buff)
                inimigos[i].buffCount--;
            if(inimigos[i].buffCount <= 0)
                inimigos[i].buff = 0;
        }
        
    }
    free(inimigos);
    battleEnd(&deck, &mao);
    return 0;
}

int main(int argc, char* args[]) // SDL requer que main tenha estes argumentos
{
    if(!init())
        printf("init failed.\n");
    else{
        cria_db_carta();
        cria_db_inimigo();
        if(!loadMediaBasic())
            printf("load failed.\n");
        else{
            int quit = 0, battle = 0;
            SDL_Event e;
            Player player = {250, 250, 100, 30, 0, 0, 1, 0};
            SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);

            // main loop
            while(!quit){
                while(SDL_PollEvent(&e) != 0){
                    if(e.type == SDL_QUIT)
                        quit = 1;
                }
                player.gameOver = 0;
                /* MAIN MENU */
                if(player.mainMenu)
                    mainMenu(&player, &e, &quit);

                /* HISTORIA */

                battle = rand() % 2;
                if(battle)
                    battle = battleOn(&player, &e, &quit, 0);

                if(player.gameOver)
                    gameOver(&e);
            }
            libera_db_carta();
            closeWindow();
        }
    }
    return 0;
}