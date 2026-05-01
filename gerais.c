#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "gerais.h"

// Para eu não me perder as funções deste .c esta organizado na mesma ordem das funções do gerais.h

void GetTamanhos(TAMANHOS *tamanhos)
{
    switch (tamanhos->escala)
    {
    case 0:
        tamanhos->tamanho_tela[0] = 640;
        tamanhos->tamanho_tela[1] = 360;
        break;

    case 1:
        tamanhos->tamanho_tela[0] = 960;
        tamanhos->tamanho_tela[1] = 540;
        break;

    case 2:
        tamanhos->tamanho_tela[0] = 1280;
        tamanhos->tamanho_tela[1] = 720;
        break;

    case 3:
        tamanhos->tamanho_tela[0] = 1600;
        tamanhos->tamanho_tela[1] = 900;
        break;

    case 4:
        tamanhos->tamanho_tela[0] = 1920;
        tamanhos->tamanho_tela[1] = 1080;
        break;

    case 5:
        tamanhos->tamanho_tela[0] = 2240;
        tamanhos->tamanho_tela[1] = 1260;
        break;

    case 6:
        tamanhos->tamanho_tela[0] = 2560;
        tamanhos->tamanho_tela[1] = 1440;
        break;

    default:
        tamanhos->escala = 0;
        tamanhos->tamanho_tela[0] = 640;
        tamanhos->tamanho_tela[1] = 360;
        break;
    }
    tamanhos->tamanho_menu[0]       = tamanhos->tamanho_tela[0] * (float)48 / 640 * 6;
    tamanhos->tamanho_menu[1]       = tamanhos->tamanho_tela[1] * (float)48 / 360 * 6;

    tamanhos->tamanho_botao1[0] = tamanhos->tamanho_tela[0] * (float)24 / 640;
    tamanhos->tamanho_botao1[1] = tamanhos->tamanho_tela[1] * (float)24 / 360;
    
    tamanhos->tamanho_bloco[0]      = tamanhos->tamanho_tela[0] * (float)64 / 640;
    tamanhos->tamanho_bloco[1]      = tamanhos->tamanho_tela[1] * (float)64 / 360;

    tamanhos->tamanho_jogador[0]    = tamanhos->tamanho_tela[0] * (float)36 / 640;
    tamanhos->tamanho_jogador[1]    = tamanhos->tamanho_tela[1] * (float)36 / 360;
    
    tamanhos->tamanho_inimigo1[0]   = tamanhos->tamanho_tela[0] * (float)36 / 640;
    tamanhos->tamanho_inimigo1[1]   = tamanhos->tamanho_tela[1] * (float)36 / 360;

    tamanhos->tamanho_inimigo2[0]   = tamanhos->tamanho_tela[0] * (float)36 / 640;
    tamanhos->tamanho_inimigo2[1]   = tamanhos->tamanho_tela[1] * (float)36 / 360;
}

void AtribuirFRectInRectA(SDL_FRect *fretangulo, SDL_Rect *retangulo)
{
    retangulo->x = fretangulo->x;
    retangulo->y = fretangulo->y;
    retangulo->h = fretangulo->h;
    retangulo->w = fretangulo->w;
}

void CentralizarRectInRect(SDL_FRect *pai, SDL_FRect *filho)
{
    filho->x = pai->x + (pai->w - filho->w) / 2;
    filho->y = pai->y + (pai->h - filho->h) / 2;
}


void CentralizarRectsInRectV(SDL_FRect *pai, SDL_FRect *filho[], int n, float borda_x, float borda_y)
{
    borda_x = borda_x * pai->w;
    borda_y = borda_y * pai->h;
    float soma = 0;
    for (int i = 0; i < n; i++)
        soma += filho[i]->h;
    float espaco_interno_remanecente = (pai->h - borda_y * 2) - soma;
    float espacamento = espaco_interno_remanecente / (n - 1);
    for (int i = 0; i < n; i++)
    {
        filho[i]->x = pai->x + (pai->w - filho[i]->w) / 2;
        if (i != 0)
        {
            filho[i]->y = filho[i - 1]->y + filho[i - 1]->h + espacamento;
        }
        else
        {
            filho[i]->y = pai->y + borda_y;
        }
    }
}



