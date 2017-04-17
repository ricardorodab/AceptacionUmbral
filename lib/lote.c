#include "lote.h"
#include <stdlib.h>
#include <math.h>

LOTE* init_lote(RUTA *ruta)
{
  LOTE *lote = malloc(sizeof(LOTE));
  lote->ruta = ruta;
  lote->promedio_soluciones = 0;
  lote->mejor_solucion = ruta;
  lote->mejor_solucion_fun = INFINITY;
  return lote;
}

#include <stdio.h>
/**
 *
 */
void calcula_lote(TEMPERATURA *t, LOTE *lote, double L)
{
  RUTA *s = lote->ruta;
  int c = 0;
  int COTA = L*2000;
  double r = 0.0;
  while(c < L && (COTA > 0)){ //Donde L = |n| con n ciudades en ruta.
    COTA--;
    RUTA *s_prima = get_ruta_vecina(s);
    double s_prima_fun = funcion_costo(s_prima);
    /*printf("\n");
    imprime_ruta(s_prima);
    printf("Funcion: %f\n",s_prima_fun);
    printf("Desconexas: %d\n",s_prima->ciudades_desconectadas);
    printf("\n");*/
    double s_fun = funcion_costo(s); 
    if (s_prima_fun < lote->mejor_solucion_fun){
      lote->mejor_solucion = s_prima;
      lote->mejor_solucion_fun = s_prima_fun;
    }
    if(s_prima_fun <= (s_fun+(t->valor))){
      if(s_fun != lote->mejor_solucion_fun)
	destroy_vecino(s);
      s = s_prima;
      c++;
      r += s_prima_fun;
    }
  }
  double promedio_aceptacion = r/L;
  if(promedio_aceptacion != 0)
    lote->promedio_soluciones = promedio_aceptacion;
  else
    lote->promedio_soluciones = lote->promedio_soluciones/2;
  lote->ruta = s;
}
