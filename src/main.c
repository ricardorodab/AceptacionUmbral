/* -------------------------------------------------------------------
 * main.c
 * version 1.0
 * Copyright (C) 2017  Jose Ricardo Rodriguez Abreu.
 * Facultad de Ciencias,
 * Universidad Nacional Autonoma de Mexico, Mexico.
 *
 * Este programa es software libre; se puede redistribuir
 * y/o modificar en los terminos establecidos por la
 * Licencia Publica General de GNU tal como fue publicada
 * por la Free Software Foundation en la version 2 o
 * superior.
 *
 * Este programa es distribuido con la esperanza de que
 * resulte de utilidad, pero SIN GARANTIA ALGUNA; de hecho
 * sin la garantia implicita de COMERCIALIZACION o
 * ADECUACION PARA PROPOSITOS PARTICULARES. Vease la
 * Licencia Publica General de GNU para mayores detalles.
 *
 * Con este programa se debe haber recibido una copia de la
 * Licencia Publica General de GNU, de no ser asi, visite el
 * siguiente URL:
 * http://www.gnu.org/licenses/gpl.html
 * o escriba a la Free Software Foundation Inc.,
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * -------------------------------------------------------------------
 */

/**
 * @file main.c
 * @author Jose Ricardo Rodriguez Abreu
 * @date 31 Mar 2017
 * @brief File containing the first project for the
 * "Combinatorial Optimization Heuristics" class.
 *
 * Este programa utiliza una heuristica conocida por "Aceptacion por umbrales"
 * sobre el famoso problema del viajero. Donde buscamos una solucion factible
 * a una ruta (no ciclo) Hamiltoniano.
 *
 * El programa usa el estandar de documentacion que define el uso de 
 * doxygen.
 *
 * @see http://www.stack.nl/~dimitri/doxygen/manual/index.html
 * @see https://github.com/ricardorodab/AceptacionUmbral
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h> 
#include "conexion.h"
#include "ciudad.h"
#include "temperatura.h"
#include "ruta.h"
#include "lote.h"
#include "aceptacion.h"

/**
 * @brief Linea del codigo actual.
 *
 * Re-definimos la constante de linea que viene por default en C.
 *
 */
#define LINEA __LINE__
#define QUERY1 "SELECT * FROM cities"
#define QUERY2 "SELECT * FROM connections"

/**
 * @brief Variable de marcardo de error.
 *
 * Si en algun momento de la ejecucion esta variable es != 0 terminamos. 
 *
 */
int ERROR = 0;

/**
 * @brief Una funcion simple para imprimir que hubo un error y en que linea.
 *
 * Si llega a ocurrir se debe anexar un mensaje de error que el usuario
 * pueda ver para facilitar la facil busqueda de este.
 *
 * @param msg -Es la cadena que se le mostrara al usuario.
 * @param linea -Es la linea del codigo donde ocurrio este error. 
 * @see ERROR
 * @note Esta funcion termina la ejecucion del programa.
 * @waring
 *
 */
void main_imprime_error(char *msg,int linea)
{
  char *err = strcat(msg,"-Linea: %d \n");
  printf(err,linea);
  exit(1);
}

/**
 * @brief Funcion usada para crear cada ciudad y agregarla a un diccionario.
 * 
 * Esta funcion no se usa directamente sino que es parametro de la funcion
 * que abre una conexion con la base de datos y manda a llamar a esta con
 * cada una de las instancias del query que se le haga.
 * En particular lo que hacemos es crear estructuras de tipo CIUDAD, 
 * inicializarlas con todos sus datos y meterla dentro de un hash.
 *
 * @param param_usr -Son los parametros que el programador puede pasar a mano.
 * @param argc -Es el numero de elementos que contiene cada instancia de
 * las busquedas de la base.
 * @param argv -Son los elementos que contiene cada instancia de la busqueda.
 * @param azColName -Son los nombres de las columnas de la busqueda hecha.
 * @return 0 si todo ocurrio sin problemas y 1 en caso contrario.
 * @see get_query
 *
 */
int init_ciudades(void *param_usr, int argc, char **argv,char **azColName)
{
  // Realizamos un cast ya que solo podemos pasarlo de forma de (void*).
  GHashTable* diccionario = (GHashTable*)param_usr;
  // Asignamos la memoria a cada una de las instancias de la base.
  CIUDAD *ciudad = malloc(sizeof(CIUDAD));
  ERROR = ciudad == NULL ? 1 : ERROR;
  char *name_fin = NULL;
  char *country_fin = NULL;
  int nam,cou;
  int id = argv[0] ? atoi(argv[0]) : -1;
  char *name = argv[1] ? argv[1] : NULL;
  // Asignacion de memoria para el nombre de cada instancia string.
  if(name != NULL){
    nam = strlen(name);    
    name_fin = malloc(sizeof(char)*nam+1);
    ERROR = name_fin == NULL ? 1 : ERROR;
    strcpy(name_fin,name);
  }
  char *country = argv[2] ? argv[2] : NULL;
    if(country != NULL){
    cou = strlen(country);    
    country_fin = malloc(sizeof(char)*cou+1);
    ERROR = country_fin == NULL ? 1 : ERROR;
    strcpy(country_fin,country);
  }
  int population = argv[3] ? atoi(argv[3]) : -1;
  double latitude = argv[4] ? atof(argv[4]) : -1.0;
  double longitude = argv[5] ? atof(argv[5]) : -1.0;
  //Inicializamos nuestra ciudad con todos sus parametros.
  init_ciudad(ciudad,id,name_fin,country_fin,population,latitude,longitude);
  //La insertamos en el diccionario de ciudades.
  int d = g_hash_table_size(diccionario);
  //Las llaves deben de ser almacenadas en el heap para no perderlas.
  int *key = malloc(sizeof(int*));
  *key = id;
  g_hash_table_insert(diccionario,key,ciudad);
  return ERROR;
}

