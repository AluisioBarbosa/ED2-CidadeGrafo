#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>

// Structs opacas 
typedef struct lista Lista;
typedef struct no No;


/**
 * @brief Cria e inicializa uma nova lista vazia.
 * @return Ponteiro para a nova lista criada. Aborta o programa se houver falha de alocação.
 */
Lista* criaLista();

/**
 * @brief Libera toda a memória associada à lista, incluindo todos os seus nós.
 * @param l Ponteiro para a lista a ser liberada.
 * @note A função não libera os dados dos nós (se for struct, etc), só da lista
 */
void liberaLista(Lista* l);

/**
 * @brief Retorna o tamanho atual da lista.
 * @param l Ponteiro para a lista.
 * @return O número de elementos na lista.
 */
int tamanhoLista(Lista* l);

/**
 * @brief Verifica se a lista está vazia.
 * @param l Ponteiro para a lista.
 * @return 'true' se a lista estiver vazia, 'false' caso contrário.
 */
bool listaVazia(Lista* l);

// --- Funções de Inserção ---

/**
 * @brief Insere um novo elemento no início da lista.
 * @param l Ponteiro para a lista onde o elemento será inserido.
 * @param dados Ponteiro 'void*' para os dados a serem armazenados no novo nó.
 */
void inserirInicio(Lista* l, void* dados);

/**
 * @brief Insere um novo elemento no final da lista.
 * @param l Ponteiro para a lista onde o elemento será inserido.
 * @param dados Ponteiro 'void*' para os dados a serem armazenados no novo nó.
 */
void inserirFim(Lista* l, void* dados);

/**
 * @brief Insere um novo elemento de forma ordenada, mantendo a ordem crescente da lista.
 * @note Esta função requer uma função de comparação de 3 vias (menor, igual, maior).
 * @param l Ponteiro para a lista.
 * @param dados Ponteiro 'void*' para os dados a serem inseridos.
 * @param comparar Ponteiro para a função de comparação. A função deve retornar:
 * - um valor < 0 se o primeiro dado for menor que o segundo.
 * - 0 se os dados forem iguais.
 * - um valor > 0 se o primeiro dado for maior que o segundo.
 */
void inserirOrdenado(Lista* l, void* dados, int (*comparar)(void* dados1, void* dados2));

// --- Funções de Remoção ---

/**
 * @brief Remove e retorna o elemento do início da lista.
 * @param l Ponteiro para a lista.
 * @return Ponteiro 'void*' para os dados do elemento removido, ou NULL se a lista estiver vazia.
 */
void* removerInicio(Lista* l);

/**
 * @brief Remove e retorna o elemento do final da lista.
 * @param l Ponteiro para a lista.
 * @return Ponteiro 'void*' para os dados do elemento removido, ou NULL se a lista estiver vazia.
 */
void* removerFim(Lista* l);

/**
 * @brief Remove um elemento específico da lista.
 * @param l Ponteiro para a lista.
 * @param dados_busca Ponteiro para um dado modelo usado para encontrar o elemento a ser removido.
 * @param sao_iguais Ponteiro para a função de verificação de igualdade. A função deve retornar:.
 * @return Ponteiro 'void*' para os dados do elemento removido, ou NULL se não for encontrado.
 */
void* removerElemento(Lista* l, void* dados_busca, bool (*sao_iguais)(void* dados1, void* dados2));

// --- Funções de Busca e Visualização ---

/**
 * @brief Busca um elemento na lista.
 * @param l Ponteiro para a lista.
 * @param dados_busca Ponteiro para um dado modelo usado para encontrar o elemento desejado.
 * @param sao_iguais Ponteiro para a função de verificação de igualdade. A função deve retornar:
 * @return Ponteiro 'void*' para os dados do elemento encontrado na lista, ou NULL se não for encontrado.
 */
void* buscarElemento(Lista* l, void* dados_busca, bool (*sao_iguais)(void* dados1, void* dados2));

/**
 * @brief Percorre a lista e imprime cada elemento.
 * @param l Ponteiro para a lista.
 * @param imprimir Ponteiro para a função que sabe como imprimir um único elemento.
 * Esta função recebe um 'void*' e não retorna nada.
 */
void imprimirLista(Lista* l, void (*imprimir)(void* dados));

#endif // LISTA_H