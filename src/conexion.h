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

sqlite3 *db;

sqlite3* open_conexion(char *db_name, sqlite3 *db);

void close_conexion(sqlite3 *db);

int get_query(char *db_name,
	      char *query,
	      sqlite3* db,
	      void* param_dicc,
	      int (*callback)(void*,int,char**,char**));
//Fin de conexion.h
