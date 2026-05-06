#include "jogo.h"


PlayerInJogo InitPlayer(SDL_Renderer *renderizador, SDL_FRect retangulo_img, SDL_FRect retangulo_coli,  char *img){
    PlayerInJogo player = {
    100.0,
    3,
    0,
    0,
    (float)retangulo_coli.w/100,
    (float)retangulo_coli.w/48,
    (float)retangulo_coli.h/48,
    retangulo_img,
    retangulo_coli,
    retangulo_coli,
    retangulo_coli,
    IMG_LoadTexture(renderizador, img),
    0,0};
    return player;
}

void CalcularPlayer(const bool *teclado, PlayerInJogo *player){
    player->frame++;
    // Logica do Player
    if (teclado[SDL_SCANCODE_A])
        player->velocidade_x -= player->acelera;
    if (teclado[SDL_SCANCODE_D])
        player->velocidade_x += player->acelera;
    if (teclado[SDL_SCANCODE_W])
        player->velocidade_y -= player->acelera;
    if (teclado[SDL_SCANCODE_S])
        player->velocidade_y += player->acelera;

    if (player->velocidade_x < player->vel_max_x *-1)
        player->velocidade_x = player->vel_max_x*-1;
    if (player->velocidade_x > player->vel_max_x)
        player->velocidade_x = player->vel_max_x;
    if (player->velocidade_y < player->vel_max_y*-1)
        player->velocidade_y = player->vel_max_y*-1;
    if (player->velocidade_y > player->vel_max_y)
        player->velocidade_y = player->vel_max_y;

    if (player->velocidade_x < 0 && player->velocidade_x > -0.5)
        player->velocidade_x = 0;
    if (player->velocidade_x > 0 && player->velocidade_x < 0.5)
        player->velocidade_x = 0;
    if (player->velocidade_y < 0 && player->velocidade_y > -0.5)
        player->velocidade_y = 0;
    if (player->velocidade_y > 0 && player->velocidade_y < 0.5)
        player->velocidade_y = 0;

    player->retangulo_coli.x += player->velocidade_x;
    player->retangulo_coli.y += player->velocidade_y;

    if (player->velocidade_x < 0)
        player->velocidade_x += 0.3;
    if (player->velocidade_x > 0)
        player->velocidade_x -= 0.3;
    if (player->velocidade_y < 0)
        player->velocidade_y += 0.3;
    if (player->velocidade_y > 0)
        player->velocidade_y -= 0.3;
}

void DesenharPlayer(SDL_Renderer *renderizador, PlayerInJogo player){
    switch(player.estado_atual){
        case(IDLE):{
            SDL_RenderTexture(renderizador,player.sprite_atlas,
            &(SDL_FRect){MedidaImgPlayerX*(player.frame%10),0,MedidaImgPlayerX,MedidaImgPlayerY},
            &player.retangulo_img
        );
        }break;
        case(PULO):{

        }break;
        case(ATAQUE):{

        }break;
        case(DANO):{

        }break;
    }
}