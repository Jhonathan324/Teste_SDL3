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
    int velocidade;
    SDL_Rect retangulo;
} Player;

typedef struct Botao{
    int estado;
    char texto[30];
    int cor[3];
    SDL_Rect retangulo;
} Botao;



#endif // GERAIS_H_INCLUDED