/**
 * @brief Funcion usada para conectar un conjunto de ciudades.
 * 
 * Esta funcion no se usa directamente sino que es parametro de la funcion
 * que abre una conexion con la base de datos y manda a llamar a esta con
 * cada una de las instancias del query que se le haga.
 * En particular creamos conexiones en las estructuras definidas como 
 * CIUDADES que contienen un diccionario de vecinos.
 *
 * @param param_usr -Son los parametros que el programador puede pasar a mano.
 * @param argc -Es el numero de elementos que contiene cada instancia de
 * las busquedas de la base.
 * @param argv -Son los elementos que contiene cada instancia de la busqueda.
 * @param azColName -Son los nombres de las columnas de la busqueda hecha.
 * @return 0 si todo ocurrio sin problemas y 1 en caso contrario.
 * @see get_query
 *
 */
int conecta_ciudades(void *param_usr, int argc, char **argv,char **azColName)
{
  // Realizamos un cast ya que solo podemos pasarlo de forma de (void*).
  GHashTable *diccionario = (GHashTable*)param_usr;
  GHashTable *dicc_uno, *dicc_dos;
  // Instanciamos todo lo que la conexion de un par de ciudades usa.
  CIUDAD *ciudad_uno, *ciudad_dos; 
  int *num_uno = malloc(sizeof(int));
  int *num_dos = malloc(sizeof(int));
  double distancia;
  //Asignamos los valores de la base de datos a las variables o una de error:
  *num_uno = argv[0] ? atoi(argv[0]) : -1;
  *num_dos = argv[1] ? atoi(argv[1]) : -1;
  distancia = argv[2] ? atof(argv[2]) : -1.0;
  //Verificamos que no haya habido ningun error.
  if(*num_uno == -1 || *num_dos == -1 || distancia == -1.0)
    return (ERROR = 1);
  //Obtenemos las ciudades:
  ciudad_uno = g_hash_table_lookup(diccionario,num_uno);
  ciudad_dos = g_hash_table_lookup(diccionario,num_dos);
  //Ahora los hacemos vecinos uno del otro:
  add_vecino(ciudad_uno,ciudad_dos,distancia);
  return 0;
}




/**
 * @brief Metodo principal del programa.
 *
 * Tratamos de encontrar soluciones factibles al problema del viajero.
 * @param argc - Es el numero de parametros. En este caso se espera argc > 2.
 * @param argv - Son los parametros que nos daran la base de datos y banderas.
 * @return Un entero 0 si todo sale bien 1 en caso contrario.
 *
 */
int main(int argc, char** argv)
{
  //Definimos las variables que usaremos en el programa:
  GHashTable *cities;
  //A leer de externo:
  char *nombre_base;
  GArray *muestra; 
  int muestra_size;
  double P, T;
  double PESO_DESC = 2;
  //En la variable cities se encuentran toda la informacion de las ciudades.
  cities = g_hash_table_new_full(g_int_hash,
				 g_int_equal,
				 free,free);
  nombre_base = "./db/hoc.db";
  //Realizamos la conexion a la base y poblamos nuestro hash.
  //Seguido de eso poblamos las conexiones de nuestras ciudades.
  if(get_query(nombre_base,QUERY1,db,(void*)cities,init_ciudades) ||
     get_query(nombre_base,QUERY2,db,(void*)cities,conecta_ciudades)){
    main_imprime_error("Error al conectar las ciudades.",LINEA-2);
  }
  // AQUI TENEMOS QUE INICIALIZAR MUESTRA;
  //109,160,200,212
  muestra_size = 5;
  muestra = g_array_sized_new (FALSE, FALSE, sizeof(int),muestra_size);
  int *a,*b,*c,*d,*e;
  a = malloc(sizeof(int*));
  b = malloc(sizeof(int*));
  c = malloc(sizeof(int*));
  d = malloc(sizeof(int*));
  e = malloc(sizeof(int*));
  *a = 109;
  *b = 160;
  *c = 200;
  *d = 212;
  *e = 239;
  g_array_append_val(muestra,*a);
  g_array_append_val(muestra,*b);
  g_array_append_val(muestra,*c);
  g_array_append_val(muestra,*d);
  g_array_append_val(muestra,*e);
  RUTA *ruta_inicial_aleatoria = init_ruta(cities,muestra,
					   muestra_size,PESO_DESC);
  //LOTE *lote = init_lote(ruta_inicial_aleatoria);
  P = .85; // .85 <= P <= .95
  T = 8;
  TEMPERATURA *temperatura = init_temperatura(T,P);
  temperatura_inicial(ruta_inicial_aleatoria,temperatura,temperatura->factor);
  //INICIA LA HEURISTICA:
  RUTA *result = aceptacion_por_umbrales(temperatura,ruta_inicial_aleatoria);
  imprime_ruta(result);
  
  return 0;
} //Fin de main.c
