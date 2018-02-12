#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(make_test, 0.1, majing@ict.ac.cn)
#AC_CONFIG_SRCDIR([h/sub1/sub1.h])
AM_INIT_AUTOMAKE(make_test,0.1)
LT_INIT
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 src/apps/Makefile
                 src/sub1/Makefile
                 src/sub2/Makefile
                 src/sub3/Makefile])
AC_OUTPUT
