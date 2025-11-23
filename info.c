#include "info.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


struct infoVertice{
    char* id;
    double x;
    double y;
};

 struct infoAresta{
    char* nome; // Nome da rua
    char* ldir; // CEP lado direito
    char* lesq; // CEP lado esquerdo
    double cmp; // Comprimento
    double vm;  // Velocidade Média
    bool ativo; // aqui é pra bloquear ou nao
};

InfoVertice* criaInfoVertice(char* id, double x, double y) {
    InfoVertice* i = (InfoVertice*) malloc(sizeof(InfoVertice));
    if (i == NULL) {
        printf("Erro ao alocar memoria para InfoVertice\n");
        exit(1);
    }
    i->id = strdup(id);
    i->x = x;
    i->y = y;
    
    return i;
}

InfoAresta* criaInfoAresta(char* nome, char* ldir, char* lesq, double cmp, double vm) {
    InfoAresta* i = (InfoAresta*) malloc(sizeof(InfoAresta));
    if (i == NULL) {
        printf("Erro ao alocar memoria para InfoAresta\n");
        exit(1);
    }
    i->nome = strdup(nome);
    i->ldir = strdup(ldir);
    i->lesq = strdup(lesq);
    i->cmp = cmp;
    i->vm = vm;
    i->ativo = true;
    return i;
}

double getVelocidade(void* i) { 
    return ((InfoAresta*)i)->vm; 
}
double getComprimento(void* i) { 
    return ((InfoAresta*)i)->cmp; 
}
bool isArestaAtiva(void* i) { 
    return ((InfoAresta*)i)->ativo; 
}
double getXVertice(void* i) { 
    return ((InfoVertice*)i)->x; 
}
double getYVertice(void* i) { 
    return ((InfoVertice*)i)->y; 
}
char* getIdVertice(void* i) { 
    return ((InfoVertice*)i)->id; 
}

void destroiInfoVertice(void* i) {
    InfoVertice* iv = (InfoVertice*)i;
    if (iv != NULL) { 
        free(iv->id); 
        free(iv); 
    }
}

void destroiInfoAresta(void* i) {
    InfoAresta* ia = (InfoAresta*)i;
    if (ia != NULL) { 
        free(ia->nome); 
        free(ia->ldir); 
        free(ia->lesq); 
        free(ia); 
    }
}

void setArestaAtiva(void* i, bool status) {
    InfoAresta* ia = (InfoAresta*) i;
    if (ia != NULL) {
        ia->ativo = status;
    }
}