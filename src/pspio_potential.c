/*
 Copyright (C) 2011-2012 J. Alberdi, M. Oliveira, Y. Pouillon, and M. Verstraete

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

#include <stdlib.h>
#include <assert.h>

#include "pspio_potential.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

int pspio_potential_alloc(pspio_potential_t **potential, const int np) {
  int ierr;

  assert(potential != NULL);
  assert(*potential == NULL);
  assert(np > 1);

  *potential = (pspio_potential_t *) malloc (sizeof(pspio_potential_t));
  FULFILL_OR_EXIT(*potential != NULL, PSPIO_ENOMEM);

  (*potential)->v = NULL;
  ierr = pspio_meshfunc_alloc(&(*potential)->v, PSPIO_INTERP_GSL_CSPLINE, np);
  if ( ierr != PSPIO_SUCCESS ) {
    pspio_potential_free(potential);
    RETURN_WITH_ERROR(ierr);
  }

  (*potential)->qn = NULL;
  ierr = pspio_qn_alloc(&(*potential)->qn);
  if ( ierr != PSPIO_SUCCESS ) {
    pspio_potential_free(potential);
    RETURN_WITH_ERROR(ierr);
  }

  return PSPIO_SUCCESS;
}


int pspio_potential_set(pspio_potential_t **potential, const pspio_qn_t *qn,
      const pspio_mesh_t *mesh, const double *vofr) {

  assert((*potential) != NULL);
  assert(qn != NULL);
  assert(mesh != NULL);
  assert(vofr != NULL);

  SUCCEED_OR_RETURN( pspio_qn_copy(&(*potential)->qn, qn) );
  SUCCEED_OR_RETURN( pspio_meshfunc_set(&(*potential)->v, mesh, vofr,
    NULL, NULL) );

  return PSPIO_SUCCESS;
}


void pspio_potential_free(pspio_potential_t **potential){

  if (*potential != NULL) {
    pspio_meshfunc_free(&(*potential)->v);
    pspio_qn_free(&(*potential)->qn);
    free(*potential);
    *potential = NULL;
  }
}


/**********************************************************************
 * Atomic routines                                                    *
 **********************************************************************/

void pspio_potential_eval(const pspio_potential_t *potential, const int np, 
			 const double *radii, double *vofr) {
  assert(potential != NULL);
  assert(radii != NULL);
  assert(vofr != NULL);

  pspio_meshfunc_eval(potential->v, np, radii, vofr);
}
