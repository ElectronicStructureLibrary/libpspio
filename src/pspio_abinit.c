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


int pspio_abinit_read_header(FILE *fp, int *format, int *np, int *have_nlcc){
  char line[MAX_STRLEN], buffer[MAX_STRLEN];
  char pspdat[6];
  char *line4;
  int pspcod, pspxc, lmax, lloc, mmax;
  int exchange, correlation, il;
  double zatom, zval, r2well, rchrg, fchrg, qchrg;

  // Line 1: skip title
  CHECK_ERROR( fgets(line, MAX_STRLEN, fp) != NULL, PSPIO_EIO);

  // Line 2: read atomic number, Z valence, psp date
  CHECK_ERROR( fgets(line, MAX_STRLEN, fp) != NULL, PSPIO_EIO);
  CHECK_ERROR( sscanf(line, "%f %f %s %s", 
    &zatom, &zval, pspdat, buffer ) == 1, PSPIO_EIO);

  // Line 3: read pspcod, pspxc, lmax, lloc, mmax, r2well
  CHECK_ERROR( fgets(line, MAX_STRLEN, fp) != NULL, PSPIO_EIO);
  CHECK_ERROR( sscanf(line, "%d %d %d %d %d %f %s",
    &pspcod, &pspxc, &lmax, &lloc, &mmax, &r2well, buffer) == 1, PSPIO_EIO);
  *np = mmax;

  // FIXME: assuming pspsxc = exchange * 1000 + correlation
  if ( pspxc >= 1000 ) {
    exchange = pspxc / 1000;
    correlation = pspxc % 1000;
  }

  // Line 4: read rchrg, fchrg, qchrg if NLCC
  CHECK_ERROR( fgets(line, MAX_STRLEN, fp) != NULL, PSPIO_EIO);
  have_nlcc = 0;
  line4 = strndup(line, 3);
  if ( strcmp("4--", line4) != 0 ) {
    CHECK_ERROR( sscanf(line, "%d %d %d %d %d %f %s",
      &rchrg, &fchrg, &qchrg, buffer) == 1, PSPIO_EIO );
    if ( abs(fchrg) >= 1.0e-14 ) {
      have_nlcc = 1;
    }
  }

  return PSPIO_SUCCESS;
}


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
      eid = abinit6_read(fp, pspdata);
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
      eid = abinit6_write(fp, pspdata);
      break;
    default:
      eid = PSPIO_EVALUE;
  }

  return eid;
}
