#include <SDL3/SDL.h>
#include <stdbool.h>

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Jogo SDL3", 800, 600, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    bool rodando = true;
    SDL_Event event;

    // Player
    float px = 100, py = 100;
    float speed = 5;

    // Inimigo
    float ex = 400, ey = 300;
    float espeed = 3;

    while (rodando) {

        // Eventos
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                rodando = false;
            }
        }

        // Teclado
        const bool *keys = SDL_GetKeyboardState(NULL);

        if (keys[SDL_SCANCODE_W]) py -= speed;
        if (keys[SDL_SCANCODE_S]) py += speed;
        if (keys[SDL_SCANCODE_A]) px -= speed;
        if (keys[SDL_SCANCODE_D]) px += speed;

        // IA simples (inimigo segue o player)
        if (px > ex) ex += espeed;
        if (px < ex) ex -= espeed;
        if (py > ey) ey += espeed;
        if (py < ey) ey -= espeed;

        // Colisão (AABB simples)
        if (px < ex + 50 &&
            px + 50 > ex &&
            py < ey + 50 &&
            py + 50 > ey) {

            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                "Game Over",
                "Você perdeu!",
                window
            );

            rodando = false;
        }

        // Render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Player (verde)
        SDL_FRect player = { px, py, 50, 50 };
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &player);

        // Inimigo (vermelho)
        SDL_FRect enemy = { ex, ey, 50, 50 };
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &enemy);

        SDL_RenderPresent(renderer);

        SDL_Delay(16); // ~60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
