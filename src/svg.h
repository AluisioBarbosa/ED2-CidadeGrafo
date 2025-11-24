#ifndef SVG_H
#define SVG_H

#include "STreap.h"  
#include "digraph.h" 
#include "lista.h"   

/*
* Esse modulo serve para criar o arquivo svg e desenhar
* seus componentes graficos, mais especificamente a cidade
* e toda sua malha viaria/animacoes e caminhos
*
*/




/**
* Estrutura opaca que guarda a referencia para o FILE do svg
* e o valor do topo da cidade para que se faça as linhas mostrando destino e inicio
*
*
*/
typedef struct svgFile SvgFile;


/**
 * @brief Cria o arquivo svg e retorna um ponteiro para a estrutura SvgFile.
 * @param diretorio Diretório onde o arquivo será salvo.
 * @param nomeGeo Nome do arquivo .geo.
 * @param nomeQry Nome do arquivo .qry (pode ser NULL).
 * @param arvore Árvore de quadras.
 * @param vias Grafo das vias.
 * @return Ponteiro para a estrutura SvgFile criada, ou NULL em caso de erro.
 */


SvgFile* svg_criar(char* diretorio, char* nomeGeo, char* nomeQry, STreap* arvore, Graph* vias);

/**
 * @brief Desenha as quadras da cidade
 * @param svg Ponteiro para a estrutura svgfile
 * @param arvore Árvore de quadras
 * @return não retorna nada
 */
void svg_desenhar_quadras(SvgFile* svg, STreap* arvore);

/**
 * @brief Desenha a região de cataclisma no arquivo SVG.
 * @param svg Ponteiro para a estrutura SvgFile.
 * @param x Coordenada x do canto superior esquerdo da região.
 * @param y Coordenada y do canto superior esquerdo da região.
 * @param w Largura da região.
 * @param h Altura da região.
 * @param corFill Cor de preenchimento da região.
 * @param corStroke Cor da borda da região.
 * @param opacidade Opacidade da região.
 * @return não retorna nada
 */
void svg_desenhar_regiao_catac(SvgFile* svg, double x, double y, double w, double h, char* corFill, char* corStroke, double opacidade);


/**
 * @brief Desenha um caminho no arquivo SVG.
 * @param svg Ponteiro para a estrutura SvgFile.
 * @param vias Grafo das vias.
 * @param listaNos Lista de nós que compõem o caminho.
 * @param cor Cor do caminho.
 * @param idUnico Identificador único para o caminho.
 * @return não retorna nada
 */
void svg_desenhar_caminho(SvgFile* svg, Graph* vias, Lista* listaNos, char* cor, char* idUnico);


/**
 * @brief Desenha um ponto no arquivo SVG.
 * @param svg Ponteiro para a estrutura SvgFile.
 * @param x Coordenada x do ponto.
 * @param y Coordenada y do ponto.
 * @param cor Cor do ponto.
 * @param texto Texto associado ao ponto.
 * @return não retorna nada
 */
void svg_desenhar_ponto(SvgFile* svg, double x, double y, char* cor, char* texto);


/**
 * @brief Desenha o grafo das vias no arquivo SVG.
 * @param svg Ponteiro para a estrutura SvgFile.
 * @param vias Grafo das vias.
 * @return não retorna nada
 */
void svg_desenhar_grafo(SvgFile* svg, Graph* vias);

/**
 * @brief Desenha um marcador vertical no arquivo SVG.
 * @param svg Ponteiro para a estrutura SvgFile.
 * @param x Coordenada x do marcador.
 * @param y Coordenada y do marcador.
 * @param cor Cor do marcador.
 * @param texto Texto associado ao marcador.
 * @return não retorna nada
 */
void svg_desenhar_marcador_vertical(SvgFile* svg, double x, double y, char* cor, char* texto);

/**
 * @brief Finaliza o arquivo SVG, escrevendo o fechamento da tag e liberando a memória.
 * @param svg Ponteiro para a estrutura SvgFile a ser finalizada.
 */
void svg_finalizar(SvgFile* svg);
#endif