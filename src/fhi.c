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
 * @file fhi.c
 * @brief implementation to read and write in FHI files 
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


int pspio_fhi_read(FILE *fp, pspio_pspdata_t *pspdata)
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
       * the behavior of FHI98PP)
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

    SUCCEED_OR_RETURN( pspio_xc_set_nlcc_scheme(pspdata->xc, PSPIO_NLCC_FHI) );

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
  if (strcmp(pspdata->symbol, "")) {
    SUCCEED_OR_RETURN( pspio_pspdata_set_symbol(pspdata, "N/D") );
  }

  return PSPIO_SUCCESS;
}

int pspio_fhi_write(FILE *fp, const pspio_pspdata_t *pspdata)
{
  int i, l, is, in, ir;
  double wf, v, r;

  assert(fp != NULL);
  assert(pspdata != NULL);

  /*
   * If one considers that the specifications of this format is the
   * way FHI98PP writes the data, then only meshes of type log1 should
   * be allowed. For the moment, we will just check that this is the
   * case.
  */
  assert(pspdata->mesh->type == PSPIO_MESH_LOG1);

  /* Write header */
  FULFILL_OR_RETURN( fprintf(fp, "%20.14E   %d\n", pspdata->zvalence,
    pspdata->l_max+1) > 0, PSPIO_EIO );
  FULFILL_OR_RETURN( fprintf(fp, " 0.0000    0.0000    0.0000   0.0000\n") > 0,
    PSPIO_EIO );
  for (i=0; i<9; i++) {
    FULFILL_OR_RETURN( fprintf(fp, "  0.0000    .00e+00   .00e+00\n") > 0,
      PSPIO_EIO );
  }

  /* Write mesh, pseudopotentials, and wavefunctions */
  for (l=0; l<pspdata->l_max+1; l++) {
    FULFILL_OR_RETURN( fprintf(fp, "%-4d %20.14E\n", pspdata->mesh->np,
      pspdata->mesh->r[1]/pspdata->mesh->r[0]) > 0, PSPIO_EIO );

    i = LJ_TO_I(l,0.0);

    in = 0;
    while ((is = pspdata->qn_to_istate[in][i]) < 0)
      in ++;

    /* This format is not suitable for j-dependent pseudos */
    FULFILL_OR_RETURN( pspio_qn_get_j(pspio_state_get_qn(pspdata->states[is])) == 0.0,
      PSPIO_EVALUE );

    for (ir=0; ir<pspdata->mesh->np; ir++) {
      r = pspdata->mesh->r[ir];
      wf = pspio_state_wf_eval(pspdata->states[is], r);
      v = pspio_potential_eval(pspdata->potentials[i], r);
      wf = wf*r;

      FULFILL_OR_RETURN( fprintf(fp, "%4d %20.14E %20.14E %20.14E\n", ir+1,
        pspdata->mesh->r[ir], wf, v) > 0, PSPIO_EIO );
    }
  }

  /* Write non-linear core corrections */
  if (pspio_xc_has_nlcc(pspdata->xc)) {
    const pspio_meshfunc_t *nlcc_dens = NULL;
    double cd, cdp, cdpp;

    nlcc_dens = pspio_xc_get_nlcc_density(pspdata->xc);

    for (ir=0; ir<pspdata->mesh->np; ir++) {
      r = pspdata->mesh->r[ir];
      cd = pspio_meshfunc_eval(nlcc_dens, r);
      cdp = pspio_meshfunc_eval_deriv(nlcc_dens, r);
      cdpp = pspio_meshfunc_eval_deriv2(nlcc_dens, r);
      cd *= M_PI*4.0; cdp *= M_PI*4.0; cdpp *= M_PI*4.0;

      FULFILL_OR_RETURN( fprintf(fp, " %18.12E %18.12E %18.12E %18.12E\n",
        r, cd, cdp, cdpp) > 0, PSPIO_EIO );
    }
  }

  return PSPIO_SUCCESS;
}
