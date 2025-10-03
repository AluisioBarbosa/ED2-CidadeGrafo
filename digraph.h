#ifndef _GRAPH__
#define _GRAPH__

#include "lista.h"
/*
Um Grafo direcionado  G e' constituido por um conjunto de vertices V e 
um conjunto de arestas E, denotado por G=(V,E). 

Pode-se associar um dado a arestas e a vertices de G.
A cada vertice e' associado um nome.

Existem tambem operacoes que possibilitam percorrer o grafo,
invocando funcoes quando arestas do grafo 
sao "descobertas", podendo, inclusive, interromper o percurso.

Este modulo faz parte de um trabalho de Estrutura de Dados do 
curso de Ciencia da Computacao da Universidade Estadual de Londrina.
E' expressamente proibido o uso de ferramentas de IA para a sua implementacao.
A documentacao deste modulo deve ser melhorada.
*/


typedef struct graph Graph;
typedef int Node;
typedef struct edge Edge;
typedef struct info Info;

/*
  Invocado quando uma aresta � "descoberta"/"percorrida"/"classificada". 
  Tambem informa os tempos de descoberta e finalizacao
 */
bool (*procEdge)(Graph* g, Edge* e, double td, double tf, void *extra); 

/*
  Invocado quando percurso e' recomecado
 */
bool (*dfsRestarted)(Graph *g, void *extra);



/*
    Cria um grafo com, no maximo, "nVert" vertices.
 */
Graph createGraph(int nVert);


/*
    Retorna numero maximo de vertices que grafo g pode conter.
 */
int getMaxNodes(Graph *g);


/*
    Retorna numero total de vertices adicionados ao grafo gr.
 */
int getTotalNodes(Graph g);


/*
    Adiciona um novo v�rtice ao grafo "g" com o nome "nome".
 */
Node addNode(Graph *g, char nome, Info *info);


/*
    Retorna no' cujo de nome e' "nome". 
 */
Node getNode(Graph *g, char nome);


/*
 */
Info getNodeInfo(Graph *g, Node *node);


/*
 */
char *getNodeName(Graph *g, Node *node);


/*
 */
void setNodeInfo(Graph *g, Node *node, Info *info);


/*
 */
Edge addEdge(Graph *g, Node *from, Node *to, Info *info);


/*
 */
Edge getEdge(Graph *g, Node *from, Node *to);


/*
 */
Node getFromNode(Graph *g, Edge *e);

  
/*
 */  
Node getToNode(Graph *g, Edge *e);


/*
 */
Info getEdgeInfo(Graph *g, Edge *e);


/*
 */
void setEdgeInfo(Graph *g, Edge *e, Info *info);


/*
 */
void removeEdge(Graph *g, Edge *e);


/*
 */
bool isAdjacent(Graph *g, Node *from, Node *to);


/* 
   Adiciona 'a lista "nosAdjacentes" os nos adjacentes 'a "node".
 */
void adjacentNodes(Graph *g, Node *node, Lista *nosAdjacentes);


/*
   Adiciona 'a lista "arestaAdjacentes" as arestas (x,y), tal que,
   x == node.
 */
void adjacentEdges(Graph *g, Node *node, Lista *arestasAdjacentes);


/*
   Insere na lista "nomesNodes" os nomes atribuidos aos nos do grafo.
 */
void  getNodeNames(Graph *g, Lista *nomesNodes);


/*
   Insere na lista "arestas", as arestas de g.
 */
void getEdges(Graph *g, Lista *arestas);


/*
   Faz percurso em profundidade sobre  g, a partir do no' node, classificando 
   as arestas do grafo, invocando a respectiva funcao.
      A busca em profundidade, eventualmente, pode produzir uma floresta.
   newTree e' invocada sempre que o percurso for retomado.
 */  
bool dfs(Graph *g, Node *node, /*procEdge treeEdge,*/ Edge *forwardEdge, Edge *returnEdge,
	 Edge* crossEdge, /*newTree,*/ void *extra);


/*
   Percorre o grafo g em largura, a partir do no' node. discoverNode e' usada
   para a aresta (x,y) usada para "descobrir" o y.
 */
bool bfs(Graph *g, Node *node, Node *discoverNode, void *extra);


/*
   Destroi o grafo "g".
 */
void killDG(Graph *g);

#endif
