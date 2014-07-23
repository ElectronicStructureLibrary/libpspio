#!/usr/bin/env python

import os
import re

# Init
re_c_cmts  = re.compile("(/\*.*\*/|//.*)")
re_cst_def = re.compile("^#define ")
re_cst_f90 = re.compile("  !%%% BEGIN PSPIO CONSTANTS.*  !%%% END PSPIO CONSTANTS", flags=re.MULTILINE+re.DOTALL)

# Translate C definitions into Fortran
f90_defs = "  !%%% BEGIN PSPIO CONSTANTS\n"
for line in file("src/pspio_common.h", "r").readlines():
  if ( re_cst_def.match(line) ):
    line = re.sub(re_c_cmts, "", line)
    line = line.split()
    cst_name = line[1]
    cst_value = " ".join(line[2:])
    if ( cst_value != "" ):
      try:
        cst_value = int(cst_value)
        f90_defs += "  integer, parameter, public :: %s = %d\n" % \
          (cst_name, cst_value)
      except:
        f90_defs += "  character(len=*), parameter, public :: %s = %s\n" % \
          (cst_name, cst_value)
f90_defs += "  !%%% END PSPIO CONSTANTS"

# Replace existing Fortran definitions
f90_file = "src/fortran/pspio_f90_types.F90"
f90_src  = file(f90_file, "r").read()
file(f90_file, "w").write(re.sub(re_cst_f90, f90_defs, f90_src))
