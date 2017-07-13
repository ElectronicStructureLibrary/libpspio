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
 * @file pspio_diff.c
 * @brief Computes the differences between two pseudopotential files
 */

#include <stdlib.h>
#include <stdio.h>

#include "pspio.h"

int main(int argc, char **argv)
{
  char *chk, *ref;
  int fmt = PSPIO_FMT_UNKNOWN, ret, pspcmp[8];
  pspio_pspdata_t *pchk = NULL, *pref = NULL;

  if ( argc < 3 ) {
    return 10;
  }

  fflush(stdout);
  ref = argv[1];
  chk = argv[2];

  if ( argc > 3 ) {
    fmt = atoi(argv[3]);
  }

  ret = pspio_pspdata_alloc(&pref);
  if ( ret == PSPIO_SUCCESS ) {
    ret = pspio_pspdata_alloc(&pchk);
  }

  fprintf(stderr, "REF - BEGIN (%s)\n", ref);
  if ( ret == PSPIO_SUCCESS ) {
    ret = pspio_pspdata_read(pref, fmt, ref);
  }
  fprintf(stderr, "REF - END\n");

  fprintf(stderr, "CHK - BEGIN (%s)\n", chk);
  if ( ret == PSPIO_SUCCESS ) {
    ret = pspio_pspdata_read(pchk, fmt, chk);
  }
  pspio_error_flush(stderr);
  fprintf(stderr, "CHK - END\n");

  if ( ret == PSPIO_SUCCESS ) {
    ret = ( pchk->format_guessed == pref->format_guessed ) ? PSPIO_SUCCESS : PSPIO_ERROR;
    if ( ret != PSPIO_SUCCESS ) {
      ret = pspio_error_add(PSPIO_EFILE_FORMAT, __FILE__, __LINE__, "main");
    }
  }

  /* FIXME: only checking the first state, first potential, and first projector */
  if ( ret == PSPIO_SUCCESS ) {
    pspcmp[0] = pspio_pspinfo_cmp(pspio_pspdata_get_pspinfo(pchk), \
          pspio_pspdata_get_pspinfo(pref));
    pspcmp[1] = pspio_mesh_cmp(pspio_pspdata_get_mesh(pchk), \
          pspio_pspdata_get_mesh(pref));
    pspcmp[2] = pspio_state_cmp(pspio_pspdata_get_state(pchk, 0), \
          pspio_pspdata_get_state(pref, 0));
    pspcmp[3] = pspio_potential_cmp(pspio_pspdata_get_potential(pchk, 0), \
          pspio_pspdata_get_potential(pref, 0));
    pspcmp[4] = pspio_projector_cmp(pspio_pspdata_get_projector(pchk, 0), \
          pspio_pspdata_get_projector(pref, 0));
    pspcmp[5] = pspio_potential_cmp(pspio_pspdata_get_vlocal(pchk), \
          pspio_pspdata_get_vlocal(pref));
    pspcmp[6] = pspio_xc_cmp(pspio_pspdata_get_xc(pchk), \
          pspio_pspdata_get_xc(pref));
    pspcmp[7] = pspio_meshfunc_cmp(pspio_pspdata_get_rho_valence(pchk), \
         pspio_pspdata_get_rho_valence(pref));

    printf("- {ref: '%s', chk: '%s', fmt: %d, info: %d, mesh: %d, state: %d, potential: %d, projector: %d, vlocal: %d, xc: %d, rho_valence: %d}\n", ref, chk, fmt, pspcmp[0], pspcmp[1], pspcmp[2], pspcmp[3], pspcmp[4], pspcmp[5], pspcmp[6], pspcmp[7]);
  }

  pspio_error_flush(stderr);

  return ( ret == PSPIO_SUCCESS ) ? 0 : 1;
}
