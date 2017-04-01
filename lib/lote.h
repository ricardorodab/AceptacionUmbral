#include "solucion.h"
#include "ruta.h"
#include "temperatura.h"
#include "funcion.h"

typedef struct Lote {
  double promedio_soluciones;
  RUTA *ruta;
} LOTE;

LOTE* calcula_lote(LOTE *lote,TEMPERATURA *t, RUTA *s);
