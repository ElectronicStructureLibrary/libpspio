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

  (*func)->mesh = NULL;
  ierr = pspio_mesh_alloc(&(*func)->mesh, np);
  if (ierr) {
    pspio_meshfunc_free(func);
    HANDLE_ERROR(ierr);
  }

  (*func)->f = (double *) malloc (np * sizeof(double));
  CHECK_ERROR((*func)->f != NULL, PSPIO_ENOMEM);
  memset((*func)->f, 0, np*sizeof(double));
  (*func)->f_spl = gsl_spline_alloc(gsl_interp_cspline, np);
  (*func)->f_acc = gsl_interp_accel_alloc();

  (*func)->fp = (double *) malloc (np * sizeof(double));
  CHECK_ERROR((*func)->fp != NULL, PSPIO_ENOMEM);
  memset((*func)->fp, 0, np*sizeof(double));
  (*func)->fp_spl = gsl_spline_alloc(gsl_interp_cspline, np);
  (*func)->fp_acc = gsl_interp_accel_alloc();

  (*func)->fpp = (double *) malloc (np * sizeof(double));
  CHECK_ERROR((*func)->fpp != NULL, PSPIO_ENOMEM);
  memset((*func)->fpp, 0, np*sizeof(double));
  (*func)->fpp_spl = gsl_spline_alloc(gsl_interp_cspline, np);
  (*func)->fpp_acc = gsl_interp_accel_alloc();

  return PSPIO_SUCCESS;
}


int pspio_meshfunc_set(pspio_meshfunc_t **func, const pspio_mesh_t *mesh, 
		       const double *f, const double *fp, const double *fpp){
  int i;

  // Copy mesh
  HANDLE_FUNC_ERROR(pspio_mesh_copy(&(*func)->mesh, mesh));

  // Function
  memcpy((*func)->f, f, mesh->np * sizeof(double));
  gsl_spline_init((*func)->f_spl, mesh->r, (*func)->f, mesh->np);

  // First derivative
  if (fp != NULL) {
    memcpy((*func)->fp, fp, mesh->np * sizeof(double));
  } else {
    for (i=0; i<mesh->np; i++) 
      (*func)->fp[i] = gsl_spline_eval_deriv((*func)->f_spl, mesh->r[i], (*func)->f_acc);
  }
  gsl_spline_init((*func)->fp_spl, mesh->r, (*func)->fp, (*func)->mesh->np);

  // Second derivative
  if (fpp != NULL) {
    memcpy((*func)->fpp, fpp, mesh->np * sizeof(double));
  } else {
    for (i=0; i<mesh->np; i++) 
      (*func)->fpp[i] = gsl_spline_eval_deriv((*func)->fp_spl, mesh->r[i], (*func)->fp_acc);    
  }
  gsl_spline_init((*func)->fpp_spl, mesh->r, (*func)->fpp, mesh->np);

  return PSPIO_SUCCESS;
}


int pspio_meshfunc_copy(pspio_meshfunc_t **dst, const pspio_meshfunc_t *src){

  ASSERT(src != NULL, PSPIO_ERROR);

  if (*dst == NULL) {
    HANDLE_FUNC_ERROR(pspio_meshfunc_alloc(dst, src->mesh->np))
  }

  HANDLE_FUNC_ERROR(pspio_mesh_copy(&(*dst)->mesh, src->mesh));
  memcpy((*dst)->f, src->f, src->mesh->np * sizeof(double));
  gsl_spline_init((*dst)->f_spl, (*dst)->mesh->r, (*dst)->f,
		  (*dst)->mesh->np) ;

  memcpy((*dst)->fp, src->fp, src->mesh->np * sizeof(double));
  gsl_spline_init((*dst)->fp_spl, (*dst)->mesh->r, (*dst)->fp,
		  (*dst)->mesh->np) ;

  memcpy((*dst)->fpp, src->fpp, src->mesh->np * sizeof(double));
  gsl_spline_init((*dst)->fpp_spl, (*dst)->mesh->r, (*dst)->fpp,
		  (*dst)->mesh->np) ;

  return PSPIO_SUCCESS;
}


