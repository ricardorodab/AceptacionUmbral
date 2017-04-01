#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ciudad.h"


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
    g_hash_table_new_full(g_int_hash,g_int_equal,NULL,g_free);
}



void delete_ciudad(CIUDAD *ciudad)
{}

CIUDAD* get_vecino(CIUDAD *ciudad, int id)
{return 0;}

void add_vecino(CIUDAD *ciudad, CIUDAD *vecino,double distancia)
{
  GHashTable *ciudad_vecino = ciudad->vecinos;
  GHashTable *vecino_vecino = ciudad->vecinos;
  double *dist = g_malloc(sizeof(double));
  memcpy(dist,&distancia,sizeof(double));
  //Las llaves deben de ser almacenadas en el heap para no perderlas.
  int *key_uno = malloc(sizeof(int*));
  int *key_dos = malloc(sizeof(int*));
  *key_uno = vecino->id;
  *key_dos = ciudad->id;
  g_hash_table_insert(ciudad_vecino,key_uno,dist);
  g_hash_table_insert(vecino_vecino,key_dos,dist);
}
double get_distancia_ciudad(CIUDAD *ciudad_origen, CIUDAD *ciudad_destino)
{return 0;}

void imprime_ciudad(CIUDAD *ciudad)
{
  printf("\nId = %d \n",ciudad->id);
  printf("Name = %s \n",ciudad->name);
  printf("Country = %s \n",ciudad->country);
  printf("Population = %d \n",ciudad->population);
  printf("Latitude = %f \n",ciudad->latitude);
  printf("Longitude = %f \n\n",ciudad->longitude);
}
