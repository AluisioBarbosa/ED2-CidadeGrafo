#ifndef QUADRA_H
#define QUADRA_H
#include <stdbool.h>



/**
 * Esse modulo serve para representar uma quadra da cidade
 * podemos manipular suas propriedades como cor, tamanho, posicao, etc
 */

/**
 * @brief Estrutura opaca que representa uma quadra
 * essa estrutura armazena informações como CEP, coordenadas (x, y), largura, altura, 
 * cores de preenchimento e borda, entre outros atributos relevantes.
 */
typedef struct quadra Quadra;


// Construtor
Quadra* quadra_cria(char*, double, double, double, double, char* , char*, double);

// Getters e setters
char* quadra_get_cep(Quadra* );
double quadra_get_x(Quadra* );
double quadra_get_y(Quadra* );
double quadra_get_largura(Quadra* );
double quadra_get_altura(Quadra*);
char* quadra_get_cor_preenchimento(Quadra* );
char* quadra_get_cor_borda(Quadra*);
double quadra_get_espessura(Quadra*);

void quadra_set_cores(Quadra*, char*, char*, double);
void quadra_destroi(Quadra*);

bool quadra_is_ativa(Quadra* q);

void quadra_set_ativa(Quadra* q, bool status);

#endif 