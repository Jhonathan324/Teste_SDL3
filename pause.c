#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "gerais.h"



void InitPause(VariveisGerais *geral, VariveisPause *pause){
    //Criação do janela para os botões
    int janela_x, janela_y, janela_w, janela_h;
    SDL_FRect rect_moldura = {0,0,600,300};
    SDL_GetWindowSize(geral->janela,&janela_w,&janela_h);
    SDL_FRect rect_janela = {0, 0, janela_w, janela_h};
    CentralizarRectInRect(&rect_janela, &rect_moldura);
    pause->moldura = InitMoldura(geral->renderizador, &rect_moldura,50,"imagens/moldura de madeira.png");

    //Criação dos botões
    pause->botao_iniciar = (Botao){0, "Iniciar Jogo", (SDL_FRect){300,300,200,60}, {70,70,70}, {30,30,30},0};
    pause->botao_conf    = (Botao){0, "Iniciar Jogo", (SDL_FRect){300,300,200,60}, {70,70,70}, {30,30,30},0};
    pause->botao_sair    = (Botao){0, "Iniciar Jogo", (SDL_FRect){300,300,200,60}, {70,70,70}, {30,30,30},0};
    SDL_FRect *botoes[] = {
        &pause->botao_iniciar.retangulo, 
        &pause->botao_conf.retangulo,
        &pause->botao_sair.retangulo};
    CentralizarRectsInRectV(&pause->moldura.retangulo, botoes ,3,0.1,0.1);
}



void CenaPauseLoop(VariveisGerais *geral, VariveisPause *pause){
    if(pause->botao_iniciar.timer){
        if (pause->botao_iniciar.timer > 1 ) pause->botao_iniciar.timer--;
        else{
            geral->cena = CENA_JOGO;
            pause->botao_iniciar.timer = 0;
        }
    }
    SDL_Rect retangulo_colisao;
    AtribuirFRectInRectA(&pause->botao_iniciar.retangulo,&retangulo_colisao);
    if(SDL_PointInRect(&geral->ponto_mouse, &retangulo_colisao))
        pause->botao_iniciar.sobre = true;
    else {
        pause->botao_iniciar.sobre = false;
    }

    if(pause->botao_iniciar.sobre && geral->botao_mouse_direito){
        pause->botao_iniciar.timer=15;
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
    if(pause->botao_iniciar.sobre && pause->botao_iniciar.timer){
        SDL_SetRenderDrawColor(geral->renderizador, pause->botao_iniciar.cor[0], pause->botao_iniciar.cor[1], pause->botao_iniciar.cor[2], 255);
    }
    else{
        SDL_SetRenderDrawColor(geral->renderizador, pause->botao_iniciar.cor2[0], pause->botao_iniciar.cor2[1], pause->botao_iniciar.cor2[2], 255);
    }
    SDL_RenderFillRect(geral->renderizador,&pause->botao_iniciar.retangulo);
}