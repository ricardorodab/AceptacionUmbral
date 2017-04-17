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
  char charset[] = "0123456789"
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  
  while (length-- > 0) {
    size_t index = (double) rand() / RAND_MAX * (sizeof charset - 1);
    *dest++ = charset[index];
  }
  *dest = '\0';
}


RUTA* genera_ruta_aleatoria_desconectados(void)
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
  int r;
  int size = rand() % 500;
  size++;
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
      init_ciudad(ciudad,i+1,nombre,pais,poblacion,
		  latitude,longitude);
      int *key = malloc(sizeof(int));
      *key = i+1;
      g_hash_table_insert(cities,key,ciudad);
    }
  
  muestra = g_array_sized_new (FALSE, FALSE,
			       sizeof(gint),size);
  
  for(i = 0; i < size; i++)
    {
      int *j = malloc(sizeof(int));
      *j = i+1;
      g_array_append_val(muestra,*j);
    }
  ruta = init_ruta(cities,muestra,size,2);
  return ruta;
}



RUTA* genera_ruta_aleatoria(void)
{
  RUTA * ruta;
  GHashTable *cities = g_hash_table_new_full(g_int_hash,
					     g_int_equal,
					     free,free);
  GArray *muestra;
  CIUDAD *ciudad;
  CIUDAD *ciudad_i,*ciudad_j;;
  char *nombre;
  char *pais;
  int poblacion;
  double latitude, longitude, distancia;
  int nombre_size;
  int pais_size;
  int r;
  int *id;
  //El size es el tamanio de mi mundo prueba.
  int size = rand() % 500;
  size += 5;
  int i;
  muestra = g_array_sized_new(FALSE, FALSE,
			      sizeof(gint),size);  
  for(i = 0; i < size; i++) 
    {
      r = rand() % 50;
      if(r == 0)
	r++;
      nombre = malloc(sizeof(char)*r+1);
      pais = malloc(sizeof(char)*r+1);
      rand_str(nombre,r);
      rand_str(pais,r);      
      poblacion = ((double)(rand() % 10000))/100.0;
      latitude = ((double)(rand() % 10000))/100.0;
      longitude = ((double)(rand() % 10000))/100.0;
      ciudad = malloc(sizeof(CIUDAD));
      int *key = malloc(sizeof(int));
      *key = i;
      init_ciudad(ciudad,*key,nombre,pais,poblacion,
		  latitude,longitude);
      g_hash_table_insert(cities,key,ciudad);
      
      g_array_append_val(muestra,i);
    }  

  int j,llave_1,llave_2;
  ruta = init_ruta(cities,muestra,size,2);
  for(i = 0; i < ruta->num_ciudades-1; i++)
    {	  
      j = i+1;
      llave_1 = g_array_index(ruta->arreglo,gint,i);
      llave_2 = g_array_index(ruta->arreglo,gint,j);
      ciudad_i = g_hash_table_lookup(ruta->ciudades,&llave_1);
      ciudad_j = g_hash_table_lookup(ruta->ciudades,&llave_2);
      add_vecino(ciudad_i,ciudad_j,rand()%999999);
    }
  set_distancias(ruta);  
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

/*
 * Prueba de get_ruta_vecina
 */
static void test_vecinos_ruta(RutaAux *ayuda,
			      gconstpointer user_data)
{ 
  int size = ayuda->ruta->num_ciudades;
  int num_pruebas = 1000;
  //Revisa 1000 vecinos.
  while(num_pruebas){
    int k = 0;
    int j = 0;
    int dif = 0;
    RUTA *vecina = get_ruta_vecina(ayuda->ruta);
    g_assert(&vecina->num_ciudades != &ayuda->ruta->num_ciudades);
    g_assert(vecina->distancia_max == ayuda->ruta->distancia_max);
    g_assert(&vecina->arreglo != &ayuda->ruta->arreglo);
    g_assert(vecina->ciudades == ayuda->ruta->ciudades);
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

/*
 * Test de set_distancias.
 */
static void test_set_distancias(RutaAux *ayuda,
			      gconstpointer user_data)
{

  RUTA *ruta = ayuda->ruta;
  RUTA *vecino;
  //Numero de veces que se corre > 50 ~ 6 minutos
  int i = 50;
  double distancia, distancia_max;
  double ciudades_desconectadas;
  double AVG;
  while(i--)
    {
      vecino = get_ruta_vecina(ruta);
      g_assert(ruta->AVG == vecino->AVG);
      RUTA *descon = genera_ruta_aleatoria_desconectados();
      set_distancias(descon);
      g_assert(ruta->AVG == vecino->AVG);
      g_assert(descon->ciudades_desconectadas ==
	       descon->num_ciudades-1);
      g_assert(descon->distancia_max == 0);
      g_assert(descon->distancia == 0);
      int j;
      int *key_1 = malloc(sizeof(int));
      int *key_2 = malloc(sizeof(int));
      for(j = 0; j < descon->num_ciudades-1; j++)
      {	
	distancia = descon->distancia;
	distancia_max = descon->distancia_max;
	AVG = descon->AVG;
	ciudades_desconectadas = descon->ciudades_desconectadas;
	int k = j+1;
	*key_1 = g_array_index(descon->arreglo,gint,j);
	*key_2 = g_array_index(descon->arreglo,gint,k);
	CIUDAD *city_1 = g_hash_table_lookup(descon->ciudades,key_1);
	CIUDAD *city_2 = g_hash_table_lookup(descon->ciudades,key_2);
	double ran = rand() % 999999;
	g_assert_false(son_vecinos(city_1,city_2));	
	g_assert(descon->ciudades_desconectadas > 0);
	add_vecino(city_1,city_2,ran);
	set_distancias(descon);
	g_assert_false(descon->AVG == AVG);
	g_assert_true(son_vecinos
		      (g_hash_table_lookup(descon->ciudades,key_2),
		       g_hash_table_lookup(descon->ciudades,key_1)));
	g_assert(distancia < descon->distancia);	 
	g_assert(distancia_max <= descon->distancia_max);
	g_assert(ciudades_desconectadas ==
		 descon->ciudades_desconectadas+1);
      }
      g_assert(descon->ciudades_desconectadas == 0);
    } 
}
static void test_recalcula_distancia(RutaAux *ayuda,
				     gconstpointer user_data)
{
  int it = 100;
  double distancia, distancia_max;
  int ciudades_desconectadas;
  double AVG;
  CIUDAD *ciudad_i, *ciudad_j;
  double distancia_vecinos;
  int rand_i, rand_j;
  int size;
  while(it--)
    {
      RUTA *ruta = genera_ruta_aleatoria();
      int i;
      int j;
      int llave_1,llave_2;
      ciudades_desconectadas = ruta->ciudades_desconectadas;
      AVG = ruta->AVG;
      distancia_max = ruta->distancia_max;
      distancia = ruta->distancia;
      size = ruta->num_ciudades;
      g_assert(ruta->ciudades_desconectadas == 0);
      g_assert(ruta->num_ciudades > 0);
      g_assert(ruta->distancia_max > 0);
      g_assert(AVG > 0);
      rand_i = rand() % size;
      rand_j = rand() % size;
      while(rand_i == rand_j)
	rand_j = rand() % size;
      int temp = g_array_index(ruta->arreglo,gint,rand_i);
      recalcula_distancia(ruta,rand_i,rand_j);
      g_array_index(ruta->arreglo,gint,rand_i) = g_array_index(ruta->arreglo,gint,rand_j);
      g_array_index(ruta->arreglo,gint,rand_j) = temp;
      if(size > 2)
	g_assert(ciudades_desconectadas < ruta->ciudades_desconectadas);
      g_assert(distancia > ruta->distancia);     
    }
}


int main(int argc, char** argv)
{
  srand(time(NULL));
  g_test_init(&argc, &argv, NULL);
  g_test_bug_base("http://misbugs.org/bug?id="); 
  g_test_add("/ruta/vecinos_ruta",RutaAux,"",
    ruta_set_up,test_vecinos_ruta, ruta_tear_down);  
  g_test_add("/ruta/vecinos_set_distancias",RutaAux,"",
    ruta_set_up,test_set_distancias,ruta_tear_down);
  g_test_add("/ruta/vecinos_recalcula_distancias",RutaAux,"",
	     ruta_set_up,test_recalcula_distancia,ruta_tear_down);
  
  return g_test_run();
}
