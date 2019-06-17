#include "vias.h"
#include "menu.h"
#include "mapa.h"

/******************************************************|
| Funcao para calcular distancia entre 2 pontos        |
|                                                      |
| Recebe: 2 apontadores PONTO e 1 apontador float      |
| Retorna: nada                                        |
|******************************************************/

void distancia(PONTO *a, PONTO *b, float *d)
{
  float x, y, aux;

  x = a->x - b->x;
  y = a->y - b->y;
  aux = pow(x,2)+pow(y,2);

  *d = sqrt(aux);
}


/******************************************************|
| Funcao para ler os dados do ficheiro das vias        |
|                                                      |
| Recebe: 1 apontador L_VIAS para topo da lista vazia  |
| Retorna: 1 apontador L_VIAS para novo topo da lista  |
|******************************************************/

L_VIAS* le_dados_vias(L_VIAS* top)
{
  FILE *fp;
  char linha[DIML];
  VIA novo;
  int controlo = 0, l=0; /*Usado para verificar o sucesso do scanf*/

  novo.estado = 0; /*As vias sao todas inicializadas com o estado "por explorar"*/

/*---Abrir ficheiro das vias (F_VIAS)---*/
  fp = fopen(F_VIAS, "r");
  if(fp == NULL) {
    printf("ERRO na abertura do ficheiro das vias.\n");
    exit(1);
  }

/*---Copiar dados do ficheiro para uma estrutura com todas as vias---*/
  while ((fgets(linha, DIML, fp)) != 0) {
    l++; /*Guarda o indice linha na qual estamos a trabalhar*/

    controlo = sscanf(linha, "%c %s %f %f %s %c %s %f %f",&novo.loc1.tipo, novo.loc1.nome, &novo.loc1.p.x, &novo.loc1.p.y, novo.nome, &novo.loc2.tipo, novo.loc2.nome, &novo.loc2.p.x, &novo.loc2.p.y); /*Le a info da localidade 1, nome da via e localidade 2*/

    if(controlo!=1) {   /*Ignoramos linhas totalmente em branco*/
      if(controlo!=9) {
        printf("Existe um erro no ficheiro de vias (linha %d)\nTera de corrigir o erro antes de continuar...\n", l);
        exit(1);
      }
      
      converter_coordenadas_ponto(&novo.loc1.p);
      converter_coordenadas_ponto(&novo.loc2.p);

      top = insere_na_lista_vias(novo, top);
    }
  }
  
/*---Fechar o ficheiro das vias---*/
  fclose(fp);

  return top; /*O topo da lista pode ter sido modificado*/
}


/******************************************************|
| Funcao para inserir dados na lista de vias           |
|                                                      |
| Recebe: 1 apontador L_VIAS e 1 VIA                   |
| Retorna: 1 apontador L_VIAS para novo topo da lista  |
|******************************************************/

L_VIAS* insere_na_lista_vias(VIA novo, L_VIAS *top)
{
  L_VIAS *new, *step;

  new = (L_VIAS *) calloc (1, sizeof (L_VIAS) );

  new->via = novo; /*Copiamos a info da nova via para o campo da lista alusivo a via*/
  new->via.estado = 0; /*Inicializamos o estado da nova via com 0 (por explorar)*/
  new->prox = NULL; /*Colocamos no campo da lista alusiva a proxima entrada o endereco de NULL para nao apontar para nada*/

  if(top == NULL) /*A lista esta vazia*/
    top = new;

  else {
    for( step = top; step->prox != NULL; step = step->prox ); /*Percorre a lista ate encontrar a ultima entrada*/
    step->prox = new; /*A lista fica com mais uma entrada. A ultima entrada passa agora apontar para a nova*/
  }

  return top;
}


/******************************************************|
| Funcao para remover uma via da lista de vias         |
|                                                      |
| Recebe: 1 apontador L_VIAS para o topo e 1 apontador |
|   para a entrada a remover                           |
| Retorna: 1 apontador L_VIAS para novo topo da lista  |
|******************************************************/

