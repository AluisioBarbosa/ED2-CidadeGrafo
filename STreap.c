#include "STreap.h"

struct info{
    void *dado;
};

typedef struct streapNode{
    double coordenadaX;
    double coordenadaY;
    double prioridade;
    Info info;
    struct streapNode *esquerda;
    struct streapNode *direita;
}StreapNode;

struct streap{
    StreapNode* raiz;
    double epsilon;
};

