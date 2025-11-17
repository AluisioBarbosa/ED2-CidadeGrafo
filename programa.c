#include <stdio.h>
#include <stdlib.h>
#include "programa.h"
#include "argumentos.h"
#include "log.h"
#include "lista.h"
#include "STreap.h"
#include "digraph.h"
#include "hashTable.h"

struct programa{
    DadosArgumentos* args;
    STreap* quadras;
    Graph* vias;
    
    HashTable* idQuadras;
    HashTable* idVertices;
    HashTable* idArestas;
};



Programa* criarPrograma(int argc, char* argv[]){
    Programa* programa = (Programa*)malloc(sizeof(Programa));
    if(programa == NULL){
        printf("Falha na alocacao de memoria para o programa\n");
        exit(1);
    }

    programa->args = processarArgumentos(argc, argv);

    printArgumentos(programa->args);

    initLog(getDIRsaida(programa->args));
    return programa;
}

void run(Programa* programa){
    printArgumentos(programa->args);





}

void destruirPrograma(Programa* programa){
    destruirArgumentos(programa->args);

}