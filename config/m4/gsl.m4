# -*- Autoconf -*-
#
# M4 macros for Libpspio
#
# Copyright (C) 2011-2015 Yann Pouillon
#
# This file is part of the Libpspio software package. For license information,
# please see the COPYING file in the top-level directory of the source
# distribution.
#

#
# GSL support
#



# PIO_CHECK_GSL()
# ---------------
#
# Check whether the GSL library is working.
#
AC_DEFUN([PIO_CHECK_GSL],[
  dnl Init
  pio_gsl_has_incs="unknown"
  pio_gsl_has_math="unknown"
  pio_gsl_has_cblas="unknown"
  pio_gsl_has_libs="unknown"
  pio_gsl_ok="unknown"
  pio_gsl_incs="${with_gsl_incs}"
  pio_gsl_libs="${with_gsl_libs}"

  dnl Backup environment
  pio_saved_CPPFLAGS="${CPPFLAGS}"
  pio_saved_LIBS="${LIBS}"

  dnl Look for a configurator
  if test "${pio_gsl_incs}" = "" -a "${pio_gsl_libs}" = ""; then
    GSL_CPPFLAGS=""
    GSL_CFLAGS=""
    GSL_LIBS=""
    PKG_CHECK_MODULES([GSL], [gsl >= 1.16])
    pio_gsl_incs="${GSL_CFLAGS}"
    pio_gsl_libs="${GSL_LIBS}"
    if test "${pio_gsl_incs}" = "" -a "${pio_gsl_libs}" = ""; then
      AX_PATH_GSL([1.16])
      pio_gsl_incs="${GSL_CFLAGS}"
      pio_gsl_libs="${GSL_LIBS}"
    fi
  fi

  dnl Display GSL configuration
  AC_MSG_CHECKING([for GSL include flags])
  AC_MSG_RESULT([${pio_gsl_incs}])
  AC_MSG_CHECKING([for GSL libraries])
  AC_MSG_RESULT([${pio_gsl_libs}])

  dnl Update environment
  CPPFLAGS="${CPPFLAGS} ${pio_gsl_incs}"
  LIBS="${pio_gsl_libs} ${LIBS}"

  dnl Look for includes
  AC_LANG_PUSH([C])
  AC_CHECK_HEADERS([gsl/gsl_sf_gamma.h],
    [pio_gsl_has_incs="yes"], [pio_gsl_has_incs="no"])
  AC_LANG_POP([C])

  dnl Look for libraries and routines
  if test "${pio_gsl_has_incs}" = "yes"; then
    if test "${pio_gsl_libs}" = ""; then
      AC_LANG_PUSH([C])
      AC_SEARCH_LIBS([exp], [m],
        [pio_gsl_has_math="yes"], [pio_gsl_has_math="no"])
      if test "${pio_gsl_has_math}" = "yes"; then
        AC_SEARCH_LIBS([cblas_dgemm], [gslcblas],
          [pio_gsl_has_cblas="yes"], [pio_gsl_has_cblas="no"])
      fi
      if test "${pio_gsl_has_cblas}" = "yes"; then
        AC_SEARCH_LIBS([gsl_sf_gamma], [gsl],
          [pio_gsl_has_libs="yes"], [pio_gsl_has_libs="no"])
      fi
      AC_LANG_POP([C])
      if test "${pio_gsl_has_libs}" = "yes"; then
        pio_gsl_libs="-lgsl -lgslcblas -lm"
      fi
    else
      pio_gsl_has_libs="yes"
    fi
  fi

  dnl Check that the libraries actually work
  if test "${pio_gsl_has_libs}" = "yes"; then
    AC_MSG_CHECKING([whether the specified GSL libraries work])
    AC_LANG_PUSH([C])
    AC_LINK_IFELSE([AC_LANG_PROGRAM(
      [[
#include "gsl/gsl_sf_gamma.h"
      ]],
      [[
        double x,y;
        x = 1.0;
        y = gsl_sf_gamma(x);
      ]])], [pio_gsl_ok="yes"], [pio_gsl_ok="no"])
    AC_LANG_POP([C])
    AC_MSG_RESULT([${pio_gsl_ok}])
  fi

  dnl Restore environment
  CPPFLAGS="${pio_saved_CPPFLAGS}"
  LIBS="${pio_saved_LIBS}"
]) # PIO_CHECK_GSL
