/* -------------------------------------------------------------------
 * lote.h
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
#ifndef LOTE_H
#define LOTE_H
#include "ruta.h"
#include "temperatura.h"
#include "funcion.h"

/**
 * @file lote.h
 * @author Jose Ricardo Rodriguez Abreu
 * @date 19 Apr 2017
 * @brief File containing the struct and funtions headers for a batch
 * for the "Combinatorial Optimization Heuristics" class. 
 *
 * En este archivo definimos las funciones y estructuras para hacer
 * manejo de las estructuras definidas como LOTE, su manejo en 
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
 * @brief Los lotes contienen rutas que usamos como resultados de O(1).
 *
 * Para evitar tener que crear funciones independientes y su manejo
 * de resultados multiples (multiples returns), se decidio por el
 * uso de una esctructura especifica y solo para manetener los resultados
 * en acceso constante.
 *
 */
typedef struct Lote {
  double promedio_soluciones; /**< Mantenemos el promedio de soluciones. */
  RUTA *mejor_solucion; /**< Es la mejor solucion SIEMPRE del programa. */
  double mejor_solucion_fun; /**< Es el resultado de f(mejor_solucion). */
  RUTA *ruta; /**< Es la ruta actual a la que sacamos vecinos. */
} LOTE;

/**
 * @brief Inicializa los valores de el lote.
 *
 * @param ruta -Son los datos iniciales del programa.
 * @return Un lote con una sola ruta y evaluada en 0.
 *
 */
LOTE *init_lote(RUTA *ruta);

/**
 * @brief En esta funcion actualizamos constantemente la ruta del lote.
 *
 * @param t - Es la temperatura que usa el programa para el margen de error.
 * @param s - Es el lote actual del programa.
 * @param L - Es una constante que nos da cuantos lotes debe sacar.
 *
 */
void calcula_lote(TEMPERATURA *t, LOTE *s, double L);
#endif //Fin de lote.h
