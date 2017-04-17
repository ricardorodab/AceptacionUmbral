#include "aceptacion.h"
#include <math.h>

/**
 * Por efectos de seguir el algoritmo propuesto por las notas
 * me apego a el nombrado de las variables como t para la estructura
 * temperatura y s para la solucion (o ruta) s.
 *
 */
#import <stdio.h>
RUTA* aceptacion_por_umbrales(TEMPERATURA *t, RUTA *s, double L)
{
  LOTE *lote_temp = init_lote(s);
  double p = INFINITY;
  int im = 0;  
  while(t->valor > t->epsilon_temp){
    double p_prima = 0;
    while(fabs(p - p_prima) > t->epsilon_equilibrio){
      printf("\n");
      imprime_ruta(lote_temp->ruta);
      printf("Desconexas: %d\n",lote_temp->ruta->ciudades_desconectadas);
      printf("\n");
      p_prima = p;
      calcula_lote(t,lote_temp,L);
      p = lote_temp->promedio_soluciones;
    }
    modificar_temperatura(t);
    }
  return lote_temp->mejor_solucion;;
}
