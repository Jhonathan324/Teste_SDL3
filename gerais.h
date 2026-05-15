#ifndef GERAIS_H_INCLUDED
#define GERAIS_H_INCLUDED
#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include "desumano.h"

#define CORCORFUNDO {20, 20, 20, 255}
#define COR1 {20, 20, 20, 255}
#define PRETO {0, 0, 0, 255}
#define BRANCO {255, 255, 255, 255}
#define VERMELHO {255, 20, 20, 255}
#define VERDE {20, 255, 20, 255}
#define AZUL {20, 20, 255, 255}
#define AZUL_CLARO {140, 210, 230, 255}
#define SEMI_PRETO {0, 0, 0, 128}

#define EscalaHud 16
#define EscalaMoldura 16
#define EscalaMarcador 16
#define EscalaBotao 8
#define CantoFixo 48
#define MedidaImgPlayerX 120
#define MedidaImgPlayerY 80
#define MedidaImgPlayerColiX 21
#define MedidaImgPlayerColiY 38
#define MedidaImgInimigo1X 48
#define MedidaImgInimigo1Y 32
#define MedidaImgInimigo2X 120
#define MedidaImgInimigo2Y 80
#define MedidaImgBloco 16

// enuns
typedef enum {
    CENA_MENU,
    CENA_JOGO,
    CENA_PAUSE,
    CENA_CRIACAO,
    CENA_CONF,
    CENA_CREDITOS, // algum dia eu vou usar
    CENA_SAIR,
}ESTADO_JOGO;

// elementos UI
typedef struct CampoTexto{
    SDL_FRect retangulo;
    char *texto;
    SDL_Color cor_fundo;
    SDL_Texture *textura_texto;
    SDL_Texture *imagem;
    float proporcao;
} CampoTexto;


typedef struct Moldura
{
    SDL_FRect retangulo;
    SDL_Texture *textura;
    SDL_FRect partes[3][3];
} Moldura;

typedef struct Marcador{
    SDL_FRect retangulo;
    int timer;
    bool sobre;
    bool ativo;
    SDL_Color cor1;
    SDL_Color cor2;
    SDL_Texture *imagem1;
    SDL_FRect partes[3][3];
} Marcador;

