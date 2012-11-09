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

 $Id: abinit_format6.c 284 2012-11-08 10:58:11Z micael $
*/

/** 
 * @file abinit_util.c
 * @brief utility routines to read and write ABNIIT files
 */
#include <string.h>
#include <ctype.h>
#include "abinit.h"
#include "util.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


int abinit_read_header(FILE *fp, int *format, int *np, int *have_nlcc){
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
