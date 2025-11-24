#include "digraph.h"


/**
 * @brief Estrutura de dados para representar o caminho resultante do algoritmo de Dijkstra
 * Essa estrutura armazena a lista de nós que compõem o caminho e o custo total associado a esse caminho.
 * 
 */
typedef struct caminho Caminho;

/**
 * @brief Tipo de função para obter o peso (custo) de uma aresta em um grafo
 * Essa função deve ser fornecida pelo usuário e é utilizada pelo algoritmo de Dijkstra para calcular os custos das arestas.
 */
typedef double (*ObterPesoFunc)(Graph *g, Edge *e);

/**
 * @brief Executa o algoritmo de Dijkstra para encontrar o caminho de menor custo entre dois nós em um grafo direcionado.
 * @param g Ponteiro para o grafo direcionado onde o algoritmo será executado.
 * @param origem Índice do nó de origem.
 * @param destino Índice do nó de destino.
 * @param getPeso Função que retorna o peso (custo) de uma aresta
 */
Caminho* dijkstra(Graph *g, int origem, int destino, ObterPesoFunc getPeso);

//destruir
void destruirCaminho(Caminho* c);

// getters
Lista* caminho_get_lista(Caminho* c);
double caminho_get_custo(Caminho* c);