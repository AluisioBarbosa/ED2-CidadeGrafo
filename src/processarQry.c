#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h> 
#include <stdbool.h>
#include "processarQry.h"
#include "svg.h"
#include "quadra.h"
#include "digraph.h"
#include "lista.h"
#include "hashTable.h"
#include "djikstra.h"
#include "info.h" 

struct {
    double x;
    double y;
    bool valido;
} EstadoGPS = {0.0, 0.0, false};

static int pontoInterno(double x, double y, double rx, double ry, double rw, double rh) {
    if (x >= rx && x <= rx + rw) {
        if (y >= ry && y <= ry + rh) {
            return 1;
        }
    }
    return 0;
}

static int resolverEndereco(HashTable* tQuadras, char* cep, char* face, double num, double* x, double* y) {
    Quadra* q = (Quadra*) buscarHashTable(tQuadras, cep);
    
    if (q == NULL) {
        return 0;
    }

    double qx = quadra_get_x(q);
    double qy = quadra_get_y(q);
    double qw = quadra_get_largura(q);
    double qh = quadra_get_altura(q);

    if (strcmp(face, "N") == 0) {
        *x = qx + num;
        *y = qy + qh;
    } else if (strcmp(face, "S") == 0) {
        *x = qx + num;
        *y = qy;
    } else if (strcmp(face, "L") == 0) {
        *x = qx;
        *y = qy + num;
    } else if (strcmp(face, "O") == 0) {
        *x = qx + qw;
        *y = qy + num;
    } else {
        *x = qx; 
        *y = qy;
    }
    return 1;
}

static int encontrarNoMaisProximo(Graph* vias, double px, double py) {
    if (vias == NULL) return -1;
    
    int maxV = getMaxNodes(vias);
    
    int melhorNo = -1;
    double menorDistQ = DBL_MAX;

    for (int i = 0; i < maxV; i++) {
        int idNode = i;
        InfoVertice* info = (InfoVertice*) getNodeInfo(vias, idNode);
        
        if (info != NULL) {
            double vx = getXVertice(info);
            double vy = getYVertice(info);
            
            double distQ = (vx - px)*(vx - px) + (vy - py)*(vy - py);
            
            if (distQ < menorDistQ) {
                menorDistQ = distQ;
                melhorNo = i;
            }
        }
    }
    
    return melhorNo;
}


static double pesoDistancia(Graph *g, Edge *e) {
    InfoAresta* info = (InfoAresta*) getEdgeInfo(g, e);
    
    if (info == NULL){
        return -1.0;
    }
    if (!isArestaAtiva(info)){
        return -1.0;
    }
    return getComprimento(info);
}

static double pesoTempo(Graph *g, Edge *e) {
    InfoAresta* info = (InfoAresta*) getEdgeInfo(g, e);
    if (info == NULL) return -1.0;
    if (!isArestaAtiva(info)) return -1.0;

    double vm = getVelocidade(info);
    if (vm <= 0.001) return DBL_MAX; 

    return getComprimento(info) / (vm / 3.6);
}


static void cmd_pnt(HashTable* idQuadras, char* cep, char* cFill, char* cStrk, FILE* txt) {
    Quadra* q = (Quadra*) buscarHashTable(idQuadras, cep);
    if (q != NULL) {
        quadra_set_cores(q, cFill, cStrk, quadra_get_espessura(q));
        fprintf(txt, "pnt: Quadra %s pintada de %s/%s.\n", cep, cFill, cStrk);
    } else {
        fprintf(txt, "pnt: Quadra %s nao encontrada.\n", cep);
    }
}

static void cmd_onde(HashTable* idQuadras, char* cep, char* face, double num, FILE* txt, SvgFile* svg) {
    double x, y;
    if (resolverEndereco(idQuadras, cep, face, num, &x, &y)) {
        fprintf(txt, "@o?: Endereco %s/%s/%.1f -> (%.2f, %.2f)\n", cep, face, num, x, y);
        
        if (svg) {
            svg_desenhar_ponto(svg, x, y, "orange", " ");
        }

        EstadoGPS.x = x;
        EstadoGPS.y = y;
        EstadoGPS.valido = true;

    } else {
        fprintf(txt, "@o?: Endereco %s invalido.\n", cep);
        EstadoGPS.valido = false; 
    }
}

static void cmd_blq(Graph* vias, HashTable* idBloqueios, char* nome, double x, double y, double w, double h, FILE* txt) {
    Lista* listaTotal = criaLista();
    getEdges(vias, listaTotal);
    Lista* bloqueadas = criaLista();

    while (!listaVazia(listaTotal)) {
        Edge* e = (Edge*) removerInicio(listaTotal);
        int u = getFromNode(vias, e);
        int v = getToNode(vias, e);
        InfoVertice* iU = (InfoVertice*) getNodeInfo(vias, u);
        InfoVertice* iV = (InfoVertice*) getNodeInfo(vias, v);

        if (iU && iV) {
            double ux = getXVertice(iU); double uy = getYVertice(iU);
            double vx = getXVertice(iV); double vy = getYVertice(iV);
            
            if (pontoInterno(ux, uy, x, y, w, h) || pontoInterno(vx, vy, x, y, w, h)) {
                void* infoA = getEdgeInfo(vias, e);
                if (infoA) {
                    setArestaAtiva(infoA, false);
                    inserirFim(bloqueadas, (void*)e);
                }
            }
        }
    }
    liberaLista(listaTotal);
    inserirHashTable(idBloqueios, nome, (void*)bloqueadas);
    fprintf(txt, "blq: Bloqueio '%s' aplicado.\n", nome);
}

