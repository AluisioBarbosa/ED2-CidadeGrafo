#include <stdio.h>
#include <stdlib.h>
#include "programa.h"
#include "argumentos.h"
#include "log.h"
#include "lista.h"

struct programa{
    DadosArgumentos* args;
    Lista* objetosGeo;
    Lista* objetosQry;
    
};



Programa* criarPrograma(int argc, char* argv[]){
    Programa* programa = (Programa*)malloc(sizeof(Programa));
    if(programa == NULL){
        printf("Falha na alocacao de memoria para o programa\n");
        exit(1);
    }

    programa->args = processarArgumentos(argc, argv);
    programa->objetosGeo = criaLista();
    programa->objetosQry = criaLista();

    initlog(getDIRsaida(programa->args));
    return programa;
}

void run(Programa* programa){






}

void destruirPrograma(Programa* programa){
    destruirArgumentos(programa->args);

}