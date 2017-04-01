#include "funcion.h"

double mi_avg(RUTA *ruta)
{
  double suma = ruta->distancia;
  double num = (double)(ruta->num_ciudades)-1.0;
  return (suma/num);
}

double mi_max(RUTA *ruta)
{
  return (ruta->tupla->distancia);
}



double funcion_costo(RUTA *ruta)
{
  double suma = ruta->distancia;
  double avg_ruta = avg(ruta)*(ruta->num_ciudades);
  return (suma/avg_ruta);
}
