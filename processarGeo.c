#include "processarGeo.h"
#include <stdio.h>
#include "argumentos.h"
#include "lista.h"
#include "formasGeometricas.h"


Lista* processarObjetos(DadosArgumentos* args){
    char* caminho = getCaminhoGeo(args);
    FILE *f = fopen(caminho, "r");
    



}