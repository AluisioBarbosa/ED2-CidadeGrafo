#include "argumentos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct dadosArgumentos{
    char* DIR_ENTRADA; // -e opcional, se nao tiver levar em consideracao o diretorio corrente
    char* nomeGeoFILE;  // -f obrigatorio, deve tar no diretorio anterior -e
    char* nomeQryFILE; // -q obrigatorio, deve tar no mesmo diretorio anterior -e
    char* DIR_SAIDA; // -o opicional, onde o .svg e o .txt vao aparecer, se nao tiver usar diretorio corrente

    char* caminhoQRY;
    char* caminhoGEO;
};

DadosArgumentos* criarEstruturaArgumentos(DadosArgumentos* args){
    args->DIR_ENTRADA = NULL;
    args->nomeGeoFILE = NULL;
    args->nomeQryFILE = NULL;
    args->DIR_SAIDA = NULL;
    args->caminhoQRY = NULL;
    args->caminhoGEO = NULL;
}

void processarArgumentos(int argc, char* argv[], DadosArgumentos* args){
    for(int i = 1; i < argc; i++){


    }




}





void destruirArgumentos(DadosArgumentos* args){
    if(args->DIR_ENTRADA != NULL){
        free(args->DIR_ENTRADA);
    }
    if(args->DIR_SAIDA != NULL){
        free(args->DIR_SAIDA);
    }

    free(args->nomeGeoFILE);
    free(args->nomeQryFILE);
    free(args->caminhoQRY);
    free(args->caminhoGEO);
}


