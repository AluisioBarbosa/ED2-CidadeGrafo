#include "quadra.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


struct quadra{
    char* cep;
    double x, y;
    double altura, largura;
    char* corPreenchimento;
    char* corBorda;
    double espessuraBorda;
};

Quadra* quadra_cria(char* cep, double x, double y, double largura, double altura, 
                    char* corPreenchimento, char* corBorda, double espessuraBorda) {
    
    Quadra* q = (Quadra*) malloc(sizeof(Quadra));
    if (q == NULL) {
        printf("Falha na alocacao de memoria para a quadra\n");
        exit(1);
    }


    q->cep = strdup(cep);
    q->x = x;
    q->y = y;
    q->largura = largura;
    q->altura = altura;
    q->corPreenchimento = strdup(corPreenchimento);
    q->corBorda = strdup(corBorda);
    q->espessuraBorda = espessuraBorda;
    return q;
}


char* quadra_get_cep(Quadra* q) {
    return q->cep;
}

double quadra_get_x(Quadra* q) {
    return q->x;
}

double quadra_get_y(Quadra* q) {
    return q->y;
}

double quadra_get_largura(Quadra* q) {
    return q->largura;
}

double quadra_get_altura(Quadra* q) {
    return q->altura;
}

char* quadra_get_cor_preenchimento(Quadra* q) {
    return q->corPreenchimento;
}

char* quadra_get_cor_borda(Quadra* q) {
    return q->corBorda;
}

double quadra_get_espessura(Quadra* q) {
    return q->espessuraBorda;
}


void quadra_set_cores(Quadra* q, char* corPreenchimento, char* corBorda, double espessura) {
    if (q == NULL){
        return;
    }
    q->corPreenchimento = strdup(corPreenchimento);
    q->corBorda = strdup(corBorda);
    q->espessuraBorda = espessura;
}


void quadra_destroi(Quadra* q) {
    if (q != NULL) {
        if (q->cep){
            free(q->cep);
        }
        if (q->corPreenchimento){ 
            free(q->corPreenchimento);
        }
        if (q->corBorda){ 
            free(q->corBorda);
        }
        free(q);
    }
}