#ifndef QUADRA_H
#define QUADRA_H

typedef struct quadra Quadra;

Quadra* quadra_cria(char*, double, double, double, double, char* , char*, double);

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

#endif 