Moldura InitMoldura(SDL_Renderer *renderizador, SDL_FRect *retangulo, char *file)
{
    Moldura moldura = {*retangulo, IMG_LoadTexture(renderizador, file)};
    SDL_SetTextureScaleMode(moldura.textura , SDL_SCALEMODE_NEAREST);

    return moldura;
}

void CalcularMolduraPartes(Moldura *moldura, float tamanho_canto)
{
    if (moldura->retangulo.w < tamanho_canto * 2)
    {
        moldura->retangulo.w = tamanho_canto * 2;
    }
    if (moldura->retangulo.h < tamanho_canto * 2)
    {
        moldura->retangulo.h = tamanho_canto * 2;
    }
    int x = moldura->retangulo.x;
    int y = moldura->retangulo.y;
    int w = moldura->retangulo.w;
    int h = moldura->retangulo.h;
    moldura->partes[0][0] = (SDL_FRect){x, y, tamanho_canto, tamanho_canto};
    moldura->partes[0][1] = (SDL_FRect){x + tamanho_canto, y, w - 2 * tamanho_canto, tamanho_canto};
    moldura->partes[0][2] = (SDL_FRect){x + w - tamanho_canto, y, tamanho_canto, tamanho_canto};
    moldura->partes[1][0] = (SDL_FRect){x, y + tamanho_canto, tamanho_canto, h - 2 * tamanho_canto};
    moldura->partes[1][1] = (SDL_FRect){x + tamanho_canto, y + tamanho_canto, w - 2 * tamanho_canto, h - 2 * tamanho_canto};
    moldura->partes[1][2] = (SDL_FRect){x + w - tamanho_canto, y + tamanho_canto, tamanho_canto, h - 2 * tamanho_canto};
    moldura->partes[2][0] = (SDL_FRect){x, y + h - tamanho_canto, tamanho_canto, tamanho_canto};
    moldura->partes[2][1] = (SDL_FRect){x + tamanho_canto, y + h - tamanho_canto, w - 2 * tamanho_canto, tamanho_canto};
    moldura->partes[2][2] = (SDL_FRect){x + w - tamanho_canto, y + h - tamanho_canto, tamanho_canto, tamanho_canto};
}

void DesenharMoldura(SDL_Renderer *renderizador, Moldura moldura)
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            SDL_RenderTexture(
                renderizador,
                moldura.textura,
                &(SDL_FRect){EscalaMoldura * j, EscalaMoldura * i, EscalaMoldura, EscalaMoldura},
                &moldura.partes[i][j]);
}

Marcador InitMarcador(SDL_Renderer *renderizador, SDL_FRect *retangulo, bool ativo, char *imagem1, SDL_Color cor1, SDL_Color cor2){
    Marcador marcador = {
        *retangulo,
        0,
        false,
        ativo,
        cor1,
        cor2,
        IMG_LoadTexture(renderizador, imagem1)
    };
    SDL_SetTextureScaleMode(marcador.imagem1 , SDL_SCALEMODE_NEAREST);
    return marcador;
}

bool VerificarMarcador(Marcador *marcador, SDL_Point mouse, bool click){
    if (marcador->timer)
    {
        if (marcador->timer > 1)
            marcador->timer--;

        else
        {
            marcador->timer = 0;
            marcador->ativo = !marcador->ativo;
            return true;
        }
    }

    // Cria um retangulo para verificar as colisões como o mouse
    SDL_Rect retangulo_colisao;
    AtribuirFRectInRectA(&marcador->retangulo, &retangulo_colisao);

    // verifica se o mouse está emcima do botão
    if (SDL_PointInRect(&mouse, &retangulo_colisao))
        marcador->sobre = true;
    else
        marcador->sobre = false;

    // verifica se o botão de sair foi clicado
    if (marcador->sobre && click)
        marcador->timer = 15;

    return false;
}

void DesenharMarcador(SDL_Renderer *renderizador, Marcador marcador){
    if(marcador.ativo) SDL_RenderTexture(renderizador, marcador.imagem1, &(SDL_FRect){EscalaMarcador ,0,EscalaMarcador ,EscalaMarcador }, &marcador.retangulo);
    else SDL_RenderTexture(renderizador, marcador.imagem1, &(SDL_FRect){0,0,EscalaMarcador ,EscalaMarcador }, &marcador.retangulo);
    if(marcador.sobre){
        SDL_SetRenderDrawBlendMode(
                renderizador,
                SDL_BLENDMODE_BLEND
            );
            SDL_SetRenderDrawColor(renderizador, marcador.cor2.r, marcador.cor2.g, marcador.cor2.b, marcador.cor2.a);
            SDL_RenderFillRect(renderizador, &marcador.retangulo);
    }
}

