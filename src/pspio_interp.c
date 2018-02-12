/* Copyright (C) 2014-2016 Micael Oliveira <micael.oliveira@mpsd.mpg.de>
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
#include <assert.h>
#include <string.h>

#include "pspio_interp.h"
#include "pspio_jb_spline.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_GSL
#include <gsl/gsl_errno.h>
#endif


/**********************************************************************
 * Data structures                                                    *
 **********************************************************************/

struct pspio_interp_t {
  int method; /**< Interpolation method */
  int size; /**< Interpolation size */

#ifdef HAVE_GSL
  /* Objects to the used with GSL interpolation */
  gsl_spline *gsl_spl;       /**< gsl spline structure */
  gsl_interp_accel *gsl_acc; /**< gsl accelerator for interpolation lookups */
#endif

  /* Objects to be used with jb_spline */
  jb_spline_t *jb_spl;       /**< JB spline structure */
};

/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

int pspio_interp_alloc(pspio_interp_t **interp, int method, int size)
{
  assert(interp != NULL);
  assert(size > 1);

  *interp = (pspio_interp_t *) malloc (sizeof(pspio_interp_t));
  FULFILL_OR_EXIT(*interp != NULL, PSPIO_ENOMEM);

  /* Make sure all pointers are initialized to NULL, as only some of them will be used */
#ifdef HAVE_GSL
  (*interp)->gsl_spl = NULL;
  (*interp)->gsl_acc = NULL;
#endif
  (*interp)->jb_spl = NULL;

  (*interp)->method = method;
  (*interp)->size = size;
  switch (method) {
#ifdef HAVE_GSL
  case PSPIO_INTERP_GSL_CSPLINE:
    (*interp)->gsl_spl = gsl_spline_alloc(gsl_interp_cspline, (*interp)->size);
    (*interp)->gsl_acc = gsl_interp_accel_alloc();
    break;
#endif
  case PSPIO_INTERP_JB_CSPLINE:
    SUCCEED_OR_RETURN( jb_spline_alloc(&((*interp)->jb_spl), (*interp)->size) );
    break;
  default:
    RETURN_WITH_ERROR( PSPIO_ENOSUPPORT );
  }

  return PSPIO_SUCCESS;
}

int pspio_interp_copy(pspio_interp_t **dst, const pspio_interp_t *src)
{
  assert(src != NULL);

  if ( *dst != NULL ) {
    pspio_interp_free(*dst);
  }
  SUCCEED_OR_RETURN(pspio_interp_alloc(dst, src->method, src->size));

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

int pspio_interp_init(pspio_interp_t *interp, const pspio_mesh_t *mesh, const double *f)
{
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

void pspio_interp_free(pspio_interp_t *interp) {

  if (interp != NULL) {

    switch (interp->method) {
#ifdef HAVE_GSL
    case PSPIO_INTERP_GSL_CSPLINE:
      gsl_spline_free(interp->gsl_spl);
      gsl_interp_accel_free(interp->gsl_acc);
      break;
#endif
    case PSPIO_INTERP_JB_CSPLINE:
      jb_spline_free(interp->jb_spl);
      break;
    default:
      /* Nothing do do */
      break;
    }

    free(interp);
  }
}


/**********************************************************************
 * Utility routines                                                   *
 **********************************************************************/

double pspio_interp_eval(const pspio_interp_t *interp, double r)
{
  assert(interp != NULL);

  switch (interp->method) {
#ifdef HAVE_GSL
  case PSPIO_INTERP_GSL_CSPLINE:
    return gsl_spline_eval(interp->gsl_spl, r, interp->gsl_acc);
#endif
  case PSPIO_INTERP_JB_CSPLINE:
    return jb_spline_eval(interp->jb_spl, r);
  default:
    return 0.0;
  }
}

double pspio_interp_eval_deriv(const pspio_interp_t *interp, double r)
{
  assert(interp != NULL);

  switch (interp->method) {
#ifdef HAVE_GSL
  case PSPIO_INTERP_GSL_CSPLINE:
    return gsl_spline_eval_deriv(interp->gsl_spl, r, interp->gsl_acc);
#endif
  case PSPIO_INTERP_JB_CSPLINE:
    return jb_spline_eval_deriv(interp->jb_spl, r);
  default:
    return 0.0;
  }
}

double pspio_interp_eval_deriv2(const pspio_interp_t *interp, double r)
{
  assert(interp != NULL);

  switch (interp->method) {
#ifdef HAVE_GSL
  case PSPIO_INTERP_GSL_CSPLINE:
    return gsl_spline_eval_deriv2(interp->gsl_spl, r, interp->gsl_acc);
#endif
  case PSPIO_INTERP_JB_CSPLINE:
    return jb_spline_eval_deriv2(interp->jb_spl, r);
  default:
    return 0.0;
  }
}
