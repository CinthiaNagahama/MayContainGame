#include <SDL2/SDL.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>

const int SCR_WID = 640;
const int SCR_HEI = 480;

enum KeyPressSurfaces{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_ACCEPT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_ACCEPT,
    KEY_PRESS_SURFACE_BACK,
    KEY_PRESS_SURFACE_TOTAL
};

// window
SDL_Window *gWindow = NULL;
// window surface
SDL_Surface *gSurface = NULL;
// window surface
SDL_Surface *gEnemySurface = NULL;
// keypress
SDL_Surface *gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];
// imagem atual
SDL_Surface *gCurrentSurface = NULL;
// imagem atual scale
SDL_Surface* gStretchedSurface = NULL;

int init(){
    int success = 1;
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL could not be initialized. SDL_Error %s\n", SDL_GetError());
        success = 0;
    }
    else{
        gWindow = SDL_CreateWindow("MayCointainGame", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCR_WID, SCR_HEI, SDL_WINDOW_SHOWN);
        if(gWindow == NULL){
            printf("window could not be created!! SDL_ERROR: %s\n", SDL_GetError());
            success = 0;
        }
        else{
            int imgflags = IMG_INIT_PNG;
            if(!(IMG_Init(imgflags) & imgflags)){
                printf("SDL_Image had some trouble initializing. SDL_Image error: %s\n", IMG_GetError());
                success = 0;
            }
            else{
                gSurface = SDL_GetWindowSurface(gWindow);
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
    return optimizedSurface;
}

int loadMedia(){
    int success = 1;
    // gEnemySurface = IMG_Load("pics/gumball1.png");
    // if(gEnemySurface == NULL){
    //     printf("Unable to load image %s! SDL Error: %s\n", "pics/gumball1.png", SDL_GetError());
    //     success = 0;
    // }
    gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("pics/bg.png");
    if(gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL){
        printf("Failed to load image 1.\n");
        success = 0;
    }
    gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = loadSurface("pics/gumball1.png");
    if(gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL){
        printf("Failed to load image 2.\n");
        success = 0;
    }
    gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface("pics/gumball2.png");
    if(gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL){
        printf("Failed to load image 3.\n");
        success = 0;
    }
    gKeyPressSurfaces[KEY_PRESS_SURFACE_ACCEPT] = loadSurface("pics/tornado.png");
    if(gKeyPressSurfaces[KEY_PRESS_SURFACE_ACCEPT] == NULL){
        printf("Failed to load image 4.\n");
        success = 0;
    }
    return success;
}

void closeWindow(){
    for(int i = 0; i < KEY_PRESS_SURFACE_TOTAL; i++){
        SDL_FreeSurface(gKeyPressSurfaces[i]);
        gKeyPressSurfaces[i] = NULL;
    }
    SDL_FreeSurface(gCurrentSurface);
    gCurrentSurface = NULL;
    SDL_FreeSurface(gSurface);
    gSurface = NULL;
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* args[]) // SDL requer que main tenha estes argumentos
{
        
    if(!init())
        printf("init failed.\n");
    else{
        if(!loadMedia())
            printf("load failed.\n");
        else{
            int quit = 0;
            SDL_Event e;
            gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
            while(!quit){
                while(SDL_PollEvent(&e) != 0){
                    if(e.type == SDL_QUIT)
                        quit = 1;
                    else if(e.type == SDL_KEYDOWN){
                        switch(e.key.keysym.sym){
                            case SDLK_UP:
                            gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
                            break;

                            case SDLK_DOWN:
                            gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
                            break;

                            case SDLK_RETURN:
                            gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_ACCEPT];
                            break;

                            case SDLK_ESCAPE:
                            quit = 1;
                            break;

                            default:
                            gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
                            break;
                        }
                    }
                }
                SDL_Rect stretchRect;
                stretchRect.x = 0;
                stretchRect.y = 0;
                stretchRect.w = SCR_WID;
                stretchRect.h = SCR_HEI;
                SDL_BlitScaled(gCurrentSurface, NULL, gSurface, &stretchRect);
                // SDL_BlitSurface(gCurrentSurface, NULL, gSurface, NULL);
                SDL_UpdateWindowSurface(gWindow);
            }
        }
    }
    closeWindow();
    return 0;

}