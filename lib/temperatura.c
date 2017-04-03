#include "temperatura.h"
#include <math.h> 
#include <stdlib.h>
#include "ruta.h"
//Eliminar:
#include <stdio.h>


TEMPERATURA* init_temperatura(double valor, double factor)
{
  TEMPERATURA *temp = malloc(sizeof(TEMPERATURA));
  temp->valor = valor;
  temp->factor = factor;
  return temp;
}

/**
 *
 * @TODO 1. La N = s->num_ciudades? 2. Liberar memoria
 */
double porcentajes_aceptados(RUTA *s, double T)
{
  double c = 0;
  int i;
  for(i = 0; i < s->num_ciudades; i++)
    {
      RUTA *s_prima = get_ruta_vecina(s);

      if(funcion_costo(s_prima) <= (funcion_costo(s)+T))
	c += 1.0;      
      s = s_prima;
      //Liberar memoria de s.
    }
  return (c/s->num_ciudades);
}

/**
 * @TODO definir et y ep como ceros virtuales.
 */
double busqueda_binaria(RUTA *s,double T1, double T2, double P)
{
  double et = 0.2;
  double ep = 0.2;
  double Tm = (T1+T2)/2;
  if((T2-T1) < et)
    return Tm;
  double p_peque = porcentajes_aceptados(s,Tm);
  if(fabs(P-p_peque) < ep)
    return Tm;
  if(p_peque > P)
    return busqueda_binaria(s,T1,Tm,P);
  return busqueda_binaria(s,Tm,T2,P);
}

void temperatura_inicial(RUTA *s, TEMPERATURA *T, double P)
{
  double T1,T2;
  double p_peque = porcentajes_aceptados(s,T->valor);
  if(fabs(P-p_peque) <= EPSILON_TEMP)
    return;
  if(p_peque < P){
    while(p_peque < P)
      {
	T->valor = 2*(T->valor);
	p_peque = porcentajes_aceptados(s,T->valor);      
      }
    T1 = T->valor/2;
    T2 = T->valor;
  }else{
    while(p_peque > P)
      {
	T->valor = T->valor/2;
	p_peque = porcentajes_aceptados(s,T->valor);
      }
    T1 = T->valor;
    T2 = 2*T->valor;
  }
  T->valor = busqueda_binaria(s,T1,T2,P);
}


void modificar_temperatura(TEMPERATURA *t)
{
  t->valor = t->valor*t->factor;
}
