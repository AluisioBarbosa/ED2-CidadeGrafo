#include <stdio.h>
#include <stdlib.h>
#include "programa.h"
#include "argumentos.h"
#include "lista.h"
#include "hashTable.h"
#include "STreap.h"
#include "digraph.h"
#include <string.h>

struct programa{
    DadosArgumentos* args;
    STreap* quadras;
    Graph* vias;
    
    HashTable* idQuadras;
    HashTable* idVertices;
    HashTable* idArestas;
    FILE* arquivoLOG;
};

Programa* criarPrograma(int argc, char* argv[]){
    Programa* programa = (Programa*)malloc(sizeof(Programa));
    if(programa == NULL){
        printf("Falha na alocacao de memoria para o programa\n");
        exit(1);
    }

    programa->args = processarArgumentos(argc, argv);

    printArgumentos(programa->args);
    return programa;
}

void run(Programa* programa){
    
}

void destruirPrograma(Programa* programa){
    destruirArgumentos(programa->args);

}