L_VIAS* remove_via(L_VIAS* top, L_VIAS* rem)
{
  L_VIAS *aux=top;

  if(top == rem)
    top = top->prox;

  else {
    while( (aux!=NULL) && (aux->prox!=rem) )
      aux = aux->prox;
    if(aux!=NULL)
      aux->prox = rem->prox;
  }

  free(rem);

  return top;    
}


/******************************************************|
| Funcao para apagar toda a lista de vias              |
|                                                      |
| Recebe: 1 apontador L_VIAS para o topo               |
| Retorna: 1 apontador L_VIAS para novo topo da lista  |
|******************************************************/

L_VIAS* apagar_lista_vias(L_VIAS* top)
{

  while (top!=NULL)
    top = remove_via(top, top);

  return top;
}


/******************************************************|
| Funcao para procurar dados na lista de vias          |
|                                                      |
| Recebe: Apontador para o topo da lista de vias,      |
|   nome da localidade a procurar                      |
| Retorna: Apontador para a 1ª via com a localidade    |
|******************************************************/

L_VIAS* procura_na_lista_vias(char nome[], L_VIAS *top)
{

  if (top == NULL) /*Lista vazia*/
    return NULL;

  do{
    if( (strcmp(nome, top->via.loc1.nome) == 0) || (strcmp(nome, top->via.loc2.nome) == 0) )
      return top;

    top = top->prox;

  } while (top != NULL);

  return NULL; /*Nao ha nenhuma entrada com esse nome na lista*/

}


/******************************************************|
| Funcao para imprimir no ecra a lista de vias         |
|                                                      |
| Recebe: 1 apontador L_VIAS para o topo da lista      |
| Retorna: nada                                        |
|******************************************************/

void mostra_lista_vias(L_VIAS *top)
{
  int conta = 1;

  if (top == NULL) /*A lista esta vazia*/
  {
    printf("A lista esta vazia.\n");
    return;
  }

  while(top != NULL) /*Ate se chegar ao fim da lista*/
  {
    printf("\t\t---- Estrada: %s _ %d (%d) ----\n", top->via.nome, top->via.estado, conta++);
    mostra_localidade(top->via.loc1); /*info localidade 1*/
    mostra_localidade(top->via.loc2); /*info localidade 2*/
    putchar('\n');

    top = top->prox;
  }
}

 
/******************************************************|
| Funcao para imprimir no ecra info de 1 localidade    |
|                                                      |
| Recebe: 1 LOCALIDADE                                 |
| Retorna: nada                                        |
|******************************************************/

void mostra_localidade(LOCALIDADE loc)
{
    printf("Localidade (tipo): %s (%c)\n", loc.nome, loc.tipo); /*info localidade*/
    printf("\tCoordenadas: %f | %f\n", loc.p.x, loc.p.y);
}


/******************************************************|
| Funcao para calcular a rota partida-destino          |
|                                                      |
| Recebe: 1 apontador para ROTA vazia,                 |
|   3 strings (nome de loc: partida, destino, proibido)|
| Retorna: apontador para o topo da ROTA preenchida    |
|******************************************************/

