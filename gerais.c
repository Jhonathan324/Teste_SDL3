#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include "gerais.h"

void atribuirFRectInRectA(SDL_FRect *fretangulo, SDL_Rect *retangulo){
    retangulo->x = fretangulo->x;
    retangulo->y = fretangulo->y;
    retangulo->h = fretangulo->h;
    retangulo->w = fretangulo->w;
}