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


bool VerificarMarcadorBloco(MarcadorBloco *marcador, SDL_Point mouse, int rolada){
		// Cria um retangulo para verificar as colisões como o mouse
		SDL_Rect retangulo_colisao;
		AtribuirFRectInRectA(&marcador->retangulo, &retangulo_colisao);
	retangulo_colisao.y += rolada;

		// verifica se o mouse está emcima do botão
		if (SDL_PointInRect(&mouse, &retangulo_colisao))
				marcador->sobre = true;
		else
				marcador->sobre = false;

		// verifica se o botão de sair foi clicado
		if (marcador->sobre)
				return true;

		return false;
}


void DesenharMarcadorBloco(SDL_Renderer *renderizador, MarcadorBloco marcador, int indix, SDL_Texture *textura,int rolada){
	SDL_FRect src = MapaTiles(indix);
	SDL_RenderTexture(renderizador, textura, &src , &(SDL_FRect){marcador.retangulo.x , marcador.retangulo.y + rolada, marcador.retangulo.w, marcador.retangulo.h});
}


void InitCenaMapa(SDL_Renderer *renderizador, VariaveisMapa *mapa, Tamanhos tamanhos){
	// fonte
		TTF_Font *fonte = TTF_OpenFont("assets/fonts/font1.fon", tamanhos.tela[1] / 10);

	// Mapa mapa
	mapa->mapa.n = 0;
	CarregarMapa(&mapa->mapa, 0);

	if(mapa->mapa.textura) SDL_DestroyTexture(mapa->mapa.textura);
	mapa->mapa.textura = IMG_LoadTexture(renderizador,"assets/imagens/world/tiles/Tiles.png");
	SDL_SetTextureScaleMode(mapa->mapa.textura, SDL_SCALEMODE_NEAREST);

	// Mapa variaveis
	mapa->camera = (Camera){tamanhos.bloco1[0],(TamanhosMapaY/2)*tamanhos.bloco1[1]};
	mapa->rolada = 0;
	mapa->velocidade = 1;
	mapa->tamanho_bloco[0] = tamanhos.bloco1[0];
	mapa->tamanho_bloco[1] = tamanhos.bloco1[1];
	mapa->selecao.w = tamanhos.bloco1[0];
	mapa->selecao.h = tamanhos.bloco1[1];
	mapa->bloco_uso = 1;
	SDL_FRect rect_moldura = {
		tamanhos.tela[0] - tamanhos.bloco1[0]*2-30,
		0,
		tamanhos.bloco1[0]*2 + 30,
		tamanhos.tela[1]
	};
	DestruirMoldura(&mapa->moldura_bloco);
	mapa->moldura_bloco = InitMoldura(renderizador, &rect_moldura, "assets/imagens/ui/panels/moldura de madeira.png");
	CalcularMolduraPartes(&mapa->moldura_bloco, CantoFixo);
	DestruirBotao(&mapa->botao_salvar);
	mapa->botao_salvar =
	InitBotao(renderizador,
				&(SDL_FRect){
						tamanhos.tela[0] - CantoFixo*4,
						tamanhos.tela[1] - tamanhos.botao1[1] - CantoFixo,
						tamanhos.botao1[0],
						tamanhos.botao1[1]}, // retangulo base
				"assets/imagens/ui/buttons/botão.png",
				"Salvar",
				(SDL_Color){70, 70, 70, 255},
				(SDL_Color){30, 30, 30, 255},
				CENA_MENU, fonte,
				(SDL_Color)PRETO);
		mapa->botao_salvar.retangulo.x -= mapa->botao_salvar.retangulo.w;
		CalcularBotaoPartes(&mapa->botao_salvar);

	DestruirBotao(&mapa->botao_carregar);
	mapa->botao_carregar =
	InitBotao(renderizador,
				&(SDL_FRect){
						tamanhos.tela[0] - CantoFixo*4,
						tamanhos.tela[1] - tamanhos.botao1[1] - CantoFixo,
						tamanhos.botao1[0],
						tamanhos.botao1[1]}, // retangulo base
				"assets/imagens/ui/buttons/botão.png",
				"Carregar",
				(SDL_Color){70, 70, 70, 255},
				(SDL_Color){30, 30, 30, 255},
				CENA_MENU, fonte,
				(SDL_Color)PRETO);
		mapa->botao_carregar.retangulo.x -= mapa->botao_carregar.retangulo.w*2;
		CalcularBotaoPartes(&mapa->botao_carregar);


	mapa->marcador_preencher = InitMarcador(
		renderizador,
			&(SDL_FRect){
			mapa->botao_carregar.retangulo.x - CantoFixo-tamanhos.botao1[0],
			mapa->botao_carregar.retangulo.y,
			tamanhos.botao1[0],
			tamanhos.botao1[1]},
		0,
		"assets/imagens/ui/buttons/marcador.png",
		(SDL_Color)SEMI_PRETO, 
		(SDL_Color)SEMI_PRETO
	);


	#define X(index, x_loc, y_loc, tipo)                                    \
	mapa->marcador[ index ] = (MarcadorBloco){                              \
		.ativo = false,													    \
		.cor1 = (SDL_Color)PRETO,									        \
		.cor2 = (SDL_Color)BRANCO,							                \
		.retangulo = (SDL_FRect){                                           \
			tamanhos.tela[0]-tamanhos.bloco1[0] -13,        \
			tamanhos.bloco1[1] * (index/2),                         \
			tamanhos.bloco1[0]        ,                             \
			tamanhos.bloco1[1]}                                     \
	};                                                       			    \
	mapa->marcador[index].retangulo.x += (!(index%2)) ? -tamanhos.bloco1[0]-2 : +0;

	TabelaBlocoAtlas

	#undef X
}