ROTA* calcula_rota(L_VIAS *lv_top, char partida[], char destino[],char proibido[])
{
  ROTA *r_top=NULL;
  L_VIAS *step=NULL, *aux=NULL, *melhor=NULL;
  LOCALIDADE dest, part, atual;
  float d_dest=0.0, d_part=0.0, d_aux=0, d_pd=0; /*Distancias ao destino e a partida, variavel auxiliar e distancia partida-destino*/
  int teste=0; /*usado para guardar o resultado da funcao compara_dist*/
  int estado=0; /*usado para verificar se uma via ja foi explorada*/

  vias_por_explorar(lv_top,proibido); /*Todas as vias devem estar por explorar menos as que ligam ao destino proibido*/

  aux = procura_na_lista_vias(destino, lv_top); /*procurar destino na lista de vias*/
  if(aux == NULL) {
    printf("Nao ha nenhuma entrada chamada %s no ficheiro de vias\n", destino);
    exit(1);
  }
  if(strcmp(destino, aux->via.loc1.nome) == 0) /*guardar info destino na localidade dest*/
    dest = aux->via.loc1;
  if(strcmp(destino, aux->via.loc2.nome) == 0)
    dest = aux->via.loc2;

  aux = procura_na_lista_vias(partida, lv_top); /*procurar partida na lista de vias*/
  if(aux == NULL){
    printf("Nao ha nenhuma entrada chamada %s no ficheiro de vias\n", partida);
    exit(1);
  }
  if(strcmp(partida, aux->via.loc1.nome) == 0) /*guardar info partida na localidade part*/
    part = aux->via.loc1;
  if(strcmp(partida, aux->via.loc2.nome) == 0)
    part = aux->via.loc2;

  distancia(&part.p, &dest.p, &d_pd); /*Calcula a distancia entre a partida e o destino*/

  r_top = insere_localidade_na_rota(r_top, part, d_part, d_pd); /*Insere na rota a localidade de partida*/

  atual = part; /*A localidade em que estamos e a de partida*/

  while( strcmp(atual.nome, destino) != 0 )/*Se ainda nao estamos no destino*/
  {
    step = lv_top; /*step aponta para o topo da lista de vias*/
    d_dest=1000; /*Inicializamos a distancia ao destino com um valor maior q qq outro possivel no mapa para que saia para outra cidade*/
    /*---Temos de verificar se existem vias por explorar a partir de atual---*/
    step = procura_na_lista_vias(atual.nome, step);
    if (step!=NULL)
      while(step->via.estado != 0) {
        step = step->prox;
        if(step == NULL)
          break;
        step = procura_na_lista_vias(atual.nome, step);
        if(step == NULL)
          break;
      }
    if (step == NULL) {   /*Nao ha nenhuma saida por explorar a partir de atual*/
      if (strcmp(atual.nome, partida) == 0 ) {    /*Estamos na partida*/
        printf("Nao ha nenhuma ligacao possivel entre %s e %s\n", partida, destino);
        printf("Se escolheu destinos intermedios, podera ser impossivel a ligacao entre pontos intermedios sem passsar pelo destino\n");
        return NULL; /*Como nao ha ligacao encerramos a funcao rota retornando o endereco NULL*/
      }
      r_top = retira_ultima_localidade_da_rota(r_top); /*Apagamos o campo com a localidade atual*/
    }
    else {    /*step!=NULL*/ /*Ha saidas por explorar a partir de atual*/
      r_top = insere_campo_na_rota(r_top); /*Criamos um campo em branco para a proxima rota*/
      while(step != NULL) {
        estado = step->via.estado;
        if (estado == 0) {    /*Via por explorar*/
          if(strcmp(atual.nome, step->via.loc1.nome) == 0) {    /*Calcula a distancia entre a cidade para onde podemos ir e o destino*/
            distancia(&(step->via.loc2.p), &dest.p, &d_aux); /*Calcula distancias ao destino e a partida*/
            distancia(&(step->via.loc2.p), &part.p, &d_part);
            teste = compara_dist(d_dest, d_aux); /*Testa se esta cidade fica mais proxima do destino que a anterior*/
            if(teste<0) {
              melhor = step; /*guardamos a via como a melhor ate agora*/
              d_dest = d_aux;
              r_top = retira_ultima_localidade_da_rota(r_top);
              r_top = insere_localidade_na_rota(r_top, step->via.loc2, d_part, d_dest);
            }
          }
          if(strcmp(atual.nome, step->via.loc2.nome) == 0) {
            distancia(&(step->via.loc1.p), &dest.p, &d_aux);
            distancia(&(step->via.loc1.p), &part.p, &d_part);
            teste = compara_dist(d_dest, d_aux);
            if(teste<0) {
              melhor = step;
              d_dest = d_aux;
              r_top = retira_ultima_localidade_da_rota(r_top);
              r_top = insere_localidade_na_rota(r_top, step->via.loc1, d_part, d_dest);
            }
          }
        }
        step = step->prox;
        step = procura_na_lista_vias(atual.nome, step); /*Vemos se existe mais alguma via possivel*/
      }
      melhor->via.estado = 1; /*Marca a via que foi escolhida como melhor com o estado 1 (Explorada)*/
    }
    atual = r_top->loc; /*Atualizar a localidade em que estamos*/
  }
  
  return r_top;
}


