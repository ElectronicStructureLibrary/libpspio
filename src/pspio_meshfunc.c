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
#include <string.h>
#include <assert.h>

#include "pspio_meshfunc.h"
#include "util.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

int pspio_meshfunc_alloc(pspio_meshfunc_t **func, const int interp_method, const int np) {
  int ierr;

  assert(func != NULL);
  assert(*func == NULL);
  assert(np > 1);

  *func = (pspio_meshfunc_t *) malloc (sizeof(pspio_meshfunc_t));
  FULFILL_OR_EXIT( *func != NULL, PSPIO_ENOMEM );

  (*func)->mesh = NULL;
  ierr = pspio_mesh_alloc(&(*func)->mesh, np);
  if ( ierr != PSPIO_SUCCESS ) {
    pspio_meshfunc_free(func);
    RETURN_WITH_ERROR(ierr);
  }

  (*func)->interp_method = interp_method;

  (*func)->f = (double *) malloc (np * sizeof(double));
  FULFILL_OR_EXIT( (*func)->f != NULL, PSPIO_ENOMEM );
  memset((*func)->f, 0, np*sizeof(double));
  SUCCEED_OR_RETURN( interpolation_alloc(&(*func)->f_interp, interp_method, np) );

  (*func)->fp = (double *) malloc (np * sizeof(double));
  FULFILL_OR_EXIT( (*func)->fp != NULL, PSPIO_ENOMEM );
  memset((*func)->fp, 0, np*sizeof(double));
  SUCCEED_OR_RETURN( interpolation_alloc(&(*func)->fp_interp, interp_method, np) );

  (*func)->fpp = (double *) malloc (np * sizeof(double));
  FULFILL_OR_EXIT( (*func)->fpp != NULL, PSPIO_ENOMEM );
  memset((*func)->fpp, 0, np*sizeof(double));
  SUCCEED_OR_RETURN( interpolation_alloc(&(*func)->fpp_interp, interp_method, np) );

  return PSPIO_SUCCESS;
}


int pspio_meshfunc_set(pspio_meshfunc_t **func, const pspio_mesh_t *mesh, 
      const double *f, const double *fp, const double *fpp) {
  int i;

  assert(func != NULL);
  assert((*func)->f != NULL);
  assert(mesh != NULL);

  // Copy mesh
  SUCCEED_OR_RETURN( pspio_mesh_copy(&(*func)->mesh, mesh) );

  // Function
  memcpy((*func)->f, f, mesh->np * sizeof(double));
  SUCCEED_OR_RETURN( interpolation_set(&(*func)->f_interp, mesh, (*func)->f) );

  // First derivative
  if ( fp != NULL ) {
    memcpy((*func)->fp, fp, mesh->np * sizeof(double));
  } else {
    for (i=0; i<mesh->np; i++) 
      interpolation_eval_deriv((*func)->f_interp, mesh->r[i], &(*func)->fp[i]);
  }
  SUCCEED_OR_RETURN( interpolation_set(&(*func)->fp_interp, mesh, (*func)->fp) );

  // Second derivative
  if ( fpp != NULL ) {
    memcpy((*func)->fpp, fpp, mesh->np * sizeof(double));
  } else {
    for (i=0; i<mesh->np; i++)
      interpolation_eval_deriv2((*func)->f_interp, mesh->r[i], &(*func)->fpp[i]);
  }
  SUCCEED_OR_RETURN( interpolation_set(&(*func)->fpp_interp, mesh, (*func)->fpp) );

  return PSPIO_SUCCESS;
}


int pspio_meshfunc_copy(pspio_meshfunc_t **dst, const pspio_meshfunc_t *src) {
  assert(src != NULL);

  if ( *dst == NULL ) {
    SUCCEED_OR_RETURN( pspio_meshfunc_alloc(dst, src->interp_method, src->mesh->np) );

  } else {
    /* All the interpolation objects of dst must be free, otherwise we might have 
       memory leaks if the interpolation method used previously in dst in not the
       same as in src. */
    interpolation_free(&(*dst)->f_interp);
    interpolation_free(&(*dst)->fp_interp);
    interpolation_free(&(*dst)->fpp_interp);
  }

  SUCCEED_OR_RETURN( pspio_mesh_copy(&(*dst)->mesh, src->mesh) );

  memcpy((*dst)->f, src->f, src->mesh->np * sizeof(double));
  SUCCEED_OR_RETURN( interpolation_alloc(&(*dst)->f_interp, src->interp_method, src->mesh->np) );
  SUCCEED_OR_RETURN( interpolation_set(&(*dst)->f_interp, (*dst)->mesh, (*dst)->f) );

  memcpy((*dst)->fp, src->fp, src->mesh->np * sizeof(double));
  SUCCEED_OR_RETURN( interpolation_alloc(&(*dst)->fp_interp, src->interp_method, src->mesh->np) );
  SUCCEED_OR_RETURN( interpolation_set(&(*dst)->fp_interp, (*dst)->mesh, (*dst)->fp) );

  memcpy((*dst)->fpp, src->fpp, src->mesh->np * sizeof(double));
  SUCCEED_OR_RETURN( interpolation_alloc(&(*dst)->fpp_interp, src->interp_method, src->mesh->np) );
  SUCCEED_OR_RETURN( interpolation_set(&(*dst)->fpp_interp, (*dst)->mesh, (*dst)->fpp) );

  return PSPIO_SUCCESS;
}


