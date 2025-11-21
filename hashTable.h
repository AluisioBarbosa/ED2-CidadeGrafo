#include <stdbool.h>

typedef struct hashTable HashTable;

HashTable* criarHashTable(int);
void inserirHashTable(HashTable*, char*, void*);
void* buscarHashTable(HashTable*, char*);
void* removerHashTable(HashTable*, char*);
void destruirHashTable(HashTable*);
bool comparaChaves(void* , void*);