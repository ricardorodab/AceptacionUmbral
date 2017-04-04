/* -------------------------------------------------------------------
 * ruta.h
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

#ifndef RUTA_H
#define RUTA_H
#include <glib.h>
#include <stdbool.h>
#include "ciudad.h"


/**
 * @file ruta.h
 * @author Jose Ricardo Rodriguez Abreu
 * @date 1 Apr 2017
 * @brief File containing the struct and funtions headers for a path
 * for the "Combinatorial Optimization Heuristics" class. 
 *
 * En este archivo definimos las funciones y estructuras para hacer
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
 * @brief Es un equivalente a los paths en teoria de graficas.
 *
 * Esta estructura define al conjunto de soluciones que puede
 * contener un camino Hamiltoneano. Esto conlleva cierto orden en el que
 * deben estar acomodadas las ciudades para demostrar una distancia
 * ya que la tupla (j,i) + (i,k) != (i,k) +(k,j) por lo que tenemos
 * una estructura para las ciudades y otra completa para definir el orden
 * y el tamanio de la ruta junto con datos convenientes de acceso O(1).
 *
 */
typedef struct ruta_estructura {
  double distancia; /**< Es la distancia total de la ∑ d(i,j). */
  int num_ciudades; /**< Es el numero total de las ciudades.|V(P)| P=Path. */
  GHashTable *ciudades; /**< Es el conjunto de ciudades que pertenece a P. */
  GArray *arreglo; /**< Esta estructura nos da el orden i->i+1->i+2... */
  double distancia_max; /**< Almacenamos esta variable para hacer calculos */
  int ciudades_desconectadas; /**< Es el numero de ciudades desconectadas. */
  double peso_desconexion; /**< Es el peso que se le da a descinexiones. */
} RUTA;

/**
 * @brief Inicializa los valores de la ruta. Incluyendo sus ciudades.
 *
 * @param cities -Un hash table donde vienen ya las estructuras ciudades
 * (sus apuntadores) con la informacion de cada una de ellas.
 * @param muestra -Es la lista (arreglo) con las ciudades que nos interesan.
 * @param size -Es el tamanio de la muestra que aunque calculable, por 
 * cuestiones de implementacion preferimos pasarla como parametro.
 * @param peso_desconexion -Lo usamos como una constante para pesos.
 * @return Una ruta aleatoria colocada en memoria de manera dinamica.
 * @see destroy_ruta
 *
 */
RUTA* init_ruta(GHashTable *cities, GArray *muestra,
		int size, double peso_desconexion);

/**
 * @brief Funcion que nos regresa un booleano que nos dice si una ciudad
 * se encuentra en la ruta dada.
 *
 * @param ruta -Es la ruta o path que vamos a inspeccionar elemento 1x1.
 * @param ciudad -Es la ciudad que queremos ver en la ruta.
 * @return true si es que la ciudad @ciudad esta en la ruta @ruta.
 */
bool is_in_ruta(RUTA *ruta, CIUDAD* ciudad);

/**
 * @brief Elimina de memoria un vecino que previamente fue creada de manera
 * dinamica. A diferencia de @destroy_ruta no elimina el diccionario.
 *
 * @param vecino -Es la ruta que en algun momento fue alojada con @init_ruta.
 * @see destroy_ruta
 */
void destroy_vecino(RUTA *vecino);

/**
 * @brief Elimina de memoria una ruta que previamente fue creada de manera
 * dinamica.
 *
 * @param ruta -Es la ruta que en algun momento fue alojada con @init_ruta.
 * @see destroy_ruta
 */
void destroy_ruta(RUTA *ruta);

/**
 * @TODO COMMENT:
 */
void set_distancia_maxima_ruta(RUTA *ruta);

/**
 * @brief Dada una ruta y un peso maximo, calcula el la distancia de una ruta.
 *
 * @param ruta -Es la ruta que queremos obtener la distancia.
 * @peso_desconexion -Este lo ocupamos para asignale peso a
 * ciudades desconectada.
 *
 */
void recalcula_distancia(RUTA *ruta);

/**
 * @brief Obtenemos una solucion vecina a la ruta actual intercambiando dos
 * ciudades de orden.
 *
 * @param ruta -Es la ruta original, a partir de ella intercambiaremos de 
 * lugar a dos ciudades para obtener una nueva solucion.
 * @return Una nueva ruta que llamaremos "Solucion vecina" a la ruta original.
 *
 */
RUTA* get_ruta_vecina(RUTA* ruta);

/**
 * @brief Transforma una estructura ruta en caracteres e imprime en consola
 *
 * @param ruta -Es la ruta que queremos observar sus datos en pantalla.
 *
 */
void imprime_ruta(RUTA* ruta);
#endif //Fin de ruta.h
