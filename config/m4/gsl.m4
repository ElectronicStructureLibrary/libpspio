# -*- Autoconf -*-
#
# M4 macros for Libpspio
#
# Copyright (C) 2011 Yann Pouillon
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
  pio_gsl_ok="unknown"
  pio_gsl_incs="${with_gsl_incs}"
  pio_gsl_libs="${with_gsl_libs}"

  dnl Backup environment
  pio_saved_CPPFLAGS="${CPPFLAGS}"
  pio_saved_LIBS="${LIBS}"

  dnl Look for a configurator
  AC_CHECK_PROGS([GSL_CONFIG],[gsl-config])
  if test "${GSL_CONFIG}" != ""; then
    if test "${pio_gsl_incs}" = ""; then
      AC_MSG_CHECKING([for GSL include flags])
      pio_gsl_incs=`${GSL_CONFIG} --cflags`
      AC_MSG_RESULT([${pio_gsl_incs}])
    fi
    if test "${pio_gsl_libs}" = ""; then
      AC_MSG_CHECKING([for GSL link flags])
      pio_gsl_libs=`${GSL_CONFIG} --libs`
      AC_MSG_RESULT([${pio_gsl_libs}])
    fi
  fi

  dnl Update environment
  CPPFLAGS="${CPPFLAGS} ${pio_gsl_incs}"
  LIBS="${pio_gsl_libs} ${LIBS}"

  dnl Look for includes
  AC_LANG_PUSH([C])
  AC_CHECK_HEADERS([gsl/gsl_sf_gamma.h],[pio_gsl_has_incs="yes"],[pio_gsl_has_incs="no"])

  dnl Look for libraries and routines
  if test "${pio_gsl_libs}" = ""; then
    LIBS="-lgslcblas -lm ${LIBS}"
    AC_CHECK_LIB([gsl],[gsl_sf_gamma],[pio_gsl_has_libs="yes"],[pio_gsl_has_libs="no"])
    if test "${pio_gsl_has_libs}" = "yes"; then
      pio_gsl_libs="-lgsl -lgslcblas -lm"
    fi
  else
    AC_MSG_CHECKING([whether the specified GSL library works])
    AC_LINK_IFELSE([AC_LANG_PROGRAM(
      [[
#include "gsl/gsl_sf_gamma.h"
      ]],
      [[
        double x,y;
        x = 1.0;
        y = gsl_sf_gamma(x);
      ]])], [pio_gsl_has_libs="yes"], [pio_gsl_has_libs="no"])
    AC_MSG_RESULT([${pio_gsl_has_libs}])
  fi
  AC_LANG_POP([C])

  dnl Take final decision
  if test "${pio_gsl_has_incs}" = "yes" -a \
          "${pio_gsl_has_libs}" = "yes"; then
    pio_gsl_ok="yes"
  fi

  dnl Restore environment
  CPPFLAGS="${pio_saved_CPPFLAGS}"
  LIBS="${pio_saved_LIBS}"
]) # PIO_CHECK_GSL
