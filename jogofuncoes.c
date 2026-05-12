#include "jogo.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


PlayerInJogo InitPlayer(SDL_Renderer *renderizador, SDL_FRect retangulo_img, SDL_Rect retangulo_coli,  char *img){
	PlayerInJogo player = {
		.coracoes         = 3,
		.estado_atual     = 0,
		.estado_passado   = 0,
		.costas           = 0,
		.coli_h           = 0,
		.coli_v           = 0,
		.vida             = 100.0,
		.frame            = 0,
		.acelera          = (float)retangulo_coli.w/100,
		.vel_max_x        = (float)retangulo_coli.w/48,
		.vel_max_y        = (float)retangulo_coli.h/48,
		.velocidade_x     = 0,
		.velocidade_y     = 0,
		.posicao_x        = retangulo_coli.x,
		.posicao_y        = retangulo_coli.y,
		.retangulo_coli   = retangulo_coli,
		.retangulo_coli_h = retangulo_coli,
		.retangulo_coli_v = retangulo_coli,
		.retangulo_img    = retangulo_img,
		.sprite_atlas     = IMG_LoadTexture(renderizador, img),
	};
	
	SDL_SetTextureScaleMode(player.sprite_atlas, SDL_SCALEMODE_NEAREST);
	return player;
}

void CalcularPlayer(const bool *teclado, PlayerInJogo *player, double delta_frame, Camera *camera, Mapa mapa, int tamanho_bloco[2], int tamanhos_tela[2]){
	double movi_v = true, movi_h = 0;
	player->frame += delta_frame;

	// Logica do Player
	movi_h = (teclado[SDL_SCANCODE_D] - teclado[SDL_SCANCODE_A]);


	if(movi_h<0) player->costas = true;
	if(movi_h>0) player->costas = false;

	if(!movi_h) player->velocidade_x = 0;

	// É necessario divirdir em dois para não ser possivel bugar nas quinas do blocos
	ColisaoPlayerMapaV(player, mapa, tamanho_bloco, tamanhos_tela, *camera);
	if(player->coli_v){
		movi_v = false;
		player->velocidade_y = false;
		player->retangulo_coli_v = player->retangulo_coli;
		player->posicao_y = player->retangulo_coli.y;
	}
	player->retangulo_coli.y = player->retangulo_coli_v.y;
	player->retangulo_coli_v = player->retangulo_coli;
	player->retangulo_coli_h.y = player->retangulo_coli_v.y;
	
	ColisaoPlayerMapaH(player, mapa, tamanho_bloco, tamanhos_tela, *camera);
	if(player->coli_h){
		player->velocidade_x = false;
		player->retangulo_coli_h = player->retangulo_coli;
		player->posicao_x = player->retangulo_coli.x;
	}
	else player->velocidade_x += player->acelera * delta_frame * movi_h;
	player->retangulo_coli.x = player->retangulo_coli_h.x;
	player->retangulo_coli_h = player->retangulo_coli;
	player->retangulo_coli_v.x = player->retangulo_coli.x;

	if(!player->coli_v) {
		if(player->pulo) {
			if(player->estado_atual != VMM_PLAYER_PULAR && player->estado_atual != VMM_PLAYER_PULO_TRANSICAO && player->estado_atual != VMM_PLAYER_CAIR){
				player->frame=0;
				player->estado_passado = player->estado_atual;
				player->estado_atual = VMM_PLAYER_PULAR;
			}
			else if(player->estado_atual == VMM_PLAYER_PULAR && player->pulo < 4){
				player->frame=0;
				player->estado_passado = player->estado_atual;
				player->estado_atual = VMM_PLAYER_PULO_TRANSICAO;
			}
			else{
				player->estado_passado = player->estado_atual;
			}
			player->velocidade_y -= player->acelera * delta_frame * 20;
			player->pulo--;
		}
		else if(player->coyote_time > 1){
			player->frame=0;
			player->estado_passado = player->estado_atual;
			player->estado_atual = VMM_PLAYER_CAIR;
		}
		else{
			player->estado_passado = player->estado_atual;
		}
		if(player->estado_atual != VMM_PLAYER_CAIR) player->coyote_time++;
		player->velocidade_y += player->acelera * delta_frame * movi_v;
	}
	else{
		player->coyote_time = false;
		if(teclado[SDL_SCANCODE_SPACE] && player->estado_atual && !player->pulo){
			player->pulo = 20;
		}
		if(movi_h){ 
			if (player->estado_passado != VMM_PLAYER_CORRER){
					player->frame=0;
			}
			player->estado_passado = player->estado_atual;
			player->estado_atual = VMM_PLAYER_CORRER;
		}
		else{
			player->estado_passado = player->estado_atual;
			player->estado_atual = VMM_PLAYER_IDLE;
		}
	}
	

	

	// vericando para a velocidade não passar do maximo
	if (player->velocidade_x < player->vel_max_x*-1)
		player->velocidade_x = player->vel_max_x*-1;

	if (player->velocidade_x > player->vel_max_x)
		player->velocidade_x = player->vel_max_x;

	if (player->velocidade_y < player->vel_max_y*-1)
		player->velocidade_y = player->vel_max_y*-1;

	if (player->velocidade_y > player->vel_max_y)
		player->velocidade_y = player->vel_max_y;

	//printf("%d\n",player->retangulo_coli_h.x);
	
	player->posicao_x += player->velocidade_x * delta_frame;
	player->posicao_y += player->velocidade_y * delta_frame;

	player->retangulo_coli_h.x = player->posicao_x;
	player->retangulo_coli_v.y = player->posicao_y;


	player->retangulo_img.y = player->retangulo_coli.y + player->retangulo_coli.h-player->retangulo_img.h - camera->y;
	player->retangulo_img.x = player->retangulo_coli.x -  (player->retangulo_img.w * ((float)44/MedidaImgPlayerX)) - (player->costas)*(player->retangulo_img.w * ((float)11/MedidaImgPlayerX)) - camera->x; 
	player->coli_v = false;
	player->coli_h = false;
}

