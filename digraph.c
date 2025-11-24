#define _POSIX_C_SOURCE 200809L
#include "digraph.h"
#include "hashTable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum{
    BRANCO = 0,
    CINZA = 1,
    PRETO = 2
}CORES;

typedef struct vertice{
    char* nome;
    InfoGrafo* info;
} Vertice;

struct edge{
    Node from;
    Node to;
    InfoGrafo* info;
};

typedef struct listaAdj{
    Vertice vertice;
    Lista* listaEdges;
    bool ativo;
    int cor;
    double td;    
    double tf;
}ListaDeAdjacencia;

struct graph{
    int nVert;
    int nEdges;
    int maxVert;
    ListaDeAdjacencia* listaAdj;
    HashTable* tabela;
    int tempo;
};


static void* intToPtr(int n) {
    size_t val = (size_t) n;
    return (void*) val;
}

static int ptrToInt(void* p) {
    size_t val = (size_t) p;
    return (int) val;
}

static bool comparaArestaPorDestino(void* item, void* ctx) {
    Edge* e = (Edge*) item;
    int* destinoDesejado = (int*) ctx;
    
    if (e->to == *destinoDesejado) {
        return true;
    } else {
        return false;
    }
}


Graph* createGraph(int nVert) {
    if (nVert <= 0) {
        return NULL;
    }

    Graph* g = (Graph*) malloc(sizeof(Graph));
    if (g == NULL) {
        return NULL;
    }

    g->maxVert = nVert;
    g->nVert = 0;
    g->nEdges = 0;
    g->tempo = 0;

    g->listaAdj = (ListaDeAdjacencia*) malloc(nVert * sizeof(ListaDeAdjacencia));
    if (g->listaAdj == NULL) {
        free(g);
        return NULL;
    }

    g->tabela = criarHashTable(nVert); 

    for (int i = 0; i < nVert; i++) {
        g->listaAdj[i].ativo = false;
        g->listaAdj[i].vertice.nome = NULL;
        g->listaAdj[i].vertice.info = NULL;
        g->listaAdj[i].listaEdges = NULL;
        
        g->listaAdj[i].cor = BRANCO;
        g->listaAdj[i].td = 0.0;
        g->listaAdj[i].tf = 0.0;
    }

    return g;
}

void killDG(Graph *g) {
    Graph* gr = g;
    if (gr == NULL) {
        return;
    }

    for (int i = 0; i < gr->maxVert; i++) {
        if (gr->listaAdj[i].ativo) {
            if (gr->listaAdj[i].vertice.nome != NULL) {
                free(gr->listaAdj[i].vertice.nome);
            }
            
            if (gr->listaAdj[i].listaEdges != NULL) {
                while (!listaVazia(gr->listaAdj[i].listaEdges)) {
                    Edge* e = (Edge*) removerInicio(gr->listaAdj[i].listaEdges);
                    free(e);
                }
                liberaLista(gr->listaAdj[i].listaEdges);
            }
        }
    }

    free(gr->listaAdj);
    free(gr);
}


int getMaxNodes(Graph *g) {
    Graph* gr = (Graph*) g;
    if (gr != NULL) {
        return gr->maxVert;
    }
    return 0;
}

int getTotalNodes(Graph g) {
    Graph* gr = (Graph*) &g; 

    return g.nVert; 
}


Node addNode(Graph *g, char *nome, InfoGrafo *info) {
    Graph* gr = (Graph*) g;
    if (gr == NULL || nome == NULL) {
        return -1;
    }

    if (buscarHashTable(gr->tabela, nome) != NULL) {
        return -1; 
    }

    if (gr->nVert >= gr->maxVert) {
        return -1;
    }

    int idx = -1;
    for (int i = 0; i < gr->maxVert; i++) {
        if (!gr->listaAdj[i].ativo) {
            idx = i;
            break;
        }
    }

    if (idx != -1) {
        gr->listaAdj[idx].ativo = true;
        gr->listaAdj[idx].vertice.nome = strdup(nome);
        gr->listaAdj[idx].vertice.info = info;
        gr->listaAdj[idx].listaEdges = criaLista();
        
        inserirHashTable(gr->tabela, nome, intToPtr(idx));
        gr->nVert++;
        return idx;
    }

    return -1;
}

