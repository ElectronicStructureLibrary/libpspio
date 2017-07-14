#
# Copyright (C) 2017  Yann Pouillon <notifications@materialsevolution.es>
#
# This file is part of Libpspio.
#
# Libpspio is free software: you can redistribute it and/or modify it under the
# terms of the GNU Lesser General Public License as published by the Free
# Software Foundation, version 3 of the License, or (at your option) any later
# version.
#
# Libpspio is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
# details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with Libpspio.  If not, see <http://www.gnu.org/licenses/> or write to
# the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
# 02110-1301  USA.

#
# Makefile for real-world tests of Libpspio
#

AM_CPPFLAGS = -I@srcdir@/../src

                    # ------------------------------------ #

#
# Source code
#

check_PROGRAMS = test_format

test_format_SOURCES = test_format.c
test_format_LDADD = ../src/libpspio.la $(LIBS_COVERAGE)

                    # ------------------------------------ #

#
# Test series
#

pio_tests_fmt_supported = \
  run_test_format_abinit6 \
  run_test_format_fhi98pp \
  run_test_format_upf

pio_tests_fmt_unsupported = \
  run_test_format_abinit1 \
  run_test_format_abinit2 \
  run_test_format_abinit3 \
  run_test_format_abinit4 \
  run_test_format_abinit5 \
  run_test_format_abinit7 \
  run_test_format_abinit8 \
  run_test_format_abinit9 \
  run_test_format_abinit10 \
  run_test_format_abinit11 \
  run_test_format_abinit17 \
  run_test_format_atom \
  run_test_format_octopus_hgh \
  run_test_format_siesta \
  run_test_format_xml

pio_tests_fmt = \
  $(pio_tests_fmt_supported) \
  $(pio_tests_fmt_unsupported)

TESTS = $(pio_tests_fmt)
XFAIL_TESTS = $(pio_tests_fmt_unsupported)

$(pio_tests_fmt):
	$(LN_S) -f run_test_format $@

CLEANFILES = $(pio_tests_fmt) pspio-test-[0-9][0-9][0-9][0-9][0-9][0-9]

EXTRA_DIST = run_test_format
