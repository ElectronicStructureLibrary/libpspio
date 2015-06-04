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
# XML support
#



# PIO_CHECK_XML()
# ---------------
#
# Check whether XML is working.
#
AC_DEFUN([PIO_CHECK_XML],[
  dnl Init
  pio_xml_has_incs="unknown"
  pio_xml_has_libs="unknown"
  pio_xml_ok="unknown"
  pio_xml_incs="${with_xml_incs}"
  pio_xml_libs="${with_xml_libs}"

  dnl Backup environment
  pio_saved_CPPFLAGS="${CPPFLAGS}"
  pio_saved_LIBS="${LIBS}"

  dnl Look for a configurator
  if test "${pio_xml_incs}" = "" -a "${pio_xml_libs}" = ""; then
    XML_CPPFLAGS=""
    XML_CFLAGS=""
    XML_LIBS=""
    PKG_CHECK_MODULES([XML], [libxml-2.0 >= 2.9.0])
    pio_xml_incs="${XML_CFLAGS}"
    pio_xml_libs="${XML_LIBS}"
    if test "${pio_xml_incs}" = "" -a "${pio_xml_libs}" = ""; then
      AM_PATH_XML2([2.9.0])
      pio_xml_incs="${XML_CPPFLAGS}"
      pio_xml_libs="${XML_LIBS}"
    fi
  fi

  dnl Display XML configuration
  AC_MSG_CHECKING([for XML include flags])
  AC_MSG_RESULT([${pio_xml_incs}])
  AC_MSG_CHECKING([for XML libraries])
  AC_MSG_RESULT([${pio_xml_libs}])

  dnl Update environment
  CPPFLAGS="${CPPFLAGS} ${pio_xml_incs}"
  LIBS="${pio_xml_libs} ${LIBS}"

  dnl Look for includes
  if test "${pio_xml_incs}" = ""; then
    pio_tmp_CPPFLAGS="${CPPFLAGS}"
    for pio_tmpdir in "/usr/include/libxml2" \
                      "/usr/local/include/libxml2" \
                      "/opt/local/include/libxml2"; do
      if test "${pio_xml_has_incs}" != "yes"; then
        AC_MSG_NOTICE([looking for XML includes in ${pio_tmpdir}])
        CPPFLAGS="${pio_tmp_CPPFLAGS} -I${pio_tmpdir}"
        AC_LANG_PUSH([C])
        AC_CHECK_HEADERS([libxml/parser.h libxml/tree.h],
          [pio_xml_has_incs="yes"], [pio_xml_has_incs="no"])
        AC_LANG_POP([C])
      fi
      if test "${pio_xml_has_incs}" = "yes"; then
        pio_xml_incs="-I${pio_tmpdir}"
        break
      fi
      CPPFLAGS="${pio_tmp_CPPFLAGS}"
    done
    unset pio_tmp_CPPFLAGS pio_tmpdir
  else
    AC_LANG_PUSH([C])
    AC_CHECK_HEADERS([libxml/parser.h libxml/tree.h],
      [pio_xml_has_incs="yes"], [pio_xml_has_incs="no"])
    AC_LANG_POP([C])
  fi

  dnl Look for libraries and routines
  if test "${pio_xml_has_incs}" = "yes"; then
    if test "${pio_xml_libs}" = ""; then
      AC_LANG_PUSH([C])
      AC_SEARCH_LIBS([xmlReadFile], [xml2],
        [pio_xml_has_libs="yes"], [pio_xml_has_libs="no"])
      AC_LANG_POP([C])
      if test "${pio_xml_has_libs}" = "yes"; then
        pio_xml_libs="-lxml2"
      fi
    else
      pio_xml_has_libs="yes"
    fi
  fi

  dnl Check that the libraries actually work
  if test "${pio_xml_has_libs}" = "yes"; then
    AC_MSG_CHECKING([whether the specified XML libraries work])
    AC_LANG_PUSH([C])
    AC_LINK_IFELSE([AC_LANG_PROGRAM(
      [[
#include "libxml/parser.h"
#include "libxml/tree.h"
      ]],
      [[
        LIBXML_TEST_VERSION
      ]])], [pio_xml_ok="yes"], [pio_xml_ok="no"])
    AC_LANG_POP([C])
    AC_MSG_RESULT([${pio_xml_ok}])
  fi

  dnl Restore environment
  CPPFLAGS="${pio_saved_CPPFLAGS}"
  LIBS="${pio_saved_LIBS}"
]) # PIO_CHECK_XML
