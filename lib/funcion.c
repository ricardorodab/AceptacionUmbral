#include "funcion.h"

double mi_avg(RUTA *ruta)
{
  return ruta->AVG;
}

double mi_max(RUTA *ruta)
{
  return (ruta->distancia_max*ruta->peso_desconexion);
}



double funcion_costo(RUTA *ruta)
{
  double suma;
  suma=ruta->distancia+(ruta->ciudades_desconectadas*(mi_max(ruta)));
  double avg_ruta = mi_avg(ruta)*((ruta->num_ciudades)-1);
  return (suma/avg_ruta);
}
