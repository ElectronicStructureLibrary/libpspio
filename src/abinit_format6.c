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

 $Id$
*/

/** 
 * @file abinit_format6.c
 * @brief routines to read PSPIO_FMT_UPF files 
 */
#include <stdio.h>

#include <string.h>
#include <ctype.h>
#include "pspio_common.h"
#include "pspio_fhi.h"
#include "abinit.h"
#include "util.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif

int abinit_format6_read(FILE *fp, pspio_pspdata_t **pspdata) {
  int format, np, have_nlcc;

  printf("A6_DEBUG: reading header begin\n");
  HANDLE_FUNC_ERROR(abinit_read_header(fp, &format, &np, &have_nlcc, pspdata));
  printf("A6_DEBUG: reading header end (format = %d)\n", format);
  CHECK_ERROR(format == PSPIO_FMT_ABINIT_6, PSPIO_EFILE_FORMAT);
  printf("A6_DEBUG: reading format 6\n");
  HANDLE_FUNC_ERROR(pspio_fhi_read(fp, pspdata));
  printf("A6_DEBUG: n_states = %d\n", (*pspdata)->n_states);

  return PSPIO_SUCCESS;
}


int abinit_format6_write(FILE *fp, const pspio_pspdata_t *pspdata) {
  HANDLE_FUNC_ERROR(abinit_write_header(fp, PSPIO_FMT_ABINIT_6, pspdata));
  HANDLE_FUNC_ERROR(pspio_fhi_write(fp, pspdata));

  return PSPIO_SUCCESS;
}
