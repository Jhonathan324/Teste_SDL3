#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "jogo.h"
#include "mapa.h"



void InitCenaJogo(VariveisGerais *geral, VariveisJogo *jogo, Tamanhos tamanhos){
    // Jogador do Geral
    geral->jogador = (Player){
        .vida = 100,
        .coracoes = 3
    };
    
    //Jogador da Cena Jogo
    jogo->jogador = InitPlayer(
        geral->renderizador, 
        (SDL_FRect){0,0,tamanhos.jogador[0],tamanhos.jogador[1]},
        (SDL_Rect){tamanhos.bloco1[0],62*tamanhos.bloco1[1],tamanhos.jogador_coli[0],tamanhos.jogador_coli[1]},
        "assets/imagens/entities/player/Guerreiro.png",
        geral->jogador.vida,
        geral->jogador.coracoes
    );

    // Inimigos
    jogo->inimigos[0] = InitInimigo(
        geral->renderizador, 
        (SDL_FRect){tamanhos.bloco1[0]*4,65*tamanhos.bloco1[1] - tamanhos.inimigo1[1],tamanhos.inimigo1[0],tamanhos.inimigo1[1]},
        (SDL_Rect) {tamanhos.bloco1[0]*4,65*tamanhos.bloco1[1] - tamanhos.inimigo1[1],tamanhos.inimigo1[0]*5,tamanhos.inimigo1[1]},
        (SDL_Rect) {tamanhos.bloco1[0]*4,65*tamanhos.bloco1[1] - tamanhos.inimigo1[1],tamanhos.inimigo1[0],tamanhos.inimigo1[1]},
        10,
        PORCO_NORMAL
    );
    jogo->sprite_atlas_inimigos[0] = IMG_LoadTexture(geral->renderizador, "assets/imagens/entities/mobs/porco marron.png");
    if (jogo->sprite_atlas_inimigos[0]) SDL_SetTextureScaleMode(jogo->sprite_atlas_inimigos[0], SDL_SCALEMODE_NEAREST);
    

    // Variaveis do Jogo
    jogo->tempo = 0;
    jogo->camera.x = 0;
    jogo->camera.y = 0;
    jogo->tamanho_bloco[0] = tamanhos.bloco1[0];
    jogo->tamanho_bloco[1] = tamanhos.bloco1[1];

    // Mapa
    jogo->mapa.n = 0;
    jogo->mapa.textura = IMG_LoadTexture(geral->renderizador, "assets/imagens/world/tiles/Tiles.png");
    if (jogo->mapa.textura) SDL_SetTextureScaleMode(jogo->mapa.textura, SDL_SCALEMODE_NEAREST);
    CarregarMapa(&jogo->mapa, jogo->mapa.n);
    /* Debug
    for(int i = 0; i<TamanhosMapaX; i++){
        //if(rand()%2) jogo->mapa.tiles[7][i] = 0;
        jogo->mapa.tiles[64][i] = rand()%3+2;
        jogo->mapa.tiles[65][i] = rand()%3+7;
        jogo->mapa.tiles[66][i] = rand()%3+12;
        jogo->mapa.tiles[67][i] = rand()%3+12;
        jogo->mapa.tiles[68][i] = rand()%3+17;
        jogo->mapa.tiles[69][i] = rand()%3+22;
    }
    for(int i = 0; i<3; i++)
    jogo->mapa.tiles[7][5+i] = rand()%3+7;
    SalvarMapa(&jogo->mapa);
    */
 
}

void CalcularCenaJogo(VariveisGerais *geral, VariveisJogo *jogo, Tamanhos tamanhos){
    // Player
    double x = tamanhos.tela[0] * ( (double)jogo->jogador.retangulo_coli.x/geral->resolucao_antiga[0]);
    double y = tamanhos.tela[1] * ( (double)jogo->jogador.retangulo_coli.y/geral->resolucao_antiga[1]);
    SDL_FRect retangulo_img = {x,y,tamanhos.jogador[0],tamanhos.jogador[1]};
    SDL_Rect retangulo_coli = {x,y,tamanhos.jogador_coli[0],tamanhos.jogador_coli[1]};

    jogo->jogador.acelera          = (float)retangulo_coli.w/100;
    jogo->jogador.vel_max_x        = (float)retangulo_coli.w/48;
    jogo->jogador.vel_max_y        = (float)retangulo_coli.h/48;
    jogo->jogador.velocidade_x     = 0;
    jogo->jogador.velocidade_y     = 0;
    jogo->jogador.posicao_x        = x;
    jogo->jogador.posicao_y        = y;
    jogo->jogador.retangulo_coli   = retangulo_coli;
    jogo->jogador.retangulo_coli_h = retangulo_coli;
    jogo->jogador.retangulo_coli_v = retangulo_coli;
    jogo->jogador.retangulo_img    = retangulo_img;

    //variaveis
    jogo->tamanho_bloco[0] = tamanhos.bloco1[0];
    jogo->tamanho_bloco[1] = tamanhos.bloco1[1];
}

