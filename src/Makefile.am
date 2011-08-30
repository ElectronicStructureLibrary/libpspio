#
# Makefile for the sources of Libpspio
#

                    # ------------------------------------ #

#
# Source files
#

# Hand-written
pio_srcs = \
  pspio_errorstr.c \
  pspio_mesh.c \
  pspio_meshfunc.c \
  pspio_qn.c \
  pspio_potential.c \
  pspio_state.c \
  pspio_projector.c \
  pspio_nlcc.h \
  read_abinit_header.c \
  read_abinit5.c \
  ab2libxc.c


# Script-generated
pio_built_srcs = \
  dummy_auto.f90

# C headers
pio_hdrs = \
  pspio_common.h \
  libxccodes.h \
  pspio_error.h \
  pspio_mesh.h \
  pspio_meshfunc.h \
  pspio_qn.h \
  pspio_potential.h \
  pspio_state.h \
  pspio_projector.h \
  pspio_nlcc.h \
  pspio.h \
  read_upf.h

                    # ------------------------------------ #

#
# Build targets
#

# Libraries
lib_LTLIBRARIES = libpspio.la
libpspio_la_LDFLAGS = -version-info 0:0:0
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