typedef struct Botao
{
    SDL_FRect retangulo;
    char *texto;
    float proporcao;
    int timer;
    bool sobre;
    int indice;
    SDL_Color cor1;
    SDL_Color cor2;
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
typedef struct Tamanhos
{
    int escala;
    float tela[2];
    float menu[2];
    float botao1[2];
    float botao2[2];
    float barra_vida[2];
    float jogador[2];
    float jogador_coli[2];
    float inimigo1[2];
    float inimigo2[2];
    float bloco1[2];
} Tamanhos;

// Structs abstratas
typedef struct Player
{
    int vida;
    int coracoes;
} Player;

// Structs especicas para organizar dados

typedef struct VariveisGerais
{
    SDL_Window *janela;
    SDL_Renderer *renderizador;
    SDL_Event evento;
    SDL_Point ponto_mouse;
    SDL_Texture *textura_hud;
    SDL_FRect barra_de_vida;
    bool debug;
    bool rodando;
    bool fullscrean;
    bool troca_reso;
    int botao_mouse_direito;
    int botao_mouse_esquerdo;
    int botao_mouse_meio;
    int botao_mouse_gira;
    int resolucao_atual[2];
    int resolucao_antiga[2];
    float mouse_x, mouse_y;
    float mouse_x_back, mouse_y_back;
    Player jogador;
    ESTADO_JOGO cena;
    ESTADO_JOGO cena_passada;
    ESTADO_JOGO cena_continuar;
} VariveisGerais;

typedef struct VariveisMenu
{
    SDL_Color cor_fundo;
    SDL_Texture *imagem;
    Moldura moldura;
    Botao botao_iniciar;
    Botao botao_criacao;
    Botao botao_conf;
    Botao botao_sair;
} VariveisMenu;

typedef struct VariveisPause
{
    SDL_Color cor_fundo;
    SDL_Texture *imagem;
    Moldura moldura;
    Botao botao_iniciar;
    Botao botao_conf;
    Botao botao_sair;
} VariveisPause;



typedef struct VariveisConf
{
    SDL_Color cor_fundo;
    SDL_Texture *imagem;
    bool valida_fullscrean;
    int reso_inicial;
    Marcador troca_fullscreen;
    Botao botao_sair;
    BotaoExpansivo botao_reso;
    Moldura moldura;
    CampoTexto texto_reso;
    CampoTexto texto_full;
} VariveisConf;

// Funções especificas
void GetTamanhos(Tamanhos *tamanhos);

// Funções desnecessarias
void AtribuirFRectInRectA(SDL_FRect *fretangulo, SDL_Rect *retangulo);

// Funções para ajustes dinamicos
void CentralizarRectInRect(SDL_FRect *rect_pai, SDL_FRect *rect_filho);
void CentralizarRectsInRectV(SDL_FRect *pai, SDL_FRect *filho[], int n, float borda_x, float borda_y);

// Funções para criação de elementos dinamicos
CampoTexto InitTexto(SDL_Renderer *renderizador, SDL_FRect *retangulo, SDL_Color cor_fundo, char *texto, char *imagem, TTF_Font *fonte, SDL_Color cor_fonte, bool alinhado);
void DesenharTexto(SDL_Renderer *renderizador, CampoTexto texto);
void DestruirTexto(CampoTexto *texto);

Moldura InitMoldura(SDL_Renderer *renderizador, SDL_FRect *retangulo, char *file);
void CalcularMolduraPartes(Moldura *moldura, float tamanhos_canto);
void DesenharMoldura(SDL_Renderer *renderizador, Moldura moldura);
void DestruirMoldura(Moldura *moldura);

Marcador InitMarcador(SDL_Renderer *renderizador, SDL_FRect *retangulo, bool ativo, char *imagem1, SDL_Color cor1, SDL_Color cor2);
void DesenharMarcador(SDL_Renderer *renderizador, Marcador marcador);
bool VerificarMarcador(Marcador *marcador, SDL_Point mouse, bool click);
void DestruirMarcador(Marcador *marcador);

Botao InitBotao(SDL_Renderer *renderizador, SDL_FRect *retangulo, char *imagem, char *texto, SDL_Color cor1, SDL_Color cor2, int indice, TTF_Font *fonte, SDL_Color cor_fonte);
void CalcularBotaoPartes(Botao *botao);
void DesenharBotao(SDL_Renderer *renderizador, Botao botao);
bool VerificarBotao(Botao *botao, SDL_Point mouse, bool click);
void DestruirBotao(Botao *botao);

BotaoExpansivo InitBotaoExpansivo(SDL_Renderer *renderizador, SDL_FRect *retangulo, char *imagem, char *texto, char *textos[], SDL_Color cor, SDL_Color cor2, int indice, TTF_Font *fonte, SDL_Color cor_fonte, int n);
void CalcularBotaoExpansivoPartes(BotaoExpansivo *botao);
void DesenharBotaoExpansivo(SDL_Renderer *renderizador, BotaoExpansivo botao);
void DestruirBotaoExpansivo(BotaoExpansivo *botao);

// Funções só pra tratamento de eventos
void ModuloEvento(VariveisGerais *geral);
void InitCenaGeral(VariveisGerais *geral, Tamanhos *tamanhos);
void CalcularGeral(VariveisGerais *geral, Tamanhos *tamanhos);

// Funções para cenas em especico
void InitCenaMenu(VariveisGerais *geral, VariveisMenu *menu, Tamanhos tamanhos);
void LoopCenaMenu(VariveisGerais *geral, VariveisMenu *menu);
void DesenharCenaMenu(VariveisGerais geral, VariveisMenu menu);

void InitCenaPause(VariveisGerais *geral, VariveisPause *pause, Tamanhos tamanhos);
void LoopCenaPause(VariveisGerais *geral, VariveisPause *pause);
void DesenharCenaPause(VariveisGerais geral, VariveisPause pause);

void InitCenaConf(VariveisGerais *geral, VariveisConf *conf, Tamanhos tamanhos);
void LoopCenaConf(VariveisGerais *geral, VariveisConf *conf, Tamanhos *tamanhos);
void DesenharCenaConf(VariveisGerais geral, VariveisConf conf);

#endif // GERAIS_H_INCLUDED
