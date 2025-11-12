#include "formasGeometricas.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


struct retangulo{
    double x;
    double y; 
    double largura;
    double altura;
    char* corBorda;
    char* corPreenchimento;
};

Retangulo* criarRetangulo(double x, double y, double largura, double altura, char* corBorda, char* corPreenchimento){
    Retangulo* ret = (Retangulo*)malloc(sizeof(Retangulo));
    if(ret == NULL){
        printf("Falha na criacao do retangulo\n");
        exit(1);    
    }
    ret->x = x;
    ret->y = y;
    ret->largura = largura;
    ret->altura = altura;
    ret->corBorda = strdup(corBorda);
    ret->corPreenchimento = strdup(corPreenchimento);
    return ret;
}