#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h> 
#include "processarQry.h"
#include "svg.h"
#include "quadra.h"
#include "digraph.h"
#include "lista.h"
#include "hashTable.h"
#include "djikstra.h"
#include "info.h" 


// --- FUNÇÕES AUXILIARES DE GEOMETRIA ---

static int pontoInterno(double x, double y, double rx, double ry, double rw, double rh) {
    if (x >= rx && x <= rx + rw) {
        if (y >= ry && y <= ry + rh) {
            return 1;
        }
    }
    return 0;
}

// Agora recebe HashTable* (ponteiro)
static int resolverEndereco(HashTable* tQuadras, char* cep, char* face, double num, double* x, double* y) {
    // buscarHashTable espera HashTable* ? Depende da sua lib.
    // Se sua lib espera HashTable (struct), usamos *tQuadras. Se espera ponteiro, usamos tQuadras.
    // Pelo padrão C, geralmente libs de hash recebem ponteiro.
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

// Agora recebe Graph* (ponteiro)
static int encontrarNoMaisProximo(Graph* vias, double px, double py) {
    if (vias == NULL) return -1;
    
    // Passamos o ponteiro direto, pois getMaxNodes deve esperar Graph*
    int maxV = getMaxNodes(vias);
    
    int melhorNo = -1;
    double menorDistQ = DBL_MAX;

    for (int i = 0; i < maxV; i++) {
        int idNode = i;
        // Passamos o ponteiro vias
        InfoVertice* info = (InfoVertice*) getNodeInfo(vias, &idNode);
        
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

// --- CALLBACKS PARA DIJKSTRA ---

static double pesoDistancia(Graph *g, Edge *e) {
    // g e e já são ponteiros aqui
    InfoAresta* info = (InfoAresta*) getEdgeInfo(g, e);
    
    if (info == NULL) return -1.0;
    if (!isArestaAtiva(info)) return -1.0;
    
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

// --- TRATADORES DE COMANDO (Tudo recebendo ponteiros) ---

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
        fprintf(txt, "@o?: Endereco %s localizado em (%.2f, %.2f).\n", cep, x, y);
        if (svg != NULL) {
            svg_desenhar_ponto(svg, x, y, "blue", cep);
        }
    } else {
        fprintf(txt, "@o?: Endereco %s invalido ou nao encontrado.\n", cep);
    }
}

static void cmd_blq(Graph* vias, HashTable* idBloqueios, char* nome, double x, double y, double w, double h, FILE* txt) {
    Lista* arestasTotais = criaLista();
    
    // Passamos ponteiro vias
    getEdges(vias, arestasTotais);

    Lista* bloqueadas = criaLista(); // Lista para guardar as arestas afetadas

    while (!listaVazia(arestasTotais)) {
        // A lista guarda ponteiros para Edge (que é void* ou struct edge*)
        // Dependendo da implementação da sua lista e getEdges, pode precisar de cast.
        Edge* e = (Edge*) removerInicio(arestasTotais);
        
        int u = getFromNode(vias, e);
        int v = getToNode(vias, e);
        
        // getNodeInfo retorna void*, cast para InfoVertice*
        InfoVertice* infoU = (InfoVertice*) getNodeInfo(vias, &u);
        InfoVertice* infoV = (InfoVertice*) getNodeInfo(vias, &v);
        
        if (infoU && infoV) {
            double ux = getXVertice(infoU); double uy = getYVertice(infoU);
            double vx = getXVertice(infoV); double vy = getYVertice(infoV);
            
            // Se origem OU destino estiverem no retângulo, bloqueia
            if (pontoInterno(ux, uy, x, y, w, h) || pontoInterno(vx, vy, x, y, w, h)) {
                
                // getEdgeInfo retorna void* (Info)
                void* infoAresta = getEdgeInfo(vias, e);
                
                if (infoAresta) {
                    // --- CORREÇÃO AQUI: Usando Setter ---
                    setArestaAtiva(infoAresta, false); // Bloqueia (false)
                    
                    inserirFim(bloqueadas, (void*)e); // Guarda a aresta na lista de bloqueios
                }
            }
        }
        // Não damos free(e) pois pertence ao grafo
    }
    liberaLista(arestasTotais);

    // Salva na hash
    inserirHashTable(idBloqueios, nome, (void*)bloqueadas);
    
    fprintf(txt, "blq: Bloqueio '%s' aplicado na regiao (%.1f, %.1f).\n", nome, x, y);
}

static void cmd_rbl(Graph* vias, HashTable* idBloqueios, char* nome, FILE* txt) {
    // Recupera a lista de arestas afetadas por este bloqueio
    Lista* listaAfetada = (Lista*) buscarHashTable(idBloqueios, nome);
    
    if (listaAfetada != NULL) {
        while (!listaVazia(listaAfetada)) {
            Edge* e = (Edge*) removerInicio(listaAfetada);
            
            // Recupera a info da aresta
            void* infoAresta = getEdgeInfo(vias, e);
            
            if (infoAresta) {
                // --- CORREÇÃO AQUI: Usando Setter ---
                setArestaAtiva(infoAresta, true); // Desbloqueia (true)
            }
        }
        // Libera a lista auxiliar
        liberaLista(listaAfetada);
        
        // Nota: Idealmente removeria da Hash também, mas se não tiver função remove,
        // o ponteiro fica lá (agora inválido ou vazio).
        
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

// Implementação completa do comando CATAC
static void cmd_catac(STreap* arvore, Graph* vias, HashTable* idQuadras, HashTable* idVertices,
                      double x, double y, double w, double h, FILE* txt, SvgFile* svg) {
    
    fprintf(txt, "catac: Processando regiao x=%.2f y=%.2f w=%.2f h=%.2f\n", x, y, w, h);

    // --- 1. Desenhar a região no SVG ---
    if (svg) {
        svg_desenhar_regiao_catac(svg, x, y, w, h, "#AB37C8", "#AA0044", 0.5);
    }

    // --- 2. Remover QUADRAS ---
    // A. Buscar quadras na região usando a STreap
    Lista* nosEncontrados = criaLista();
    
    // getStreapNodeRegiaoSTrp espera (STreap*, x, y, w, h, Lista*)
    // Ela retorna os StreapNode*.
    getStreapNodeRegiaoSTrp(arvore, x, y, w, h, nosEncontrados);

    while (!listaVazia(nosEncontrados)) {
        // O nó da árvore contém a Quadra na info
        // Dependendo da sua STreap, o retorno na lista pode ser o Nó ou a Info direta.
        // Assumindo que retorna StreapNode* conforme seu STreap.c original:
        void* noArv = removerInicio(nosEncontrados); 
        
        // Precisamos extrair a Quadra do nó da árvore
        Quadra* q = (Quadra*) *getInfoSTrp(arvore, noArv); // getInfoSTrp retorna Info*
        
        if (q) {
            // Verifica geometria exata (se está totalmente contida)
            if (retanguloDentroRetangulo(quadra_get_x(q), quadra_get_y(q), 
                                         quadra_get_largura(q), quadra_get_altura(q),
                                         x, y, w, h)) {
                
                // LOG TXT
                char* cep = quadra_get_cep(q);
                fprintf(txt, "  - Quadra Removida: CEP=%s\n", cep);

                // REMOVE DA HASH
                // removerHashTable(idQuadras, cep); // Se sua hash tiver remover

                // REMOVE DA ARVORE
                // deleteStreapNodeSTrp retorna a Info removida
                // Note: Isso remove o nó da árvore.
                // Cuidado para não invalidar o ponteiro 'q' antes de usar o cep.
                
                // Como deleteStreapNodeSTrp pode alterar a estrutura da árvore, 
                // iterar e deletar ao mesmo tempo é perigoso se não for feito com cuidado.
                // Mas aqui pegamos todos os nós numa lista separada antes, então é seguro.
                
                Info* infoRemovida = deleteStreapNodeSTrp(arvore, noArv);
                
                // Libera a memória da Quadra
                if (infoRemovida) {
                    quadra_destroi((Quadra*) *infoRemovida);
                }
            }
        }
    }
    liberaLista(nosEncontrados);


    // --- 3. Remover VÉRTICES e ARESTAS do Grafo ---
    // Como não temos árvore espacial para o grafo, precisamos iterar todos os vértices
    // (Solução O(V), aceitável para V < 100.000)
    
    int maxV = getMaxNodes(vias);
    // Lista de IDs para remover depois (para não alterar o grafo enquanto itera índices se fosse lista ligada)
    // Como é vetor estático com flag ativo, podemos remover direto.
    
    for (int i = 0; i < maxV; i++) {
        int idNode = i;
        // Verifica se nó existe
        InfoVertice* iv = (InfoVertice*) getNodeInfo(vias, &idNode);
        
        if (iv != NULL) {
            double vx = getXVertice(iv);
            double vy = getYVertice(iv);

            // Verifica se ponto está dentro do retângulo
            if (pontoInterno(vx, vy, x, y, w, h)) {
                // LOG TXT
                fprintf(txt, "  - Vertice Removido: ID=%s (%.2f, %.2f)\n", getIdVertice(iv), vx, vy);

                // REMOVE DA HASH DE VÉRTICES
                // removerHashTable(idVertices, getIdVertice(iv));

                // REMOVE DO GRAFO (Nó e Arestas Incidentes)
                removeNodeAndEdges(vias, idNode);
                
                // Libera memória da Info do Vértice
                destroiInfoVertice(iv);
                // O grafo deve setar o ponteiro de info para NULL internamente ou nós fazemos:
                setNodeInfo(vias, idNode, NULL);
            }
        }
    }
}


static void cmd_path(Graph* vias, HashTable* idQuadras, 
                     char* cO, char* fO, double nO, 
                     char* cD, char* fD, double nD,
                     char* cor1, char* cor2, FILE* txt, SvgFile* svg) {
    
    double xO, yO, xD, yD;
    
    int resO = resolverEndereco(idQuadras, cO, fO, nO, &xO, &yO);
    int resD = resolverEndereco(idQuadras, cD, fD, nD, &xD, &yD);

    if (!resO || !resD) {
        fprintf(txt, "p?: Enderecos de origem ou destino invalidos.\n");
        return;
    }

    int noO = encontrarNoMaisProximo(vias, xO, yO);
    int noD = encontrarNoMaisProximo(vias, xD, yD);

    if (noO == -1 || noD == -1) {
        fprintf(txt, "p?: Nao foi possivel encontrar nos proximos aos enderecos.\n");
        return;
    }

    if (svg) {
        svg_desenhar_ponto(svg, xO, yO, "green", "O");
        svg_desenhar_ponto(svg, xD, yD, "red", "D");
    }

    // Dijkstra Distância
    // vias já é Graph*, então passamos direto
    Caminho* cDist = dijkstra(vias, noO, noD, pesoDistancia);
    if (cDist) {
        double custo = caminho_get_custo(cDist);
        fprintf(txt, "p? (Distancia): %.2f metros.\n", custo);
        
        if (svg) {
            // Assumindo que svg_desenhar_caminho foi atualizada para receber Graph*
            svg_desenhar_caminho(svg, vias, caminho_get_lista(cDist), cor1);
        }
        destruirCaminho(cDist);
    } else {
        fprintf(txt, "p? (Distancia): Caminho nao encontrado.\n");
    }

    // Dijkstra Tempo
    Caminho* cTempo = dijkstra(vias, noO, noD, pesoTempo);
    if (cTempo) {
        double custo = caminho_get_custo(cTempo);
        fprintf(txt, "p? (Tempo): %.2f segundos.\n", custo);
        
        if (svg) {
            svg_desenhar_caminho(svg, vias, caminho_get_lista(cTempo), cor2);
        }
        destruirCaminho(cTempo);
    } else {
        fprintf(txt, "p? (Tempo): Caminho nao encontrado.\n");
    }
}


// --- FUNÇÃO PRINCIPAL ---

// Assinatura atualizada com ponteiros
void processarQry(char* pathQry, char* dirSaida, char* nomeBaseGeo, char* nomeBaseQry,
                         HashTable* idQuadras, Graph* vias, HashTable* idBloqueios, STreap* arvoreQuadras, HashTable* idVertices) {
    
    if (pathQry == NULL) {
        return;
    }

    // Monta nome do arquivo TXT
    char pathTxt[512];
    if (dirSaida[strlen(dirSaida)-1] == '/') {
        sprintf(pathTxt, "%s%s-%s.txt", dirSaida, nomeBaseGeo, nomeBaseQry);
    } else {
        sprintf(pathTxt, "%s/%s-%s.txt", dirSaida, nomeBaseGeo, nomeBaseQry);
    }

    FILE* fqry = fopen(pathQry, "r");
    FILE* ftxt = fopen(pathTxt, "w");

    if (fqry == NULL) {
        printf("Erro: Falha ao abrir .qry: %s\n", pathQry);
        return;
    }

    // Cria SVG com a função que concatena nomes
    SvgFile* svg = svg_criar(dirSaida, nomeBaseGeo, nomeBaseQry, arvoreQuadras, vias);
    // Desenha mapa base (Quadras)
    if (svg != NULL && arvoreQuadras != NULL) {
        svg_desenhar_quadras(svg, arvoreQuadras); // Passa conteúdo se a função espera struct, ou ponteiro se espera ponteiro.
        // ATENÇÃO: Seu svg_desenhar_quadras deve ser atualizado para receber STreap* também!
        // Se svg.h tiver void svg_desenhar_quadras(SvgFile* s, STreap* t), então remove o *
        // Vou assumir que você vai atualizar o svg.h para usar STreap*
        // svg_desenhar_quadras(svg, arvoreQuadras); 
    }

    char cmd[10];
    char s1[256], s2[256], s3[256], s4[256], s5[256], s6[256];
    double d1, d2, d3, d4;

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
            fscanf(fqry, "%s %s %lf %s %s %lf %s %s", 
                   s1, s2, &d1, s3, s4, &d2, s5, s6);
            cmd_path(vias, idQuadras, s1, s2, d1, s3, s4, d2, s5, s6, ftxt, svg);
        }
        else if (strcmp(cmd, "catac") == 0) {
            fscanf(fqry, "%lf %lf %lf %lf", &d1, &d2, &d3, &d4);
            cmd_catac(arvoreQuadras, vias, idQuadras, idVertices, d1, d2, d3, d4, ftxt, svg);
        }
    }

    if (svg != NULL) {
        svg_finalizar(svg);
    }
    
    if (ftxt != NULL) {
        fclose(ftxt);
    }
    
    fclose(fqry);
}