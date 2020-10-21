#ifndef MAIN_H
#define MAIN_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../src/structs.h"
#include "../Deck/deck.h"
#include "../Batalha/Inimigo.h"
#include "../src/main.h"
#include "../Batalha/Battle.h"

int mouseOver(SDL_Rect *button, SDL_Event *e);

int mouseOverClickEnemy(SDL_Rect *button, SDL_Event *e);

int mouseOverClickCard(SDL_Rect *button, SDL_Event *e);

void itoa(char *string, int x);

#endif