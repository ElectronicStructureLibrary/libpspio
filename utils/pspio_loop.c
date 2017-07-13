/* Copyright (C) 2017      Yann Pouillon <notifications@materialsevolution.es>
 *
 * This file is part of Libpspio.
 *
 * Libpspio is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, version 3 of the License, or (at your option) any later
 * version.
 *
 * Libpspio is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Libpspio.  If not, see <http://www.gnu.org/licenses/> or write to
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301  USA.
 */

/**
 * @file pspio_loop.c
 * @brief Reads a pseudopotential file and writes a copy in the same format
 */

#include <stdlib.h>
#include <stdio.h>

#include "pspio.h"

int main(int argc, char **argv)
{
  char *dst, *src;
  int fmt = PSPIO_FMT_UNKNOWN, rd1 = -1, rd2 = -1, ret = -1, wr1 = -1;
  pspio_pspdata_t *pspdata = NULL;

  if ( argc < 3 ) {
    return 10;
  }

  src = argv[1];
  dst = argv[2];

  ret = pspio_pspdata_alloc(&pspdata);

  if ( ret == PSPIO_SUCCESS) {
    ret = pspio_pspdata_read(pspdata, PSPIO_FMT_UNKNOWN, src);
    rd1 = ret;
  }

  if ( ret == PSPIO_SUCCESS) {
    fmt = pspdata->format_guessed;
    ret = pspio_pspdata_write(pspdata, fmt, dst);
    wr1 = ret;
  }

  if ( ret == PSPIO_SUCCESS) {
    pspio_pspdata_reset(pspdata);
    ret = pspio_pspdata_read(pspdata, PSPIO_FMT_UNKNOWN, dst);
    rd2 = ret;
  }

  printf("- {ref: '%s', chk: '%s', fmt: %d, rd1: %d, rd2: %d, wr1: %d, err: %d}\n", src, dst, fmt, rd1, rd2, wr1, ret);
  pspio_error_flush(stderr);

  return ( ret == PSPIO_SUCCESS ) ? 0 : 1;
}
