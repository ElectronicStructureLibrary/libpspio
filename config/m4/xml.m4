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
# XML support
#



# PIO_CHECK_XML()
# ---------------
#
# Check whether XML is working.
#
AC_DEFUN([PIO_CHECK_XML],[
  dnl Init
  pio_xml_ok="unknown"
  pio_xml_incs="${with_xml_incs}"
  pio_xml_libs="${with_xml_libs}"

  dnl Backup environment
  pio_saved_CPPFLAGS="${CPPFLAGS}"
  pio_saved_LIBS="${LIBS}"

  dnl Look for a configurator
  if test "${PKG_CONFIG}" != ""; then
    if test "${pio_xml_incs}" = ""; then
      AC_MSG_CHECKING([for XML include flags])
      pio_xml_incs=`${PKG_CONFIG} --silence-errors --cflags libxml-2.0`
      if test "${?}" = "0"; then
        AC_MSG_RESULT([${pio_xml_incs}])
      else
        pio_xml_incs=""
        AC_MSG_RESULT([none found])
      fi
    fi
    if test "${pio_xml_libs}" = ""; then
      AC_MSG_CHECKING([for XML link flags])
      pio_xml_libs=`${PKG_CONFIG} --silence-errors --libs libxml-2.0`
      if test "${?}" = "0"; then
        AC_MSG_RESULT([${pio_xml_libs}])
      else
        pio_xml_libs=""
        AC_MSG_RESULT([none found])
      fi
    fi
  fi

  dnl Update environment
  CPPFLAGS="${CPPFLAGS} ${pio_xml_incs}"
  LIBS="${pio_xml_libs} ${LIBS}"

  dnl Look for includes
  AC_LANG_PUSH([C])
  AC_CHECK_HEADERS([libxml/parser.h libxml/tree.h],[pio_xml_has_incs="yes"],[pio_xml_has_incs="no"])

  dnl Look for libraries and routines
  if test "${pio_xml_libs}" = ""; then
    AC_CHECK_LIB([xml2],[xmlReadFile],[pio_xml_has_libs="yes"],[pio_xml_has_libs="no"])
    if test "${pio_xml_has_libs}" = "yes"; then
      pio_xml_libs="-lxml2"
    fi
  else
    AC_MSG_CHECKING([whether the specified XML library works])
    AC_LINK_IFELSE([AC_LANG_PROGRAM(
      [[
#include "libxml/parser.h"
#include "libxml/tree.h"
      ]],
      [[
        LIBXML_TEST_VERSION
      ]])], [pio_xml_has_libs="yes"], [pio_xml_has_libs="no"])
    AC_MSG_RESULT([${pio_xml_has_libs}])
  fi
  AC_LANG_POP([C])

  dnl Take final decision
  if test "${pio_xml_has_incs}" = "yes" -a \
          "${pio_xml_has_libs}" = "yes"; then
    pio_xml_ok="yes"
  fi

  dnl Restore environment
  CPPFLAGS="${pio_saved_CPPFLAGS}"
  LIBS="${pio_saved_LIBS}"
]) # PIO_CHECK_XML
