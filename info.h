#include <stdbool.h>
typedef struct infoAresta InfoAresta;
typedef struct infoVertice InfoVertice;

InfoVertice* criaInfoVertice(char* id, double x, double y);
InfoAresta* criaInfoAresta(char* nome, char* ldir, char* lesq, double cmp, double vm);

void destroiInfoVertice(void* i);
void destroiInfoAresta(void* i);

double getVelocidade(void*);
double getComprimento(void*);
bool isArestaAtiva(void*);
double getXVertice(void*);
double getYVertice(void*);
char* getIdVertice(void*);


void setArestaAtiva(void* infoAresta, bool status);