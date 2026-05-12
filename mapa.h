#ifndef MAPA_H_INCLUDED
#define MAPA_H_INCLUDED

#include "jogo.h"

typedef struct VariaveisMapa{
    SDL_Color cor_fundo;
    SDL_FRect selecao;
    SDL_Rect selecao_coli;
    int rolada;
    Marcador *marcador;
    Camera camera;
    Mapa mapa;
    int velocidade;
    int tamanho_bloco[2];

}VariaveisMapa;

void InitCenaMapa(SDL_Renderer *renderizador, VariaveisMapa *mapa, TAMANHOS tamanhos);
void LoopCenaMapa(VariveisGerais *geral, VariaveisMapa *mapa);
void DesenharCenaMapa(VariveisGerais geral, VariaveisMapa mapa);

#endif // MAPA_H_INCLUDED
