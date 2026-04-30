#ifndef GERAIS_H_INCLUDED
#define GERAIS_H_INCLUDED
#include <SDL3/SDL.h>
#define CORCORFUNDO {20, 20, 20, 255}
#define COR1 {20, 20, 20, 255}
#define PRETO {0, 0, 0, 255}
#define BRANCO {255, 255, 255, 255}
#define VERMELHO {255, 20, 20, 255}
#define VERDE {20, 255, 20, 255}
#define AZUL {20, 20, 25, 255}
#define EscalaMoldura 16
#define EscalaBotao 8

enum ESTADO_JOGO
{
    CENA_MENU,
    CENA_JOGO,
    CENA_PAUSE,
    CENA_CONF,
    CENA_CREDITOS,
    CENA_SAIR,
};

typedef struct Player
{
    int vida;
    int coracoes;
    float velocidade;
    SDL_FRect retangulo;
} Player;

typedef struct Botao
{
    int timer;
    char *texto;
    SDL_FRect retangulo;
    SDL_Color cor1;
    SDL_Color cor2;
    int indice;
    bool sobre;
    float proporcao;
    SDL_Texture *textura;
    SDL_Texture *imagem;
    SDL_FRect partes[3][3];
} Botao;

typedef struct Moldura
{
    SDL_FRect retangulo;
    SDL_Texture *textura;
    SDL_FRect partes[3][3];
} Moldura;

typedef struct VariveisGerais
{
    SDL_Window *janela;
    SDL_Renderer *renderizador;
    SDL_Event evento;
    SDL_Point ponto_mouse;
    int cena;
    int cena_passada;
    bool rodando;
    float mouse_x, mouse_y;
    bool botao_mouse_direito;
    bool botao_mouse_esquerdo;
    Player jogador;
} VariveisGerais;

typedef struct VariveisMenu
{
    SDL_Color cor_fundo;
    Moldura moldura;
    Botao botao_iniciar;
    Botao botao_conf;
    Botao botao_sair;
    SDL_Texture *imagem;
} VariveisMenu;

typedef struct VariveisPause
{
    SDL_Color cor_fundo;
    Moldura moldura;
    Botao botao_iniciar;
    Botao botao_conf;
    Botao botao_sair;
    SDL_Texture *imagem;
} VariveisPause;

typedef struct VariveisJogo
{
    SDL_Color cor_fundo;
    int velocidade_jogador_x;
    int velocidade_jogador_y;
} VariveisJogo;

typedef struct VariveisConf
{
    SDL_Color cor_fundo;
    Moldura moldura;
    Botao botao_sair;
    SDL_Texture *imagem;
} VariveisConf;

typedef struct TAMANHOS
{
    int escala;
    float tamanho_tela[2];
    float tamanho_menu[2];
    float tamanho_botao_menu[2];
    float tamanho_jogador[2];
    float tamanho_inimigo1[2];
    float tamanho_inimigo2[2];
    float tamanho_bloco[2];
} TAMANHOS;

Moldura InitMoldura(SDL_Renderer *renderer, SDL_FRect *retangulo, int tamanho_canto, char *file);
Botao InitBotao(SDL_Renderer *renderer, SDL_FRect *retangulo, char *file, char *texto, SDL_Color cor, SDL_Color cor2, int indice, TTF_Font *fonte, SDL_Color cor_fonte);
void CalcularBotaoParte(Botao *botao);
void GetTamanhos(TAMANHOS *tamanhos);
void DesenharMoldura(SDL_Renderer *renderer, Moldura moldura);
void DesenharBotao(SDL_Renderer *renderer, Botao botao);
void AtribuirFRectInRectA(SDL_FRect *fretangulo, SDL_Rect *retangulo);
void CentralizarRectInRect(SDL_FRect *rect_pai, SDL_FRect *rect_filho);
void CentralizarRectsInRectV(SDL_FRect *pai, SDL_FRect *filho[], int n, float borda_x, float borda_y);

void ModuloEvento(VariveisGerais *geral);
void InitGeral(VariveisGerais *geral, TAMANHOS *tamanhos);

void InitMenu(VariveisGerais *geral, VariveisMenu *menu, TAMANHOS tamanhos);
void CenaMenuLoop(VariveisGerais *geral, VariveisMenu *menu);
void CenaMenuDesenhar(VariveisGerais *geral, VariveisMenu *menu);

void InitPause(VariveisGerais *geral, VariveisPause *pause, TAMANHOS tamanhos);
void CenaPauseLoop(VariveisGerais *geral, VariveisPause *pause);
void CenaPauseDesenhar(VariveisGerais *geral, VariveisPause *pause);

void InitJogo(VariveisGerais *geral, VariveisJogo *jogo, TAMANHOS tamanhos);
void CenaJogoLoop(VariveisGerais *geral, VariveisJogo *jogo);
void CenaJogoDesenhar(VariveisGerais *geral, VariveisJogo *jogo);

void InitConf(VariveisGerais *geral, VariveisConf *conf, TAMANHOS tamanhos);
void CenaConfLoop(VariveisGerais *geral, VariveisConf *conf);
void CenaConfDesenhar(VariveisGerais *geral, VariveisConf *conf);

#endif // GERAIS_H_INCLUDED
