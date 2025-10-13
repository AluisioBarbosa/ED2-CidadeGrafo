#include <stdio.h>
#include <stdlib.h>
#include "programa.h"
#include "argumentos.h"
#include "log.h"

struct programa{
    DadosArgumentos* args;

    
};



Programa* criarPrograma(int argc, char* argv[]){
    Programa* programa = (Programa*)malloc(sizeof(Programa));
    if(programa == NULL){
        printf("Falha na alocacao de memoria para o programa\n");
        exit(1);
    }

    programa->args = processarArgumentos(argc, argv);

    initlog(getDIRsaida(programa->args));
    return programa;
}

void run(Programa* programa){






}

void destruirPrograma(Programa* programa){
    destruirArgumentos(programa->args);

}