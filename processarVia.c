#include "processarVia.h"
#include "digraph.h"
#include "hashTable.h"
#include "info.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _POSIX_C_SOURCE 200809L

static int ehEspaco(char c) {
    return (c == ' ' || c == '\n' || c == '\r' || c == '\t');
}

static void trimEnd(char* str) {
    if (str == NULL) {
        return;
    }
    int len = strlen(str);

    while (len > 0) {
        char c = str[len - 1];
        if (ehEspaco(c)) {
            str[len - 1] = '\0';
            len--;
        } else {
            break;
        }
    }

    if (len > 0) {
        if (ehEspaco(str[0])) {
            memmove(str, str + 1, len); 
        }
    }
}

Graph* processarVIA(char* pathVia, HashTable* idVertices) {
    if (pathVia == NULL) {
        return NULL;
    }

    FILE* f = fopen(pathVia, "r");
    if (f == NULL) {
        printf("Erro: Nao foi possivel abrir o arquivo .via: %s\n", pathVia);
        return NULL;
    }

    char cmd[10];
    int numVertices = 0;
    char bufferLixo[256];

    while (fscanf(f, "%s", cmd) != EOF) {
        if (strcmp(cmd, "v") == 0) {
            numVertices++;
        }
        fgets(bufferLixo, 256, f);
    }

    Graph *g = createGraph(numVertices + 5);
    
    if (g == NULL) {
        fclose(f);
        return NULL;
    }

    rewind(f);

    char id[256], orig[256], dest[256];
    char ldir[256], lesq[256], nomeRua[256];
    double x, y, cmp, vm;

    while (fscanf(f, "%s", cmd) != EOF) {
        
        if (strcmp(cmd, "v") == 0) {
            fscanf(f, "%s %lf %lf", id, &x, &y);
            
            InfoVertice* iv = criaInfoVertice(id, x, y);
            
            
            int index = addNode(g, id, iv);
            
            if (index >= 0) {
                int* ptrIndex = (int*) malloc(sizeof(int));
                *ptrIndex = index;
                inserirHashTable(idVertices, id, ptrIndex);
            } else {
                printf("Erro ou duplicata no vertice %s\n", id);
                free(iv);
            }
        }
        
        else if (strcmp(cmd, "e") == 0) {
            fscanf(f, "%s %s %s %s %lf %lf", orig, dest, ldir, lesq, &cmp, &vm);
            fgets(nomeRua, 256, f);
            trimEnd(nomeRua); 

            int* uPtr = (int*) buscarHashTable(idVertices, orig);
            int* vPtr = (int*) buscarHashTable(idVertices, dest);

            if (uPtr != NULL && vPtr != NULL) {
                InfoAresta* ia = criaInfoAresta(nomeRua, ldir, lesq, cmp, vm);
                addEdge(g, *uPtr, *vPtr, ia);
            }
        }
    }

    fclose(f);
    return g;
}