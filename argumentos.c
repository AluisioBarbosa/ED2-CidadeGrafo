#include "argumentos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct dadosArgumentos{
    char* DIR_ENTRADA; // -e opcional, se nao tiver levar em consideracao o diretorio corrente
    char* nomeGeoFILE;  // -f obrigatorio, deve tar no diretorio anterior -e
    char* nomeQryFILE; // -q obrigatorio, deve tar no mesmo diretorio anterior -e
    char* DIR_SAIDA; // -o opicional, onde o .svg e o .txt vao aparecer, se nao tiver usar diretorio corrente

    char* caminhoQRY;
    char* caminhoGEO;
};

DadosArgumentos* criarEstruturaArgumentos(){
    DadosArgumentos* args = (DadosArgumentos*)malloc(sizeof(DadosArgumentos));
    if(args == NULL){
        printf("Falha na alocacao de memoria para a estrutura de argumentos");
        exit(1);
    }
    
    args->DIR_ENTRADA = NULL;
    args->nomeGeoFILE = NULL;
    args->nomeQryFILE = NULL;
    args->DIR_SAIDA = NULL;
    args->caminhoQRY = NULL;
    args->caminhoGEO = NULL;
    return args;
}

static char* montarCaminho(char* diretorio, char* arquivo){
    size_t tamCaminho = strlen(diretorio) + strlen(arquivo) + 2;
    size_t tamDiretorio = strlen(diretorio);

    char* caminho = malloc(tamCaminho);
    
    if(caminho == NULL){
        printf("Erro na hora de alocacao do caminho para montar o caminho do diretorio\n");
        exit(1);
    }

    if(diretorio[tamDiretorio - 1] == '/'){
        snprintf(caminho, tamCaminho, "%s%s", diretorio, arquivo);
    }
    else{
        snprintf(caminho, tamCaminho, "%s/%s", diretorio, arquivo);
    }

    return caminho;
}



DadosArgumentos* processarArgumentos(int argc, char* argv[]){
    DadosArgumentos* args = criarEstruturaArgumentos();
    for(int i = 1; i < argc; i + 2){
        if(strcmp(argv[i], "-e") == true){
            args->DIR_ENTRADA = strdup(argv[i + 1]);
        }
        else if(strcmp(argv[i], "-f") == true){
            args->nomeGeoFILE = strdup(argv[i + 1]);
        }
        else if(strcmp(argv[i], "-q") == true){
            args->nomeQryFILE = strdup(argv[i + 1]);
        }
        else if(strcmp(argv[i], "-o") == true){
            args->DIR_SAIDA = strdup(argv[i + 1]);
        }
        else{
            printf("Flag de entrada %c desconhecida\n", argv[i]);
        }
    }


    if(args->nomeGeoFILE == NULL || args->nomeQryFILE == NULL){
        printf("ERRO: Os parametros obrigatorios do programa não foram todos fornecidos\n");
        exit(1);
    }

    // se nao for informado diretorio de entrada e/ou saida, usa o corrente
    if(args->DIR_ENTRADA == NULL){
        args->DIR_ENTRADA = strdup(".");
    }

    if(args->DIR_SAIDA == NULL){
        args->DIR_SAIDA = strdup("./");
    }

    args->caminhoGEO = montarCaminho(args->DIR_ENTRADA, args->nomeGeoFILE);
    args->caminhoQRY = montarCaminho(args->DIR_ENTRADA, args->nomeQryFILE);

    return args;
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

char* getDIRentrada(DadosArgumentos* args){
    return args->DIR_ENTRADA;
}

char* getCaminhoGeo(DadosArgumentos* args){
    return args->caminhoGEO;
}
char* getCaminhoQry(DadosArgumentos* args){
    return args->caminhoQRY;
}
char* getDIRsaida(DadosArgumentos* args){
    return args->DIR_SAIDA;
}