#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "gerais.h"

void InitMenu(VariveisGerais *geral, VariveisMenu *menu, TAMANHOS tamanhos)
{
    // fonte
    TTF_Font *fonte = TTF_OpenFont("assets/fonts/font1.fon", tamanhos.tamanho_tela[1] / 10);

    // fundo
    menu->imagem = IMG_LoadTexture(geral->renderizador, "assets/imagens/UI/backgrounds/menu.png");

    // Criação do menu para os botões
    SDL_FRect rect_moldura = {0, 0, tamanhos.tamanho_menu[0], tamanhos.tamanho_menu[1]};

    // obtenção do rect da janela
    int janela_x, janela_y, janela_w, janela_h;
    SDL_GetWindowSize(geral->janela, &janela_w, &janela_h);
    SDL_FRect rect_janela = {0, 20, janela_w, janela_h};
    CentralizarRectInRect(&rect_janela, &rect_moldura); // centralização do menu com base na tela

    menu->moldura = InitMoldura(geral->renderizador, &rect_moldura, 50, "assets/images/ui/panels/moldura de madeira.png");

    // Criação dos botões
    menu->botao_iniciar =
        InitBotao(geral->renderizador,
                  &(SDL_FRect){0, 0, tamanhos.tamanho_botao_menu[0], tamanhos.tamanho_botao_menu[1]}, // retangulo base
                  "assets/images/ui/buttons/botão.png",
                  "Iniciar Jogo",
                  (SDL_Color){70, 70, 70, 255},
                  (SDL_Color){30, 30, 30, 255},
                  CENA_JOGO,
                  fonte,
                  (SDL_Color){0, 0, 0, 255});

    menu->botao_conf =
        InitBotao(geral->renderizador,
                  &(SDL_FRect){0, 0, tamanhos.tamanho_botao_menu[0], tamanhos.tamanho_botao_menu[1]}, // retangulo base
                  "assets/images/ui/buttons/botão.png",
                  "Configuracoes",
                  (SDL_Color){70, 70, 70, 255},
                  (SDL_Color){30, 30, 30, 255},
                  CENA_CONF,
                  fonte,
                  (SDL_Color){0, 0, 0, 255});

    menu->botao_sair =
        InitBotao(geral->renderizador,
                  &(SDL_FRect){0, 0, tamanhos.tamanho_botao_menu[0], tamanhos.tamanho_botao_menu[1]}, // retangulo base
                  "assets/images/ui/buttons/botão.png",
                  "Sair do Jogo",
                  (SDL_Color){70, 70, 70, 255},
                  (SDL_Color){30, 30, 30, 255},
                  CENA_SAIR, fonte,
                  (SDL_Color){0, 0, 0, 255});

    // Necessario para alinhar os botões de forma mais pratica
    SDL_FRect *retangulos[] = {
        &menu->botao_iniciar.retangulo,
        &menu->botao_conf.retangulo,
        &menu->botao_sair.retangulo};
    CentralizarRectsInRectV(&menu->moldura.retangulo, retangulos, 3, 0.1, 0.2);

    // Calculo das partes dos botões para as imagens
    CalcularBotaoParte(&menu->botao_iniciar);
    CalcularBotaoParte(&menu->botao_conf);
    CalcularBotaoParte(&menu->botao_sair);
}

void CenaMenuLoop(VariveisGerais *geral, VariveisMenu *menu)
{
    Botao *botoes[] = {
        &menu->botao_iniciar,
        &menu->botao_conf,
        &menu->botao_sair};

    for (int i = 0; i < 3; i++)
    {
        if (botoes[i]->timer)
        {
            if (botoes[i]->timer > 1)
                botoes[i]->timer--;
            else
            {
                geral->cena_passada = geral->cena;
                geral->cena = botoes[i]->indice;
                botoes[i]->timer = 0;
            }
        }
        // criar o retangulo para os calculos de colisão com o mause
        SDL_Rect retangulo_colisao;
        AtribuirFRectInRectA(&botoes[i]->retangulo, &retangulo_colisao);
        
        // Verifica se o mouse esta sobre o botão
        if (SDL_PointInRect(&geral->ponto_mouse, &retangulo_colisao)) botoes[i]->sobre = true;
        else botoes[i]->sobre = false;
        
        // Inicia o timer do botão se ele foi clicado
        if (botoes[i]->sobre && geral->botao_mouse_direito) botoes[i]->timer = 15;
        
    }

    SDL_GetMouseState(&geral->mouse_x, &geral->mouse_y);
    geral->ponto_mouse.x = geral->mouse_x;
    geral->ponto_mouse.y = geral->mouse_y;
}

void CenaMenuDesenhar(VariveisGerais *geral, VariveisMenu *menu)
{
    Botao *botoes[] = {
        &menu->botao_iniciar,
        &menu->botao_conf,
        &menu->botao_sair};
    // limpeza de tela
    SDL_SetRenderDrawColor(geral->renderizador, menu->cor_fundo.r, menu->cor_fundo.g, menu->cor_fundo.b, menu->cor_fundo.a);
    SDL_RenderClear(geral->renderizador);

    // botões
    DesenharMoldura(geral->renderizador, menu->moldura);
    for (int i = 0; i < 3; i++)
        DesenharBotao(geral->renderizador, *botoes[i]);
}
