#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "gerais.h"

int main(void)
{
    //Ligando os mudulos
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    //iniacilização de tudo
    TAMANHOS tamanhos;
    tamanhos.escala = 0;
    VariveisGerais geral;
    InitGeral(&geral, &tamanhos);

    //iniciazação das variaveis da cena menu
    VariveisMenu menu = {AZUL}; // cor de fundo
    InitMenu(&geral, &menu, tamanhos);

    //iniciazação das variaveis da cena pause
    VariveisPause pause = {SEMI_PRETO}; // cor de fundo
    InitPause(&geral, &pause, tamanhos);

    //iniciazação das variaveis da cena jogo
    VariveisJogo jogo = {
        AZUL, // cor de fundo
        0,             // velocidade jogodaor em x
        0              // velocidade jogodaor em y
    };
    //iniciazação das variaveis da cena da conf
    VariveisConf conf;
    InitConf(&geral, &conf, tamanhos);
    conf.reso_inicial = tamanhos.escala;

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

        case (CENA_CONF):
            CenaConfLoop(&geral, &conf, &tamanhos);
            CenaConfDesenhar(&geral, &conf);
            break;

        case (CENA_SAIR):
            geral.rodando = false;
            break;

        default:
            geral.cena = CENA_MENU;
            break;
        }

        if (geral.troca_reso ){
            geral.troca_reso = false;
            InitMenu(&geral, &menu, tamanhos);
            InitPause(&geral, &pause, tamanhos);
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
