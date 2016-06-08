#
# Copyright (C) 2012-2015 M. Oliveira, Y. Pouillon
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 3 of the License, or 
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#
#

#
# Makefile for the sources of Libpspio-Fortran
#

SUBDIRS = . tests

                    # ------------------------------------ #

#
# Source files
#

# Fortran source - keep this in alphabetical order
fpio_srcs = \
  fpspio.F90

fpio_inc_srcs = \
  api_error_inc.F90 \
  api_info_inc.F90 \
  api_meshfunc_inc.F90 \
  api_mesh_inc.F90 \
  api_potential_inc.F90 \
  api_projector_inc.F90 \
  api_pspdata_inc.F90 \
  api_qn_inc.F90 \
  api_state_inc.F90 \
  api_xc_inc.F90 \
  interface_error_inc.F90 \
  interface_info_inc.F90 \
  interface_meshfunc_inc.F90 \
  interface_mesh_inc.F90 \
  interface_potential_inc.F90 \
  interface_projector_inc.F90 \
  interface_pspdata_inc.F90 \
  interface_qn_inc.F90 \
  interface_state_inc.F90 \
  interface_xc_inc.F90

# Fortran modules
if F90_MOD_UPPERCASE
  fpio_mods = FPSPIO_M.@ax_cv_f90_modext@
else
  fpio_mods = fpspio_m.@ax_cv_f90_modext@
endif

# Included Fortran files are not distributed by default
EXTRA_DIST = $(fpio_inc_srcs)

                    # ------------------------------------ #

#
# Build targets
#

# Essential build parameters
AM_CFLAGS = @CFLAGS_COVERAGE@
AM_FCFLAGS = @FCFLAGS_COVERAGE@

# Libraries
lib_LTLIBRARIES = libfpspio.la

libfpspio_la_LDFLAGS = -version-info 0:0:0
libfpspio_la_SOURCES = $(fpio_srcs)

# Headers
nodist_include_HEADERS = $(fpio_mods)

# Force check to build the library first
check-local: libfpspio.la

                    # ------------------------------------ #

#
# Explicit dependencies
#

# Fortran
fpspio.$(LTOBJEXT): $(fpio_inc_srcs)

$(fpio_mods): fpspio.$(LTOBJEXT)
	@echo "Modules already built"

                    # ------------------------------------ #

#
# Explicit targets
#

# Fortran modules have to be cleaned explicitely
CLEANFILES = $(fpio_mods)
