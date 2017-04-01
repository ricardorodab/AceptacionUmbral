#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([Proyecto01], [0.1], [ricardo_rodab@ciencias.unam.mx])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([lib/ciudad.c])
# Checks for programs.
#LT_INIT
AC_PROG_CC
AC_PROG_RANLIB
# Checks for libraries.
AC_PROG_INSTALL
PKG_CHECK_MODULES(GLIB, glib-2.0)

# Checks for header files.

#Esto es lo que hizo Canek para gtk, probablemente lo usare:
#PKG_CHECK_MODULES(GLIB_COMPILE_RESOURCES, glib-compile-resources)
# PKG_CHECK_MODULES(EJEMPLO, [ gtk+-3.0 ])
#AC_SUBST(EJEMPLO_CFLAGS)
#AC_SUBST(EJEMPLO_LIBS)


# Checks for typedefs, structures, and compiler characteristics.


# Checks for library functions.
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
