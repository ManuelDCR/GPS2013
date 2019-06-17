#include "vias.h"
#include "menu.h"
#include "mapa.h"

/******************************************************|
| Funcao inicializar campos da estrutura fronteiras    |
|                                                      |
| Recebe: estrutura fronteiras e dimensao              |
| Retorna: nada                                        |
|******************************************************/

void inicializa (FRONTEIRA fronteiras [], int dim)
{
  int i, j;

  for(i=0; i<dim; i++)
  {
    fronteiras[i].nome[0] = '\0';
    for (j=0; j<MAXP; j++)
    {                
      fronteiras[i].vp[j].x=0.0;
      fronteiras[i].vp[j].y=0.0;
    }
  }
}


/******************************************************|
| Funcao para ler os dados do ficheiro das fronteiras  |
|                                                      |
| Recebe: estrutura fronteiras e dimensao              |
| Retorna: nada                                        |
|******************************************************/

void le_dados_do_ficheiro(FRONTEIRA fronteiras[], int dim)
{
  char linha[DIML];
  int i=0, j=0;
  FILE *fp;
  float a=0, b=0;

/*Abrir ficheiro com as coordenadas das fronteiras dos distritos*/
  fp = fopen(F_FRONT, "r"); 
  if (fp == NULL) {
    printf("ERRO na abertura do ficheiro das fronteiras. \n");
    exit(0);
  }
  
  while ((fgets(linha, DIML, fp))!=0)
  { 
    if (linha[0] == 'F')   /*ler nome da fronteira*/
    {
      i++;                            
      strcpy(fronteiras[i-1].nome, linha);
      j=0;                       
    }
     else                 /*ler pontos da fronteira*/ 
    {
      sscanf(linha, "%f %f", &a,&b);
      fronteiras[i-1].vp[j].x=b;
      fronteiras[i-1].vp[j].y=a;
      j=j+1;  
    }  
  }

  converter_coordenadas_fronteiras(fronteiras, MAXF);
}


/******************************************************|
| Funcao para imprimir no ecra conjunto de fornteiras  |
|                                                      |
| Recebe: conjunto de fronteiras                       |
| Retorna: nada                                        |
|******************************************************/

void mostra_fronteiras (FRONTEIRA fronteiras[])
{
  int i, j;

  for(i=0; i<MAXF; i++) {
    printf("\nDistrito: %s", fronteiras[i].nome);
    for(j=0; j<MAXP; j++) {
      printf("\tLat: %f\tLon: %f\n", fronteiras[i].vp[j].x, fronteiras[i].vp[j].y);
    }
  }
}


/******************************************************|
| Funcao para converter coordenadas de fronteiras      |
|                                                      |
| Recebe: estrutura fronteiras e dimensao              |
| Retorna: nada                                        |
|******************************************************/

void converter_coordenadas_fronteiras(FRONTEIRA fronteiras[], int dim)
{ 
  int i, j;
  float a,b,m0,n0;

/*Conversao das coordenadas de referencia de graus para radianos*/
  a = ( llat * M_PI) / 180;
  b = ( llon * M_PI) / 180;

  for(i=0; i<dim; i++)
  {
    for (j=0; j<4; j++) {
      m0=(fronteiras[i].vp[j].y * 3.14) / 180; 
      n0=(fronteiras[i].vp[j].x * 3.14) / 180; 
      fronteiras[i].vp[j].x=(6371*cos( m0 )*tan( n0 - b ));
      fronteiras[i].vp[j].y=(6371*tan( m0 - a ));
    }
  }
}


/******************************************************|
| Funcao para converter coordenadas de pontos          |
|                                                      |
| Recebe: apontador para ponto                         |
| Retorna: nada                                        |
|******************************************************/

void converter_coordenadas_ponto(PONTO *p)
{
  p->y = R_TERRA*cos(((p->x)*M_PI/180))*tan(((p->y)-llon)*M_PI/180);
  p->x = R_TERRA*tan(((p->x)-llat)*M_PI/180);
}


