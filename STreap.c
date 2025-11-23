#include "STreap.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>


typedef struct BoundingBox {
    double x1, x2, y1, y2;
} BoundingBox;

struct streapNode {
    double coordenadaX;
    double coordenadaY;
    double prioridade;
    BoundingBox* box;
    Info info;
    struct streapNode *esquerda;
    struct streapNode *direita;
};

struct streap {
    StreapNode* raiz;
    double epsilon;
    int altura;
    int nRoot;
};


static double min_val(double a, double b) {
    if (a < b) {
        return a;
    } else {
        return b;
    }
}

static double max_val(double a, double b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

static void atualizaBox(StreapNode* no) {
    if (no == NULL) {
        return;
    }
    if (no->box == NULL) {
        return;
    }

    no->box->x1 = no->coordenadaX;
    no->box->x2 = no->coordenadaX;
    no->box->y1 = no->coordenadaY;
    no->box->y2 = no->coordenadaY;

    if (no->esquerda != NULL) {
        if (no->esquerda->box != NULL) {
            no->box->x1 = min_val(no->box->x1, no->esquerda->box->x1);
            no->box->x2 = max_val(no->box->x2, no->esquerda->box->x2);
            no->box->y1 = min_val(no->box->y1, no->esquerda->box->y1);
            no->box->y2 = max_val(no->box->y2, no->esquerda->box->y2);
        }
    }

    if (no->direita != NULL) {
        if (no->direita->box != NULL) {
            no->box->x1 = min_val(no->box->x1, no->direita->box->x1);
            no->box->x2 = max_val(no->box->x2, no->direita->box->x2);
            no->box->y1 = min_val(no->box->y1, no->direita->box->y1);
            no->box->y2 = max_val(no->box->y2, no->direita->box->y2);
        }
    }
}

static StreapNode* rotacaoDireita(StreapNode* y) {
    StreapNode* x = y->esquerda;
    y->esquerda = x->direita;
    x->direita = y;

    atualizaBox(y);
    atualizaBox(x);

    return x;
}

static StreapNode* rotacaoEsquerda(StreapNode* x) {
    StreapNode* y = x->direita;
    x->direita = y->esquerda;
    y->esquerda = x;

    atualizaBox(x);
    atualizaBox(y);

    return y;
}

static int comparaChaves(double x1, double y1, double x2, double y2, double eps) {
    if (fabs(x1 - x2) > eps) {
        if (x1 < x2) {
            return -1;
        } else {
            return 1;
        }
    }
    if (fabs(y1 - y2) > eps) {
        if (y1 < y2) {
            return -1;
        } else {
            return 1;
        }
    }
    return 0;
}

static StreapNode* criaNo(double x, double y, Info info) {
    StreapNode* no = (StreapNode*) malloc(sizeof(StreapNode));
    if (no == NULL) {
        return NULL;
    }

    no->box = (BoundingBox*) malloc(sizeof(BoundingBox));
    if (no->box == NULL) {
        free(no);
        return NULL;
    }

    no->coordenadaX = x;
    no->coordenadaY = y;
    no->prioridade = (double) rand() / RAND_MAX;
    no->info = info;
    no->esquerda = NULL;
    no->direita = NULL;

    atualizaBox(no);
    return no;
}

static StreapNode* insereRec(StreapNode* no, double x, double y, Info info, double eps, StreapNode** noInserido) {
    if (no == NULL) {
        *noInserido = criaNo(x, y, info);
        return *noInserido;
    }

    int cmp = comparaChaves(x, y, no->coordenadaX, no->coordenadaY, eps);

    if (cmp < 0) {
        no->esquerda = insereRec(no->esquerda, x, y, info, eps, noInserido);
        if (no->esquerda->prioridade > no->prioridade) {
            no = rotacaoDireita(no);
        }
    } else if (cmp > 0) {
        no->direita = insereRec(no->direita, x, y, info, eps, noInserido);
        if (no->direita->prioridade > no->prioridade) {
            no = rotacaoEsquerda(no);
        }
    } else {
        *noInserido = NULL; 
    }

    atualizaBox(no);
    return no;
}

static StreapNode* removeRec(StreapNode* no, double x, double y, double eps, Info* infoRemovida) {
    if (no == NULL) {
        return NULL;
    }

    int cmp = comparaChaves(x, y, no->coordenadaX, no->coordenadaY, eps);

    if (cmp < 0) {
        no->esquerda = removeRec(no->esquerda, x, y, eps, infoRemovida);
    } else if (cmp > 0) {
        no->direita = removeRec(no->direita, x, y, eps, infoRemovida);
    } else {
        if (*infoRemovida == NULL) {
            *infoRemovida = no->info;
        }

        if (no->esquerda == NULL) {
            StreapNode* temp = no->direita;
            free(no->box);
            free(no);
            return temp;
        } else if (no->direita == NULL) {
            StreapNode* temp = no->esquerda;
            free(no->box);
            free(no);
            return temp;
        }

        if (no->esquerda->prioridade < no->direita->prioridade) {
            no = rotacaoEsquerda(no);
            no->esquerda = removeRec(no->esquerda, x, y, eps, infoRemovida);
        } else {
            no = rotacaoDireita(no);
            no->direita = removeRec(no->direita, x, y, eps, infoRemovida);
        }
    }

    if (no != NULL) {
        atualizaBox(no);
    }
    return no;
}

static int temInterseccao(double r1x1, double r1y1, double r1x2, double r1y2,
                          double r2x, double r2y, double r2w, double r2h) {
    double r2x2 = r2x + r2w;
    double r2y2 = r2y + r2h;

    if (r1x1 > r2x2 || r2x > r1x2) {
        return 0;
    }
    if (r1y1 > r2y2 || r2y > r1y2) {
        return 0;
    }
    return 1;
}

static void buscaRegiaoRec(StreapNode* no, double x, double y, double w, double h, Lista* res) {
    if (no == NULL) {
        return;
    }

    if (!temInterseccao(no->box->x1, no->box->y1, no->box->x2, no->box->y2, x, y, w, h)) {
        return;
    }

    if (no->coordenadaX >= x && no->coordenadaX <= (x + w)) {
        if (no->coordenadaY >= y && no->coordenadaY <= (y + h)) {
            inserirFim(res, (void*)no);
        }
    }

    buscaRegiaoRec(no->esquerda, x, y, w, h, res);
    buscaRegiaoRec(no->direita, x, y, w, h, res);
}

static void killRec(StreapNode* no) {
    if (no == NULL) {
        return;
    }
    killRec(no->esquerda);
    killRec(no->direita);
    free(no->box);
    free(no);
}


STreap* createSTrp(double epsilon) {
    STreap* t = (STreap*) malloc(sizeof(STreap));
    if (t != NULL) {
        t->raiz = NULL;
        t->epsilon = epsilon;
        t->altura = 0;
        t->nRoot = 0;
    }
    return (STreap*) t;
}

StreapNode* insertSTrp(STreap* t, double x, double y, Info info) {
    STreap* st = (STreap*) t;
    if (st == NULL) {
        return NULL;
    }
    
    StreapNode* inserido = NULL;
    st->raiz = insereRec(st->raiz, x, y, info, st->epsilon, &inserido);
    
    if (inserido != NULL) {
        st->nRoot++;
    }
    
    return inserido;
}

void getStreapNodeRegiaoSTrp(STreap* t, double x, double y, double w, double h, Lista* resultado) {
    STreap* st = (STreap*) t;
    if (st == NULL) {
        return;
    }
    buscaRegiaoRec(st->raiz, x, y, w, h, resultado);
}

Info* getInfoSTrp(STreap* t, StreapNode* n) {
    if (n != NULL) {
        return &(n->info);
    }
    return NULL;
}

StreapNode* getStreapNodeSTrp(STreap* t, double xa, double ya) {
    STreap* st = (STreap*) t;
    if (st == NULL) {
        return NULL;
    }
    
    StreapNode* atual = st->raiz;
    while (atual != NULL) {
        int cmp = comparaChaves(xa, ya, atual->coordenadaX, atual->coordenadaY, st->epsilon);
        if (cmp == 0) {
            return atual;
        } else if (cmp < 0) {
            atual = atual->esquerda;
        } else {
            atual = atual->direita;
        }
    }
    return NULL;
}

void updateInfoSTrp(STreap* t, StreapNode* n, Info i) {
    if (n != NULL) {
        n->info = i;
    }
}

Info* deleteStreapNodeSTrp(STreap* t, StreapNode* n) {
    if (t == NULL || n == NULL) {
        return NULL;
    }
    return removeSTrp(t, n->coordenadaX, n->coordenadaY);
}

Info* removeSTrp(STreap* t, double xa, double ya) {
    STreap* st = (STreap*) t;
    if (st == NULL) {
        return NULL;
    }
    
    Info infoRemovida = NULL;
    st->raiz = removeRec(st->raiz, xa, ya, st->epsilon, &infoRemovida);
    
    if (infoRemovida != NULL) {
        st->nRoot--;
        static Info temp;
        temp = infoRemovida;
        return &temp;
    }
    return NULL;
}

void killSTrp(STreap* t) {
    STreap* st = (STreap*) t;
    if (st != NULL) {
        killRec(st->raiz);
        free(st);
    }
}


static void percursoProfundidadeRec(StreapNode* no, FvisitaNo fVisita, void *aux) {
    if (no == NULL) {
        return;
    }
    
    fVisita(no->info, no->coordenadaX, no->coordenadaY, 
            no->box->x1, no->box->y1, no->box->x2, no->box->y2, aux);
    
    percursoProfundidadeRec(no->esquerda, fVisita, aux);
    percursoProfundidadeRec(no->direita, fVisita, aux);
}

static void percursoSimetricoRec(StreapNode* no, FvisitaNo fVisita, void *aux) {
    if (no == NULL) {
        return;
    }
    
    percursoSimetricoRec(no->esquerda, fVisita, aux);
    
    fVisita(no->info, no->coordenadaX, no->coordenadaY, 
            no->box->x1, no->box->y1, no->box->x2, no->box->y2, aux);
            
    percursoSimetricoRec(no->direita, fVisita, aux);
}

static void printSTrpRec(StreapNode* no, FILE* f) {
    if (no == NULL) {
        return;
    }
    
    fprintf(f, "    n%p [label=\"{ (%.1f, %.1f) | P:%.2f | Box:[%.1f, %.1f, %.1f, %.1f] }\"];\n", 
            (void*)no, no->coordenadaX, no->coordenadaY, no->prioridade,
            no->box->x1, no->box->y1, no->box->x2, no->box->y2);

    if (no->esquerda != NULL) {
        fprintf(f, "    n%p -> n%p [color=blue];\n", (void*)no, (void*)no->esquerda);
        printSTrpRec(no->esquerda, f);
    } else {
        fprintf(f, "    nullL%p [shape=point];\n", (void*)no);
        fprintf(f, "    n%p -> nullL%p [color=blue];\n", (void*)no, (void*)no);
    }
    if (no->direita != NULL) {
        fprintf(f, "    n%p -> n%p [color=red];\n", (void*)no, (void*)no->direita);
        printSTrpRec(no->direita, f);
    } else {
        fprintf(f, "    nullR%p [shape=point];\n", (void*)no);
        fprintf(f, "    n%p -> nullR%p [color=red];\n", (void*)no, (void*)no);
    }
}


void percursoProfundidade(STreap* t, FvisitaNo fVisita, void *aux) {
    STreap* st = t;
    if (st == NULL) {
        return;
    }
    percursoProfundidadeRec(st->raiz, fVisita, aux);
}

void percursoSimetrico(STreap* t, FvisitaNo fVisita, void *aux) {
    STreap* st = t;
    if (st == NULL) {
        return;
    }
    percursoSimetricoRec(st->raiz, fVisita, aux);
}

void percursoLargura(STreap* t, FvisitaNo fVisita, void *aux) {
    STreap* st = t;
    if (st == NULL) {
        return;
    }
    if (st->raiz == NULL) {
        return;
    }
    

    Lista* l = criaLista();
    inserirFim(l, (void*)st->raiz);
    
    while (!listaVazia(l)) {
        StreapNode* no = (StreapNode*) removerInicio(l);
        if (no != NULL) {
            fVisita(no->info, no->coordenadaX, no->coordenadaY, 
                    no->box->x1, no->box->y1, no->box->x2, no->box->y2, aux);
            
            if (no->esquerda != NULL) {
                inserirFim(l, (void*)no->esquerda);
            }
            if (no->direita != NULL) {
                inserirFim(l, (void*)no->direita);
            }
        }
    }
    liberaLista(l);
}

void printSTrp(STreap* t, char *nomeArq) {
    STreap* st = t;
    if (st == NULL) {
        return;
    }
    
    FILE* f = fopen(nomeArq, "w");
    if (f == NULL) {
        return;
    }

    fprintf(f, "digraph STreap {\n");
    fprintf(f, "    node [shape=record, fontname=\"Arial\"];\n");

    printSTrpRec(st->raiz, f);
    
    fprintf(f, "}\n");
    fclose(f);
}