static void cmd_rbl(Graph* vias, HashTable* idBloqueios, char* nome, FILE* txt) {
    Lista* lista = (Lista*) buscarHashTable(idBloqueios, nome);
    if (lista) {
        while (!listaVazia(lista)) {
            Edge* e = (Edge*) removerInicio(lista);
            void* infoA = getEdgeInfo(vias, e);
            if (infoA) setArestaAtiva(infoA, true);
        }
        liberaLista(lista);
        fprintf(txt, "rbl: Bloqueio '%s' removido.\n", nome);
    } else {
        fprintf(txt, "rbl: Bloqueio '%s' nao encontrado.\n", nome);
    }
}
static int retanguloDentroRetangulo(double r1x, double r1y, double r1w, double r1h, 
                                    double r2x, double r2y, double r2w, double r2h) {
    return (r1x >= r2x && 
            r1y >= r2y && 
            r1x + r1w <= r2x + r2w && 
            r1y + r1h <= r2y + r2h);
}

static void cmd_catac(STreap* arvore, Graph* vias, HashTable* idQuadras, 
                      double x, double y, double w, double h, FILE* txt, SvgFile* svg) {
    
    fprintf(txt, "catac: Regiao (%.1f, %.1f, %.1f, %.1f)\n", x, y, w, h);
    
    if (svg) {
        svg_desenhar_regiao_catac(svg, x, y, w, h, "#AB37C8", "#AA0044", 0.5);
    }

    if (arvore && vias) {
        Lista* listaNos = criaLista();
        getStreapNodeRegiaoSTrp(arvore, x, y, w, h, listaNos);
        while(!listaVazia(listaNos)) {
            void* no = removerInicio(listaNos);
            Quadra* q = (Quadra*) *getInfoSTrp(arvore, no);
            
            if (q && quadra_is_ativa(q)) {
                if (retanguloDentroRetangulo(quadra_get_x(q), quadra_get_y(q), 
                                             quadra_get_largura(q), quadra_get_altura(q), x, y, w, h)) {
                    fprintf(txt, " - Quadra removida: %s\n", quadra_get_cep(q));
                    quadra_set_ativa(q, false); // Fundamental para o SVG não desenhar depois
                    deleteStreapNodeSTrp(arvore, no);
                }
            }
        }
        liberaLista(listaNos);

        int maxV = getMaxNodes(vias);
        for(int i=0; i<maxV; i++) {
            int id = i;
            InfoVertice* iv = (InfoVertice*) getNodeInfo(vias, id);
            if (iv) {
                if (pontoInterno(getXVertice(iv), getYVertice(iv), x, y, w, h)) {
                    fprintf(txt, " - Vertice removido: %s\n", getIdVertice(iv));
                    removeNodeAndEdges(vias, id);
                    destroiInfoVertice(iv);
                    setNodeInfo(vias, id, NULL);
                }
            }
        }
    }
}

static void cmd_path(Graph* vias, HashTable* idQuadras, 
                     char* cepD, char* faceD, double numD,             
                     char* cor1, char* cor2, FILE* txt, SvgFile* svg) {

    static int pathCounter = 0; 
    pathCounter++;

    char idDist[32], idTempo[32];
    sprintf(idDist, "path_d_%d", pathCounter);
    sprintf(idTempo, "path_t_%d", pathCounter);
    
    if (!EstadoGPS.valido) {
        fprintf(txt, "p?: Erro - Nenhuma origem definida (use @o? antes de p?).\n");
        return;
    }

    double xOrigem = EstadoGPS.x;
    double yOrigem = EstadoGPS.y;
    double xDest, yDest;
    
    if (!resolverEndereco(idQuadras, cepD, faceD, numD, &xDest, &yDest)) {
        fprintf(txt, "p?: Destino %s invalido ou removido.\n", cepD);
        return;
    }

    if (svg) {
        svg_desenhar_marcador_vertical(svg, EstadoGPS.x, EstadoGPS.y, "blue", "origem");
        svg_desenhar_marcador_vertical(svg, xDest, yDest, "red", "destino");
    }

    int noO = encontrarNoMaisProximo(vias, xOrigem, yOrigem);
    int noD = encontrarNoMaisProximo(vias, xDest, yDest);

    if (noO == -1 || noD == -1) {
        fprintf(txt, "p?: Impossivel mapear coordenadas para o grafo viario.\n");
        return;
    }

    Caminho* cDist = dijkstra(vias, noO, noD, pesoDistancia);
    if (cDist) {
        fprintf(txt, "p? (Menor Distancia): %.2f metros.\n", caminho_get_custo(cDist));
        if (svg) {
            svg_desenhar_caminho(svg, vias, caminho_get_lista(cDist), cor1, idDist);
        }
        destruirCaminho(cDist);
    } else {
        fprintf(txt, "p? (Menor Distancia): Nao ha caminho.\n");
    }

    Caminho* cTempo = dijkstra(vias, noO, noD, pesoTempo);
    if (cTempo) {
        fprintf(txt, "p? (Mais Rapido): %.2f segundos.\n", caminho_get_custo(cTempo));
        if (svg) {
            static int contadorPath = 0;
            char idUnico[32];
            sprintf(idUnico, "rota_tempo_%d", contadorPath++);
            
            svg_desenhar_caminho(svg, vias, caminho_get_lista(cTempo), cor2, idTempo);
        }
        destruirCaminho(cTempo);
    } else {
        fprintf(txt, "p? (Mais Rapido): Nao ha caminho.\n");
    }
}



