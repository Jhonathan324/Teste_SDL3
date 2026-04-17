#ifndef GERAIS_H_INCLUDED
#define GERAIS_H_INCLUDED
#include <SDL3/SDL.h>

enum ESTADO_JOGO{
    CENA_MENU,
    CENA_JOGO,
    CENA_PAUSE,
    CENA_CONF,
    CENA_SAIR,
    CENA_CREDITOS,
    CENA_MENU_RESOLUCAO
};


typedef struct Player{
    int vida;
    int coracoes;
    float velocidade;
    SDL_FRect retangulo;
    SDL_Rect retangulo_int;
} Player;

typedef struct Botao{
    int estado;
    char texto[30];
    SDL_FRect retangulo;
    SDL_Rect retangulo_int;
    int cor[3];
    int cor2[3];
} Botao;

void atribuirFRectInRectA(SDL_FRect *fretangulo, SDL_Rect *retangulo);

#endif // GERAIS_H_INCLUDED
