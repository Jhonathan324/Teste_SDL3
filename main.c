#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "gerais.h"

int main(void)
{

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    TAMANHOS tamanhos;
    GetTamanhos(&tamanhos);
    VariveisGerais geral = {SDL_CreateWindow("Teste", tamanhos.tamanho_tela[0], tamanhos.tamanho_tela[1], 0)}; // (640,360) resolução base. não pode ser menor;
    geral.renderizador = SDL_CreateRenderer(geral.janela, NULL);
    geral.cena = CENA_MENU;
    geral.rodando = true;
    geral.jogador = (Player){100, 3, (float)tamanhos.tamanho_bloco[0]/tamanhos.tamanho_jogador[0]*3, (SDL_FRect){100, 100, tamanhos.tamanho_jogador[0], tamanhos.tamanho_jogador[1]}};

    VariveisMenu menu = {BRANCO}; // cor de fundo
    InitMenu(&geral, &menu, tamanhos);

    VariveisPause pause = {VERDE}; // cor de fundo
    InitPause(&geral, &pause,tamanhos);

    VariveisJogo jogo = {
        {30, 30, 200}, // cor de fundo
        0,             // velocidade jogodaor em x
        0              // velocidade jogodaor em y
    };

    while (geral.rodando)
    {

        while (SDL_PollEvent(&geral.evento))
        {
            ModuloEvento(&geral);
        }

        switch (geral.cena)
        {
            // Cenas

        case (CENA_MENU):
            CenaMenuLoop(&geral, &menu);
            CenaMenuDesenhar(&geral, &menu);
            break;

        case (CENA_JOGO):
            CenaJogoLoop(&geral, &jogo);
            CenaJogoDesenhar(&geral, &jogo);
            break;

        case (CENA_PAUSE):
            CenaPauseLoop(&geral, &pause);
            CenaPauseDesenhar(&geral, &pause);
            break;

        case (CENA_SAIR):
            geral.rodando = false;
            break;

        default:
            geral.cena = CENA_MENU;
            break;
        }

        // Limpar a Tela
        SDL_RenderPresent(geral.renderizador);
        SDL_Delay(16); // ~60 FPS
    }

    // Saindo do jogo
    SDL_DestroyRenderer(geral.renderizador);
    SDL_DestroyWindow(geral.janela);
    SDL_Quit();

    return 0;
}
