#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "gerais.h"

void InitConf(VariveisGerais *geral, VariveisConf *conf, TAMANHOS tamanhos)
{

    // fonte
    TTF_Font *fonte = TTF_OpenFont("assets/fonts/font1.fon", tamanhos.tamanho_tela[1] / 10);

    // fundo
    conf->imagem = IMG_LoadTexture(geral->renderizador, "assets/imagens/UI/backgrounds/conf.png");

    // Criação da moldura para os botões
    SDL_FRect rect_moldura = {0, 0, tamanhos.tamanho_menu[0], tamanhos.tamanho_menu[1]};

    // obtenção do rect da janela
    int janela_x, janela_y, janela_w, janela_h;
    SDL_GetWindowSize(geral->janela, &janela_w, &janela_h);
    SDL_FRect rect_janela = {0, 20, janela_w, janela_h};
    CentralizarRectInRect(&rect_janela, &rect_moldura);

    conf->moldura = InitMoldura(geral->renderizador, &rect_moldura, "assets/images/ui/panels/moldura de madeira.png");
    CalcularMolduraPartes(&conf->moldura, 48);

    // Botão para sair das configurações
    conf->botao_sair =
    InitBotao(geral->renderizador,
        &(SDL_FRect){
            conf->moldura.retangulo.x + conf->moldura.retangulo.w - 20,
            conf->moldura.retangulo.y + conf->moldura.retangulo.h - tamanhos.tamanho_botao1[1] - 20,
            tamanhos.tamanho_botao1[0],
            tamanhos.tamanho_botao1[1]}, // retangulo base
        "assets/images/ui/buttons/botão.png",
        "Voltar",
        (SDL_Color){70, 70, 70, 255},
        (SDL_Color){30, 30, 30, 255},
        CENA_MENU, fonte,
        (SDL_Color)PRETO);
    conf->botao_sair.retangulo.x -= conf->botao_sair.retangulo.w;
    CalcularBotaoPartes(&conf->botao_sair);

    char resolucao_atual[11];
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
            conf->moldura.retangulo.x + 48,
            conf->moldura.retangulo.y + 48,
            tamanhos.tamanho_botao1[0],
            tamanhos.tamanho_botao1[1]}, // retangulo base
        "assets/images/ui/buttons/botão.png",
        resolucao_atual,
        textos,
        (SDL_Color)BRANCO,
        (SDL_Color)SEMI_PRETO,
        CENA_CONF, fonte,
        (SDL_Color)PRETO,
        7);
    CalcularBotaoExpansivoPartes(&conf->botao_reso);
}

void CenaConfLoop(VariveisGerais *geral, VariveisConf *conf, TAMANHOS *tamanhos)
{   
    // obtendo a localização do mouese
    SDL_GetMouseState(&geral->mouse_x, &geral->mouse_y);
    geral->ponto_mouse.x = geral->mouse_x;
    geral->ponto_mouse.y = geral->mouse_y;

    int quant_botoes = 2;
    Botao *botoes[] = {
        &conf->botao_sair,
        &conf->botao_reso.botao_pai
    };

    if(VerificarBotao(botoes[0],geral->ponto_mouse,geral->botao_mouse_direito)){
        int temp = geral->cena;
        geral->cena = geral->cena_passada;
        geral->cena_passada=temp;
        if(conf->reso_inicial != tamanhos->escala) {
            conf->reso_inicial = tamanhos->escala;
            geral->troca_reso = true;
        }
    }
    if(VerificarBotao(botoes[1],geral->ponto_mouse,geral->botao_mouse_direito)){
        conf->botao_reso.expandido = !conf->botao_reso.expandido;
    }
    if(conf->botao_reso.expandido){
        for(int i = 0; i < 7; i++){
            if(VerificarBotao(&conf->botao_reso.botao_filho[i],geral->ponto_mouse,geral->botao_mouse_direito) ){
                if(i == tamanhos->escala)conf->botao_reso.expandido = false;
                else{
                    tamanhos->escala = i;
                    CalcularGeral(geral, tamanhos);
                    InitConf(geral, conf, *tamanhos);
                }
            }
        }
    }
    
}

void CenaConfDesenhar(VariveisGerais *geral, VariveisConf *conf)
{
    // isso vai ser pratico mais para frente - eu sei disso -
    Botao *botoes[] = {
        &conf->botao_sair};

    // limpeza de tela
    // SDL_SetRenderDrawColor(geral->renderizador, conf->cor_fundo.r, conf->cor_fundo.g, conf->cor_fundo.b, conf->cor_fundo.a);
    // SDL_RenderClear(geral->renderizador);

    // botões
    DesenharMoldura(geral->renderizador, conf->moldura);
    for (int i = 0; i < 1; i++)
        DesenharBotao(geral->renderizador, *botoes[i]);
    
    DesenharBotaoExpansivo(geral->renderizador, conf->botao_reso);
}