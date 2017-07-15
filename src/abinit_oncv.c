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
 * @file abinit_oncv.c
 * @brief implementation to read and write Hamann ONCV files 
 */
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

#include "pspio_error.h"
#include "pspio_pspdata.h"
#include "util.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


int pspio_oncv_read(FILE *fp, pspio_pspdata_t *pspdata)
{
  char line[PSPIO_STRLEN_LINE];
  int ext_switch[2], il, ir, npts;
  int has_nlcc = -1, has_soc = -1;
  double ekb[2];

  assert(fp != NULL);
  assert(pspdata != NULL);

  /* Initial setup, preparing for the most complex case */
  assert(pspdata->n_projectors == 0);
  assert(pspdata->n_projectors_per_l == NULL);
  FULFILL_OR_EXIT( (pspdata->n_projectors_per_l = (int *) malloc (2 * pspdata->l_max+1 * sizeof(int))) != NULL, PSPIO_ENOMEM );
  memset(pspdata->n_projectors_per_l, 0, (2 * pspdata->l_max + 1) * sizeof(int));

  /* Read numbers of projectors */
  FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  for (il=0; il < pspdata->l_max+1; il++) {
    FULFILL_OR_RETURN( sscanf(line, "%d", &pspdata->n_projectors_per_l[il]) == 1, PSPIO_EFILE_CORRUPT );
  }

  /* Read Spin-orbit coupling descriptor */
  FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  for (il=0; il < 2; il++) {
    FULFILL_OR_RETURN( sscanf(line, "%d", &ext_switch[il]) == 1, PSPIO_EFILE_CORRUPT );
    if ( ((ext_switch[il] == 0) || (ext_switch[il] == 1)) && (has_soc != 1) ) {
      has_soc = 0;
    } else if ( ((ext_switch[il] == 2) || (ext_switch[il] == 3)) && (has_soc != 0) ) {
      has_soc = 1;
    } else {
      RETURN_WITH_ERROR(PSPIO_EFILE_CORRUPT);
    }
  }

  /* Read additional numbers of projectors if needed */
  if ( has_soc == 1 ) {
    FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
    for (il=pspdata->l_max+1; il < pspdata->l_max*2+1; il++) {
      FULFILL_OR_RETURN( sscanf(line, "%d", &pspdata->n_projectors_per_l[il]) == 1, PSPIO_EFILE_CORRUPT );
    }
  }

  /* Update pspdata */
  pspdata->projectors_l_max = -1;
  for (il=0; il < pspdata->l_max*2+1; il++) {
    pspdata->n_projectors += pspdata->n_projectors_per_l[il];
    if ( pspdata->n_projectors_per_l[il] > 0 ) {
      pspdata->projectors_l_max = il;
    }
  }

  /* Read projectors */
  /* FIXME: add SOC case */
  for (il=0; il < pspdata->l_max+1; il++) {
    FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
    FULFILL_OR_RETURN( sscanf(line, "%d %lf %lf", &npts, &ekb[0], &ekb[1]) == 3, PSPIO_EFILE_CORRUPT );
    for (ir=0; ir < pspdata->mesh->np; ir++) {
      FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
    }
  }
}


int pspio_oncv_write(FILE *fp, const pspio_pspdata_t *pspdata)
{
  int ip, ir, l;
  pspio_projector_t *proj1, *proj2;

  assert(fp != NULL);
  assert(pspdata != NULL);

  /*
   * If one considers that the specifications of this format is the
   * way ONCVPSP writes the data, then only meshes of type linear should
   * be allowed. For the moment, we will just check that this is the
   * case.
  */
  assert(pspdata->mesh->type == PSPIO_MESH_LINEAR);

  /* We cannot treat l_max > 5 for now */
  assert(pspdata->l_max < 6);

  /* Make sure we know how many projectors there are for each angular
     momentum */
  FULFILL_OR_RETURN( pspdata->n_projectors_per_l != NULL, PSPIO_ERROR );

  /* FIXME: write extension_switch */

  /* Write projectors */
  for (l=0; l<pspdata->projectors_l_max+1; l++) {
    if ( l == pspdata->l_local ) {
      FULFILL_OR_RETURN( fprintf(fp, "%4d\n", l) > 0, PSPIO_EIO );
      for (ir=0; ir<pspdata->mesh->np; ir++) {
        FULFILL_OR_RETURN( fprintf(fp, "%6d %21.13E %21.13E\n",
          ir+1, pspdata->mesh->r[ir], pspdata->vlocal->v->f[ir]) > 0, PSPIO_EIO );
      }
    } else if ( pspdata->n_projectors_per_l[l] == 1 ) {
      for (ip=0; ip<pspdata->n_projectors; ip++) {
        proj1 = pspdata->projectors[ip];
        if ( proj1->qn->l == l ) {
          FULFILL_OR_RETURN( fprintf(fp, "%4d %21.13E\n", l,
            proj1->energy) > 0, PSPIO_EIO );
          break;
        }
      }
      /* FIXME: output values */
    } else if ( pspdata->n_projectors_per_l[l] == 2 ) {
      proj1 = NULL;
      proj2 = NULL;
      for (ip=0; ip<pspdata->n_projectors; ip++) {
        if ( (proj1 != NULL) && (proj2 != NULL) ) {
          FULFILL_OR_RETURN( fprintf(fp, "%4d %21.13E %21.13E\n", l,
            proj1->energy, proj2->energy) > 0, PSPIO_EIO );
          break;
        }
        if ( proj1 == NULL ) {
          proj1 = pspdata->projectors[ip];
          if ( proj1->qn->l != l ) {
            proj1 = NULL;
          }
        } else if ( proj2 == NULL ) {
          proj2 = pspdata->projectors[ip];
          if ( proj2->qn->l != l ) {
            proj2 = NULL;
          }
        }
      }
      FULFILL_OR_RETURN( (proj1 != NULL) && (proj2 != NULL), PSPIO_ERROR );
      /* FIXME: output values */
    }
  }

  return PSPIO_SUCCESS;
}
