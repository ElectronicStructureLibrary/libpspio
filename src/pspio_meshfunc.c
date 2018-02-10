/* Copyright (C) 2011-2016 Joseba Alberdi <alberdi@hotmail.es>
 *                         Matthieu Verstraete <matthieu.jean.verstraete@gmail.com>
 *                         Micael Oliveira <micael.oliveira@mpsd.mpg.de>
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

int pspio_meshfunc_alloc(pspio_meshfunc_t **func, int np)
{
  int ierr;

  assert(func != NULL);
  assert(*func == NULL);
  assert(np > 1);

  *func = (pspio_meshfunc_t *) malloc (sizeof(pspio_meshfunc_t));
  FULFILL_OR_EXIT( *func != NULL, PSPIO_ENOMEM );

  (*func)->mesh = NULL;
  ierr = pspio_mesh_alloc(&(*func)->mesh, np);
  if ( ierr != PSPIO_SUCCESS ) {
    pspio_meshfunc_free(*func);
    *func = NULL;
    RETURN_WITH_ERROR( ierr );
  }

#ifdef HAVE_GSL
  (*func)->interp_method = PSPIO_INTERP_GSL_CSPLINE;
#else
  (*func)->interp_method = PSPIO_INTERP_JB_CSPLINE;
#endif

  (*func)->f = (double *) malloc (np * sizeof(double));
  FULFILL_OR_EXIT( (*func)->f != NULL, PSPIO_ENOMEM );
  memset((*func)->f, 0, np*sizeof(double));
  SUCCEED_OR_RETURN( pspio_interp_alloc(&(*func)->f_interp, (*func)->interp_method, np) );

  (*func)->fp = (double *) malloc (np * sizeof(double));
  FULFILL_OR_EXIT( (*func)->fp != NULL, PSPIO_ENOMEM );
  memset((*func)->fp, 0, np*sizeof(double));
  SUCCEED_OR_RETURN( pspio_interp_alloc(&(*func)->fp_interp, (*func)->interp_method, np) );

  (*func)->fpp = (double *) malloc (np * sizeof(double));
  FULFILL_OR_EXIT( (*func)->fpp != NULL, PSPIO_ENOMEM );
  memset((*func)->fpp, 0, np*sizeof(double));
  SUCCEED_OR_RETURN( pspio_interp_alloc(&(*func)->fpp_interp, (*func)->interp_method, np) );

  return PSPIO_SUCCESS;
}

int pspio_meshfunc_init(pspio_meshfunc_t *func, const pspio_mesh_t *mesh, 
			const double *f, const double *fp, const double *fpp)
{
  int i;

  assert(func != NULL);
  assert(func->f != NULL);
  assert(mesh != NULL);

  /* Copy mesh */
  SUCCEED_OR_RETURN( pspio_mesh_copy(&func->mesh, mesh) );

  /* Function */
  memcpy(func->f, f, mesh->np * sizeof(double));
  SUCCEED_OR_RETURN( pspio_interp_init(func->f_interp, mesh, func->f) );

  /* First derivative */
  if ( fp != NULL ) {
    memcpy(func->fp, fp, mesh->np * sizeof(double));
  } else {
    for (i=0; i<mesh->np; i++) 
      func->fp[i] = pspio_interp_eval_deriv(func->f_interp, mesh->r[i]);
  }
  SUCCEED_OR_RETURN( pspio_interp_init(func->fp_interp, mesh, func->fp) );

  /* Second derivative */
  if ( fpp != NULL ) {
    memcpy(func->fpp, fpp, mesh->np * sizeof(double));
  } else {
    for (i=0; i<mesh->np; i++)
      func->fpp[i] = pspio_interp_eval_deriv2(func->f_interp, mesh->r[i]);
  }
  SUCCEED_OR_RETURN( pspio_interp_init(func->fpp_interp, mesh, func->fpp) );

  return PSPIO_SUCCESS;
}

int pspio_meshfunc_copy(pspio_meshfunc_t **dst, const pspio_meshfunc_t *src)
{
  int np;

  assert(src != NULL);

  np = pspio_mesh_get_np(src->mesh);

  if ( *dst == NULL ) {
    SUCCEED_OR_RETURN( pspio_meshfunc_alloc(dst, np) )
  }

  /* The mesh of the destination function must have the same number of points as the mesh of the source function */
  if ( pspio_mesh_get_np((*dst)->mesh) != np ) {
    pspio_meshfunc_free(*dst);
    *dst = NULL;
    SUCCEED_OR_RETURN(pspio_meshfunc_alloc(dst, np));
  }

  /* All the interpolation objects of dst must be free, otherwise
     we might have memory leaks if the interpolation method used
     previously in dst in not the same as in src. */
  if ( (*dst)->f_interp != NULL ) {
    pspio_interp_free((*dst)->f_interp);
    (*dst)->f_interp = NULL;
  }
  if ( (*dst)->fp_interp != NULL ) {
    pspio_interp_free((*dst)->fp_interp);
    (*dst)->fp_interp = NULL;
  }
  if ( (*dst)->fpp_interp != NULL ) {
    pspio_interp_free((*dst)->fpp_interp);
    (*dst)->fpp_interp = NULL;
  }

  SUCCEED_OR_RETURN( pspio_mesh_copy(&(*dst)->mesh, src->mesh) );

  memcpy((*dst)->f, src->f, np * sizeof(double));
  SUCCEED_OR_RETURN( pspio_interp_alloc(&(*dst)->f_interp, src->interp_method, np) );
  SUCCEED_OR_RETURN( pspio_interp_init((*dst)->f_interp, (*dst)->mesh, (*dst)->f) );

  memcpy((*dst)->fp, src->fp, np * sizeof(double));
  SUCCEED_OR_RETURN( pspio_interp_alloc(&(*dst)->fp_interp, src->interp_method, np) );
  SUCCEED_OR_RETURN( pspio_interp_init((*dst)->fp_interp, (*dst)->mesh, (*dst)->fp) );

  memcpy((*dst)->fpp, src->fpp, np * sizeof(double));
  SUCCEED_OR_RETURN( pspio_interp_alloc(&(*dst)->fpp_interp, src->interp_method, np) );
  SUCCEED_OR_RETURN( pspio_interp_init((*dst)->fpp_interp, (*dst)->mesh, (*dst)->fpp) );

  return PSPIO_SUCCESS;
}

