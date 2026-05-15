#include <stdio.h>
#include <stdbool.h>
#include "gerais.h"

void InitCenaConf(VariveisGerais *geral, VariveisConf *conf, Tamanhos tamanhos)
{

    // fonte
    TTF_Font *fonte = TTF_OpenFont("assets/fonts/font1.fon", tamanhos.tela[1] / 10);

    // fundo
    if(conf->imagem){
        SDL_DestroyTexture(conf->imagem);
    }
    conf->imagem = IMG_LoadTexture(geral->renderizador, "assets/imagens/UI/backgrounds/conf.png");

    // Criação da moldura para os botões
    SDL_FRect rect_moldura = {0, 0, tamanhos.menu[0], tamanhos.menu[1]};

    // obtenção do rect da janela
    int janela_w, janela_h;
    SDL_GetWindowSize(geral->janela, &janela_w, &janela_h);
    SDL_FRect rect_janela = {0, 20, janela_w, janela_h};
    CentralizarRectInRect(&rect_janela, &rect_moldura);

    DestruirMoldura(&conf->moldura);
    conf->moldura = InitMoldura(geral->renderizador, &rect_moldura, "assets/imagens/ui/panels/moldura de madeira.png");
    CalcularMolduraPartes(&conf->moldura, CantoFixo);

    //Textos
    DestruirTexto(&conf->texto_reso);
    DestruirTexto(&conf->texto_full);
    conf->texto_reso = InitTexto(geral->renderizador, 
        &(SDL_FRect){conf->moldura.retangulo.x + CantoFixo -10,
            conf->moldura.retangulo.y + CantoFixo,
            tamanhos.botao1[0],
            tamanhos.botao1[1]/2},
        (SDL_Color){0,0,0,0},
        "Resolução",
        NULL,
        fonte,
        (SDL_Color)PRETO,
        false
    );
    conf->texto_full = InitTexto(geral->renderizador, 
        &(SDL_FRect){0,
            conf->moldura.retangulo.y + CantoFixo,
            tamanhos.botao1[0],
            tamanhos.botao1[1]/2},
        (SDL_Color){0,0,0,0},
        "Tela Cheia",
        NULL,
        fonte,
        (SDL_Color)PRETO,
        false
    );
    conf->texto_full.retangulo.x = conf->moldura.retangulo.x + conf->moldura.retangulo.w - conf->texto_full.retangulo.w - CantoFixo/2;

    // Botão para sair das configurações
    DestruirBotao(&conf->botao_sair);
    DestruirMarcador(&conf->troca_fullscreen);
    DestruirBotaoExpansivo(&conf->botao_reso);
    conf->botao_sair =
    InitBotao(geral->renderizador,
        &(SDL_FRect){
            conf->moldura.retangulo.x + conf->moldura.retangulo.w - CantoFixo/2,
            conf->moldura.retangulo.y + conf->moldura.retangulo.h - tamanhos.botao1[1] - CantoFixo/2,
            tamanhos.botao1[0],
            tamanhos.botao1[1]}, // retangulo base
        "assets/imagens/ui/buttons/botão.png",
        "Voltar",
        (SDL_Color){70, 70, 70, 255},
        (SDL_Color){30, 30, 30, 255},
        CENA_MENU, fonte,
        (SDL_Color)PRETO);
    conf->botao_sair.retangulo.x -= conf->botao_sair.retangulo.w;
    CalcularBotaoPartes(&conf->botao_sair);

    conf->troca_fullscreen = InitMarcador(geral->renderizador ,&(SDL_FRect){
        conf->texto_full.retangulo.x+CantoFixo*((float)2/3),
        conf->texto_full.retangulo.y + conf->texto_full.retangulo.h ,
        tamanhos.botao1[1],
        tamanhos.botao1[1]},
        geral->fullscrean,
        "assets/imagens/ui/buttons/marcador.png",
        (SDL_Color)PRETO,
        (SDL_Color)BRANCO
    );

    char resolucao_atual[32];
    sprintf(resolucao_atual,"(%d, %d)",geral->resolucao_atual[0],geral->resolucao_atual[1]);
    char *textos[] = {
        "640, 360",
        "960, 540",
        "1280, 720",
        "1600, 900",
        "1920, 1080",
        "2240, 1260",
        "2560, 1440"
    };
    conf->botao_reso = 
    InitBotaoExpansivo(geral->renderizador,
        &(SDL_FRect){
            conf->moldura.retangulo.x + CantoFixo,
            conf->texto_reso.retangulo.y + conf->texto_reso.retangulo.h ,
            tamanhos.botao1[0],
            tamanhos.botao1[1]}, // retangulo base
        "assets/imagens/ui/buttons/botão.png",
        resolucao_atual,
        textos,
        (SDL_Color)BRANCO,
        (SDL_Color)SEMI_PRETO,
        CENA_CONF, fonte,
        (SDL_Color)PRETO,
        7);
    CalcularBotaoExpansivoPartes(&conf->botao_reso);
}

