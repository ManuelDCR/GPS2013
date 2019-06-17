#include "vias.h"
#include "menu.h"
#include "mapa.h"

/******************************************************|
| Funcao processar os argumentos da linha de comandos  |
|                                                      |
| Recebe: numero de arg, vetor com os arg, string de   |
|   distrito e de cor e endereco da ampliacao e cor    |
| Retorna: nada                                        |
|******************************************************/

void processa_argumentos(int argc, char *argv[], char dist[], char cor[], float *amp, int *ap_cor)
{

  int a; /*posicao do argumento na linha de comandos*/

  for(a=0; a < argc; a++){
    if(argv[a+1]!=NULL){
      if(strcmp("-a", argv[a]) == 0 )
        sscanf(argv[a+1],"%f",amp);

      if(strcmp("-c", argv[a]) == 0 )
        sscanf(argv[a+1],"%s",cor);

      if(strcmp("-d", argv[a]) == 0 )
        sscanf(argv[a+1],"%s",dist); 
    }
  }

  *ap_cor = escolhe_cor(cor);

}


/******************************************************|
| Funcao para acrescentar '(' e ')' ao nome            |
|                                                      |
| Recebe: 1 string (vetor de carateres)                |
| Retorna: nada                                        |
|******************************************************/

void ProcessaNome(char nome[])
{

  char aux[DIM1];
 
  sprintf(aux,"(%s)",nome);

  strcpy(nome,aux);
}


/******************************************************|
| Funcao para limpar "lixo" do buffer                  |
|                                                      |
| Recebe: nada                                         |
| Retorna: nada                                        |
|******************************************************/

void limpa_buffer()
{
  while(getchar()!='\n');  /*limpar o buffer*/
}


/******************************************************|
| Funcao descodificar a cor introduzida nos argumentos |
|                                                      |
| Recebe: string com o nome da cor                     |
| Retorna: inteiro que representa a cor no g2          |
|******************************************************/

int escolhe_cor(char strc[])
{
  int cor=100;
/*A cor e definida como 100 para no fim das condicoes podermos
  verificar se houvealteracao no valor cor
  de modo a sabermos se o utilizador introduziu uma cor valida*/

  if(strcmp("branco", strc) == 0 ){
    cor = 0;
    printf("A cor que escolheu pode tornar a visualizacao do mapa dificil. \n");}
  if(strcmp("preto", strc) == 0 ){
    cor = 1;
    printf("A cor que escolheu nao ira alterar a cor normal das fronteiras. \n");}
  if(strcmp("vermelho", strc) == 0 )
    cor = 19;
  if(strcmp("azul", strc) == 0 )
    cor = 3;
  if(strcmp("verde", strc) == 0 )
    cor = 4;
  if(strcmp("laranja", strc) == 0 )
    cor = 22;
  if(strcmp("amarelo", strc) == 0 )
    cor = 25;
  if(strcmp("castanho", strc) == 0 )
    cor = 10;
  if(strcmp("cinzento", strc) == 0 )
    cor = 14;
  if(strcmp("preto", strc) == 0 )
    cor = 1;
  if(strcmp("azul_escuro", strc) == 0 )
    cor = 2;
  if(strcmp("azul_claro", strc) == 0 )
    cor = 6;
  if(strcmp("azul_esverdeado", strc) == 0 )
    cor = 8;
  if(strcmp("Sporting", strc) == 0 )   /*Easter Egg*/
    cor = 7;
  if(strcmp("verde_claro", strc) == 0 )
    cor = 7;
  if(strcmp("roxo", strc) == 0 )
    cor = 11;
  if(strcmp("verde_lima", strc) == 0 )
    cor = 13;
  if(strcmp("rosa", strc) == 0 )
    cor = 20;
  if(strcmp("rosa_choque", strc) == 0 )
    cor = 20;
  if(strcmp("bege", strc) == 0 )
    cor = 26;
  if(strcmp("", strc) == 0)
    cor = 1;
  if(cor==100){      /*Forma de verificar se o utilizador introduziu uma cor invalida*/
   printf("\n\nA cor que escolheu nao esta disponivel, a cor sera a predefinida - preto\n\n");
   cor=1;}

  return cor;
}


/******************************************************|
| Funcao selecionar o que o utilizador pretende fazer  |
|                                                      |
| Recebe: apontadores para ampliacao, centro g2 (x,y), |
|   topo lista vias e para apontador topo rota         |
| Retorna: inteiro (1=continuar, 0=sair)               |
|******************************************************/

int Menu(float *ap_a, float *ap_x0, float *ap_y0, L_VIAS *lv_top, ROTA **ap_rota_topo)
{
  char m[DIM1];
  char opcao;

  m[1]='\0';
  printf("\nMenu:\n[1] Zoom Out\n[3] Zoom In\n[2] Baixo\n[4] Esquerda\n[6] Direita\n[8] Cima\n[7] Rota Direta\n[9] Rota com Destino Intermedio\n[0] Sair\nOpcao:");
  
  fgets(m,DIM1, stdin);
  opcao =m[0];
  if(m[1]!='\n')
    return 1;

  switch(opcao){ /*Atua conforme a opcao escolhida pelo utilizador*/
    case '4': /*Mover para a esquerda*/
      *ap_x0=*ap_x0-50;
      break;
    case '2': /*Mover para baixo*/
      *ap_y0=*ap_y0-50;
      break;
    case '6': /*Mover para a direita*/
      *ap_x0=*ap_x0+50;
      break;
    case '8': /*Mover para cima*/
      *ap_y0=*ap_y0+50;
      break;
    case '1':  /*Diminuir a ampliacao*/
      *ap_a=*ap_a*0.9;
      break;
    case '3': /*Aumentar a ampliacao*/
      *ap_a=*ap_a*1.1;
      break;
    case '7': /*Calcular Rota Direta*/
      rota_direta(lv_top, ap_rota_topo);
      break;
    case '9': /*Calcular Rota com Destino Intermedio*/
      rota_destino_intermedio(lv_top, ap_rota_topo);
      break;
    case '0': /*Sair*/
      return 0;
    default: /*Opcao introduzida é invalida*/
      printf(" --- > Opcao invalida, tente de novo.\n");
  }

  return 1;
}


