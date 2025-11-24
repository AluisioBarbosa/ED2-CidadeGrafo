#define _POSIX_C_SOURCE 200809L
#include "argumentos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct dadosArgumentos{
    char* DIR_ENTRADA; // -e opcional, se nao tiver levar em consideracao o diretorio corrente
    char* nomeGeoFILE;  // -f obrigatorio, deve tar no diretorio anterior -e
    char* nomeQryFILE; // -q opcional, deve tar no mesmo diretorio anterior -e
    char* DIR_SAIDA; // -o obrigatorio, onde o .svg e o .txt vao aparecer, se nao tiver usar diretorio corrente
    char* camVia; // -v opcional, deve tar no mesmo diretorio anterior -e

    char* caminhoQRY;
    char* caminhoGEO;
    char* caminhoVIA;
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
    args->camVia = NULL;
    args->caminhoQRY = NULL;
    args->caminhoGEO = NULL;
    args->caminhoVIA = NULL;
    return args;
}

static char* montarCaminho(char* diretorio, char* arquivo){
    // se começar com ./ ignora o ./ para montar o caminho
    if(strncmp(arquivo, "./", 2) == 0){
        arquivo += 2;
    }

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

static char* gerarCaminhoVia(char* diretorio, char* nomeGeo) {
    if (nomeGeo == NULL) {
        return NULL;
    }

    char* ponto = strrchr(nomeGeo, '.');
    size_t tamanhoBase;

    if (ponto != NULL) {
        tamanhoBase = ponto - nomeGeo;
    } else {
        tamanhoBase = strlen(nomeGeo); 
    }

    char* nomeViaTemp = (char*) malloc(tamanhoBase + 7);
    if (nomeViaTemp == NULL) {
        printf("Erro critico: Falha de alocacao de memoria para nome via\n");
        exit(1);
    }

    strncpy(nomeViaTemp, nomeGeo, tamanhoBase);
    nomeViaTemp[tamanhoBase] = '\0';
    strcat(nomeViaTemp, "-v.via");

    char* caminhoFinal = montarCaminho(diretorio, nomeViaTemp);
    free(nomeViaTemp);

    return caminhoFinal;
}


DadosArgumentos* processarArgumentos(int argc, char* argv[]){
    DadosArgumentos* args = criarEstruturaArgumentos();
    for(int i = 1; i < argc; i += 2){
        if(strcmp(argv[i], "-e") == 0){
            args->DIR_ENTRADA = strdup(argv[i + 1]);
        }
        else if(strcmp(argv[i], "-f") == 0){
            args->nomeGeoFILE = strdup(argv[i + 1]);
        }
        else if(strcmp(argv[i], "-q") == 0){
            args->nomeQryFILE = strdup(argv[i + 1]);
        }
        else if(strcmp(argv[i], "-o") == 0){
            args->DIR_SAIDA = strdup(argv[i + 1]);
        }
        else if(strcmp(argv[i], "-v") == 0){
            args->camVia = strdup(argv[i + 1]);
        }
        else{
            printf("Flag de entrada %s desconhecida\n", argv[i]);
        }
    }


    if(args->nomeGeoFILE == NULL || args->DIR_SAIDA == NULL){
        printf("ERRO: Os parametros obrigatorios do programa não foram todos fornecidos\n");
        exit(1);
    }

    // se nao for informado diretorio de entrada usa o corrente
    if(args->DIR_ENTRADA == NULL){
        args->DIR_ENTRADA = strdup("./");
    }

    args->caminhoGEO = montarCaminho(args->DIR_ENTRADA, args->nomeGeoFILE);

    if(args->nomeQryFILE != NULL){
        args->caminhoQRY = montarCaminho(args->DIR_ENTRADA, args->nomeQryFILE);
    } 
    else {
        args->caminhoQRY = NULL;
    }

    if(args->camVia != NULL){
        args->caminhoVIA = gerarCaminhoVia(args->DIR_ENTRADA, args->nomeGeoFILE);
    } 
    else {
        args->caminhoVIA = NULL;
    }

    return args;
}


void destruirArgumentos(DadosArgumentos* args){
    free(args->DIR_ENTRADA);
    free(args->DIR_SAIDA);
    free(args->nomeGeoFILE);
    free(args->nomeQryFILE);
    free(args->caminhoQRY);
    free(args->caminhoGEO);
    free(args->caminhoVIA);
    free(args);
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
char* getCaminhoVia(DadosArgumentos* args){
    return args->caminhoVIA;
}

// Debugar

void printArgumentos(DadosArgumentos* args){
    printf("Diretorio de entrada: %s\n", args->DIR_ENTRADA);
    printf("Nome do arquivo .geo: %s\n", args->nomeGeoFILE);
    printf("Nome do arquivo .qry: %s\n", args->nomeQryFILE);
    printf("Diretorio de saida: %s\n", args->DIR_SAIDA);
    printf("Caminho completo do .geo: %s\n", args->caminhoGEO);
    printf("Caminho completo do .qry: %s\n", args->caminhoQRY);
    printf("Caminho completo do .via: %s\n", args->caminhoVIA);
}