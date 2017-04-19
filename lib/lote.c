/* -------------------------------------------------------------------
 * lote.c
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

#include "lote.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

/**
 * @file lote.c
 * @author Jose Ricardo Rodriguez Abreu
 * @date 19 Apr 2017
 * @brief File containing the struct and funtions for a path of batch
 * for the "Combinatorial Optimization Heuristics" class. 
 *
 * En este archivo implementamos las funciones definidas para hacer
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
 * Para inicializar a los lotes, necesitamos una ruta donde incluye todas
 * las ciudades disponibles en el orden de inicializacion.
 * Los valores de las funciones son 0 y f(mejor_solucion) es infinito.
 *
 */
LOTE* init_lote(RUTA *ruta)
{
  LOTE *lote = malloc(sizeof(LOTE));
  lote->ruta = ruta;
  lote->promedio_soluciones = 0;
  lote->mejor_solucion = ruta;
  lote->mejor_solucion_fun = INFINITY;
  return lote;
}

/**
 * Esta es la funcion que hace toda el trabajo de encontrar
 * las soluciones del programa ya que busca todos los vecinos
 * y almecena las mejores soluciones en el @lote para posterior
 * regresar los datos de la busqueda de vecinos.
 * 
 */
void calcula_lote(TEMPERATURA *t, LOTE *lote, double L)
{
  RUTA *s = lote->ruta;
  int c = 0;
  int COTA = L*2000;
  double r = 0.0;
  while(c < L && (COTA > 0)){ //Donde L = |n| con n ciudades en ruta.
    COTA--;
    RUTA *s_prima = get_ruta_vecina(s);
    double s_prima_fun = funcion_costo(s_prima);
    double s_fun = funcion_costo(s); 
    if (s_prima_fun < lote->mejor_solucion_fun){
      lote->mejor_solucion = s_prima;
      lote->mejor_solucion_fun = s_prima_fun;
      printf("\n");
      imprime_ruta(lote->mejor_solucion);
      printf("Desconexas: %d\n",lote->mejor_solucion->ciudades_desconectadas);
      printf("Funcion: %f\n",s_prima_fun);
      printf("\n");
    }
    if(s_prima_fun <= (s_fun+(t->valor))){
      if(s_fun != lote->mejor_solucion_fun)
	destroy_vecino(s);
      s = s_prima;
      c++;
      r += s_prima_fun;
      lote->ruta = s;
    }else{
      destroy_vecino(s_prima);
    }
  }
  double promedio_aceptacion = r/L;
  if(promedio_aceptacion != 0)
    lote->promedio_soluciones = promedio_aceptacion;
  else
    lote->promedio_soluciones = lote->promedio_soluciones/2;
} //Fin de lote.c
