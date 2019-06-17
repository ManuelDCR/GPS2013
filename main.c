/* -------------------------------------------------------------------
  ||
  ||  Nº: 78679  NOME: Manuel Diogo Costa Rosa
  ||  Nº: 79157  NOME: Jose Carlos Franco Confraria
  ||
-------------------------------------------------------------------- */


#include "vias.h"
#include "menu.h"
#include "mapa.h"

int main (int argc, char *argv[])
{
  float stra=1.0,a=1,y0,x0;
  char strd[100]={'\0'}, strc[100]={'\0'};
  FRONTEIRA conjunto_fronteiras[MAXF];  /*Estrutura que contem as fronteiras*/
  L_VIAS *lv_top = NULL;  /*apontador para o topo da fila de vias*/
  ROTA *rota_topo=NULL;  /*apontador para o topo da pilha com a rota*/
  int m=1, cor,d,dev;

/*---Processa os argumentos introduzidos na linha de comandos---*/
  processa_argumentos(argc, argv, strd, strc, &stra, &cor);

/*---Inicializa estrutura das fronteiras---*/  
  inicializa(conjunto_fronteiras, MAXF);
  
/*---Le os dados do ficheiro das fronteiras e guarda no conjunto de fronteiras---*/ 
  le_dados_do_ficheiro(conjunto_fronteiras, MAXF);

/*---Le os dados do ficheiro das vias e insere-os numa fila de vias---*/
  lv_top = le_dados_vias(lv_top);
     
/*---Valores iniciais da origem do grafico---*/
  x0=stra*400/2;
  y0=stra*650/2;
   
/*---G2---*/
  dev = g2_open_X11(600*stra,750*stra); /* Tamanho da janela */

  d=g2_open_vd();    /* abrir virtual device */

  g2_clear(d); /*Limpa todo o conteudo do g2*/
 
  g2_attach(d, dev); /* anexar o virtual device com o physical device */

  g2_set_auto_flush(d,0); /* usado para atualizar */

/*---Apresenta o menu e executa as diferentes opcoes---*/
  while(m != 0){
    g2_clear(d);

  /*Funcao que desenha no g2*/
    desenha(dev,rota_topo, lv_top, x0, y0, a, d, conjunto_fronteiras, strd, cor, stra);

    m = Menu(&a, &x0, &y0, lv_top, &rota_topo);
  }

/*---Liberta a memoria antes de encerrar---*/
  lv_top = apagar_lista_vias(lv_top);
  rota_topo = apagar_rota(rota_topo);

  return 0;
}
