#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "programa.h"
#include "argumentos.h"
#include "lista.h"
#include "hashTable.h"
#include "STreap.h"
#include "digraph.h"
#include "processarGeo.h"
#include "processarVia.h"
#include "processarQry.h"
#include "svg.h"
#define _POSIX_C_SOURCE 200809L


struct programa{
    DadosArgumentos* args;
    STreap* quadras;
    Graph* vias;
    
    HashTable* idQuadras;
    HashTable* idVertices;
    HashTable* idBloqueios;
};

static char* extrairNomeBase(char* caminhoCompleto) {
    if (caminhoCompleto == NULL) {
        return NULL;
    }
    char* nomeComExtensao = strrchr(caminhoCompleto, '/');
    if (nomeComExtensao != NULL) {
        nomeComExtensao++; 
    } else {
        nomeComExtensao = caminhoCompleto;
    }

    char* nomeBase = strdup(nomeComExtensao);
    
    char* ponto = strrchr(nomeBase, '.');
    if (ponto != NULL) {
        *ponto = '\0';
    }

    return nomeBase;
}

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

    if(getCaminhoQry(programa->args) == NULL || getCaminhoVia(programa->args) == NULL){
        SvgFile* svg = svg_criar(getDIRsaida(programa->args),
                   extrairNomeBase(getCaminhoGeo(programa->args)),
                   NULL,
                   programa->quadras,
                   NULL);
        svg_desenhar_quadras(svg, programa->quadras);
        svg_finalizar(svg);
        return;
    }

    printSTrp(programa->quadras, "arquivo.dot");
    char* nomeBaseGeo = extrairNomeBase(getCaminhoGeo(programa->args));
    char* nomeBaseQry = extrairNomeBase(getCaminhoQry(programa->args));

    programa->vias = processarVIA(getCaminhoVia(programa->args), programa->idVertices);
    processarQry(getCaminhoQry(programa->args),
                        getDIRsaida(programa->args),
                        nomeBaseGeo,
                        nomeBaseQry,
                        programa->idQuadras,
                        programa->vias,
                        programa->idBloqueios,
                        programa->quadras,
                        programa->idVertices);
}





void destruirPrograma(Programa* programa){
    destruirArgumentos(programa->args);

}

