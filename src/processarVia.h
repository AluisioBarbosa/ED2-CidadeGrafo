#include "digraph.h"
#include "hashTable.h"


/** * Processa o arquivo de vias e retorna um grafo direcionado representando as vias
 * @param caminhoArquivo: caminho para o arquivo de vias
 * @param tabelaVertices: tabela hash contendo os vertices (nos) do grafo
 * @return Grafo direcionado representando as vias lidas do arquivo
*/
Graph* processarVIA(char*, HashTable*);