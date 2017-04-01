AUTOMAKE_OPTIONS =  subdir-objects
AM_CFLAGS = -lsqlite3 -I$(top_srcdir)/lib $(GLIB_CFLAGS) $(GLIB_LIBS)
noinst_LIBRARIES = lib/libutils.a

END_LIST =

lib_libutils_a_SOURCES =    \
	lib/ciudad.c        \
	lib/ciudad.h        \
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


#SUBDIRS = lib src
