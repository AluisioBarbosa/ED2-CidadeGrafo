#include "STreap.h"
#include "digraph.h"
#include "hashTable.h"

/** 
 * Responsavel por processar o arquivo qry e executar os comandos nele contidos
 * @param pathQry: caminho para o arquivo qry
 * @param dirSaida: diretorio de saida dos arquivos gerados
 * @param nomeBaseGeo: nome base do arquivo geo (sem extensao)
 * @param nomeBaseQry: nome base do arquivo qry (sem extensao)
 * @param idQuadras: tabela hash contendo as quadras cadastradas
 * @param vias: grafo direcionado representando as vias da cidade
 * @param idBloqueios: tabela hash contendo os bloqueios cadastrados
 * @param arvoreQuadras: arvore STreap contendo as quadras cadastradas
 * @param idVertices: tabela hash contendo os vertices cadastrados
*/
void processarQry(char* pathQry, char* dirSaida, char* nomeBaseGeo, char* nomeBaseQry,
     HashTable* idQuadras, Graph* vias, HashTable* idBloqueios, STreap* arvoreQuadras, HashTable* idVertices);