#ifndef JOGO_H_INCLUDED
#define JOGO_H_INCLUDED
#include "gerais.h"

#define TamanhosMapaX 512
#define TamanhosMapaY 128


//animação, quantidade de frames, tempo de execução
//VMM: vou me matar 
//VMMA: vou me matar amanha 

#define TabelaPlayerAnim \
		X(VMM_PLAYER_ATAQUE,            4) \
		X(VMM_PLAYER_ATAQUE2,           6) \
		X(VMM_PLAYER_ATAQUE2_MOVIMENTO, 6) \
		X(VMM_PLAYER_ATAQUE_COMBO,      10) \
		X(VMM_PLAYER_ATAQUE_MOVIMENTO_COMBO, 10) \
		X(VMM_PLAYER_ATAQUE_MOVIMENTO, 4) \
		X(VMM_PLAYER_AGACHAR,          1) \
		X(VMM_PLAYER_AGACHAR_TODO,     3) \
		X(VMM_PLAYER_AGACHAR_ATAQUE,   4) \
		X(VMM_PLAYER_LEVANTAR,         1) \
		X(VMM_PLAYER_CORRER_AGACHADO,  8) \
		X(VMM_PLAYER_DASH,             2) \
		X(VMM_PLAYER_MORRER,          10) \
		X(VMM_PLAYER_MORRER_CORRENDO, 10) \
		X(VMM_PLAYER_CAIR,             3) \
		X(VMM_PLAYER_HIT,              1) \
		X(VMM_PLAYER_IDLE,            10) \
		X(VMM_PLAYER_PULAR,            3) \
		X(VMM_PLAYER_PULO_TRANSICAO,   2) \
		X(VMM_PLAYER_ROLAR,           12) \
		X(VMM_PLAYER_CORRER,          10) \
		X(VMM_PLAYER_DESLIZAR,         2)\
		X(VMM_PLAYER_DESLIZAR_TODO,    4) \
		X(VMM_PLAYER_DESLIZAR_TRANSICAO_FIM, 1) \
		X(VMM_PLAYER_DESLIZAR_TRANSICAO_COMECO, 1) \
		X(VMM_PLAYER_INVERTER_PASSO,   3) \
		X(VMM_PLAYER_ESCALAR,          7) \
		X(VMM_PLAYER_ESCALAR_MOVIMENTO,7) \
		X(VMM_PLAYER_AGARRAR,          1) \
		X(VMM_PLAYER_DESLIZAR_PAREDE,  3) 

#define TabelaPorcoAnim \
		X(VMM_PORCO_ANDAR,   6) \
		X(VMM_PORCO_CORRER,  6) \
		X(VMM_PORCO_IDLE,    4) \
		X(VMM_PORCO_HIT,     4) \

typedef enum{
	PORCO_NORMAL,
	ABELHA,
	CARACOL,
} TiposInimigo;

typedef enum {
		VMMA_SEM_COLI,
		VMMA_GRAMA_ON,
		VMMA_PEDRA_ON,
} TiposVMMA;


