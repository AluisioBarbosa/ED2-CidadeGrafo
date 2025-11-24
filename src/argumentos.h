
/**
 * Esse modulo serve para gerenciar os argumentos passados na linha de comando.
 * Ele processa os argumentos, armazena-os em uma estrutura de dados apropriada 
 * e fornece funções para acessar esses argumentos conforme necessário.
 * 
*/
typedef struct dadosArgumentos DadosArgumentos;


/**
 * @brief Funcoes para criar, destruir e processar argumentos da linha de comando
 */

 /**
  * @brief Cria e inicializa a estrutura de dados para armazenar os argumentos
  * @return Ponteiro para a estrutura de dados inicializada
  */
DadosArgumentos* criarEstruturaArgumentos();

/**
 * @brief Destrói a estrutura de dados dos argumentos e libera os recursos alocados
 * @param args Ponteiro para a estrutura de dados dos argumentos a ser destruída
 */
void destruirArgumentos(DadosArgumentos*);

/**
 * @brief Processa os argumentos da linha de comando e os armazena na estrutura de dados
 * @param argc Número de argumentos da linha de comando
 * @param argv Array de strings contendo os argumentos da linha de comando
 * @return Ponteiro para a estrutura de dados contendo os argumentos processados
 */
DadosArgumentos* processarArgumentos(int, char*[]);

/**
 * @brief Funções para obter os caminhos dos arquivos e diretórios a partir da estrutura de argumentos
 */

 /**
  * @brief Retorna o caminho completo do arquivo .geo
  * @param args Ponteiro para a estrutura de dados dos argumentos
  * @return Ponteiro para a string contendo o caminho do arquivo .geo
  */
char* getCaminhoGeo(DadosArgumentos*);

/**
 * @brief Retorna o caminho completo do arquivo .qry
 * @param args Ponteiro para a estrutura de dados dos argumentos
 * @return Ponteiro para a string contendo o caminho do arquivo .qry
 */
char* getCaminhoQry(DadosArgumentos*);

/**
 * @brief Retorna o caminho completo do arquivo .via
 * @param args Ponteiro para a estrutura de dados dos argumentos
 * @return Ponteiro para a string contendo o caminho do arquivo .via
 */
char* getCaminhoVia(DadosArgumentos*);

/**
 * @brief Retorna o diretório de saída
 * @param args Ponteiro para a estrutura de dados dos argumentos
 * @return Ponteiro para a string contendo o diretório de saída
 */
char* getDIRsaida(DadosArgumentos*);
/**
 * @brief Imprime os argumentos armazenados na estrutura de dados
 * @param args Ponteiro para a estrutura de dados dos argumentos
 * usado para debugar
 */
void printArgumentos(DadosArgumentos*);