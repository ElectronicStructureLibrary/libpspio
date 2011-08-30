#
# Makefile for the sources of Libpspio
#

                    # ------------------------------------ #

#
# Source files
#

# Hand-written - keep this in alphabetical order
pio_srcs = \
  ab2libxc.c \
  nlcc_abinit.c \
  pspio_error.c \
  pspio_errorstr.c \
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


# Script-generated
pio_built_srcs = \
  dummy_auto.f90

# C headers - keep this in alphabetical order
pio_hdrs = \
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
  read_upf.h \
  xc_func.h

                    # ------------------------------------ #

#
# Build targets
#

# Libraries
lib_LTLIBRARIES = libpspio.la
libpspio_la_SOURCES = $(pio_srcs)
if DO_BUILD_FORTRAN
  nodist_libpspio_la_SOURCES = $(pio_built_srcs)
endif

# Headers
include_HEADERS = $(pio_hdrs)

# Fortran modules have to be cleaned explicitely
clean-local:
	rm -f *.mod *.MOD

                    # ------------------------------------ #

# Explicit dependencies (if needed)
# Use $(LTOBJEXT) as extension for object files
# ...
