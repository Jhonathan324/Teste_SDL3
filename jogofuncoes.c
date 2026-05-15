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
	PlayerInJogo jogador = {
		.coracoes         = coracoes,
		.estado_atual     = 0,
		.estado_passado   = 0,
		.pulo             = 0,
		.ataque           = 0,
		.costas           = 0,
		.coli_h           = 0,
		.coli_v           = 0,
		.vida             = vida,
		.dano             = 10,
		.dano_sofrido     = 0,
		.frame            = 0,
		.tempo_safe       = 0,
		.tempo_hit        = 0,
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
		.retangulo_dano = {0,0,0,0}
	};

	SDL_SetTextureScaleMode(jogador.sprite_atlas, SDL_SCALEMODE_NEAREST);
	return jogador;
}

void CalcularPlayer(const bool *teclado, PlayerInJogo *jogador, double delta_frame, Camera *camera, Mapa mapa, int tamanho_bloco[2], int tamanhos_tela[2]){
	double movi_v = true, movi_h = 0;

	if(jogador->vida <= 0) {
		jogador->vida = 100;
		jogador->dano_sofrido = 0;
		jogador->coracoes--;
		jogador->retangulo_coli.x = tamanho_bloco[0];
		jogador->retangulo_coli.y = 62*tamanho_bloco[1] - jogador->retangulo_coli.h;
		jogador->retangulo_coli_h = jogador->retangulo_coli;
		jogador->retangulo_coli_v = jogador->retangulo_coli;
		jogador->posicao_x = jogador->retangulo_coli.x;
		jogador->posicao_y = jogador->retangulo_coli.y;
	}

	jogador->frame += delta_frame;

	// Logica do Player
	movi_h = (teclado[SDL_SCANCODE_D] - teclado[SDL_SCANCODE_A]);
	/*
	*/
	if(jogador->ataque){
		jogador->ataque -= delta_frame;
		if(jogador->ataque < 0) jogador->ataque = 0;
		jogador->retangulo_dano.w = 0;
	}
	if(teclado[SDL_SCANCODE_F] && !jogador->ataque){
		jogador->ataque = 40;
		jogador->retangulo_dano.w = tamanho_bloco[0]*2;
		jogador->retangulo_dano.h = tamanho_bloco[1]*2;
		jogador->retangulo_dano.y = jogador->retangulo_coli.y;
		if  ( jogador->costas)jogador->retangulo_dano.x = jogador->retangulo_coli.x - jogador->retangulo_dano.w*1.2;
		else                  jogador->retangulo_dano.x = jogador->retangulo_coli.x + jogador->retangulo_coli.w*1.2;
	}
	


	if(movi_h<0) jogador->costas = true;
	if(movi_h>0) jogador->costas = false;

	if(!movi_h) jogador->velocidade_x = 0;

	// É necessario divirdir em dois para não ser possivel bugar nas quinas do blocos
	ColisaoPlayerMapaV(jogador, mapa, tamanho_bloco, tamanhos_tela, *camera);
	if(jogador->coli_v){
		movi_v = false;
		jogador->velocidade_y = false;
		jogador->retangulo_coli_v = jogador->retangulo_coli;
		jogador->posicao_y = jogador->retangulo_coli.y;
	}
	jogador->retangulo_coli.y = jogador->retangulo_coli_v.y;
	jogador->retangulo_coli_v = jogador->retangulo_coli;
	jogador->retangulo_coli_h.y = jogador->retangulo_coli_v.y;

	ColisaoPlayerMapaH(jogador, mapa, tamanho_bloco, tamanhos_tela, *camera);
	if(jogador->coli_h){
		jogador->velocidade_x = false;
		jogador->retangulo_coli_h = jogador->retangulo_coli;
		jogador->posicao_x = jogador->retangulo_coli.x;
	}
	else jogador->velocidade_x += jogador->acelera * delta_frame * movi_h;
	jogador->retangulo_coli.x = jogador->retangulo_coli_h.x;
	jogador->retangulo_coli_h = jogador->retangulo_coli;
	jogador->retangulo_coli_v.x = jogador->retangulo_coli.x;


	// eu poderia fazer de uma forma mais organizada, mais isso me custaria um tempo que eu não tenho. Fica para projetos futuros.
	if (!jogador->tempo_hit)
	{
		if (!jogador->coli_v)
		{
			if (jogador->pulo)
			{
				if (jogador->ataque)
				{
					if (jogador->estado_passado != VMM_PLAYER_ATAQUE2_MOVIMENTO && jogador->estado_passado != VMM_PLAYER_ATAQUE2)
					{
						jogador->frame = 0;
					}
					jogador->estado_passado = jogador->estado_atual;
					jogador->estado_atual = VMM_PLAYER_ATAQUE2_MOVIMENTO;
				}
				else
				{
					if (jogador->estado_atual != VMM_PLAYER_PULAR && jogador->estado_atual != VMM_PLAYER_PULO_TRANSICAO && jogador->estado_atual != VMM_PLAYER_CAIR)
					{
						jogador->frame = 0;
						jogador->estado_passado = jogador->estado_atual;
						jogador->estado_atual = VMM_PLAYER_PULAR;
					}
					else if (jogador->estado_atual == VMM_PLAYER_PULAR && jogador->pulo < 4)
					{
						jogador->frame = 0;
						jogador->estado_passado = jogador->estado_atual;
						jogador->estado_atual = VMM_PLAYER_PULO_TRANSICAO;
					}
					else
					{
						jogador->estado_passado = jogador->estado_atual;
					}
				}
				jogador->velocidade_y -= jogador->acelera * delta_frame * (jogador->pulo / 6);
				jogador->pulo -= delta_frame;
				if (jogador->pulo < 0)
					jogador->pulo = 0;
			}
			else if (jogador->coyote_time > 1)
			{
				if (jogador->ataque)
				{
					if (jogador->estado_passado != VMM_PLAYER_ATAQUE2_MOVIMENTO && jogador->estado_passado != VMM_PLAYER_ATAQUE2)
					{
						jogador->frame = 0;
					}
					jogador->estado_passado = jogador->estado_atual;
					jogador->estado_atual = VMM_PLAYER_ATAQUE2_MOVIMENTO;
				}
				else
				{
					jogador->frame = 0;
					jogador->estado_passado = jogador->estado_atual;
					jogador->estado_atual = VMM_PLAYER_CAIR;
				}
			}
			else
			{
				jogador->estado_passado = jogador->estado_atual;
			}
			if (jogador->estado_atual != VMM_PLAYER_CAIR)
				jogador->coyote_time++;
			jogador->velocidade_y += jogador->acelera * delta_frame * movi_v;
		}
		else
		{
			jogador->coyote_time = false;
			if (teclado[SDL_SCANCODE_SPACE] && jogador->estado_atual && !jogador->pulo)
			{
				jogador->pulo = 50;
			}
			if (movi_h)
			{
				if (jogador->ataque)
				{
					if (jogador->estado_passado != VMM_PLAYER_ATAQUE2_MOVIMENTO && jogador->estado_passado != VMM_PLAYER_ATAQUE2)
					{
						jogador->frame = 0;
					}
					jogador->estado_passado = jogador->estado_atual;
					jogador->estado_atual = VMM_PLAYER_ATAQUE2_MOVIMENTO;
				}
				else
				{
					if (jogador->estado_passado != VMM_PLAYER_CORRER)
					{
						jogador->frame = 0;
					}
					jogador->estado_passado = jogador->estado_atual;
					jogador->estado_atual = VMM_PLAYER_CORRER;
				}
			}
			else
			{
				if (jogador->ataque)
				{
					if (jogador->estado_passado != VMM_PLAYER_ATAQUE2_MOVIMENTO && jogador->estado_passado != VMM_PLAYER_ATAQUE2)
					{
						jogador->frame = 0;
					}
					jogador->estado_passado = jogador->estado_atual;
					jogador->estado_atual = VMM_PLAYER_ATAQUE2;
				}
				else
				{
					if (jogador->estado_passado != VMM_PLAYER_IDLE)
					{
						jogador->frame = 0;
					}
					jogador->estado_passado = jogador->estado_atual;
					jogador->estado_atual = VMM_PLAYER_IDLE;
				}
			}
		}
	}
	if (jogador->dano_sofrido)
	{
		if (jogador->dano_sofrido > 0)
		{
			// printf("frame: %f\n", delta_frame);
			// printf("tempo: %f\n", jogador->tempo_safe);
			if (jogador->dano_sofrido > delta_frame && jogador->tempo_safe)
			{
				jogador->dano_sofrido -= delta_frame;
				jogador->vida -= delta_frame;
			}
			else
			{
				jogador->vida -= jogador->dano_sofrido;
				jogador->dano_sofrido = 0;
			}
		}
		else
		{
			jogador->dano_sofrido = 0;
		}
	}
	if (jogador->tempo_hit)
	{
		jogador->retangulo_coli_v = jogador->retangulo_coli;
		jogador->retangulo_coli_h = jogador->retangulo_coli;
		movi_h = false;
		movi_v = false;
		jogador->velocidade_x = 0;
		jogador->velocidade_y = 0;
		if (jogador->tempo_hit > delta_frame)
			jogador->tempo_hit -= delta_frame;
		else
			jogador->tempo_hit = 0;
		jogador->estado_atual = VMM_PLAYER_HIT;
	}
	else if (jogador->estado_atual == VMM_PLAYER_HIT)
	{
		jogador->estado_atual = jogador->estado_passado;
	}
	if (jogador->tempo_safe)
	{
		if (jogador->tempo_safe > delta_frame)
			jogador->tempo_safe -= delta_frame;
		else
			jogador->tempo_safe = 0;
	}


	// vericando para a velocidade não passar do maximo
	if (jogador->velocidade_x < jogador->vel_max_x*-1)
		jogador->velocidade_x = jogador->vel_max_x*-1;

	if (jogador->velocidade_x > jogador->vel_max_x)
		jogador->velocidade_x = jogador->vel_max_x;

	if (jogador->velocidade_y < jogador->vel_max_y*-1)
		jogador->velocidade_y = jogador->vel_max_y*-1;

	if (jogador->velocidade_y > jogador->vel_max_y)
		jogador->velocidade_y = jogador->vel_max_y;

	//printf("%d\n",jogador->retangulo_coli_h.x);

	jogador->posicao_x += jogador->velocidade_x * delta_frame;
	jogador->posicao_y += jogador->velocidade_y * delta_frame;

	jogador->retangulo_coli_h.x = jogador->posicao_x;
	jogador->retangulo_coli_v.y = jogador->posicao_y;


	jogador->retangulo_img.y = jogador->retangulo_coli.y + jogador->retangulo_coli.h-jogador->retangulo_img.h - camera->y;
	jogador->retangulo_img.x = jogador->retangulo_coli.x -  (jogador->retangulo_img.w * ((float)44/MedidaImgPlayerX)) - (jogador->costas)*(jogador->retangulo_img.w * ((float)11/MedidaImgPlayerX)) - camera->x; 
	jogador->coli_v = false;
	jogador->coli_h = false;
}