Node getNode(Graph *g, char *nome) {
    Graph* gr = (Graph*) g;
    if (gr == NULL || nome == NULL) {
        return -1;
    }
    
    void* res = buscarHashTable(gr->tabela, nome);
    if (res != NULL) {
        return ptrToInt(res);
    }
    return -1;
}

InfoGrafo* getNodeInfo(Graph *g, Node *node) {
    Graph* gr = (Graph*) g;
    if (gr == NULL || node == NULL) {
        return NULL;
    }
    
    int idx = *node;
    if (idx >= 0) {
        if (idx < gr->maxVert) {
            if (gr->listaAdj[idx].ativo) {
                return (InfoGrafo*) gr->listaAdj[idx].vertice.info; 
            }
        }
    }
    return NULL;
}

char *getNodeName(Graph *g, Node node) {
    Graph* gr = (Graph*) g;
    if (gr == NULL) {
        return NULL;
    }
    
    if (node >= 0) {
        if (node < gr->maxVert) {
            if (gr->listaAdj[node].ativo) {
                return gr->listaAdj[node].vertice.nome;
            }
        }
    }
    return NULL;
}

void setNodeInfo(Graph *g, Node node, InfoGrafo* info) {
    Graph* gr = (Graph*) g;
    if (gr == NULL) {
        return;
    }
    
    if (node >= 0) {
        if (node < gr->maxVert) {
            if (gr->listaAdj[node].ativo) {
                gr->listaAdj[node].vertice.info = info;
            }
        }
    }
}


Edge* addEdge(Graph *g, Node from, Node to, InfoGrafo* info) {
    Graph* gr = (Graph*) g;
    if (gr == NULL) {
        return NULL;
    }

    if (from < 0 || from >= gr->maxVert || !gr->listaAdj[from].ativo) {
        return NULL;
    }
    if (to < 0 || to >= gr->maxVert || !gr->listaAdj[to].ativo) {
        return NULL;
    }

    if (getEdge(g, from, to) != NULL) {
        return NULL;
    }

    Edge* e = (Edge*) malloc(sizeof(Edge));
    if (e == NULL) {
        return NULL;
    }

    e->from = from;
    e->to = to;
    e->info = info;

    inserirFim(gr->listaAdj[from].listaEdges, (void*)e);
    gr->nEdges++;

    return (Edge*) e;
}

Edge* getEdge(Graph *g, Node from, Node to) {
    Graph* gr = (Graph*) g;
    if (gr == NULL) {
        return NULL;
    }
    if (from < 0 || from >= gr->maxVert || !gr->listaAdj[from].ativo) {
        return NULL;
    }

    int target = to;
    
    return (Edge*) buscarElemento(gr->listaAdj[from].listaEdges, &target, comparaArestaPorDestino);
}

Node getFromNode(Graph *g, Edge *e) {
    Edge* ed = (Edge*) e;
    if (ed != NULL) {
        return ed->from;
    }
    return -1;
}

Node getToNode(Graph *g, Edge *e) {
    Edge* ed = (Edge*) e;
    if (ed != NULL) {
        return ed->to;
    }
    return -1;
}

InfoGrafo* getEdgeInfo(Graph *g, Edge *e) {
    Edge* ed = (Edge*) e;
    if (ed != NULL) {
        return (InfoGrafo*) ed->info;
    }
    return NULL;
}

void setEdgeInfo(Graph *g, Edge *e, InfoGrafo* info) {
    Edge* ed = (Edge*) e;
    if (ed != NULL) {
        ed->info = info;
    }
}

void removeEdge(Graph *g, Edge *e) {
    Graph* gr = (Graph*) g;
    Edge* ed = (Edge*) e;
    
    if (gr == NULL || ed == NULL) {
        return;
    }

    int u = ed->from;
    if (u < 0 || u >= gr->maxVert) {
        return;
    }

    int target = ed->to;

    Edge* removido = (Edge*) removerElemento(gr->listaAdj[u].listaEdges, &target, comparaArestaPorDestino);
    
    if (removido != NULL) {
        free(removido);
        gr->nEdges--;
    }
}

bool isAdjacent(Graph *g, Node from, Node to) {
    if (getEdge(g, from, to) != NULL) {
        return true;
    } else {
        return false;
    }
}


