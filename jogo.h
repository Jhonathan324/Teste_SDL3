#ifndef JOGO_H_INCLUDED
#define JOGO_H_INCLUDED
#include "gerais.h"

#define TamanhosMapaX 512
#define TamanhosMapaY 128
#define MedidaImgPlayerX 64
#define MedidaImgPlayerY 48

enum ESTADO_PLAYER{
    IDLE,
    PULO,
    ATAQUE,
    DANO,
};

typedef struct PlayerInJogo
{
    float vida;
    int coracoes;
    float velocidade_x;
    float velocidade_y;
    float acelera;
    float vel_max_x;
    float vel_max_y;
    SDL_FRect retangulo_img;
    SDL_FRect retangulo_coli;
    SDL_FRect retangulo_coli_h;
    SDL_FRect retangulo_coli_v;
    SDL_Texture *sprite_atlas;
    int estado_atual;
    int frame;
}PlayerInJogo;

typedef struct VariveisJogo
{
    SDL_Color cor_fundo;
    int velocidade_jogador_x;
    int velocidade_jogador_y;
    uint8_t mapa[TamanhosMapaX][TamanhosMapaY];
    PlayerInJogo jogador;
} VariveisJogo;



void InitJogo(VariveisGerais *geral, VariveisJogo *jogo, TAMANHOS tamanhos);
void CalcularJogo(VariveisGerais *geral, VariveisJogo *jogo, TAMANHOS tamanhos);
void CenaJogoLoop(VariveisGerais *geral, VariveisJogo *jogo);
void CenaJogoDesenhar(VariveisGerais *geral, VariveisJogo *jogo);

PlayerInJogo InitPlayer(SDL_Renderer *renderizador, SDL_FRect retangulo_img, SDL_FRect retangulo_coli,  char *img);
void CalcularPlayer(const bool *teclado, PlayerInJogo *player);
void DesenharPlayer(SDL_Renderer *renderizador, PlayerInJogo player);

#endif // JOGO_H_INCLUDED