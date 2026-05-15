#ifndef MAPA_H_INCLUDED
#define MAPA_H_INCLUDED

#include "jogo.h"

typedef struct MarcadorBloco{
    SDL_FRect retangulo;
    bool sobre;
    bool ativo;
    SDL_Color cor1;
    SDL_Color cor2;
    SDL_FRect partes[3][3];
} MarcadorBloco;

typedef struct VariaveisMapa{
    SDL_Color cor_fundo;
    SDL_FRect selecao;
    SDL_Point selecao_coli;
    SDL_Point selecao_coli_back;
    SDL_Point selecao_coli_preencimento;
    int rolada;
    int bloco_uso;
    int velocidade;
    int tamanho_bloco[2];
    bool preencher;
    Mapa mapa;
    Camera camera;
    Camera camera_back;
    Botao botao_salvar;
    Botao botao_carregar;
    Moldura moldura_bloco;
    Marcador marcador_preencher;
    MarcadorBloco marcador[600];
}VariaveisMapa;

void InitCenaMapa(SDL_Renderer *renderizador, VariaveisMapa *mapa, Tamanhos tamanhos);
void LoopCenaMapa(VariveisGerais *geral, VariaveisMapa *mapa);
void DesenharCenaMapa(VariveisGerais geral, VariaveisMapa mapa);

#endif // MAPA_H_INCLUDED
