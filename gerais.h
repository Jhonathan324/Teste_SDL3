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
#define SEMI_PRETO {0, 0, 0, 128}
#define EscalaMoldura 16
#define EscalaBotao 8

// enuns
enum ESTADO_JOGO
{
    CENA_MENU,
    CENA_JOGO,
    CENA_PAUSE,
    CENA_CONF,
    CENA_CREDITOS,
    CENA_SAIR,
};

// elementos UI
typedef struct Moldura
{
    SDL_FRect retangulo;
    SDL_Texture *textura;
    SDL_FRect partes[3][3];
} Moldura;

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

typedef struct BotaoExpansivo
{
    int n;
    Botao botao_pai;
    Botao *botao_filho;
    bool expandido;
} BotaoExpansivo;

// Coisas mais especificas
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

// Structs abstratas
typedef struct Player
{
    int vida;
    int coracoes;
    float velocidade;
    SDL_FRect retangulo;
} Player;

// Structs especicas para organizar dados

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
    int resolucao_atual[2];
    bool fullscrean;
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
    BotaoExpansivo botao_reso;
    SDL_Texture *imagem;
} VariveisConf;

// Funções especificas
void GetTamanhos(TAMANHOS *tamanhos);

// Funções desnecessarias
void AtribuirFRectInRectA(SDL_FRect *fretangulo, SDL_Rect *retangulo);

// Funções para ajustes dinamicos
void CentralizarRectInRect(SDL_FRect *rect_pai, SDL_FRect *rect_filho);
void CentralizarRectsInRectV(SDL_FRect *pai, SDL_FRect *filho[], int n, float borda_x, float borda_y);
bool VerificarBotao(Botao *botao, SDL_Point mouse, bool click);

// Funções para criação de elementos dinamicos
Moldura InitMoldura(SDL_Renderer *renderer, SDL_FRect *retangulo, char *file);
void CalcularMolduraPartes(Moldura *moldura, float tamanhos_canto);
void DesenharMoldura(SDL_Renderer *renderer, Moldura moldura);

Botao InitBotao(SDL_Renderer *renderer, SDL_FRect *retangulo, char *imagem, char *texto, SDL_Color cor1, SDL_Color cor2, int indice, TTF_Font *fonte, SDL_Color cor_fonte);
void CalcularBotaoPartes(Botao *botao);
void DesenharBotao(SDL_Renderer *renderer, Botao botao);

BotaoExpansivo InitBotaoExpansivo(SDL_Renderer *renderer, SDL_FRect *retangulo, char *imagem, char *texto, char *textos[], SDL_Color cor, SDL_Color cor2, int indice, TTF_Font *fonte, SDL_Color cor_fonte, int n);
void CalcularBotaoExpansivoPartes(BotaoExpansivo *botao);
void DesenharBotaoExpansivo(SDL_Renderer *renderer, BotaoExpansivo botao);

// Funções só pra tratamento de eventos
void ModuloEvento(VariveisGerais *geral);
void InitGeral(VariveisGerais *geral, TAMANHOS *tamanhos);
void CalcularGeral(VariveisGerais *geral, TAMANHOS *tamanhos);

// Funções para cenas em especico
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
bool CenaConfLoop(VariveisGerais *geral, VariveisConf *conf, TAMANHOS *tamanhos);
void CenaConfDesenhar(VariveisGerais *geral, VariveisConf *conf);

#endif // GERAIS_H_INCLUDED
