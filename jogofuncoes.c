#include "jogo.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


void DesenharHud(VariveisGerais geral, VariveisJogo jogo, Tamanhos tamanhos){
	SDL_FRect barra = {CantoFixo/3,CantoFixo/3,tamanhos.barra_vida[0],tamanhos.barra_vida[1]};
	SDL_RenderTexture(
		geral.renderizador, 
		geral.textura_hud, 
		&geral.barra_de_vida, 
		&barra);

	SDL_FRect vida  = geral.barra_de_vida;
	vida.y = EscalaHud * 5;
	SDL_RenderTexture(
		geral.renderizador, 
		geral.textura_hud, 
		&vida, 
		&barra);

	vida.y = EscalaHud * 7;
	vida.w *= (jogo.jogador.vida/100);
	barra.w *= (jogo.jogador.vida/100);
	SDL_RenderTexture(
		geral.renderizador, 
		geral.textura_hud, 
		&vida, 
		&barra);
}


PlayerInJogo InitPlayer(SDL_Renderer *renderizador, SDL_FRect retangulo_img, SDL_Rect retangulo_coli,  char *img, float vida, int coracoes){
	PlayerInJogo player = {
		.coracoes         = coracoes,
		.estado_atual     = 0,
		.estado_passado   = 0,
		.costas           = 0,
		.coli_h           = 0,
		.coli_v           = 0,
		.vida             = vida,
		.dano_sofrido     = 0,
		.frame            = 0,
		.tempo_safe       = 0,
		.acelera          = (float)retangulo_coli.w/100,
		.vel_max_x        = (float)retangulo_coli.w/32,
		.vel_max_y        = (float)retangulo_coli.h/32,
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
			player->pulo-= delta_frame;
			if(player->pulo < 0 ) player->pulo = 0;
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
			player->pulo = 40;
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
	if(player->dano_sofrido){
		if(player->dano_sofrido > 0){
			printf("frame: %f\n", delta_frame);
			printf("tempo: %d\n", player->tempo_safe);
			
			if( player->tempo_safe < delta_frame){
				player->tempo_safe = 0;
				if(player->dano_sofrido > delta_frame){
					player->dano_sofrido -= delta_frame;
					player->vida -= delta_frame;
					player->tempo_safe -= delta_frame;
				}
				else{
					player->vida-=player->dano_sofrido;
					player->dano_sofrido = 0;
				}
			}
			else{
				player->tempo_safe -= delta_frame;
				player->vida-=player->dano_sofrido;
				player->dano_sofrido = 0;
			}
		}
		else{
			player->dano_sofrido=0;
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


	/* se tiver em teste eu não comento
	SDL_SetRenderDrawColor(renderizador, 255, 0, 0, 255);
	SDL_RenderFillRect(renderizador, &(SDL_FRect){
		player.retangulo_coli.x-camera.x,
		player.retangulo_coli.y-camera.y,
		player.retangulo_coli.w,
		player.retangulo_coli.h});
	*/

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

Inimigo InitInimigo(SDL_Renderer *renderizador, SDL_FRect retangulo_img, SDL_Rect retangulo_area, SDL_Rect retangulo_coli, float dano, int index){
	Inimigo inimigo = {
		.index			  = index,
		.estado_atual     = 0,
		.estado_passado   = 0,
		.costas           = 0,
		.coli_h           = 0,
		.coli_v           = 0,
		.vida             = 100.0,
		.dano             = dano,
		.frame            = 0,
		.acelera          = (float)retangulo_coli.w/100,
		.vel_max_x        = (float)retangulo_coli.w/48,
		.vel_max_y        = (float)retangulo_coli.h/48,
		.velocidade_x     = 0,
		.velocidade_y     = 0,
		.retangulo_area   = retangulo_area,
		.posicao_x        = retangulo_coli.x,
		.posicao_y        = retangulo_coli.y,
		.retangulo_coli   = retangulo_coli,
		.retangulo_coli_h = retangulo_coli,
		.retangulo_coli_v = retangulo_coli,
		.retangulo_img    = retangulo_img,
	};
	return inimigo;
}

void CalcularInimigo(Inimigo *inimigo, double delta_frame, Camera *camera, Mapa mapa, int tamanho_bloco[2], int tamanhos_tela[2]){
	double movi_v = 1, movi_h = 0;
	inimigo->frame += delta_frame;

	// Logica do inimigo
	//printf("retangulo %d\n",inimigo->retangulo_coli.x );
	//printf("area %d\n",inimigo->retangulo_area.x );
	//printf("area + w %d\n",inimigo->retangulo_area.x + inimigo->retangulo_area.w );
	//inimigo->costas = true;
	movi_h = (inimigo->costas ? -1 : 1);
	if(inimigo->retangulo_coli.x > inimigo->retangulo_area.x + inimigo->retangulo_area.w) inimigo->costas = true;
	if(inimigo->retangulo_coli.x < inimigo->retangulo_area.x) inimigo->costas = false;


	//if(movi_h<0) inimigo->costas = true;
	//if(movi_h>0) inimigo->costas = false;

	if(!movi_h) inimigo->velocidade_x = 0;

	// É necessario divirdir em dois para não ser possivel bugar nas quinas do blocos
	ColisaoInimigoMapaV(inimigo, mapa, tamanho_bloco, tamanhos_tela, *camera);
	if(inimigo->coli_v){
		movi_v = false;
		inimigo->velocidade_y = false;
		inimigo->retangulo_coli_v = inimigo->retangulo_coli;
		inimigo->posicao_y = inimigo->retangulo_coli.y;
	}
	inimigo->retangulo_coli.y = inimigo->retangulo_coli_v.y;
	inimigo->retangulo_coli_v = inimigo->retangulo_coli;
	inimigo->retangulo_coli_h.y = inimigo->retangulo_coli_v.y;

	
	ColisaoInimigoMapaH(inimigo, mapa, tamanho_bloco, tamanhos_tela, *camera);
	if(inimigo->coli_h){
		inimigo->velocidade_x = false;
		inimigo->retangulo_coli_h = inimigo->retangulo_coli;
		inimigo->posicao_x = inimigo->retangulo_coli.x;
	}
	else inimigo->velocidade_x += inimigo->acelera * delta_frame * movi_h;
	inimigo->retangulo_coli.x = inimigo->retangulo_coli_h.x;
	inimigo->retangulo_coli_h = inimigo->retangulo_coli;
	inimigo->retangulo_coli_v.x = inimigo->retangulo_coli.x;
	if(!inimigo->coli_v){
		inimigo->velocidade_y += inimigo->acelera * delta_frame * movi_v;
	}
	
	if(movi_h){ 
		if (inimigo->estado_passado != VMM_PORCO_ANDAR){
				inimigo->frame=0;
		}
		inimigo->estado_passado = inimigo->estado_atual;
		inimigo->estado_atual = VMM_PORCO_ANDAR;
	}
	else{
		inimigo->estado_passado = inimigo->estado_atual;
		inimigo->estado_atual = VMM_PORCO_IDLE;
	}
	

	// vericando para a velocidade não passar do maximo
	if (inimigo->velocidade_x < inimigo->vel_max_x*-1)
		inimigo->velocidade_x = inimigo->vel_max_x*-1;

	if (inimigo->velocidade_x > inimigo->vel_max_x)
		inimigo->velocidade_x = inimigo->vel_max_x;

	if (inimigo->velocidade_y < inimigo->vel_max_y*-1)
		inimigo->velocidade_y = inimigo->vel_max_y*-1;

	if (inimigo->velocidade_y > inimigo->vel_max_y)
		inimigo->velocidade_y = inimigo->vel_max_y;

	//printf("%d\n",inimigo->retangulo_coli_h.x);

	if(inimigo->estado_atual == VMM_PORCO_CORRER) inimigo->posicao_x += inimigo->velocidade_x * delta_frame;
	else                                          inimigo->posicao_x += inimigo->velocidade_x/2 * delta_frame;
	inimigo->posicao_y += inimigo->velocidade_y * delta_frame;

	inimigo->retangulo_coli_h.x = inimigo->posicao_x;
	inimigo->retangulo_coli_v.y = inimigo->posicao_y;


	inimigo->retangulo_img.y = inimigo->retangulo_coli.y;
	inimigo->retangulo_img.x = inimigo->retangulo_coli.x; 
	inimigo->coli_v = false;
	inimigo->coli_h = false;
}

void DesenharInimigo(SDL_Renderer *renderizador, Inimigo inimigo, SDL_Texture *sprite_atlas, Camera camera ){
	/* se tiver em teste eu não comento
	SDL_SetRenderDrawColor(renderizador, 255, 0, 0, 255);
	SDL_RenderFillRect(renderizador, &(SDL_FRect){
		inimigo.retangulo_coli.x-camera.x,
		inimigo.retangulo_coli.y-camera.y,
		inimigo.retangulo_coli.w,
		inimigo.retangulo_coli.h});
	*/

	Uint64 frame_atual = inimigo.frame / 10; 
	SDL_FRect retangulo_img= {
		inimigo.retangulo_img.x - camera.x,
		inimigo.retangulo_img.y - camera.y,
		inimigo.retangulo_img.w,
		inimigo.retangulo_img.h
	};
	#define X(index,quant) \
		case(index):{ \
		SDL_RenderTextureRotated(renderizador, sprite_atlas, &(SDL_FRect){MedidaImgInimigo1X*(frame_atual%quant),MedidaImgInimigo1Y*index,MedidaImgInimigo1X,MedidaImgInimigo1Y}, \
		&retangulo_img,0,0, inimigo.costas ? 0: SDL_FLIP_HORIZONTAL);}break;
	if(inimigo.index == PORCO_NORMAL){
		switch(inimigo.estado_atual){
			TabelaPorcoAnim
		}
	}
	#undef X
}

void ColisaoPlayerInimigo(PlayerInJogo *jogador, Inimigo *inimigo){
	if(SDL_HasRectIntersection(&jogador->retangulo_coli, &inimigo->retangulo_coli)) {
		if(!jogador->tempo_safe){
			jogador->tempo_safe = 300000;
			jogador->dano_sofrido = inimigo->dano;
		}
	}





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

void ColisaoInimigoMapaV(Inimigo *inimigo, Mapa mapa, int tamanho_bloco[2], int tamanho_tela[2],Camera camera){
	camera.x -= tamanho_tela[0]/2;
	camera.y -= tamanho_tela[1]/2;
	for(int i = camera.y/tamanho_bloco[1]; i*tamanho_bloco[1] < tamanho_tela[1]*2 + camera.y && i < TamanhosMapaY; i++){
		for(int j = camera.x/tamanho_bloco[0]; j*tamanho_bloco[0] < tamanho_tela[0]*2 + camera.x && j < TamanhosMapaX; j++){
			if(mapa.tiles[i][j]){ 
				TiposVMMA tipo_de_coli = CalcularTipoVMMA(mapa.tiles[i][j]);
				switch (tipo_de_coli){
					case VMMA_GRAMA_ON:
					case VMMA_PEDRA_ON:{
							SDL_Rect retangulo = {j*tamanho_bloco[0], i*tamanho_bloco[1], tamanho_bloco[0], tamanho_bloco[1]};
							if(SDL_HasRectIntersection(&retangulo, &inimigo->retangulo_coli_v))
									inimigo->coli_v = true;
					}break;
				}
			}
		}
	}
}

void ColisaoInimigoMapaH(Inimigo *inimigo, Mapa mapa, int tamanho_bloco[2], int tamanho_tela[2],Camera camera){
	camera.x -= tamanho_tela[0]/2;
	camera.y -= tamanho_tela[1]/2;
	for(int i = camera.y/tamanho_bloco[1]; i*tamanho_bloco[1] < tamanho_tela[1]*2 + camera.y && i < TamanhosMapaY; i++){
		for(int j = camera.x/tamanho_bloco[0]; j*tamanho_bloco[0] < tamanho_tela[0]*2 + camera.x && j < TamanhosMapaX; j++){
			if(mapa.tiles[i][j]){ 
				TiposVMMA tipo_de_coli = CalcularTipoVMMA(mapa.tiles[i][j]);
				switch (tipo_de_coli){
					case VMMA_GRAMA_ON:
					case VMMA_PEDRA_ON:{
							SDL_Rect retangulo = {j*tamanho_bloco[0], i*tamanho_bloco[1], tamanho_bloco[0], tamanho_bloco[1]};
							if(SDL_HasRectIntersection(&retangulo, &inimigo->retangulo_coli_h))
									inimigo->coli_h = true;
					}break;
				}
			}
		}
	}
}


void DesenharMapa(SDL_Renderer *renderizador, Mapa mapa, Camera camera, int tamanho_bloco[2], int tamanho_tela[2]){
	int i = camera.y/tamanho_bloco[1];
	
	if(i<0) i = 0;
	for(; i*tamanho_bloco[1] < tamanho_tela[1] + camera.y && i < TamanhosMapaY; i++){
		int j = camera.x/tamanho_bloco[0];
		if(j<0) j = 0;
		for(; j*tamanho_bloco[0] < tamanho_tela[0] + camera.x && j < TamanhosMapaX; j++){
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