#!/usr/bin/env python
#
# Copyright (C) 2014-2016 Micael Oliveira <micael.oliveira@mpsd.mpg.de>
#                         Yann Pouillon <notifications@materialsevolution.es>
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

"""\
This script generates Fortran constants in src/fpspio.F90 from those
declared in src/pspio_common.h, to ensure a perfect match between C
and Fortran namespaces and avoid the inclusion of pspio_common.h in
Fortran files.
"""

__author__ = "Yann Pouillon"
__copyright__ = "Copyright (C) 2014-2015 Yann Pouillon"
__license__ = "LGPL version 3"
__version__ = "0.1"

import os
import re

# Init
re_c_cmts  = re.compile("(/\*.*\*/|//.*)")
re_cst_def = re.compile("^#define ")
re_cst_f90 = re.compile("  !%%% BEGIN PSPIO CONSTANTS.*  !%%% END PSPIO CONSTANTS", flags=re.MULTILINE+re.DOTALL)

# Translate C definitions into Fortran
f90_defs = "  !%%% BEGIN PSPIO CONSTANTS\n"
for line in file("../src/pspio_common.h", "r").readlines():
  if ( re_cst_def.match(line) ):
    line = re.sub(re_c_cmts, "", line)
    line = line.split()
    cst_name = line[1]
    cst_value = " ".join(line[2:])
    if ( cst_value != "" ):
      try:
        cst_value = int(cst_value)
        f90_defs += "  integer(c_int), parameter, public :: %s = %d\n" % \
          (cst_name, cst_value)
      except:
        # The following should be change to use the iso_c_bindings
        f90_defs += "  character(len=*), parameter, public :: %s = %s\n" % \
          (cst_name, cst_value)
f90_defs += "  !%%% END PSPIO CONSTANTS"

# Replace existing Fortran definitions
f90_file = "src/fpspio.F90"
f90_src  = file(f90_file, "r").read()
file(f90_file, "w").write(re.sub(re_cst_f90, f90_defs, f90_src))