void DesenharPlayer(SDL_Renderer *renderizador, PlayerInJogo player, Camera camera ){
	// se tiver em teste eu não comento
	SDL_SetRenderDrawColor(renderizador, 255, 0, 0, 255);
	SDL_RenderFillRect(renderizador, &(SDL_FRect){
		player.retangulo_coli.x-camera.x,
		player.retangulo_coli.y-camera.y,
		player.retangulo_coli.w,
		player.retangulo_coli.h});

	Uint64 frame_atual = player.frame / 10; 
	#define X(index,quant) \
		case(index):{ \
		SDL_RenderTextureRotated(renderizador, player.sprite_atlas,&(SDL_FRect){MedidaImgPlayerX*(frame_atual%quant),MedidaImgPlayerY*index,MedidaImgPlayerX,MedidaImgPlayerY}, \
		&player.retangulo_img,0,0, player.costas ? SDL_FLIP_HORIZONTAL: 0);}break;

	switch(player.estado_atual){
		TabelaPlayerAnim
	}
	#undef X
}

void DesenharBloco(SDL_Renderer *renderizador, Bloco bloco){
	SDL_RenderTexture(renderizador, bloco.textura, &bloco.loc, &bloco.retangulo);

}

void ColisaoPlayerMapaV(PlayerInJogo *jogador, Mapa mapa, int tamanho_bloco[2], int tamanho_tela[2],Camera camera){
	for(int i = camera.y/tamanho_bloco[1]; i*tamanho_bloco[1] < tamanho_tela[1] + camera.y && i < TamanhosMapaY; i++){
		for(int j = camera.x/tamanho_bloco[0]; j*tamanho_bloco[0] < tamanho_tela[0] + camera.x && j < TamanhosMapaX; j++){
			if(mapa.tiles[i][j]){ 
				TiposVMMA tipo_de_coli = CalcularTipoVMMA(mapa.tiles[i][j]);
				switch (tipo_de_coli){
					case VMMA_GRAMA_ON:
					case VMMA_PEDRA_ON:{
							SDL_Rect retangulo = {j*tamanho_bloco[0], i*tamanho_bloco[1], tamanho_bloco[0], tamanho_bloco[1]};
							if(SDL_HasRectIntersection(&retangulo, &jogador->retangulo_coli_v))
									jogador->coli_v = true;
					}break;

				}

			}
		}
	}
}

void ColisaoPlayerMapaH(PlayerInJogo *jogador, Mapa mapa, int tamanho_bloco[2], int tamanho_tela[2],Camera camera){
	for(int i = camera.y/tamanho_bloco[1]; i*tamanho_bloco[1] < tamanho_tela[1] + camera.y && i < TamanhosMapaY; i++){
		for(int j = camera.x/tamanho_bloco[0]; j*tamanho_bloco[0] < tamanho_tela[0] + camera.x && j < TamanhosMapaX; j++){
			if(mapa.tiles[i][j]){ 
				TiposVMMA tipo_de_coli = CalcularTipoVMMA(mapa.tiles[i][j]);
				switch (tipo_de_coli){
					case VMMA_GRAMA_ON:
					case VMMA_PEDRA_ON:{
							SDL_Rect retangulo = {j*tamanho_bloco[0], i*tamanho_bloco[1], tamanho_bloco[0], tamanho_bloco[1]};
							if(SDL_HasRectIntersection(&retangulo, &jogador->retangulo_coli_h))
									jogador->coli_h = true;
					}break;

				}
			}
		}
	}
}


void DesenharMapa(SDL_Renderer *renderizador, Mapa mapa, Camera camera, int tamanho_bloco[2], int tamanho_tela[2]){
	for(int i = camera.y/tamanho_bloco[1]; i*tamanho_bloco[1] < tamanho_tela[1] + camera.y && i < TamanhosMapaY; i++){
		for(int j = camera.x/tamanho_bloco[0]; j*tamanho_bloco[0] < tamanho_tela[0] + camera.x && j < TamanhosMapaX; j++){
			if(mapa.tiles[i][j]){ 
				SDL_FRect src = MapaTiles(mapa.tiles[i][j]);
				SDL_RenderTexture(renderizador, mapa.textura, &src , &(SDL_FRect){j*tamanho_bloco[0] - camera.x, i*tamanho_bloco[1] - camera.y, tamanho_bloco[0],tamanho_bloco[1]});
			}
		}
	}
}

SDL_FRect MapaTiles(int n){
	SDL_FRect rect = {
		.w = MedidaImgBloco,
		.h = MedidaImgBloco
	};

	#define X(index, x_loc, y_loc, tipo) \
		case(index):{ \
			rect.x = x_loc * MedidaImgBloco; \
			rect.y = y_loc * MedidaImgBloco;  \
		}break; 

	switch(n-1){
		TabelaBlocoAtlas
	}

	#undef X

	return rect;
}

TiposVMMA CalcularTipoVMMA(int n){
	TiposVMMA x;
	#define X(index, x_loc, y_loc, tipo) \
	case(index):{ \
		x = tipo; \
	}break; 

	switch(n-1){
		TabelaBlocoAtlas
	}

	#undef X

	return x;
}