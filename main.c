#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include "gerais.h"

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Teste", 800, 600, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);


    SDL_Event evento;

    bool rodando = true;


    int cena = CENA_MENU;

    int cor_fundo_menu[3] = {30,200,30};
    Botao botao_iniciar = {0, "Iniciar Jogo", (SDL_FRect){300,300,200,60}, (SDL_Rect){300,300,200,60}, {70,70,70}, {30,30,30}};
    float mouse_x, mouse_y;
    bool mouse_d;


    int cor_fundo_jogo[3] = {30,30,200};
    Player jogador = {100, 3, 1, (SDL_FRect){100,100,50,50}};
    float velocidade_jogador_x = 0;
    float velocidade_jogador_y = 0;


    while (rodando) {

        while (SDL_PollEvent(&evento)) {
            if (evento.type == SDL_EVENT_QUIT) {
                rodando = false;
            }
            if(evento.type == SDL_EVENT_MOUSE_BUTTON_DOWN) mouse_d = true;
            else mouse_d = false;
        }




        switch(cena){
        case(CENA_MENU):
            if(botao_iniciar.estado){
                if (botao_iniciar.estado > 1 ) botao_iniciar.estado--;
                else{
                    cena = CENA_JOGO;
                    botao_iniciar.estado = 0;
                }
            }



            SDL_GetMouseState(&mouse_x, &mouse_y);
            SDL_Point ponto_mouse = {mouse_x, mouse_y};




            SDL_SetRenderDrawColor(renderer, cor_fundo_menu[0], cor_fundo_menu[1], cor_fundo_menu[2], 255);
            SDL_RenderClear(renderer);
            atribuirFRectInRectA(&botao_iniciar.retangulo,&botao_iniciar.retangulo_int);

            if(SDL_PointInRect(&ponto_mouse, &botao_iniciar.retangulo_int) && mouse_d){
                SDL_SetRenderDrawColor(renderer, botao_iniciar.cor[0], botao_iniciar.cor[1], botao_iniciar.cor[2], 255);
                botao_iniciar.estado = 5;
            }
            else{
                SDL_SetRenderDrawColor(renderer, botao_iniciar.cor2[0], botao_iniciar.cor2[1], botao_iniciar.cor2[2], 255);
            }
            SDL_RenderFillRect(renderer,&botao_iniciar.retangulo);



            break;






        case(CENA_JOGO):
            const bool *teclado = SDL_GetKeyboardState(NULL);

            if (teclado[SDL_SCANCODE_ESCAPE]) cena = CENA_PAUSE;

            // Logica do Player

            if (teclado[SDL_SCANCODE_A]) velocidade_jogador_x -= jogador.velocidade;
            if (teclado[SDL_SCANCODE_D]) velocidade_jogador_x += jogador.velocidade;
            if (teclado[SDL_SCANCODE_W]) velocidade_jogador_y -= jogador.velocidade;
            if (teclado[SDL_SCANCODE_S]) velocidade_jogador_y += jogador.velocidade;

            if (velocidade_jogador_x < -10) velocidade_jogador_x = -10;
            if (velocidade_jogador_x >  10) velocidade_jogador_x =  10;
            if (velocidade_jogador_y < -10) velocidade_jogador_y = -10;
            if (velocidade_jogador_y >  10) velocidade_jogador_y =  10;

            if (velocidade_jogador_x < 0 && velocidade_jogador_x > - 0.5) velocidade_jogador_x = 0;
            if (velocidade_jogador_x > 0 && velocidade_jogador_x <   0.5) velocidade_jogador_x = 0;
            if (velocidade_jogador_y < 0 && velocidade_jogador_y > - 0.5) velocidade_jogador_y = 0;
            if (velocidade_jogador_y > 0 && velocidade_jogador_y <   0.5) velocidade_jogador_y = 0;

            jogador.retangulo.x += velocidade_jogador_x;
            jogador.retangulo.y += velocidade_jogador_y;

            if (velocidade_jogador_x < 0) velocidade_jogador_x += 0.3;
            if (velocidade_jogador_x > 0) velocidade_jogador_x -= 0.3;
            if (velocidade_jogador_y < 0) velocidade_jogador_y += 0.3;
            if (velocidade_jogador_y > 0) velocidade_jogador_y -= 0.3;




            SDL_SetRenderDrawColor(renderer, cor_fundo_jogo[0], cor_fundo_jogo[1], cor_fundo_jogo[2], 255);
            SDL_RenderClear(renderer);

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer, &jogador.retangulo);
            break;


        case(CENA_PAUSE):
            if(botao_iniciar.estado){
                if (botao_iniciar.estado > 1 ) botao_iniciar.estado--;
                else{
                    cena = CENA_JOGO;
                    botao_iniciar.estado = 0;
                }
            }



            SDL_GetMouseState(&mouse_x, &mouse_y);
            ponto_mouse.x = mouse_x;
            ponto_mouse.y = mouse_y;




            SDL_SetRenderDrawColor(renderer, cor_fundo_menu[0], cor_fundo_menu[1], cor_fundo_menu[2], 255);
            SDL_RenderClear(renderer);
            atribuirFRectInRectA(&botao_iniciar.retangulo,&botao_iniciar.retangulo_int);
            if(SDL_PointInRect(&ponto_mouse, &botao_iniciar.retangulo_int) && mouse_d){
                SDL_SetRenderDrawColor(renderer, botao_iniciar.cor[0], botao_iniciar.cor[1], botao_iniciar.cor[2], 255);
                botao_iniciar.estado = 5;
            }
            else{
                SDL_SetRenderDrawColor(renderer, botao_iniciar.cor2[0], botao_iniciar.cor2[1], botao_iniciar.cor2[2], 255);
            }
            SDL_RenderFillRect(renderer,&botao_iniciar.retangulo);

            break;
        }
        // Limpar a Tela

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