void DesenharPlayer(SDL_Renderer *renderizador, PlayerInJogo jogador, Camera camera ){


	/* se tiver em teste eu não comento
	SDL_SetRenderDrawColor(renderizador, 255, 0, 0, 255);
	SDL_RenderFillRect(renderizador, &(SDL_FRect){
		jogador.retangulo_coli.x-camera.x,
		jogador.retangulo_coli.y-camera.y,
		jogador.retangulo_coli.w,
		jogador.retangulo_coli.h});
		*/
	SDL_SetRenderDrawColor(renderizador, 0, 255, 0, 255);
	SDL_RenderFillRect(renderizador, &(SDL_FRect){
		jogador.retangulo_dano.x-camera.x,
		jogador.retangulo_dano.y-camera.y,
		jogador.retangulo_dano.w,
		jogador.retangulo_dano.h});

	Uint64 frame_atual = jogador.frame / 10; 
	
	#define X(index,quant) \
		case(index):{ \
		SDL_RenderTextureRotated(renderizador, jogador.sprite_atlas,&(SDL_FRect){MedidaImgPlayerX*(frame_atual%quant),MedidaImgPlayerY*index,MedidaImgPlayerX,MedidaImgPlayerY}, \
		&jogador.retangulo_img,0,0, jogador.costas ? SDL_FLIP_HORIZONTAL: 0);\
		if(jogador.tempo_safe && (int)jogador.tempo_safe % 2) { \
			SDL_SetTextureBlendMode(jogador.sprite_atlas, SDL_BLENDMODE_ADD); \
			SDL_SetTextureColorMod(jogador.sprite_atlas, 255, 255, 255);\
			SDL_RenderTextureRotated(renderizador, jogador.sprite_atlas,&(SDL_FRect){MedidaImgPlayerX*(frame_atual%quant),MedidaImgPlayerY*index,MedidaImgPlayerX,MedidaImgPlayerY}, \
			&jogador.retangulo_img,0,0, jogador.costas ? SDL_FLIP_HORIZONTAL: 0);\
			SDL_SetTextureBlendMode(jogador.sprite_atlas, SDL_BLENDMODE_BLEND); \
		}\
}break;

	switch(jogador.estado_atual){
		TabelaPlayerAnim
	}
	#undef X
}