Botao InitBotao(SDL_Renderer *renderizador, SDL_FRect *retangulo, char *imagem, char *texto, SDL_Color cor1, SDL_Color cor2, int indice, TTF_Font *fonte, SDL_Color cor_fonte)
{
    // texto
    int x, y;
    float proporcao = 1;
    SDL_Texture *textura_texto;
    if (texto)
    {
        textura_texto = SDL_CreateTextureFromSurface(renderizador, TTF_RenderText_Solid(fonte, texto, 0, cor_fonte));
        TTF_GetStringSize(fonte, texto, 0, &x, &y);
        proporcao = (float)x / y;
        retangulo->w = retangulo->h * proporcao;
    }
    // Botão

    Botao botao = {
        *retangulo, // retangulo
        texto,      // texto
        proporcao,  // proporcao (porporção entre a largura sobre altura)
        0,          // timer
        false,      // sobre (verificar se o mouse esta sobre)
        indice,     // indice
        cor1,       // cor1 e cor2
        cor2,
        textura_texto,                          // textura texto
        IMG_LoadTexture(renderizador, imagem)}; // textura da imagem

    float tamanho_canto;
    if (retangulo->h >= retangulo->w)
        tamanho_canto = retangulo->w / 2;
    else
        tamanho_canto = retangulo->h / 2;

    // imagem
    SDL_SetTextureScaleMode(botao.imagem, SDL_SCALEMODE_NEAREST);
    return botao;
}

void CalcularBotaoPartes(Botao *botao)
{
    float tamanho_canto;
    float x = botao->retangulo.x;
    float y = botao->retangulo.y;
    float w = botao->retangulo.w;
    float h = botao->retangulo.h;

    if (h >= w)
        tamanho_canto = w / 2;
    else
        tamanho_canto = h / 2;
    botao->partes[0][0] = (SDL_FRect){x, y, tamanho_canto, tamanho_canto};
    botao->partes[0][1] = (SDL_FRect){x + tamanho_canto, y, w - 2 * tamanho_canto, tamanho_canto};
    botao->partes[0][2] = (SDL_FRect){x + w - tamanho_canto, y, tamanho_canto, tamanho_canto};
    botao->partes[1][0] = (SDL_FRect){x, y + tamanho_canto, tamanho_canto, h - 2 * tamanho_canto};
    botao->partes[1][1] = (SDL_FRect){x + tamanho_canto, y + tamanho_canto, w - 2 * tamanho_canto, h - 2 * tamanho_canto};
    botao->partes[1][2] = (SDL_FRect){x + w - tamanho_canto, y + tamanho_canto, tamanho_canto, h - 2 * tamanho_canto};
    botao->partes[2][0] = (SDL_FRect){x, y + h - tamanho_canto, tamanho_canto, tamanho_canto};
    botao->partes[2][1] = (SDL_FRect){x + tamanho_canto, y + h - tamanho_canto, w - 2 * tamanho_canto, tamanho_canto};
    botao->partes[2][2] = (SDL_FRect){x + w - tamanho_canto, y + h - tamanho_canto, tamanho_canto, tamanho_canto};
}

void DesenharBotao(SDL_Renderer *renderizador, Botao botao)
{
    if (!botao.imagem)
    {
        if (!botao.sobre)
        {
            SDL_SetRenderDrawColor(renderizador, botao.cor1.r, botao.cor1.g, botao.cor1.b, botao.cor1.a);
        }
        else
        {
            SDL_SetRenderDrawColor(renderizador, botao.cor2.r, botao.cor2.g, botao.cor2.b, botao.cor2.a);
        }
        SDL_RenderFillRect(renderizador, &botao.retangulo);
    }
    else
    {
        if (botao.partes)
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    SDL_RenderTexture(
                        renderizador,
                        botao.imagem,
                        &(SDL_FRect){EscalaBotao * j, EscalaBotao * i, EscalaBotao, EscalaBotao},
                        &botao.partes[i][j]);
        else
            SDL_RenderTexture(
                renderizador,
                botao.imagem,
                NULL,
                &botao.retangulo);

        if(botao.sobre){
            SDL_SetRenderDrawBlendMode(
                renderizador,
                SDL_BLENDMODE_BLEND
            );
            SDL_SetRenderDrawColor(renderizador, botao.cor2.r, botao.cor2.g, botao.cor2.b, botao.cor2.a);
            SDL_RenderFillRect(renderizador, &botao.retangulo);
        }
    }
    if (botao.textura)
    {
        SDL_FRect retangulo_texto = botao.retangulo;
        retangulo_texto.h *= 0.8;
        retangulo_texto.w = retangulo_texto.h * botao.proporcao;
        retangulo_texto.x = (botao.retangulo.x + (botao.retangulo.w - retangulo_texto.w) / 2);
        retangulo_texto.y = (botao.retangulo.y + (botao.retangulo.h - retangulo_texto.h) / 2);
        SDL_RenderTexture(renderizador, botao.textura, NULL, &retangulo_texto);
    }
}

