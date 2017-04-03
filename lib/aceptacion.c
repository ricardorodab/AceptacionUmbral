#include "aceptacion.h"
#include <math.h>

/**
 * Por efectos de seguir el algoritmo propuesto por las notas
 * me apego a el nombrado de las variables como t para la estructura
 * temperatura y s para la solucion (o ruta) s.
 *
 */
RUTA* aceptacion_por_umbrales(TEMPERATURA *t, RUTA *s)
{  
  LOTE *lote_temp = init_lote(s);
  double p = INFINITY;
  while(t->valor > EPSILON_TEMP){
    double p_prima = 0;
    while(fabs(p - p_prima) > EQUILIBRIO_TERM){
      p_prima = p;
      calcula_lote(t,lote_temp);
      p = lote_temp->promedio_soluciones;      
    }
    modificar_temperatura(t);
  }
  return lote_temp->ruta;
}
