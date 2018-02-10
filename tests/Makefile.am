#
# Copyright (C) 2017 Yann Pouillon <devops@materialsevolution.es>
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
# Makefile for real-world tests of Libpspio
#

AM_CPPFLAGS = -I@srcdir@/../src

                    # ------------------------------------ #

#
# Source code
#

check_PROGRAMS = test_format

test_format_SOURCES = test_format.c
test_format_LDADD = ../src/libpspio.la

                    # ------------------------------------ #

#
# Test series
#

pio_tests_fmt_supported = \
  run_test_format_6 \
  run_test_format_14 \
  run_test_format_17

pio_tests_fmt_unsupported = \
  run_test_format_1 \
  run_test_format_2 \
  run_test_format_3 \
  run_test_format_4 \
  run_test_format_5 \
  run_test_format_7 \
  run_test_format_8 \
  run_test_format_9 \
  run_test_format_10 \
  run_test_format_11 \
  run_test_format_12 \
  run_test_format_13 \
  run_test_format_15 \
  run_test_format_16 \
  run_test_format_18

pio_tests_fmt = \
  $(pio_tests_fmt_supported) \
  $(pio_tests_fmt_unsupported)

TESTS = $(pio_tests_fmt)
XFAIL_TESTS = $(pio_tests_fmt_unsupported)

$(pio_tests_fmt):
	$(LN_S) -f run_test_format $@

CLEANFILES = $(pio_tests_fmt) pspio-test-[0-9][0-9][0-9][0-9][0-9][0-9]

EXTRA_DIST = run_test_format
