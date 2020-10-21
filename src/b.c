#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../Deck/deck.h"
#include "../Batalha/Inimigo.h"

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
SDL_Texture *gFontTexture = NULL;
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

SDL_Surface* loadSurface(char* path){
    SDL_Surface *optimizedSurface = NULL;
    SDL_Surface *loadedSurface = IMG_Load(path);
    if(loadedSurface == NULL)
        printf("Unable do load image %s! SDL Error: %s\n", path, IMG_GetError());
    else{
        optimizedSurface = SDL_ConvertSurface(loadedSurface, gSurface->format, 0);
        if(optimizedSurface == NULL)
            printf("Unable to optimize image %s! SDL Error %s\n", path, SDL_GetError());
        SDL_FreeSurface(loadedSurface);
    }
    return loadedSurface;
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

int renderText(char *string, SDL_Color textColor){
    if(gFontTexture != NULL){
        SDL_DestroyTexture(gFontTexture);
        gFontTexture = NULL;
    }

    SDL_Surface *textSurface = TTF_RenderUTF8_Blended(gFont, string, textColor);
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

int renderTextWrapped(char *string, SDL_Color textColor){
    if(gFontTexture != NULL){
        SDL_DestroyTexture(gFontTexture);
        gFontTexture = NULL;
    }

    SDL_Surface *textSurface = TTF_RenderUTF8_Blended_Wrapped(gFont, string, textColor, 800);
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

void itoa(char *string, int x){
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

    renderTextWrapped(msg, black);
    SDL_RenderCopy(gRenderer, gFontTexture, NULL, &msgArea);
    msgArea.x -= 4; msgArea.y -= 4;
    renderTextWrapped(msg, color);
    SDL_RenderCopy(gRenderer, gFontTexture, NULL, &msgArea);
    
}

void renderBattleMessage(char *msg, SDL_Rect *Viewport){
    SDL_Rect msgArea;
    msgArea.x = 25; msgArea.y = 19;
    TTF_SizeUTF8(gFont, msg, &msgArea.w, &msgArea.h);

    SDL_RenderSetViewport(gRenderer, Viewport);
    SDL_SetTextureAlphaMod(gBattleBoxTexture, 200);
    SDL_RenderCopy(gRenderer, gBattleBoxTexture, NULL, NULL);

    renderText(msg, black);
    SDL_RenderCopy(gRenderer, gFontTexture, NULL, &msgArea);
    msgArea.x -= 4; msgArea.y -= 4;
    renderText(msg, color);
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
                        mousehover.h = 65;
                        mousehover.y = y - mousehover.h;
                        mousehover.w = 150;
                        if(x + mousehover.w >= 1280)
                            mousehover.x = x - mousehover.w;
                        else
                            mousehover.x = x;
                        texthover.x = 19; texthover.y = 19;
                        TTF_SizeUTF8(gFont, inimigo_db[inimigos[i].id].nome, &(mousehover.w), &(mousehover.h));
                        // mousehover.w += 30; mousehover.h += 30;
                        mousehover.w *= 2; mousehover.h *= 2;
                        mousehover.w += 45; mousehover.h += 45;

                        SDL_RenderSetViewport(gRenderer, &mousehover);
                        SDL_RenderCopy(gRenderer, gBattleBoxTexture, NULL, NULL);
                        TTF_SizeUTF8(gFont, inimigo_db[inimigos[i].id].nome, &(texthover.w), &(texthover.h));
                        renderText(inimigo_db[inimigos[i].id].nome, black);
                        SDL_RenderCopy(gRenderer, gFontTexture, NULL, &texthover);
                        texthover.x -= 4; texthover.y -= 4;
                        renderText(inimigo_db[inimigos[i].id].nome, color);
                        SDL_RenderCopy(gRenderer, gFontTexture, NULL, &texthover);
                        
                        texthover.x += 4; texthover.y += texthover.h + 4;
                        char metaData[100];
                        itoa(metaData, inimigos[i].hp);
                        renderText(metaData, black);
                        SDL_RenderCopy(gRenderer, gFontTexture, NULL, &texthover);
                        texthover.x -= 4; texthover.y -= 4;
                        renderText(metaData, color);
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
            renderText("/", black);
            SDL_RenderCopy(gRenderer, gFontTexture, NULL, &aux);
            aux.x -= 4; aux.y -= 4;
            renderText("/", color);
            SDL_RenderCopy(gRenderer, gFontTexture, NULL, &aux);
            
            // barra HP
            aux.x = 80; aux.y = 22; aux.w = 10; aux.h = 28;
            renderText("/", black);
            SDL_RenderCopy(gRenderer, gFontTexture, NULL, &aux);
            aux.x -= 4; aux.y -= 4;
            renderText("/", color);
            SDL_RenderCopy(gRenderer, gFontTexture, NULL, &aux);

            // CR MAX
            aux.x = 102; aux.y = 54; aux.w = 47*0.75; aux.h = 30;
            renderText("30", black);
            SDL_RenderCopy(gRenderer, gFontTexture, NULL, &aux);
            aux.x -= 4; aux.y -= 4;
            renderText("30", color);
            SDL_RenderCopy(gRenderer, gFontTexture, NULL, &aux);

            // CR Atual
            aux.x = 35; aux.y = 54; aux.w = 47*0.75; aux.h = 30;
            itoa(a, qtd_deck(*deck));
            renderText(a, black);
            SDL_RenderCopy(gRenderer, gFontTexture, NULL, &aux);
            aux.x -= 4; aux.y -= 4;
            renderText(a, color);
            SDL_RenderCopy(gRenderer, gFontTexture, NULL, &aux);

            // HP
            renderText("HP", black);
            SDL_Rect hp = {156, 19, 47, 33};
            SDL_RenderCopy(gRenderer, gFontTexture, NULL, &hp);
            hp.x -= 4; hp.y -= 4;
            renderText("HP", color);
            SDL_RenderCopy(gRenderer, gFontTexture, NULL, &hp);

            // HP MAX
            aux.x = 102; aux.y = 21; aux.w = 47*0.75; aux.h = 30;
            renderText("250", black);
            SDL_RenderCopy(gRenderer, gFontTexture, NULL, &aux);
            aux.x -= 4; aux.y -= 4;
            renderText("250", color);
            SDL_RenderCopy(gRenderer, gFontTexture, NULL, &aux);

            // HP Atual
            aux.x = 35; aux.y = 21; aux.w = 47*0.75; aux.h = 30;
            itoa(a, player->hp);
            renderText(a, black);
            SDL_RenderCopy(gRenderer, gFontTexture, NULL, &aux);
            aux.x -= 4; aux.y -= 4;
            renderText(a, color);
            SDL_RenderCopy(gRenderer, gFontTexture, NULL, &aux);

            // CR
            SDL_Rect deckInfo = {156, 52, 47, 33};
            renderText("CR", black);
            SDL_RenderCopy(gRenderer, gFontTexture, NULL, &deckInfo);
            deckInfo.x -= 4; deckInfo.y -= 4;
            renderText("CR", color);
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

        renderText(carta_db[idcarta].carta_info.nome_carta, color);
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
                renderText(carta_db[idcarta].carta_info.nome_carta, black);
                SDL_RenderCopy(gRenderer, gFontTexture, NULL, &texthover);
                texthover.x -= 4; texthover.y -= 4;
                renderText(carta_db[idcarta].carta_info.nome_carta, color);
                SDL_RenderCopy(gRenderer, gFontTexture, NULL, &texthover);
                texthover.x += 4; texthover.y += texthover.h + 4;
                TTF_SizeUTF8(gFont, carta_db[idcarta].carta_info.elemento_carta, &(texthover.w), &(texthover.h));
                renderText(carta_db[idcarta].carta_info.elemento_carta, black);
                SDL_RenderCopy(gRenderer, gFontTexture, NULL, &texthover);
                texthover.x -= 4; texthover.y -= 4;
                renderText(carta_db[idcarta].carta_info.elemento_carta, color);
                SDL_RenderCopy(gRenderer, gFontTexture, NULL, &texthover);
                texthover.x += 4; texthover.y += texthover.h + 4;
                texthover.w = mousehover.w - 30;
                texthover.h *= 2;
                renderTextWrapped(carta_db[idcarta].carta_info.descricao, black);
                SDL_RenderCopy(gRenderer, gFontTexture, NULL, &texthover);
                texthover.x -= 4; texthover.y -= 4;
                renderTextWrapped(carta_db[idcarta].carta_info.descricao, color);
                SDL_RenderCopy(gRenderer, gFontTexture, NULL, &texthover);
            }
        }
    }
}

