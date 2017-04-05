#include "temperatura.h"
#include <math.h> 
#include <stdlib.h>
#include "ruta.h"
//Eliminar:
#include <stdio.h>


TEMPERATURA* init_temperatura(double valor, double factor,
			      double epsilon_temp,
			      double epsilon_equilibrio)
{
  TEMPERATURA *temp = malloc(sizeof(TEMPERATURA));
  temp->valor = valor;
  temp->factor = factor;
  temp->epsilon_temp = epsilon_temp;
  temp->epsilon_equilibrio = epsilon_equilibrio;
  return temp;
}

/**
 *
 * @TODO 1. La N = s->num_ciudades? 2. Liberar memoria
 */
double porcentajes_aceptados(RUTA *ruta, double T)
{
  RUTA *s = ruta;
  int cota = s->num_ciudades;
  double c = 0;
  int i;
  for(i = 0; i < cota; i++)
    {
      RUTA *s_prima = get_ruta_vecina(s);
      //printf("Distancia s : %f\n",s->distancia);
      //printf("Distancia s': %f\n",s_prima->distancia);
      //printf("DESCONEC': %d\n",ruta->ciudades_desconectadas);
      //printf("IF: %f <= %f + %f\n",funcion_costo(s_prima),funcion_costo(s),T);
      if(funcion_costo(s_prima) <= (funcion_costo(s)+T))
	c++;      
      s = s_prima;
      //Liberar memoria de s.
    }
  return (c/s->num_ciudades);
}

/**
 * @TODO definir et y ep como ceros virtuales.
 */
double busqueda_binaria(RUTA *s,double T1, double T2,
			double P,TEMPERATURA *temp)
{
  double et = temp->epsilon_temp;
  double ep = temp->epsilon_equilibrio;
  double Tm = (T1+T2)/2;
  if((T2-T1) < et)
    return Tm;
  double p_peque = porcentajes_aceptados(s,Tm);
  if(fabs(P-p_peque) < ep)
    return Tm;
  if(p_peque > P)
    return busqueda_binaria(s,T1,Tm,P,temp);
  return busqueda_binaria(s,Tm,T2,P,temp);
}

void temperatura_inicial(RUTA *s, TEMPERATURA *T, double P)
{
  double T1,T2;
  double p_peque = porcentajes_aceptados(s,T->valor);
  //printf("1. %f\n",T->valor);
  // printf("1.5 %f\n",p_peque);
  if(fabs(P-p_peque) <= T->epsilon_temp)
    return;
  if(p_peque < P){
    while(p_peque < P)
      {
	T->valor = 2*(T->valor);
	p_peque = porcentajes_aceptados(s,T->valor);      
      }
    T1 = T->valor/2;
    T2 = T->valor;
    //printf("2. %f\n",T->valor);
  }else{
    while(p_peque > P)
      {
	T->valor = T->valor/2;
	//printf("T->VALOR. %f\n",T->valor);
	p_peque = porcentajes_aceptados(s,T->valor);
	//printf("pe_peue %f\n",p_peque);
      }
    T1 = T->valor;
    T2 = 2*T->valor;
  }
  T->valor = busqueda_binaria(s,T1,T2,P,T);
  //printf("3. %f\n",T->valor);
}


void modificar_temperatura(TEMPERATURA *t)
{
  t->valor = t->valor*t->factor;
}
