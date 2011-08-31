#
# Makefile for the sources of Libpspio
#

                    # ------------------------------------ #

#
# Source files
#

# C source - keep this in alphabetical order
pio_core_srcs = \
  ab2libxc.c \
  nlcc_abinit.c \
  pspio_error.c \
  pspio_init.c \
  pspio_mesh.c \
  pspio_meshfunc.c \
  pspio_nlcc.c \
  pspio_potential.c \
  pspio_projector.c \
  pspio_qn.c \
  pspio_state.c \
  read_abinit_header.c \
  read_abinit5.c \
  read_upf.c

# C headers - keep this in alphabetical order
pio_core_hdrs = \
  libxccodes.h \
  pspio.h \
  pspio_common.h \
  pspio_error.h \
  pspio_mesh.h \
  pspio_meshfunc.h \
  pspio_nlcc.h \
  pspio_potential.h \
  pspio_projector.h \
  pspio_qn.h \
  pspio_state.h \
  read_upf.h

# Fortran source files and modules
if DO_BUILD_FORTRAN
  pio_fortran_srcs = pspio_fortran.F90
if F90_MOD_UPPERCASE
  pio_fortran_mods = PSPIO_FORTRAN.$(F90_MOD_EXT)
else
  pio_fortran_mods = pspio_fortran.$(F90_MOD_EXT)
endif
else
  pio_fortran_srcs =
  pio_fortran_mods =
endif

# Script-generated file lists
pio_built_srcs = $(pio_fortran_srcs)
pio_built_hdrs = $(pio_fortran_mods)

                    # ------------------------------------ #

#
# Build targets
#

# Essential build parameters
INCLUDES = @pio_core_incs@

# Libraries
lib_LTLIBRARIES = libpspio.la

libpspio_la_LDFLAGS = -version-info 0:0:0
libpspio_la_SOURCES = $(pio_core_srcs)
if DO_BUILD_FORTRAN
  nodist_libpspio_la_SOURCES = $(pio_built_srcs)
endif

# Headers
include_HEADERS = $(pio_core_hdrs)
if DO_BUILD_FORTRAN
  nodist_include_HEADERS = $(pio_built_hdrs)
endif

# Fortran modules have to be cleaned explicitely
clean-local:
	rm -f $(pio_fortran_mods) dummy

                    # ------------------------------------ #

# Explicit dependencies (if needed, e.g. with Fortran)
# Use $(LTOBJEXT) as extension for object files
# ...