void LoopCenaConf(VariveisGerais *geral, VariveisConf *conf, Tamanhos *tamanhos)
{   
    // obtendo a localização do mouese
    SDL_GetMouseState(&geral->mouse_x, &geral->mouse_y);
    geral->ponto_mouse.x = geral->mouse_x;
    geral->ponto_mouse.y = geral->mouse_y;

    Botao *botoes[] = {
        &conf->botao_sair,
        &conf->botao_reso.botao_pai
    };

    if(VerificarBotao(botoes[0],geral->ponto_mouse,geral->botao_mouse_esquerdo)){
        
        int temp = geral->cena;
        geral->cena = geral->cena_passada;
        geral->cena_passada=temp;
        if(conf->reso_inicial != tamanhos->escala || conf->valida_fullscrean!=geral->fullscrean) {
            conf->valida_fullscrean = geral->fullscrean;
            conf->reso_inicial = tamanhos->escala;
            geral->troca_reso = true;
        }
    }

    if(VerificarMarcador(&conf->troca_fullscreen,geral->ponto_mouse, geral->botao_mouse_esquerdo)){
        geral->fullscrean = !geral->fullscrean;
        CalcularGeral(geral, tamanhos);
        InitCenaConf(geral, conf, *tamanhos);
    }

    if(VerificarBotao(botoes[1],geral->ponto_mouse,geral->botao_mouse_esquerdo)){
        conf->botao_reso.expandido = !conf->botao_reso.expandido;
    }
    if(conf->botao_reso.expandido){
        for(int i = 0; i < 7; i++){
            if(VerificarBotao(&conf->botao_reso.botao_filho[i],geral->ponto_mouse,geral->botao_mouse_esquerdo) ){
                if(i == tamanhos->escala)conf->botao_reso.expandido = false;
                else{
                    tamanhos->escala = i;
                    CalcularGeral(geral, tamanhos);
                    InitCenaConf(geral, conf, *tamanhos);
                }
            }
        }
    }

}

void DesenharCenaConf(VariveisGerais geral, VariveisConf conf)
{
    // isso vai ser pratico mais para frente - eu sei disso -
    Botao *botoes[] = {&conf.botao_sair};

    // limpeza de tela
    // SDL_SetRenderDrawColor(geral->renderizador, conf->cor_fundo.r, conf->cor_fundo.g, conf->cor_fundo.b, conf->cor_fundo.a);
    // SDL_RenderClear(geral->renderizador);

    // botões
    DesenharMoldura(geral.renderizador, conf.moldura);
    DesenharTexto(geral.renderizador, conf.texto_reso);
    DesenharTexto(geral.renderizador, conf.texto_full);
    for (int i = 0; i < 1; i++)
        DesenharBotao(geral.renderizador, *botoes[i]);
    DesenharBotaoExpansivo(geral.renderizador, conf.botao_reso);
    DesenharMarcador(geral.renderizador,conf.troca_fullscreen);
}