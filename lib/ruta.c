/* -------------------------------------------------------------------
 * ruta.c
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

#include "ruta.h"
#include <stdlib.h>
#include <time.h> 
#include <stdio.h>

/**
 * @file ruta.c
 * @author Jose Ricardo Rodriguez Abreu
 * @date 1 Apr 2017
 * @brief File containing the struct and funtions for a path of cities
 * for the "Combinatorial Optimization Heuristics" class. 
 *
 * En este archivo implementamos las funciones definidas para hacer
 * manejo de las estructuras definidas como RUTA, su manejo en 
 * memoria y la de los atributos que constituye su estructura.
 *
 * El programa usa el estandar de documentacion que define el uso de 
 * doxygen.
 *
 * @see http://www.stack.nl/~dimitri/doxygen/manual/index.html
 * @see https://github.com/ricardorodab/AceptacionUmbral
 *
 */



/**
 * @brief Es la funcion auxiliar que nos ayuda a crear un path aleatorio.
 *
 * Esta funcion primero hace consultas sobre todas las posibles ciudades
 * que contiene los diccionarios que tienen de parametros @cities. 
 * Crea un path aleatorio a partir de las ciudades que incluye el path
 * "ordenado" por el usuario en el parametro @muestra.
 * @param ruta -Es la ruta que sera aleatoria que queremos terminar de crear.
 * @param cities -Es el diccionario que contiene todas las posibles ciudades.
 * @param muestra -Son las muestras de ciudades con las que trabajaremos.
 * @param size -Es el tamanio de las muestras con las que trabajaremos.
 * @see init_ruta
 * @TODO -Falta implementar cuando no estan unidos dos caminos.
 * @TODO -Modificar random para que sea determinista dado una semilla.
 */
void insert_ciudades_aux(RUTA *ruta, GHashTable *cities,
			 GArray *muestra, int size){
  //GArray *arreglo = g_array_sized_new(FALSE,FALSE,sizeof(int),size); 
  CIUDAD *temp;
  int *key;
  int r = 0;
  srand(time(NULL));
  //Iteraremos la lista aleatoriamente para generar una ruta aleatoria:
  while(size > 0)
    {      
      r = rand() % size;
      key = malloc(sizeof(int));
      *key = g_array_index(muestra,gint,r);
       temp = g_hash_table_lookup(cities,key);
      g_hash_table_insert(ruta->ciudades,key,temp);
      ruta->arreglo = g_array_append_val(ruta->arreglo,*key);
      g_array_remove_index(muestra,r);
      size--;      
    }
  set_distancia_maxima_ruta(ruta);
  recalcula_distancia(ruta);
}

/**
 * Para inicializar a las rutas, necesitamos un hash donde incluye todas
 * las ciudades disponibles para poder cear el path y la muestra que
 * queremos tenga el path. 
 * El trabajo verdadero lo hace la funcion @insert_ciudades_aux sin
 * embargo, aqui instanciamos la ruta y le damos algunos valores.
 *
 * @see insert_ciudades_aux
 */
RUTA* init_ruta(GHashTable *cities, GArray *muestra,
		int size, double peso_desconexion)
{
  RUTA *ruta = malloc(sizeof(RUTA));
  ruta->distancia = 0;
  ruta->peso_desconexion = peso_desconexion;
  ruta->distancia_max = 0;
  ruta->num_ciudades = size;
  ruta->ciudades = 
    g_hash_table_new_full(g_int_hash,g_int_equal,free,free);
  ruta->arreglo = g_array_sized_new(FALSE,TRUE,sizeof(gint),size);
  insert_ciudades_aux(ruta,cities,muestra,size);
  return ruta;
}

/**
 * Para facilidad del programador, creamos esta funcion que nos regresa
 * un valor booleano y evita que tenga que escribir lineas repetitivas.
 *
 */
bool is_in_ruta(RUTA *ruta, CIUDAD* ciudad)
{
  if(g_hash_table_contains(ruta->ciudades, &(ciudad->id)))
    return true;
  return false;
}

/**
 * Separamos esta funcion en dos partes: La parte que libera la memoria
 * del arreglo y la parte que libera a el apuntador de la ruta.
 * no eliminamos el apuntador del diccionario puesto que algun vecino
 * podria estar usandolo.
 *
 */
void destroy_vecino(RUTA *vecino){
  g_array_free(vecino->arreglo,TRUE);
  free(vecino);
}

/**
 * Liberamos la memoria colocada de manera dinamica totalmente para
 * evitar que se llene con rutas alternas. Liberamos el arreglo, 
 * el diccionario y a la ruta misma.
 *
 */
void destroy_ruta(RUTA *ruta)
{
  if(ruta->arreglo != NULL)
    g_array_free(ruta->arreglo,TRUE);
  if(ruta->ciudades != NULL)
    g_hash_table_destroy(ruta->ciudades);  
  free(ruta);
}

/**
 * @TODO COMMENT:
 */
