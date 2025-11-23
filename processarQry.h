#include "STreap.h"
#include "digraph.h"
#include "hashTable.h"


void processarQry(char* pathQry, char* dirSaida, char* nomeBaseGeo, char* nomeBaseQry,
     HashTable* idQuadras, Graph* vias, HashTable* idBloqueios, STreap* arvoreQuadras, HashTable* idVertices);