#define TabelaBlocoAtlas \
		X(VMM_BLOCO_GRAMA0x0, 0, 0, VMMA_SEM_COLI) \
		X(VMM_BLOCO_GRAMA0x1, 1, 0, VMMA_SEM_COLI) \
		X(VMM_BLOCO_GRAMA0x2, 2, 0, VMMA_SEM_COLI) \
		X(VMM_BLOCO_GRAMA0x3, 3, 0, VMMA_SEM_COLI) \
		X(VMM_BLOCO_GRAMA0x4, 4, 0, VMMA_SEM_COLI) \
		\
		X(VMM_BLOCO_GRAMA1x0, 0, 1, VMMA_GRAMA_ON) \
		X(VMM_BLOCO_GRAMA1x1, 1, 1, VMMA_GRAMA_ON) \
		X(VMM_BLOCO_GRAMA1x2, 2, 1, VMMA_GRAMA_ON) \
		X(VMM_BLOCO_GRAMA1x3, 3, 1, VMMA_GRAMA_ON) \
		X(VMM_BLOCO_GRAMA1x4, 4, 1, VMMA_GRAMA_ON) \
		\
		X(VMM_BLOCO_GRAMA2x0, 0, 2, VMMA_GRAMA_ON) \
		X(VMM_BLOCO_GRAMA2x1, 1, 2, VMMA_GRAMA_ON) \
		X(VMM_BLOCO_GRAMA2x2, 2, 2, VMMA_GRAMA_ON) \
		X(VMM_BLOCO_GRAMA2x3, 3, 2, VMMA_GRAMA_ON) \
		X(VMM_BLOCO_GRAMA2x4, 4, 2, VMMA_GRAMA_ON) \
		\
		X(VMM_BLOCO_GRAMA3x0, 0, 3, VMMA_GRAMA_ON) \
		X(VMM_BLOCO_GRAMA3x1, 1, 3, VMMA_GRAMA_ON) \
		X(VMM_BLOCO_GRAMA3x2, 2, 3, VMMA_GRAMA_ON) \
		X(VMM_BLOCO_GRAMA3x3, 3, 3, VMMA_GRAMA_ON) \
		X(VMM_BLOCO_GRAMA3x4, 4, 3, VMMA_GRAMA_ON) \
		\
		X(VMM_BLOCO_GRAMA4x0, 0, 4, VMMA_GRAMA_ON) \
		X(VMM_BLOCO_GRAMA4x1, 1, 4, VMMA_GRAMA_ON) \
		X(VMM_BLOCO_GRAMA4x2, 2, 4, VMMA_GRAMA_ON) \
		X(VMM_BLOCO_GRAMA4x3, 3, 4, VMMA_GRAMA_ON) \
		X(VMM_BLOCO_GRAMA4x4, 4, 4, VMMA_GRAMA_ON) \
		\
		\
		X(VMM_BLOCO_PEDRA0x0, 0, 5, VMMA_SEM_COLI) \
		X(VMM_BLOCO_PEDRA0x1, 1, 5, VMMA_SEM_COLI) \
		X(VMM_BLOCO_PEDRA0x2, 2, 5, VMMA_SEM_COLI) \
		X(VMM_BLOCO_PEDRA0x3, 3, 5, VMMA_SEM_COLI) \
		X(VMM_BLOCO_PEDRA0x4, 4, 5, VMMA_SEM_COLI) \
		\
		X(VMM_BLOCO_PEDRA1x0, 0, 6, VMMA_PEDRA_ON) \
		X(VMM_BLOCO_PEDRA1x1, 1, 6, VMMA_SEM_COLI) \
		X(VMM_BLOCO_PEDRA1x2, 2, 6, VMMA_SEM_COLI) \
		X(VMM_BLOCO_PEDRA1x3, 3, 6, VMMA_SEM_COLI) \
		X(VMM_BLOCO_PEDRA1x4, 4, 6, VMMA_PEDRA_ON) \
		\
		X(VMM_BLOCO_PEDRA2x0, 0, 7, VMMA_PEDRA_ON) \
		X(VMM_BLOCO_PEDRA2x1, 1, 7, VMMA_SEM_COLI) \
		X(VMM_BLOCO_PEDRA2x2, 2, 7, VMMA_SEM_COLI) \
		X(VMM_BLOCO_PEDRA2x3, 3, 7, VMMA_SEM_COLI) \
		X(VMM_BLOCO_PEDRA2x4, 4, 7, VMMA_PEDRA_ON) \
		\
		X(VMM_BLOCO_PEDRA3x0, 0, 8, VMMA_PEDRA_ON) \
		X(VMM_BLOCO_PEDRA3x1, 1, 8, VMMA_SEM_COLI) \
		X(VMM_BLOCO_PEDRA3x2, 2, 8, VMMA_SEM_COLI) \
		X(VMM_BLOCO_PEDRA3x3, 3, 8, VMMA_SEM_COLI) \
		X(VMM_BLOCO_PEDRA3x4, 4, 8, VMMA_PEDRA_ON) \
		\
		X(VMM_BLOCO_PEDRA4x0, 0, 9, VMMA_PEDRA_ON) \
		X(VMM_BLOCO_PEDRA4x1, 1, 9, VMMA_PEDRA_ON) \
		X(VMM_BLOCO_PEDRA4x2, 2, 9, VMMA_PEDRA_ON) \
		X(VMM_BLOCO_PEDRA4x3, 3, 9, VMMA_PEDRA_ON) \
		X(VMM_BLOCO_PEDRA4x4, 4, 9, VMMA_PEDRA_ON) \
		\
		\
		X(VMM_BLOCO_PEDRA_MUSGO0x0, 0, 10, VMMA_SEM_COLI) \
		X(VMM_BLOCO_PEDRA_MUSGO0x1, 1, 10, VMMA_SEM_COLI) \
		\
		X(VMM_BLOCO_PEDRA_MUSGO1x0, 0, 11, VMMA_PEDRA_ON) \
		X(VMM_BLOCO_PEDRA_MUSGO1x1, 1, 11, VMMA_PEDRA_ON) \
		\
		\
		X(VMM_BLOCO_PEDRA_LATERAL0x0, 2, 10, VMMA_SEM_COLI) \
		X(VMM_BLOCO_PEDRA_LATERAL0x1, 3, 10, VMMA_SEM_COLI) \
		\
		X(VMM_BLOCO_PEDRA_LATERAL1x0, 2, 11, VMMA_PEDRA_ON) \
		X(VMM_BLOCO_PEDRA_LATERAL1x1, 3, 11, VMMA_PEDRA_ON) \




