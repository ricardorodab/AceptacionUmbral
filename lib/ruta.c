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
  set_distancias(ruta);
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
  ruta->AVG = 0;
  ruta->num_ciudades = size;
  ruta->ciudades_desconectadas = 0;
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
  vecino->ciudades = NULL;
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
void set_distancias(RUTA *ruta)
{
  CIUDAD *ciudad_i, *ciudad_j;
  int key_1,key_2;
  double distancia_max = 0;  
  double distancia = 0;
  int aristas = 0;
  int ciudades_desconectadas = ruta->num_ciudades-1;
  double AVG = 0;
  int i,j;
  for(i = 0; i < ruta->num_ciudades; i++){
    for(j = i+1; j < ruta->num_ciudades; j++){
      key_1 = g_array_index(ruta->arreglo,gint,i);
      key_2 = g_array_index(ruta->arreglo,gint,j);
      //Buscamos las ciudades:
      ciudad_i = g_hash_table_lookup(ruta->ciudades,&key_1);
      ciudad_j = g_hash_table_lookup(ruta->ciudades,&key_2);
      //Revisamos que sean vecinas: 
      if(son_vecinos(ciudad_i,ciudad_j)){
	AVG += get_distancia_ciudad(ciudad_i,ciudad_j);
	aristas++;
	if(get_distancia_ciudad(ciudad_i,ciudad_j) > distancia_max)
	  distancia_max = get_distancia_ciudad(ciudad_i,ciudad_j);
      }
    }
  }
  for(i = 0; i < ruta->num_ciudades-1; i++){
    j = i+1;
    key_1 = g_array_index(ruta->arreglo,gint,i);
    key_2 = g_array_index(ruta->arreglo,gint,j);
    //Buscamos las ciudades:
    ciudad_i = g_hash_table_lookup(ruta->ciudades,&key_1);
    ciudad_j = g_hash_table_lookup(ruta->ciudades,&key_2);
    if(son_vecinos(ciudad_i,ciudad_j)){
      distancia += get_distancia_ciudad(ciudad_i,ciudad_j);
      ciudades_desconectadas--;
    }    
  }
  ruta->distancia = distancia;
  ruta->ciudades_desconectadas = ciudades_desconectadas;
  ruta->AVG = AVG/aristas;
  ruta->distancia_max = distancia_max;
}

/**
 * AUX
 *
 */
void quita_ciudad(RUTA *ruta, int id_city_array, CIUDAD *quitar)
{
  CIUDAD *ciudad_temp;
  int key;
  int j,k;
  double distancia = ruta->distancia;
  int ciudades_desconectadas = ruta->ciudades_desconectadas;
  j = id_city_array+1;
  k = id_city_array-1;
  if(k > 0){
    key = g_array_index(ruta->arreglo,gint,k);
    ciudad_temp = g_hash_table_lookup(ruta->ciudades,&key);
    if(son_vecinos(quitar,ciudad_temp)){
      ciudades_desconectadas++;
      distancia -= get_distancia_ciudad(quitar,ciudad_temp);
    }
  }
  if(j < ruta->num_ciudades){
    key = g_array_index(ruta->arreglo,gint,j);
    ciudad_temp = g_hash_table_lookup(ruta->ciudades,&key);
    if(son_vecinos(quitar,ciudad_temp)){
      ciudades_desconectadas++;
      distancia -= get_distancia_ciudad(quitar,ciudad_temp);
    }    
  }  
  ruta->distancia = distancia;
  ruta->ciudades_desconectadas = ciudades_desconectadas;  
}

/**
 * AUX
 *
 */
void agrega_ciudad(RUTA *ruta, int id_city_array, CIUDAD *poner)
{
  CIUDAD *ciudad_temp;
  int key;
  int i,j,k;
  double distancia = ruta->distancia;
  int ciudades_desconectadas = ruta->ciudades_desconectadas;
  j = id_city_array+1;
  k = id_city_array-1;
  if(k > 0){
    key = g_array_index(ruta->arreglo,gint,k);
    ciudad_temp = g_hash_table_lookup(ruta->ciudades,&key);
    if(son_vecinos(poner,ciudad_temp)){
      ciudades_desconectadas--;
      distancia += get_distancia_ciudad(poner,ciudad_temp);
    }
  }
  if(j < ruta->num_ciudades){
    key = g_array_index(ruta->arreglo,gint,j);
    ciudad_temp = g_hash_table_lookup(ruta->ciudades,&key);
    if(son_vecinos(poner,ciudad_temp)){
      ciudades_desconectadas--;
      distancia += get_distancia_ciudad(poner,ciudad_temp);
    }    
  }  
  ruta->distancia = distancia;
  ruta->ciudades_desconectadas = ciudades_desconectadas;  
}



