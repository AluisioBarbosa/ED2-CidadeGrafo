#include "STreap.h"


typedef struct BoundingBox{
    double x1, x2, y1, y2;
} BoundingBox;

struct info{
    void *dado;
};

typedef struct streapNode{
    double coordenadaX;
    double coordenadaY;
    double prioridade;
    BoundingBox* box;
    Info info;
    struct streapNode *esquerda;
    struct streapNode *direita;
}StreapNode;

struct streap{
    StreapNode* raiz;
    double epsilon;
    int altura;
    int nRoot;
};