void set_distancia_maxima_ruta(RUTA *ruta)
{
  CIUDAD *ciudad_i, *ciudad_j;
  int key_1,key_2;
  double distancia_max = 0;
  int i,j;
  int hay_desconectados = 0;
  for(i = 0; i < ruta->num_ciudades; i++){
    for(j = i+1; j < ruta->num_ciudades; j++){
      key_1 = g_array_index(ruta->arreglo,gint,i);
      ciudad_i = g_hash_table_lookup(ruta->ciudades,&key_1);
      key_2 = g_array_index(ruta->arreglo,gint,j);
      ciudad_j = g_hash_table_lookup(ruta->ciudades,&key_2);
      if(son_vecinos(ciudad_i,ciudad_j)){
	if(get_distancia_ciudad(ciudad_i,ciudad_j) > distancia_max)
	  distancia_max = get_distancia_ciudad(ciudad_i,ciudad_j);
      }else{
	hay_desconectados++;
      }
    }
  }
  if(hay_desconectados)
    ruta->distancia_max = distancia_max;
}

/**
 * Esta funcion calcula el peso que tiene una ruta que
 * le sea dada de la siguiente manera:
 * calculamos la ditancia de la ciudad i con la i+1 usando
 * la funcion @get_distancia_ciudad del archivo ciudad.h
 * Iteramos sobre el orden que tiene su arreglo interno.
 * Si dos ciudades no estan conectadas su valor de peso sera
 * la constante @peso_desconexion por el valor mayor de distancia.
 *
 * @TODO C -constante de max camino.
 */
void recalcula_distancia(RUTA *ruta)
{
  CIUDAD *temp, *vecino;
  int i,j;  
  double val_dist, no_conectados,dist;
  dist = 0;
  no_conectados = 0;
  //Ahora calculamos la distancia.
  for(i = 0; i < ruta->num_ciudades-1; i++)
    {
      j = i+1;
      int index_uno = (g_array_index(ruta->arreglo,gint,i));
      int index_dos = (g_array_index(ruta->arreglo,gint,j));      
      temp = g_hash_table_lookup(ruta->ciudades,&index_uno);
      vecino = g_hash_table_lookup(ruta->ciudades,&index_dos);      
      val_dist = get_distancia_ciudad(temp,vecino);
      //Si resulta un -1 o algo raro lo ignoramos y aumentamos desconectados.
      if(0 > val_dist) {
	no_conectados++;	
      }else{
        //Sumamos la distancia a la distancia total.
	//Revisamos que si la nueva distancia es la mayor la sustituimos.
	dist += val_dist;
      }    
    }
  ruta->ciudades_desconectadas = no_conectados;
  // MAX(P)X(C=2) si (u,v) no estan conectados.
  double no_conect;
  /**if(ruta->ciudades_desconectadas == 0)
     printf("Si es factible %d\n",ruta->ciudades_desconectadas);*/
  no_conect = (no_conectados*ruta->distancia_max*ruta->peso_desconexion);
  ruta->distancia = dist+no_conect;
}

/**
 * La definicion de ruta vecina en esta funcion es la siguiente:
 * Dos rutas son vecinas si una ciudad i es intecambiada por j
 * con i y j diferentes. Obtenemos dos numeros aleatorios
 * i y j, creamos una @RUTA completamente nueva con los mismos
 * parametros que la recibimos de parametros por excepcion de la distancia
 * y el arreglo ya que este sera cambiado por el swap de i y j.
 *
 * @TODO -Modificar la semilla de @srand
 * @TODO Mejorar complejidad en lugar de recalcular toda la ruta
 * mejor solo restar [(i,(i+1))+((i-1),i)+(j,(j+1))+((j-1),j)] 
 * y sumarle [(j,(i+1))+((i-1),j)+(i,(j+1))+((j-1),i)]
 */
RUTA* get_ruta_vecina(RUTA* ruta)
{
  srand(time(NULL));
  int r_i = 0;
  int r_j = 0;
  RUTA *vecino = malloc(sizeof(RUTA));
  int size = ruta->num_ciudades;  
  if(size < 2)
    return ruta;
  r_i = rand() % size;
  r_j = r_i;
  while(r_i == r_j)
    r_j = rand() % size;
  vecino->num_ciudades = size;
  vecino->ciudades = ruta->ciudades;
  vecino->distancia_max = ruta->distancia_max;
  vecino->peso_desconexion = ruta->peso_desconexion;
  GArray *muestra = g_array_sized_new(FALSE,TRUE,sizeof(gint),size);
  int i;
  for(i = 0; i < size; i++)
    {
      if(i == r_i){
	g_array_append_val(muestra,g_array_index(ruta->arreglo,gint,r_j));
      }else if(i == r_j){
	g_array_append_val(muestra,g_array_index(ruta->arreglo,gint,r_i));
      }else{
	g_array_append_val(muestra,g_array_index(ruta->arreglo,gint,i));
      }
    }  
  vecino->arreglo = muestra;
  recalcula_distancia(vecino);
  return vecino;
}

/**
 * Con propositos de resolver errores y molestos segmentation faults 
 * decidi crear esta funcion para ir rastreando errores de impresion.
 */
void imprime_ruta(RUTA* ruta)
{
  int i;
  for(i = 0; i < ruta->num_ciudades; i++)
    {
      int key = (g_array_index(ruta->arreglo,gint,i));      
      CIUDAD *temp = g_hash_table_lookup(ruta->ciudades,&key);
      imprime_ciudad(temp);
    }
}
