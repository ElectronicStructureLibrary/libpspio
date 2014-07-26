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

#include <assert.h>

#include "interpolation.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

int interpolation_alloc(interpolation_t **interp, const int method, const int np) {
  
  assert(interp != NULL);
  assert(np > 1);

  *interp = (interpolation_t *) malloc (sizeof(interpolation_t));
  FULFILL_OR_EXIT(*interp != NULL, PSPIO_ENOMEM);

  //Make sure all pointers are initialized to NULL, as only some of them will be used
#ifdef HAVE_GSL
  (*interp)->gsl_spl = NULL;
  (*interp)->gsl_acc = NULL;
#endif

  (*interp)->method = method;
  switch (method) {
#ifdef HAVE_GSL
  case PSPIO_INTERP_GSL_CSPLINE:
    (*interp)->gsl_spl = gsl_spline_alloc(gsl_interp_cspline, np);
    (*interp)->gsl_acc = gsl_interp_accel_alloc();
    break;
#endif
  case PSPIO_INTERP_JB_CSPLINE:
    //Not yet implemented
  default:
    RETURN_WITH_ERROR(PSPIO_ENOSUPPORT);
  }

  return PSPIO_SUCCESS;
}


int interpolation_set(interpolation_t **interp, const pspio_mesh_t *mesh, const double *f) {
  assert(interp != NULL);
  assert(*interp != NULL);
  assert(mesh != NULL);
  assert(f != NULL);

  switch ((*interp)->method) {
#ifdef HAVE_GSL
  case PSPIO_INTERP_GSL_CSPLINE:
    int ierr;
    ierr = gsl_spline_init((*interp)->gsl_spl, mesh->r, f, mesh->np);
    if ( ierr ) {
      RETURN_WITH_ERROR( PSPIO_EGSL );
    }
    break;
#endif
  case PSPIO_INTERP_JB_CSPLINE:
    //Not yet implemented
  default:
    RETURN_WITH_ERROR(PSPIO_ENOSUPPORT);
  }
  
  return PSPIO_SUCCESS;
}


void interpolation_free(interpolation_t **interp) {

  if (*interp != NULL) {

    switch ((*interp)->method) {
#ifdef HAVE_GSL
    case PSPIO_INTERP_GSL_CSPLINE:
      gsl_spline_free((*interp)->gsl_spl);
      gsl_interp_accel_free((*interp)->gsl_acc);
      break;
#endif
    }

    free(*interp);
    *interp = NULL;
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
  }

}
