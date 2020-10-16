#include <SDL2/SDL.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include "../Deck/deck.h"
#include <time.h>

const int SCR_WID = 1280;
const int SCR_HEI = 750;

typedef struct player{
    int hp;
    int atk;
    int def;
} Player;

typedef struct inimigo{
    char nome[11];
    int hp;
} Inimigo;

typedef struct sprite_inimigo{
    char path[35];
    int w;
    int h;
} SpriteInimigo;

enum LButtonSprite{
    BUTTON_MOUSE_OUT,
    BUTTON_MOUSE_OVER,
    BUTTON_MOUSE_DOWN,
    BUTTON_MOUSE_UP,
    BUTTON_TOTAL
};

enum KeyPressSurfaces{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_ACCEPT,
    KEY_PRESS_SURFACE_BACK,
    KEY_PRESS_SURFACE_TOTAL
};

enum Inimigos{
    PINGUIM,
    PINGUIM_BOLADO,
    GOBLIN,
    LADRAO,
    ABU,
    INIMIGO_TOTAL
};

SpriteInimigo gInimigoPath[5];
int loadPaths(){
    strcpy(gInimigoPath[0].path, "Sprites/Inimigos/gumball1.png");
    gInimigoPath[0].w = 200;
    gInimigoPath[0].h = 234;
    strcpy(gInimigoPath[1].path, "Sprites/Inimigos/gumball2.png");
    gInimigoPath[1].w = 200;
    gInimigoPath[1].h = 234;
    strcpy(gInimigoPath[2].path, "Sprites/Inimigos/Goblin.png");
    strcpy(gInimigoPath[3].path, "Sprites/Inimigos/Ladrao.png");
    strcpy(gInimigoPath[4].path, "Sprites/Inimigos/Abu.png");
    return 1;
}
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
SDL_Texture **gCardTexture = NULL;
SDL_Texture *gModTexture = NULL;
// Fonte
TTF_Font *gFont = NULL;
// rendered texture
SDL_Texture *gFontTexture = NULL;;

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
                gEnemyTexture = (SDL_Texture**) malloc(sizeof(SDL_Texture*) * 3);
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
        printf("text failed.\n");
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
    gBoxTexture = loadTexture("Sprites/Misc/textbox1.png");
    if(gBoxTexture == NULL){
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
    return success;
}

int loadMediaCurrent(int *enemy, int numeroInimigos)
{
    int success = 1, i;
    // if(gEnemyTexture != NULL){
    //     for(i = 0; i < 3; i++){
    //         if(gEnemyTexture[i] != NULL)
    //             SDL_DestroyTexture(gEnemyTexture[i]);
    //     }
    // }
    for(i = 0; i <= numeroInimigos; i++){
        gEnemyTexture[i] = loadTexture(gInimigoPath[enemy[i]].path);
        if(gEnemyTexture[i] == NULL){
            printf("EnemyTexture fail. \n");
            success = 0;
        }
    }
    return success;
}

