#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "gerais.h"


void GetTamanhos(int *escala, float (*tamanho_tela)[2], float (*tamanho_menu)[2], float (*tamanho_botao_menu)[2], float (*tamanho_jogador)[2], float (*tamanho_inimigo1)[2], float (*tamanho_inimigo2)[2]){
    switch (*escala){
    case 0:
        (*tamanho_tela)[0] = 640;
        (*tamanho_tela)[1]= 360;
        break;

    case 1:
        (*tamanho_tela)[0] = 854;
        (*tamanho_tela)[1]= 480;
        break;

    case 2:
        (*tamanho_tela)[0] = 1280;
        (*tamanho_tela)[1]= 720;
        break;

    case 3:
        (*tamanho_tela)[0] = 1366;
        (*tamanho_tela)[1]= 768;
        break;

    case 4:
        (*tamanho_tela)[0] = 1600;
        (*tamanho_tela)[1]= 900;
        break;

    case 5:
        (*tamanho_tela)[0] = 1920;
        (*tamanho_tela)[1]= 1080;
        break;

    case 6:
        (*tamanho_tela)[0] = 2560;
        (*tamanho_tela)[1]= 1440;
        break;

    default:
        *escala = 0;
        (*tamanho_tela)[0] = 640;
        (*tamanho_tela)[1]= 360;
        break;
    }
}

void AtribuirFRectInRectA(SDL_FRect *fretangulo, SDL_Rect *retangulo){
    retangulo->x = fretangulo->x;
    retangulo->y = fretangulo->y;
    retangulo->h = fretangulo->h;
    retangulo->w = fretangulo->w;
}

Moldura InitMoldura(SDL_Renderer *renderer, SDL_FRect *retangulo, int tamanho_canto,char *file){
    if(retangulo->w < tamanho_canto*2){
        retangulo->w = tamanho_canto*2;
    }
    if(retangulo->h < tamanho_canto*2){
        retangulo->h = tamanho_canto*2;
    }
    SDL_Texture *textura = IMG_LoadTexture(renderer, file);
    SDL_SetTextureScaleMode(textura,SDL_SCALEMODE_NEAREST);
    Moldura moldura = {*retangulo,textura};
    int x = retangulo->x;
    int y = retangulo->y;
    int w = retangulo->w;
    int h = retangulo->h;
    moldura.partes[0][0]= (SDL_FRect){x                 , y                , tamanho_canto     , tamanho_canto    };
    moldura.partes[0][1]= (SDL_FRect){x+tamanho_canto   , y                , w-2*tamanho_canto , tamanho_canto    };
    moldura.partes[0][2]= (SDL_FRect){x+w-tamanho_canto , y                , tamanho_canto     , tamanho_canto    };
    moldura.partes[1][0]= (SDL_FRect){x                 , y+tamanho_canto  , tamanho_canto     , h-2*tamanho_canto};
    moldura.partes[1][1]= (SDL_FRect){x+tamanho_canto   , y+tamanho_canto  , w-2*tamanho_canto , h-2*tamanho_canto};
    moldura.partes[1][2]= (SDL_FRect){x+w-tamanho_canto , y+tamanho_canto  , tamanho_canto     , h-2*tamanho_canto};
    moldura.partes[2][0]= (SDL_FRect){x                 , y+h-tamanho_canto, tamanho_canto     , tamanho_canto    };
    moldura.partes[2][1]= (SDL_FRect){x+tamanho_canto   , y+h-tamanho_canto, w-2*tamanho_canto , tamanho_canto    };
    moldura.partes[2][2]= (SDL_FRect){x+w-tamanho_canto , y+h-tamanho_canto, tamanho_canto     , tamanho_canto    };
    return moldura;
}

void DesenharMoldura(SDL_Renderer *renderer, Moldura moldura){
    for(int i = 0; i < 3; i++)for(int j = 0; j < 3; j++)
    SDL_RenderTextureRotated(
        renderer,
        moldura.textura,
        &(SDL_FRect){EscalaMoldura*(2-j),EscalaMoldura*(2-i),EscalaMoldura,EscalaMoldura},
        &moldura.partes[i][j],
        180.0,
        NULL,
        SDL_FLIP_NONE);
}

void DesenharBotao(SDL_Renderer *renderizador, Botao botao){
    if(!botao.imagem){
        if(!botao.sobre && botao.cor){
            SDL_SetRenderDrawColor(renderizador, botao.cor[0], botao.cor[1], botao.cor[2], 255);
        }
        else if(botao.cor2){
            SDL_SetRenderDrawColor(renderizador, botao.cor2[0], botao.cor2[1], botao.cor2[2], 255);
        }
        SDL_RenderFillRect(renderizador,&botao.retangulo);
    }
    else SDL_RenderTexture(renderizador, botao.imagem,NULL,&botao.retangulo);
    if(botao.textura){
        SDL_FRect retangulo_texto = botao.retangulo;
        retangulo_texto.w *= 0.8;
        retangulo_texto.h *= 0.8;
        retangulo_texto.x = (botao.retangulo.x + (botao.retangulo.w - retangulo_texto.w)/2);
        retangulo_texto.y = (botao.retangulo.y + (botao.retangulo.h - retangulo_texto.h)/2);
        SDL_RenderTexture(renderizador,botao.textura, NULL, &retangulo_texto);
    } 

}

void ModuloEvento(VariveisGerais *jogo){
    if (jogo->evento.type == SDL_EVENT_QUIT) {
                jogo->rodando = false;
            }
            if(jogo->evento.type == SDL_EVENT_MOUSE_BUTTON_DOWN) jogo->botao_mouse_direito = true;
            else jogo->botao_mouse_direito = false;
    
}

void CentralizarRectInRect(SDL_FRect *pai, SDL_FRect *filho){
    filho->x = pai->x + (pai->w-filho->w)/2;
    filho->y = pai->y + (pai->h-filho->h)/2;
}

void CentralizarRectsInRectV(SDL_FRect *pai, SDL_FRect *filho[], int n, float borda_x, float borda_y){
    borda_x = borda_x * pai->w;
    borda_y = borda_y * pai->h;
    float soma = 0;
    for(int i = 0; i < n; i++) soma += filho[i]-> h;
    float espaco_interno_remanecente = (pai->h-borda_y*2) - soma;
    float espacamento = espaco_interno_remanecente / (n-1);
    for(int i = 0; i < n; i++){
        filho[i]->x = pai->x + (pai->w-filho[i]->w)/2;
        if(i!=0){
            filho[i]->y = filho[i-1]->y + filho[i-1]->h + espacamento;
        }
        else {
            filho[i]->y = pai->y + borda_y;
        }

    }
}
