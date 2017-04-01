#!/bin/bash

# Script para eliminar el exceso de documentos que la herramienta autotools genera:
make clean ;
rm config.guess ;
rm config.sub ;
rm Makefile ;
rm Makefile.in ;
rm aclocal.m4 ;
rm -r autom4te.cache/ ;
rm compile ;
rm config.log ;
rm config.status ;
rm configure ;
rm depcomp ;
rm install-sh ;
rm missing ; 
