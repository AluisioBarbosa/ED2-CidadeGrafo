#include <stdio.h>
#include <stdlib.h>
#include <float.h> 
#include <stdbool.h>
#include "djikstra.h"
#include "digraph.h"
#include "lista.h"


struct caminho {
    Lista* nos;         
    double custoTotal;  
};

static Caminho* criarEstruturaCaminho() {
    Caminho* c = (Caminho*) malloc(sizeof(struct caminho));
    if (c != NULL) {
        c->nos = criaLista(); 
        c->custoTotal = 0.0;
    }
    return c;
}


Caminho* dijkstra(Graph *g, int origem, int destino, ObterPesoFunc getPeso) {
    if (g == NULL) {
        return NULL;
    }
    
    int maxV = getMaxNodes(g);
    
    if (origem < 0 || origem >= maxV) {
        return NULL;
    }
    if (destino < 0 || destino >= maxV) {
        return NULL;
    }

    double* dist = (double*) malloc(maxV * sizeof(double));
    int* pai = (int*) malloc(maxV * sizeof(int));
    bool* visitado = (bool*) malloc(maxV * sizeof(bool));
    
    if (dist == NULL || pai == NULL || visitado == NULL) {
        if (dist) { 
            free(dist); 
        }
        if (pai) { 
            free(pai); 
        }
        if (visitado) {
             free(visitado); 
        }
        return NULL;
    }

    for (int i = 0; i < maxV; i++) {
        dist[i] = DBL_MAX; 
        pai[i] = -1;  
        visitado[i] = false;
    }

    dist[origem] = 0.0;

    for (int i = 0; i < maxV; i++) {
        
        int u = -1;
        double minDist = DBL_MAX;

        for (int v = 0; v < maxV; v++) {
            if (visitado[v] == false) {
                if (dist[v] < minDist) {
                    minDist = dist[v];
                    u = v;
                }
            }
        }

        if (u == -1 || dist[u] == DBL_MAX) {
            break; 
        }
        if (u == destino) {
            break; 
        }

        visitado[u] = true;

        Lista* arestasVizinhas = criaLista();
        adjacentEdges(g, u, arestasVizinhas);

        while (!listaVazia(arestasVizinhas)) {
            Edge* e = (Edge*) removerInicio(arestasVizinhas); 
            int v = getToNode(g, e);
            
            double peso = getPeso(g, e);

            if (peso >= 0.0) {
                if (dist[u] + peso < dist[v]) {
                    dist[v] = dist[u] + peso;
                    pai[v] = u;
                }
            }
        }
        
        liberaLista(arestasVizinhas); 
    }

    Caminho* resultado = NULL;

    if (dist[destino] != DBL_MAX) {
        resultado = criarEstruturaCaminho();
        resultado->custoTotal = dist[destino];

        int atual = destino;
        
        while (atual != -1) {
            int* noPtr = (int*) malloc(sizeof(int));
            *noPtr = atual;
            
            inserirInicio(resultado->nos, (void*)noPtr);
            
            atual = pai[atual];
        }
    }

    free(dist);
    free(pai);
    free(visitado);

    return resultado;
}

void destruirCaminho(Caminho* c) {
    if (c != NULL) {
        if (c->nos != NULL) {
            while (!listaVazia(c->nos)) {
                void* dado = removerInicio(c->nos);
                free(dado); 
            }
            liberaLista(c->nos);
        }
        free(c);
    }
}

Lista* caminho_get_lista(Caminho* c){
    return c->nos;
}

double caminho_get_custo(Caminho* c){
    return c->custoTotal;
}