/******************************************************|
| Funcao para desenhar no g2 (tudo)                    |
|                                                      |
| Recebe: dispositivo, apontadores para o topo da rota |
|   e das vias, centro g2 (x,y), ampliacao, virtual    |
|   device, fronteiras e arg da linha de comandos      |
| Retorna: nada                                        |
|******************************************************/

void desenha(int dev, ROTA *top, L_VIAS *topvia, float x0, float y0, float a, int d, FRONTEIRA fronteiras[], char strd[], int cor, float stra)
{
  ROTA *aux=NULL,*proximo=NULL,*contagem;
  int conta=0,conta1,conta2;
  char numero1[10],numero2[10];
  float x=-250,y=400;

  aux=top;
  contagem=top;
  while(contagem!=NULL){
    contagem=contagem->prox;
    conta++;
  }

  g2_set_line_width (dev,1.2);
    
  g2_set_coordinate_system(d, x0, y0, a, a);
  desenha_vias(dev, topvia);
  
  g2_set_coordinate_system(d, 550/2, 650/2, 1, 1);
  desenha_legenda(dev,d);

  g2_set_coordinate_system(d, x0, y0, a, a);
  desenha_fronteiras(d,dev,fronteiras,strd,cor,stra,x0,y0,a);

  conta1=conta;
  conta2=conta1-1;
  
  if (top!=NULL)
    while(aux->prox != NULL) {
      proximo=aux->prox; 

      g2_set_line_width (dev,3.5);
   
      g2_pen(dev,19);         
      g2_line(dev, aux->loc.p.y, aux->loc.p.x, proximo->loc.p.y, proximo->loc.p.x);
      g2_filled_circle(dev,   aux->loc.p.y+2, aux->loc.p.x, 2);
      g2_filled_circle(dev, proximo->loc.p.y+2, proximo->loc.p.x, 2);
      g2_set_coordinate_system(d, 550/2, 650/2, 1, 1);
      g2_string(dev, x+15, y, aux->loc.nome);
      g2_string(dev, x+15, y-16, proximo->loc.nome);
      sprintf(numero1,"%d",conta1);
      sprintf(numero2,"%d",conta2);
      g2_string(dev, x, y, numero1);
      g2_string(dev, x, y-16, numero2);
      g2_set_coordinate_system(d, x0, y0, a, a);
      g2_pen(dev,1);
      g2_string(dev, aux->loc.p.y, aux->loc.p.x, aux->loc.nome);
      g2_string(dev, proximo->loc.p.y, proximo->loc.p.x, proximo->loc.nome);

      aux=aux->prox;
      conta1--;
      conta2--;
      g2_flush(d);   
      y=y-16;    
    }
}


/******************************************************|
| Funcao para desenhar a legenda do mapa no g2         |
|                                                      |
| Recebe: dispositivo e virtual device                 |
| Retorna: nada                                        |
|******************************************************/

void desenha_legenda(int dev,int d)
{
  g2_set_coordinate_system(d, 550/2, 650/2, 1, 1);
  g2_pen(dev,3);    /*Distrito*/
  g2_filled_circle(dev, X_LEG, Y_LEG, 3);
  g2_string(dev, X_LEG+6, Y_LEG-4,"->Distrito");
  g2_pen(dev,22);   /*Concelho*/
  g2_filled_circle(dev, X_LEG, Y_LEG-20, 2.5);
  g2_string(dev,X_LEG+6, Y_LEG-24,"-> Concelho");
  g2_pen(dev,21);   /*Freguesia*/
  g2_filled_circle(dev, X_LEG, Y_LEG-40, 2);
  g2_string(dev,X_LEG+6, Y_LEG-44,"-> Freguesia");
  g2_pen(dev,9);   /*Aldeia*/
  g2_filled_circle(dev, X_LEG, Y_LEG-60, 1.5);
  g2_string(dev,X_LEG+6, Y_LEG-64,"-> Aldeia");
}


