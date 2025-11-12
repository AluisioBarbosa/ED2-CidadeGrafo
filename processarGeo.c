#include "processarGeo.h"
#include <stdio.h>
#include "argumentos.h"
#include "lista.h"
#include "formasGeometricas.h"


Lista* processarObjetos(DadosArgumentos* args){
    char* caminho = getCaminhoGeo(args);
    if(caminho == NULL){
        printf("Erro ao abrir o caminho do arquivo .geo\n");
        exit(1);
    }
    
    FILE *f = fopen(caminho, "r");
    



}