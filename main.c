#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "gerais.h"


int main(void) {
    
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    TAMANHOS tamanhos;
    GetTamanhos(&tamanhos.escala,&tamanhos.tamanho_tela,&tamanhos.tamanho_menu,&tamanhos.tamanho_botao_menu,&tamanhos.tamanho_jogador,&tamanhos.tamanho_bloco,&tamanhos.tamanho_inimigo1,&tamanhos.tamanho_inimigo2);
    VariveisGerais geral = {SDL_CreateWindow("Teste", tamanhos.tamanho_tela[0], tamanhos.tamanho_tela[1], 0)};// (640,360) resolução base. não pode ser menor;
    geral.renderizador = SDL_CreateRenderer(geral.janela,NULL);
    geral.cena = CENA_MENU;
    geral.rodando = true;
    geral.jogador = (Player)
    {100, 3, 2, 
    (SDL_FRect)
    {100,100,50,50}
    };

    VariveisMenu menu={{255,255,255}}; // cor de fundo
    InitMenu(&geral, &menu, tamanhos);

    VariveisPause pause={{30,200,30}};// cor de fundo
    InitPause(&geral, &pause);

    VariveisJogo jogo = {
        {30,30,200},   // cor de fundo
        0,             // velocidade jogodaor em x
        0              // velocidade jogodaor em y
    };
    

    while (geral.rodando) {

        while (SDL_PollEvent(&geral.evento)){
            ModuloEvento(&geral);
        }

        switch(geral.cena){
        // Cenas

            case(CENA_MENU):
                CenaMenuLoop(&geral, &menu);
                CenaMenuDesenhar(&geral, &menu);
                break;


            case(CENA_JOGO):
                CenaJogoLoop(&geral, &jogo);
                CenaJogoDesenhar(&geral, &jogo);
                break;


            case(CENA_PAUSE):
                CenaPauseLoop(&geral, &pause);
                CenaPauseDesenhar(&geral, &pause);
                break;
            

            case(CENA_SAIR):
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
