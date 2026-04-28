#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "gerais.h"
#define  EscalaMoldura 16

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
    moldura.partes[0][0]= (SDL_FRect){x                 , y                , tamanho_canto     , tamanho_canto    };
    moldura.partes[0][1]= (SDL_FRect){x+tamanho_canto   , y                , w-2*tamanho_canto , tamanho_canto    };
    moldura.partes[0][2]= (SDL_FRect){x+w-tamanho_canto , y                , tamanho_canto     , tamanho_canto    };
    moldura.partes[1][0]= (SDL_FRect){x                 , y+tamanho_canto  , tamanho_canto     , h-2*tamanho_canto};
    moldura.partes[1][1]= (SDL_FRect){x+tamanho_canto   , y+tamanho_canto  , w-2*tamanho_canto , h-2*tamanho_canto};
    moldura.partes[1][2]= (SDL_FRect){x+w-tamanho_canto , y+tamanho_canto  , tamanho_canto     , h-2*tamanho_canto};
    moldura.partes[2][0]= (SDL_FRect){x                 , y+h-tamanho_canto, tamanho_canto     , tamanho_canto    };
    moldura.partes[2][1]= (SDL_FRect){x+tamanho_canto   , y+h-tamanho_canto, w-2*tamanho_canto , tamanho_canto    };
    moldura.partes[2][2]= (SDL_FRect){x+w-tamanho_canto , y+h-tamanho_canto, tamanho_canto     , tamanho_canto    };
    return moldura;
}

void DesenharMoldura(SDL_Renderer *renderer, Moldura moldura){
    for(int i = 0; i < 3; i++)for(int j = 0; j < 3; j++)
    SDL_RenderTextureRotated(
        renderer,
        moldura.textura,
        &(SDL_FRect){EscalaMoldura*(2-j),EscalaMoldura*(2-i),EscalaMoldura,EscalaMoldura},
        &moldura.partes[i][j],
        180.0,
        NULL,
        SDL_FLIP_NONE);
}


void ModuloEvento(VariveisGerais *jogo){
    if (jogo->evento.type == SDL_EVENT_QUIT) {
                jogo->rodando = false;
            }
            if(jogo->evento.type == SDL_EVENT_MOUSE_BUTTON_DOWN) jogo->botao_mouse_direito = true;
            else jogo->botao_mouse_direito = false;
    
}

void CentralizarRectInRect(SDL_FRect *pai, SDL_FRect *filho){
    filho->x = pai->x + (pai->w-filho->w)/2;
    filho->y = pai->y + (pai->h-filho->h)/2;
}

void CentralizarRectsInRectV(SDL_FRect *pai, SDL_FRect *filho[], int n, float borda_x, float borda_y){
    borda_x = borda_x * pai->w;
    borda_y = borda_y * pai->y;
    float soma = 0;
    for(int i = 0; i < n; i++) soma += filho[i]-> h;
    float espaco_interno_remanecente = (pai->h-borda_y*2) - soma;
    float espacamento = espaco_interno_remanecente / (n-1);
    for(int i = 0; i < n; i++){
        filho[i]->x = pai->x + (pai->w-filho[i]->w)/2;
        if(i!=0){
            filho[i]->y = filho[i-1]->y + filho[i-1]->h + espacamento;
        }
        else {
            filho[i]->y = pai->h + (pai->w-soma)/2;
        }

    }
}