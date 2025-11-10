#include <stdio.h>
#include "programa.h"


int main(int argc, char* argv[]){
    Programa* programa = criarPrograma(argc, argv);
    run(programa);





    destruirPrograma(programa);
    return 0;
}