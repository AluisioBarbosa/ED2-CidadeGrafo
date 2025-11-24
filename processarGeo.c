#include "processarGeo.h"
#include "quadra.h"
#include "STreap.h"
#include "hashTable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define _POSIX_C_SOURCE 200809L


void processarGeo(char* caminho, STreap* arvoreQuadras, HashTable* idQuadras){
    if(caminho == NULL){
        printf("Erro ao abrir o caminho do arquivo .geo\n");
        exit(1);
    }
    
    FILE *f = fopen(caminho, "r");
    if(f == NULL){
        printf("Erro ao abrir o arquivo .geo\n");
        exit(1);
    }
    char comando[1024];
    

    char currentFill[1024];
    char currentStroke[1024];
    double currentSw = 1.0;

    strcpy(currentFill, "white");
    strcpy(currentStroke, "black");


    char cep[1024];
    double x, y, w, h;
    char tempSw[1024];

    while (fscanf(f, "%s", comando) != EOF) {
        
        if (strcmp(comando, "c") == 0) {
            fscanf(f, "%s %s", currentFill, currentStroke);
        }
        else if (strcmp(comando, "cq") == 0) {
            fscanf(f, "%s %s %s", tempSw, currentFill, currentStroke);
            currentSw = atof(tempSw); 
        }
        else if (strcmp(comando, "q") == 0) {
            fscanf(f, "%s %lf %lf %lf %lf", cep, &x, &y, &w, &h);
            Quadra* q = quadra_cria(cep, x, y, w, h, currentFill, currentStroke, currentSw);
            insertSTrp(arvoreQuadras, x, y, (void*)q);
            inserirHashTable(idQuadras, cep, (void*)q);
        }
    }

    fclose(f);
}