#include <stdio.h>
#include <stdbool.h>
#include "mapa.h"

int main(void)
{
    //Ligando os mudulos
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    //iniacilização de tudo
    TAMANHOS tamanhos;
    tamanhos.escala = 2;
    VariveisGerais geral;
    InitCenaGeral(&geral, &tamanhos);

    //iniciazação das variaveis da cena menu
    VariveisMenu menu = {AZUL}; // cor de fundo
    InitCenaMenu(&geral, &menu, tamanhos);

    //iniciazação das variaveis da cena pause
    VariveisPause pause = {SEMI_PRETO}; // cor de fundo
    InitCenaPause(&geral, &pause, tamanhos);

    //iniciazação das variaveis da cena jogo
    VariveisJogo jogo = {AZUL_CLARO};
    InitCenaJogo(&geral, &jogo, tamanhos);

    //iniciazação das variaveis da cena da conf
    VariveisConf conf = {AZUL};
    InitCenaConf(&geral, &conf, tamanhos);
    conf.reso_inicial = tamanhos.escala;

    //iniciazação das variaveis da cena. Apenas para debug
    VariaveisMapa mapa = {AZUL};
    InitCenaMapa(geral.renderizador, &mapa, tamanhos);



    // normalização do tempo
    Uint64 tempo_inicial = SDL_GetPerformanceCounter();
    double tempo;

    // loop principal
    while (geral.rodando)
    {
        tempo_inicial = SDL_GetPerformanceCounter();
        while (SDL_PollEvent(&geral.evento))
        {
            ModuloEvento(&geral);
        }

        switch (geral.cena)
        {
        // Cenas
        case (CENA_MENU):
            LoopCenaMenu(&geral, &menu);
            DesenharCenaMenu(geral, menu);
            break;

        case (CENA_JOGO):
            LoopCenaJogo(&geral, &jogo, tempo);
            DesenharCenaJogo(geral, jogo);
            break;
        
        case (CENA_CRIACAO):
            LoopCenaMapa(&geral, &mapa);
            DesenharCenaMapa(geral, mapa);
            break;

        case (CENA_PAUSE):
            LoopCenaPause(&geral, &pause);
            DesenharCenaPause(geral, pause);
            break;

        case (CENA_CONF):
            LoopCenaConf(&geral, &conf, &tamanhos);
            DesenharCenaConf(geral, conf);
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
            InitCenaMenu(&geral, &menu, tamanhos);
            InitCenaPause(&geral, &pause, tamanhos);
            CalcularCenaJogo(&geral, &jogo, tamanhos);
        }

        // Limpar a Tela
        SDL_RenderPresent(geral.renderizador);
        SDL_Delay(16); // ~60 FPS
        tempo = (double)(SDL_GetPerformanceCounter() - tempo_inicial) / SDL_GetPerformanceFrequency();
    }

    // Saindo do jogo
    SDL_DestroyRenderer(geral.renderizador);
    SDL_DestroyWindow(geral.janela);
    SDL_Quit();

    return 0;
}
