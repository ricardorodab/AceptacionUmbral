/* -------------------------------------------------------------------
 * ciudad.c
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
 * @file ciudad.c
 * @author Jose Ricardo Rodriguez Abreu
 * @date 1 Apr 2017
 * @brief File containing the struct and funtions to emulate cities
 * for the "Combinatorial Optimization Heuristics" class. 
 *
 * En este archivo implementamos las funciones definidas para hacer
 * manejo de las estructuras definidas como CIUDAD, su manejo en 
 * memoria y la de los atributos que constituye su estructura.
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
#include "ciudad.h"

/**
 * Utiliza el metodo de pasar la direccion de memoria de la
 * estructura para inicializarla junto con el conjunto de datos
 * que debe tener la estructura. Tambien inicializa el diccionario
 * que contendra a los vecinos de cada ciudad.
 * @warning -Debe llamarse a delete_ciudad si se desea eliminar.
 *
 */
void init_ciudad(CIUDAD *ciudad, int id,
		 char *name, char *country,
		 int population, double latitude,
		 double longitude)
{  
  ciudad->id = id;
  ciudad->name = name;
  ciudad->country = country;
  ciudad->population = population;
  ciudad->latitude = latitude;
  ciudad->longitude = longitude;
  ciudad->vecinos =
    g_hash_table_new_full(g_int_hash,g_int_equal,free,free);
}


/**
 * Se llama esta funcion para quitar a el objeto ciudad colocado
 * de manera dinamica con la funcion @init_ciudad. Se debe llamar
 * solo con aquellas ciudades que se conoce se encuentren en memoria.
 *
 */
void delete_ciudad(CIUDAD *ciudad)
{
  if(ciudad->vecinos != NULL)
    g_hash_table_destroy(ciudad->vecinos);
  free(ciudad);
}

/**
 * Esta funcion es necesaria para reducir el codigo demasiado verboso 
 * de hacer lookups a los diccionarios de una ciudad. 
 * Recordar que los vecinos se almacenan de forma de tuplas:
 * Vecinos = (Ciudad,distancia). Si no existiera el vecino regresa
 * el valor de NULL.
 *
 */
CIUDAD* get_vecino(CIUDAD *ciudad, int id)
{
  if(g_hash_table_contains(ciudad->vecinos, &id)){
    CIUDAD_VECINO *tupla = g_hash_table_lookup(ciudad->vecinos,&id);
    return tupla->ciudad;
  }
  return NULL;
}

/**
 * Inicializamos las tuplas de forma (Ciudad, distancia) para agregar
 * a los vecinos de una ciudad a su diccionario.
 */
CIUDAD_VECINO* init_ciudad_vecino(CIUDAD *vec,double dist)
{
  CIUDAD_VECINO *tupla = malloc(sizeof(CIUDAD_VECINO));
  tupla->ciudad = vec;
  tupla->distancia = dist;
  return tupla;
}

/**
 * Evitamos tener que escribir codigo de vecindad fuera del archivo
 * en cuestion. Ademas que se utiliza en varias ocasiones en codigo
 * repetitivo.
 *
 */
bool son_vecinos(CIUDAD *ciudad, CIUDAD *vecino)
{
  int *id = &vecino->id;
  return g_hash_table_contains(ciudad->vecinos,id);
}

/**
 * Al tener que asignarle memoria de manera dinamica a las tuplas de los 
 * vecinos, hay que eliminar las tuplas de la memoria igual de manera manual.
 *
 */
void delete_ciudad_vecino(CIUDAD_VECINO* tupla)
{
  free(tupla);
}

/**
 * Para mejorar la lectura del codigo, separamos la creacion de las tuplas
 * de la asignacion de esta en los diccionarios de cada uno de los vecinos.
 * Notece que para cada asignacion de vecinos, los vecinos tambien agregan
 * a las ciudades origen.
 * @see init_ciudad_vecino
 *
 */
void add_vecino(CIUDAD *ciudad, CIUDAD *vecino,double distancia)
{
  GHashTable *ciudad_vecino = ciudad->vecinos;
  GHashTable *vecino_vecino = vecino->vecinos;
  //Primer aproach con un apuntador de double:
  //double *dist = g_malloc(sizeof(double));
  //memcpy(dist,&distancia,sizeof(double)); 
  CIUDAD_VECINO *tupla_uno = init_ciudad_vecino(vecino,distancia);
  CIUDAD_VECINO *tupla_dos = init_ciudad_vecino(ciudad,distancia);
  //Las llaves deben de ser almacenadas en el heap para no perderlas.
  int *key_uno = malloc(sizeof(int*));
  int *key_dos = malloc(sizeof(int*));
  *key_uno = vecino->id;
  *key_dos = ciudad->id;
  g_hash_table_insert(ciudad_vecino,key_uno,tupla_uno);
  g_hash_table_insert(vecino_vecino,key_dos,tupla_dos);
}

/**
 * Para ahorrar repeticion y codigo y hacerlo mas legible, esta funcion
 * llama a la tupla de una de las ciudades (es indistinguible cual puesto
 * que ambas ciudades tienen la distancia) y devuelve el double de la 
 * tupla propuesto.
 * @warning Regresa -1 si no existe la conexion.
 */
double get_distancia_ciudad(CIUDAD *ciudad_origen, CIUDAD *ciudad_destino)
{
  int *id = &ciudad_destino->id;
  if(g_hash_table_contains(ciudad_origen->vecinos,id)){
    CIUDAD_VECINO *tupla = g_hash_table_lookup(ciudad_origen->vecinos,id);
    return tupla->distancia;
  }
  return -1;
}

/**
 * Con propositos de resolver errores y molestos segmentation faults 
 * decidi crear esta funcion para ir rastreando errores de impresion.
 */
void imprime_ciudad(CIUDAD *ciudad)
{
  printf("\nId = %d \n",ciudad->id);
  printf("Name = %s \n",ciudad->name);
  printf("Country = %s \n",ciudad->country);
  printf("Population = %d \n",ciudad->population);
  printf("Latitude = %f \n",ciudad->latitude);
  printf("Longitude = %f \n\n",ciudad->longitude);
} //Fin de ciudad.c