void LoopCenaJogo(VariveisGerais *geral, VariveisJogo *jogo, double delta_t){
    //teclado
    const bool *teclado = SDL_GetKeyboardState(NULL);
    if (teclado[SDL_SCANCODE_ESCAPE]){
        geral->cena_continuar = geral->cena;
        geral->cena_passada = geral->cena;
        geral->cena = CENA_PAUSE;
    }
    // Logica da camera 
    if(-jogo->camera.x+jogo->jogador.retangulo_coli.x + jogo->jogador.retangulo_coli.w > geral->resolucao_atual[0]*0.6) jogo->camera.x = jogo->jogador.retangulo_coli.x  + jogo->jogador.retangulo_coli.w - geral->resolucao_atual[0]*0.6;
    else if(-jogo->camera.x+jogo->jogador.retangulo_coli.x  < geral->resolucao_atual[0]*0.4) jogo->camera.x = jogo->jogador.retangulo_coli.x - geral->resolucao_atual[0]*0.4;
    if(-jogo->camera.y+jogo->jogador.retangulo_coli.y + jogo->jogador.retangulo_coli.h > geral->resolucao_atual[1]*0.7) jogo->camera.y = jogo->jogador.retangulo_coli.y  + jogo->jogador.retangulo_coli.h - geral->resolucao_atual[1]*0.7;
    else if(-jogo->camera.y+jogo->jogador.retangulo_coli.y  < geral->resolucao_atual[1]*0.3) jogo->camera.y = jogo->jogador.retangulo_coli.y - geral->resolucao_atual[1]*0.3;
    if(jogo->camera.x<0)jogo->camera.x=0;
    if(jogo->camera.y<0)jogo->camera.y=0;
    //player
    CalcularPlayer(teclado, &jogo->jogador, delta_t*100, &jogo->camera, jogo->mapa, jogo->tamanho_bloco, geral->resolucao_atual);
    //inimigos
    SDL_Rect camera = {jogo->camera.x- geral->resolucao_atual[0]/2, jogo->camera.y - geral->resolucao_atual[1]/2, geral->resolucao_atual[0]*2, geral->resolucao_atual[1]*2};
    for(int i = 0; i<1; i++){
        if(jogo->inimigos[i].vida > 0 &&  SDL_HasRectIntersection(&camera, &jogo->inimigos[i].retangulo_coli)){
        CalcularInimigo(&jogo->inimigos[i], delta_t*100, &jogo->camera, jogo->mapa, jogo->tamanho_bloco, geral->resolucao_atual);
        ColisaoPlayerInimigo(&jogo->jogador, &jogo->inimigos[i]);
    }
    }
}

void DesenharCenaJogo(VariveisGerais geral, VariveisJogo jogo, Tamanhos tamanhos)
{
    // Limpeza de Tela
    SDL_SetRenderDrawColor(geral.renderizador, jogo.cor_fundo.r, jogo.cor_fundo.g, jogo.cor_fundo.b, jogo.cor_fundo.a);
    SDL_RenderClear(geral.renderizador);

    // Elementos
    DesenharMapa(geral.renderizador, jogo.mapa, jogo.camera, jogo.tamanho_bloco, geral.resolucao_atual);
    for(int i = 0; i<1; i++){
        if(jogo.inimigos[i].vida > 0)
        DesenharInimigo(geral.renderizador, jogo.inimigos[i], jogo.sprite_atlas_inimigos[jogo.inimigos[i].index], jogo.camera);
    }
    DesenharPlayer(geral.renderizador, jogo.jogador, jogo.camera);
    DesenharHud(geral, jogo, tamanhos);
}

