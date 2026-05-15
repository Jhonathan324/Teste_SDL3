#include <stdio.h>
#include <stdbool.h>
#include "gerais.h"


void InitCenaGeral(VariveisGerais *geral, Tamanhos *tamanhos){
    GetTamanhos(tamanhos);
    geral->janela = SDL_CreateWindow(
        "Teste", 
        tamanhos->tela[0], 
        tamanhos->tela[1], 
        0
    ); // (640,360) resolução base. não pode ser menor;

    geral->rodando              = true;
    geral->fullscrean           = false;
    geral->troca_reso           = false;
    geral->debug                = false;
    geral->botao_mouse_direito  = false;
    geral->botao_mouse_esquerdo = false;
    geral->botao_mouse_meio     = false;
    geral->botao_mouse_gira     = false;
    geral->cena                 = CENA_MENU;
    geral->cena_continuar       = CENA_JOGO;
    geral->resolucao_atual[0]   = tamanhos->tela[0];
    geral->resolucao_atual[1]   = tamanhos->tela[1];
    geral->resolucao_antiga[0]  = tamanhos->tela[0];
    geral->resolucao_antiga[1]  = tamanhos->tela[1];
    geral->renderizador         = SDL_CreateRenderer(geral->janela, NULL);
    geral->textura_hud          = IMG_LoadTexture(geral->renderizador, "assets/imagens/ui/panels/hud geral.png");
    geral->barra_de_vida        = (SDL_FRect){EscalaHud*19, EscalaHud*1, EscalaHud*4, EscalaHud*1};

    SDL_SetTextureScaleMode(geral->textura_hud, SDL_SCALEMODE_NEAREST);
}

void CalcularGeral(VariveisGerais *geral, Tamanhos *tamanhos){
    GetTamanhos(tamanhos);
    SDL_SetWindowSize(geral->janela,tamanhos->tela[0], tamanhos->tela[1]);
    SDL_SetWindowFullscreen(geral->janela, geral->fullscrean);

    geral->resolucao_atual[0] = tamanhos->tela[0];
    geral->resolucao_atual[1] = tamanhos->tela[1];
}
