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
    double s_fun = funcion_costo(s); 
    if (s_prima_fun < lote->mejor_solucion_fun){
      lote->mejor_solucion = s_prima;
      lote->mejor_solucion_fun = s_prima_fun;
      //int iterador = 0;
      //if(iterador % 50 == 0){
      printf("FUNCION:%f\n",funcion_costo(lote->mejor_solucion));
      char *factible = lote->mejor_solucion->ciudades_desconectadas == 0 ? "SI" : "NO";
      printf("Solucion factible: %s\n",factible);
      if(lote->mejor_solucion->ciudades_desconectadas > 0)
	printf("El numero de desconexidades es de :%d\n",lote->mejor_solucion->ciudades_desconectadas);
      //}
      
      //printf("COSTO: %f ", lote->mejor_solucion_fun);imprime_ruta(s_prima); 
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
