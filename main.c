#include <stdio.h>
#include "programa.h"
#define _POSIX_C_SOURCE 200809L


int main(int argc, char* argv[]){
    Programa* programa = criarPrograma(argc, argv);
    run(programa);
    destruirPrograma(programa);
    return 0;
}