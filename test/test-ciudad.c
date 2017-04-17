#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <ciudad.h>

typedef struct {
  CIUDAD *ciudad;
  CIUDAD *helper1;
  CIUDAD *helper2;
  double distancia0_1;
  double distancia0_2;
  double distancia1_2;
} CiudadVerificador;


static void ciudad_set_up (CiudadVerificador *ayuda,
			   gconstpointer user_data)
{
  ayuda->ciudad = malloc(sizeof(CIUDAD));
  ayuda->helper1 = malloc(sizeof(CIUDAD));
  ayuda->helper2 = malloc(sizeof(CIUDAD)); 
  char **nombre = (char**)user_data;
  init_ciudad(ayuda->ciudad, 1,
	      nombre[0],nombre[1],
	      1729,-30.3985,-97.6551);
   init_ciudad(ayuda->helper1, 2,
	      nombre[2],nombre[3],
	      4234, 51.361558, -0.697747);
  init_ciudad(ayuda->helper2, 3,
	      nombre[4],nombre[5],
	      1024, 20.361558, -100.697747);
  ayuda->distancia0_1 = 12345.678;
  ayuda->distancia0_2 = 87654.321;
  ayuda->distancia1_2 = 10293.845;
}

static void ciudad_tear_down(CiudadVerificador *ayuda,
			     gconstpointer user_data)
{
  delete_ciudad(ayuda->ciudad);
  delete_ciudad(ayuda->helper1);
  delete_ciudad(ayuda->helper2);
}

static void test_add_vecino(CiudadVerificador *ayuda,
			    gconstpointer user_data)
{
  CIUDAD *ciudad = ayuda->ciudad;
  CIUDAD *helper1 = ayuda->helper1;
  CIUDAD *helper2 = ayuda->helper2;
  //printf("%d\n",son_vecinos(ciudad,helper1));
  g_assert(!son_vecinos(ciudad,helper1));
  g_assert(!son_vecinos(helper1,ciudad));
  
  g_assert(!son_vecinos(ciudad,helper2));
  g_assert(!son_vecinos(helper2,ciudad));
  
  g_assert(!son_vecinos(helper2,helper1));
  g_assert(!son_vecinos(helper1,helper2));
  
  //Los hacemos vecinos ciuad y helper1:
  add_vecino(ciudad,helper1,ayuda->distancia0_1);
  g_assert(son_vecinos(ciudad,helper1));
  g_assert(son_vecinos(helper1,ciudad));
  
  g_assert(!son_vecinos(ciudad,helper2));
  g_assert(!son_vecinos(helper2,ciudad));
  
  g_assert(!son_vecinos(helper2,helper1));
  g_assert(!son_vecinos(helper1,helper2));

  //Hacemos vecinos ciudad y helper 2
  add_vecino(ciudad,helper2,ayuda->distancia0_2);
  g_assert(son_vecinos(ciudad,helper1));
  g_assert(son_vecinos(helper1,ciudad));
  
  g_assert(son_vecinos(ciudad,helper2));
  g_assert(son_vecinos(helper2,ciudad));
  
  g_assert(!son_vecinos(helper2,helper1));
  g_assert(!son_vecinos(helper1,helper2));

  //Hacemos vecinos a helper 1 y 2
  add_vecino(helper2,helper1,ayuda->distancia1_2);
  g_assert(son_vecinos(ciudad,helper1));
  g_assert(son_vecinos(helper1,ciudad));
  
  g_assert(son_vecinos(ciudad,helper2));
  g_assert(son_vecinos(helper2,ciudad));
  
  g_assert(son_vecinos(helper2,helper1));
  g_assert(son_vecinos(helper1,helper2));
}

int main(int argc, char** argv)
{
  char *nombres[] = {"Prueba","Reino Aventura",
		    "Oz","Not Kansas",
		     "Foofaraw","Maybe Kansas"};
  g_test_init(&argc, &argv, NULL);
  g_test_bug_base("http://misbugs.org/bug?id=");
  
  g_test_add("/ciudad/add_vecino",CiudadVerificador,nombres,
	     ciudad_set_up,test_add_vecino, ciudad_tear_down);
  
  return g_test_run();
}
