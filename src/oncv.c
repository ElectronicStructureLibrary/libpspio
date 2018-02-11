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

#include "pspio_common.h"
#include "pspio_error.h"
#include "pspio_pspdata.h"
#include "util.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


int pspio_oncv_read(FILE *fp, pspio_pspdata_t *pspdata)
{
  char line[PSPIO_STRLEN_LINE];
  char *strval;
  int il, ipl, iproj, ir, l, npts, npl;
  double *eofl, *rval, *vofr;
  double **pofr;
  pspio_mesh_t *mesh;
  pspio_meshfunc_t *rhoval;
  pspio_potential_t *vlocal;
  pspio_qn_t *qn;

  assert(fp != NULL);
  assert(pspdata != NULL);
  assert(pspdata->n_projectors > 0);
  assert(pspdata->n_projectors_per_l != NULL);

  /* Init */
  npts = pspio_mesh_get_np(pspdata->mesh);
  mesh = NULL;
  qn = NULL;
  eofl = NULL;
  pofr = NULL;
  rhoval = NULL;
  rval = NULL;
  rval = (double *) malloc(npts*sizeof(double));
  vlocal = NULL;

  /* Projectors */
  iproj = 0;
  for (il=0; il < pspdata->l_max; il++) {

    /* Manage memory to deal with a dynamic number of projectors */
    npl = pspdata->n_projectors_per_l[il];
    if ( npl == 0 ) continue;
    eofl = (double *) malloc(npl*sizeof(double));
    pofr = (double **) malloc(npl*sizeof(vlocal));
    for (ipl=0; ipl < npl; ipl++) {
      pofr[ipl] = (double *) malloc(npts*sizeof(double));
    }

    /* First line: l */
    FULFILL_OR_BREAK( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
    strval = strtok(line, " ");
    FULFILL_OR_BREAK( strval != NULL, PSPIO_EFILE_CORRUPT );
    FULFILL_OR_BREAK( sscanf(strval, "%d", &l) == 1, PSPIO_EFILE_CORRUPT );

    /* First line: [ekb ...] */
    for (ipl=0; ipl < npl; ipl++) {
      strval = strtok(NULL, " ");
      FULFILL_OR_BREAK( strval != NULL, PSPIO_EFILE_CORRUPT );
      FULFILL_OR_BREAK( sscanf(strval, "%lf", &eofl[ipl]) == 1, PSPIO_EFILE_CORRUPT );
    }

    /* Values on the grid: index, radius, [projector ...] */
    for (ir=0; ir < npts; ir++) {
      FULFILL_OR_BREAK( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );

      /* Index */
      strval = strtok(line, " ");
      FULFILL_OR_BREAK( strval != NULL, PSPIO_EFILE_CORRUPT );

      /* radius */
      strval = strtok(NULL, " ");
      FULFILL_OR_BREAK( strval != NULL, PSPIO_EFILE_CORRUPT );
      FULFILL_OR_BREAK( sscanf(strval, "%lf", &rval[ir]) == 1, PSPIO_EFILE_CORRUPT );

      /* [projector ...] */
      for (ipl=0; ipl < npl; ipl++) {
        strval = strtok(NULL, " ");
        FULFILL_OR_BREAK( strval != NULL, PSPIO_EFILE_CORRUPT );
        FULFILL_OR_BREAK( sscanf(strval, "%lf", &pofr[ipl][ir]) == 1, PSPIO_EFILE_CORRUPT );
      }
    }

    /* Configure projectors, assuming a linear grid */
    SUCCEED_OR_BREAK( pspio_mesh_alloc(&mesh, npts) );
    SUCCEED_OR_BREAK( pspio_mesh_init_from_parameters(mesh, PSPIO_MESH_LINEAR, (rval[npts-1]-rval[0])/((double)(npts-1)), rval[0]) );
    SUCCEED_OR_BREAK( pspio_qn_alloc(&qn) );
    SUCCEED_OR_BREAK( pspio_qn_init(qn, 0, l, 0.0) );
    for (ipl=0; ipl < npl; ipl++) {
      SUCCEED_OR_BREAK( pspio_projector_init(pspdata->projectors[iproj+ipl], qn, eofl[ipl], mesh, pofr[ipl]) );
      free(pofr[ipl]);
    }
    pspio_mesh_free(mesh);
    mesh = NULL;
    pspio_qn_free(qn);
    qn = NULL;
    free(eofl);
    eofl = NULL;
    free(pofr);
    pofr = NULL;

    iproj += npl;
  }

  /* Return on error after making sure internal variables are freed */
  free(rval);
  RETURN_ON_DEFERRED_ERROR;

  /* Local potential */
  rval = NULL;
  rval = (double *) malloc(npts*sizeof(double));
  vofr = NULL;
  vofr = (double *) malloc(npts*sizeof(double));
  DEFER_TEST_ERROR( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  DEFER_TEST_ERROR( sscanf(line, "%d", &l) == 1, PSPIO_EFILE_CORRUPT );
  assert(l == pspio_pspdata_get_l_local(pspdata));
  for (ir=0; ir < npts; ir++) {
    FULFILL_OR_BREAK(fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO);
    FULFILL_OR_BREAK(sscanf(line, "%d %lf %lf", &ipl, &rval[ir], &vofr[ir]) == 3, PSPIO_EFILE_CORRUPT);
  }
  DEFER_FUNC_ERROR( pspio_qn_alloc(&qn) );
  DEFER_FUNC_ERROR( pspio_qn_init(qn, 0, l, 0.0) );
  DEFER_FUNC_ERROR( pspio_potential_alloc(&vlocal, npts) );
  DEFER_FUNC_ERROR( pspio_potential_init(vlocal, qn, pspdata->mesh, vofr) );
  DEFER_FUNC_ERROR( pspio_pspdata_set_vlocal(pspdata, vlocal) );

  /* Return on error after making sure internal variables are freed */
  pspio_potential_free(vlocal);
  pspio_qn_free(qn);
  free(rval);
  free(vofr);
  RETURN_ON_DEFERRED_ERROR;

  /* Non-linear core-corrections */
  if ( pspio_xc_get_nlcc_scheme(pspdata->xc) != PSPIO_NLCC_NONE ) {
    double r12, *cd, *cdp, *cdpp;

    /* Allocate memory */
    cd = NULL;
    cd = (double *) malloc (npts*sizeof(double));
    FULFILL_OR_EXIT(cd != NULL, PSPIO_ENOMEM);
    cdp = NULL;
    cdp = (double *) malloc (npts*sizeof(double));
    FULFILL_OR_EXIT(cdp != NULL, PSPIO_ENOMEM);
    cdpp = NULL;
    cdpp = (double *) malloc (npts*sizeof(double));
    FULFILL_OR_EXIT(cdpp != NULL, PSPIO_ENOMEM);

    /* Read core density */
    for (ir=0; ir < npts; ir++) {
      if ( ir != 0 ) {
	FULFILL_OR_BREAK(fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO);
      }

      FULFILL_OR_BREAK(sscanf(line, "%lf %lf %lf %lf", &r12, &cd[ir],
        &cdp[ir], &cdpp[ir]) == 4, PSPIO_EFILE_CORRUPT);
      cd[ir] /= (M_PI*4.0); cdp[ir] /= (M_PI*4.0); cdpp[ir] /= (M_PI*4.0);
    }

    /* Store the non-linear core corrections in the pspdata structure */
    SKIP_FUNC_ON_ERROR(pspio_xc_set_nlcc_density(pspdata->xc, pspdata->mesh, cd, cdp, cdpp));

    /* Return on error after making sure internal variables are freed */
    free(cd);
    free(cdp);
    free(cdpp);
    RETURN_ON_DEFERRED_ERROR;
  }

  /* Valence density */
  if ( pspdata->rho_valence_type != PSPIO_RHOVAL_NONE ) {
    /* TODO: read values */
    SKIP_FUNC_ON_ERROR( pspio_meshfunc_alloc(&rhoval, npts) );
    SKIP_FUNC_ON_ERROR( pspio_meshfunc_init(rhoval, pspdata->mesh, vofr, NULL, NULL) );
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
