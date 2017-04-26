#include "aceptacion.h"
#include <math.h>
#include <stdio.h>


RUTA* barrido_vecinos(RUTA *ruta)
{
  int i,j,k;
  RUTA *ruta_1, *ruta_2, *ruta_3;
  for(i = 0; i < ruta->num_ciudades-2; i++){
    j = i+1;
    k = i+2;
    ruta_1 = get_ruta_vecina_index(ruta,i,j);
    ruta_2 = get_ruta_vecina_index(ruta,i,k);
    ruta_3 = get_ruta_vecina_index(ruta,j,k);
    if(funcion_costo(ruta) > funcion_costo(ruta_1)){
      destroy_vecino(ruta);
      ruta = ruta_1;
    }
    if(funcion_costo(ruta) > funcion_costo(ruta_2)){
      destroy_vecino(ruta);
      ruta = ruta_2;
    }
    if(funcion_costo(ruta) > funcion_costo(ruta_3)){
      destroy_vecino(ruta);
      ruta = ruta_3;
    }
  }
  return ruta;
}

/**
 * Por efectos de seguir el algoritmo propuesto por las notas
 * me apego a el nombrado de las variables como t para la estructura
 * temperatura y s para la solucion (o ruta) s.
 *
 */
RUTA* aceptacion_por_umbrales(TEMPERATURA *t, RUTA *s, double L)
{
  LOTE *lote_temp = init_lote(s);
  RUTA *temp = s;
  double p = INFINITY;
  int im = 0;  
  while(t->valor > t->epsilon_temp){
    double p_prima = 0;
    while(fabs(p - p_prima) > t->epsilon_equilibrio){
      //printf("\n");
      //imprime_ruta(lote_temp->ruta);
      //printf("Desconexas: %d\n",lote_temp->ruta->ciudades_desconectadas);
      //printf("Distancia: %f\n",lote_temp->ruta->distancia);
      //printf("\n");
      p_prima = p;
      calcula_lote(t,lote_temp,L);
      p = lote_temp->promedio_soluciones;
    }
    modificar_temperatura(t);
    }
  return barrido_vecinos(lote_temp->mejor_solucion);
}
