#include "hashTable.h"
#include <stdio.h>
#include <stdlib.h>
#include "lista.h"
#include <stdbool.h>


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


// função de hash que os veteranos recomendaram utilizar
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

    // 1. Tenta buscar se a chave já existe usando sua função genérica
    // Passamos a função 'comparaChaves' e a string 'chave'
    HashNode* nodeExistente = (HashNode*) buscarElemento(listaDoBalde, chave, comparaChaves);

    if (nodeExistente != NULL) {
        // Atualiza o valor se já existir
        nodeExistente->info = info;
    } else {
        // Cria um novo HashNode
        HashNode* novoNode = (HashNode*)malloc(sizeof(HashNode));
        if (novoNode == NULL) {
            printf("Erro ao alocar no da hash\n");
            exit(1);
        }
        
        novoNode->chave = strdup(chave); // Copia a string (IMPORTANTE)
        novoNode->info = info;

        // Insere na lista (no fim ou no início, tanto faz para hash)
        inserirFim(listaDoBalde, novoNode);
    }
}

// --- Busca ---
void* buscarHashTable(HashTable* ht, char* chave) {
    unsigned long int idx = hashFunction(chave, ht->size);
    Lista* listaDoBalde = ht->baldes[idx];

    // Usa a sua função genérica buscarElemento
    HashNode* node = (HashNode*) buscarElemento(listaDoBalde, chave, comparaChaves);

    if (node != NULL) {
        return node->info; // Retorna o dado guardado
    }
    
    return NULL; // Não achou
}

// --- Remoção ---
void* removerHashTable(HashTable* ht, char* chave) {
    unsigned long int idx = hashFunction(chave, ht->size);
    Lista* listaDoBalde = ht->baldes[idx];

    // Usa a sua função genérica removerElemento.
    // Ela retorna o ponteiro para o DADO (no caso, o HashNode*) e o remove da lista.
    HashNode* nodeRemovido = (HashNode*) removerElemento(listaDoBalde, chave, comparaChaves);

    if (nodeRemovido != NULL) {
        void* infoRetorno = nodeRemovido->info;
        
        // Precisamos liberar a memória que nós alocamos para o HashNode
        // O nó da lista já foi liberado pela sua função removerElemento
        free(nodeRemovido->chave); // Libera a string duplicada
        free(nodeRemovido);        // Libera a struct HashNode
        
        return infoRetorno; // Retorna o dado útil para o usuário
    }

    return NULL; // Não encontrou
}

// --- Destruição ---
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
        
        // Agora que a lista está vazia (mas alocada), liberamos a estrutura da lista
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