#include <stdio.h>
#include <stdbool.h>
#include "gerais.h"

#define QuantBotao 4

void InitCenaMenu(VariveisGerais *geral, VariveisMenu *menu, Tamanhos tamanhos)
{
    // fonte
    TTF_Font *fonte = TTF_OpenFont("assets/fonts/font1.fon", tamanhos.botao1[1]);

    // fundo
    if(menu->imagem) SDL_DestroyTexture(menu->imagem);
    menu->imagem = IMG_LoadTexture(geral->renderizador, "assets/imagens/ui/background/menu inicial.png");
    SDL_SetTextureScaleMode(menu->imagem, SDL_SCALEMODE_NEAREST);

    // Criação do menu para os botões
    SDL_FRect rect_moldura = {tamanhos.menu[1]*0.1, geral->resolucao_atual[1]-tamanhos.menu[1]*0.9, tamanhos.menu[0], tamanhos.menu[1]};

    // obtenção do rect da janela
    int janela_x, janela_y, janela_w, janela_h;
    SDL_GetWindowSize(geral->janela, &janela_w, &janela_h);
    //SDL_FRect rect_janela = {0, 20, janela_w, janela_h};
    //CentralizarRectInRect(&rect_janela, &rect_moldura); // centralização do menu com base na tela
    DestruirMoldura(&menu->moldura);
    menu->moldura = InitMoldura(geral->renderizador, &rect_moldura, "assets/imagens/ui/panels/moldura de madeira.png");
    CalcularMolduraPartes(&menu->moldura, CantoFixo);

    // Criação dos botões
    DestruirBotao(&menu->botao_conf);
    DestruirBotao(&menu->botao_criacao);
    DestruirBotao(&menu->botao_iniciar);
    DestruirBotao(&menu->botao_sair);
    menu->botao_iniciar =
        InitBotao(geral->renderizador,
                  &(SDL_FRect){0, 0, tamanhos.botao1[0], tamanhos.botao1[1]}, // retangulo base
                  "assets/imagens/ui/buttons/botão.png",
                  "Iniciar Jogo",
                  (SDL_Color){70, 70, 70, 255},
                  (SDL_Color)SEMI_PRETO,
                  CENA_JOGO,
                  fonte,
                  (SDL_Color)PRETO);

    menu->botao_criacao =
        InitBotao(geral->renderizador,
                  &(SDL_FRect){0, 0, tamanhos.botao1[0], tamanhos.botao1[1]}, // retangulo base
                  "assets/imagens/ui/buttons/botão.png",
                  "Criar Mapas",
                  (SDL_Color){70, 70, 70, 255},
                  (SDL_Color)SEMI_PRETO,
                  CENA_CRIACAO,
                  fonte,
                  (SDL_Color)PRETO);

    menu->botao_conf =
        InitBotao(geral->renderizador,
                  &(SDL_FRect){0, 0, tamanhos.botao1[0], tamanhos.botao1[1]}, // retangulo base
                  "assets/imagens/ui/buttons/botão.png",
                  "Configuracoes",
                  (SDL_Color){70, 70, 70, 255},
                  (SDL_Color)SEMI_PRETO,
                  CENA_CONF,
                  fonte,
                  (SDL_Color)PRETO);

    menu->botao_sair =
        InitBotao(geral->renderizador,
                  &(SDL_FRect){0, 0, tamanhos.botao1[0], tamanhos.botao1[1]}, // retangulo base
                  "assets/imagens/ui/buttons/botão.png",
                  "Sair do Jogo",
                  (SDL_Color){70, 70, 70, 255},
                  (SDL_Color)SEMI_PRETO,
                  CENA_SAIR, fonte,
                  (SDL_Color)PRETO);

    // Necessario para alinhar os botões de forma mais pratica
    SDL_FRect *retangulos[] = {
        &menu->botao_iniciar.retangulo,
        &menu->botao_criacao.retangulo,
        &menu->botao_conf.retangulo,
        &menu->botao_sair.retangulo};
    CentralizarRectsInRectV(&menu->moldura.retangulo, retangulos, QuantBotao, 0.1, 0.2);

    // Calculo das partes dos botões para as imagens
    CalcularBotaoPartes(&menu->botao_iniciar);
    CalcularBotaoPartes(&menu->botao_conf);
    CalcularBotaoPartes(&menu->botao_criacao);
    CalcularBotaoPartes(&menu->botao_sair);
}

void LoopCenaMenu(VariveisGerais *geral, VariveisMenu *menu)
{
    SDL_GetMouseState(&geral->mouse_x, &geral->mouse_y);
    geral->ponto_mouse.x = geral->mouse_x;
    geral->ponto_mouse.y = geral->mouse_y;

    int quant_botoes = QuantBotao;
    Botao *botoes[] = {
        &menu->botao_iniciar,
        &menu->botao_criacao,
        &menu->botao_conf,
        &menu->botao_sair
    };

    for (int i = 0; i < quant_botoes; i++){
        if(VerificarBotao(botoes[i],geral->ponto_mouse,geral->botao_mouse_esquerdo)){
            geral->cena_passada = geral->cena;
            geral->cena = botoes[i]->indice;
        }
    }

}

void DesenharCenaMenu(VariveisGerais geral, VariveisMenu menu){
    Botao *botoes[] = {
        &menu.botao_iniciar,
        &menu.botao_criacao,
        &menu.botao_conf,
        &menu.botao_sair};
    
    // limpeza de tela
    SDL_SetRenderDrawColor(geral.renderizador, menu.cor_fundo.r, menu.cor_fundo.g, menu.cor_fundo.b, menu.cor_fundo.a);
    SDL_RenderClear(geral.renderizador);

    SDL_RenderTexture(geral.renderizador, menu.imagem, NULL,
        &(SDL_FRect){
            0,0,
            geral.resolucao_atual[0],
            geral.resolucao_atual[1]
        }
    );

    // botões
    //DesenharMoldura(geral.renderizador, menu.moldura);
    for (int i = 0; i < QuantBotao; i++)
        DesenharBotao(geral.renderizador, *botoes[i]);
}
