typedef struct dadosArgumentos DadosArgumentos;


DadosArgumentos* criarEstruturaArgumentos();
void destruirArgumentos(DadosArgumentos*);
DadosArgumentos* processarArgumentos(int, char*[]);
char* getCaminhoGeo(DadosArgumentos*);
char* getCaminhoQry(DadosArgumentos*);
char* getDIRsaida(DadosArgumentos*);