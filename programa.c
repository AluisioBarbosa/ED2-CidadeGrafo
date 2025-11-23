#include <stdio.h>
#include <stdlib.h>
#include "programa.h"
#include "argumentos.h"
#include "lista.h"
#include "hashTable.h"
#include "STreap.h"
#include "digraph.h"
#include "processarGeo.h"
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
    programa->quadras = createSTrp(0.0006);
    programa->vias = createGraph(10000);
    programa->idQuadras = criarHashTable(5000);
    programa->idVertices = criarHashTable(5000);
    programa->idArestas = criarHashTable(5000);

    printArgumentos(programa->args);
    return programa;
}

void run(Programa* programa){
    processarGeo(getCaminhoGeo(programa->args), programa->quadras, programa->idQuadras);
    printSTrp(programa->quadras, "arquivo.dot");
}

void destruirPrograma(Programa* programa){
    destruirArgumentos(programa->args);

}