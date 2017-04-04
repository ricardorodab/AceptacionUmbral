#!/bin/bash

# El proposito de este script es no tener que correr continuamente
# a mano las herramientas de autotools para instalar el proyecto
# Proyecto01.

aclocal ;
autoconf ;

pwd ;

automake --add-missing --foreign ;
./configure ;
