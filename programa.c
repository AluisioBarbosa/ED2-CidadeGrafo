#include <stdio.h>
#include <stdlib.h>
#include "programa.h"
#include "argumentos.h"
#include "lista.h"
#include "hashTable.h"
#include "STreap.h"
#include "digraph.h"
#include "processarGeo.h"
#include "processarVia.h"
#include <string.h>

struct programa{
    DadosArgumentos* args;
    STreap* quadras;
    Graph* vias;
    
    HashTable* idQuadras;
    HashTable* idVertices;
    HashTable* idBloqueios;
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
    programa->vias = NULL;
    programa->idQuadras = criarHashTable(5003);
    programa->idVertices = criarHashTable(5003);
    programa->idBloqueios = criarHashTable(5003);

    printArgumentos(programa->args);
    return programa;
}





void run(Programa* programa){
    processarGeo(getCaminhoGeo(programa->args), programa->quadras, programa->idQuadras);
    printSTrp(programa->quadras, "arquivo.dot");

    programa->vias = processarVia(getCaminhoVia(programa->args), programa->idVertices);
}





void destruirPrograma(Programa* programa){
    destruirArgumentos(programa->args);

}