bool VerificarBotao(Botao *botao, SDL_Point mouse, bool click)
{
    if (botao->timer)
    {
        if (botao->timer > 1)
            botao->timer--;

        else
        {
            botao->timer = 0;
            return true;
        }
    }

    // Cria um retangulo para verificar as colisões como o mouse
    SDL_Rect retangulo_colisao;
    AtribuirFRectInRectA(&botao->retangulo, &retangulo_colisao);

    // verifica se o mouse está emcima do botão
    if (SDL_PointInRect(&mouse, &retangulo_colisao))
        botao->sobre = true;
    else
        botao->sobre = false;

    // verifica se o botão de sair foi clicado
    if (botao->sobre && click)
        botao->timer = 15;

    return false;
}

BotaoExpansivo InitBotaoExpansivo(SDL_Renderer *renderizador, SDL_FRect *retangulo, char *imagem, char *texto, char *textos[], SDL_Color cor1, SDL_Color cor2, int indice, TTF_Font *fonte, SDL_Color cor_fonte, int n)
{
    // texto
    int x, y;
    float proporcao = 1;
    SDL_Texture *textura_texto;
    if (texto)
    {
        SDL_Surface *surface_texto = TTF_RenderText_Solid(fonte, texto, 0, cor_fonte);
        textura_texto = SDL_CreateTextureFromSurface(renderizador, surface_texto);
        TTF_GetStringSize(fonte, texto, 0, &x, &y);
        proporcao = (float)x / y;
        retangulo->w = retangulo->h * proporcao;
    }
    // Botão

    Botao botao = {
        *retangulo, // retangulo
        texto,      // texto
        proporcao,  // proporcao (porporção entre a largura sobre altura)
        0,          // timer
        false,      // sobre (verificar se o mouse esta sobre)
        indice,     // indice
        cor1,       // cor1 e cor2
        cor2,
        textura_texto,                          // textura texto
        IMG_LoadTexture(renderizador, imagem)}; // textura da imagem

    float tamanho_canto;
    if (retangulo->h >= retangulo->w)
        tamanho_canto = retangulo->w / 2;
    else
        tamanho_canto = retangulo->h / 2;

    // imagem
    SDL_SetTextureScaleMode(botao.imagem, SDL_SCALEMODE_NEAREST);

    Botao *botoes = malloc(sizeof(Botao) * n);
    for (int i = 0; i < n; i++)
    {
        botoes[i] = botao;
        botoes[i].texto = textos[i];
        botoes[i].retangulo.y = botao.retangulo.y + botao.retangulo.h * (i + 1);

        SDL_Surface *surface_texto = TTF_RenderText_Solid(fonte, textos[i], 0, cor_fonte);
        botoes[i].textura = SDL_CreateTextureFromSurface(renderizador, surface_texto);
        TTF_GetStringSize(fonte, textos[i], 0, &x, &y);
        botoes[i].proporcao = (float)x / y;
        // printf("%s ",botoes[i].texto);
        // printf("%f\n",botoes[i].retangulo.y);
    }
    BotaoExpansivo botao_e = {n, botao, botoes, false};
    return botao_e;
}

