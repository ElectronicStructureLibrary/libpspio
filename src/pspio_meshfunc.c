/*
 Copyright (C) 2011 J. Alberdi, M. Oliveira, Y. Pouillon, and M. Verstraete

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

 $Id$
*/

#include <stdlib.h>
#include <string.h>

#include "pspio_meshfunc.h"
#include "util.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

int pspio_meshfunc_alloc(pspio_meshfunc_t **func, const int np){
  int ierr;

  ASSERT(func != NULL, PSPIO_ERROR);
  ASSERT(*func == NULL, PSPIO_ERROR);
  ASSERT(np > 1, PSPIO_EVALUE);

  *func = (pspio_meshfunc_t *) malloc (sizeof(pspio_meshfunc_t));
  CHECK_ERROR(*func != NULL, PSPIO_ENOMEM);

  (*func)->f = (double *) malloc (np * sizeof(double));
  CHECK_ERROR((*func)->f != NULL, PSPIO_ENOMEM);
  memset((*func)->f, 0, np*sizeof(double));

  (*func)->mesh = NULL;
  ierr = pspio_mesh_alloc(&(*func)->mesh, np);
  if (ierr) {
    pspio_meshfunc_free(func);
    HANDLE_ERROR(ierr);
  }

  (*func)->spl = gsl_spline_alloc(gsl_interp_cspline, np);
  (*func)->acc = gsl_interp_accel_alloc();
  
  return PSPIO_SUCCESS;
}


int pspio_meshfunc_set(pspio_meshfunc_t **func, const pspio_mesh_t *mesh, 
		       const double *f){

  HANDLE_FUNC_ERROR(pspio_mesh_copy(&(*func)->mesh, mesh));
  memcpy((*func)->f, f, (*func)->mesh->np * sizeof(double));
  gsl_spline_init((*func)->spl, (*func)->mesh->r, (*func)->f,
		  (*func)->mesh->np) ;

  return PSPIO_SUCCESS;
}


int pspio_meshfunc_copy(pspio_meshfunc_t **dst, const pspio_meshfunc_t *src){

  ASSERT(src != NULL, PSPIO_ERROR);

  if (*dst == NULL) {
    HANDLE_FUNC_ERROR(pspio_meshfunc_alloc(dst, src->mesh->np))
  }

  HANDLE_FUNC_ERROR(pspio_mesh_copy(&(*dst)->mesh, src->mesh));
  memcpy((*dst)->f, src->f, src->mesh->np * sizeof(double));

  gsl_spline_init((*dst)->spl, (*dst)->mesh->r, (*dst)->f,
		  (*dst)->mesh->np) ;

  return PSPIO_SUCCESS;
}


int pspio_meshfunc_free(pspio_meshfunc_t **func){

  if (*func != NULL) {
    if ((*func)->f != NULL) free((*func)->f);
    HANDLE_FUNC_ERROR(pspio_mesh_free(&(*func)->mesh));
    gsl_spline_free((*func)->spl);
    gsl_interp_accel_free((*func)->acc);
    free(*func);
    *func = NULL;
  }

  return PSPIO_SUCCESS;
}


/**********************************************************************
 * Atomic routines                                                    *
 **********************************************************************/

int pspio_meshfunc_eval(const pspio_meshfunc_t *func, const double r,
      double *f){
  ASSERT(func != NULL, PSPIO_ERROR);

  // If the value of r is smaller than the first mesh point or if it is greater or equal to
  // the last mesh point, then we use a linear extrapolation to evaluate the function at r.
  if (r < func->mesh->r[0]) {
    *f = linear_extrapolation(func->mesh->r[0], func->mesh->r[1], func->f[0], func->f[1], r);

  } else if (r >= func->mesh->r[func->mesh->np-1]) {
    int np = func->mesh->np;
    *f = linear_extrapolation(func->mesh->r[np-2], func->mesh->r[np-1], func->f[np-2], func->f[np-1], r);

  } else {
    *f = gsl_spline_eval(func->spl, r, func->acc);

  }

  return PSPIO_SUCCESS;
}


int pspio_meshfunc_eval_deriv(const pspio_meshfunc_t *func, const double r,
      double *fp){
  double r1, r2, f1, f2;

  ASSERT(func != NULL, PSPIO_ERROR);  

  // If the value of r is smaller than the first mesh point or if it is greater or equal to
  // the last mesh point, then we use a linear extrapolation to evaluate the function at r.
  if (r < func->mesh->r[0]) {
    r1 = func->mesh->r[0];
    f1 = gsl_spline_eval_deriv(func->spl, r1, func->acc);
    r2 = func->mesh->r[1];
    f2 = gsl_spline_eval_deriv(func->spl, r2, func->acc);

    *fp = linear_extrapolation(r1, r2, f1, f2, r);

  } else if (r >= func->mesh->r[func->mesh->np-1]) {
    r1 = func->mesh->r[func->mesh->np-1];
    f1 = gsl_spline_eval_deriv(func->spl, r1, func->acc);
    r2 = func->mesh->r[func->mesh->np-2];
    f2 = gsl_spline_eval_deriv(func->spl, r2, func->acc);

    *fp = linear_extrapolation(r1, r2, f1, f2, r);

  } else {
    *fp = gsl_spline_eval_deriv(func->spl, r, func->acc);

  }

  return PSPIO_SUCCESS;
}


int pspio_meshfunc_eval_deriv2(const pspio_meshfunc_t *func, const double r,
      double *fpp){
  double r1, r2, f1, f2;

  ASSERT(func != NULL, PSPIO_ERROR);

  // If the value of r is smaller than the first mesh point or if it is greater or equal to
  // the last mesh point, then we use a linear extrapolation to evaluate the function at r.
  if (r < func->mesh->r[0]) {
    r1 = func->mesh->r[0];
    f1 = gsl_spline_eval_deriv2(func->spl, r1, func->acc);
    r2 = func->mesh->r[1];
    f2 = gsl_spline_eval_deriv2(func->spl, r2, func->acc);

    *fpp = linear_extrapolation(r1, r2, f1, f2, r);

  } else if (r >= func->mesh->r[func->mesh->np-1]) {
    r1 = func->mesh->r[func->mesh->np-1];
    f1 = gsl_spline_eval_deriv2(func->spl, r1, func->acc);
    r2 = func->mesh->r[func->mesh->np-2];
    f2 = gsl_spline_eval_deriv2(func->spl, r2, func->acc);

    *fpp = linear_extrapolation(r1, r2, f1, f2, r);

  } else {
    *fpp = gsl_spline_eval_deriv2(func->spl, r, func->acc);

  }

  return PSPIO_SUCCESS;
}