void pspio_meshfunc_free(pspio_meshfunc_t *func)
{
  if (func != NULL) {
    pspio_mesh_free(func->mesh);

    free(func->f);
    pspio_interp_free(func->f_interp);

    free(func->fp);
    pspio_interp_free(func->fp_interp);

    free(func->fpp);
    pspio_interp_free(func->fpp_interp);

    free(func);
  }
}


/**********************************************************************
 * Getters                                                            *
 **********************************************************************/

const double *pspio_meshfunc_get_function(const pspio_meshfunc_t *func)
{
  assert(func != NULL);

  return func->f;
}

const double *pspio_meshfunc_get_deriv1(const pspio_meshfunc_t *func)
{
  assert(func != NULL);

  return func->fp;
}

const double *pspio_meshfunc_get_deriv2(const pspio_meshfunc_t *func)
{
  assert(func != NULL);

  return func->fpp;
}

int pspio_meshfunc_get_interp_method(const pspio_meshfunc_t *func)
{
  assert(func != NULL);

  return func->interp_method;
}

const pspio_mesh_t *pspio_meshfunc_get_mesh(const pspio_meshfunc_t *func)
{
  assert(func != NULL);

  return func->mesh;
}


/**********************************************************************
 * Utility routines                                                   *
 **********************************************************************/

int pspio_meshfunc_cmp(const pspio_meshfunc_t *meshfunc1,
                       const pspio_meshfunc_t *meshfunc2)
{
  int i;

  assert(meshfunc1 != NULL);
  assert(meshfunc2 != NULL);

  if ( pspio_mesh_cmp(meshfunc1->mesh, meshfunc2->mesh) == PSPIO_EQUAL) {
    for (i=0; i<pspio_mesh_get_np(meshfunc1->mesh); i++) {
      if ( (meshfunc1->f[i]   != meshfunc2->f[i])  ||
           (meshfunc1->fp[i]  != meshfunc2->fp[i]) ||
           (meshfunc1->fpp[i] != meshfunc2->fpp[i])
         ) return PSPIO_DIFF;
    }
    return PSPIO_EQUAL;
  } else {
    return PSPIO_DIFF;
  }
}

double pspio_meshfunc_eval(const pspio_meshfunc_t *func, double r)
{
  assert(func != NULL);

  /*
    If the value of r is smaller than the first mesh point or if
    it is greater or equal to the last mesh point, then we use a
    linear extrapolation to evaluate the function at r.
  */
  if ( r < func->mesh->r[0] ) {
    return linear_extrapolation(func->mesh->r[0], func->mesh->r[1], func->f[0], func->f[1], r);
  } else if ( r >= func->mesh->r[func->mesh->np-1] ) {
    return linear_extrapolation(func->mesh->r[func->mesh->np-2], func->mesh->r[func->mesh->np-1], 
				func->f[func->mesh->np-2], func->f[func->mesh->np-1], r);
  } else {
    return pspio_interp_eval(func->f_interp, r);
  }
}

double pspio_meshfunc_eval_deriv(const pspio_meshfunc_t *func, double r)
{
  assert(func != NULL);

  /* If the value of r is smaller than the first mesh point or if
     it is greater or equal to the last mesh point, then we use a
     linear extrapolation to evaluate the function at r.
  */
  if ( r < func->mesh->r[0] ) {
    return linear_extrapolation(func->mesh->r[0], func->mesh->r[1], func->fp[0], func->fp[1], r);
  } else if ( r >= func->mesh->r[func->mesh->np-1] ) {
    return linear_extrapolation(func->mesh->r[func->mesh->np-2], func->mesh->r[func->mesh->np-1], 
				func->fp[func->mesh->np-2], func->fp[func->mesh->np-1], r);
  } else {
    return pspio_interp_eval(func->fp_interp, r);
  }
}

double pspio_meshfunc_eval_deriv2(const pspio_meshfunc_t *func, double r)
{
  assert(func != NULL);

  /*
    If the value of r is smaller than the first mesh point or if
    it is greater or equal to the last mesh point, then we use a
    linear extrapolation to evaluate the function at r.
  */
  if ( r < func->mesh->r[0] ) {
    return linear_extrapolation(func->mesh->r[0], func->mesh->r[1], func->fpp[0], func->fpp[1], r);
  } else if ( r >= func->mesh->r[func->mesh->np-1] ) {
    return linear_extrapolation(func->mesh->r[func->mesh->np-2], func->mesh->r[func->mesh->np-1], 
				func->fpp[func->mesh->np-2], func->fpp[func->mesh->np-1], r);
  } else {
    return pspio_interp_eval(func->fpp_interp, r);
  }
}
