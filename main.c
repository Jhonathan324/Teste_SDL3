#include <SDL3/SDL.h>
#include <stdbool.h>
#include "gerais.h"

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Teste", 800, 600, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    SDL_Event evento;

    bool rodando = true;


    int cena = CENA_MENU;

    Botao botao_iniciar = {0, "Iniciar Jogo", (int){40,40,40}, (SDL_Rect){300,300,200,60}};
    mouse_x, mouse_y;
    bool mouse = SDL_MouseButtonEvent(&mouse_x, &mouse_y);




    Player jogador = {100, 3, 5, (SDL_Rect){100,100,50,50}};
    int velocidade_jogador_x = 0;
    int velocidade_jogador_y = 0;


    while (rodando) {

        while (SDL_PollEvent(&evento)) {
            if (evento.type == SDL_EVENT_QUIT) {
                rodando = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);


        switch(cena){
        case(CENA_MENU):
            bool mouse = SDL_MouseButtonEvent(&mouse_x, &mouse_y);
            SDL_SetRenderDrawColor(renderer, botao_iniciar.cor[0], botao_iniciar.cor[1], botao_iniciar.cor[2], 255);
            SDL_RenderClear(renderer);
            SDL_RenderFillRect(renderer,botao_iniciar.retangulo);






            break;







        case(CENA_JOGO:
            const bool *teclado = SDL_GetKeyboardState(NULL);

            if (teclado[SDL_SCANCODE_W]) velocidade_jogador_x -= jogador.velocidade;
            if (teclado[SDL_SCANCODE_S]) velocidade_jogador_x += jogador.velocidade;
            if (teclado[SDL_SCANCODE_A]) velocidade_jogador_x -= jogador.velocidade;
            if (teclado[SDL_SCANCODE_D]) velocidade_jogador_x += jogador.velocidade;

            SDL_FRect enemy = { ex, ey, 50, 50 };
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer, &enemy);
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
