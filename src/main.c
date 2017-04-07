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
#include <ctype.h>
#include <time.h> 
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
  char *err = "-Linea:";
  printf("%s%s%d\n",msg,err,linea);
  exit(1);
}

/**
 * @brief Manejador y parseador del archivo donde viene la muestra.
 *
 * Al requerir una muestra con la que trabajar, nuestro programa 
 * necesita almacenar esta muestra en un archivo para facilitar las
 * pruebas y experimentacion del programa.
 * @param ubicacion_muestra -Es el nombre del archivo donde viene la muestra.
 * @param muestra_size -Es el tamanio que debe contener nuestra muestra.
 * @return Un arreglo GArray con los indices de nuestras muestras.
 *
 */
GArray* lee_muestra(char *ubicacion_muestra,int *muestra_size)
{
  GArray *muestra;
  //Esto esta feo pero es rapido y facilita el parseo:
  GList *entrada = NULL;
  int linea;
  char char_file[1];
  char numero[80];
  FILE *file;
  int int_file;
  file = fopen(ubicacion_muestra,"r+");
  //Revisamos si el archivo abrio bien.
  if(file == NULL){
    main_imprime_error("El archivo no existe o existe un problema al abrirlo.\n",__LINE__);
  }
  while((int_file = fscanf(file,"%c",char_file)) != -1)
    {
      //Caso especial: Un comentario empezando por '#'
      if(char_file[0] == '#'){
	int comment_int = 0;       
	while(((comment_int = fscanf(file,"%c",char_file)) != -1)
	      && char_file[0] != '\n'){}
	if(comment_int == -1)
	  break;
      }
      char *index = malloc(sizeof(char));
      *index = char_file[0];
      entrada = g_list_append(entrada,index);
    }
  linea = 0;
  int i = 0;
  int cant_num = 0;
  //Inicializamos nuestro arreglo de muestras de tamanio *muestra_size
  muestra = g_array_sized_new (FALSE, FALSE, sizeof(gint),*muestra_size);
  GList *l;
  for(l = entrada; l != NULL; l = l->next)
    {
      char *data = (l->data);
      //Caso linea 0 -> numero de muestras:
      if(linea == 0){
	if(*data == '\n'){
	  linea++;
	  numero[i] = '\0';
	  *muestra_size = atoi(numero);
	  i = 0;
	}else if(isdigit(*data)){
	  numero[i] = *data;
	  i++;
	}	
      }else{
	if(*data == ','){
	  numero[i] = '\0';
	  int *val = malloc(sizeof(int));
	  *val = atoi(numero);
	  g_array_append_val(muestra,*val);
	  cant_num++;
	  i = 0;
	}else if(l->next == NULL){
	  numero[i] = *data;
	  numero[i+1] = '\0';	  
	  int *val = malloc(sizeof(int));
	  *val = atoi(numero);
	  g_array_append_val(muestra,*val);
	  cant_num++;
	  i = 0;
	}else if(isdigit(*data)){
	  numero[i] = *data;
	  i++;
	}
      }
    }
  if(cant_num != *muestra_size)
    main_imprime_error("No coincide el numero de muestras con estas!",
		       __LINE__);
  return muestra;
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
  int *key = malloc(sizeof(int));
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
  GArray *muestra;
  int muestra_size;
  //A leer de externo ./etc/config.cfg:
  char *UBICACION_MUESTRA;
  char *UBICACION_BASE;
  double P_FACTOR_CAMBIO;
  double TEMPERATURA_INICIAL;
  double PESO_DESCONEXION;
  double EPSILON_EQUILIBRIO;
  double EPSILON_TEMP;
  double L_ITERACIONES;

  //Inicializamos la semilla aleatoria (@TODO - Cambiar semilla)
  srand(time(NULL));
  
  //Leemos el archivo de configuracion:
  GKeyFile *keyfile;
  GKeyFileFlags flags;
  GError *error = NULL;
  keyfile = g_key_file_new ();
  flags = G_KEY_FILE_NONE;
  if (!g_key_file_load_from_file (keyfile,
				  "./etc/config.cfg",
				  flags, &error))
    {
      printf("Error al cargar archivo de configuracion. LINEA= %d",__LINE__);
      return 1;
    }
  
  //Ahora asignamos a cada una de las variables sus valores del .cfg:
  // 1. Primero la base de datos:
  UBICACION_BASE = g_key_file_get_string(keyfile,"DATABASE","UBICACION",NULL);
  // 2. Variables de experimentacion:
  TEMPERATURA_INICIAL = g_key_file_get_double(keyfile,"HEURISTICA",
					      "TEMPERATURA_INICIAL",NULL);
  L_ITERACIONES       = g_key_file_get_double(keyfile,"HEURISTICA",
					      "L_ITERACIONES",NULL);
  EPSILON_TEMP        = g_key_file_get_double(keyfile,"HEURISTICA",
					      "EPSILON_TEMP",NULL);
  EPSILON_EQUILIBRIO  = g_key_file_get_double(keyfile,"HEURISTICA",
					      "EPSILON_EQUILIBRIO",NULL);
  P_FACTOR_CAMBIO     = g_key_file_get_double(keyfile,"HEURISTICA",
					      "P_FACTOR_CAMBIO",NULL); 
  PESO_DESCONEXION    = g_key_file_get_double(keyfile,"HEURISTICA",
					      "PESO_DESCONEXION",NULL);
  // 3. Por ultimo el nombre donde esta la muestra:
  UBICACION_MUESTRA = g_key_file_get_string(keyfile,"MUESTRA",
					    "UBICACION",NULL);
  
  //En la variable cities se encuentran toda la informacion de las ciudades.
  cities = g_hash_table_new_full(g_int_hash,
				 g_int_equal,
				 free,free);
  //Realizamos la conexion a la base y poblamos nuestro hash.
  //Seguido de eso poblamos las conexiones de nuestras ciudades.
  if(get_query(UBICACION_BASE,QUERY1,db,(void*)cities,init_ciudades) ||
     get_query(UBICACION_BASE,QUERY2,db,(void*)cities,conecta_ciudades)){
    main_imprime_error("Error al conectar las ciudades.",LINEA-2);
  }
  // AQUI TENEMOS QUE INICIALIZAR Y LEER LA MUESTRA;
  muestra = lee_muestra(UBICACION_MUESTRA,&muestra_size);
  // Creamos la ruta y aleatorizamos a la ruta.
  RUTA *ruta_inicial_aleatoria = init_ruta(cities,muestra,
					   muestra_size,PESO_DESCONEXION);
  // Creamos una temperatura con los parametros del archivo de config.cfg
  TEMPERATURA *temperatura = init_temperatura(TEMPERATURA_INICIAL,
					      P_FACTOR_CAMBIO,
					      EPSILON_TEMP,
					      EPSILON_EQUILIBRIO);
  imprime_ruta(ruta_inicial_aleatoria);
  printf("INICIAL:%f\n",temperatura->valor);
  //temperatura_inicial(ruta_inicial_aleatoria,temperatura,P_FACTOR_CAMBIO);
  temperatura->valor = 8;  
  printf("FINAL:%f\n",temperatura->valor);
  //INICIA LA HEURISTICA:
  RUTA *result = aceptacion_por_umbrales(temperatura,
					 ruta_inicial_aleatoria,
					 L_ITERACIONES);
  imprime_ruta(result);
  printf("FUNCION:%f\n",funcion_costo(result));
  char *factible = result->ciudades_desconectadas == 0 ? "SI" : "NO";
  printf("Solucion factible: %s\n",factible);
  if(result->ciudades_desconectadas > 0)
    printf("El numero de desconexidades es de :%d\n",result->ciudades_desconectadas);
  imprime_gps(result);
  return 0; 
} //Fin de main.c

