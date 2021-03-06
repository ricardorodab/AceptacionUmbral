#ifndef TEMPERATURA_H
#define TEMPERATURA_H
#include "ruta.h"
#include "funcion.h"

/* @TODO quitar esta parte */
//#define EPSILON_TEMP 0.2
//#define EQUILIBRIO_TERM 1



typedef struct temp {  
  double valor;
  double factor;
  double epsilon_temp;
  double epsilon_equilibrio;
} TEMPERATURA;


TEMPERATURA* init_temperatura(double valor, double factor,
			      double epsilon_temp,
			      double epsilon_equilibrio);
double porcentajes_aceptados(RUTA *s, double T);
double busqueda_binaria(RUTA *s,double T1, double T2,
			double P,TEMPERATURA *temp);
void temperatura_inicial(RUTA *s, TEMPERATURA *T, double P);
void modificar_temperatura(TEMPERATURA *t);

#endif //Fin de temperatura.h