void pspio_meshfunc_free(pspio_meshfunc_t **func){

  if (*func != NULL) {
    pspio_mesh_free(&(*func)->mesh);

    if ((*func)->f != NULL) free((*func)->f);
    gsl_spline_free((*func)->f_spl);
    gsl_interp_accel_free((*func)->f_acc);

    if ((*func)->fp != NULL) free((*func)->fp);
    gsl_spline_free((*func)->fp_spl);
    gsl_interp_accel_free((*func)->fp_acc);

    if ((*func)->fpp != NULL) free((*func)->fpp);
    gsl_spline_free((*func)->fpp_spl);
    gsl_interp_accel_free((*func)->fpp_acc);

    free(*func);
    *func = NULL;
  }
}


/**********************************************************************
 * Atomic routines                                                    *
 **********************************************************************/

void pspio_meshfunc_eval(const pspio_meshfunc_t *func, const int np, 
			 const double *r, double *f){
  ASSERT(func != NULL, PSPIO_ERROR);
  ASSERT(r != NULL, PSPIO_ERROR);
  ASSERT(f != NULL, PSPIO_ERROR);

  int i;

  for (i=0; i<np; i++) {
    // If the value of r is smaller than the first mesh point or if it is greater or equal to
    // the last mesh point, then we use a linear extrapolation to evaluate the function at r.
    if (r[i] < func->mesh->r[0]) {
      f[i] = linear_extrapolation(func->mesh->r[0], func->mesh->r[1], func->f[0], func->f[1], r[i]);
      
    } else if (r[i] >= func->mesh->r[func->mesh->np-1]) {
      f[i] = linear_extrapolation(func->mesh->r[func->mesh->np-2], func->mesh->r[func->mesh->np-1], 
				func->f[func->mesh->np-2], func->f[func->mesh->np-1], r[i]);

    } else {
      f[i] = gsl_spline_eval(func->f_spl, r[i], func->f_acc);
    }
  }
}


void pspio_meshfunc_eval_deriv(const pspio_meshfunc_t *func, const int np, 
			       const double *r, double *fp){
  ASSERT(func != NULL, PSPIO_ERROR);
  ASSERT(r != NULL, PSPIO_ERROR);
  ASSERT(fp != NULL, PSPIO_ERROR);

  int i;

  for (i=0; i<np; i++) {
    // If the value of r is smaller than the first mesh point or if it is greater or equal to
    // the last mesh point, then we use a linear extrapolation to evaluate the function at r.
    if (r[i] < func->mesh->r[0]) {
      fp[i] = linear_extrapolation(func->mesh->r[0], func->mesh->r[1], func->fp[0], func->fp[1], r[i]);

    } else if (r[i] >= func->mesh->r[func->mesh->np-1]) {
      fp[i] = linear_extrapolation(func->mesh->r[func->mesh->np-2], func->mesh->r[func->mesh->np-1], 
				 func->fp[func->mesh->np-2], func->fp[func->mesh->np-1], r[i]);

    } else {
      fp[i] = gsl_spline_eval(func->fp_spl, r[i], func->fp_acc);
    }
  }
}


void pspio_meshfunc_eval_deriv2(const pspio_meshfunc_t *func, const int np, 
				const double *r, double *fpp){
  ASSERT(func != NULL, PSPIO_ERROR);
  ASSERT(r != NULL, PSPIO_ERROR);
  ASSERT(fpp != NULL, PSPIO_ERROR);

  int i;

  for (i=0; i<np; i++) {
    // If the value of r is smaller than the first mesh point or if it is greater or equal to
    // the last mesh point, then we use a linear extrapolation to evaluate the function at r.
    if (r[i] < func->mesh->r[0]) {
      fpp[i] = linear_extrapolation(func->mesh->r[0], func->mesh->r[1], func->fpp[0], func->fpp[1], r[i]);
      
    } else if (r[i] >= func->mesh->r[func->mesh->np-1]) {
      fpp[i] = linear_extrapolation(func->mesh->r[func->mesh->np-2], func->mesh->r[func->mesh->np-1], 
				  func->fpp[func->mesh->np-2], func->fpp[func->mesh->np-1], r[i]);
      
    } else {
      fpp[i] = gsl_spline_eval(func->fpp_spl, r[i], func->fpp_acc);
    }
  }
}
