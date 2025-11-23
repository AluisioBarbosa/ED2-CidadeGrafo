#include "lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct no {
    void* dados;
    struct no* prox;
} No;

struct lista {
    No* inicio;
    No* fim;
    int tamanho;
};

Lista* criaLista() {
    Lista* l = (Lista*)malloc(sizeof(Lista));
    if(l == NULL){
        printf("Falha na alocacao de memoria para criar uma lista\n");
        exit(1);
    }
    l->inicio = NULL;
    l->fim = NULL;
    l->tamanho = 0;
    return l;
}

bool listaVazia(Lista* l) {
    if(l->tamanho == 0){
        return true;
    }
    return false;
}

void liberaLista(Lista* l) {
    if (l != NULL) {
        No* atual = l->inicio;
        while (atual != NULL) {
            No* temp = atual;
            atual = atual->prox;
            free(temp);
        }
        free(l);
    }
}

int tamanhoLista(Lista* l) {
    if(l == NULL){
        printf("Lista nao existe e/ou nao foi alocada corretamente\n");
        return 0;
    }
    return l->tamanho;
}

void inserirInicio(Lista* l, void* dados) {
    if (l == NULL){
       //printf("Lista nao existe e/ou nao foi alocada corretamente\n");
        return;
    }
    No* novo = (No*)malloc(sizeof(No));
    if (novo == NULL){
        printf("Falha na alocacao de memoria do no novo na insercao no inicio da lista\n");
        exit(1);
    }
    novo->dados = dados;
    novo->prox = l->inicio;
    l->inicio = novo;
    if (l->fim == NULL) {
        l->fim = novo;
    }
    l->tamanho++;
}

void inserirFim(Lista* l, void* dados) {
    if (l == NULL) {
        //printf("Lista nao existe e/ou nao foi alocada corretamente\n");
        return;
    }
    No* novo = (No*)malloc(sizeof(No));
    if (novo == NULL){
        printf("Falha na alocacao de memoria do no novo na insercao no fim da lista\n");
        exit(1);
    }
    novo->dados = dados;
    novo->prox = NULL;
    if (l->inicio == NULL) {
        l->inicio = novo;
    } else {
        l->fim->prox = novo;
    }
    l->fim = novo;
    l->tamanho++;
}


void* removerInicio(Lista* l) {
    if (l == NULL || listaVazia(l)){
        //printf("Impossivel realizar a remocao no inicio da lista, lista vazia e/ou nao alocada\n");
        return NULL;
    }
    No* no_removido = l->inicio;
    void* dados_retorno = no_removido->dados;
    l->inicio = no_removido->prox;
    if (l->inicio == NULL) {
        l->fim = NULL;
    }
    free(no_removido);
    l->tamanho--;
    return dados_retorno;
}

void* removerFim(Lista* l) {
    if (l == NULL || listaVazia(l)){
        //printf("Impossivel realizar a remocao no fim da lista, lista vazia e/ou nao alocada\n");
        return NULL;
    }
    No* atual = l->inicio;
    No* anterior = NULL;
    while (atual->prox != NULL) {
        anterior = atual;
        atual = atual->prox;
    }
    void* dados_retorno = atual->dados;
    if (anterior == NULL) {
        l->inicio = NULL;
        l->fim = NULL;
    } else {
        anterior->prox = NULL;
        l->fim = anterior;
    }
    free(atual);
    l->tamanho--;
    return dados_retorno;
}

void* removerElemento(Lista* l, void* dados_busca, bool (*sao_iguais)(void*, void*)) {
    if (l == NULL || listaVazia(l)){
        //printf("Impossivel remover elemento, lista vazia e/ou nao alocada\n");
        return NULL;
    }
    No* atual = l->inicio;
    No* anterior = NULL;

    while(atual != NULL && sao_iguais(atual->dados, dados_busca) == false){
        anterior = atual;
        atual = atual->prox;
    }

    if(atual == NULL){
        //printf("Elemento nao encontrado na lista para a remocao\n");
        return NULL; 
    }
    void* dados_retorno = atual->dados;
    if(anterior == NULL){
        l->inicio = atual->prox;
        if(l->inicio == NULL) l->fim = NULL;
    } else {
        anterior->prox = atual->prox;
        if(anterior->prox == NULL) l->fim = anterior;
    }
    free(atual);
    l->tamanho--;
    return dados_retorno;
}

void* buscarElemento(Lista* l, void* dados_busca, bool (*sao_iguais)(void*, void*)) {
    if(l == NULL || l->inicio == NULL){
        //printf("Busca impossivel, lista vazia e/ou nao alocada\n");
        return NULL;
    }
    No* atual = l->inicio;
    while(atual != NULL){
        if(sao_iguais(atual->dados, dados_busca) == true){
            return atual->dados;
        }
        atual = atual->prox;
    }
    //printf("Elemento nao encontrado na lista\n");
    return NULL;
}

void imprimirLista(Lista* l, void (*imprimir)(void*)) {
    if (l == NULL || listaVazia(l)){
        printf("Lista nao existe e/ou nao foi alocada corretamente\n");
        return;
    }
    printf("Lista (Tamanho: %d): [ ", l->tamanho);
    No* atual = l->inicio;
    while (atual != NULL) {
        imprimir(atual->dados);
        if (atual->prox != NULL) {
            printf(" -> ");
        }
        atual = atual->prox;
    }
    printf(" ]\n");
}