void LoopCenaMapa(VariveisGerais *geral, VariaveisMapa *mapa){


	//teclado
	const bool *teclado = SDL_GetKeyboardState(NULL);
	if (teclado[SDL_SCANCODE_ESCAPE]){
		geral->cena_continuar = CENA_CRIACAO;
		geral->cena_passada = CENA_CRIACAO;
				geral->cena = CENA_PAUSE;
	}

	int aumentar = (teclado[SDL_SCANCODE_UP] - teclado[SDL_SCANCODE_DOWN]);
	mapa->velocidade += aumentar;
	int movimento_h = (teclado[SDL_SCANCODE_D] - teclado[SDL_SCANCODE_A]) * mapa->velocidade;
	int movimento_v = (teclado[SDL_SCANCODE_S] - teclado[SDL_SCANCODE_W]) * mapa->velocidade;

	mapa->camera.x += movimento_h;
	mapa->camera.y += movimento_v;



	mapa->selecao.x = ((geral->ponto_mouse.x+mapa->camera.x )/mapa->tamanho_bloco[0]) * mapa->tamanho_bloco[0] - mapa->camera.x;
	mapa->selecao.y = ((geral->ponto_mouse.y+mapa->camera.y )/mapa->tamanho_bloco[1]) * mapa->tamanho_bloco[1] - mapa->camera.y;

	mapa->selecao_coli.x = (geral->ponto_mouse.x+mapa->camera.x )/mapa->tamanho_bloco[0];
	mapa->selecao_coli.y = (geral->ponto_mouse.y+mapa->camera.y )/mapa->tamanho_bloco[1];


	if(VerificarBotao(&mapa->botao_salvar, geral->ponto_mouse, geral->botao_mouse_esquerdo)){
		SalvarMapa(&mapa->mapa);
	}
	if(VerificarBotao(&mapa->botao_carregar, geral->ponto_mouse, geral->botao_mouse_esquerdo)){
		CarregarMapa(&mapa->mapa, mapa->mapa.n);
	}
	if(VerificarMarcador(&mapa->marcador_preencher, geral->ponto_mouse, geral->botao_mouse_esquerdo)){
		printf("preenchimento\n");
	}
	if(geral->mouse_x > mapa->moldura_bloco.retangulo.x){
		mapa->rolada += geral->botao_mouse_gira * 20;
		geral->botao_mouse_gira = false;
	}
	else if(teclado[SDL_SCANCODE_LCTRL]){
		mapa->camera.x = mapa->camera.x / mapa->tamanho_bloco[0];
		mapa->camera.y = mapa->camera.y / mapa->tamanho_bloco[1];
		mapa->tamanho_bloco[0] += geral->botao_mouse_gira * 2;
		mapa->tamanho_bloco[1] += geral->botao_mouse_gira * 2;
		mapa->selecao.w = mapa->tamanho_bloco[0];
		mapa->selecao.h = mapa->tamanho_bloco[1];
		mapa->camera.x = mapa->camera.x * mapa->tamanho_bloco[0];
		mapa->camera.y = mapa->camera.y * mapa->tamanho_bloco[1];
		geral->botao_mouse_gira = false;
	}
	else{
		if(teclado[SDL_SCANCODE_LSHIFT]){
			mapa->camera.x -= geral->botao_mouse_gira * mapa->velocidade;
			geral->botao_mouse_gira = false;
		}
		else{
			mapa->camera.y -= geral->botao_mouse_gira * mapa->velocidade;
			geral->botao_mouse_gira = false;
		}
	}

	if(geral->botao_mouse_esquerdo && !mapa->botao_salvar.sobre && geral->mouse_x > mapa->moldura_bloco.retangulo.x){
		//printf("canau\n");
		#define X(index, x_loc, y_loc, tipo)                                                  \
		if(VerificarMarcadorBloco(&mapa->marcador[index], geral->ponto_mouse, mapa->rolada)){ \
			 mapa->bloco_uso = index+1;                                                       \
			}                                                                                 \

		TabelaBlocoAtlas

		#undef X
	}
	else if(geral->botao_mouse_esquerdo && !mapa->botao_salvar.sobre && !mapa->marcador_preencher.sobre){
		if(mapa->marcador_preencher.ativo){
			mapa->selecao_coli_preencimento.x = mapa->selecao_coli.x;
			mapa->selecao_coli_preencimento.y = mapa->selecao_coli.y;
			mapa->preencher = true;
		}
		else{
			mapa->mapa.tiles[mapa->selecao_coli.y][mapa->selecao_coli.x] = mapa->bloco_uso;
		}
		//printf("x:%d y:%d\n", mapa->selecao_coli.x, mapa->selecao_coli.y);

	}
	if(geral->botao_mouse_direito){
		mapa->mapa.tiles[mapa->selecao_coli.y][mapa->selecao_coli.x] = 0;
		//printf("x:%d y:%d\n", mapa->selecao_coli.x, mapa->selecao_coli.y);
	}

	if(!geral->botao_mouse_meio){
		mapa->camera_back.x = mapa->camera.x;
		mapa->camera_back.y = mapa->camera.y;
		geral->mouse_x_back = geral->mouse_x;
		geral->mouse_y_back = geral->mouse_y;

	}


	if(!geral->botao_mouse_esquerdo){
		if(mapa->preencher){
			int modulo_x = mapa->selecao_coli_preencimento.x-mapa->selecao_coli_back.x;
			int modulo_y = mapa->selecao_coli_preencimento.y-mapa->selecao_coli_back.y;
			if(modulo_y < 0){
				for(int i = mapa->selecao_coli_back.y; i >= mapa->selecao_coli_back.y + modulo_y; i--){
					if(modulo_x < 0)
					for(int j = mapa->selecao_coli_back.x; j >= mapa->selecao_coli_back.x + modulo_x; j--){
						mapa->mapa.tiles[i][j] = mapa->bloco_uso;
					}
					else
					for(int j = mapa->selecao_coli_back.x; j <= mapa->selecao_coli_back.x + modulo_x; j++){
						mapa->mapa.tiles[i][j] = mapa->bloco_uso;
					}
				}
			}
			else{
				//printf("banana2\n");
				for(int i = mapa->selecao_coli_back.y; i <= mapa->selecao_coli_back.y + modulo_y; i++){
					if(modulo_x < 0)
					for(int j = mapa->selecao_coli_back.x; j >= mapa->selecao_coli_back.x + modulo_x; j--){
						mapa->mapa.tiles[i][j] = mapa->bloco_uso;
					}
					else
					for(int j = mapa->selecao_coli_back.x; j <= mapa->selecao_coli_back.x + modulo_x; j++){
						mapa->mapa.tiles[i][j] = mapa->bloco_uso;
					}
				}
			}
			mapa->preencher = false;
		}



		mapa->selecao_coli_back.x = mapa->selecao_coli.x; 
		mapa->selecao_coli_back.y = mapa->selecao_coli.y; 
	}

	if(geral->botao_mouse_meio){
		mapa->camera.x = mapa->camera_back.x + geral->mouse_x_back - geral->mouse_x;
		mapa->camera.y = mapa->camera_back.y + geral->mouse_y_back - geral->mouse_y;
	}


}