void CalcularBotaoExpansivoPartes(BotaoExpansivo *botao)
{
    float tamanho_canto;
    float x = botao->botao_pai.retangulo.x;
    float y = botao->botao_pai.retangulo.y;
    float w = botao->botao_pai.retangulo.w;
    float h = botao->botao_pai.retangulo.h;

    if (h >= w)
        tamanho_canto = w / 2;
    else
        tamanho_canto = h / 2;
    botao->botao_pai.partes[0][0] = (SDL_FRect){x, y, tamanho_canto, tamanho_canto};
    botao->botao_pai.partes[0][1] = (SDL_FRect){x + tamanho_canto, y, w - 2 * tamanho_canto, tamanho_canto};
    botao->botao_pai.partes[0][2] = (SDL_FRect){x + w - tamanho_canto, y, tamanho_canto, tamanho_canto};
    botao->botao_pai.partes[1][0] = (SDL_FRect){x, y + tamanho_canto, tamanho_canto, h - 2 * tamanho_canto};
    botao->botao_pai.partes[1][1] = (SDL_FRect){x + tamanho_canto, y + tamanho_canto, w - 2 * tamanho_canto, h - 2 * tamanho_canto};
    botao->botao_pai.partes[1][2] = (SDL_FRect){x + w - tamanho_canto, y + tamanho_canto, tamanho_canto, h - 2 * tamanho_canto};
    botao->botao_pai.partes[2][0] = (SDL_FRect){x, y + h - tamanho_canto, tamanho_canto, tamanho_canto};
    botao->botao_pai.partes[2][1] = (SDL_FRect){x + tamanho_canto, y + h - tamanho_canto, w - 2 * tamanho_canto, tamanho_canto};
    botao->botao_pai.partes[2][2] = (SDL_FRect){x + w - tamanho_canto, y + h - tamanho_canto, tamanho_canto, tamanho_canto};
    for (int i = 0; i < botao->n; i++)
    {
        x = botao->botao_filho[i].retangulo.x;
        y = botao->botao_filho[i].retangulo.y;
        w = botao->botao_filho[i].retangulo.w;
        h = botao->botao_filho[i].retangulo.h;

        if (h >= w)
            tamanho_canto = w / 2;
        else
            tamanho_canto = h / 2;
        botao->botao_filho[i].partes[0][0] = (SDL_FRect){x, y, tamanho_canto, tamanho_canto};
        botao->botao_filho[i].partes[0][1] = (SDL_FRect){x + tamanho_canto, y, w - 2 * tamanho_canto, tamanho_canto};
        botao->botao_filho[i].partes[0][2] = (SDL_FRect){x + w - tamanho_canto, y, tamanho_canto, tamanho_canto};
        botao->botao_filho[i].partes[1][0] = (SDL_FRect){x, y + tamanho_canto, tamanho_canto, h - 2 * tamanho_canto};
        botao->botao_filho[i].partes[1][1] = (SDL_FRect){x + tamanho_canto, y + tamanho_canto, w - 2 * tamanho_canto, h - 2 * tamanho_canto};
        botao->botao_filho[i].partes[1][2] = (SDL_FRect){x + w - tamanho_canto, y + tamanho_canto, tamanho_canto, h - 2 * tamanho_canto};
        botao->botao_filho[i].partes[2][0] = (SDL_FRect){x, y + h - tamanho_canto, tamanho_canto, tamanho_canto};
        botao->botao_filho[i].partes[2][1] = (SDL_FRect){x + tamanho_canto, y + h - tamanho_canto, w - 2 * tamanho_canto, tamanho_canto};
        botao->botao_filho[i].partes[2][2] = (SDL_FRect){x + w - tamanho_canto, y + h - tamanho_canto, tamanho_canto, tamanho_canto};
        // printf("%s ",botao->botao_filho[i].texto);
        // printf("%f \n",botao->botao_filho[i].retangulo.y);
    }
}

