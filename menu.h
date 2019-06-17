#ifndef _MENU_
#define _MENU_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <g2.h>
#include <g2_X11.h>

void processa_argumentos(int argc, char *argv[], char dist[], char cor[], float *amp, int *ap_cor);
void ProcessaNome(char nome[]);
void limpa_buffer();
int escolhe_cor(char strc[]);
int Menu(float *ap_a, float *ap_x0, float *ap_y0, L_VIAS *lv_top, ROTA **ap_rota_topo);
void rota_direta(L_VIAS *lv_top, ROTA **ap_rota_topo);
void rota_destino_intermedio(L_VIAS *lv_top, ROTA **ap_rota_topo);

#endif /*caso _MENU_ ja tenha sido defenido*/