static void copiarParaLista(Lista* origem, Lista* destino, bool isNode) {
    if (origem == NULL || destino == NULL) {
        return;
    }

    int tam = tamanhoLista(origem);
    for (int i = 0; i < tam; i++) {
        Edge* e = (Edge*) removerInicio(origem);
        
        if (isNode) {
            int* nodePtr = (int*) malloc(sizeof(int));
            *nodePtr = e->to;
            inserirFim(destino, (void*) nodePtr);
        } else {
            inserirFim(destino, (void*) e);
        }

        inserirFim(origem, (void*) e); 
    }
}

void adjacentNodes(Graph *g, Node node, Lista *nosAdjacentes) {
    Graph* gr = (Graph*) g;
    if (gr == NULL || node < 0 || node >= gr->maxVert || !gr->listaAdj[node].ativo) {
        return;
    }
    copiarParaLista(gr->listaAdj[node].listaEdges, nosAdjacentes, true);
}

void adjacentEdges(Graph *g, Node node, Lista *arestasAdjacentes) {
    Graph* gr = (Graph*) g;
    if (gr == NULL || node < 0 || node >= gr->maxVert || !gr->listaAdj[node].ativo) {
        return;
    }
    copiarParaLista(gr->listaAdj[node].listaEdges, arestasAdjacentes, false);
}

void getNodeNames(Graph *g, Lista *nomesNodes) {
    Graph* gr = (Graph*) g;
    if (gr == NULL) {
        return;
    }

    for (int i = 0; i < gr->maxVert; i++) {
        if (gr->listaAdj[i].ativo) {
            inserirFim(nomesNodes, (void*) gr->listaAdj[i].vertice.nome);
        }
    }
}

void getEdges(Graph *g, Lista *arestas) {
    Graph* gr = (Graph*) g;
    if (gr == NULL) {
        return;
    }

    for (int i = 0; i < gr->maxVert; i++) {
        if (gr->listaAdj[i].ativo) {
            copiarParaLista(gr->listaAdj[i].listaEdges, arestas, false);
        }
    }
}


static bool dfsRec(Graph* gr, int u, 
                   procEdge tree, procEdge fwd, procEdge ret, procEdge cross, 
                   void* extra) {
    
    gr->listaAdj[u].cor = CINZA;
    gr->tempo++;
    gr->listaAdj[u].td = (double) gr->tempo;

    Lista* l = gr->listaAdj[u].listaEdges;
    int tam = tamanhoLista(l);
    bool abortar = false;

    for (int k = 0; k < tam; k++) {
        Edge* e = (Edge*) removerInicio(l);
        int v = e->to;

        if (!abortar) {
            bool continuar = true;

            // MUDANÇA: Comparando com ENUM
            if (gr->listaAdj[v].cor == BRANCO) {
                // Tree Edge
                if (tree != NULL) {
                    continuar = tree((Graph*)gr, (Edge*)e, gr->listaAdj[u].td, 0.0, extra);
                }
                
                if (continuar) {
                    if (!dfsRec(gr, v, tree, fwd, ret, cross, extra)) {
                        abortar = true;
                    }
                } else {
                    abortar = true;
                }

            } else if (gr->listaAdj[v].cor == CINZA) {
                if (ret != NULL) {
                    continuar = ret((Graph*)gr, (Edge*)e, gr->listaAdj[u].td, 0.0, extra);
                }
                if (!continuar) {
                    abortar = true;
                }

            } else {
                if (gr->listaAdj[u].td < gr->listaAdj[v].td) {
                    if (fwd != NULL) {
                        continuar = fwd((Graph*)gr, (Edge*)e, gr->listaAdj[u].td, gr->listaAdj[v].td, extra);
                    }
                } else {
                    if (cross != NULL) {
                        continuar = cross((Graph*)gr, (Edge*)e, gr->listaAdj[u].td, gr->listaAdj[v].td, extra);
                    }
                }

                if (!continuar) {
                    abortar = true;
                }
            }
        }
        
        inserirFim(l, (void*)e);
    }

    gr->listaAdj[u].cor = PRETO;
    gr->tempo++;
    gr->listaAdj[u].tf = (double) gr->tempo;

    if (abortar) {
        return false;
    } else {
        return true;
    }
}

