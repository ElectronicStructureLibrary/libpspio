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

SUBDIRS = . tests

                    # ------------------------------------ #

#
# Source files
#

# Fortran source - keep this in alphabetical order
fpio_srcs = \
  pspiof.F90

fpio_inc_srcs = \
  api_error_inc.F90 \
  api_info_inc.F90 \
  api_meshfunc_inc.F90 \
  api_mesh_inc.F90 \
  api_potential_inc.F90 \
  api_projector_inc.F90 \
  api_pspdata_inc.F90 \
  api_pspinfo_inc.F90 \
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
  interface_pspinfo_inc.F90 \
  interface_qn_inc.F90 \
  interface_state_inc.F90 \
  interface_xc_inc.F90

# Fortran modules
if F90_MOD_UPPERCASE
  fpio_mods = FPSPIO_M.@ax_cv_f90_modext@
else
  fpio_mods = pspiof_m.@ax_cv_f90_modext@
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
lib_LTLIBRARIES = libpspiof.la

libpspiof_la_LDFLAGS = -version-info 0:0:0
libpspiof_la_SOURCES = $(fpio_srcs)

# Headers
nodist_include_HEADERS = $(fpio_mods)

# Force check to build the library first
check-local: libpspiof.la

                    # ------------------------------------ #

#
# Explicit dependencies
#

# Fortran
pspiof.$(LTOBJEXT): $(fpio_inc_srcs)

$(fpio_mods): pspiof.$(LTOBJEXT)
	@echo "Modules already built"

                    # ------------------------------------ #

#
# Explicit targets
#

# Fortran modules have to be cleaned explicitely
CLEANFILES = $(fpio_mods)
