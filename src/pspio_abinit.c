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
 * @file pspio_abinit.c
 * @brief implementation to read and write Abinit files 
 */
#include <string.h>
#include <ctype.h>

#include "pspio_abinit.h"
#include "pspio_error.h"
#include "abinit.h"
#include "util.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


int pspio_abinit_read(FILE *fp, pspio_pspdata_t **pspdata, const int format){
  int eid;

  switch(format) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 17:
      eid = PSPIO_ENOSUPPORT;
      break;

    case 5:
    case 6:
      eid = abinit_format6_read(fp, pspdata);
      break;
    default:
      eid = PSPIO_EVALUE;
  }

  return eid;
}

int pspio_abinit_write(FILE *fp, const pspio_pspdata_t *pspdata, const int format){
  int eid;

  ASSERT (pspdata != NULL, PSPIO_ERROR);

  switch(format) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 17:
      eid = PSPIO_ENOSUPPORT;
      break;

    case 5:
    case 6:
      eid = abinit_format6_write(fp, pspdata);
      break;
    default:
      eid = PSPIO_EVALUE;
  }

  return eid;
}
