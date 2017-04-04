#include "lote.h"
#include <stdlib.h>

//Borrar este:
#include <stdio.h>

LOTE* init_lote(RUTA *ruta)
{
  LOTE *lote = malloc(sizeof(LOTE));
  lote->ruta = ruta;
  lote->promedio_soluciones = 0;
  lote->mejor_solucion = ruta;
  lote->mejor_solucion_fun = -1;
  return lote;
}

/**
 *
 */
void calcula_lote(TEMPERATURA *t, LOTE *lote, double L)
{
  int c = 0;
  int cota = L*100;
  double r = 0.0;
  printf("%d < %f \n",c,L); 
  while(c < L){ // && (COTA > 0)){ //Donde L es el numero de soluciones a observar.
    RUTA *s_prima = get_ruta_vecina(lote->ruta);    
    double s_prima_fun = funcion_costo(s_prima);
    double s_fun = funcion_costo(lote->ruta); 
    lote->ruta = s_prima;
    c++;
    r += s_prima_fun;
    //Disminuimos nuestra cota maxima.
    cota--;    
    if(s_prima_fun <= (s_fun+(t->valor))){
      if (s_prima_fun < lote->mejor_solucion_fun){
	lote->mejor_solucion = s_prima;
	lote->mejor_solucion_fun = s_prima_fun;	
      }
      //destroy_vecino(lote->ruta);
    }
  }
  double promedio_aceptacion = r/L;
  lote->promedio_soluciones = promedio_aceptacion;
  lote->ruta = lote->mejor_solucion;
  //lote->ruta = s;  
}
