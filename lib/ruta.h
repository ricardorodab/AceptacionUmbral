#include <glib.h>
#include <stdbool.h>
#include "ciudad.h"

typedef struct tupla {
  CIUDAD* tupla_uno;
  CIUDAD* tupla_dos;
  double distancia;
} PAR_CIUDAD;

typedef struct ruta_estructura {
  double distancia;
  int num_ciudades;
  GHashTable *diccionario;
  PAR_CIUDAD *tupla;
} RUTA;

bool is_in_ruta(RUTA *ruta, CIUDAD* ciudad);
CIUDAD* get_ciudad(RUTA *ruta, int id);
int get_num_ciudades(RUTA *ruta);
double get_distancia(RUTA *ruta);
bool estan_conectados(RUTA *ruta, CIUDAD *ciudad_uno, CIUDAD *ciudad_dos);