/******************************************************|
| Funcao coordenar os passos de calculo de rota direta |
|                                                      |
| Recebe: apontador para topo lista vias               |
|   e para apontador para topo rota(ROTA**)            |
| Retorna: nada                                        |
|******************************************************/

void rota_direta(L_VIAS *lv_top, ROTA **ap_rota_topo)
{
  int existe=0;
  char partida[DIM1], destino[DIM1];

/*Apaga a rota desenhada anteriormente*/
  *ap_rota_topo = apagar_rota(*ap_rota_topo);

/*Pede ao utilizador o local de partida*/
  printf("Local de partida: "); 
  scanf("%s", partida); 
  existe = verifica_existencia_localidade(partida, lv_top);
  if(existe == 0) /*Nao existe "partida" na lista de vias*/
    return;

/*Pede ao utilizador o local de destino final*/
  printf("Local de destino: "); 
  scanf("%s", destino); 
  existe = verifica_existencia_localidade(destino, lv_top);
  if(existe == 0) /*Nao existe "destino" na lista de vias*/
    return;

  limpa_buffer();

/*Evoca a funcao para calcular a rota entre a partida e o destino (sem restricoes)*/
  *ap_rota_topo = calcula_rota(lv_top, partida, destino,"\0");

}


/******************************************************|
| Funcao coordenar calculo de rota destino intermedio  |
|                                                      |
| Recebe: apontador para topo lista vias               |
|   e para apontador para topo rota(ROTA**)            |
| Retorna: nada                                        |
|******************************************************/

void rota_destino_intermedio(L_VIAS *lv_top, ROTA **ap_rota_topo)
{
  int i=0, conversoes=0, pontos=0, existe =0;
  char partida[DIM1], destino[DIM1], **intermedio;

/*Apaga a rota desenhada anteriormente*/
  *ap_rota_topo = apagar_rota(*ap_rota_topo);

/*Pergunta ao utilizador qual o numero de destinos intermedios*/
  do{
    printf("Por quantos destinos intermedios deseja passar?\n"); conversoes = scanf("%d",&pontos);
    limpa_buffer();
  }while((conversoes!=1)||(pontos<0));
  if(pontos == 0) {   /*O utilizador nao quer destinos intermedios*/
    printf("Escolha a opcao Rota Direta\n");
    return;
  }

/*Cria no vetor "intermedio" espacos para guardar os enderecos dos nomes dos destinos intermedios*/
  intermedio = (char **) calloc(pontos, sizeof(char*));
  if(intermedio == (char **)NULL) {
    printf("ERRO_1 na alocacao de memoria - escolha um menor numero de passagens intermedias\n");
    return;
  }

/*Cria 1 vetor de carateres (DIM1) para cada endereco do vetor de enderecos "intermedio"*/
  for(i=0;i<pontos;i++) {
    intermedio[i] = (char *) calloc(DIM1, sizeof(char));
    if(intermedio[i] == (char *) NULL) {    /*verifica sucesso na alocacao de memoria*/
      while(i>0){    /*libertamos a memoria que ja foi alocada*/
        free(intermedio[i]);
        i--;
      }
      break;
    }
  }
  if (i != pontos) {    /*a alocacao de memoria falhou*/
    printf("ERRO_2 na alocacao de memoria - escolha um menor numero de passagens intermedias\n");
    return;
  }

/*Pede ao utilizador qual o local de partida*/
  printf("Local de partida: "); scanf("%s", partida); 
  existe = verifica_existencia_localidade(partida, lv_top);
  if(existe == 0) /*Nao existe "partida" na lista de vias*/
    return;

/*Pede ao utilizador qual o(s) local de destino intermedio(s)*/      
  for(i=0;i<pontos;i++) {
    printf("Local intermedio no. %d: ", i+1); scanf("%s", intermedio[i]);
    existe = verifica_existencia_localidade(intermedio[i], lv_top);
    if(existe == 0) /*Nao existe "intermedio[i]" na lista de vias*/
      break;
  }
  if(existe == 0) /*Nao existe UM dos destino intermedios na lista de vias*/
    return;

/*Pede ao utilizador qual o local de destino final*/
  printf("Local de destino: "); scanf("%s", destino); 
  existe = verifica_existencia_localidade(destino, lv_top);
  if(existe == 0) /*Nao existe "destino" na lista de vias*/
    return;

/*Verifica se o destino nao esta nas localidades intermedias*/
  for(i=0; i<pontos; i++)
    if( ( strcmp(intermedio[i],destino) == 0 )||strcmp(partida,destino)==0) {
      printf("Ponto intermedio ou partida nao podem coincidir com o destino final.\n");
      limpa_buffer();
      return;
    }

  limpa_buffer();

/*Evoca a funcao para calcular a rota entre a partida e o destino passando pelos destinos intermedios*/
   *ap_rota_topo= rota_passagens_intermedias(lv_top, partida, destino, intermedio, pontos);
}
