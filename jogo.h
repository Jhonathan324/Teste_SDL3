#ifndef JOGO_H_INCLUDED
#define JOGO_H_INCLUDED
#include "gerais.h"

#define TamanhosMapaX 512
#define TamanhosMapaY 128


//animação, quantidade de frames, tempo de execução
//VMM: vou me matar 
//VMMA: vou me matar amanha 






typedef struct PlayerInJogo
{
	int coracoes;
	int estado_atual;
	int estado_passado;
	int coyote_time;
	bool costas;
	bool coli_h;
	bool coli_v;
	float vida;
	float dano;
	float dano_sofrido;
	double frame;
	double pulo;
	double ataque;
	double tempo_hit;
	double tempo_safe;
	double acelera;
	double vel_max_x;
	double vel_max_y;
	double velocidade_x;
	double velocidade_y;
	double posicao_x;
	double posicao_y;
	SDL_Rect retangulo_dano;
	SDL_Rect retangulo_coli;
	SDL_Rect retangulo_coli_h;
	SDL_Rect retangulo_coli_v;
	SDL_FRect retangulo_img;
	SDL_Texture *sprite_atlas;
}PlayerInJogo;

typedef struct Inimigo
{
	int estado_atual;
	int estado_passado;
	int pulo;
	int coyote_time;
	bool costas;
	bool coli_h;
	bool coli_v;
	float vida;
	float dano;
	double frame;
	double tempo_safe;
	double acelera;
	double vel_max_x;
	double vel_max_y;
	double velocidade_x;
	double velocidade_y;
	double posicao_x;
	double posicao_y;
	SDL_Rect retangulo_area;
	SDL_Rect retangulo_coli;
	SDL_Rect retangulo_coli_h;
	SDL_Rect retangulo_coli_v;
	SDL_FRect retangulo_img;
	TiposInimigo index;
}Inimigo;

typedef struct Bloco{
	SDL_Texture *textura;
	SDL_FRect loc;
	SDL_FRect retangulo;
} Bloco;

typedef struct Mapa{
	int n;
	uint16_t tiles[TamanhosMapaY][TamanhosMapaX];
	SDL_Texture *textura;
	SDL_Rect area_vitoria;
} Mapa;

typedef struct Camera {
	int x;
	int y;
} Camera;

typedef struct VariveisJogo
{
	SDL_Color cor_fundo;
	SDL_Texture *sprite_atlas_inimigos[3];

	PlayerInJogo jogador;
	Inimigo inimigos[10];
	Camera camera;
	Mapa mapa;

	int tamanho_bloco[2];
	double tempo;
} VariveisJogo;



void InitCenaJogo    (VariveisGerais *geral, VariveisJogo *jogo, Tamanhos tamanhos);
void CalcularCenaJogo(VariveisGerais *geral, VariveisJogo *jogo, Tamanhos tamanhos);
void LoopCenaJogo    (VariveisGerais *geral, VariveisJogo *jogo, double delta_t);
void DesenharCenaJogo(VariveisGerais  geral, VariveisJogo  jogo, Tamanhos tamanhos);


void DesenharBloco(SDL_Renderer *renderizador, Bloco bloco);
void DesenharMapa(SDL_Renderer *renderizador, Mapa mapa, Camera camera, int tamanho_bloco[2], int tamanhos_tela[2]);
void DesenharHud(VariveisGerais geral, VariveisJogo jogo, Tamanhos tamanhos);
SDL_FRect MapaTiles(int n);

PlayerInJogo InitPlayer(SDL_Renderer *renderizador, SDL_FRect retangulo_img, SDL_Rect retangulo_coli,  char *img, float vida, int coracoes);
void CalcularPlayer(const bool *teclado, PlayerInJogo *player, double delta_frame, Camera *camera, Mapa mapa, int tamanho_bloco[2], int tamanhos_tela[2]);
void DesenharPlayer(SDL_Renderer *renderizador, PlayerInJogo player, Camera camera);

Inimigo InitInimigo(SDL_Renderer *renderizador, SDL_FRect retangulo_img, SDL_Rect retangulo_area, SDL_Rect retangulo_coli, float vida, float dano, int index);
void CalcularInimigo(Inimigo *inimigo, double delta_frame, Camera *camera, Mapa mapa, int tamanho_bloco[2], int tamanhos_tela[2]);
void DesenharInimigo(SDL_Renderer *renderizador, Inimigo inimigo, SDL_Texture *sprite_atlas, Camera camera );


TiposVMMA CalcularTipoVMMA(int n);
void ColisaoPlayerMapaH(PlayerInJogo *jogador, Mapa Mapa, int tamanho_bloco[2], int tamanho_tela[2], Camera camera);
void ColisaoPlayerMapaV(PlayerInJogo *jogador, Mapa Mapa, int tamanho_bloco[2], int tamanho_tela[2], Camera camera);

void ColisaoInimigoMapaH(Inimigo *inimigo, Mapa mapa, int tamanho_bloco[2], int tamanho_tela[2],Camera camera);
void ColisaoInimigoMapaV(Inimigo *inimigo, Mapa mapa, int tamanho_bloco[2], int tamanho_tela[2],Camera camera);

void ColisaoPlayerInimigo(PlayerInJogo *jogador, Inimigo *Inimigo);


//Criação de Mapa
void SalvarMapa(Mapa *c);
void CarregarMapa(Mapa *c, int n);

#endif // JOGO_H_INCLUDED