void DesenharBotaoExpansivo(SDL_Renderer *renderizador, BotaoExpansivo botao)
{
    if (!botao.botao_pai.imagem){
        SDL_SetRenderDrawColor(renderizador, botao.botao_pai.cor1.r, botao.botao_pai.cor1.g, botao.botao_pai.cor1.b, botao.botao_pai.cor1.a);
        SDL_RenderFillRect(renderizador, &botao.botao_pai.retangulo);
    }
    else
    {
        if (botao.botao_pai.partes)
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    SDL_RenderTexture(
                        renderizador,
                        botao.botao_pai.imagem,
                        &(SDL_FRect){EscalaBotao * j, EscalaBotao * i, EscalaBotao, EscalaBotao},
                        &botao.botao_pai.partes[i][j]);
        else
            SDL_RenderTexture(
                renderizador,
                botao.botao_pai.imagem,
                NULL,
                &botao.botao_pai.retangulo);
    }
    if(botao.expandido){
        SDL_SetRenderDrawBlendMode(
            renderizador,
            SDL_BLENDMODE_BLEND
        );
        SDL_SetRenderDrawColor(renderizador, botao.botao_pai.cor2.r, botao.botao_pai.cor2.g, botao.botao_pai.cor2.b, botao.botao_pai.cor2.a);
        SDL_RenderFillRect(renderizador, &botao.botao_pai.retangulo);
    }
    

    if (botao.botao_pai.textura)
    {
        SDL_FRect retangulo_texto = botao.botao_pai.retangulo;
        retangulo_texto.h *= 0.8;
        retangulo_texto.w = retangulo_texto.h * botao.botao_pai.proporcao;
        retangulo_texto.x = (botao.botao_pai.retangulo.x + (botao.botao_pai.retangulo.w - retangulo_texto.w) / 2);
        retangulo_texto.y = (botao.botao_pai.retangulo.y + (botao.botao_pai.retangulo.h - retangulo_texto.h) / 2);
        SDL_RenderTexture(renderizador, botao.botao_pai.textura, NULL, &retangulo_texto);
    }
    for (int i1 = 0; i1 < botao.n && botao.expandido; i1++)
    {
        if (!botao.botao_filho[i1].imagem)
        {
            if (!botao.botao_filho[i1].sobre)
            {
                SDL_SetRenderDrawColor(renderizador, botao.botao_filho[i1].cor1.r, botao.botao_filho[i1].cor1.g, botao.botao_filho[i1].cor1.b, botao.botao_filho[i1].cor1.a);
            }
            else
            {
                SDL_SetRenderDrawColor(renderizador, botao.botao_filho[i1].cor2.r, botao.botao_filho[i1].cor2.g, botao.botao_filho[i1].cor2.b, botao.botao_filho[i1].cor2.a);
            }
            SDL_RenderFillRect(renderizador, &botao.botao_filho[i1].retangulo);
        }
        else
        {
            if (botao.botao_filho[i1].partes)
                for (int i = 0; i < 3; i++)
                    for (int j = 0; j < 3; j++)
                    {
                        SDL_RenderTexture(
                            renderizador,
                            botao.botao_filho[i1].imagem,
                            &(SDL_FRect){EscalaBotao * j, EscalaBotao * i, EscalaBotao, EscalaBotao},
                            &botao.botao_filho[i1].partes[i][j]);
                    }
            else
                SDL_RenderTexture(
                    renderizador,
                    botao.botao_filho[i1].imagem,
                    NULL,
                    &botao.botao_filho[i1].retangulo);

            if(botao.botao_filho[i1].sobre ){
            SDL_SetRenderDrawBlendMode(
                renderizador,
                SDL_BLENDMODE_BLEND
            );
            SDL_SetRenderDrawColor(renderizador, botao.botao_filho[i1].cor2.r, botao.botao_filho[i1].cor2.g, botao.botao_filho[i1].cor2.b, botao.botao_filho[i1].cor2.a);
            SDL_RenderFillRect(renderizador, &botao.botao_filho[i1].retangulo);
        }
        }
        
        if (botao.botao_filho[i1].textura)
        {
            SDL_FRect retangulo_texto = botao.botao_filho[i1].retangulo;
            retangulo_texto.h *= 0.8;
            retangulo_texto.w = retangulo_texto.h * botao.botao_filho[i1].proporcao;
            retangulo_texto.x = (botao.botao_filho[i1].retangulo.x + (botao.botao_filho[i1].retangulo.w - retangulo_texto.w) / 2);
            retangulo_texto.y = (botao.botao_filho[i1].retangulo.y + (botao.botao_filho[i1].retangulo.h - retangulo_texto.h) / 2);
            SDL_RenderTexture(renderizador, botao.botao_filho[i1].textura, NULL, &retangulo_texto);
        }
    }
}

void ModuloEvento(VariveisGerais *jogo)
{
    if (jogo->evento.type == SDL_EVENT_QUIT)
    {
        jogo->rodando = false;
    }
    if (jogo->evento.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
        jogo->botao_mouse_direito = true;
    else
        jogo->botao_mouse_direito = false;
}
