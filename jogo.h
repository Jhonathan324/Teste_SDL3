#ifndef JOGO_H_INCLUDED
#define JOGO_H_INCLUDED
#include "gerais.h"

#define TamanhosMapaX 512
#define TamanhosMapaY 128

typedef struct PlayerInJogo
{
    int vida;
    int coracoes;
    float velocidade;
    SDL_FRect retangulo;
}PlayerInJogo;

typedef struct VariveisJogo
{
    SDL_Color cor_fundo;
    int velocidade_jogador_x;
    int velocidade_jogador_y;
    uint8_t mapa[TamanhosMapaX][TamanhosMapaY];
    PlayerinJogo jogodor;
} VariveisJogo;



void InitJogo(VariveisGerais *geral, VariveisJogo *jogo, TAMANHOS tamanhos);
void CenaJogoLoop(VariveisGerais *geral, VariveisJogo *jogo);
void CenaJogoDesenhar(VariveisGerais *geral, VariveisJogo *jogo);

#endif // JOGO_H_INCLUDED