void agrega_y_quita_repetido_ciudad(RUTA *ruta,
				    int id_city_array,
				    int id_city_array_vecino,
				    CIUDAD *poner)
{

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
 * @TODO C - Rehacer la funcion.. y mis esperanzas
 */
void recalcula_distancia(RUTA *ruta, int ii, int jj)
{
  CIUDAD *ciudad_i, *ciudad_j;
  int key_1,key_2;
  key_1 = g_array_index(ruta->arreglo,gint,ii);
  key_2 = g_array_index(ruta->arreglo,gint,jj);
  //Buscamos las ciudades:
  ciudad_i = g_hash_table_lookup(ruta->ciudades,&key_1);
  ciudad_j = g_hash_table_lookup(ruta->ciudades,&key_2);
  if(ii-1 == jj || ii+1 == jj){
    quita_ciudad(ruta,jj,ciudad_i);
    agrega_ciudad(ruta,jj,ciudad_j);
  }else{
    quita_ciudad(ruta,ii,ciudad_i);
    quita_ciudad(ruta,jj,ciudad_j);
    agrega_ciudad(ruta,ii,ciudad_j);
    agrega_ciudad(ruta,jj,ciudad_i);
    }
}


/**
 * La definicion de ruta vecina en esta funcion es la siguiente:
 * Dos rutas son vecinas si una ciudad i es intecambiada por j
 * con i y j diferentes. Obtenemos dos numeros aleatorios
 * i y j, creamos una @RUTA completamente nueva con los mismos
 * parametros que la recibimos de parametros por excepcion de la distancia
 * y el arreglo ya que este sera cambiado por el swap de i y j.
 *
 * @TODO Mejorar complejidad en lugar de recalcular toda la ruta
 * mejor solo restar [(i,(i+1))+((i-1),i)+(j,(j+1))+((j-1),j)] 
 * y sumarle [(j,(i+1))+((i-1),j)+(i,(j+1))+((j-1),i)]
 */
RUTA* get_ruta_vecina(RUTA* ruta)
{
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
  vecino->AVG = ruta->AVG;
  vecino->peso_desconexion = ruta->peso_desconexion;
  GArray *muestra = g_array_sized_new(FALSE,TRUE,sizeof(gint),size);
  int i;
  for(i = 0; i < size; i++)
    {
      //if(i == r_i){
      //g_array_append_val(muestra,g_array_index(ruta->arreglo,gint,r_j));
      //}else if(i == r_j){
      //g_array_append_val(muestra,g_array_index(ruta->arreglo,gint,r_i));
      //}else{
      g_array_append_val(muestra,g_array_index(ruta->arreglo,gint,i));
      //}
    }  
  vecino->arreglo = muestra;
  
  int temp = g_array_index(vecino->arreglo,gint,r_i);
  g_array_index(vecino->arreglo,gint,r_i) =
    g_array_index(vecino->arreglo,gint,r_j);
  g_array_index(vecino->arreglo,gint,r_j) = temp;

  recalcula_distancia(vecino,r_i,r_j);
  
  return vecino;
}

/**
 * Con propositos de resolver errores y molestos segmentation faults 
 * decidi crear esta funcion para ir rastreando errores de impresion.
 */
void imprime_ruta(RUTA* ruta)
{
  int i;
  int ruta_id[ruta->num_ciudades];
  int *key = malloc(sizeof(int));
  for(i = 0; i < ruta->num_ciudades; i++)
    {
      *key = (g_array_index(ruta->arreglo,gint,i));
      ruta_id[i] = *key;
      CIUDAD *temp = g_hash_table_lookup(ruta->ciudades,key);
    }
  for(i = 0; i < ruta->num_ciudades-1; i++)
    {
      printf("%d->",ruta_id[i]);
    }
  printf("%d\n",ruta_id[i]);
  free(key);
}

void imprime_gps(RUTA* ruta)
{
  int i;
  int ruta_id[ruta->num_ciudades];
  int *key = malloc(sizeof(int));
  for(i = 0; i < ruta->num_ciudades; i++)
    {
      *key = (g_array_index(ruta->arreglo,gint,i));
      ruta_id[i] = *key;
      CIUDAD *temp = g_hash_table_lookup(ruta->ciudades,key);
      printf("%f,",temp->latitude);
      printf("%f\n",temp->longitude);
    }
  free(key);
}
