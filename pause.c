#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "gerais.h"



void CenaPause(VariveisGerais *geral, VariveisPause *pause){
    if(pause->botao_iniciar.timer){
        if (pause->botao_iniciar.timer > 1 ) pause->botao_iniciar.timer--;
        else{
            geral->cena = CENA_JOGO;
            pause->botao_iniciar.timer = 0;
        }
    }
    if(SDL_PointInRect(&geral->ponto_mouse, &pause->botao_iniciar.retangulo_int))
        pause->botao_iniciar.sobre = true;
    else {
        pause->botao_iniciar.sobre = false;
    }

    if(pause->botao_iniciar.sobre && geral->botao_mouse_direito){
        pause->botao_iniciar.timer=5;
    }

    SDL_GetMouseState(&geral->mouse_x, &geral->mouse_y);
    geral->ponto_mouse.x = geral->mouse_x;
    geral->ponto_mouse.y = geral->mouse_y;
}


void CenaPauseDesenhar(VariveisGerais *geral, VariveisPause *pause){
    //limpeza de tela
    SDL_SetRenderDrawColor(geral->renderizador, pause->cor_fundo[0], pause->cor_fundo[1], pause->cor_fundo[2], 255);
    SDL_RenderClear(geral->renderizador);

    // botões
    DesenharMoldura(geral->renderizador, pause->moldura);

    AtribuirFRectInRectA(&pause->botao_iniciar.retangulo,&pause->botao_iniciar.retangulo_int);
    if(pause->botao_iniciar.sobre && pause->botao_iniciar.timer){
        SDL_SetRenderDrawColor(geral->renderizador, pause->botao_iniciar.cor[0], pause->botao_iniciar.cor[1], pause->botao_iniciar.cor[2], 255);
    }
    else{
        SDL_SetRenderDrawColor(geral->renderizador, pause->botao_iniciar.cor2[0], pause->botao_iniciar.cor2[1], pause->botao_iniciar.cor2[2], 255);
    }
    SDL_RenderFillRect(geral->renderizador,&pause->botao_iniciar.retangulo);
}