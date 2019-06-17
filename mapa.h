#ifndef _MAPA_
#define _MAPA_

#define MAXF 53
#define MAXP 4
#define llon -9.136333 
#define llat 38.716583
#define R_TERRA 6371
#define M_PI 3.14159265358979323846
#define F_FRONT "fronteiras_distritos.txt"
#define X_LEG 225
#define Y_LEG 400

typedef struct fronteira
  {
    char nome[DIM1];
    PONTO vp[MAXP];
  } FRONTEIRA;

void inicializa (FRONTEIRA fronteiras [], int dim);
void le_dados_do_ficheiro (FRONTEIRA fronteiras[], int dim);
void mostra_fronteiras (FRONTEIRA fronteiras[]);
void converter_coordenadas_fronteiras(FRONTEIRA fronteiras[], int dim);
void converter_coordenadas_ponto(PONTO *p);
void desenha(int dev, ROTA *top,L_VIAS *topvia,float x0,float y0,float a,int d, FRONTEIRA fronteiras[], char strd[], int cor, float stra);
void desenha_legenda(int dev,int d);
void desenha_fronteiras(int dev, int d, FRONTEIRA fronteiras[], char distrito[], int cor, float stra,float x0,float y0,float zoom);
void desenha_vias(int dev, L_VIAS *top);

#endif /*caso _MAPA_ ja tenha sido defenido*/