void processarQry(char* pathQry, char* dirSaida, char* nomeBaseGeo, char* nomeBaseQry,
                  HashTable* idQuadras, Graph* vias, HashTable* idBloqueios, STreap* arvoreQuadras, HashTable* idVertices) {
    
    if (!pathQry) return;

    EstadoGPS.x = 0; EstadoGPS.y = 0; EstadoGPS.valido = false;

    char pathTxt[512];
    if (dirSaida[strlen(dirSaida)-1] == '/')
        sprintf(pathTxt, "%s%s-%s.txt", dirSaida, nomeBaseGeo, nomeBaseQry);
    else
        sprintf(pathTxt, "%s/%s-%s.txt", dirSaida, nomeBaseGeo, nomeBaseQry);

    FILE* fqry = fopen(pathQry, "r");
    FILE* ftxt = fopen(pathTxt, "w");
    if (!fqry) return;

    char cmd[10];
    char s1[256], s2[256], s3[256], s4[256], s5[256], s6[256];
    double d1, d2, d3, d4;


    
    while (fscanf(fqry, "%s", cmd) != EOF) {
        if (strcmp(cmd, "catac") == 0) {
            fscanf(fqry, "%lf %lf %lf %lf", &d1, &d2, &d3, &d4);
            cmd_catac(arvoreQuadras, vias, idQuadras, d1, d2, d3, d4, ftxt, NULL);
        } 
        else {
            if (strcmp(cmd, "pnt") == 0) fscanf(fqry, "%*s %*s %*s");
            else if (strcmp(cmd, "@o?") == 0) fscanf(fqry, "%*s %*s %*lf");
            else if (strcmp(cmd, "blq") == 0) fscanf(fqry, "%*s %*s %*lf %*lf %*lf %*lf");
            else if (strcmp(cmd, "rbl") == 0) fscanf(fqry, "%*s");
            else if (strcmp(cmd, "p?") == 0) fscanf(fqry, "%*s %*s %*lf %*s %*s");
        }
    }

    SvgFile* svg = svg_criar(dirSaida, nomeBaseGeo, nomeBaseQry, arvoreQuadras, vias);
    if (svg) svg_desenhar_quadras(svg, arvoreQuadras);


    rewind(fqry);

    while (fscanf(fqry, "%s", cmd) != EOF) {
        
        if (strcmp(cmd, "pnt") == 0) {
            fscanf(fqry, "%s %s %s", s1, s2, s3);
            cmd_pnt(idQuadras, s1, s2, s3, ftxt);
        }
        else if (strcmp(cmd, "@o?") == 0) {
            fscanf(fqry, "%s %s %lf", s1, s2, &d1);
            cmd_onde(idQuadras, s1, s2, d1, ftxt, svg);
        }
        else if (strcmp(cmd, "blq") == 0) {
            fscanf(fqry, "%s %s %lf %lf %lf %lf", s1, s2, &d1, &d2, &d3, &d4);
            cmd_blq(vias, idBloqueios, s1, d1, d2, d3, d4, ftxt);
        }
        else if (strcmp(cmd, "rbl") == 0) {
            fscanf(fqry, "%s", s1);
            cmd_rbl(vias, idBloqueios, s1, ftxt);
        }
        else if (strcmp(cmd, "p?") == 0) {
            fscanf(fqry, "%s %s %lf %s %s", s1, s2, &d1, s3, s4);
            cmd_path(vias, idQuadras, s1, s2, d1, s3, s4, ftxt, svg);
        }
        else if (strcmp(cmd, "catac") == 0) {
            fscanf(fqry, "%lf %lf %lf %lf", &d1, &d2, &d3, &d4);
            if (svg) {
                svg_desenhar_regiao_catac(svg, d1, d2, d3, d4, "#AB37C8", "#AA0044", 0.5);
            }
        }
    }

    if (svg) svg_finalizar(svg);
    if (ftxt) fclose(ftxt);
    fclose(fqry);
}