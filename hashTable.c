#include "hashTable.h"
#include <stdio.h>
#include <stdlib.h>
#include "lista.h"


typedef struct HashNode{
    char* chave;
    void* dado;
    struct HashNode* next;
}HashNode;

struct hashTable{
    int size;
    Lista** balde;
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
    ht->balde = (Lista**)malloc(size * sizeof(Lista*));
    if(ht->balde == NULL){
        printf("Falha na alocacao de memoria para a tabela da HashTable\n");
        exit(1);
    }

    for(int i = 0; i < size; i++){
        ht->balde[i] = criarLista();
    }

    return ht;
}


// função de hash que os veteranos recomendaram utilizar
static unsigned long int hashFunction(char* chave, int size){
    unsigned long int hash = 5381;
    int c;
    while ((c = *chave++)){
        hash = ((hash << 5) + hash) + c;
    }
    return hash % size;
}