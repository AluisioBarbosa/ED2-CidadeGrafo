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

// Converte void* para int
static int ptrToInt(void* p) {
    size_t val = (size_t) p;
    return (int) val;
}

// MUDANÇA AQUI: Comparação recebe um int* diretamente como contexto, sem struct auxiliar
static bool comparaArestaPorDestino(void* item, void* ctx) {
    struct edge* e = (struct edge*) item;
    int* destinoDesejado = (int*) ctx;
    
    // Compara o destino da aresta com o inteiro passado no contexto
    if (e->to == *destinoDesejado) {
        return true;
    } else {
        return false;
    }
}

// --- CRIAÇÃO E DESTRUIÇÃO ---

Graph* createGraph(int nVert) {
    if (nVert <= 0) {
        return NULL;
    }

    struct graph* g = (struct graph*) malloc(sizeof(struct graph));
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
        
        // MUDANÇA: Inicializa com o ENUM
        g->listaAdj[i].cor = BRANCO;
        g->listaAdj[i].td = 0.0;
        g->listaAdj[i].tf = 0.0;
    }

    return (Graph*) g;
}

void killDG(Graph *g) {
    struct graph* gr = (struct graph*) g;
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
                    struct edge* e = (struct edge*) removerInicio(gr->listaAdj[i].listaEdges);
                    free(e);
                }
                liberaLista(gr->listaAdj[i].listaEdges);
            }
        }
    }

    // destruirHashTable(gr->tabela); // Se houver função
    free(gr->listaAdj);
    free(gr);
}

// --- GETTERS GERAIS ---

int getMaxNodes(Graph *g) {
    struct graph* gr = (struct graph*) g;
    if (gr != NULL) {
        return gr->maxVert;
    }
    return 0;
}

int getTotalNodes(Graph g) {
    // Acesso direto assumindo struct graph conhecida aqui
    struct graph* gr = (struct graph*) &g; 
    // Nota: Dependendo de como 'Graph' é passado (valor ou ponteiro), isso pode variar.
    // Assumindo a lógica anterior de acesso direto à struct.
    return g.nVert; 
}

// --- VÉRTICES ---

