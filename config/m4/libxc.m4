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
# LibXC support
#



# PIO_CHECK_LIBXC()
# ---------------------
#
# Check whether the LibXC library is working.
#
AC_DEFUN([PIO_CHECK_LIBXC],[
  dnl Init
  pio_libxc_ok="unknown"
  pio_libxc_incs="${with_libxc_incs}"
  pio_libxc_libs="${with_libxc_libs}"

  dnl Backup environment
  pio_saved_CPPFLAGS="${CPPFLAGS}"
  pio_saved_LIBS="${LIBS}"

  dnl Look for a configurator
  if test "${PKG_CONFIG}" != ""; then
    if test "${pio_libxc_incs}" = ""; then
      AC_MSG_CHECKING([for LibXC include flags])
      pio_libxc_incs=`${PKG_CONFIG} --silence-errors --cflags libxc`
      if test "${?}" = "0"; then
        AC_MSG_RESULT([${pio_libxc_incs}])
      else
        pio_libxc_incs=""
        AC_MSG_RESULT([none found])
      fi
    fi
    if test "${pio_libxc_libs}" = ""; then
      AC_MSG_CHECKING([for LibXC link flags])
      pio_libxc_libs=`${PKG_CONFIG} --silence-errors --libs libxc`
      if test "${?}" = "0"; then
        AC_MSG_RESULT([${pio_libxc_libs}])
      else
        pio_libxc_libs=""
        AC_MSG_RESULT([none found])
      fi
    fi
  fi

  dnl Update environment
  CPPFLAGS="${CPPFLAGS} ${pio_libxc_incs}"
  LIBS="${pio_libxc_libs} ${LIBS}"

  dnl Look for includes
  AC_LANG_PUSH([C])
  AC_CHECK_HEADERS([xc.h xc_funcs.h],[pio_libxc_has_incs="yes"],[pio_libxc_has_incs="no"])

  dnl Look for libraries and routines
  if test "${pio_libxc_libs}" = ""; then
    AC_SEARCH_LIBS([xc_func_init],[xc dft_xc],[pio_libxc_has_libs="yes"],[pio_libxc_has_libs="no"])
    if test "${pio_libxc_has_libs}" = "yes"; then
      if test "${ac_cv_search_xc_func_init}" != "none required"; then
        pio_libxc_libs="${ac_cv_search_xc_func_init}"
      fi
    fi
  else
    AC_MSG_CHECKING([whether the specified LibXC library works])
    AC_LINK_IFELSE([AC_LANG_PROGRAM(
      [[
#include "xc.h"
#include "xc_funcs.h"
      ]],[[
        xc_func_type *p;
        xc_func_init(p,1,1)
      ]])],[pio_libxc_has_libs="yes"],[pio_libxc_has_libs="no"])
    AC_MSG_RESULT([${pio_libxc_has_libs}])
  fi
  AC_LANG_POP([C])

  dnl Restore and update environment
  CPPFLAGS="${pio_saved_CPPFLAGS}"
  LIBS="${pio_saved_LIBS}"

  dnl Take final decision
  if test "${pio_libxc_has_incs}" = "yes" -a \
          "${pio_libxc_has_libs}" = "yes"; then
    pio_libxc_ok="yes"
  fi
]) # PIO_CHECK_LIBXC



# PIO_CHECK_LIBXC_FORTRAN()
# -------------------------
#
# Checks that the LibXC library has proper Fortran support.
#
AC_DEFUN([PIO_CHECK_LIBXC_FORTRAN],[
  dnl Init
  pio_libxc_fortran_ok="unknown"

  dnl Backup environment
  pio_saved_FCFLAGS="${FCFLAGS}"
  pio_saved_LIBS="${LIBS}"

  dnl Check the LibXC Fortran interface
  FCFLAGS="${FCFLAGS} ${pio_libxc_incs}"
  LIBS="${pio_libxc_libs} ${LIBS}"
  AC_LANG_PUSH([Fortran])
  AC_MSG_CHECKING([whether the LibXC Fortran bindings work])
  AC_LINK_IFELSE([AC_LANG_PROGRAM([],
    [[
      use xc_f90_types_m
      use xc_f90_lib_m

      implicit none

      TYPE(xc_f90_pointer_t) :: xc_func
      TYPE(xc_f90_pointer_t) :: xc_info
      integer :: func_id = 1

      call xc_f90_func_init(xc_func, xc_info, func_id, XC_UNPOLARIZED)
      call xc_f90_func_end(xc_func)
    ]])], [pio_libxc_fortran_ok="yes"], [pio_libxc_fortran_ok="no"])
  AC_MSG_RESULT([${pio_libxc_fortran_ok}])
  AC_LANG_POP([Fortran])

  dnl Restore environment
  FCFLAGS="${pio_saved_FCFLAGS}"
  LIBS="${pio_saved_LIBS}"
]) # PIO_CHECK_LIBXC_FORTRAN
