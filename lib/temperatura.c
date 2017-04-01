#include "temperatura.h"

TEMPERATURA* init_temperatura(TEMPERATURA *temp, double t, double p)
{
  
}


TEMPERATURA* modificar_temperatura(TEMPERATURA *t)
{
  t->valor = t->valor*t->factor;
  return t;
}
