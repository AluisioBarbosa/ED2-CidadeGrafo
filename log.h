#include <stdlib.h>

const char* horaAtualFormatada();

void                logInfo                 (const char*); // Função para mostrar informações e coloca-la em um arquivo log
void                logError                (const char*); // Função para mostrar erros e coloca-lo em um arquivo log
void                logToFile               (const char*); // Função que coloca um texto diretamente num arquivo log

FILE*                initLog                 (const char*); // Função para iniciar o log
void                closeLog                (); // Função que fecha o log