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

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

int pspio_meshfunc_alloc(pspio_meshfunc_t **func, const int np){
  int ierr;

  ASSERT(np > 1, PSPIO_EVALUE);
  ASSERT(*func == NULL, PSPIO_ERROR);

  *func = (pspio_meshfunc_t *) malloc (sizeof(pspio_meshfunc_t));
  HANDLE_FATAL_ERROR (*func == NULL, PSPIO_ENOMEM);

  (*func)->f = (double *) malloc (np * sizeof(double));
  HANDLE_FATAL_ERROR ((*func)->f == NULL, PSPIO_ENOMEM);
  memset((*func)->f, 0, np*sizeof(double));

  (*func)->mesh = NULL;
  ierr = pspio_mesh_alloc(&(*func)->mesh, np);
  if (ierr) {
    pspio_meshfunc_free(func);
    HANDLE_ERROR (ierr);
  }

  (*func)->spl = gsl_spline_alloc(gsl_interp_cspline, np);
  (*func)->acc = gsl_interp_accel_alloc();
  
  return PSPIO_SUCCESS;
}


int pspio_meshfunc_set(pspio_meshfunc_t **func, const pspio_mesh_t *mesh, 
		       const double *f){

  HANDLE_FUNC_ERROR (pspio_mesh_copy(&(*func)->mesh, mesh));
  memcpy((*func)->f, f, (*func)->mesh->np * sizeof(double));
  gsl_spline_init((*func)->spl, (*func)->mesh->r, (*func)->f,
		  (*func)->mesh->np) ;

  return PSPIO_SUCCESS;
}


int pspio_meshfunc_copy(pspio_meshfunc_t **dst, const pspio_meshfunc_t *src){

  ASSERT (src != NULL, PSPIO_ERROR);

  if (*dst == NULL) {
    HANDLE_FUNC_ERROR (pspio_meshfunc_alloc(dst, src->mesh->np))
  }

  HANDLE_FUNC_ERROR (pspio_mesh_copy(&(*dst)->mesh, src->mesh));
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
  ASSERT (func != NULL, PSPIO_ERROR);

  *f = gsl_spline_eval(func->spl, r, func->acc);

  return PSPIO_SUCCESS;
}


int pspio_meshfunc_eval_deriv(const pspio_meshfunc_t *func, const double r,
      double *fp){
  ASSERT (func != NULL, PSPIO_ERROR);  

  *fp = gsl_spline_eval_deriv(func->spl, r, func->acc);

  return PSPIO_SUCCESS;
}


int pspio_meshfunc_eval_deriv2(const pspio_meshfunc_t *func, const double r,
      double *fpp){
  ASSERT (func != NULL, PSPIO_ERROR);  

  *fpp = gsl_spline_eval_deriv2(func->spl, r, func->acc);

  return PSPIO_SUCCESS;
}