Inimigo InitInimigo(SDL_Renderer *renderizador, SDL_FRect retangulo_img, SDL_Rect retangulo_area, SDL_Rect retangulo_coli, float vida, float dano, int index){
	Inimigo inimigo = {
		.index			  = index,
		.estado_atual     = 0,
		.estado_passado   = 0,
		.costas           = 0,
		.coli_h           = 0,
		.coli_v           = 0,
		.vida             = vida,
		.dano             = dano,
		.frame            = 0,
		.tempo_safe       = 0,
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
	if(!inimigo->tempo_safe)movi_h = (inimigo->costas ? -1 : 1);

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
		inimigo->costas = !inimigo->costas;
		//printf("vefica coli ini\n");
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

	if(inimigo->tempo_safe){
		if(inimigo->tempo_safe>delta_frame)inimigo->tempo_safe-=delta_frame;
		else inimigo->tempo_safe=0;
	}

	else if(movi_h){ 
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
	if(SDL_HasRectIntersection(&jogador->retangulo_dano, &inimigo->retangulo_coli) && !inimigo->tempo_safe){
		inimigo->tempo_safe = 40;
		inimigo->vida -= jogador->dano;
		printf("vida inimigo: %f\n", inimigo->vida);
	}
	else if(SDL_HasRectIntersection(&jogador->retangulo_coli, &inimigo->retangulo_coli) && !jogador->tempo_safe) {
		
		jogador->tempo_hit  = 50;
		jogador->tempo_safe = 100;
		jogador->dano_sofrido = inimigo->dano;
		
	}
}

void InitInimigosinMap(VariveisJogo jogo);

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