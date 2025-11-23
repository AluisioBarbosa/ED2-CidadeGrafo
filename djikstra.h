#include "digraph.h"

typedef struct caminho Caminho;


typedef double (*ObterPesoFunc)(Graph *g, Edge *e);
Caminho* dijkstra(Graph *g, int origem, int destino, ObterPesoFunc getPeso);
void destruirCaminho(Caminho* c);

Lista* caminho_get_lista(Caminho* c);
double caminho_get_custo(Caminho* c);