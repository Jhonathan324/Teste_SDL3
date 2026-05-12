#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "mapa.h"


// 🔹 Salvar Mapa
void SalvarMapa(Mapa *c) {
	char nome[64];
	sprintf(nome, "map/map_%d.bin", c->n);

	FILE *f = fopen(nome, "wb");
	if (!f) {
		printf("nao foi possivel salvar a Mapa\n");
		return;
	}
	printf("mapa salvo\n");
	fwrite(c->tiles, sizeof(uint8_t), TamanhosMapaX * TamanhosMapaY, f);
	fclose(f);
}


void CarregarMapa(Mapa *c, int n) {
	c->n = n;

	char nome[64];
	sprintf(nome, "map/map_%d.bin", c->n);

	FILE *f = fopen(nome, "rb");

	if (f) {
		fread(c->tiles, sizeof(uint8_t), TamanhosMapaX * TamanhosMapaY, f);
		fclose(f);
		printf("mapa %d carregado do disco\n", n);
	} else {
		printf("mapa %d gerado\n", n);
		memset(c->tiles,0,sizeof(c->tiles));
		SalvarMapa(c);
	}
}

void DesenharMapaGrid(SDL_Renderer *renderizador, Mapa mapa, Camera camera, int tamanho_bloco[2], int tamanho_tela[2], SDL_Point ponto_mouse){
	for(int i = camera.y/tamanho_bloco[1]; i*tamanho_bloco[1] < tamanho_tela[1] + camera.y && i < TamanhosMapaY; i++){
		for(int j = camera.x/tamanho_bloco[0]; j*tamanho_bloco[0] < tamanho_tela[0] + camera.x && j < TamanhosMapaX; j++){
			if(mapa.tiles[i][j]){ 
				SDL_FRect src = MapaTiles(mapa.tiles[i][j]);
				SDL_RenderTexture(renderizador, mapa.textura, &src , &(SDL_FRect){j*tamanho_bloco[0] - camera.x, i*tamanho_bloco[1] - camera.y, tamanho_bloco[0],tamanho_bloco[1]});
			}
		}
	}
}

void InitCenaMapa(SDL_Renderer *renderizador, VariaveisMapa *mapa, TAMANHOS tamanhos){
	// Mapa mapa
	mapa->mapa.n = 0;
	CarregarMapa(&mapa->mapa, 0);
	mapa->mapa.textura = IMG_LoadTexture(renderizador,"C:/Users/17898981/projetos/Teste_SDL3/assets/images/world/tiles/Tiles.png");
	SDL_SetTextureScaleMode(mapa->mapa.textura, SDL_SCALEMODE_NEAREST);

	// Mapa variaveis
	mapa->camera = (Camera){0,0};
	mapa->rolada = 7*5;
	mapa->velocidade = 1;
	mapa->tamanho_bloco[0] = tamanhos.tamanho_bloco1[0];
	mapa->tamanho_bloco[1] = tamanhos.tamanho_bloco1[1];
	mapa->selecao.w = tamanhos.tamanho_bloco1[0];
	mapa->selecao.h = tamanhos.tamanho_bloco1[1];

	mapa->selecao_coli.w = tamanhos.tamanho_bloco1[0];
	mapa->selecao_coli.h = tamanhos.tamanho_bloco1[1];
}

void LoopCenaMapa(VariveisGerais *geral, VariaveisMapa *mapa){
	//mouse
	SDL_GetMouseState(&geral->mouse_x, &geral->mouse_y);
    geral->ponto_mouse.x = geral->mouse_x;
    geral->ponto_mouse.y = geral->mouse_y;

	//teclado
	const bool *teclado = SDL_GetKeyboardState(NULL);
	if (teclado[SDL_SCANCODE_ESCAPE])
        geral->cena = CENA_PAUSE;

	int aumentar = (teclado[SDL_SCANCODE_UP] - teclado[SDL_SCANCODE_DOWN]);
	mapa->velocidade += aumentar;
	int movimento_h = (teclado[SDL_SCANCODE_D] - teclado[SDL_SCANCODE_A]) * mapa->velocidade;
	int movimento_v = (teclado[SDL_SCANCODE_S] - teclado[SDL_SCANCODE_W]) * mapa->velocidade;

	mapa->camera.x += movimento_h;
	mapa->camera.y += movimento_v;

	mapa->selecao.x = (geral->ponto_mouse.x/mapa->tamanho_bloco[0]) * mapa->tamanho_bloco[0] - (mapa->camera.x % mapa->tamanho_bloco[0]);
	mapa->selecao.y = (geral->ponto_mouse.y/mapa->tamanho_bloco[1]) * mapa->tamanho_bloco[1] - (mapa->camera.y % mapa->tamanho_bloco[1]);

	mapa->selecao_coli.x = mapa->selecao.x;
	mapa->selecao_coli.y = mapa->selecao.y;
}


void DesenharCenaMapa(VariveisGerais geral, VariaveisMapa mapa){
 	// Limpeza de Tela
    SDL_SetRenderDrawColor(geral.renderizador, mapa.cor_fundo.r, mapa.cor_fundo.g, mapa.cor_fundo.b, mapa.cor_fundo.a);
    SDL_RenderClear(geral.renderizador);

    // Elementos
    DesenharMapaGrid(geral.renderizador, mapa.mapa, mapa.camera, mapa.tamanho_bloco, geral.resolucao_atual, geral.ponto_mouse);
	SDL_Color cor = SEMI_PRETO;
	SDL_SetRenderDrawColor(geral.renderizador, cor.r, cor.g, cor.b, cor.a);
	SDL_RenderFillRect(geral.renderizador, &mapa.selecao);
}
