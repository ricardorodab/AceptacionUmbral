/* -------------------------------------------------------------------
 * conexion.h
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
 * @file conexion.h
 * @author Jose Ricardo Rodriguez Abreu
 * @date 31 Mar 2017
 * @brief File containing the conexion to the database for the 
 * "Combinatorial Optimization Heuristics" class. 
 *
 * El proposito de este archivo y funciones no es otra mas que el 
 * de definir funciones que nos den la posibilidad de 
 * una conexion a la base de datos para poder aplicar 
 * la funcion que le sea asignada a ese conjunto de datos.
 *
 * El programa usa el estandar de documentacion que define el uso de 
 * doxygen.
 *
 * @see http://www.stack.nl/~dimitri/doxygen/manual/index.html
 * @see https://github.com/ricardorodab/AceptacionUmbral
 *
 */
#include <sqlite3.h>

/**
 * Este es el apuntador a el objeto que mantiene en memeria la base de datos.
 * Fundamental para poder acceder a ella y realizar peticiones.
 */
sqlite3 *db;

/**
 * @brief Funcion que abre la conexion a la base de datos.
 *
 * @param db_name -Es la cadena que define la ubicacion y nombre de la base
 * de datos.
 * @param db -Es el apuntador al objeto que tiene la base de datos a abrir.
 * @return Un objeto apuntador a la direccion que debe realizar peticiones
 * la base de datos.
 * 
 */
sqlite3* open_conexion(char *db_name, sqlite3 *db);

/**
 * @brief Funcion que cierra la conexion con la base de datos.
 *
 * @db -Es el apuntador al objeto que tiene la base de datos a cerrar.
 */
void close_conexion(sqlite3 *db);

/**
 * @brief Funcion dedicada a realizar una peticion y extraer informacion de
 * una base de datos.
 *
 * @param db_name -Es la cadena que define la ubicacion y nombre de la base
 * de datos.
 * @param query -Esta es la indicacion de la peticion que se le debe pasar a 
 * la base de datos. Debe de ser de tipo SELECT respetando la sintaxis del 
 * manejador de base de datos.
 * @param db -Es el apuntador al objeto que tiene la base de datos a realizar
 * la peticion @query.
 * @param param_dicc -Es una estructura de datos o aquello que queramos que 
 * la funcion @callback reciba como parametro del usuario programador.
 * @param callback -Es un apuntador a una funcion que devuelve un entero. Su 
 * funcion es la de manejar como el programador crea conveniente aquella 
 * informacion que recibira del SELECT de @query.
 * @return Un entero 0 si todo sale bien y 1 en caso contrario.
 */
int get_query(char *db_name,
	      char *query,
	      sqlite3* db,
	      void* param_dicc,
	      int (*callback)(void*,int,char**,char**));
//Fin de conexion.h
