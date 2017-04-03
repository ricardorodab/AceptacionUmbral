#include "lote.h"
#include <stdlib.h>

LOTE* init_lote(RUTA *ruta)
{
  LOTE *lote = malloc(sizeof(LOTE));
  lote->ruta = ruta;
  lote->promedio_soluciones = 0;
  return lote;
}

/**
 * @TODO liberar memoria.
 */
void calcula_lote(TEMPERATURA *t, LOTE *lote)
{
  int L = 500; //Numero de soluciones a observar.
  int c = 0;
  double r = 0.0;
  while(c < L){ //Donde L es el numero de soluciones a observar.
    RUTA *s_prima = get_ruta_vecina(lote->ruta);    
    double s_prima_fun = funcion_costo(s_prima);
    double s_fun = funcion_costo(lote->ruta);
    if(s_prima_fun <= (s_fun+(t->valor))){
      destroy_vecino(lote->ruta);
      lote->ruta = s_prima;

      //Liberar memoria del vecino.
      c++;
      r += s_prima_fun;
    }
  }
  double promedio_aceptacion = r/L;
  lote->promedio_soluciones = promedio_aceptacion;
  //lote->ruta = s;  
}
