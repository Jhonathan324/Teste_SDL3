#ifndef GERAIS_H_INCLUDED
#define GERAIS_H_INCLUDED
#include <SDL3/SDL.h>
#define COR1 (20,20,20);
#define  EscalaMoldura 16

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
} Player;

typedef struct Botao{
    int timer;
    char texto[30];
    SDL_FRect retangulo;
    int cor[3];
    int cor2[3];
    int indice;
    bool sobre;
    SDL_Texture *textura;
    SDL_Texture *imagem;
} Botao;


typedef struct Moldura{
    SDL_FRect retangulo;
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
    Moldura moldura;
    Botao botao_iniciar;
    Botao botao_conf;
    Botao botao_sair;
} VariveisPause;

typedef struct VariveisJogo{
    int cor_fundo[3];
    int velocidade_jogador_x;
    int velocidade_jogador_y;
} VariveisJogo;

Moldura InitMoldura(SDL_Renderer *renderer, SDL_FRect *retangulo, int tamanho_canto,char *file);
void GetTamanhos(int *escala, float (*tamanho_tela)[2], float (*tamanho_menu)[2], float (*tamanho_botao_menu)[2], float (*tamanho_jogador)[2], float (*tamanho_inimigo1)[2], float (*tamanho_inimigo2)[2]);
void DesenharMoldura(SDL_Renderer *renderer, Moldura moldura);
void DesenharBotao(SDL_Renderer *renderer, Botao botao);
void AtribuirFRectInRectA(SDL_FRect *fretangulo, SDL_Rect *retangulo);
void CentralizarRectInRect(SDL_FRect *rect_pai, SDL_FRect *rect_filho);
void CentralizarRectsInRectV(SDL_FRect *pai, SDL_FRect *filho[], int n, float borda_x, float borda_y);


void ModuloEvento(VariveisGerais *geral);

void InitMenu(VariveisGerais *geral, VariveisMenu *menu);
void CenaMenuLoop(VariveisGerais *geral, VariveisMenu *menu);
void CenaMenuDesenhar(VariveisGerais *geral, VariveisMenu *menu);

void InitPause(VariveisGerais *geral, VariveisPause *pause);
void CenaPauseLoop(VariveisGerais *geral, VariveisPause *pause);
void CenaPauseDesenhar(VariveisGerais *geral, VariveisPause *pause);

void InitJogo(VariveisGerais *geral, VariveisJogo *jogo);
void CenaJogoLoop(VariveisGerais *geral, VariveisJogo *jogo);
void CenaJogoDesenhar(VariveisGerais *geral, VariveisJogo *jogo);

#endif // GERAIS_H_INCLUDED