// void renderData(Player *player, SDL_Event *e, Deck *deck, Mao *mao, SDL_Rect *enemyVP, SDL_Rect *cardAreaVP, InimigoBatalha inimigos, int numeroInimigos, Seletor *select){

// }
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

int battleOn(Player *player, SDL_Event *e, int *quit){
    int idcarta, battle = 1, idinimigo[3], numeroInimigos, inimigosTurno;
    int width, x, y, k, i, critical = 0,  roll;
    int escudoCount = 0, prevCount = 0, primeiroTurno = 1, pause = 1;
    double fatorAleatorio, dano = 0;
    char msg[100];
    Deck *deck = NULL;
    Mao *mao = NULL;
    InimigoBatalha *inimigos = NULL;
    Animation animate = {0, 0, 0, 0, {0,0,0,0}, {0,0,0,0}};
    Seletor select = {1, 1, -1, -1, 0, 0};
    SDL_Rect numeroSize;

    battleInit(&deck, &mao);
    player->turno = 1;
    
    // GERADOR INIMIGOS
    numeroInimigos = rand() % 3;
    inimigos = (InimigoBatalha*) malloc(sizeof(InimigoBatalha) * (numeroInimigos + 1));
    for(i = 0; i <= numeroInimigos; i++){
        inimigos[i].id = rand() % 5;
        inimigos[i].hp = inimigo_db[inimigos[i].id].HP;
        inimigos[i].buff = 0;
        inimigos[i].buffCount = 0;
        inimigos[i].bolado = 0;
        inimigos[i].turno = 0;
        inimigos[i].derrotado = 0;
        inimigos[i].alpha = 0;
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
                battle = 0;
                *quit = 1;
            }
            else if(e->type == SDL_KEYDOWN){
                switch(e->key.keysym.sym){
                    case SDLK_ESCAPE:
                    battle = 0; *quit = 1;
                    break;
                    case SDLK_RETURN:
                    battle = 0;
                    break;
                }
                if(!battle)
                    break;
            }
        }
        if(!battle)
            break;

        
        if(primeiroTurno){
            // Animacao monstros surgindo
            renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
            SDL_RenderPresent(gRenderer);
            while(inimigos[0].alpha < 254){
                for(i = 0; i <= numeroInimigos;i++)
                    inimigos[i].alpha += 2;
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
            renderBattleMessage(msg, &msgAreaTopVP);
            SDL_RenderPresent(gRenderer);
            SDL_Delay(2000);

            // Compra cartas iniciais
            for(i = 0; i < 5; i++){
                compra_carta(deck, mao);
                renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                SDL_RenderPresent(gRenderer);
                SDL_Delay(50);
            }

            // Mensagem #2: Inicio da batalha
            strcpy(msg, "É hora do duelo!");
            renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
            renderBattleMessage(msg, &msgAreaTopVP);
            SDL_RenderPresent(gRenderer);
            SDL_Delay(2000);

            primeiroTurno = 0;
        }
        // else if(deck_vazio(deck)){

        // }
        // Compra carta a cada rodada
        else if(!mao_cheia(mao)){
            compra_carta(deck, mao);
            renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
            SDL_RenderPresent(gRenderer);
            SDL_Delay(50);
        }

        // Mensagem #3: Turno do jogador
        player->turno = 1;
        strcpy(msg, "Sua vez!");
        renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
        renderBattleMessage(msg, &msgAreaTopVP);
        SDL_RenderPresent(gRenderer);
        select.ativoc = 0;
        select.ativoe = 0;
        while(pause){
            SDL_WaitEvent(e);
            if(e->type == SDL_MOUSEBUTTONDOWN)
                break;
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
                            battle = 0; *quit = 1; player->turno = 0;
                            break;
                            case SDLK_RETURN:
                            battle = 0;
                            break;
                        }
                        if(!battle)
                            break;
                    }
                else if(e->type == SDL_MOUSEBUTTONDOWN){
                    // if(cartaEscolhida(player, e, &deck, &mao, &idcarta, &cardAreaVP, &select)){}
                    for(i = 0; i < qtd_mao(mao); i++){
                        if(mouseOverClickCard(&cardAreaVP[i], e)){
                            select.indexc = i;
                            consulta_mao(mao, select.indexc, &idcarta);
                            if(strcmp(carta_db[idcarta].carta_info.efeito, "Ataque") == 0){
                                select.ativoc = 1;
                                while(select.ativoc){
                                    renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                                    SDL_RenderPresent(gRenderer);
                                    while(SDL_PollEvent(e) != 0){
                                        if(e->type == SDL_QUIT){
                                            battle = 0;
                                            *quit = 1;
                                            select.ativoc = 0;
                                            select.indexc = -1;
                                            player->turno = 0;
                                        }
                                        else if(e->type == SDL_KEYDOWN){
                                                switch(e->key.keysym.sym){
                                                    case SDLK_ESCAPE:
                                                    battle = 0; *quit = 1; player->turno = 0;
                                                    break;
                                                    case SDLK_RETURN:
                                                    battle = 0;
                                                    break;
                                                }
                                                if(!battle)
                                                    break;
                                        }
                                        else if(e->type == SDL_MOUSEBUTTONDOWN){
                                            if(e->button.button == SDL_BUTTON_RIGHT){
                                                select.ativoc = 0;
                                                select.indexc = -1;
                                                break;
                                            }
                                            for(k = 0; k <= numeroInimigos; k++){
                                                if(!inimigos[k].derrotado){
                                                    if(mouseOverClickEnemy(&enemyVP[k], e)){
                                                        select.indexe = k;
                                                        select.ativoe = 1;
                                                        renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                                                        SDL_RenderPresent(gRenderer);

                                                        // Calculo de dano
                                                        fatorAleatorio = SDL_floor((rand() % (carta_db[idcarta].carta_info.range + 1)) + carta_db[idcarta].carta_info.rangemin);
                                                        dano = SDL_floor(fatorAleatorio / 100 * player->atk * carta_db[idcarta].carta_info.multiplicador_carta/100);
                                                        if(fatorAleatorio >= carta_db[idcarta].carta_info.rangemin + carta_db[idcarta].carta_info.range)
                                                            critical = 1;
                                                        if(strcmp(carta_db[idcarta].carta_info.elemento_carta, "Fogo") == 0)
                                                            dano = SDL_floor(dano * inimigo_db[inimigos[select.indexe].id].RElemento[0]/100);
                                                        else if(strcmp(carta_db[idcarta].carta_info.elemento_carta, "Gelo") == 0)
                                                            dano = SDL_floor(dano * inimigo_db[inimigos[select.indexe].id].RElemento[1]/100);
                                                        else if(strcmp(carta_db[idcarta].carta_info.elemento_carta, "Elec") == 0)
                                                            dano = SDL_floor(dano * inimigo_db[inimigos[select.indexe].id].RElemento[2]/100);
                                                        dano = SDL_floor(dano - inimigo_db[inimigos[select.indexe].id].DEF);
                                                            if(dano > 999)
                                                            dano = 999;
                                                        if(dano <= 0)
                                                            dano = 1;
                                                        select.dano = dano;

                                                        /* animacao efeito */

                                                        // Animacao Dano
                                                        numeroSize.x = enemyVP[k].x + enemyVP[k].w / 2 - 47;
                                                        numeroSize.y = enemyVP[k].y + enemyVP[k].h / 2 - 33;
                                                        numeroSize.w = 94; numeroSize.h = 66;
                                                        animate.alpha = 255;
                                                        while(animate.alpha > 0){
                                                            renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                                                            SDL_RenderSetViewport(gRenderer, NULL);
                                                            if(critical){
                                                                TTF_SetFontOutline(gFont, 10);
                                                                strcpy(msg, "CRITICAL!");
                                                                renderText(msg, black);
                                                                SDL_SetTextureAlphaMod(gFontTexture, animate.alpha);
                                                                numeroSize.y -= 30;
                                                                numeroSize.x -= 15;
                                                                numeroSize.w += 30;
                                                                numeroSize.h -= 20;
                                                                SDL_RenderCopy(gRenderer, gFontTexture, NULL, &numeroSize);
                                                                TTF_SetFontOutline(gFont, 0);
                                                                renderText(msg, yellow);
                                                                SDL_SetTextureAlphaMod(gFontTexture, animate.alpha);
                                                                SDL_RenderCopy(gRenderer, gFontTexture, NULL, &numeroSize);
                                                                SDL_RenderPresent(gRenderer);
                                                                numeroSize.y += 30;
                                                                numeroSize.x += 15;
                                                                numeroSize.w -= 30;
                                                                numeroSize.h += 20;

                                                            }
                                                            itoa(msg, dano);
                                                            TTF_SetFontOutline(gFont, 2);
                                                            renderText(msg, black);
                                                            SDL_SetTextureAlphaMod(gFontTexture, animate.alpha);
                                                            SDL_RenderCopy(gRenderer, gFontTexture, NULL, &numeroSize);
                                                            TTF_SetFontOutline(gFont, 0);
                                                            renderText(msg, color);
                                                            SDL_SetTextureAlphaMod(gFontTexture, animate.alpha);
                                                            SDL_RenderCopy(gRenderer, gFontTexture, NULL, &numeroSize);
                                                            SDL_RenderPresent(gRenderer);
                                                            numeroSize.y -= 3;
                                                            animate.alpha -= 4;
                                                        }
                                                        critical = 0;
                                                        inimigos[k].hp -= dano;
                                                        if(inimigos[k].hp <= 0){
                                                            inimigos[k].derrotado = 1;

                                                            inimigos[k].alpha = 255;
                                                            while(inimigos[k].alpha > 0){
                                                                inimigos[k].alpha -= 2;
                                                                renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                                                                SDL_RenderPresent(gRenderer);
                                                            }
                                                        }
                                                        select.ativoc = 0;
                                                        select.ativoe = 0;
                                                        descarta_carta(mao, select.indexc);
                                                        // select.indexc = -1;
                                                        player->turno = 0;
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            else if(strcmp(carta_db[idcarta].carta_info.efeito, "Cura") == 0){
                                fatorAleatorio = SDL_floor((rand() % (carta_db[idcarta].carta_info.range + 1)) + carta_db[idcarta].carta_info.rangemin);
                                dano = SDL_floor(fatorAleatorio/100 * player->atk * carta_db[idcarta].carta_info.multiplicador_carta/100);
                                if(dano > 999)
                                    dano = 999;
                                if(dano <= 0)
                                    dano = 1;

                                select.ativoc = 1;
                                player->hp += dano;
                                if(player->hp > player->hpmax)
                                    player->hp = player->hpmax;
                                renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                                SDL_RenderPresent(gRenderer);
                                
                                itoa(msg, dano);
                                numeroSize.x = 31; numeroSize.y = 600; numeroSize.w = 94; numeroSize.h = 66;
                                animate.alpha = 255;
                                while(animate.alpha > 0){
                                    renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                                    SDL_RenderSetViewport(gRenderer, NULL);
                                    TTF_SetFontOutline(gFont, 2);
                                    renderText(msg, black);
                                    SDL_SetTextureAlphaMod(gFontTexture, animate.alpha);
                                    SDL_RenderCopy(gRenderer, gFontTexture, NULL, &numeroSize);
                                    TTF_SetFontOutline(gFont, 0);
                                    renderText(msg, green);
                                    SDL_SetTextureAlphaMod(gFontTexture, animate.alpha);
                                    SDL_RenderCopy(gRenderer, gFontTexture, NULL, &numeroSize);
                                    SDL_RenderPresent(gRenderer);
                                    numeroSize.y -= 3;
                                    animate.alpha -= 4;
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

                                compra_carta(deck, mao);
                                renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                                SDL_RenderPresent(gRenderer);
                                if(!mao_cheia(mao)){
                                    compra_carta(deck, mao);
                                    renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                                    SDL_RenderPresent(gRenderer);
                                }

                                player->turno = 0;
                            }
                            SDL_Delay(100);
                        }
                    }
                    if(!player->turno)
                        break;
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
        // while(inimigosTurno){
        //     for(i = 0; i <= numeroInimigos; i++){
        //         inimigos[i].turno = 1;
        //         while(inimigos[i].turno && !inimigos[i].derrotado){
        //             while(SDL_PollEvent(e) != 0){
        //                 if(e->type == SDL_QUIT){
        //                     *quit = 1;
        //                     battle = 1;
        //                 }
        //             }
        //             strcpy(msg, "Vez do ");
        //             strcat(msg, inimigo_db[inimigos[i].id].nome);
        //             strcat(msg, "!");
        //             renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
        //             renderBattleMessage(msg, &msgAreaTopVP);
        //             SDL_RenderPresent(gRenderer);
        //             SDL_Delay(2000);

        //             roll = rand() % 100 + 1;
        //             for(k = 0; k < 5; k++){
                    //     if (inimigo_db[inimigos[i].id].chance[k].min > roll && inimigo_db[inimigos[i].id].chance[k].max <= roll)
                    //         roll = k;
                    // }
                    // switch(roll){
                    //     case ATTACK:
                    //         if(select.escudo){
                    //             strcpy(msg, inimigo_db[inimigos[i].id].nome);
                    //             strcat(msg, " tentou atacar, mas Escudo Mágico te protegeu!");
                    //             renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                    //             renderBattleMessage(msg, &msgAreaTopVP);
                    //             SDL_RenderPresent(gRenderer);
                    //             while(pause){
                    //                 SDL_WaitEvent(e);
                    //                 if(e->type == SDL_MOUSEBUTTONDOWN)
                    //                     break;
                    //             }
                    //             inimigos[i].turno = 0;
                    //             // renderMessageWrapped(msg, &msgAreaTopVP);
                    //             break;
                    //         }
                    //         fatorAleatorio = (rand() % 100 + 50)/100;
                    //         dano = fatorAleatorio * inimigo_db[inimigos[i].id].ATK * inimigo_db[inimigos[i].id].comportamento.mult_atk;

                    //         itoa(msg, dano);
                    //         numeroSize.x = 31; numeroSize.y = 600; numeroSize.w = 94; numeroSize.h = 66;
                    //         numeroSize.w = 94; numeroSize.h = 66;
                    //         animate.alpha = 255;
                    //         while(animate.alpha > 0){
                    //             renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                    //             SDL_RenderSetViewport(gRenderer, NULL);
                    //             if(critical){
                    //                 TTF_SetFontOutline(gFont, 10);
                    //                 strcpy(msg, "CRITICAL!");
                    //                 renderText(msg, black);
                    //                 SDL_SetTextureAlphaMod(gFontTexture, animate.alpha);
                    //                 numeroSize.y -= 30;
                    //                 numeroSize.x -= 15;
                    //                 numeroSize.w += 30;
                    //                 numeroSize.h -= 20;
                    //                 SDL_RenderCopy(gRenderer, gFontTexture, NULL, &numeroSize);
                    //                 TTF_SetFontOutline(gFont, 0);
                    //                 renderText(msg, yellow);
                    //                 SDL_SetTextureAlphaMod(gFontTexture, animate.alpha);
                    //                 SDL_RenderCopy(gRenderer, gFontTexture, NULL, &numeroSize);
                    //                 SDL_RenderPresent(gRenderer);
                    //                 numeroSize.y += 30;
                    //                 numeroSize.x += 15;
                    //                 numeroSize.w -= 30;
                    //                 numeroSize.h += 20;

                    //             }
                    //             itoa(msg, dano);
                    //             TTF_SetFontOutline(gFont, 2);
                    //             renderText(msg, black);
                    //             SDL_SetTextureAlphaMod(gFontTexture, animate.alpha);
                    //             SDL_RenderCopy(gRenderer, gFontTexture, NULL, &numeroSize);
                    //             TTF_SetFontOutline(gFont, 0);
                    //             renderText(msg, red);
                    //             SDL_SetTextureAlphaMod(gFontTexture, animate.alpha);
                    //             SDL_RenderCopy(gRenderer, gFontTexture, NULL, &numeroSize);
                    //             SDL_RenderPresent(gRenderer);
                    //             numeroSize.y -= 3;
                    //             animate.alpha -= 4;
                    //         }
                    //         critical = 0;
                    //         player->hp -= (dano - player->def);
                    //         if(player->hp < 0)
                    //             player->hp = 0;
                    //         if(player->hp == 0){
                    //             battle = 1;
                    //             *quit = 1;
                    //             player->gameOver = 1;
                    //         }
                    //     inimigos[i].turno = 0;
                    //     break;

                    //     case MUG:
                    //         if(select.prev){
                    //             strcpy(msg, inimigo_db[inimigos[i].id].nome);
                    //             strcat(msg, " tentou te roubar, mas Punho Firme te protegeu!");
                    //             renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                    //             renderBattleMessage(msg, &msgAreaTopVP);
                    //             SDL_RenderPresent(gRenderer);
                    //             while(pause){
                    //                 SDL_WaitEvent(e);
                    //                 if(e->type == SDL_MOUSEBUTTONDOWN)
                    //                     break;
                    //             }
                    //             inimigos[i].turno = 0;
                    //             break;
                    //         }
                    //         k = inimigo_db[inimigos[i].id].comportamento.discard_num;
                    //         strcpy(msg, inimigo_db[inimigos[i].id].nome);
                    //         strcat(msg, " te roubou!");
                    //         renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                    //         renderBattleMessage(msg, &msgAreaTopVP);
                    //         SDL_RenderPresent(gRenderer);
                    //         SDL_Delay(2000);
                    //         while(k > 0){
                    //             if(mao_vazia(mao)){
                    //                 strcpy(msg, "Sua mao está vazia!!");
                    //                 renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                    //                 renderBattleMessage(msg, &msgAreaTopVP);
                    //                 SDL_RenderPresent(gRenderer);
                    //                 SDL_Delay(2000);
                    //                 break;
                    //             }
                    //             else{
                    //                 roll = rand() % qtd_mao(mao);
                    //                 consulta_mao(mao, roll, idcarta);
                    //                 descarta_carta(mao, roll);
                    //                 strcpy(msg, carta_db[idcarta].carta_info.nome_carta);
                    //                 strcat(msg, " foi perdida!");
                    //                 renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                    //                 renderBattleMessage(msg, &msgAreaTopVP);
                    //                 SDL_RenderPresent(gRenderer);
                    //                 SDL_Delay(2000;)
                    //             }
                    //             k--;
                    //         }
                    //     inimigos[i].turno = 0;
                    //     break;

                    //     case FLEE:
                    //         roll = rand() % 100 + 1;
                    //         if(roll <= inimigo_db[inimigos[i].id].comportamento.flee_chance){
                    //             strcpy(msg, inimigo_db[inimigos[i].id].nome);
                    //             strcat(msg, " fugiu!");
                    //             inimigos[i].alpha = 255;
                    //             while(inimigos[i].alpha > 0){
                    //                 inimigos[i].alpha -= 3;
                    //                 renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                    //                 renderBattleMessage(msg, &msgAreaTopVP);
                    //                 SDL_RenderPresent(gRenderer);
                    //             }
                    //             inimigos[i].derrotado = 1;
                    //         }
                    //         else{
                    //             strcpy(msg, inimigo_db[inimigos[i].id].nome);
                    //             strcat(msg, " está tentando fugir...");
                    //         }
                    //         renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                    //         renderBattleMessage(msg, &msgAreaTopVP);
                    //         SDL_RenderPresent(gRenderer);
                    //         SDL_Delay(2000);
                    //     inimigos[i].turno = 0;
                    //     break;

                    //     case SPECIAL_MOVE:
                    //         if(select.escudo){
                    //             strcpy(msg, inimigo_db[inimigos[i].id].nome);
                    //             strcat(msg, " tentou atacar, mas Escudo Mágico te protegeu!");
                    //             renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                    //             renderBattleMessage(msg, &msgAreaTopVP);
                    //             SDL_RenderPresent(gRenderer);
                    //             SDL_Delay(2000);
                    //             // renderMessageWrapped(msg, &msgAreaTopVP);
                    //             inimigos[i].turno = 0;
                    //             break;
                    //         }

                    //         strcpy(msg, inimigo_db[inimigos[i].id].nome);
                    //         strcat(msg, " irá atacar!!");
                    //         renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                    //         renderBattleMessage(msg, &msgAreaTopVP);
                    //         SDL_RenderPresent(gRenderer);
                    //         SDL_Delay(2000);
                    //         strcpy(msg, inimigo_db[inimigos[i].id].comportamento.nome_special);
                    //         renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                    //         renderBattleMessage(msg, &msgAreaTopVP);
                    //         SDL_RenderPresent(gRenderer);
                    //         SDL_Delay(2000);
                    //         fatorAleatorio = (rand() % 100 + 50)/100;
                    //         dano = fatorAleatorio * inimigo_db[inimigos[i].id].ATK * inimigo_db[inimigos[i].id].comportamento.mult_atk * inimigo_db[inimigos[i].id].comportamento.mult_special;

                    //         itoa(msg, dano);
                    //         numeroSize.x = 31; numeroSize.y = 600; numeroSize.w = 94; numeroSize.h = 66;
                    //         numeroSize.w = 94; numeroSize.h = 66;
                    //         animate.alpha = 255;
                    //         while(animate.alpha > 0){
                    //             renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                    //             SDL_RenderSetViewport(gRenderer, NULL);
                    //             if(critical){
                    //                 TTF_SetFontOutline(gFont, 10);
                    //                 strcpy(msg, "CRITICAL!");
                    //                 renderText(msg, black);
                    //                 SDL_SetTextureAlphaMod(gFontTexture, animate.alpha);
                    //                 numeroSize.y -= 30;
                    //                 numeroSize.x -= 15;
                    //                 numeroSize.w += 30;
                    //                 numeroSize.h -= 20;
                    //                 SDL_RenderCopy(gRenderer, gFontTexture, NULL, &numeroSize);
                    //                 TTF_SetFontOutline(gFont, 0);
                    //                 renderText(msg, yellow);
                    //                 SDL_SetTextureAlphaMod(gFontTexture, animate.alpha);
                    //                 SDL_RenderCopy(gRenderer, gFontTexture, NULL, &numeroSize);
                    //                 SDL_RenderPresent(gRenderer);
                    //                 numeroSize.y += 30;
                    //                 numeroSize.x += 15;
                    //                 numeroSize.w -= 30;
                    //                 numeroSize.h += 20;

                    //             }
                    //             itoa(msg, dano);
                    //             TTF_SetFontOutline(gFont, 2);
                    //             renderText(msg, black);
                    //             SDL_SetTextureAlphaMod(gFontTexture, animate.alpha);
                    //             SDL_RenderCopy(gRenderer, gFontTexture, NULL, &numeroSize);
                    //             TTF_SetFontOutline(gFont, 0);
                    //             renderText(msg, red);
                    //             SDL_SetTextureAlphaMod(gFontTexture, animate.alpha);
                    //             SDL_RenderCopy(gRenderer, gFontTexture, NULL, &numeroSize);
                    //             SDL_RenderPresent(gRenderer);
                    //             numeroSize.y -= 3;
                    //             animate.alpha -= 4;
                    //         }
                    //         critical = 0;
                    //         player->hp -= (dano - player->def);
                    //         if(player->hp < 0)
                    //             player->hp = 0;
                            
                    //         if(player->hp == 0){
                    //             battle = 1;
                    //             *quit = 1;
                    //             player->gameOver = 1;
                    //         }
                    //     inimigos[i].turno = 0;
                    //     break;

                    //     case DEFEND:
                    //         strcpy(msg, inimigo_db[inimigos[i].id].nome);
                    //         strcat(msg, " está preparando para se defender.");
                    //         renderBattle(player, e, &deck, &mao, enemyVP, cardAreaVP, inimigos, numeroInimigos, &select);
                    //         renderBattleMessage(msg, &msgAreaTopVP);
                    //         SDL_RenderPresent(gRenderer);
                    //         SDL_Delay(2000);
                    //         inimigos[i].buff = 1;
                    //         inimigos[i].buffCount = inimigo_db[inimigos[i].id].buffdefault + 1;
                    //     inimigos[i].turno = 0;
                    //     break;
                    // }
        //             if(!battle)
        //                 break;
        //         }
        //     }
        //     inimigosTurno = 0;
        // }

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
            renderBattleMessage(msg, &msgAreaTopVP);
        }
        if(select.prev)
            prevCount++;
        if(prevCount >= 4){
            select.prev = 0;
            strcpy(msg, "Punho Firme se dissipou!");
            renderBattleMessage(msg, &msgAreaTopVP);
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
            int quit = 0, battle = 0, i = 0;
            SDL_Event e;
            Player player = {250, 250, 100, 75, 0, 0};
                
            // main loop
            while(!quit){
                while(SDL_PollEvent(&e) != 0){
                    if(e.type == SDL_QUIT)
                        quit = 1;
                }
                battle = rand() % 2;
                if(battle)
                    battle = battleOn(&player, &e, &quit);
                else
                    printf("no battle %i", i++);
                if(battle < 0)
                    break;
            }
            libera_db_carta();
            closeWindow();
            
        }
    }
    return 0;
}