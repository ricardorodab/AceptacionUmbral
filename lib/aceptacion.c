#include "aceptacion.h"
#include <math.h>

RUTA* aceptacion_por_umbrales(TEMPERATURA *t, RUTA *s)
{
  RUTA *solucion = s;
  LOTE lote_temp;
  lote_temp.ruta = s;
  lote_promedio_soluciones = 0;
  float p = INFINITY;
  while(t->valor > EPSILON_TEMP){
    float p_prima = 0;
    while(abs(p - p_prima) > EQUILIBRIO_TERM){
      p_prima = p;
      calcula_lote(&lote_temp,t,lote_temp.ruta);
      solucion = (lote_temp.ruta,solucion);
    }
    modificar_temperatura(t);
  }
  return solucion;
}
