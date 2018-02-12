#
# Copyright (C) 2011-2017 Joseba Alberdi <alberdi@hotmail.es>
#                         Matthieu Verstraete <matthieu.jean.verstraete@gmail.com>
#                         Micael Oliveira <micael.oliveira@mpsd.mpg.de>
#                         Yann Pouillon <devops@materialsevolution.es>
#
# This file is part of Libpspio.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# version 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.
#
# Libpspio is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE. See the Mozilla Public License version 2.0 for more
# details.

#
# Makefile for the sources of Libpspio
#

                    # ------------------------------------ #

#
# Source files
#

# C source - keep this in alphabetical order
pio_core_srcs = \
  abinit.c \
  abinit_util.c \
  abinit_xc.c \
  fhi.c \
  oncv.c \
  pspio_error.c \
  pspio_info.c \
  pspio_interp.c \
  pspio_jb_spline.c \
  pspio_mesh.c \
  pspio_meshfunc.c \
  pspio_potential.c \
  pspio_projector.c \
  pspio_pspinfo.c \
  pspio_pspdata.c \
  pspio_qn.c \
  pspio_state.c \
  pspio_xc.c \
  upf.c \
  upf_read.c \
  upf_tag.c \
  upf_write.c \
  upf_xc.c \
  util.c


# Exported C headers - keep this in alphabetical order
pio_core_hdrs = \
  pspio.h \
  pspio_common.h \
  pspio_error.h \
  pspio_info.h \
  pspio_interp.h \
  pspio_jb_spline.h \
  pspio_mesh.h \
  pspio_meshfunc.h \
  pspio_potential.h \
  pspio_projector.h \
  pspio_pspdata.h \
  pspio_pspinfo.h \
  pspio_qn.h \
  pspio_state.h \
  pspio_xc_funcs.h \
  pspio_xc.h


# Internal C headers - keep this in alphabetical order
pio_hidden_hdrs = \
  abinit.h \
  check_pspio.h \
  fhi.h \
  oncv.h \
  upf.h \
  util.h

                    # ------------------------------------ #

#
# Build targets
#

# Essential build parameters
AM_CPPFLAGS = @pio_core_incs@
AM_CFLAGS = @CFLAGS_COVERAGE@
AM_LDFLAGS = @LDFLAGS_COVERAGE@

# Libraries
lib_LTLIBRARIES = libpspio.la

libpspio_la_LDFLAGS = -version-info 0:0:0
libpspio_la_SOURCES = $(pio_core_srcs)

# Headers
include_HEADERS = $(pio_core_hdrs)
noinst_HEADERS = $(pio_hidden_hdrs)

                    # ------------------------------------ #

#
# Test programs
#

pio_basic_tests = \
  check_pspio
pio_gcov_pre_tests = \
  gcov_clean
pio_gcov_post_tests = \
  gcov_report
pio_memprof_tests = \
  test_memory

check_PROGRAMS = $(pio_basic_tests)
nodist_check_SCRIPTS = \
  $(pio_gcov_pre_tests) \
  $(pio_gcov_post_tests) \
  $(pio_memprof_tests)

check_pspio_SOURCES = \
  check_pspio.h \
  check_pspio_info.c \
  check_pspio_error.c \
  check_pspio_qn.c \
  check_pspio_mesh.c \
  check_pspio_meshfunc.c \
  check_pspio_potential.c \
  check_pspio_projector.c \
  check_pspio_state.c \
  check_pspio_xc.c \
  check_pspio_pspinfo.c \
  check_pspio_pspdata.c \
  check_pspio.c
check_pspio_CPPFLAGS = -I$(top_srcdir)/src @pio_check_incs@
check_pspio_CFLAGS = @pio_check_cflags@
check_pspio_LDFLAGS = @pio_check_ldflags@
check_pspio_LDADD = -lpspio $(LIBS_COVERAGE) @pio_check_libs@
check_pspio_DEPENDENCIES = libpspio.la

TESTS = \
  $(pio_gcov_pre_tests) \
  $(pio_basic_tests) \
  $(pio_gcov_post_tests) \
  $(pio_memprof_tests)

                    # ------------------------------------ #

#
# Explicit dependencies
#

-include gcov-tests-deps.mf

                    # ------------------------------------ #

#
# Clean targets
#

# Temporary test outputs
MOSTLYCLEANFILES = *.gcda *.gcno *.gcov test_*.tmp
