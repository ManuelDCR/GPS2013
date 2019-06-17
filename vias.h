#ifndef _VIAS_
#define _VIAS_

#define F_VIAS "vias_localidades.txt"
#define DIM1 100 /*para strings no geral*/
#define DIML 150 /*para linhas de ficheiros*/

typedef struct {
    float x; /*latitude*/
    float y; /*longitude*/
  } PONTO;

typedef struct {
    char nome[DIM1]; /*nome da localidade*/
    char tipo; /*tipo de localidade (D, C, V...)*/
    PONTO p; /*coordenadas onde se localiza*/
  } LOCALIDADE;

typedef struct {
    char nome[10]; /*nome da via (A1, A2...)*/
    LOCALIDADE loc1, loc2; /*Localidades ligadas pela via*/
    int estado; /*0=via ainda nao percorrida*/
  } VIA;

typedef struct lista_vias {
    VIA via;
    struct lista_vias *prox; /*Apontador para a via seguinte*/
  } L_VIAS;

typedef struct rota {
    LOCALIDADE loc;
    float d_partida, d_destino;
    struct rota *prox;
  } ROTA;

void distancia(PONTO *a, PONTO *b, float *d);
L_VIAS* le_dados_vias(L_VIAS* top);
L_VIAS* insere_na_lista_vias(VIA novo, L_VIAS *top);
L_VIAS* remove_via(L_VIAS* top, L_VIAS* rem);
L_VIAS* apagar_lista_vias(L_VIAS* top);
L_VIAS* procura_na_lista_vias(char nome[], L_VIAS *top);
void mostra_lista_vias(L_VIAS *top);
void mostra_localidade(LOCALIDADE loc);
ROTA* calcula_rota(L_VIAS *lv_top, char partida[], char destino[],char proibido[]);
ROTA* insere_localidade_na_rota(ROTA* top, LOCALIDADE loc, float d_partida, float d_destino);
ROTA* insere_campo_na_rota(ROTA* top);
ROTA* apagar_rota(ROTA *top);
ROTA* retira_ultima_localidade_da_rota(ROTA* top);
int compara_dist(float d_destino, float d_destino_nova);
void mostra_rota(ROTA *top);
void vias_por_explorar(L_VIAS *top,char proibido[]);
ROTA* rota_passagens_intermedias(L_VIAS *listavias_topo,  char partida[], char destino[], char **intermedio, int pontos);
ROTA* une_rotas(ROTA* r1, ROTA* r2);
int verifica_existencia_localidade(char nome[],L_VIAS *top);

#endif /*caso _VIAS_ ja tenha sido defenido*/