/******************************************************|
| Funcao para comparar as distancias ao destino        |
|                                                      |
| Recebe: distancia anterior e distancia nova          |
| Retorna: 0=dist igual, -1=mais perto ou 1=mais longe |
|******************************************************/

int compara_dist(float d_destino, float d_destino_nova)
{
  if(d_destino_nova < d_destino) /*Se a localidade esta mais perto do destino q a anterior*/
    return -1;

  if(d_destino_nova > d_destino) /*Se a localidade esta mais longe do destino q a anterior*/
    return 1;

  return 0;
}


/******************************************************|
| Funcao para adicionar uma localidade a rota          |
|                                                      |
| Recebe: apontador para topo da rota, 1 localidade e  |
|   distancias ao destino e a partida                  |
| Retorna: apontador para novo topo da rota            |
|******************************************************/

ROTA* insere_localidade_na_rota(ROTA* top, LOCALIDADE loc, float d_partida, float d_destino)
{
  ROTA *new;

/*---Cria e preenche uma nova estrutura do tipo ROTA---*/
  new = (ROTA*) calloc (1, sizeof(ROTA) );
  new->loc = loc;
  new->d_partida = d_partida;
  new->d_destino = d_destino;
  new->prox = NULL;

  if (top == NULL)
    return new;

  new->prox = top;
  top = new;

  return top;
}


/******************************************************|
| Funcao para adicionar campo em branco a rota         |
|                                                      |
| Recebe: apontador para topo da rota                  |
| Retorna: apontador para novo topo da rota            |
|******************************************************/

ROTA* insere_campo_na_rota(ROTA* top)
{
  ROTA *new;

/*---Cria e preenche uma nova estrutura do tipo ROTA---*/
  new = (ROTA*) calloc (1, sizeof(ROTA) );
  new->prox = NULL;

  if (top == NULL)
    return new;

  new->prox = top;
  top = new;

  return top;
}


/******************************************************|
| Funcao apagar toda uma rota                          |
|                                                      |
| Recebe: apontador para o topo da ROTA                |
| Retorna: apontador para o topo com valor NULL        |
|******************************************************/

ROTA* apagar_rota(ROTA *top)
{

  while(top!=NULL)
    top = retira_ultima_localidade_da_rota(top);

  return top;
}


/******************************************************|
| Funcao para retirar a ultima localidade a rota       |
|                                                      |
| Recebe: apontador para topo da rota                  |
| Retorna: apontador para novo topo da rota            |
|******************************************************/

ROTA* retira_ultima_localidade_da_rota(ROTA* top)
{
  ROTA *aux;

  aux = top;

  top = top->prox;

  free(aux);

  return top;
}


/******************************************************|
| Funcao para imprimir no ecra toda a rota             |
|                                                      |
| Recebe: apontador para topo da rota                  |
| Retorna: nada                                        |
|******************************************************/

void mostra_rota(ROTA *top)
{
  int conta = 1;

  printf("\t\t **** ROTA ATUAL ****\n");

  if (top == NULL){
    printf("A rota esta vazia\n");
    return;
  }

  while(top != NULL){ /*Ate se chegar ao fim da lista*/
    printf("\t\t---- Ponto %d ----\n", conta++);
    mostra_localidade(top->loc); /*info localidade*/
    printf("Distancia a partida: %f \t\tDistancia ao destino : %f", top->d_partida, top->d_destino);
    putchar('\n');

    top = top->prox;
  }
  
  printf("\t\t ********************\n");
}


