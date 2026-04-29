#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "gerais.h"


void InitMenu(VariveisGerais *geral, VariveisMenu *menu, TAMANHOS tamanhos){
    //Criação do menu para os botões
    SDL_FRect rect_moldura = {0,0,tamanhos.tamanho_menu[0],tamanhos.tamanho_menu[1]};
    
    //obtenção do rect da janela
    int janela_x, janela_y, janela_w, janela_h;
    SDL_GetWindowSize(geral->janela,&janela_w,&janela_h);
    SDL_FRect rect_janela = {0, 20, janela_w, janela_h};
    CentralizarRectInRect(&rect_janela, &rect_moldura);//centralização do menu com base na tela

    // Criação do menu
    menu->moldura = InitMoldura(geral->renderizador, &rect_moldura,50,"imagens/moldura de madeira.png");

    TTF_Font *fonte = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", tamanhos.tamanho_tela[1]/10);
    SDL_Color cor_fonte = {255,255,255,255};
    //Criação dos botões
    menu->botao_iniciar = InitBotao(geral->renderizador, &(SDL_FRect){0,0,tamanhos.tamanho_botao_menu[0],tamanhos.tamanho_botao_menu[1]}, 50,"imagens/botão.png", "Iniciar Jogo", (int[3]){70,70,70}, (int[3]){30,30,30}, CENA_JOGO,fonte, cor_fonte);
    menu->botao_conf    = InitBotao(geral->renderizador, &(SDL_FRect){0,0,tamanhos.tamanho_botao_menu[0],tamanhos.tamanho_botao_menu[1]}, 50,"imagens/botão.png", "Configuracoes", (int[3]){70,70,70}, (int[3]){30,30,30}, CENA_CONF,fonte, cor_fonte);
    menu->botao_sair    = InitBotao(geral->renderizador, &(SDL_FRect){0,0,tamanhos.tamanho_botao_menu[0],tamanhos.tamanho_botao_menu[1]}, 50,"imagens/botão.png", "Sair do Jogo", (int[3]){70,70,70}, (int[3]){30,30,30}, CENA_SAIR,fonte, cor_fonte);

    //necessario para colocar o texto nos botões de forma mais prática
    Botao *botoes[] = {
        &menu->botao_iniciar, 
        &menu->botao_conf,
        &menu->botao_sair};

    //Necessario para alinhar os botões de forma mais pratica
    SDL_FRect *retangulos[] = {
        &menu->botao_iniciar.retangulo, 
        &menu->botao_conf.retangulo,
        &menu->botao_sair.retangulo};
        
    CentralizarRectsInRectV(&menu->moldura.retangulo, retangulos ,3,0.1,0.2);
}


void CenaMenuLoop(VariveisGerais *geral, VariveisMenu *menu){
    Botao *botoes[] = {
        &menu->botao_iniciar,
        &menu->botao_conf,
        &menu->botao_sair
    };


    for(int i = 0; i < 3; i++){
        if(botoes[i]->timer){
            if (botoes[i]->timer > 1 ) botoes[i]->timer--;
            else{
                geral->cena = botoes[i]->indice;
                botoes[i]->timer = 0;
            }
        }
        SDL_Rect retangulo_colisao;
        AtribuirFRectInRectA(&botoes[i]->retangulo,&retangulo_colisao);
        if(SDL_PointInRect(&geral->ponto_mouse, &retangulo_colisao))
            botoes[i]->sobre = true;
        else {
            botoes[i]->sobre = false;
        }

        if(botoes[i]->sobre && geral->botao_mouse_direito){
            botoes[i]->timer=15;
        }
    }
    
    SDL_GetMouseState(&geral->mouse_x, &geral->mouse_y);
    geral->ponto_mouse.x = geral->mouse_x;
    geral->ponto_mouse.y = geral->mouse_y;

}


void CenaMenuDesenhar(VariveisGerais *geral, VariveisMenu *menu){
    Botao *botoes[] = {
        &menu->botao_iniciar,
        &menu->botao_conf,
        &menu->botao_sair
    };
    //limpeza de tela
    SDL_SetRenderDrawColor(geral->renderizador, menu->cor_fundo[0], menu->cor_fundo[1], menu->cor_fundo[2], 255);
    SDL_RenderClear(geral->renderizador);

    // botões
    DesenharMoldura(geral->renderizador, menu->moldura);
    for(int i = 0; i < 3; i++) DesenharBotao(geral->renderizador, *botoes[i]);
}