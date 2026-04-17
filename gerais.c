#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include "gerais.h"

void atribuirFRectInRectA(SDL_FRect *fretangulo, SDL_Rect *retangulo){
    retangulo->x = fretangulo->x;
    retangulo->y = fretangulo->y;
    retangulo->h = fretangulo->h;
    retangulo->w = fretangulo->w;
}