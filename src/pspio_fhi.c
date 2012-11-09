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

 $Id: pspio_upf.c 256 2012-09-06 10:06:13Z micael $
*/

/** 
 * @file pspio_fhi.c
 * @brief implementation to read and write in FHI files 
 */
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "pspio_error.h"
#include "pspio_xc_funcs.h"
#include "pspio_pspdata.h"
#include "util.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


int pspio_fhi_read(FILE *fp, pspio_pspdata_t **pspdata){
  char line[MAX_STRLEN];
  int i, l, np, ir, has_nlcc;
  double r12;
  double *wf, *r, *v;
  pspio_qn_t *qn = NULL;

  // Read header
  CHECK_ERROR(fgets(line, MAX_STRLEN, fp) != NULL, PSPIO_EIO);
  CHECK_ERROR(sscanf(line, "%lf %d", &(*pspdata)->zvalence, &(*pspdata)->n_potentials ) == 2, PSPIO_EIO);
  for (i=0; i<10; i++) { 
    // We ignore the next 10 lines, as they contain no information
    CHECK_ERROR(fgets(line, MAX_STRLEN, fp) != NULL, PSPIO_EIO);
  }
  (*pspdata)->l_max = (*pspdata)->n_potentials - 1;
  (*pspdata)->n_states = (*pspdata)->n_potentials;


  //Allocate states and potentials
  (*pspdata)->states = (pspio_state_t **)malloc( (*pspdata)->n_states*sizeof(pspio_state_t *));
  CHECK_ERROR((*pspdata)->states != NULL, PSPIO_ENOMEM);
  for (i=0; i<(*pspdata)->n_states; i++) {
    (*pspdata)->states[i] = NULL;
  }
  (*pspdata)->potentials = (pspio_potential_t **)malloc( (*pspdata)->n_potentials*sizeof(pspio_potential_t *));
  CHECK_ERROR((*pspdata)->potentials != NULL, PSPIO_ENOMEM);
  for (i=0; i<(*pspdata)->n_potentials; i++) {
    (*pspdata)->potentials[i] = NULL;
  }

  
  // Read mesh, potentials and wavefunctions
  for (l=0; l < (*pspdata)->l_max+1; l++) {
    CHECK_ERROR(fgets(line, MAX_STRLEN, fp) != NULL, PSPIO_EIO);
    CHECK_ERROR(sscanf(line, "%d %lf", &np, &r12 ) == 2, PSPIO_EIO);

    //Allocate temporary data
    HANDLE_FUNC_ERROR(pspio_qn_alloc(&qn));

    r = (double *)malloc(np*sizeof(double));
    CHECK_ERROR(r != NULL, PSPIO_ENOMEM);

    v = (double *)malloc(np*sizeof(double));
    CHECK_ERROR(v != NULL, PSPIO_ENOMEM);

    wf = (double *)malloc(np*sizeof(double));
    CHECK_ERROR(wf != NULL, PSPIO_ENOMEM);

    //Read first line of block
    for (ir=0; ir<np; ir++) {
      CHECK_ERROR(fgets(line, MAX_STRLEN, fp) != NULL, PSPIO_EIO);
      CHECK_ERROR(sscanf(line, "%d %lf %lf %lf", &i, &r[ir], &wf[ir], &v[ir]) == 4, PSPIO_EIO);
      wf[ir] = wf[ir]/r[ir];
    }

    if (l==0) {
      //We will use the first block to generate the mesh, as the mesh is supposed to be the same for all blocks (at least that is the behavior of FHI98PP)
      HANDLE_FUNC_ERROR(pspio_mesh_alloc(&(*pspdata)->mesh, np));
      HANDLE_FUNC_ERROR(pspio_mesh_init_from_points(&(*pspdata)->mesh, r, NULL));
    }

    //Set pseudopotential and wavefunction
    HANDLE_FUNC_ERROR(pspio_qn_set(&qn, 0, l, 0.0));
    HANDLE_FUNC_ERROR(pspio_potential_alloc(&(*pspdata)->potentials[LJ_TO_I(l,0.0)], np));
    HANDLE_FUNC_ERROR(pspio_potential_set(&(*pspdata)->potentials[LJ_TO_I(l,0.0)], qn, (*pspdata)->mesh, v));
    HANDLE_FUNC_ERROR(pspio_state_alloc( &(*pspdata)->states[l], np));
    HANDLE_FUNC_ERROR(pspio_state_set(&(*pspdata)->states[l], 0.0, "", qn, 0.0, 0.0, (*pspdata)->mesh, wf));

    //Free memory
    free(r);
    free(v);
    free(wf);
    HANDLE_FUNC_ERROR(pspio_qn_free(&qn));
  }

  //Non-linear core-corrections
  has_nlcc = (fgets(line, MAX_STRLEN, fp) != NULL);
  if (has_nlcc) {
    double *cd, *cdp, *cdpp;

    HANDLE_FUNC_ERROR(pspio_xc_alloc(&(*pspdata)->xc, PSPIO_NLCC_FHI, np));

    //Allocate memory
    cd = (double *)malloc(np*sizeof(double));
    CHECK_ERROR(cd != NULL, PSPIO_ENOMEM);

    cdp = (double *)malloc(np*sizeof(double));
    CHECK_ERROR(cdp != NULL, PSPIO_ENOMEM);

    cdpp = (double *)malloc(np*sizeof(double));
    CHECK_ERROR(cdpp != NULL, PSPIO_ENOMEM);

    //Read core density
    for (ir=0; ir<np; ir++) {
      if (ir != 0) {
	CHECK_ERROR(fgets(line, MAX_STRLEN, fp) != NULL, PSPIO_EIO);
      }

      CHECK_ERROR(sscanf(line, "%lf %lf %lf %lf", &r12, &cd[ir], &cdp[ir], &cdpp[ir]) == 4, PSPIO_EIO);
      cd[ir] /= (M_PI*4.0); cdp[ir] /= (M_PI*4.0); cdpp[ir] /= (M_PI*4.0);
    }

    //Store the non-linear core corrections in the pspdata structure
    HANDLE_FUNC_ERROR(pspio_xc_nlcc_set(&(*pspdata)->xc, (*pspdata)->mesh, cd, cdp, cdpp));

    //Free memory
    free(cd); 
    free(cdp); 
    free(cdpp);

  } else {
    HANDLE_FUNC_ERROR(pspio_xc_alloc(&(*pspdata)->xc, PSPIO_NLCC_NONE, np));
  }

  //We do not know the xc functional:
  HANDLE_FUNC_ERROR(pspio_xc_set(&(*pspdata)->xc, XC_NONE, XC_NONE));

  return PSPIO_SUCCESS;
}


