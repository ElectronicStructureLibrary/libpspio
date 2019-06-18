#!/usr/bin/env python
#
# Copyright (C) 2014-2017 Micael Oliveira <micael.oliveira@mpsd.mpg.de>
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

"""\
This script generates Fortran constants in src/pspiof.F90 from those
declared in src/pspio_common.h, to ensure a perfect match between C
and Fortran namespaces and avoid the inclusion of pspio_common.h in
Fortran files.
"""

import os
import re


__author__ = "Yann Pouillon"
__copyright__ = "Copyright (C) 2014-2015 Yann Pouillon"
__license__ = "LGPL version 3"
__version__ = "0.2"

# Init
re_c_cmts = re.compile("(/\*.*\*/|//.*)")
re_cst_def = re.compile("^#define ")
re_cst_f90 = re.compile(
    "  !%%% BEGIN PSPIO CONSTANTS.*  !%%% END PSPIO CONSTANTS",
    flags=re.MULTILINE+re.DOTALL)

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
                f90_defs += "  integer(c_int), parameter, public :: {name} = {value:d}\n".format(name=cst_name, value=cst_value)
            except:
                # The following should be changed to use iso_c_binding
                f90_defs += "  character(len=*), parameter, public :: {name} = {value}\n".format(name=cst_name, value=cst_value)
f90_defs += "  !%%% END PSPIO CONSTANTS"

# Replace existing Fortran definitions
f90_path = "src/pspiof.F90"
with open(f90_path, "r+") as f90_file:
    f90_src = f90_file.read()
    f90_file.seek(0)
    f90_file.write(re.sub(re_cst_f90, f90_defs, f90_src))
    f90_file.truncate()
