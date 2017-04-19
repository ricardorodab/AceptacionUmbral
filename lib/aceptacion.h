#ifndef ACEPTACION_H
#define ACEPTACION_H
#include "ruta.h"
#include "temperatura.h"
#include "lote.h"

RUTA* barrido_vecinos(RUTA *ruta);
RUTA* aceptacion_por_umbrales(TEMPERATURA *t, RUTA *r,double L);
#endif //Fin de aceptacion.h
