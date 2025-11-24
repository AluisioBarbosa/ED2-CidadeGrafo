#include "hashTable.h"
#include "STreap.h"

/**
 * @brief Processa o arquivo .geo e popula a STreap e a HashTable com os dados das quadras.
 * @param caminhoGeo Caminho para o arquivo .geo a ser processado.
 * @param streap Ponteiro para a STreap onde os dados serão armazenados.
 * @param hashTable Ponteiro para a HashTable onde os dados serão armazenados.
 */
void processarGeo(char*, STreap*, HashTable*);