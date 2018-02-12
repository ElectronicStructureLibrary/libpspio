#!/bin/sh
#
# Copyright (C) 2012-2017 Yann Pouillon <devops@materialsevolution.es>
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

# Note: this script is temporary and will be removed upon release.

# Stop at first error and echo commands
set -ev

# Check that we are in the correct directory
test -s "configure.ac" -a -s "src/pspiof.F90" || exit 0

# Init build parameters
export FC="gfortran"
export FCFLAGS="-O0 -g3 -ggdb -Wall -Wextra -fbounds-check -fno-inline"

# Prepare source tree
./wipeout.sh
./autogen.sh

# Check default build
mkdir tmp-minimal
cd tmp-minimal
../configure
make dist
make
make clean && make -j4
make check
mkdir install-minimal
make install DESTDIR="${PWD}/install-minimal"
ls -lR install-minimal >install-minimal.log
cd ..

# Make distcheck
mkdir tmp-distcheck
cd tmp-distcheck
../configure
make distcheck -j4
make distcleancheck

# Clean-up the mess
rm -rf tmp-minimal tmp-distcheck
