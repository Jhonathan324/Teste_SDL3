#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "gerais.h"

void CenaJogo(VariveisGerais *geral, VariveisJogo *jogo){
    const bool *teclado = SDL_GetKeyboardState(NULL);

    if (teclado[SDL_SCANCODE_ESCAPE]) geral->cena = CENA_PAUSE;

    // Logica do Player

    if (teclado[SDL_SCANCODE_A]) jogo->velocidade_jogador_x -= geral->jogador.velocidade;
    if (teclado[SDL_SCANCODE_D]) jogo->velocidade_jogador_x += geral->jogador.velocidade;
    if (teclado[SDL_SCANCODE_W]) jogo->velocidade_jogador_y -= geral->jogador.velocidade;
    if (teclado[SDL_SCANCODE_S]) jogo->velocidade_jogador_y += geral->jogador.velocidade;

    if (jogo->velocidade_jogador_x < -10) jogo->velocidade_jogador_x = -10;
    if (jogo->velocidade_jogador_x >  10) jogo->velocidade_jogador_x =  10;
    if (jogo->velocidade_jogador_y < -10) jogo->velocidade_jogador_y = -10;
    if (jogo->velocidade_jogador_y >  10) jogo->velocidade_jogador_y =  10;

    if (jogo->velocidade_jogador_x < 0 && jogo->velocidade_jogador_x > - 0.5) jogo->velocidade_jogador_x = 0;
    if (jogo->velocidade_jogador_x > 0 && jogo->velocidade_jogador_x <   0.5) jogo->velocidade_jogador_x = 0;
    if (jogo->velocidade_jogador_y < 0 && jogo->velocidade_jogador_y > - 0.5) jogo->velocidade_jogador_y = 0;
    if (jogo->velocidade_jogador_y > 0 && jogo->velocidade_jogador_y <   0.5) jogo->velocidade_jogador_y = 0;

    geral->jogador.retangulo.x += jogo->velocidade_jogador_x;
    geral->jogador.retangulo.y += jogo->velocidade_jogador_y;

    if (jogo->velocidade_jogador_x < 0) jogo->velocidade_jogador_x += 0.3;
    if (jogo->velocidade_jogador_x > 0) jogo->velocidade_jogador_x -= 0.3;
    if (jogo->velocidade_jogador_y < 0) jogo->velocidade_jogador_y += 0.3;
    if (jogo->velocidade_jogador_y > 0) jogo->velocidade_jogador_y -= 0.3;
}

void CenaJogoDesenhar(VariveisGerais *geral, VariveisJogo *jogo){
    //Limpeza de Tela
    SDL_SetRenderDrawColor(geral->renderizador, jogo->cor_fundo[0], jogo->cor_fundo[1], jogo->cor_fundo[2], 255);
    SDL_RenderClear(geral->renderizador);

    //player
    SDL_SetRenderDrawColor(geral->renderizador, 255, 0, 0, 255);
    SDL_RenderFillRect(geral->renderizador, &geral->jogador.retangulo);
}