void DesenharCenaMapa(VariveisGerais geral, VariaveisMapa mapa){
	// Limpeza de Tela
		SDL_SetRenderDrawColor(geral.renderizador, mapa.cor_fundo.r, mapa.cor_fundo.g, mapa.cor_fundo.b, mapa.cor_fundo.a);
		SDL_RenderClear(geral.renderizador);

		// Elementos
		DesenharMapa(geral.renderizador, mapa.mapa, mapa.camera, mapa.tamanho_bloco, geral.resolucao_atual);
	SDL_Color cor = SEMI_PRETO;
	SDL_SetRenderDrawColor(geral.renderizador, cor.r, cor.g, cor.b, cor.a);
	SDL_RenderFillRect(geral.renderizador, &mapa.selecao);
	DesenharMoldura(geral.renderizador, mapa.moldura_bloco);

	#define X(index, x_loc, y_loc, tipo)                         \
	DesenharMarcadorBloco(geral.renderizador, mapa.marcador[index], index+1, mapa.mapa.textura, mapa.rolada);   

	TabelaBlocoAtlas

	#undef X

	DesenharBotao(geral.renderizador, mapa.botao_salvar);
	DesenharBotao(geral.renderizador, mapa.botao_carregar);
	DesenharMarcador(geral.renderizador, mapa.marcador_preencher);
}
