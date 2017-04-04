#ifndef LOTE_H
#define LOTE_H
#include "solucion.h"
#include "ruta.h"
#include "temperatura.h"
#include "funcion.h"

typedef struct Lote {
  double promedio_soluciones;
  RUTA *mejor_solucion;  
  double mejor_solucion_fun;
  RUTA *ruta;  
} LOTE;

LOTE *init_lote(RUTA *ruta);
void calcula_lote(TEMPERATURA *t, LOTE *s, double L);
#endif //Fin de lote.h
