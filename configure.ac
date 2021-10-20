#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(GreedyZnake, 0.1, a19931031@gmail.com)
AC_CONFIG_SRCDIR([snake_src/snake.cpp])

AM_INIT_AUTOMAKE()

# Checks for programs.
AC_PROG_CXX([g++])
AC_PROG_CC([gcc])
AC_PROG_RANLIB([libs])

# Checks for libraries.
#AC_CHECK_LIB([ncurses], [getch])

# Checks for header files.
AC_CHECK_HEADERS([unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.
AC_CHECK_FUNCS([select sqrt])

#AC_CONFIG_FILES([Makefile])

AC_OUTPUT(Makefile)
