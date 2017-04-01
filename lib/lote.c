#include "lote.h"

LOTE* calcula_lote(LOTE *lote, TEMPERATURA *t, RUTA *s)
{
  int L = 1000; //Numero de soluciones a observar.
  int c = 0;
  double r = 0.0;
  while(c < L){ //Donde L es el numero de soluciones a observar.
    RUTA *s_prima = get_vecino(s);
    double s_prima_fun = funcion_costo(s_prima);
    double s_fun = funcion_costo(s);
    if(s_prima_fun <= (s_fun+(t->valor))){
      s = s_prima;
      c++;
      r += s_prima_fun;
    }
  }
  double promedio_aceptacion = r/L;
  lote->promedio_soluciones = promedio_aceptacion;
  lote->ruta = s;  
  return lote;
}
