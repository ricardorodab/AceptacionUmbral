#ifndef SOLUCION_H
#define SOLUCION_H
#include "ruta.h"

RUTA* get_solucion(RUTA *ruta);
RUTA* get_vecino_solucion(RUTA *ruta);
RUTA* get_mejor_solucion(RUTA* sol_1, RUTA* sol_2);

#endif //Fin de solucion.h