Node addNode(Graph *g, char *nome, InfoGrafo *info) {
    struct graph* gr = (struct graph*) g;
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
    struct graph* gr = (struct graph*) g;
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
    struct graph* gr = (struct graph*) g;
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
    struct graph* gr = (struct graph*) g;
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
    struct graph* gr = (struct graph*) g;
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

// --- ARESTAS ---

Edge* addEdge(Graph *g, Node from, Node to, InfoGrafo* info) {
    struct graph* gr = (struct graph*) g;
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

    struct edge* e = (struct edge*) malloc(sizeof(struct edge));
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
    struct graph* gr = (struct graph*) g;
    if (gr == NULL) {
        return NULL;
    }
    if (from < 0 || from >= gr->maxVert || !gr->listaAdj[from].ativo) {
        return NULL;
    }

    // MUDANÇA: Passa o endereço da variável 'to' diretamente
    int target = to;
    
    // buscarElemento espera (Lista, void* ctx, bool (*cmp)(void*, void*))
    return (Edge*) buscarElemento(gr->listaAdj[from].listaEdges, &target, comparaArestaPorDestino);
}

Node getFromNode(Graph *g, Edge *e) {
    struct edge* ed = (struct edge*) e;
    if (ed != NULL) {
        return ed->from;
    }
    return -1;
}

Node getToNode(Graph *g, Edge *e) {
    struct edge* ed = (struct edge*) e;
    if (ed != NULL) {
        return ed->to;
    }
    return -1;
}

InfoGrafo* getEdgeInfo(Graph *g, Edge *e) {
    struct edge* ed = (struct edge*) e;
    if (ed != NULL) {
        return (InfoGrafo*) ed->info;
    }
    return NULL;
}

void setEdgeInfo(Graph *g, Edge *e, InfoGrafo* info) {
    struct edge* ed = (struct edge*) e;
    if (ed != NULL) {
        ed->info = info;
    }
}

void removeEdge(Graph *g, Edge *e) {
    struct graph* gr = (struct graph*) g;
    struct edge* ed = (struct edge*) e;
    
    if (gr == NULL || ed == NULL) {
        return;
    }

    int u = ed->from;
    if (u < 0 || u >= gr->maxVert) {
        return;
    }

    // MUDANÇA: Passa o endereço do destino diretamente como contexto
    int target = ed->to;

    // Assumindo que removerElemento retorna o dado removido
    struct edge* removido = (struct edge*) removerElemento(gr->listaAdj[u].listaEdges, &target, comparaArestaPorDestino);
    
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

// --- AUXILIARES PARA LISTAGEM ---

static void copiarParaLista(Lista* origem, Lista* destino, bool isNode) {
    if (origem == NULL || destino == NULL) {
        return;
    }

    int tam = tamanhoLista(origem);
    for (int i = 0; i < tam; i++) {
        struct edge* e = (struct edge*) removerInicio(origem);
        
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
    struct graph* gr = (struct graph*) g;
    if (gr == NULL || node < 0 || node >= gr->maxVert || !gr->listaAdj[node].ativo) {
        return;
    }
    copiarParaLista(gr->listaAdj[node].listaEdges, nosAdjacentes, true);
}

void adjacentEdges(Graph *g, Node node, Lista *arestasAdjacentes) {
    struct graph* gr = (struct graph*) g;
    if (gr == NULL || node < 0 || node >= gr->maxVert || !gr->listaAdj[node].ativo) {
        return;
    }
    copiarParaLista(gr->listaAdj[node].listaEdges, arestasAdjacentes, false);
}

void getNodeNames(Graph *g, Lista *nomesNodes) {
    struct graph* gr = (struct graph*) g;
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
    struct graph* gr = (struct graph*) g;
    if (gr == NULL) {
        return;
    }

    for (int i = 0; i < gr->maxVert; i++) {
        if (gr->listaAdj[i].ativo) {
            copiarParaLista(gr->listaAdj[i].listaEdges, arestas, false);
        }
    }
}

// --- PERCURSOS (DFS / BFS) ---

static bool dfsRec(struct graph* gr, int u, 
                   procEdge tree, procEdge fwd, procEdge ret, procEdge cross, 
                   void* extra) {
    
    // MUDANÇA: Usando ENUM
    gr->listaAdj[u].cor = CINZA;
    gr->tempo++;
    gr->listaAdj[u].td = (double) gr->tempo;

    Lista* l = gr->listaAdj[u].listaEdges;
    int tam = tamanhoLista(l);
    bool abortar = false;

    for (int k = 0; k < tam; k++) {
        struct edge* e = (struct edge*) removerInicio(l);
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
                // Back Edge
                if (ret != NULL) {
                    continuar = ret((Graph*)gr, (Edge*)e, gr->listaAdj[u].td, 0.0, extra);
                }
                if (!continuar) {
                    abortar = true;
                }

            } else {
                // Preto
                if (gr->listaAdj[u].td < gr->listaAdj[v].td) {
                    // Forward
                    if (fwd != NULL) {
                        continuar = fwd((Graph*)gr, (Edge*)e, gr->listaAdj[u].td, gr->listaAdj[v].td, extra);
                    }
                } else {
                    // Cross
                    if (cross != NULL) {
                        continuar = cross((Graph*)gr, (Edge*)e, gr->listaAdj[u].td, gr->listaAdj[v].td, extra);
                    }
                }

                if (!continuar) {
                    abortar = true;
                }
            }
        }
        
        inserirFim(l, (void*)e); // Devolve aresta
    }

    // MUDANÇA: Usando ENUM
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
    
    struct graph* gr = (struct graph*) g;
    if (gr == NULL) {
        return false;
    }

    // Inicializa
    for (int i = 0; i < gr->maxVert; i++) {
        gr->listaAdj[i].cor = BRANCO; // Enum
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
    struct graph* gr = (struct graph*) g;
    if (gr == NULL) {
        return false;
    }
    if (node < 0 || node >= gr->maxVert) {
        return false;
    }

    procEdge onDiscover = (procEdge) discoverNode;

    for (int i = 0; i < gr->maxVert; i++) {
        gr->listaAdj[i].cor = BRANCO; // Enum
        gr->listaAdj[i].td = 0.0;
    }

    Lista* fila = criaLista();
    if (fila == NULL) {
        return false;
    }

    gr->listaAdj[node].cor = CINZA; // Enum
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
            struct edge* e = (struct edge*) removerInicio(l);
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
                // Não break aqui
            }
        }
        gr->listaAdj[u].cor = PRETO; // Enum

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