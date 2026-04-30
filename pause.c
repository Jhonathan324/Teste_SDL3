#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "gerais.h"

void InitPause(VariveisGerais *geral, VariveisPause *pause, TAMANHOS tamanhos)
{
    // fonte
    TTF_Font *fonte = TTF_OpenFont("assets/fonts/font1.fon", tamanhos.tamanho_tela[1] / 10);

    // fundo
    pause->imagem = IMG_LoadTexture(geral->renderizador, "assets/imagens/UI/backgrounds/pause.png");

    // Criação do pause para os botões
    SDL_FRect rect_moldura = {0, 0, tamanhos.tamanho_menu[0], tamanhos.tamanho_menu[1]};

    // obtenção do rect da janela
    int janela_x, janela_y, janela_w, janela_h;
    SDL_GetWindowSize(geral->janela, &janela_w, &janela_h);
    SDL_FRect rect_janela = {0, 20, janela_w, janela_h};
    CentralizarRectInRect(&rect_janela, &rect_moldura); // centralização do pause com base na tela

    pause->moldura = InitMoldura(geral->renderizador, &rect_moldura, "assets/images/ui/panels/moldura de madeira.png");
    CalcularMolduraPartes(&pause->moldura,50);

    // Criação do pause
    // Criação dos botões
    pause->botao_iniciar =
        InitBotao(geral->renderizador,
                  &(SDL_FRect){0, 0, tamanhos.tamanho_botao_menu[0], tamanhos.tamanho_botao_menu[1]}, // retangulo base
                  "assets/images/ui/buttons/botão.png",
                  "Continuar",
                  (SDL_Color){70, 70, 70, 255},
                  (SDL_Color){30, 30, 30, 255},
                   CENA_JOGO,
                   fonte,
                  (SDL_Color)PRETO);

    pause->botao_conf =
        InitBotao(geral->renderizador,
                  &(SDL_FRect){0, 0, tamanhos.tamanho_botao_menu[0], tamanhos.tamanho_botao_menu[1]}, // retangulo base
                  "assets/images/ui/buttons/botão.png",
                  "Configuracoes",
                  (SDL_Color){70, 70, 70, 255},
                  (SDL_Color){30, 30, 30, 255},
                  CENA_CONF,
                  fonte,
                  (SDL_Color)PRETO);

    pause->botao_sair =
        InitBotao(geral->renderizador,
                  &(SDL_FRect){0, 0, tamanhos.tamanho_botao_menu[0], tamanhos.tamanho_botao_menu[1]}, // retangulo base
                  "assets/images/ui/buttons/botão.png",
                  "Menu Inicial",
                  (SDL_Color){70, 70, 70, 255},
                  (SDL_Color){30, 30, 30, 255},
                  CENA_MENU, fonte,
                  (SDL_Color)PRETO);

    // Necessario para alinhar os botões de forma mais pratica
    SDL_FRect *retangulos[] = {
        &pause->botao_iniciar.retangulo,
        &pause->botao_conf.retangulo,
        &pause->botao_sair.retangulo};
    CentralizarRectsInRectV(&pause->moldura.retangulo, retangulos, 3, 0.1, 0.2);

    //Calculo das partes dos botões para as imagens
    CalcularBotaoPartes(&pause->botao_iniciar);
    CalcularBotaoPartes(&pause->botao_conf);
    CalcularBotaoPartes(&pause->botao_sair);
}

void CenaPauseLoop(VariveisGerais *geral, VariveisPause *pause)
{
    SDL_GetMouseState(&geral->mouse_x, &geral->mouse_y);
    geral->ponto_mouse.x = geral->mouse_x;
    geral->ponto_mouse.y = geral->mouse_y;

    int quant_botoes = 3;
    Botao *botoes[] = {
        &pause->botao_iniciar,
        &pause->botao_conf,
        &pause->botao_sair};

    for (int i = 0; i < quant_botoes; i++)
    {
        if(VerificarBotao(botoes[i],geral->ponto_mouse,geral->botao_mouse_direito)){
            geral->cena_passada = geral->cena;
            geral->cena = botoes[i]->indice;
        }
    }
}

void CenaPauseDesenhar(VariveisGerais *geral, VariveisPause *pause)
{
    Botao *botoes[] = {
        &pause->botao_iniciar,
        &pause->botao_conf,
        &pause->botao_sair};

    // limpeza de tela
    // SDL_SetRenderDrawColor(geral->renderizador, pause->cor_fundo.r, pause->cor_fundo.g, pause->cor_fundo.b, pause->cor_fundo.a);
    // SDL_RenderClear(geral->renderizador);

    // botões
    DesenharMoldura(geral->renderizador, pause->moldura);
    for (int i = 0; i < 3; i++)
        DesenharBotao(geral->renderizador, *botoes[i]);
}
