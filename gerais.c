#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "gerais.h"
#define  tp1 16

void AtribuirFRectInRectA(SDL_FRect *fretangulo, SDL_Rect *retangulo){
    retangulo->x = fretangulo->x;
    retangulo->y = fretangulo->y;
    retangulo->h = fretangulo->h;
    retangulo->w = fretangulo->w;
}

Moldura InitMoldura(SDL_Renderer *renderer, int x, int y, int w, int h, int tamanho_canto,char *file){
    if(w < tamanho_canto*2){
        w = tamanho_canto*2;
    }
    if(h < tamanho_canto*2){
        h = tamanho_canto*2;
    }
    SDL_Texture *textura = IMG_LoadTexture(renderer, file);
    Moldura moldura = {x,y,w,h,textura};
    moldura.partes[0][0]= (SDL_FRect){x,y, tamanho_canto, tamanho_canto};
    moldura.partes[0][1]= (SDL_FRect){x+tamanho_canto,y, h-2*tamanho_canto, tamanho_canto};
    moldura.partes[0][2]= (SDL_FRect){x+w-tamanho_canto,y, tamanho_canto, tamanho_canto};
    moldura.partes[1][0]= (SDL_FRect){x,y+tamanho_canto, tamanho_canto, tamanho_canto};
    moldura.partes[1][1]= (SDL_FRect){x+tamanho_canto,y+tamanho_canto, w-2*tamanho_canto, tamanho_canto};
    moldura.partes[1][2]= (SDL_FRect){x+w-tamanho_canto,y+tamanho_canto, tamanho_canto, tamanho_canto};
    moldura.partes[2][0]= (SDL_FRect){x,y+h-tamanho_canto, tamanho_canto, tamanho_canto};
    moldura.partes[2][1]= (SDL_FRect){x+tamanho_canto,y+h-tamanho_canto, w-2*tamanho_canto, tamanho_canto};
    moldura.partes[2][2]= (SDL_FRect){x+w-tamanho_canto,y+h-tamanho_canto, tamanho_canto, tamanho_canto};
    return moldura;
}

void DesenharMoldura(SDL_Renderer *renderer, Moldura moldura){
    SDL_RenderTextureRotated(
        renderer,
        moldura.textura,
        &(SDL_FRect){tp1,tp1,tp1,tp1}, 
        &moldura.partes[0][0],
        180.0,
        NULL,
        SDL_FLIP_NONE);
    // a terminar
}