#define X(index, quant) index,
typedef enum {
	 TabelaPlayerAnim
} ESTADO_PLAYER;

typedef enum {
	 TabelaPorcoAnim
} ESTADO_PORCO;

#undef X

#define X(index, x_loc, y_loc, tipo) index,

typedef enum {
		TabelaBlocoAtlas
} IndexImgBloco;

#undef X



typedef struct PlayerInJogo
{
		int coracoes;
		int estado_atual;
		int estado_passado;
		int pulo;
		int coyote_time;
		bool costas;
		bool coli_h;
		bool coli_v;
		float vida;
		double frame;
		double acelera;
		double vel_max_x;
		double vel_max_y;
		double velocidade_x;
		double velocidade_y;
		double posicao_x;
		double posicao_y;
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
	double frame;
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
		uint8_t tiles[TamanhosMapaY][TamanhosMapaX];
		SDL_Texture *textura;
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
void DesenharCenaJogo(VariveisGerais geral, VariveisJogo jogo, Tamanhos tamanhos);


void DesenharBloco(SDL_Renderer *renderizador, Bloco bloco);
void DesenharMapa(SDL_Renderer *renderizador, Mapa mapa, Camera camera, int tamanho_bloco[2], int tamanhos_tela[2]);
void DesenharHud(VariveisGerais geral, VariveisJogo jogo, Tamanhos tamanhos);
SDL_FRect MapaTiles(int n);

PlayerInJogo InitPlayer(SDL_Renderer *renderizador, SDL_FRect retangulo_img, SDL_Rect retangulo_coli,  char *img);
void CalcularPlayer(const bool *teclado, PlayerInJogo *player, double delta_frame, Camera *camera, Mapa mapa, int tamanho_bloco[2], int tamanhos_tela[2]);
void DesenharPlayer(SDL_Renderer *renderizador, PlayerInJogo player, Camera camera);

Inimigo InitInimigo(SDL_Renderer *renderizador, SDL_FRect retangulo_img, SDL_Rect retangulo_area, SDL_Rect retangulo_coli,  int index);
void CalcularInimigo(Inimigo *inimigo, double delta_frame, Camera *camera, Mapa mapa, int tamanho_bloco[2], int tamanhos_tela[2]);
void DesenharInimigo(SDL_Renderer *renderizador, Inimigo inimigo, SDL_Texture *sprite_atlas, Camera camera );


TiposVMMA CalcularTipoVMMA(int n);
void ColisaoPlayerMapaH(PlayerInJogo *jogador, Mapa Mapa, int tamanho_bloco[2], int tamanho_tela[2], Camera camera);
void ColisaoPlayerMapaV(PlayerInJogo *jogador, Mapa Mapa, int tamanho_bloco[2], int tamanho_tela[2], Camera camera);

void ColisaoInimigoMapaH(Inimigo *inimigo, Mapa mapa, int tamanho_bloco[2], int tamanho_tela[2],Camera camera);
void ColisaoInimigoMapaV(Inimigo *inimigo, Mapa mapa, int tamanho_bloco[2], int tamanho_tela[2],Camera camera);


//Criação de Mapa
void SalvarMapa(Mapa *c);
void CarregarMapa(Mapa *c, int n);

#endif // JOGO_H_INCLUDED