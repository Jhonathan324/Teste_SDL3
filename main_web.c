#include <stdio.h>
#include <stdbool.h>
#include "mapa.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

// Estado global necessário para o loop do Emscripten
typedef struct EstadoJogo {
    Tamanhos tamanhos;
    VariveisGerais geral;
    VariveisMenu menu;
    VariveisPause pause;
    VariveisJogo jogo;
    VariveisConf conf;
    VariaveisMapa mapa;
    Uint64 tempo_inicial;
    double tempo;
} EstadoJogo;

static EstadoJogo estado;

static void loop_principal(void)
{
    EstadoJogo *e = &estado;

    e->tempo_inicial = SDL_GetPerformanceCounter();

    while (SDL_PollEvent(&e->geral.evento))
    {
        ModuloEvento(&e->geral);
    }

    switch (e->geral.cena)
    {
    case CENA_MENU:
        LoopCenaMenu(&e->geral, &e->menu);
        DesenharCenaMenu(e->geral, e->menu);
        break;

    case CENA_JOGO:
        LoopCenaJogo(&e->geral, &e->jogo, e->tempo);
        DesenharCenaJogo(e->geral, e->jogo, e->tamanhos);
        break;

    case CENA_CRIACAO:
        LoopCenaMapa(&e->geral, &e->mapa);
        DesenharCenaMapa(e->geral, e->mapa);
        break;

    case CENA_PAUSE:
        LoopCenaPause(&e->geral, &e->pause);
        DesenharCenaPause(e->geral, e->pause);
        break;

    case CENA_CONF:
        LoopCenaConf(&e->geral, &e->conf, &e->tamanhos);
        DesenharCenaConf(e->geral, e->conf);
        break;

    case CENA_SAIR:
        e->geral.rodando = false;
#ifdef __EMSCRIPTEN__
        emscripten_cancel_main_loop();
#endif
        return;

    default:
        e->geral.cena = CENA_MENU;
        break;
    }

    if (e->geral.troca_reso) {
        e->geral.troca_reso = false;
        InitCenaMenu(&e->geral, &e->menu, e->tamanhos);
        InitCenaPause(&e->geral, &e->pause, e->tamanhos);
        InitCenaMapa(e->geral.renderizador, &e->mapa, e->tamanhos);
        CalcularCenaJogo(&e->geral, &e->jogo, e->tamanhos);
        e->geral.resolucao_antiga[0] = e->geral.resolucao_atual[0];
        e->geral.resolucao_antiga[1] = e->geral.resolucao_atual[1];
    }
    if (e->geral.carregar_mapa) {
        CalcularCenaJogo(&e->geral, &e->jogo, e->tamanhos);
        e->geral.carregar_mapa = false;
    }

    SDL_RenderPresent(e->geral.renderizador);
    e->tempo = (double)(SDL_GetPerformanceCounter() - e->tempo_inicial) / SDL_GetPerformanceFrequency();
}

int main(void)
{
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    EstadoJogo *e = &estado;

    e->tamanhos.escala = 2;
    InitCenaGeral(&e->geral, &e->tamanhos);

    e->menu = (VariveisMenu){AZUL};
    InitCenaMenu(&e->geral, &e->menu, e->tamanhos);

    e->pause = (VariveisPause){SEMI_PRETO};
    InitCenaPause(&e->geral, &e->pause, e->tamanhos);

    e->jogo = (VariveisJogo){AZUL_CLARO};
    InitCenaJogo(&e->geral, &e->jogo, e->tamanhos);

    e->conf = (VariveisConf){AZUL};
    InitCenaConf(&e->geral, &e->conf, e->tamanhos);
    e->conf.reso_inicial = e->tamanhos.escala;

    e->mapa = (VariaveisMapa){AZUL};
    InitCenaMapa(e->geral.renderizador, &e->mapa, e->tamanhos);

    e->tempo_inicial = SDL_GetPerformanceCounter();
    e->tempo = 0.0;

#ifdef __EMSCRIPTEN__
    // 0 = usar requestAnimationFrame (fps controlado pelo browser), 1 = simular loop bloqueante
    emscripten_set_main_loop(loop_principal, 0, 1);
#else
    while (e->geral.rodando)
    {
        loop_principal();
        SDL_Delay(16);
    }
    SDL_DestroyRenderer(e->geral.renderizador);
    SDL_DestroyWindow(e->geral.janela);
    SDL_Quit();
#endif

    return 0;
}
