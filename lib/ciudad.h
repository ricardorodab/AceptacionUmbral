/* -------------------------------------------------------------------
 * ciudad.h
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
 * @file ciudad.h
 * @author Jose Ricardo Rodriguez Abreu
 * @date 1 Apr 2017
 * @brief File containing the struct and funtions to simulate cities
 * for the "Combinatorial Optimization Heuristics" class. 
 *
 * En este archivo definimos la estructura ciudad y ciudad_vecino junto a 
 * la firma de funciones que se hacen uso de las estructuras y
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
#ifndef CIUDAD_H
#define CIUDAD_H
#include <glib.h>

/**
 * @brief La estructura ciudad contiene datos basicos de ella.
 *
 * Usamos esta estructura para mantener unidos los datos que contiene
 * una ciudad como nombre, pais, ..., y le asignamos un id para
 * identificarla de manera univocua.
 *
 */
typedef struct ciudad_estructura {
  int id; /**< Es el id que representa a la ciudad, en teoria unico. */
  char *name; /**< Es el nombre en caracteres de la ciudad. */
  char *country; /**< Es el nombre en caracteres del pais. */
  int population; /**< Es la poblacion que tiene esta ciudad. */
  double latitude; /**< Es la latitude geolocal de la ciudad. */
  double longitude; /**< Es la longitud geolocal de la ciudad. */
  GHashTable *vecinos; /**< Un diccionario con las tuplas de ciudad_vecinos*/
} CIUDAD;

/**
 * @brief Esta estructura se crea para facilitar el almacenamiento de vecinos
 *
 * Al tener que estar constantemente llenando los vecinos con ciudades
 * constantemente se decidio el diseno de mantener a ambos elementos
 * (los vecinos y la distancia) como una tupla para su almacenamiento en un
 * diccionario que contiene cada una de las ciudades.
 *
 */
typedef struct ciudad_vecinos {
  CIUDAD *ciudad; /**< Es la ciudad que representa a el vecino. */
  double distancia; /**< Es la distancia en km (supongo) entre ciudades.  */
} CIUDAD_VECINO;

/**
 * @brief Metodo de inicializacion de estructura para su almacenamiento en 
 * la memoria dinamica (heap).
 *
 * @param ciudad -Es el apuntador de la ciudad que se quiere inicializar.
 * @param id -Es el identificador (en principio unico) de la ciudad.
 * @param name -Es el nombre de la ciudad que se quiere inicializar en char*.
 * @param country -Es el nombre del pais de la ciudad. Debe estar en char*
 * @param population -Representa a la poblacion y por lo mismo no hay .5.
 * @param latitude -Es la posicion geografica de la ciudad en latitud.
 * @param longitude -Es la posicion geografica de la ciudad en longitud.
 *
 */
void init_ciudad(CIUDAD *ciudad, int id,
		    char *name, char *country,
		    int population, double latitude,
		    double longitude);

/**
 * @brief Cuando se desee desechar una ciudad se debe quitar de manera
 * dinamica, justo como fue instanciada en la memoria.
 *
 * @param ciudad -Es la ciudad que queremos quitar del heap.
 *
 */
void delete_ciudad(CIUDAD *ciudad);

/**
 * @brief Facilitam la obtencion de una ciudad dado un id y su vecino.
 *
 * @param ciudad -Es la ciudad vecina que tiene a su ciudad origen.
 * @param id -Es el identificador (en principio unico) de la ciudad a buscar.
 * @return La ciudad vecina con id @id y que es vecino de @ciudad.
 *
 */
CIUDAD* get_vecino(CIUDAD *ciudad, int id);

/**
 * @brief Inicizaliza la tupla que sera vecinos de las ciudades.
 *
 * @param vec -Es la ciudad que contendra la tulpa. Generalmente sera la
 * ciudad vecina de otra ciudad.
 * @param dist -Es la distancia que contendra la tupla. Es simetrica y
 * la misma tendra la tupla de la ciudad vecina.
 * @see delete_ciudad_vecino.
 * @return Una tupla conformada por una ciudad y un vecino.
 *
 */
CIUDAD_VECINO* init_ciudad_vecino(CIUDAD *vec,double dist);

/**
 * @brief Facilita la liberacion de memoria de la tupla que contendra vecinos
 *
 * @param tupla -Es la tupla que se desea liberar de la memoria incializada 
 * con la funcion @init_ciudad_vecino.
 * @see init_ciudad_vecino.
 *
 */
void delete_ciudad_vecino(CIUDAD_VECINO* tupla);

/**
 * @brief Sirve para agregar un vecino en una de los diccionarios que 
 * contiene cada uno de las ciudades en su estructura.
 *
 * @param ciudad -Es una de la ciudades que se agregara su vecino.
 * @param vecino -Es la otra ciudad que se agregará su vecino.
 * @param distancia -Es la distancia que existe entre @ciudad y @vecino.
 *
 */
void add_vecino(CIUDAD *ciudad, CIUDAD *vecino, double distancia);

/**
 * @brief Funcion que nos regresa la distancia que contiene la tupla que 
 * tiene cada uno de los vecinos definidos en su vecindad.
 *
 * @param ciudad_origen -Sin perdida de generalidad es una de las ciudades.
 * @param ciudad_destino -Es la segunda ciudad a sacar el destino que existe.
 * @return La distancia > 0 que se encuentra en las tuplas de los vecinos 
definido por las dos ciudades. Si no existe la conexion regresa -1.
 *
 */
double get_distancia_ciudad(CIUDAD *ciudad_origen, CIUDAD *ciudad_destino);

/**
 * @brief Transforma una estructura ciudad en caracteres e imprime en consola
 *
 * @param ciudad -Es la ciudad que queremos observar sus datos en pantalla.
 *
 */
void imprime_ciudad(CIUDAD *ciudad);
#endif //Fin de ciudad.h
