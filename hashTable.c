#include "hashTable.h"
#include <stdio.h>
#include <stdlib.h>
#include "lista.h"
#include <stdbool.h>
#include <string.h>


typedef struct HashNode{
    char* chave;
    void* info;
}HashNode;

struct hashTable{
    int size;
    Lista** baldes;
};

HashTable* criarHashTable(int size){
    if(size < 0){
        printf("Tamanho invalido para a HashTable\n");
        exit(1);
    }

    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    if(ht == NULL){
        printf("Falha na alocacao de memoria para a HashTable\n");
        exit(1);
    }

    ht->size = size;
    ht->baldes = (Lista**)malloc(size * sizeof(Lista*));
    if(ht->baldes == NULL){
        printf("Falha na alocacao de memoria para a tabela da HashTable\n");
        exit(1);
    }

    for(int i = 0; i < size; i++){
        ht->baldes[i] = criaLista();
    }

    return ht;
}


static unsigned long int hashFunction(char* chave, int size){
    unsigned long int hash = 5381;
    int c;
    while ((c = *chave++)){
        hash = ((hash << 5) + hash) + c;
    }
    return hash % size;
}
void inserirHashTable(HashTable* ht, char* chave, void* info) {
    unsigned long int idx = hashFunction(chave, ht->size);
    Lista* listaDoBalde = ht->baldes[idx];


    HashNode* nodeExistente = (HashNode*) buscarElemento(listaDoBalde, chave, comparaChaves);

    if (nodeExistente != NULL) {
        nodeExistente->info = info;
    } else {
        HashNode* novoNode = (HashNode*)malloc(sizeof(HashNode));
        if (novoNode == NULL) {
            printf("Erro ao alocar no da hash\n");
            exit(1);
        }
        
        novoNode->chave = strdup(chave); 
        novoNode->info = info;

        inserirFim(listaDoBalde, novoNode);
    }
}

void* buscarHashTable(HashTable* ht, char* chave) {
    unsigned long int idx = hashFunction(chave, ht->size);
    Lista* listaDoBalde = ht->baldes[idx];

    HashNode* node = (HashNode*) buscarElemento(listaDoBalde, chave, comparaChaves);

    if (node != NULL) {
        return node->info; 
    }
    
    return NULL; 
}

void* removerHashTable(HashTable* ht, char* chave) {
    unsigned long int idx = hashFunction(chave, ht->size);
    Lista* listaDoBalde = ht->baldes[idx];


    HashNode* nodeRemovido = (HashNode*) removerElemento(listaDoBalde, chave, comparaChaves);

    if (nodeRemovido != NULL) {
        void* infoRetorno = nodeRemovido->info;
        
        free(nodeRemovido->chave);
        free(nodeRemovido);        
        
        return infoRetorno; 
    }

    return NULL;
}

void destruirHashTable(HashTable* ht) {
    if (ht == NULL){
        return;
    }
    for (int i = 0; i < ht->size; i++) {
        Lista* l = ht->baldes[i];
        
        while (!listaVazia(l)) {
            HashNode* node = (HashNode*) removerInicio(l);
            if (node != NULL) {
                free(node->chave);
                free(node);
            }
        }
        
        liberaLista(l);
    }

    free(ht->baldes);
    free(ht);
}

bool comparaChaves(void* itemDaLista, void* chaveBusca) {
    HashNode* node = (HashNode*) itemDaLista;
    char* chave = (char*) chaveBusca;
    
    return strcmp(node->chave, chave) == 0;
}