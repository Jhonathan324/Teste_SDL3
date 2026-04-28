#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "gerais.h"


void InitMenu(VariveisGerais *geral, VariveisMenu *menu){
    //Criação do menu para os botões
    SDL_FRect rect_moldura = {0,0,300,300};
    
    //obtenção do rect da janela
    int janela_x, janela_y, janela_w, janela_h;
    SDL_GetWindowSize(geral->janela,&janela_w,&janela_h);
    SDL_FRect rect_janela = {0, 20, janela_w, janela_h};
    CentralizarRectInRect(&rect_janela, &rect_moldura);//centralização do menu com base na tela

    // Criação do menu
    menu->moldura = InitMoldura(geral->renderizador, &rect_moldura,50,"imagens/moldura de madeira.png");

    //Criação dos botões
    menu->botao_iniciar = (Botao){0, "Iniciar Jogo",  (SDL_FRect){0,0,100,30}, {70,70,70}, {30,30,30},CENA_JOGO,0};
    menu->botao_conf    = (Botao){0, "Configuracoes", (SDL_FRect){0,0,100,30}, {70,70,70}, {30,30,30},CENA_CONF,0};
    menu->botao_sair    = (Botao){0, "Sair do Jogo",  (SDL_FRect){0,0,100,30}, {70,70,70}, {30,30,30},CENA_SAIR,0};
    
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
        
    //pegando a fonte
    TTF_Font *fonte = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 244);
    SDL_Color cor_fonte = {255,255,255,255};

    //atribuindo o texto e uma imagem a cada botão
    for(int i = 0; i < 3; i++){
        SDL_Surface *surface_texto = TTF_RenderText_Solid(fonte, botoes[i]->texto, 0, cor_fonte);
        botoes[i]->textura = SDL_CreateTextureFromSurface(geral->renderizador, surface_texto);
        botoes[i]->imagem = IMG_LoadTexture(geral->renderizador, "imagens/botão.png");
        SDL_SetTextureScaleMode(botoes[i]->imagem,SDL_SCALEMODE_NEAREST);
    }
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