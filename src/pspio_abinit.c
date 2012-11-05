/*
 Copyright (C) 2012 M. Oliveira, Y. Pouillon

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 3 of the License, or 
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

 $Id: pspio_abinit.c 256 2012-09-06 10:06:13Z micael $
*/

/** 
 * @file pspio_abinit.c
 * @brief implementation to read and write Abinit files 
 */
#include <string.h>
#include <ctype.h>
#include "abinit.h"
#include "util.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


int pspio_abinit1_read(FILE *fp, pspio_pspdata_t **pspdata){
  int np, has_nlcc;

  // Make sure we are at the beginning of the file
  rewind(fp);

  HANDLE_FUNC_ERROR(abinit_read_info(fp, pspdata));

  //At the moment the wave equation type is not defined in the header,
  //so we set it to 0 if the ADDINFO tag is not present, and to DIRAC if it is
  if (abinit_tag_isdef(fp,"PP_ADDINFO")){
    (*pspdata)->wave_eq = DIRAC;
  } else {
    (*pspdata)->wave_eq = 0;
  }

  HANDLE_FUNC_ERROR(abinit_read_header(fp, &np, pspdata));
  HANDLE_FUNC_ERROR(abinit_read_mesh(fp, np, pspdata));
  HANDLE_FUNC_ERROR(pspio_xc_has_nlcc((*pspdata)->xc, &has_nlcc));
  if (has_nlcc) {
    HANDLE_FUNC_ERROR(abinit_read_nlcc(fp, np, pspdata));
  }
  HANDLE_FUNC_ERROR(abinit_read_nonlocal(fp, np, pspdata));
  HANDLE_FUNC_ERROR(abinit_read_pswfc(fp, np, pspdata));
  HANDLE_FUNC_ERROR(abinit_read_local(fp, np, pspdata));
  HANDLE_FUNC_ERROR(abinit_read_rhoatom(fp, np, pspdata));

  return PSPIO_SUCCESS;
}

int pspio_abinit_write(FILE *fp, const pspio_pspdata_t *pspdata){
  int has_nlcc;

  ASSERT (pspdata != NULL, PSPIO_ERROR);

  HANDLE_FUNC_ERROR(abinit_write_info(fp, pspdata));
  HANDLE_FUNC_ERROR(abinit_write_header(fp, pspdata));
  HANDLE_FUNC_ERROR(abinit_write_mesh(fp, pspdata));
  HANDLE_FUNC_ERROR(pspio_xc_has_nlcc(pspdata->xc, &has_nlcc));
  if (has_nlcc) {
    HANDLE_FUNC_ERROR(abinit_write_nlcc(fp, pspdata));
  }
  HANDLE_FUNC_ERROR(abinit_write_local(fp, pspdata));
  HANDLE_FUNC_ERROR(abinit_write_nonlocal(fp, pspdata));
  HANDLE_FUNC_ERROR(abinit_write_pswfc(fp, pspdata));
  HANDLE_FUNC_ERROR(abinit_write_rhoatom(fp, pspdata));
  if (pspdata->wave_eq == DIRAC) {
    HANDLE_FUNC_ERROR(abinit_write_addinfo(fp, pspdata));
  }

  return PSPIO_SUCCESS;
}
