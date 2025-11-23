#ifndef SVG_H
#define SVG_H

#include "STreap.h"  
#include "digraph.h" 
#include "lista.h"   

typedef struct svgFile SvgFile;


SvgFile* svg_criar(char* diretorio, char* nomeGeo, char* nomeQry, STreap* arvore, Graph* vias);

void svg_desenhar_quadras(SvgFile* svg, STreap* arvore);
void svg_desenhar_regiao_catac(SvgFile* svg, double x, double y, double w, double h, char* corFill, char* corStroke, double opacidade);

void svg_desenhar_caminho(SvgFile* svg, Graph* vias, Lista* listaNos, char* cor);

void svg_desenhar_ponto(SvgFile* svg, double x, double y, char* cor, char* texto);


void svg_finalizar(SvgFile* svg);
#endif