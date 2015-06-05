/*
 Copyright (C) 2014 M. Oliveira

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
#include <string.h>

#include "interpolation.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_GSL
#include <gsl/gsl_errno.h>
#endif


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

int interpolation_alloc(interpolation_t **interp, const int method, const int np) {
  
  assert(interp != NULL);
  assert(np > 1);

  *interp = (interpolation_t *) malloc (sizeof(interpolation_t));
  FULFILL_OR_EXIT(*interp != NULL, PSPIO_ENOMEM);

  /* Make sure all pointers are initialized to NULL, as only some of them will be used */
#ifdef HAVE_GSL
  (*interp)->gsl_spl = NULL;
  (*interp)->gsl_acc = NULL;
#endif
  (*interp)->jb_spl = NULL;

  (*interp)->method = method;
  switch (method) {
#ifdef HAVE_GSL
  case PSPIO_INTERP_GSL_CSPLINE:
    (*interp)->gsl_spl = gsl_spline_alloc(gsl_interp_cspline, np);
    (*interp)->gsl_acc = gsl_interp_accel_alloc();
    break;
#endif
  case PSPIO_INTERP_JB_CSPLINE:
    SUCCEED_OR_RETURN( jb_spline_alloc(&((*interp)->jb_spl), np) );
    break;
  default:
    RETURN_WITH_ERROR( PSPIO_ENOSUPPORT );
  }

  return PSPIO_SUCCESS;
}


int interpolation_copy(interpolation_t **dst, const interpolation_t *src) {
  int np;

  assert(src != NULL);

#ifdef HAVE_GSL
  np = src->gsl_spl->size;
#else
  np = src->jb_spl->np;
#endif

  if ( *dst != NULL ) {
    interpolation_free(*dst);
  }
  SUCCEED_OR_RETURN(interpolation_alloc(dst, src->method, np));

  switch (src->method) {
#ifdef HAVE_GSL
    case PSPIO_INTERP_GSL_CSPLINE:
      (*dst)->gsl_acc->cache = src->gsl_acc->cache;
      (*dst)->gsl_acc->miss_count = src->gsl_acc->miss_count;
      (*dst)->gsl_acc->hit_count = src->gsl_acc->hit_count;
      (*dst)->gsl_spl->interp->xmin = src->gsl_spl->interp->xmin;
      (*dst)->gsl_spl->interp->xmax = src->gsl_spl->interp->xmax;
      (*dst)->gsl_spl->interp->size = src->gsl_spl->interp->size;
      memcpy((*dst)->gsl_spl->x, src->gsl_spl->x,
        src->gsl_spl->size * sizeof(double));
      memcpy((*dst)->gsl_spl->y, src->gsl_spl->y,
        src->gsl_spl->size * sizeof(double));
      break;
#endif
    case PSPIO_INTERP_JB_CSPLINE:
      SUCCEED_OR_RETURN( jb_spline_copy(&((*dst)->jb_spl), src->jb_spl) );
      break;
    default:
      RETURN_WITH_ERROR( PSPIO_ENOSUPPORT );
  }

  return PSPIO_SUCCESS;
}


int interpolation_init(interpolation_t *interp, const pspio_mesh_t *mesh, const double *f) {
#ifdef HAVE_GSL
  int ierr;
#endif

  assert(interp != NULL);
  assert(mesh != NULL);
  assert(f != NULL);

  switch (interp->method) {
#ifdef HAVE_GSL
    case PSPIO_INTERP_GSL_CSPLINE:
      ierr = gsl_spline_init(interp->gsl_spl, mesh->r, f, mesh->np);
      if ( ierr != GSL_SUCCESS ) {
        RETURN_WITH_ERROR( PSPIO_EGSL );
      }
      break;
#endif
    case PSPIO_INTERP_JB_CSPLINE:
      SUCCEED_OR_RETURN( jb_spline_init(&interp->jb_spl, mesh->r, f, mesh->np) );
      break;
    default:
      RETURN_WITH_ERROR( PSPIO_ENOSUPPORT );
  }

  return PSPIO_SUCCESS;
}


void interpolation_free(interpolation_t *interp) {

  if (interp != NULL) {

    switch (interp->method) {
#ifdef HAVE_GSL
    case PSPIO_INTERP_GSL_CSPLINE:
      gsl_spline_free(interp->gsl_spl);
      gsl_interp_accel_free(interp->gsl_acc);
      break;
    case PSPIO_INTERP_JB_CSPLINE:
      jb_spline_free(interp->jb_spl);
      break;
#endif
    }

    free(interp);
  }
}


/**********************************************************************
 * Atomic routines                                                    *
 **********************************************************************/

void interpolation_eval(const interpolation_t *interp, const double r, double *f) {
  assert(interp != NULL);
  assert(f != NULL);

  switch (interp->method) {
#ifdef HAVE_GSL
  case PSPIO_INTERP_GSL_CSPLINE:
    *f = gsl_spline_eval(interp->gsl_spl, r, interp->gsl_acc);
    break;
#endif
  case PSPIO_INTERP_JB_CSPLINE:
    *f = jb_spline_eval(interp->jb_spl, r);
    break;
  }

}


void interpolation_eval_deriv(const interpolation_t *interp, const double r, double *fp) {
  assert(interp != NULL);
  assert(fp != NULL);

  switch (interp->method) {
#ifdef HAVE_GSL
  case PSPIO_INTERP_GSL_CSPLINE:
    *fp = gsl_spline_eval_deriv(interp->gsl_spl, r, interp->gsl_acc);
    break;
#endif
  case PSPIO_INTERP_JB_CSPLINE:
    *fp = jb_spline_eval_deriv(interp->jb_spl, r);
    break;
  }

}


void interpolation_eval_deriv2(const interpolation_t *interp, const double r, double *fpp) {
  assert(interp != NULL);
  assert(fpp != NULL);

  switch (interp->method) {
#ifdef HAVE_GSL
  case PSPIO_INTERP_GSL_CSPLINE:
    *fpp = gsl_spline_eval_deriv2(interp->gsl_spl, r, interp->gsl_acc);
    break;
#endif
  case PSPIO_INTERP_JB_CSPLINE:
    *fpp = jb_spline_eval_deriv2(interp->jb_spl, r);
    break;
  }

}
