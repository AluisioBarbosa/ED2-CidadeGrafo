#include <stdbool.h>


/**
 * @brief estruturas de informações dos vértices e arestas
 * Esse modulo serve para controlar as informações associadas aos vértices e arestas do grafo.
 * Essas informações podem incluir atributos como nomes, coordenadas geográficas, velocidades, comprimentos, entre outros.
 * 
 */
typedef struct infoAresta InfoAresta;
typedef struct infoVertice InfoVertice;
// construtores
InfoVertice* criaInfoVertice(char* id, double x, double y);
InfoAresta* criaInfoAresta(char* nome, char* ldir, char* lesq, double cmp, double vm);

// destruidores
void destroiInfoVertice(void* i);
void destroiInfoAresta(void* i);

// getters e setters
double getVelocidade(void*);
double getComprimento(void*);
bool isArestaAtiva(void*);
double getXVertice(void*);
double getYVertice(void*);
char* getIdVertice(void*);
void setArestaAtiva(void* infoAresta, bool status);
