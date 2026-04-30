#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "gerais.h"

void InitConf(VariveisGerais *geral, VariveisConf *conf, TAMANHOS tamanhos){
    // fonte
    TTF_Font *fonte = TTF_OpenFont("assets/fonts/font1.fon", tamanhos.tamanho_tela[1] / 10);

    // fundo
    conf->imagem = IMG_LoadTexture(geral->renderizador, "assets/imagens/UI/backgrounds/conf.png");

    // Criação do conf para os botões
    SDL_FRect rect_moldura = {0, 0, tamanhos.tamanho_menu[0], tamanhos.tamanho_menu[1]};

    // obtenção do rect da janela
    int janela_x, janela_y, janela_w, janela_h;
    SDL_GetWindowSize(geral->janela, &janela_w, &janela_h);
    SDL_FRect rect_janela = {0, 20, janela_w, janela_h};
    CentralizarRectInRect(&rect_janela, &rect_moldura);

    conf->moldura = InitMoldura(geral->renderizador, &rect_moldura, 50, "assets/images/ui/panels/moldura de madeira.png");

    conf->botao_sair =
    InitBotao(geral->renderizador,
                &(SDL_FRect){
                    conf->moldura.retangulo.x + conf->moldura.retangulo.w - tamanhos.tamanho_botao_menu[0],
                    conf->moldura.retangulo.y + conf->moldura.retangulo.h - tamanhos.tamanho_botao_menu[1],
                    tamanhos.tamanho_botao_menu[0],
                    tamanhos.tamanho_botao_menu[1]}, // retangulo base
                "assets/images/ui/buttons/botão.png",
                "Voltar",
                (SDL_Color){70, 70, 70, 255},
                (SDL_Color){30, 30, 30, 255},
                CENA_MENU, fonte,
                (SDL_Color){0, 0, 0, 255});

    CalcularBotaoParte(&conf->botao_sair);
}


void CenaConfLoop(VariveisGerais *geral, VariveisConf *conf){
    if (conf->botao_sair.timer){
        if (conf->botao_sair.timer > 1)
            conf->botao_sair.timer--;
        else
        {
            geral->cena = geral->cena_passada;
            conf->botao_sair.timer = 0;
        }
    }
    // Cria um retangulo para verificar as colisões como o mouse 
    SDL_Rect retangulo_colisao;
    AtribuirFRectInRectA(&conf->botao_sair.retangulo, &retangulo_colisao);

    //verifica se o mouse está emcima do botão
    if (SDL_PointInRect(&geral->ponto_mouse, &retangulo_colisao)) conf->botao_sair.sobre = true; 
    else conf->botao_sair.sobre = false; 
    
    //verifica se o botão de sair foi clicado
    if (conf->botao_sair.sobre && geral->botao_mouse_direito) conf->botao_sair.timer = 15;
    
    

    SDL_GetMouseState(&geral->mouse_x, &geral->mouse_y);
    geral->ponto_mouse.x = geral->mouse_x;
    geral->ponto_mouse.y = geral->mouse_y;
}
void CenaConfDesenhar(VariveisGerais *geral, VariveisConf *conf){
    Botao *botoes[] = {
        &conf->botao_sair
    };
    // limpeza de tela
    SDL_SetRenderDrawColor(geral->renderizador, conf->cor_fundo.r, conf->cor_fundo.g, conf->cor_fundo.b, conf->cor_fundo.a);
    SDL_RenderClear(geral->renderizador);

    // botões
    DesenharMoldura(geral->renderizador, conf->moldura);
    for (int i = 0; i < 1; i++)
        DesenharBotao(geral->renderizador, *botoes[i]);
}