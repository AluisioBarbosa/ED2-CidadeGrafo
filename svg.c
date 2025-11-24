#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "svg.h"
#include "quadra.h" 
#include "info.h"  
#include "STreap.h"
#include "digraph.h"
#define _POSIX_C_SOURCE 200809L

struct svgFile {
    FILE* f;
    double topoMapY;
};
typedef struct {
    double minX, minY;
    double maxX, maxY;
} BBox;


static void callback_desenhar_quadra(void* info, double x, double y, double mbbX1, double mbbY1, double mbbX2, double mbbY2, void* aux) {
    FILE* f = (FILE*) aux;
    Quadra* q = (Quadra*) info; 
    if (q != NULL && f != NULL) {
        fprintf(f, "\t<rect x=\"%.2f\" y=\"%.2f\" width=\"%.2f\" height=\"%.2f\" "
                   "style=\"fill:%s;stroke:%s;stroke-width:%.2f\" opacity=\"0.9\" />\n",
                quadra_get_x(q),
                quadra_get_y(q),
                quadra_get_largura(q),
                quadra_get_altura(q),
                quadra_get_cor_preenchimento(q),
                quadra_get_cor_borda(q),
                quadra_get_espessura(q));
        fprintf(f, "\t<text x=\"%.2f\" y=\"%.2f\" font-size=\"10\" fill=\"black\">%s</text>\n",
                quadra_get_x(q) + 5, 
                quadra_get_y(q) + 15, 
                quadra_get_cep(q));
        
    }
}
static void callback_limites_quadra(void* info, double x, double y, double mbbX1, double mbbY1, double mbbX2, double mbbY2, void* aux) {
    BBox* box = (BBox*) aux;
    Quadra* q = (Quadra*) info;
    
    if (q) {
        double qx = quadra_get_x(q);
        double qy = quadra_get_y(q);
        double qw = quadra_get_largura(q);
        double qh = quadra_get_altura(q);

        if (qx < box->minX) box->minX = qx;
        if (qy < box->minY) box->minY = qy;
        if (qx + qw > box->maxX) box->maxX = qx + qw;
        if (qy + qh > box->maxY) box->maxY = qy + qh;
    }
}


SvgFile* svg_criar(char* diretorio, char* nomeGeo, char* nomeQry, STreap* arvore, Graph* vias) {
    if (!diretorio || !nomeGeo) return NULL; 

    // Calcula tamanho
    size_t len = strlen(diretorio) + strlen(nomeGeo) + 7;
    if (nomeQry) len += strlen(nomeQry) + 1; // +1 do hifen

    char* path = (char*) malloc(len);
    
    // Monta caminho
    if (nomeQry && strlen(nomeQry) > 0) {
        // Com QRY: dir/geo-qry.svg
        if (diretorio[strlen(diretorio) - 1] == '/')
            sprintf(path, "%s%s-%s.svg", diretorio, nomeGeo, nomeQry);
        else
            sprintf(path, "%s/%s-%s.svg", diretorio, nomeGeo, nomeQry);
    } else {
        // Sem QRY: dir/geo.svg
        if (diretorio[strlen(diretorio) - 1] == '/')
            sprintf(path, "%s%s.svg", diretorio, nomeGeo);
        else
            sprintf(path, "%s/%s.svg", diretorio, nomeGeo);
    }

    FILE* f = fopen(path, "w");
    free(path);
    if (!f) return NULL;

    SvgFile* svg = (SvgFile*) malloc(sizeof(SvgFile));
    svg->f = f;

    // 2. CALCULA OS LIMITES (Bounding Box)
    BBox box;
    box.minX = DBL_MAX;
    box.minY = DBL_MAX;
    box.maxX = -DBL_MAX;
    box.maxY = -DBL_MAX;

    // A. Percorre a Árvore de Quadras
    if (arvore) {
        // Usa o percurso para visitar todas as quadras e expandir a box
        percursoSimetrico(arvore, callback_limites_quadra, &box);
    }

    // B. Percorre os Vértices do Grafo (se houver pontos fora das quadras)
    if (vias) {
        int maxV = getMaxNodes(vias);
        for (int i = 0; i < maxV; i++) {
            int id = i;
            void* info = getNodeInfo(vias, &id);
            if (info) {
                double vx = getXVertice(info);
                double vy = getYVertice(info);
                if (vx < box.minX) box.minX = vx;
                if (vy < box.minY) box.minY = vy;
                if (vx > box.maxX) box.maxX = vx;
                if (vy > box.maxY) box.maxY = vy;
            }
        }
    }

    // Validação: Se não achou nada (arquivo vazio), define padrão
    if (box.minX == DBL_MAX) {
        box.minX = 0; box.minY = 0; box.maxX = 1000; box.maxY = 1000;
    }

    // 3. Adiciona uma margem (Padding) para não cortar as bordas
    double padding = 50.0;
    box.minX -= padding;
    box.minY -= padding;
    box.maxX += padding;
    box.maxY += padding;
    svg->topoMapY = box.minY + 20.0;

    double width = box.maxX - box.minX;
    double height = box.maxY - box.minY;

    // 4. Escreve o Cabeçalho com viewBox
    // Isso garante que TUDO caiba na tela, não importa se é 100 ou 100000
    fprintf(f, "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" viewBox=\"%.2f %.2f %.2f %.2f\">\n",
            box.minX, box.minY, width, height);
    
    return svg;
}

