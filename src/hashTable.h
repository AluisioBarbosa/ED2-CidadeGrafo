#include <stdbool.h>

/**
 * @brief Estrutura de dados da tabela hash
 * Uma tabela hash é uma estrutura de dados que implementa um array associativo, uma estrutura que pode mapear chaves para valores.
 * Ela utiliza uma função hash para computar um índice em um array de buckets ou baldes, a partir do qual o valor desejado pode ser encontrado.
 * Cada balde na tabela hash é uma lista ligada que armazena os elementos que colidem no mesmo índice.
 * Cada elemento na lista ligada é um nó que contém uma chave e a informação associada a essa chave.
 * As funções contidas nesse arquivo são auto explicativas.
 */
typedef struct hashTable HashTable;

//construtor
HashTable* criarHashTable(int);

/**
 * @brief Insere um novo par chave-valor na tabela hash.
 * @param tabela Ponteiro para a tabela hash.
 * @param chave Chave do elemento a ser inserido.
 * @param valor Valor associado à chave.
 */
void inserirHashTable(HashTable*, char*, void*);

/**
 * @brief Busca um valor na tabela hash com base na chave fornecida.
 * @param tabela Ponteiro para a tabela hash.
 * @param chave Chave do elemento a ser buscado.
 * @return Ponteiro para o valor associado à chave, ou NULL se não encontrado.
 */
void* buscarHashTable(HashTable*, char*);

/**
 * @brief Remove um par chave-valor da tabela hash com base na chave fornecida.
 * @param tabela Ponteiro para a tabela hash.
 * @param chave Chave do elemento a ser removido.
 * @return Ponteiro para o valor associado à chave removida, ou NULL se não encontrado.
 */
void* removerHashTable(HashTable*, char*);
/**
 * @brief Destrói a tabela hash e libera toda a memória associada.
 * @param tabela Ponteiro para a tabela hash a ser destruída.
 */
void destruirHashTable(HashTable*);

/**
 * @brief Função de comparação de chaves para busca e remoção na tabela hash.
 * @param itemDaLista Ponteiro para o nó da lista.
 * @param chaveBusca Ponteiro para a chave a ser comparada.
 * @return true se as chaves forem iguais, false caso contrário.
 */
bool comparaChaves(void* , void*);