#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "gerais.h"



void CenaMenu(VariveisGerais *geral, VariveisMenu *menu){
    if(menu->botao_iniciar.timer){
        if (menu->botao_iniciar.timer > 1 ) menu->botao_iniciar.timer--;
        else{
            geral->cena = CENA_JOGO;
            menu->botao_iniciar.timer = 0;
        }
    }
    if(SDL_PointInRect(&geral->ponto_mouse, &menu->botao_iniciar.retangulo_int))
        menu->botao_iniciar.sobre = true;
    else {
        menu->botao_iniciar.sobre = false;
    }

    if(menu->botao_iniciar.sobre && geral->botao_mouse_direito){
        menu->botao_iniciar.timer=5;
    }

    SDL_GetMouseState(&geral->mouse_x, &geral->mouse_y);
    geral->ponto_mouse.x = geral->mouse_x;
    geral->ponto_mouse.y = geral->mouse_y;
}


void CenaMenuDesenhar(VariveisGerais *geral, VariveisMenu *menu){
    //limpeza de tela
    SDL_SetRenderDrawColor(geral->renderizador, menu->cor_fundo[0], menu->cor_fundo[1], menu->cor_fundo[2], 255);
    SDL_RenderClear(geral->renderizador);

    // botões
    DesenharMoldura(geral->renderizador, menu->moldura);

    AtribuirFRectInRectA(&menu->botao_iniciar.retangulo,&menu->botao_iniciar.retangulo_int);
    if(menu->botao_iniciar.sobre && menu->botao_iniciar.timer){
        SDL_SetRenderDrawColor(geral->renderizador, menu->botao_iniciar.cor[0], menu->botao_iniciar.cor[1], menu->botao_iniciar.cor[2], 255);
    }
    else{
        SDL_SetRenderDrawColor(geral->renderizador, menu->botao_iniciar.cor2[0], menu->botao_iniciar.cor2[1], menu->botao_iniciar.cor2[2], 255);
    }
    SDL_RenderFillRect(geral->renderizador,&menu->botao_iniciar.retangulo);
}