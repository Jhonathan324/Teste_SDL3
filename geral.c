#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "gerais.h"


void InitGeral(VariveisGerais *geral, TAMANHOS *tamanhos){
    GetTamanhos(tamanhos);
    geral->janela = SDL_CreateWindow("Teste", tamanhos->tamanho_tela[0], tamanhos->tamanho_tela[1], 0); // (640,360) resolução base. não pode ser menor;
    geral->rodando = true;
    geral->cena = CENA_MENU;
    geral->renderizador = SDL_CreateRenderer(geral->janela, NULL);
    geral->jogador = (Player){100, 3, (float)tamanhos->tamanho_bloco[0]/tamanhos->tamanho_jogador[0]*3, (SDL_FRect){100, 100, tamanhos->tamanho_jogador[0], tamanhos->tamanho_jogador[1]}};
}