/******************************************************|
| Funcao para inicializar estado de todas as           |
|    rotas como por explorar                           |
|                                                      |
| Recebe: apontador para o topo da lista de rotas      |
| Retorna: nada                                        |
|******************************************************/

void vias_por_explorar(L_VIAS *top,char proibido[])
{
  if (top == NULL) /*A lista esta vazia*/
    return;

  do {
    if( (strcmp(proibido, top->via.loc1.nome) == 0) || (strcmp(proibido, top->via.loc2.nome) == 0) ) {
      top->via.estado = 1;
      top = top->prox; /*Podemos alterar o top pois nao o retornamos, logo nao se perde info*/
    }
    else {
      top->via.estado = 0;
      top = top->prox;
    }
  } while(top != NULL);
}


/******************************************************|
| Funcao para calcular rotas que tenham destinos       |
|  intermedios obrigatorios                            |
|                                                      |
| Recebe: Apontador para o topo da lista das vias      |
| Retorna: topo da rota com as passagens intermedias   |
|******************************************************/

ROTA* rota_passagens_intermedias(L_VIAS *listavias_topo,  char partida[], char destino[], char **intermedio, int pontos)
{
  ROTA *rota_final=NULL, **rota_intermedia=NULL;
  int i=0;
       
  rota_intermedia = (ROTA **) calloc(pontos+1,sizeof (ROTA*)); /*Cria 1 vetor para guardar o endereco do topo de todas as rotas intermedias*/

  if(rota_intermedia==(ROTA **)NULL) {    /*Erro na alocacao de memoria*/
    printf("ERRO_3 na alocacao de memoria - escolha um menor numero de passagens intermedias\n");
    exit(1);
  }

/*Calculo dos varios segmentos do percusro final*/
  rota_intermedia[i] = calcula_rota(listavias_topo, partida, intermedio[i], destino);
  for(i=1; i<pontos; i++)
    rota_intermedia[i] = calcula_rota(listavias_topo, intermedio [i-1], intermedio[i], destino);
  rota_intermedia[i] = calcula_rota(listavias_topo, intermedio[i-1], destino, "\0");

/*Ligacao de todas as rotas intermedias numa so*/
  rota_final = rota_intermedia[0];
  for(i=1; i < pontos+1; i++)
    rota_final = une_rotas(rota_final, rota_intermedia[i]);

/*Libertacao de memoria desnecessaria*/
  for(i=0; i < pontos; i++)
    free(intermedio[i]);
  free(intermedio);
  free(rota_intermedia);

  return rota_final;
}


/******************************************************|
| Funcao para unir 2 rotas numa so                     |
|                                                      |
| Recebe: 1 apontador para o topo de cada 1 das rotas  |
| Retorna: apontador para o topo da rota unida         |
|******************************************************/

ROTA* une_rotas(ROTA* r1, ROTA* r2)
{
  ROTA *step = NULL;

/*Percorremos a rota 2 até chegarmos ao seu fim*/
  for(step = r2; step->prox != NULL; step = step->prox);

  step->prox = r1->prox;

  free(r1);

  return r2;
}


/******************************************************|
| Funcao para verificar a existencia de uma localidade |
|                                                      |
| Recebe: nome da localidade e apontador para as vias  |
| Retorna: inteiro (0 = nao existe, 1 = existe )       |
|******************************************************/

int verifica_existencia_localidade(char nome[],L_VIAS *top)
{
  ProcessaNome(nome);  /*Leitura, processamento e procura no ficheiro da localidade introduzida*/
  top = procura_na_lista_vias(nome, top);

  if(top==NULL) {   /*Nao ha nenhuma entrada com este nome*/
    printf("Nao ha nenhuma localidade chamada %s no ficheiro.\n", nome);
    limpa_buffer();
    return 0;
  }

  return 1;
}
