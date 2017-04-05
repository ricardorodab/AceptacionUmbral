AUTOMAKE_OPTIONS =  subdir-objects
AM_CFLAGS = -lsqlite3 -I$(top_srcdir)/lib $(GLIB_CFLAGS) $(GLIB_LIBS)
noinst_LIBRARIES = lib/libutils.a

END_LIST =

lib_libutils_a_SOURCES =    \
	lib/aceptacion.c    \
	lib/aceptacion.h    \
	lib/ciudad.c        \
	lib/ciudad.h        \
	lib/lote.c          \
	lib/lote.h          \
	lib/mapa.c          \
	lib/mapa.h          \
	lib/ruta.c          \
	lib/ruta.h          \
	lib/funcion.c       \
	lib/funcion.h       \
	lib/solucion.c      \
	lib/solucion.h      \
	lib/temperatura.c   \
	lib/temperatura.h   \
	$(END_LIST)

lib_libutils_a_CPPFLAGS = $(GLIB_CFLAGS)
lib_libutils_a_LDFLAGS= $(GLIB_LIBS)	

bin_PROGRAMS = src/main
src_main_LDADD = $(top_builddir)/lib/libutils.a 
src_main_CFLAGS = $(AM_CFLAGS) 
src_main_SOURCES = src/main.c src/conexion.c src/conexion.h #intefaz-grafica.c/h

TESTS = test/test-ciudad test/test-ruta

TESTCIUDADSOURCES = test/test-ciudad.c
TESTRUTASOURCES =test/test-ruta.c

test_test_ciudad_CFLAGS = $(AM_CFLAGS)
test_test_ciudad_LDADD = $(top_builddir)/lib/libutils.a
test_test_ruta_CFLAGS = $(AM_CFLAGS)
test_test_ruta_LDADD = $(top_builddir)/lib/libutils.a  

check_PROGRAMS = $(TESTS)

#SUBDIRS = lib src
