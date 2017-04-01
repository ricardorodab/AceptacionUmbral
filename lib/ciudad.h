#include <glib.h>

typedef struct ciudad_estructura {
  int id;
  char *name;
  char *country;
  int population;
  double latitude;
  double longitude;
  GHashTable *vecinos;
} CIUDAD;

void init_ciudad(CIUDAD *ciudad, int id,
		    char *name, char *country,
		    int population, double latitude,
		    double longitude);

void delete_ciudad(CIUDAD *ciudad);

CIUDAD* get_vecino(CIUDAD *ciudad, int id);
void add_vecino(CIUDAD *ciudad, CIUDAD *vecino, double distancia);
double get_distancia_ciudad(CIUDAD *ciudad_origen, CIUDAD *ciudad_destino);
void imprime_ciudad(CIUDAD *ciudad);
