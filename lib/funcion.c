#include "funcion.h"
//Eliminar:
#include <stdio.h>

double mi_avg(RUTA *ruta)
{
  //double desc = (ruta->distancia_max*ruta->ciudades_desconectadas);
  //double suma = ruta->distancia-desc;
  //double num = (double)(ruta->num_ciudades-ruta->ciudades_desconectadas);
  return ruta->AVG;
}

double mi_max(RUTA *ruta)
{
  return (ruta->distancia_max);
}



double funcion_costo(RUTA *ruta)
{
  double suma;
  suma=ruta->distancia+(ruta->ciudades_desconectadas*(mi_max(ruta)));
  double avg_ruta = mi_avg(ruta)*((ruta->num_ciudades)-1);
  return (suma/avg_ruta);
}
