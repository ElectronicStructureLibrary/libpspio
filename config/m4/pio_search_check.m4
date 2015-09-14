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
# Support for the Check unit test framework
#



# PIO_SEARCH_CHECK()
# ------------------
#
# Look for the Check libraries.
#
AC_DEFUN([PIO_SEARCH_CHECK],[
  dnl Init
  pio_check_has_hdrs="unknown"
  pio_check_has_libs="unknown"
  pio_check_ok="unknown"
  pio_check_cflags="${PSPIO_CHECK_CFLAGS}"
  pio_check_incs="${PSPIO_CHECK_INCS}"
  pio_check_ldflags="${PSPIO_CHECK_LDFLAGS}"
  pio_check_libs="${PSPIO_CHECK_LIBS}"
  pio_pthread_ok="unknown"
  pio_rt_ok="unknown"

  dnl Define missing environment variables
  AC_ARG_VAR([PSPIO_CHECK_INCS],
    [Include flags for the Check unit test framework])
  AC_ARG_VAR([PSPIO_CHECK_LDFLAGS],
    [C flags for the Check unit test framework])

  dnl Backup environment
  pio_saved_CPPFLAGS="${CPPFLAGS}"
  pio_saved_CFLAGS="${CFLAGS}"
  pio_saved_LDFLAGS="${LDFLAGS}"
  pio_saved_LIBS="${LIBS}"

  dnl Init build parameters
  CPPFLAGS="${CPPFLAGS} ${pio_check_incs}"
  CFLAGS="${CFLAGS} ${pio_check_cflags}"
  LDFLAGS="${LDFLAGS} ${pio_check_ldflags}"
  LIBS="${pio_check_libs} ${LIBS}"

  dnl Search first with pkg-config, then manually
  if test "${pio_check_cflags}" = "" -a \
          "${pio_check_incs}" = "" -a \
          "${pio_check_ldflags}" = "" -a \
          "${pio_check_libs}" = ""; then
    PKG_CHECK_MODULES([PSPIO_CHECK], [check >= 0.9.4],
      [pio_pkg_check="yes"; pio_check_has_hdrs="yes"; pio_check_has_libs="yes"],
      [pio_pkg_check="no"])
    if test "${pio_pkg_check}" = "yes"; then
      pio_check_cflags="${PSPIO_CHECK_CFLAGS}"
      pio_check_libs="${PSPIO_CHECK_LIBS}"
    else
      AX_PTHREAD([pio_pthread_ok="yes"], [pio_pthread_ok="no"])
      if test "${pio_pthread_ok}" = "yes"; then
        pio_check_cflags="${pio_check_cflags} ${PTHREAD_CFLAGS}"
        pio_check_libs="${PTHREAD_LIBS} ${pio_check_libs}"
        CFLAGS="${CFLAGS} ${pio_check_cflags}"
        LIBS="${pio_check_libs} ${LIBS}"
      fi
      AC_SEARCH_LIBS([shm_open], [rt], [pio_rt_ok="yes"], [pio_rt_ok="no"])
      if test "${pio_rt_ok}" = "yes"; then
        if test "${ac_cv_search_shm_open}" != "none required"; then
          pio_check_libs="${ac_cv_search_shm_open} ${pio_check_libs}"
        fi
      else
        AC_MSG_WARN([no real-time extensions available])
      fi
      AC_CHECK_HEADERS([check.h],
        [pio_check_has_hdrs="yes"], [pio_check_has_hdrs="no"])
      if test "${pio_check_has_hdrs}" = "yes"; then
        AC_SEARCH_LIBS([srunner_create], [check_pic check],
          [pio_check_has_libs="yes"], [pio_check_has_libs="no"])
        if test "${pio_check_has_libs}" = "yes"; then
          if test "${ac_cv_search_srunner_create}" != "none required"; then
            pio_check_libs="${ac_cv_search_srunner_create} ${pio_check_libs}"
          fi
        fi
      fi
    fi
  fi

  dnl Look for headers if still needed
  if test "${pio_check_has_hdrs}" != "yes"; then
    AC_CHECK_HEADERS([check.h],
      [pio_check_has_hdrs="yes"], [pio_check_has_hdrs="no"])
  fi

  dnl Check that the specified libraries work
  AC_LANG_PUSH([C])
  AC_LINK_IFELSE([
    AC_LANG_PROGRAM(
      [[
#include "check.h"
      ]],
      [[
        SRunner *sr;
        srunner_run_all(sr, CK_VERBOSE);
      ]])],
    [pio_check_ok="yes"], [pio_check_ok="no"])
  AC_LANG_POP([C])

  dnl Display Check configuration
  AC_MSG_CHECKING([for Check C flags])
  AC_MSG_RESULT([${pio_check_cflags}])
  AC_MSG_CHECKING([for Check include flags])
  AC_MSG_RESULT([${pio_check_incs}])
  AC_MSG_CHECKING([for Check link flags])
  AC_MSG_RESULT([${pio_check_ldflags}])
  AC_MSG_CHECKING([for Check libraries])
  AC_MSG_RESULT([${pio_check_libs}])

  dnl Restore environment
  CPPFLAGS="${pio_saved_CPPFLAGS}"
  CFLAGS="${pio_saved_CFLAGS}"
  LDFLAGS="${pio_saved_LDFLAGS}"
  LIBS="${pio_saved_LIBS}"

  dnl Substitute variables
  AC_SUBST(pio_check_cflags)
  AC_SUBST(pio_check_incs)
  AC_SUBST(pio_check_ldflags)
  AC_SUBST(pio_check_libs)
]) # PIO_SEARCH_CHECK
