#
# Copyright (C) 2017      Yann Pouillon <notifications@materialsevolution.es>
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
# Makefile for the utilities of Libpspio
#

                    # ------------------------------------ #

#
# Programs
#

#if DO_DEBUG
check_PROGRAMS = pspio_diff pspio_loop
#else
noinst_PROGRAMS = pspio_diff pspio_loop
#endif

pspio_diff_SOURCES = pspio_diff.c
pspio_diff_CPPFLAGS = -I$(top_srcdir)/src
pspio_diff_LDADD = ../src/libpspio.la $(LIBS_COVERAGE)

pspio_loop_SOURCES = pspio_loop.c
pspio_loop_CPPFLAGS = -I$(top_srcdir)/src
pspio_loop_LDADD = ../src/libpspio.la $(LIBS_COVERAGE)

#if DO_DEBUG
TESTS = test_pspio_formats
#endif

                    # ------------------------------------ #

#
# Clean targets
#

# Temporary test directories
mostlyclean-local:
	rm -rf tmp-tests

# Temporary test outputs
MOSTLYCLEANFILES = *.gcda *.gcno *.gcov test_*.tmp