bool dfs(Graph *g, Node node, procEdge treeEdge, procEdge forwardEdge, procEdge returnEdge,
    procEdge crossEdge, dfsRestarted newTree, void *extra) {
    
    Graph* gr = (Graph*) g;
    if (gr == NULL) {
        return false;
    }

    for (int i = 0; i < gr->maxVert; i++) {
        gr->listaAdj[i].cor = BRANCO; 
        gr->listaAdj[i].td = 0.0;
        gr->listaAdj[i].tf = 0.0;
    }
    gr->tempo = 0;

    if (node >= 0 && node < gr->maxVert && gr->listaAdj[node].ativo) {
        if (!dfsRec(gr, node, treeEdge, forwardEdge, returnEdge, crossEdge, extra)) {
            return false;
        }
    }

    for (int i = 0; i < gr->maxVert; i++) {
        if (gr->listaAdj[i].ativo) {
            if (gr->listaAdj[i].cor == BRANCO) {
                
                if (newTree != NULL) {
                    if (!newTree(g, extra)) {
                        return false;
                    }
                }

                if (!dfsRec(gr, i, treeEdge, forwardEdge, returnEdge, crossEdge, extra)) {
                    return false;
                }
            }
        }
    }

    return true;
}

bool bfs(Graph *g, Node node, Node discoverNode, void *extra) {
    Graph* gr = (Graph*) g;
    if (gr == NULL) {
        return false;
    }
    if (node < 0 || node >= gr->maxVert) {
        return false;
    }

    procEdge onDiscover = (procEdge) discoverNode;

    for (int i = 0; i < gr->maxVert; i++) {
        gr->listaAdj[i].cor = BRANCO; 
        gr->listaAdj[i].td = 0.0;
    }

    Lista* fila = criaLista();
    if (fila == NULL) {
        return false;
    }

    gr->listaAdj[node].cor = CINZA; 
    gr->listaAdj[node].td = 0.0;

    int* start = (int*) malloc(sizeof(int));
    *start = node;
    inserirFim(fila, (void*)start);

    bool sucesso = true;

    while (!listaVazia(fila)) {
        int* uPtr = (int*) removerInicio(fila);
        int u = *uPtr;
        free(uPtr);

        Lista* l = gr->listaAdj[u].listaEdges;
        int tam = tamanhoLista(l);

        for (int k = 0; k < tam; k++) {
            Edge* e = (Edge*) removerInicio(l);
            int v = e->to;

            if (gr->listaAdj[v].cor == BRANCO) {
                gr->listaAdj[v].cor = CINZA;
                gr->listaAdj[v].td = gr->listaAdj[u].td + 1.0;

                if (onDiscover != NULL) {
                    if (!onDiscover(g, (Edge*)e, 0.0, 0.0, extra)) {
                        sucesso = false;
                    }
                }

                int* vPtr = (int*) malloc(sizeof(int));
                *vPtr = v;
                inserirFim(fila, (void*)vPtr);
            }

            inserirFim(l, (void*)e);

            if (!sucesso) {
                break;
            }
        }
        gr->listaAdj[u].cor = PRETO; 

        if (!sucesso) {
            break;
        }
    }

    while (!listaVazia(fila)) {
        void* p = removerInicio(fila);
        free(p);
    }
    liberaLista(fila);

    return sucesso;
}

void removeNodeAndEdges(Graph* g, Node node) {
    Graph* gr = g;
    if (!gr){
         return;
    }
    
    if (node < 0 || node >= gr->maxVert || !gr->listaAdj[node].ativo){
        return;
    }
    Lista* l = gr->listaAdj[node].listaEdges;
    while (!listaVazia(l)) {
        Edge* e = (Edge*) removerInicio(l);
        free(e); 
    }
    

    for (int i = 0; i < gr->maxVert; i++) {

        if (gr->listaAdj[i].ativo && i != node) {
            Lista* adj = gr->listaAdj[i].listaEdges;
            
            int tam = tamanhoLista(adj);
            for (int k = 0; k < tam; k++) {
                Edge* e = (Edge*) removerInicio(adj);
                
                if (e->to == node) {
                    free(e); 
                    gr->nEdges--;
                } else {
                    inserirFim(adj, e); 
                }
            }
        }
    }


    gr->listaAdj[node].ativo = false;
    gr->nVert--;
    
    if (gr->listaAdj[node].vertice.nome) {
        free(gr->listaAdj[node].vertice.nome);
        gr->listaAdj[node].vertice.nome = NULL;
    }

}