#ifndef GERAIS_H_INCLUDED
#define GERAIS_H_INCLUDED
#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>

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
    int timer;
    char texto[30];
    SDL_FRect retangulo;
    SDL_Rect retangulo_int;
    int cor[3];
    int cor2[3];
    bool sobre;
} Botao;


typedef struct Moldura{
    int x;
    int y;
    int w;
    int h;
    SDL_Texture *textura;
    SDL_FRect partes[3][3];
} Moldura;

typedef struct VariveisGerais{
    SDL_Window *janela;
    SDL_Renderer *renderizador;
    SDL_Event evento;
    SDL_Point ponto_mouse;
    int cena;
    bool rodando;
    float mouse_x, mouse_y;
    bool botao_mouse_direito;
    bool botao_mouse_esquerdo;
    Player jogador;
} VariveisGerais;

typedef struct VariveisMenu{
    int cor_fundo[3];
    Moldura moldura;
    Botao botao_iniciar;
    Botao botao_conf;
    Botao botao_sair;

} VariveisMenu;

typedef struct VariveisPause{
    int cor_fundo[3];
    Botao botao_iniciar;
    Moldura moldura;

} VariveisPause;

typedef struct VariveisJogo{
    int cor_fundo[3];
    int velocidade_jogador_x;
    int velocidade_jogador_y;

} VariveisJogo;

Moldura InitMoldura(SDL_Renderer *renderer, int x, int y, int w, int h, int tamanho_canto,char *file);

void AtribuirFRectInRectA(SDL_FRect *fretangulo, SDL_Rect *retangulo);

void DesenharMoldura(SDL_Renderer *renderer, Moldura moldura);

void ModuloEvento(VariveisGerais *geral);

void CenaMenu(VariveisGerais *geral, VariveisMenu *menu);

void CenaMenuDesenhar(VariveisGerais *geral, VariveisMenu *menu);

void CenaPause(VariveisGerais *geral, VariveisPause *pause);

void CenaPauseDesenhar(VariveisGerais *geral, VariveisPause *pause);

void CenaJogo(VariveisGerais *geral, VariveisJogo *jogo);

void CenaJogoDesenhar(VariveisGerais *geral, VariveisJogo *jogo);

#endif // GERAIS_H_INCLUDED
