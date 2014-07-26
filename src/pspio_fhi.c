/*
 Copyright (C) 2011-2012 M. Oliveira

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

*/

/** 
 * @file pspio_fhi.c
 * @brief implementation to read and write in FHI files 
 */
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

#include "pspio_error.h"
#include "pspio_xc_funcs.h"
#include "pspio_pspdata.h"
#include "util.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


int pspio_fhi_read(FILE *fp, pspio_pspdata_t **pspdata) {
  char line[PSPIO_STRLEN_LINE];
  int i, l, np, ir, has_nlcc;
  double r12;
  double *wf, *r, *v;
  pspio_qn_t *qn = NULL;

  assert(fp != NULL);
  assert((*pspdata) != NULL); 

  // Read header
  FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  FULFILL_OR_RETURN( sscanf(line, "%lf %d", &(*pspdata)->zvalence, &(*pspdata)->n_potentials ) == 2, PSPIO_EFILE_CORRUPT );
  for (i=0; i<10; i++) { 
    // We ignore the next 10 lines, as they contain no information
    FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  }
  (*pspdata)->l_max = (*pspdata)->n_potentials - 1;
  (*pspdata)->n_states = (*pspdata)->n_potentials;


  // Allocate states and potentials
  (*pspdata)->states = (pspio_state_t **) malloc ( (*pspdata)->n_states*sizeof(pspio_state_t *));
  FULFILL_OR_EXIT( (*pspdata)->states != NULL, PSPIO_ENOMEM );
  for (i=0; i<(*pspdata)->n_states; i++) {
    (*pspdata)->states[i] = NULL;
  }
  (*pspdata)->potentials = (pspio_potential_t **) malloc ( (*pspdata)->n_potentials*sizeof(pspio_potential_t *));
  FULFILL_OR_EXIT( (*pspdata)->potentials != NULL, PSPIO_ENOMEM );
  for (i=0; i<(*pspdata)->n_potentials; i++) {
    (*pspdata)->potentials[i] = NULL;
  }

  
  // Read mesh, potentials and wavefunctions
  for (l=0; l < (*pspdata)->l_max+1; l++) {
    FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
    FULFILL_OR_RETURN( sscanf(line, "%d %lf", &np, &r12 ) == 2, PSPIO_EFILE_CORRUPT );

    // Allocate temporary data
    SUCCEED_OR_RETURN( pspio_qn_alloc(&qn) );
    r = (double *) malloc (np*sizeof(double));
    FULFILL_OR_EXIT( r != NULL, PSPIO_ENOMEM );
    v = (double *) malloc (np*sizeof(double));
    FULFILL_OR_EXIT( v != NULL, PSPIO_ENOMEM );
    wf = (double *) malloc (np*sizeof(double));
    FULFILL_OR_EXIT(wf != NULL, PSPIO_ENOMEM);

    // Read first line of block
    for (ir=0; ir<np; ir++) {
      FULFILL_OR_BREAK( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
      FULFILL_OR_BREAK( sscanf(line, "%d %lf %lf %lf", &i, &r[ir],
        &wf[ir], &v[ir]) == 4, PSPIO_EFILE_CORRUPT );
      wf[ir] = wf[ir]/r[ir];
    }

    if ( l == 0 ) {
      // We will use the first block to generate the mesh, as the mesh
      // is supposed to be the same for all blocks (at least this is
      // the behavior of FHI98PP)
      SKIP_FUNC_ON_ERROR( pspio_mesh_alloc(&(*pspdata)->mesh, np) );
      SKIP_CALL_ON_ERROR( pspio_mesh_init_from_points(&(*pspdata)->mesh,
        r, NULL) );
    }

    // Set pseudopotential and wavefunction
    SKIP_FUNC_ON_ERROR( pspio_qn_set(&qn, 0, l, 0.0) );
    SKIP_FUNC_ON_ERROR(
      pspio_potential_alloc(&(*pspdata)->potentials[LJ_TO_I(l,0.0)], np) );
    SKIP_FUNC_ON_ERROR( pspio_potential_set(&(*pspdata)->potentials[LJ_TO_I(l,0.0)],
      qn, (*pspdata)->mesh, v) );
    SKIP_FUNC_ON_ERROR( pspio_state_alloc(&(*pspdata)->states[l], np) );
    SKIP_FUNC_ON_ERROR( pspio_state_set(&(*pspdata)->states[l], 0.0, "", qn,
      0.0, 0.0, (*pspdata)->mesh, wf) );

    // Free temporary data
    free(r);
    free(v);
    free(wf);
    pspio_qn_free(&qn);

    // Return on error after making sure internal variables are freed
    RETURN_ON_DEFERRED_ERROR;
  }

  // If not done yet, then allocate the xc structure
  if ( (*pspdata)->xc == NULL ) {
    SUCCEED_OR_RETURN( pspio_xc_alloc(&(*pspdata)->xc) );
  }

  // Non-linear core-corrections
  has_nlcc = ( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL );
  if ( has_nlcc ) {
    double *cd, *cdp, *cdpp;

    pspio_xc_set_nlcc_scheme(&(*pspdata)->xc, PSPIO_NLCC_FHI);

    // Allocate memory
    cd = (double *) malloc (np*sizeof(double));
    FULFILL_OR_EXIT(cd != NULL, PSPIO_ENOMEM);
    cdp = (double *) malloc (np*sizeof(double));
    FULFILL_OR_EXIT(cdp != NULL, PSPIO_ENOMEM);
    cdpp = (double *) malloc (np*sizeof(double));
    FULFILL_OR_EXIT(cdpp != NULL, PSPIO_ENOMEM);

    // Read core density
    for (ir=0; ir<np; ir++) {
      if ( ir != 0 ) {
	FULFILL_OR_BREAK(fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO);
      }

      FULFILL_OR_BREAK(sscanf(line, "%lf %lf %lf %lf", &r12, &cd[ir],
        &cdp[ir], &cdpp[ir]) == 4, PSPIO_EFILE_CORRUPT);
      cd[ir] /= (M_PI*4.0); cdp[ir] /= (M_PI*4.0); cdpp[ir] /= (M_PI*4.0);
    }

    // Store the non-linear core corrections in the pspdata structure
    SKIP_FUNC_ON_ERROR(pspio_xc_set_nlcc_density(&(*pspdata)->xc,
      (*pspdata)->mesh, cd, cdp, cdpp));

    // Free temporary variables
    free(cd); 
    free(cdp); 
    free(cdpp);

    // Return on error after making sure internal variables are freed
    RETURN_ON_DEFERRED_ERROR;
  }

  //We do not know the symbol (note that it might have been set somewhere else)
  if ( (*pspdata)->symbol == NULL ) {
    (*pspdata)->symbol = (char *) malloc (3*sizeof(char));
    FULFILL_OR_RETURN( (*pspdata)->symbol != NULL, PSPIO_ENOMEM );
    sprintf((*pspdata)->symbol, "N/D");
  }

  return PSPIO_SUCCESS;
}


int pspio_fhi_write(FILE *fp, const pspio_pspdata_t *pspdata){
  int i, l, is, ir;
  double wf, v, r;

  assert(fp != NULL);
  assert(pspdata != NULL);

  // If one considers that the specifications of this format is the way
  // FHI98PP writes the data, then only meshes of type log1 should be
  // allowed. For the moment, we will just check that this is the case.
  assert(pspdata->mesh->type == PSPIO_MESH_LOG1);

  // Write header
  FULFILL_OR_RETURN( fprintf(fp, "%20.14E   %d\n", pspdata->zvalence,
    pspdata->l_max+1) > 0, PSPIO_EIO );
  FULFILL_OR_RETURN( fprintf(fp, " 0.0000    0.0000    0.0000   0.0000\n") > 0,
    PSPIO_EIO );
  for (i=0; i<9; i++) {
    FULFILL_OR_RETURN( fprintf(fp, "  0.0000    .00e+00   .00e+00\n") > 0,
      PSPIO_EIO );
  }

  // Write mesh, pseudopotentials, and wavefunctions
  for (l=0; l<pspdata->l_max+1; l++) {
    FULFILL_OR_RETURN( fprintf(fp, "%-4d %20.14E\n", pspdata->mesh->np,
      pspdata->mesh->r[1]/pspdata->mesh->r[0]) > 0, PSPIO_EIO );

    i = LJ_TO_I(l,0.0);
    is = pspdata->qn_to_istate[0][i];

    // This format is not suitable for j-dependent pseudos
    FULFILL_OR_RETURN( pspio_state_get_j(pspdata->states[is]) == 0.0,
      PSPIO_EVALUE );

    for (ir=0; ir<pspdata->mesh->np; ir++) {
      r = pspdata->mesh->r[ir];
      pspio_state_wf_eval(pspdata->states[is], 1, &r, &wf);
      pspio_potential_eval(pspdata->potentials[i], 1, &r, &v);
      wf = wf*r;

      FULFILL_OR_RETURN( fprintf(fp, "%4d %20.14E %20.14E %20.14E\n", ir+1,
        pspdata->mesh->r[ir], wf, v) > 0, PSPIO_EIO );
    }
  }

  // Write non-linear core corrections
  if (pspio_xc_has_nlcc(pspdata->xc)) {
    pspio_meshfunc_t *nlcc_dens = NULL;
    double cd, cdp, cdpp;

    pspio_xc_get_nlcc_density(pspdata->xc, &nlcc_dens);

    for (ir=0; ir<pspdata->mesh->np; ir++) {
      r = pspdata->mesh->r[ir];
      pspio_meshfunc_eval(nlcc_dens, 1, &r, &cd);
      pspio_meshfunc_eval_deriv(nlcc_dens, 1, &r, &cdp);
      pspio_meshfunc_eval_deriv2(nlcc_dens, 1, &r, &cdpp);
      cd *= M_PI*4.0; cdp *= M_PI*4.0; cdpp *= M_PI*4.0;

      FULFILL_OR_RETURN( fprintf(fp, " %18.12E %18.12E %18.12E %18.12E\n",
        r, cd, cdp, cdpp) > 0, PSPIO_EIO );
    }
  }

  return PSPIO_SUCCESS;
}