void closeWindow(){
    TTF_CloseFont(gFont);
    gFont = NULL;
    for(int i = 0; i < 16;i++){
        SDL_DestroyTexture(gCardTexture[i]);
    }
    gCardTexture = NULL;
    SDL_DestroyTexture(gEnemyTexture);
    gEnemyTexture = NULL;
    SDL_DestroyTexture(gBoxTexture);
    gBoxTexture = NULL;
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

void renderBattle(Player *player, SDL_Event *e, int *j, Deck **deck, Mao **mao, SDL_Rect *enemyVP, SDL_Rect *cardAreaVP, int* idinimigo, int numeroInimigos, Inimigo *enemy){
    int idcarta = 15, i = 0, x = 0, y = 0;
    char a[5];
    const SDL_Rect battleBGVP = {0, 0, SCR_WID, 3 * SCR_HEI / 4};
    const SDL_Rect menuVP = {0, 3 * SCR_HEI / 4, SCR_WID, SCR_HEI/4};
    const SDL_Rect statsVP = {0, 3 * SCR_HEI / 4, SCR_WID/6, SCR_HEI/4};
    const SDL_Rect stockVP = {SCR_WID / 6 + 35, 3 * SCR_HEI / 4, 5 * SCR_WID/6 - 10, SCR_HEI/4};
    const SDL_Rect enemyAreaVP = {120, 30, 1040, 3 * SCR_HEI / 4 - 30};
    SDL_Rect cardText[5];
    SDL_Rect mousehover;
    SDL_Rect texthover;
    SDL_Color color = {200,200,200};
    SDL_Color black = {0,0,0};

    // limpa
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    SDL_RenderClear(gRenderer);
    
    // Battle Background
    SDL_RenderSetViewport(gRenderer, &battleBGVP);
    SDL_RenderCopy(gRenderer, gBGTexture, NULL, NULL);

    // Enemy Area
    SDL_RenderSetViewport(gRenderer, &enemyAreaVP);
    SDL_SetTextureAlphaMod(gEnemyTexture, *j);
        
        // Inimigo
        for(i = 0; i <= numeroInimigos; i++){
            if(mouseOver(&enemyVP[i], e))
                enemyVP[i].y = 3*(3 * SCR_HEI / 4 - 30)/4 - enemyVP[i].h - 15;
            else
                enemyVP[i].y = 3*(3 * SCR_HEI / 4 - 30)/4 - enemyVP[i].h;
            if(idinimigo[i] == 0){
                SDL_Rect cutGB = {114, 15, gInimigoPath[0].w, gInimigoPath[0].h};
                SDL_RenderCopy(gRenderer, gEnemyTexture[i], &cutGB, &enemyVP[i]);
            }
            else{
                SDL_RenderCopy(gRenderer, gEnemyTexture[i], NULL, &enemyVP[i]);
            }
        }

        // hover effect
        for(i = 0; i <= numeroInimigos; i++){
        if(mouseOver(&enemyVP[i], e)){
            SDL_GetMouseState(&x, &y);
            SDL_SetTextureAlphaMod(gBoxTexture, 0);
            mousehover.h = 65;
            mousehover.y = y - mousehover.h;
            mousehover.w = 150;
            if(x + mousehover.w >= 1280)
                mousehover.x = x - mousehover.w;
            else
                mousehover.x = x;
            texthover.x = 15;
            texthover.y = 15;
            TTF_SizeUTF8(gFont, enemy[i].nome, &(mousehover.w), &(mousehover.h));
            mousehover.w += 30; mousehover.h += 30;
            SDL_RenderSetViewport(gRenderer, &mousehover);
            SDL_RenderCopy(gRenderer, gBoxTexture, NULL, NULL);
            TTF_SizeUTF8(gFont, enemy[i].nome, &(texthover.w), &(texthover.h));
            renderText(enemy[i].nome, color);
            SDL_RenderCopy(gRenderer, gFontTexture, NULL, &texthover);
        }
    }

    // Menu
    SDL_RenderSetViewport(gRenderer, &menuVP);
    SDL_RenderCopy(gRenderer, gBoxTexture, NULL, NULL);

        // Stats
        SDL_RenderSetViewport(gRenderer, &statsVP);
        SDL_RenderCopy(gRenderer, gBoxTexture, NULL, NULL);
                  
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

    gCurrentSurface = SDL_GetWindowSurface(gWindow);
    // Cartas
    SDL_RenderSetViewport(gRenderer, &stockVP);
    for (i = 0; i < qtd_mao(*mao); i++){
        if(mouseOver(&(cardAreaVP[i]), e))
            cardAreaVP[i].y = 0;
        else
            cardAreaVP[i].y = 15;
        cardText[i].x = cardAreaVP[i].x + 10;
        cardText[i].y = cardAreaVP[i].y + 3 * cardAreaVP[0].h/4 + 5;
        cardText[i].w = cardAreaVP[i].w - 20;
        cardText[i].h = 20;
        consulta_mao(*mao, i, &idcarta);
        SDL_RenderCopy(gRenderer, gCardTexture[idcarta], NULL, &cardAreaVP[i]);
        renderText(carta_db[idcarta].carta_info.nome_carta, color);
        SDL_RenderCopy(gRenderer, gFontTexture, NULL, &cardText[i]);
    }

    // mouse hover effect
    for (i = 0; i < qtd_mao(*mao); i++){
        if(mouseOver(&cardAreaVP[i], e)){
            SDL_GetMouseState(&x, &y);
            SDL_SetTextureAlphaMod(gBoxTexture, 10);
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
            SDL_RenderCopy(gRenderer, gBoxTexture, NULL, NULL);
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
    
    SDL_RenderPresent(gRenderer);
    if(*j < 254) *j+=2;
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

int battleOn(Player *player, SDL_Event *e, int *quit){
    int idcarta, battle = 1, idinimigo[3], i, numeroInimigos;
    int width;
    int escudo = 0, prev = 0, escudoCount = 0, prevCount = 0, primeiroTurno = 1;
    Deck *deck = NULL;
    Mao *mao = NULL;
    battleInit(&deck, &mao);

    // alterar com o db de inimigo
    Inimigo enemy[3];
    strcpy(enemy[0].nome, "Gumball");
    strcpy(enemy[1].nome, "GB_Susto");
    strcpy(enemy[2].nome, "what");
    enemy[0].hp = 200;
    enemy[1].hp = 100;
    enemy[2].hp = 150;

    numeroInimigos = rand() % 3;
    for(i = 0; i <= numeroInimigos; i++)
        idinimigo[i] = rand() % 2;
    
    if(!loadMediaCurrent(&idinimigo, numeroInimigos)){
        printf("load2 failed.\n");
        return -1;
    }
    width = 1040 / (numeroInimigos + 2);
    SDL_Rect enemyVP[3];
    for(i = 0;i <= numeroInimigos; i++){
        enemyVP[i].w = gInimigoPath[idinimigo[i]].w;
        enemyVP[i].h = gInimigoPath[idinimigo[i]].h;
        enemyVP[i].x = (width * (i + 1) - enemyVP[i].w / 2);
        enemyVP[i].y = 3*(3 * SCR_HEI / 4 - 30)/4 - enemyVP[i].h;
    }
    SDL_Rect cardAreaVP[5];
    for(i = 0; i < 5;i++){
        cardAreaVP[i].x = i * (213 - 15);
        cardAreaVP[i].y = 15;
        cardAreaVP[i].w = 213 - 30;
        cardAreaVP[i].h = SCR_HEI/4 - 35;
    }
    int j = 0;
    int dano = 0;
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
            }
        }
        renderBattle(player, e, &j, &deck, &mao, &enemyVP, &cardAreaVP, &idinimigo, numeroInimigos, &enemy);
        if(primeiroTurno){
            for(i = 0; i < 5; i++){
                compra_carta(deck, mao);
                SDL_Delay(50);
                renderBattle(player, e, &j, &deck, &mao, &enemyVP, &cardAreaVP, &idinimigo, numeroInimigos, &enemy);
            }
            primeiroTurno = 0;
        }
        // else if(deck_vazio(deck)){

        // }
        else if(!mao_cheia(mao)){
            compra_carta(deck, mao);
        }
        SDL_WaitEvent(e);
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
            }
        if(e->type == SDL_MOUSEBUTTONDOWN){
            for(i = 0; i < qtd_mao(mao); i++){
                if(mouseOver(&(cardAreaVP[i]), e) && e->button.button == SDL_BUTTON_LEFT){
                    usa_carta(mao, i, &idcarta);
                    if(strcmp(carta_db[idcarta].carta_info.efeito, "Ataque") == 0){
                        dano = 10 * carta_db[idcarta].carta_info.multiplicador_carta;
                        gModTexture = SDL_CreateTextureFromSurface(gRenderer, gCurrentSurface);
                        SDL_RenderCopy(gRenderer, gModTexture, NULL, NULL);
                        SDL_Delay(1000);
                    }
                    else if(strcmp(carta_db[idcarta].carta_info.efeito, "Cura") == 0)
                        dano = 10 * carta_db[idcarta].carta_info.multiplicador_carta;
                    else if(strcmp(carta_db[idcarta].carta_info.efeito, "Escudo") == 0){
                        escudo = 1;
                        escudoCount = -1;
                    }
                    else if(strcmp(carta_db[idcarta].carta_info.efeito, "Prev") == 0){
                        prev = 1;
                        prevCount = -1;
                    }
                    else if(strcmp(carta_db[idcarta].carta_info.efeito, "Saque") == 0){
                        compra_carta(deck, mao);
                        compra_carta(deck, mao);
                    }
                }
            }
        }
        if(escudo)
            escudoCount++;
        if(escudoCount <= 2)
            escudo = 0;
        if(prev)
            prevCount++;
        if(prevCount <= 4)
            prev = 0;
        // if(!(enemy[idinimigo[i]].hp > 0))
        //     battle = 0;
    }
    battleEnd(&deck, &mao);
    return 0;
}

int main(int argc, char* args[]) // SDL requer que main tenha estes argumentos
{
    if(!init())
        printf("init failed.\n");
    else{
        cria_db_carta();
        loadPaths();
        if(!loadMediaBasic())
            printf("load failed.\n");
        else{
            int quit = 0, battle = 0, i = 0;
            SDL_Event e;
            Player player = {250, 100, 75};

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