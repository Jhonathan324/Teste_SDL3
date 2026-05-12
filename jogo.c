#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "jogo.h"
#include "mapa.h"



void InitCenaJogo(VariveisGerais *geral, VariveisJogo *jogo, TAMANHOS tamanhos){
    // Jogador do Geral
    geral->jogador = (Player){
        .vida = 100,
        .coracoes = 3
    };
    
    //Jogador da Cena Jogo
    jogo->jogador = InitPlayer(
        geral->renderizador, 
        (SDL_FRect){100,50,tamanhos.tamanho_jogador[0],tamanhos.tamanho_jogador[1]},
        (SDL_Rect){100,50,tamanhos.tamanho_jogador_coli[0],tamanhos.tamanho_jogador_coli[1]},
        "C:/Users/17898981/projetos/Teste_SDL3/assets/images/entities/player/Guerreiro.png"
    );

    // Variaveis do Jogo
    jogo->tempo = 0;
    jogo->camera.x = 0;
    jogo->camera.y = 0;
    jogo->tamanho_bloco[0] = tamanhos.tamanho_bloco1[0];
    jogo->tamanho_bloco[1] = tamanhos.tamanho_bloco1[1];

    // Mapa
    jogo->mapa.n = 0;
    jogo->mapa.textura = IMG_LoadTexture(geral->renderizador, "C:/Users/17898981/projetos/Teste_SDL3/assets/images/world/tiles/Tiles.png");
    SDL_SetTextureScaleMode(jogo->mapa.textura, SDL_SCALEMODE_NEAREST);
    CarregarMapa(&jogo->mapa, jogo->mapa.n);

    /* Debug
    for(int i = 0; i<TamanhosMapaX; i++){
        if(rand()%2) jogo->mapa.tiles[7][i] = 0;
        //jogo->mapa.tiles[7][i] = rand()%3+2;
        //jogo->mapa.tiles[8][i] = rand()%3+7;
    }
    for(int i = 0; i<3; i++)
    jogo->mapa.tiles[7][5+i] = rand()%3+7;
    SalvarMapa(&jogo->mapa);
    */


}

void CalcularCenaJogo(VariveisGerais *geral, VariveisJogo *jogo, TAMANHOS tamanhos){
    // Player
    double tela_antiga_x = jogo->jogador.retangulo_img.w / ((double)MedidaImgPlayerX / 640); 
    double tela_antiga_y = jogo->jogador.retangulo_img.h / ((double)MedidaImgPlayerY / 360);  
    double x = tamanhos.tamanho_tela[0] * ( (double)jogo->jogador.retangulo_coli.x/tela_antiga_x );
    double y = tamanhos.tamanho_tela[1] * ( (double)jogo->jogador.retangulo_coli.y/tela_antiga_y );
    jogo->jogador = InitPlayer(
        geral->renderizador, 
        (SDL_FRect){x,y,tamanhos.tamanho_jogador[0],tamanhos.tamanho_jogador[1]},
        (SDL_Rect){x,y,tamanhos.tamanho_jogador_coli[0],tamanhos.tamanho_jogador_coli[1]},
        "C:/Users/17898981/projetos/Teste_SDL3/assets/images/entities/player/Guerreiro.png"
    );

    //variaveis
    jogo->tamanho_bloco[0] = tamanhos.tamanho_bloco1[0];
    jogo->tamanho_bloco[1] = tamanhos.tamanho_bloco1[1];
}

void LoopCenaJogo(VariveisGerais *geral, VariveisJogo *jogo, double delta_t)
{
    //teclado
    const bool *teclado = SDL_GetKeyboardState(NULL);
    if (teclado[SDL_SCANCODE_ESCAPE])
        geral->cena = CENA_PAUSE;

    //player
    if(-jogo->camera.x+jogo->jogador.retangulo_coli.x + jogo->jogador.retangulo_coli.w > geral->resolucao_atual[0]*0.6) jogo->camera.x = jogo->jogador.retangulo_coli.x  + jogo->jogador.retangulo_coli.w - geral->resolucao_atual[0]*0.6;
    else if(-jogo->camera.x+jogo->jogador.retangulo_coli.x  < geral->resolucao_atual[0]*0.4) jogo->camera.x = jogo->jogador.retangulo_coli.x - geral->resolucao_atual[0]*0.4;
    if(-jogo->camera.y+jogo->jogador.retangulo_coli.y + jogo->jogador.retangulo_coli.h > geral->resolucao_atual[1]*0.7) jogo->camera.y = jogo->jogador.retangulo_coli.y  + jogo->jogador.retangulo_coli.h - geral->resolucao_atual[1]*0.7;
    else if(-jogo->camera.y+jogo->jogador.retangulo_coli.y  < geral->resolucao_atual[1]*0.3) jogo->camera.y = jogo->jogador.retangulo_coli.y - geral->resolucao_atual[1]*0.3;
    if(jogo->camera.x<0)jogo->camera.x=0;
    if(jogo->camera.y<0)jogo->camera.y=0;
    CalcularPlayer(teclado, &jogo->jogador, delta_t*100, &jogo->camera, jogo->mapa, jogo->tamanho_bloco, geral->resolucao_atual);
}

void DesenharCenaJogo(VariveisGerais geral, VariveisJogo jogo)
{
    // Limpeza de Tela
    SDL_SetRenderDrawColor(geral.renderizador, jogo.cor_fundo.r, jogo.cor_fundo.g, jogo.cor_fundo.b, jogo.cor_fundo.a);
    SDL_RenderClear(geral.renderizador);

    // Elementos
    DesenharMapa(geral.renderizador, jogo.mapa, jogo.camera, jogo.tamanho_bloco, geral.resolucao_atual);
    DesenharPlayer(geral.renderizador, jogo.jogador, jogo.camera);


}

