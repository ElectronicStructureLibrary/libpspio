/* Copyright (C) 2012-2017 Micael Oliveira <micael.oliveira@mpsd.mpg.de>
 *                         Yann Pouillon <devops@materialsevolution.es>
 *
 * This file is part of Libpspio.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * version 2.0. If a copy of the MPL was not distributed with this file, You
 * can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Libpspio is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the Mozilla Public License version 2.0 for
 * more details.
 */

/** 
 * @file oncv.c
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
  int i, l, np, n_potentials, ir, has_nlcc;
  double zvalence, r12;
  double *wf, *r, *v;
  pspio_qn_t *qn = NULL;

  assert(fp != NULL);
  assert(pspdata != NULL); 

  /* Read header */
  FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  FULFILL_OR_RETURN( sscanf(line, "%lf %d", &zvalence, &n_potentials ) == 2, PSPIO_EFILE_CORRUPT );
  SUCCEED_OR_RETURN( pspio_pspdata_set_zvalence(pspdata, zvalence) );
  SUCCEED_OR_RETURN( pspio_pspdata_set_n_potentials(pspdata, n_potentials) );
  for (i=0; i<10; i++) {
    /* We ignore the next 10 lines, as they contain no information */
    FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  }
  SUCCEED_OR_RETURN( pspio_pspdata_set_l_max(pspdata, n_potentials-1) );
  SUCCEED_OR_RETURN( pspio_pspdata_set_n_states(pspdata, n_potentials) );


  /* Read mesh, potentials and wavefunctions */
  for (l=0; l < pspdata->l_max+1; l++) {
    FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
    FULFILL_OR_RETURN( sscanf(line, "%d %lf", &np, &r12 ) == 2, PSPIO_EFILE_CORRUPT );

    /* Allocate temporary data */
    SUCCEED_OR_RETURN( pspio_qn_alloc(&qn) );
    r = (double *) malloc (np*sizeof(double));
    FULFILL_OR_EXIT( r != NULL, PSPIO_ENOMEM );
    v = (double *) malloc (np*sizeof(double));
    FULFILL_OR_EXIT( v != NULL, PSPIO_ENOMEM );
    wf = (double *) malloc (np*sizeof(double));
    FULFILL_OR_EXIT(wf != NULL, PSPIO_ENOMEM);

    /* Read first line of block */
    for (ir=0; ir<np; ir++) {
      FULFILL_OR_BREAK( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
      FULFILL_OR_BREAK( sscanf(line, "%d %lf %lf %lf", &i, &r[ir],
        &wf[ir], &v[ir]) == 4, PSPIO_EFILE_CORRUPT );
      wf[ir] = wf[ir]/r[ir];
    }

    if ( l == 0 ) {
      /*
       * We will use the first block to generate the mesh, as the mesh
       * is supposed to be the same for all blocks (at least this is
       * the behavior of ONCVPSP)
      */
      SKIP_FUNC_ON_ERROR( pspio_mesh_alloc(&pspdata->mesh, np) );
      SKIP_CALL_ON_ERROR( pspio_mesh_init_from_points(pspdata->mesh, r, NULL) );
    }

    /* Set pseudopotential and wavefunction */
    SKIP_FUNC_ON_ERROR( pspio_qn_init(qn, 0, l, 0.0) );
    SKIP_FUNC_ON_ERROR(
      pspio_potential_alloc(&pspdata->potentials[LJ_TO_I(l,0.0)], np) );
    SKIP_FUNC_ON_ERROR( pspio_potential_init(pspdata->potentials[LJ_TO_I(l,0.0)],
      qn, pspdata->mesh, v) );
    SKIP_FUNC_ON_ERROR( pspio_state_alloc(&pspdata->states[l], np) );
    SKIP_FUNC_ON_ERROR( pspio_state_init(pspdata->states[l], 0.0, qn,
      0.0, 0.0, pspdata->mesh, wf, NULL) );

    /* Free temporary data */
    free(r);
    free(v);
    free(wf);
    pspio_qn_free(qn);
    qn = NULL;

    /* Return on error after making sure internal variables are freed */
    RETURN_ON_DEFERRED_ERROR;
  }

  /* If not done yet, then allocate the xc structure */
  if ( pspdata->xc == NULL ) {
    SUCCEED_OR_RETURN( pspio_xc_alloc(&pspdata->xc) );
  }

  /* Non-linear core-corrections */
  has_nlcc = ( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL );
  if ( has_nlcc ) {
    double *cd, *cdp, *cdpp;

    SUCCEED_OR_RETURN( pspio_xc_set_nlcc_scheme(pspdata->xc, PSPIO_NLCC_ONCV) );

    /* Allocate memory */
    cd = (double *) malloc (np*sizeof(double));
    FULFILL_OR_EXIT(cd != NULL, PSPIO_ENOMEM);
    cdp = (double *) malloc (np*sizeof(double));
    FULFILL_OR_EXIT(cdp != NULL, PSPIO_ENOMEM);
    cdpp = (double *) malloc (np*sizeof(double));
    FULFILL_OR_EXIT(cdpp != NULL, PSPIO_ENOMEM);

    /* Read core density */
    for (ir=0; ir<np; ir++) {
      if ( ir != 0 ) {
	FULFILL_OR_BREAK(fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO);
      }

      FULFILL_OR_BREAK(sscanf(line, "%lf %lf %lf %lf", &r12, &cd[ir],
        &cdp[ir], &cdpp[ir]) == 4, PSPIO_EFILE_CORRUPT);
      cd[ir] /= (M_PI*4.0); cdp[ir] /= (M_PI*4.0); cdpp[ir] /= (M_PI*4.0);
    }

    /* Store the non-linear core corrections in the pspdata structure */
    SKIP_FUNC_ON_ERROR(pspio_xc_set_nlcc_density(pspdata->xc, pspdata->mesh, cd, cdp, cdpp));

    /* Free temporary variables */
    free(cd); 
    free(cdp); 
    free(cdpp);

    /* Return on error after making sure internal variables are freed */
    RETURN_ON_DEFERRED_ERROR;
  }
  
  /* We do not know the symbol (note that it might have been set somewhere else) */
  if ( pspdata->symbol == NULL ) {
    SUCCEED_OR_RETURN( pspio_pspdata_set_symbol(pspdata, "N/D") );
  }

  return PSPIO_SUCCESS;
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

  /* Write projectors */
  for (l=0; l<pspdata->projectors_l_max+1; l++) {
    if ( l == pspdata->l_local ) {
      FULFILL_OR_RETURN( fprintf(fp, "%4d\n", l) > 0, PSPIO_EIO );
      for (ir=0; ir<pspdata->mesh->np; ir++) {
        FULFILL_OR_RETURN( fprintf(fp, "%6d %21.13E %21.13E\n",
          ir+1, pspdata->mesh->r[ir], pspdata->vlocal->v[ir]) > 0, PSPIO_EIO );
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