/******************************************************|
| Funcao para desenhar as fronteiras no g2             |
|                                                      |
| Recebe: dispositivo, virtual device, fronteiras, arg |
|   da linha de comandos, centro g2 (x,y) e ampliacao  |
| Retorna: nada                                        |
|******************************************************/

void desenha_fronteiras(int dev, int d, FRONTEIRA fronteiras[], char strd[], int cor, float stra,float x0,float y0,float zoom)
{
  int i,j, n=0;       /* usada para contar*/
  char distrito[100];
     
  sprintf(distrito,"(%s)",strd);
  g2_set_coordinate_system(d, x0, y0, zoom, zoom);
  
  for(i=0;i<MAXF;i++) {
    if(distrito[0]!='\0') {
      if(strstr(fronteiras[i].nome, distrito) != NULL ) {   /*Mecanismo de detecao de introducao de nomes de distritos incompletos*/     
        if((strstr(fronteiras[i].nome, distrito) == NULL ) && (n==0))
           printf("Nao introduziu um distrito valido.\n");
        n++;
      }
    }

    if(strstr(fronteiras[i].nome, distrito) != NULL )      /*Definicao das cores do distrito*/ 
      g2_pen(d, cor);
    else
      g2_pen(d, 1);
      
    for(j=0; j<3;j++) { 
      g2_line(d, fronteiras[i].vp[j].x, fronteiras[i].vp[j].y, fronteiras[i].vp[j+1].x, fronteiras[i].vp[j+1].y);
      g2_flush(d);
    }
  }

  g2_flush(d);
}


/******************************************************|
| Funcao para desenhar as vias no g2                   |
|                                                      |
| Recebe: dispositivo e apontador para o topo das vias |
| Retorna: nada                                        |
|******************************************************/

void desenha_vias(int dev, L_VIAS *top)
{
/*Percorrer a lista vias e desenhar as ligacoes e os respetivos circulos de cada ponto*/
  while(top!=NULL) {
     g2_pen(dev, 4);
     g2_line(dev, top->via.loc1.p.y, top->via.loc1.p.x, top->via.loc2.p.y, top->via.loc2.p.x);

     if(top->via.loc1.tipo=='D'){     g2_pen(dev,3);
       g2_filled_circle(dev, top->via.loc1.p.y, top->via.loc1.p.x, 3);}
     if(top->via.loc1.tipo=='C'){     g2_pen(dev,22);
       g2_filled_circle(dev, top->via.loc1.p.y, top->via.loc1.p.x, 2.5);}
     if(top->via.loc1.tipo=='F'){     g2_pen(dev,21);
       g2_filled_circle(dev, top->via.loc1.p.y, top->via.loc1.p.x, 2);}
     if(top->via.loc1.tipo=='A'){     g2_pen(dev,9);
       g2_filled_circle(dev, top->via.loc1.p.y, top->via.loc1.p.x, 1.5);}
     if(top->via.loc2.tipo=='D'){     g2_pen(dev,3);
       g2_filled_circle(dev, top->via.loc2.p.y, top->via.loc2.p.x, 3);}
     if(top->via.loc2.tipo=='C'){     g2_pen(dev,22);
       g2_filled_circle(dev, top->via.loc2.p.y, top->via.loc2.p.x, 2.5);}
     if(top->via.loc2.tipo=='F'){     g2_pen(dev,21);
       g2_filled_circle(dev, top->via.loc2.p.y, top->via.loc2.p.x, 2);}
     if(top->via.loc2.tipo=='A'){     g2_pen(dev,9);
       g2_filled_circle(dev, top->via.loc2.p.y, top->via.loc2.p.x, 1.5);}
     g2_pen(dev,1);
  /* g2_char(dev, top->via.loc1.p.y, top->via.loc1.p.x, top->via.loc1.tipo);
     g2_char(dev, top->via.loc2.p.y, top->via.loc2.p.x, top->via.loc2.tipo); */

     top=top->prox; /*avancar um elemento da lista*/
  }
}
