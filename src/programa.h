
/**
 * Estrutura opaca do programa
 * guarda referencia para quase todas as partes importantes do programa em si
 * esse modulo é importante para manter o controle do programa como um todo e
 * facilitar a passagem de parametros entre funcoes. Também é importante para nao deixar o main poluído e com variaveis globais
 * Responsavel por criar, rodar e destruir o programa.
 */
typedef struct programa Programa;

/**
 * @brief Funcoes para criar, rodar e destruir o programa
 * Essas funcoes sao responsaveis por inicializar a estrutura do programa, 
 * executar o fluxo principal do programa e liberar os recursos alocados.
 */

 /**
  * @brief Cria e inicializa a estrutura do programa
  * @param argc Numero de argumentos da linha de comando
  * @param argv Vetor de argumentos da linha de comando
  * @return Ponteiro para a estrutura do programa inicializada
  */
Programa* criarPrograma(int, char*[]);

/**
 * @brief Executa o fluxo principal do programa
 * @param programa Ponteiro para a estrutura do programa
 */
void run(Programa*);

/**
 * @brief Destrói a estrutura do programa e libera os recursos alocados
 * @param programa Ponteiro para a estrutura do programa a ser destruída
 */
void destruirPrograma(Programa*);