void pspio_meshfunc_free(pspio_meshfunc_t **func) {

  if (*func != NULL) {
    pspio_mesh_free(&(*func)->mesh);

    free((*func)->f);
    interpolation_free(&(*func)->f_interp);

    free((*func)->fp);
    interpolation_free(&(*func)->fp_interp);

    free((*func)->fpp);
    interpolation_free(&(*func)->fpp_interp);

    free(*func);
    *func = NULL;
  }
}


/**********************************************************************
 * Atomic routines                                                    *
 **********************************************************************/

void pspio_meshfunc_eval(const pspio_meshfunc_t *func, const int np, 
			 const double *r, double *f) {
  int i;

  assert(func != NULL);
  assert(r != NULL);
  assert(f != NULL);

  for (i=0; i<np; i++) {
    // If the value of r is smaller than the first mesh point or if
    // it is greater or equal to the last mesh point, then we use a
    // linear extrapolation to evaluate the function at r.
    if ( r[i] < func->mesh->r[0] ) {
      f[i] = linear_extrapolation(func->mesh->r[0], func->mesh->r[1],
        func->f[0], func->f[1], r[i]);
    } else if ( r[i] >= func->mesh->r[func->mesh->np-1] ) {
      f[i] = linear_extrapolation(func->mesh->r[func->mesh->np-2],
        func->mesh->r[func->mesh->np-1], func->f[func->mesh->np-2],
        func->f[func->mesh->np-1], r[i]);
    } else {
      interpolation_eval(func->f_interp, r[i], &f[i]);
    }
  }
}


void pspio_meshfunc_eval_deriv(const pspio_meshfunc_t *func, const int np, 
       const double *r, double *fp) {
  int i;

  assert(func != NULL);
  assert(r != NULL);
  assert(fp != NULL);

  for (i=0; i<np; i++) {
    // If the value of r is smaller than the first mesh point or if
    // it is greater or equal to the last mesh point, then we use a
    // linear extrapolation to evaluate the function at r.
    if ( r[i] < func->mesh->r[0] ) {
      fp[i] = linear_extrapolation(func->mesh->r[0], func->mesh->r[1],
        func->fp[0], func->fp[1], r[i]);
    } else if ( r[i] >= func->mesh->r[func->mesh->np-1] ) {
      fp[i] = linear_extrapolation(func->mesh->r[func->mesh->np-2],
        func->mesh->r[func->mesh->np-1], func->fp[func->mesh->np-2],
        func->fp[func->mesh->np-1], r[i]);
    } else {
      interpolation_eval(func->fp_interp, r[i], &fp[i]);
    }
  }
}


void pspio_meshfunc_eval_deriv2(const pspio_meshfunc_t *func, const int np, 
       const double *r, double *fpp) {
  int i;

  assert(func != NULL);
  assert(r != NULL);
  assert(fpp != NULL);

  for (i=0; i<np; i++) {
    // If the value of r is smaller than the first mesh point or if
    // it is greater or equal to the last mesh point, then we use a
    // linear extrapolation to evaluate the function at r.
    if ( r[i] < func->mesh->r[0] ) {
      fpp[i] = linear_extrapolation(func->mesh->r[0], func->mesh->r[1],
        func->fpp[0], func->fpp[1], r[i]);
    } else if ( r[i] >= func->mesh->r[func->mesh->np-1] ) {
      fpp[i] = linear_extrapolation(func->mesh->r[func->mesh->np-2],
        func->mesh->r[func->mesh->np-1], func->fpp[func->mesh->np-2],
        func->fpp[func->mesh->np-1], r[i]);
    } else {
      interpolation_eval(func->fpp_interp, r[i], &fpp[i]);
    }
  }
}
