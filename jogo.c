#include <stdio.h>
#include <stdbool.h>
#include "gerais.h"
#include "jogo.h"



void InitJogo(VariveisGerais *geral, VariveisJogo *jogo, TAMANHOS tamanhos){
    geral->jogador = (Player){100,
         3,
        (float)tamanhos.tamanho_bloco[0]/tamanhos.tamanho_jogador[0]*3,
    };

    jogo->jogador = InitPlayer(
    geral->renderizador, 
    (SDL_FRect){100,100,tamanhos.tamanho_jogador[0],tamanhos.tamanho_jogador[1]},
    (SDL_FRect){100,100,tamanhos.tamanho_jogador[0]/2,tamanhos.tamanho_jogador[1]*((float)2/3)},
    "C:/Users/17898981/projetos/Teste_SDL3/assets/images/entities/player/Samurai.png"
    );

    jogo->cor_fundo = (SDL_Color)AZUL;
    jogo->velocidade_jogador_x = 0;
    jogo->velocidade_jogador_y = 0;
    memset(jogo->mapa,0,sizeof(jogo->mapa));

}

void CalcularJogo(VariveisGerais *geral, VariveisJogo *jogo, TAMANHOS tamanhos){
    // a fazer
    float tela_antiga_x = jogo->jogador.retangulo_img.w / ((float)64 / 640); 
    float tela_antiga_y = jogo->jogador.retangulo_img.h / ((float)48 / 360);  

    float x = tamanhos.tamanho_tela[0] * ( (double)jogo->jogador.retangulo_coli.x/tela_antiga_x );
    float y = tamanhos.tamanho_tela[1] * ( (double)jogo->jogador.retangulo_coli.y/tela_antiga_y );
    jogo->jogador = InitPlayer(
    geral->renderizador, 
    (SDL_FRect){x,y,tamanhos.tamanho_jogador[0],tamanhos.tamanho_jogador[1]},
    (SDL_FRect){x,y,tamanhos.tamanho_jogador[0]/2,tamanhos.tamanho_jogador[1]*((float)2/3)},
    "C:/Users/17898981/projetos/Teste_SDL3/assets/images/entities/player/Samurai.png"
    );
}

void CenaJogoLoop(VariveisGerais *geral, VariveisJogo *jogo)
{
    const bool *teclado = SDL_GetKeyboardState(NULL);

    if (teclado[SDL_SCANCODE_ESCAPE])
        geral->cena = CENA_PAUSE;

    CalcularPlayer(teclado, &jogo->jogador);
}

void CenaJogoDesenhar(VariveisGerais *geral, VariveisJogo *jogo)
{
    // Limpeza de Tela
    SDL_SetRenderDrawColor(geral->renderizador, jogo->cor_fundo.r, jogo->cor_fundo.g, jogo->cor_fundo.b, jogo->cor_fundo.a);
    SDL_RenderClear(geral->renderizador);

    // Elementos
    DesenharPlayer(geral->renderizador, jogo->jogador);

    // player
    SDL_SetRenderDrawColor(geral->renderizador, 255, 0, 0, 255);
    SDL_RenderFillRect(geral->renderizador, &jogo->jogador.retangulo_coli);
}

