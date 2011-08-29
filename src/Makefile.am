#
# Makefile for the sources of Libpspio
#

                    # ------------------------------------ #

#
# Source files
#

# Hand-written
pio_srcs = \
  read_abinit_header.c

# Script-generated
pio_built_srcs = \
  dummy_auto.f90

# C headers
pio_hdrs = \
  libxccodes.h \
  pspio.h

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