int pspio_fhi_write(FILE *fp, const pspio_pspdata_t *pspdata){
  int i, l, is, ir, has_nlcc;
  double wf, v, r, j;

  ASSERT(pspdata != NULL, PSPIO_ERROR);

  // If one considers that the specifications of this format is the way how FHIPP98 writes the
  // data, then only meshes of type log1 should be allowed. For the moment, we will just
  // check that this is the case.
  ASSERT(pspdata->mesh->type == PSPIO_MESH_LOG1, PSPIO_ERROR);  

  // Write header
  fprintf(fp, "%20.14E   %d\n", pspdata->zvalence, pspdata->l_max+1);
  fprintf(fp, " 0.0000    0.0000    0.0000   0.0000\n");
  for (i=0; i<9; i++)   fprintf(fp, "  0.0000    .00e+00   .00e+00\n");

  // Write mesh, pseudopotentials, and wavefunctions
  for (l=0; l<pspdata->l_max+1; l++) {
    fprintf(fp, "%-4d %20.14E\n", pspdata->mesh->np, pspdata->mesh->r[1]/pspdata->mesh->r[0]);

    i = LJ_TO_I(l,0.0);
    is = pspdata->qn_to_istate[0][i];

    HANDLE_FUNC_ERROR(pspio_state_get_j(pspdata->states[is], &j));
    CHECK_ERROR(j == 0.0, PSPIO_EVALUE); // This format is not suitable for j-dependent pseudos

    for (ir=0; ir<pspdata->mesh->np; ir++) {
      r = pspdata->mesh->r[ir];
      HANDLE_FUNC_ERROR(pspio_state_wf_eval(pspdata->states[is], 1, &r, &wf));
      HANDLE_FUNC_ERROR(pspio_potential_eval(pspdata->potentials[i], 1, &r, &v));
      wf = wf*r;

      fprintf(fp, "%4d %20.14E %20.14E %20.14E\n", ir+1, pspdata->mesh->r[ir], wf, v);
    }

  }

  // Write non-linear core corrections
  HANDLE_FUNC_ERROR(pspio_xc_has_nlcc(pspdata->xc, &has_nlcc));
  if (has_nlcc) {
    pspio_meshfunc_t *core_dens = NULL;
    double cd, cdp, cdpp;

    HANDLE_FUNC_ERROR(pspio_xc_nlcc_get(pspdata->xc, &core_dens));

    for (ir=0; ir<pspdata->mesh->np; ir++) {
      r = pspdata->mesh->r[ir];
      HANDLE_FUNC_ERROR(pspio_meshfunc_eval(core_dens, 1, &r, &cd));
      HANDLE_FUNC_ERROR(pspio_meshfunc_eval_deriv(core_dens, 1, &r, &cdp));
      HANDLE_FUNC_ERROR(pspio_meshfunc_eval_deriv2(core_dens, 1, &r, &cdpp));
      cd *= M_PI*4.0; cdp *= M_PI*4.0; cdpp *= M_PI*4.0;

      fprintf(fp, " %18.12E %18.12E %18.12E %18.12E\n", r, cd, cdp, cdpp);
    }
  }

  return PSPIO_SUCCESS;
}
