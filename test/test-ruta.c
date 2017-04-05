#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <ciudad.h>
#include <time.h> 
#include <ruta.h>

typedef struct {
  RUTA *ruta;
  RUTA *ruta_2;    
} RutaAux;

static void rand_str(char *dest, size_t length)
{
  srand(time(NULL));
  char charset[] = "0123456789"
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  
  while (length-- > 0) {
    size_t index = (double) rand() / RAND_MAX * (sizeof charset - 1);
    *dest++ = charset[index];
  }
  *dest = '\0';
}

RUTA* genera_ruta_aleatoria(void)
{
  RUTA * ruta;
  GHashTable *cities = g_hash_table_new_full(g_int_hash,
					     g_int_equal,
					     free,free);
  GArray *muestra;
  CIUDAD *ciudad;
  char *nombre;
  char *pais;
  int poblacion;
  double latitude, longitude, distancia;
  int nombre_size;
  int pais_size;
  srand(time(NULL));
  int r = rand() % 1000;
  if(r == 0)
    r++;
  int size = rand() % 1000;
  int i;
  for(i = 0; i < size; i++)
    {
      r = rand() % 50;
      if(r == 0)
	r++;
      nombre = malloc(sizeof(char)*r);
      pais = malloc(sizeof(char)*r);
      rand_str(nombre,r);
      rand_str(pais,r);
      poblacion = ((double)(rand() % 10000))/100.0;
      latitude = ((double)(rand() % 10000))/100.0;
      longitude = ((double)(rand() % 10000))/100.0;
      ciudad = malloc(sizeof(CIUDAD));
      init_ciudad(ciudad,i,nombre,pais,poblacion,
		  latitude,longitude);
      int *key = malloc(sizeof(int));
      *key = i;
      g_hash_table_insert(cities,key,ciudad);
    }

    int muestra_size = rand() % size;
    muestra = g_array_sized_new (FALSE, FALSE,
				   sizeof(gint),muestra_size);
    
    for(i = 0; i < muestra_size; i++)
      {
	int j = i+1;
	add_vecino(g_hash_table_lookup(cities,&i),
		   g_hash_table_lookup(cities,&j),
		   rand()%999999);
    }
  
  for(i = 0; i < size-1; i++)
    {
      int *j = malloc(sizeof(int));
      *j = i;
      g_array_append_val(muestra,*j);
    }
  ruta = init_ruta(cities,muestra,size-1,2);
  return ruta;
}


static void ruta_set_up (RutaAux *ayuda,
			   gconstpointer user_data)
{
  ayuda->ruta = genera_ruta_aleatoria();
  ayuda->ruta_2 = genera_ruta_aleatoria();
}

static void ruta_tear_down(RutaAux *ayuda,
			     gconstpointer user_data)
{
  destroy_ruta(ayuda->ruta);
  destroy_ruta(ayuda->ruta_2);
}

static void test_vecinos_ruta(RutaAux *ayuda,
			      gconstpointer user_data)
{
  srand(time(NULL));
  int size = ayuda->ruta->num_ciudades;
  int num_pruebas = 1000;
  //Revisa 1000 vecinos.
  while(num_pruebas){
    int k = 0;
    int j = 0;
    int dif = 0;
    RUTA *vecina = get_ruta_vecina(ayuda->ruta);
    int i;
    for(i = 0; i < size; i++)
      {
	if(g_array_index(ayuda->ruta->arreglo,gint,i) !=
	   g_array_index(vecina->arreglo,gint,i)){
	  if(dif == 0)
	    k = i;
	  else if(dif == 1)
	    j = i;
	  else
	    g_assert_true(0);
	  dif++;
	}      
      }
    g_assert_cmpint(k, !=, j);
    for(i = 0; i < size; i++)
      {
	int tmp = i;
	if(i == k)
	  tmp = j;
	if(i == j)
	  tmp = k;
	g_assert_cmpint(g_array_index(ayuda->ruta->arreglo,gint,i),
			==,
			g_array_index(vecina->arreglo,gint,tmp));
      }
    num_pruebas--;
  }
}

int main(int argc, char** argv)
{
  g_test_init(&argc, &argv, NULL);
  g_test_bug_base("http://misbugs.org/bug?id=");
  
  g_test_add("/ruta/vecinos_ruta",RutaAux,"",
  	     ruta_set_up,test_vecinos_ruta, ruta_tear_down);

  return g_test_run();
}
