#include <stdio.h>
#include <stdbool.h>
#include "gerais.h"
#include "jogo.h"



void InitJogo(VariveisGerais *geral, VariveisJogo *jogo, TAMANHOS tamanhos){
    geral->jogador = geral->jogador = (Player){100,
         3,
        (float)tamanhos.tamanho_bloco[0]/tamanhos.tamanho_jogador[0]*3,
        (SDL_FRect){100, 100, tamanhos.tamanho_jogador[0], tamanhos.tamanho_jogador[1]}};

    jogo->cor_fundo = (SDL_Color)AZUL;
    jogo->velocidade_jogador_x = 0;
    jogo->velocidade_jogador_y = 0;
    memset(jogo->mapa,1,sizeof(jogo->mapa));

    for(int i = 0; i < TamanhosMapaX; i++) printf("%d", jogo->mapa[i][0]);



}

void CenaJogoLoop(VariveisGerais *geral, VariveisJogo *jogo)
{
    const bool *teclado = SDL_GetKeyboardState(NULL);

    if (teclado[SDL_SCANCODE_ESCAPE])
        geral->cena = CENA_PAUSE;

    // Logica do Player

    if (teclado[SDL_SCANCODE_A])
        jogo->velocidade_jogador_x -= geral->jogador.velocidade;
    if (teclado[SDL_SCANCODE_D])
        jogo->velocidade_jogador_x += geral->jogador.velocidade;
    if (teclado[SDL_SCANCODE_W])
        jogo->velocidade_jogador_y -= geral->jogador.velocidade;
    if (teclado[SDL_SCANCODE_S])
        jogo->velocidade_jogador_y += geral->jogador.velocidade;

    if (jogo->velocidade_jogador_x < -10)
        jogo->velocidade_jogador_x = -10;
    if (jogo->velocidade_jogador_x > 10)
        jogo->velocidade_jogador_x = 10;
    if (jogo->velocidade_jogador_y < -10)
        jogo->velocidade_jogador_y = -10;
    if (jogo->velocidade_jogador_y > 10)
        jogo->velocidade_jogador_y = 10;

    if (jogo->velocidade_jogador_x < 0 && jogo->velocidade_jogador_x > -0.5)
        jogo->velocidade_jogador_x = 0;
    if (jogo->velocidade_jogador_x > 0 && jogo->velocidade_jogador_x < 0.5)
        jogo->velocidade_jogador_x = 0;
    if (jogo->velocidade_jogador_y < 0 && jogo->velocidade_jogador_y > -0.5)
        jogo->velocidade_jogador_y = 0;
    if (jogo->velocidade_jogador_y > 0 && jogo->velocidade_jogador_y < 0.5)
        jogo->velocidade_jogador_y = 0;

    geral->jogador.retangulo.x += jogo->velocidade_jogador_x;
    geral->jogador.retangulo.y += jogo->velocidade_jogador_y;

    if (jogo->velocidade_jogador_x < 0)
        jogo->velocidade_jogador_x += 0.3;
    if (jogo->velocidade_jogador_x > 0)
        jogo->velocidade_jogador_x -= 0.3;
    if (jogo->velocidade_jogador_y < 0)
        jogo->velocidade_jogador_y += 0.3;
    if (jogo->velocidade_jogador_y > 0)
        jogo->velocidade_jogador_y -= 0.3;
}

void CenaJogoDesenhar(VariveisGerais *geral, VariveisJogo *jogo)
{
    // Limpeza de Tela
    SDL_SetRenderDrawColor(geral->renderizador, jogo->cor_fundo.r, jogo->cor_fundo.g, jogo->cor_fundo.b, jogo->cor_fundo.a);
    SDL_RenderClear(geral->renderizador);

    // player
    SDL_SetRenderDrawColor(geral->renderizador, 255, 0, 0, 255);
    SDL_RenderFillRect(geral->renderizador, &geral->jogador.retangulo);
}