#
# Copyright (C) 2011-2017 Micael Oliveira <micael.oliveira@mpsd.mpg.de>
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
# Makefile for the sources of Libpspio-Fortran
#

                    # ------------------------------------ #

#
# Build parameters
#

AM_CFLAGS = @CFLAGS_COVERAGE@
AM_FCFLAGS = @FCFLAGS_COVERAGE@ -I..

                    # ------------------------------------ #

#
# FRUIT unit test framework
#

noinst_LTLIBRARIES = libfruit.la
libfruit_la_SOURCES = fruit.f90

                    # ------------------------------------ #

#
# Test programs
#
fpio_basic_tests = \
  test_fortran_error \
  test_fortran_info \
  test_fortran_mesh \
  test_fortran_meshfunc
fpio_xfail_tests = \
  test_fortran_io
fpio_wrapped_tests = \
  test_fortran_io_multiple
fpio_gcov_pre_tests = \
  gcov_fortran_clean
fpio_gcov_post_tests = \
  gcov_fortran_report
fpio_memprof_tests = \
  test_fortran_memory

check_PROGRAMS = $(fpio_basic_tests) $(fpio_xfail_tests)
nodist_check_SCRIPTS = \
  $(fpio_gcov_pre_tests) \
  $(fpio_wrapped_tests) \
  $(fpio_gcov_post_tests) \
  $(fpio_memprof_tests)

test_fortran_error_SOURCES = \
  fruit_error_test.F90 \
  test_fortran_error.F90
test_fortran_error_LDADD = \
  libfruit.la \
  ../libpspiof.la \
  @pio_pspio_libs@ \
  $(LIBS_COVERAGE)
test_fortran_error_DEPENDENCIES = \
  ../libpspiof.la \
  @pio_pspio_libs@

test_fortran_info_SOURCES = \
  fruit_info_test.F90 \
  test_fortran_info.F90
test_fortran_info_LDADD = \
  libfruit.la \
  ../libpspiof.la \
  @pio_pspio_libs@ \
  $(LIBS_COVERAGE)
test_fortran_info_DEPENDENCIES = \
  ../libpspiof.la \
  @pio_pspio_libs@

test_fortran_mesh_SOURCES = \
  fruit_mesh_test.F90 \
  fruit_mesh_basket.F90 \
  test_fortran_mesh.F90
test_fortran_mesh_LDADD = \
  libfruit.la \
  ../libpspiof.la \
  @pio_pspio_libs@ \
  $(LIBS_COVERAGE)
test_fortran_mesh_DEPENDENCIES = \
  ../libpspiof.la \
  @pio_pspio_libs@

test_fortran_meshfunc_SOURCES = \
  fruit_meshfunc_test.F90 \
  fruit_meshfunc_basket.F90 \
  test_fortran_meshfunc.F90
test_fortran_meshfunc_LDADD = \
  libfruit.la \
  ../libpspiof.la \
  @pio_pspio_libs@ \
  $(LIBS_COVERAGE)
test_fortran_meshfunc_DEPENDENCIES = \
  ../libpspiof.la \
  @pio_pspio_libs@

test_fortran_io_SOURCES = test_fortran_io.F90
test_fortran_io_LDADD = \
  ../libpspiof.la \
  @pio_pspio_libs@ \
  $(LIBS_COVERAGE)
test_fortran_io_DEPENDENCIES = \
  ../libpspiof.la \
  @pio_pspio_libs@

TESTS = \
  $(fpio_gcov_pre_tests) \
  $(fpio_basic_tests) \
  $(fpio_xfail_tests) \
  $(fpio_wrapped_tests) \
  $(fpio_gcov_post_tests) \
  $(fpio_memprof_tests)
XFAIL_TESTS = \
  $(fpio_xfail_tests)

                    # ------------------------------------ #

#
# Explicit dependencies
#

# Code coverage
-include gcov-fortran-tests-deps.mf

                    # ------------------------------------ #

#
# Explicit targets
#

# Temporary test outputs
MOSTLYCLEANFILES = *.gcda *.gcno *.gcov test_*.tmp
