# -*- Autoconf -*-
#
# Copyright (C) 2015 Yann Pouillon
#
# This file is part of the Libpspio-Fortran software package. For license
# information, please see the COPYING file in the top-level directory of
# the Libpspio-Fortran source distribution.
#

#
# Versatile pseudopotential I/O support
#



# AX_SEARCH_LIBPSPIO(API_MAJOR, API_MINOR_LOW, API_MINOR_HIGH)
# ------------------------------------------------------------
#
# Check whether the Libpspio library is working.
#
AC_DEFUN([AX_SEARCH_LIBPSPIO],[
  dnl Check parameters
  dnl m4_if([$1],[],AC_MSG_ERROR([$0 requires API major version number]))
  dnl m4_if([$2],[],AC_MSG_ERROR([$0 requires API minimum minor version number]))
  dnl m4_if([$3],[],AC_MSG_ERROR([$0 requires API maximum minor version number]))

  dnl Init
  tmp_pspio_has_hdrs="unknown"
  tmp_pspio_has_libs="unknown"
  tmp_pspio_has_version="unknown"
  pio_pspio_ok="unknown"
  pio_pspio_incs="${with_pspio_incs}"
  pio_pspio_libs="${with_pspio_libs}"
  if test "${with_libpspio}" != ""; then
    test "${pio_pspio_incs}" = "" && \
      pio_pspio_incs="-I${with_libpspio}/include"
    test "${pio_pspio_libs}" = "" && \
      pio_pspio_libs="-L${with_libpspio}/lib -lpspio"
  fi

  dnl Display input parameters
  AC_MSG_CHECKING([whether Libpspio includes have been specified])
  if test "${pio_pspio_incs}" = ""; then
    AC_MSG_RESULT([no])
  else
    AC_MSG_RESULT([yes])
  fi
  AC_MSG_CHECKING([whether Libpspio libraries have been specified])
  if test "${pio_pspio_libs}" = ""; then
    AC_MSG_RESULT([no])
  else
    AC_MSG_RESULT([yes])
  fi

  dnl Prepare environment
  tmp_saved_CPPFLAGS="${CPPFLAGS}"
  tmp_saved_FCFLAGS="${FCFLAGS}"
  tmp_saved_LIBS="${LIBS}"
  CPPFLAGS="${CPPFLAGS} ${pio_pspio_incs}"
  FCFLAGS="${FCFLAGS} ${pio_pspio_incs}"
  LIBS="${pio_pspio_libs} ${LIBS}"

  dnl Look for C includes
  AC_LANG_PUSH([C])
  AC_CHECK_HEADERS([pspio.h],
    [tmp_pspio_has_hdrs="yes"],[tmp_pspio_has_hdrs="no"])
  AC_LANG_POP([C])

  dnl Look for C libraries and routines
  if test "${tmp_pspio_has_hdrs}" = "yes"; then
    if test "${pio_pspio_libs}" = ""; then
      AC_LANG_PUSH([C])
      AC_SEARCH_LIBS([pspio_pspdata_alloc],[pspio],
        [tmp_pspio_has_libs="yes"], [tmp_pspio_has_libs="no"])
      AC_LANG_POP([C])
      if test "${ac_cv_search_pspio_pspdata_alloc}" != "no"; then
        if test "${ac_cv_search_pspio_pspdata_alloc}" != "none required"; then
          pio_pspio_libs="${ac_cv_search_pspio_pspdata_alloc}"
        fi
      fi
    else
      tmp_pspio_has_libs="yes"
    fi
  fi

  dnl Check that Libpspio runs as expected
  if test "${tmp_pspio_has_libs}" = "yes"; then
    AC_MSG_CHECKING([whether Libpspio libraries work])
    AC_LANG_PUSH([C])
    AC_LINK_IFELSE([AC_LANG_PROGRAM(
      [[
#include <pspio.h>
      ]],
      [[
        pspio_pspdata_t *p;
        return pspio_pspdata_read(p, 0, "conftest.dat");
      ]])], [tmp_pspio_has_libs="yes"], [tmp_pspio_has_libs="no"])
    AC_MSG_RESULT([${tmp_pspio_has_libs}])
    AC_LANG_POP([C])
  fi

  dnl Look for alternate locations if needed
  if test \( "${tmp_pspio_has_hdrs}" != "yes" -o \
             "${tmp_pspio_has_libs}" != "yes" \) -a \
          \( "${pio_pspio_incs}" = "" -a \
             "${pio_pspio_libs}" = "" \); then
    for tmp_incdir in "${ac_top_srcdir}/src" "${ac_top_srcdir}/../src"; do
      AC_MSG_CHECKING([for pspio.h in ${tmp_incdir}])
      if test -s "${tmp_incdir}/pspio.h"; then
        pio_pspio_incs="-I${tmp_incdir}"
        tmp_pspio_has_hdrs="yes"
        AC_MSG_RESULT([${tmp_pspio_has_hdrs}])
        break
      fi
      AC_MSG_RESULT([no])
    done
    for tmp_libdir in "${ac_builddir}/src/.libs" \
                      "${ac_builddir}/../src/.libs"; do
      AC_MSG_CHECKING([for libpspio.a in ${tmp_libdir}])
      if test -s "${tmp_libdir}/libpspio.a"; then
        pio_pspio_libs="${tmp_libdir}/libpspio.a"
        tmp_pspio_has_libs="yes"
        AC_MSG_RESULT([${tmp_pspio_has_libs}])
        break
      fi
      AC_MSG_RESULT([no])
    done

    if test "${pio_pspio_incs}" != "" -a \
            "${pio_pspio_libs}" != ""; then
      CPPFLAGS="${CPPFLAGS} ${pio_pspio_incs}"
      FCFLAGS="${FCFLAGS} ${pio_pspio_incs}"
      LIBS="${pio_pspio_libs} ${LIBS}"
    fi
  fi

  dnl Check that we have the correct Libpspio version
  if test "${tmp_pspio_has_hdrs}" = "yes" -a \
          "${tmp_pspio_has_libs}" = "yes"; then
    AC_MSG_CHECKING([whether Libpspio version is between $1.$2 and $1.$3])
    LDFLAGS="${CC_LDFLAGS}"
    AC_LANG_PUSH([C])
    AC_RUN_IFELSE([AC_LANG_PROGRAM(
      [[
#include "pspio.h"
      ]],
      [[
        int major = -1, minor = -1, micro = -1;
        pspio_info_version(&major, &minor, &micro);
        if ( (major != $1) || (minor < $2) || (minor > $3) ) {
          return 1; }
      ]])], [tmp_pspio_has_version="yes"], [tmp_pspio_has_version="no"])
    AC_LANG_POP([C])
    AC_MSG_RESULT([${tmp_pspio_has_version}])
  fi

  dnl Take final decision
  if test "${tmp_pspio_has_hdrs}" = "yes" -a \
          "${tmp_pspio_has_libs}" = "yes" -a \
          "${tmp_pspio_has_version}" = "yes"; then
    pio_pspio_ok="yes"
  else
    pio_pspio_ok="no"
  fi

  dnl Allow for slave mode
  if test "${pio_pspio_ok}" != "yes"; then
    if test "${with_pspio}" = "" -a \
            "${with_pspio_incs}" = "" -a \
            "${with_pspio_libs}" = ""; then
      AC_MSG_WARN([assuming slave build - bypassing Libpspio detection])
      pio_pspio_ok="yes"
      pio_pspio_incs="-I${ac_abs_top_srcdir}/src"
      pio_pspio_libs="${ac_abs_top_builddir}/src/.libs/libpspio.a"
    fi
  fi

  dnl Restore build environment
  CPPFLAGS="${tmp_saved_CPPFLAGS}"
  FCFLAGS="${tmp_saved_FCFLAGS}"
  LIBS="${tmp_saved_LIBS}"

  dnl Substitute variables needed for the use of the libraries
  AC_SUBST(pio_pspio_incs)
  AC_SUBST(pio_pspio_libs)
]) # AX_SEARCH_LIBPSPIO
