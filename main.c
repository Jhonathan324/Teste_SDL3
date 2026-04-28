#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "gerais.h"

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    VariveisGerais geral = {SDL_CreateWindow("Teste", 640, 360, 0)};// (640,360) resolução base. não pode ser menor;
    geral.renderizador = SDL_CreateRenderer(geral.janela,NULL);
    geral.cena = CENA_MENU;
    geral.rodando = true;
    geral.jogador = (Player)
    {100, 3, 2, 
    (SDL_FRect)
    {100,100,50,50}
    };

    VariveisMenu menu={{30,200,30}}; // cor de fundo
    menu.moldura = InitMoldura(geral.renderizador,0,0,600,300,50,"imagens/moldura de madeira.png");
    menu.botao_iniciar = (Botao){0, "Iniciar Jogo", (SDL_FRect){300,300,200,60}, (SDL_Rect){300,300,200,60}, {70,70,70}, {30,30,30},0};
    menu.botao_conf    = (Botao){0, "Iniciar Jogo", (SDL_FRect){300,300,200,60}, (SDL_Rect){300,300,200,60}, {70,70,70}, {30,30,30},0};
    menu.botao_sair    = (Botao){0, "Iniciar Jogo", (SDL_FRect){300,300,200,60}, (SDL_Rect){300,300,200,60}, {70,70,70}, {30,30,30},0};

    VariveisPause pause={
        {30,200,30},
        (Botao){0, "Iniciar Jogo", (SDL_FRect){300,300,200,60}, (SDL_Rect){300,300,200,60}, {70,70,70}, {30,30,30}},
        InitMoldura(geral.renderizador,0,0,600,300,50,"imagens/moldura de madeira.png")
    };


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
                CenaMenu(&geral, &menu);
                CenaMenuDesenhar(&geral, &menu);
                break;


            case(CENA_JOGO):
                CenaJogo(&geral, &jogo);
                CenaJogoDesenhar(&geral, &jogo);
                break;


            case(CENA_PAUSE):
                CenaPause(&geral, &pause);
                CenaPauseDesenhar(&geral, &pause);
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