void svg_desenhar_marcador_vertical(SvgFile* svg, double x, double y, char* cor, char* texto) {
    if (!svg || !svg->f) return;

    double yTopo = svg->topoMapY;

    fprintf(svg->f, "\n\t\n", texto);
    

    fprintf(svg->f, "\t<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" "
                    "style=\"stroke:%s;stroke-width:2;stroke-dasharray:5,5\" />\n",
            x, y, x, yTopo, cor);


    fprintf(svg->f, "\t<circle cx=\"%.2f\" cy=\"%.2f\" r=\"6\" fill=\"%s\" stroke=\"black\" stroke-width=\"1\" />\n", 
            x, y, cor);


    fprintf(svg->f, "\t<text x=\"%.2f\" y=\"%.2f\" fill=\"%s\" text-anchor=\"middle\" font-weight=\"bold\" font-size=\"14\">%s</text>\n", 
            x, yTopo - 5.0, cor, texto);
}

void svg_desenhar_quadras(SvgFile* svg, STreap* arvore) {
    if (svg == NULL || svg->f == NULL || arvore == NULL) {
        return;
    }

    fprintf(svg->f, "\n\t\n");
    
    percursoSimetrico(arvore, callback_desenhar_quadra, (void*) svg->f);
}

void svg_desenhar_caminho(SvgFile* svg, Graph* vias, Lista* listaNos, char* cor, char* idUnico) {
    if (svg == NULL || svg->f == NULL || vias == NULL || listaNos == NULL) {
        return;
    }
    
    if (listaVazia(listaNos)) {
        return;
    }

    fprintf(svg->f, "\n\t\n", idUnico);

    fprintf(svg->f, "\t<path id=\"%s\" d=\"", idUnico);

    int tam = tamanhoLista(listaNos);
    int primeiro = 1; 
    
    for (int i = 0; i < tam; i++) {
        int* ptrId = (int*) removerInicio(listaNos);
        int idNo = *ptrId;

        void* infoV = getNodeInfo(vias, &idNo); 
        
        if (infoV != NULL) {
            double x = getXVertice(infoV);
            double y = getYVertice(infoV);
            
            if (primeiro) {
                fprintf(svg->f, "M %.2f,%.2f ", x, y); 
                primeiro = 0;
            } else {
                fprintf(svg->f, "L %.2f,%.2f ", x, y);
            }
        }

        inserirFim(listaNos, (void*) ptrId); 
    }

    fprintf(svg->f, "\" style=\"fill:none;stroke:%s;stroke-width:4;stroke-opacity:0.6\" />\n", cor);

    fprintf(svg->f, "\t<circle r=\"6\" fill=\"%s\" stroke=\"black\" stroke-width=\"1\">\n", cor);
    

    fprintf(svg->f, "\t\t<animateMotion dur=\"6s\" repeatCount=\"indefinite\" rotate=\"auto\">\n");
    fprintf(svg->f, "\t\t\t<mpath href=\"#%s\"/>\n", idUnico);
    fprintf(svg->f, "\t\t</animateMotion>\n");
    
    fprintf(svg->f, "\t</circle>\n");
}

void svg_desenhar_regiao_catac(SvgFile* svg, double x, double y, double w, double h, char* corFill, char* corStroke, double opacidade) {
    if (!svg || !svg->f) return;

    fprintf(svg->f, "\n\t\n");
    fprintf(svg->f, "\t<rect x=\"%.2f\" y=\"%.2f\" width=\"%.2f\" height=\"%.2f\" "
                    "style=\"fill:%s;stroke:%s;stroke-width:5;fill-opacity:%.2f\" />\n",
            x, y, w, h, corFill, corStroke, opacidade);
}

void svg_desenhar_ponto(SvgFile* svg, double x, double y, char* cor, char* texto) {
    if (svg == NULL || svg->f == NULL) {
        return;
    }

    fprintf(svg->f, "\n\t\n");
    
    fprintf(svg->f, "\t<circle cx=\"%.2f\" cy=\"%.2f\" r=\"6\" fill=\"%s\" stroke=\"white\" stroke-width=\"1\" />\n", 
            x, y, cor);
    
    if (texto != NULL) {
        fprintf(svg->f, "\t<text x=\"%.2f\" y=\"%.2f\" fill=\"black\" font-family=\"sans-serif\" font-size=\"10\" font-weight=\"bold\">%s</text>\n", 
                x + 8, y + 4, texto);
    }
}

void svg_finalizar(SvgFile* svg) {
    if (svg == NULL) {
        return;
    }

    if (svg->f != NULL) {
        fprintf(svg->f, "</svg>\n");
        fclose(svg->f);
    }
    
    free(svg);
}

