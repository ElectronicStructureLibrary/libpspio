/* Copyright (C) 2011-2017 Micael Oliveira <micael.oliveira@mpsd.mpg.de>
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

#include "pspio_xc.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

int pspio_xc_alloc(pspio_xc_t **xc)
{
  assert(xc != NULL);
  assert(*xc == NULL);

  *xc = (pspio_xc_t *) malloc (sizeof(pspio_xc_t));
  FULFILL_OR_RETURN( *xc != NULL, PSPIO_ENOMEM );

  (*xc)->correlation = XC_NONE;
  (*xc)->exchange = XC_NONE;

  (*xc)->nlcc_scheme = PSPIO_NLCC_NONE;
  (*xc)->nlcc_pf_scale = 0.0;
  (*xc)->nlcc_pf_value = 0.0;
  (*xc)->nlcc_dens = NULL;

  return PSPIO_SUCCESS;
}

int pspio_xc_init(pspio_xc_t *xc, int exchange, int correlation,
		  int nlcc_scheme, double nlcc_pfs, double nlcc_pfv,
		  const pspio_mesh_t *mesh,
		  const double *cd, const double *cdd, const double *cddd)
{
  assert(xc != NULL);  

  SUCCEED_OR_RETURN( pspio_xc_set_exchange(xc, exchange) );
  SUCCEED_OR_RETURN( pspio_xc_set_correlation(xc, correlation) );
  SUCCEED_OR_RETURN( pspio_xc_set_nlcc_scheme(xc, nlcc_scheme) );
  if ( nlcc_scheme == PSPIO_NLCC_NONE ) {
    SUCCEED_OR_RETURN( pspio_xc_set_nlcc_prefactors(xc, 0.0, 0.0) );
  } else {
    SUCCEED_OR_RETURN( pspio_xc_set_nlcc_prefactors(xc, nlcc_pfs, nlcc_pfv) );
    SUCCEED_OR_RETURN( pspio_xc_set_nlcc_density(xc, mesh, cd, cdd, cddd) );
  }

  return PSPIO_SUCCESS;
}

int pspio_xc_copy(pspio_xc_t **dst, const pspio_xc_t *src)
{
  assert(src != NULL);

  if ( *dst == NULL ) {
    SUCCEED_OR_RETURN( pspio_xc_alloc(dst) );
  }

  (*dst)->exchange = src->exchange;
  (*dst)->correlation = src->correlation;
  (*dst)->nlcc_scheme = src->nlcc_scheme;
  (*dst)->nlcc_pf_scale = src->nlcc_pf_scale;
  (*dst)->nlcc_pf_value = src->nlcc_pf_value;

  if (src->nlcc_scheme != PSPIO_NLCC_NONE) {
    if ( (*dst)->nlcc_dens != NULL) {
      if ( pspio_mesh_get_np(pspio_meshfunc_get_mesh(src->nlcc_dens)) != 
	   pspio_mesh_get_np(pspio_meshfunc_get_mesh((*dst)->nlcc_dens)) ) {
	pspio_meshfunc_free((*dst)->nlcc_dens);
	(*dst)->nlcc_dens = NULL;
      }
    }

    SUCCEED_OR_RETURN( pspio_meshfunc_copy(&(*dst)->nlcc_dens, src->nlcc_dens) );
  }

  return PSPIO_SUCCESS;
}

void pspio_xc_free(pspio_xc_t *xc)
{
  if (xc != NULL) {
    if ( xc->nlcc_dens != NULL) {
      pspio_meshfunc_free(xc->nlcc_dens);
    }
    free(xc);
  }
}


/**********************************************************************
 * Setters                                                            *
 **********************************************************************/

int pspio_xc_set_exchange(pspio_xc_t *xc, int exchange)
{
  assert(xc != NULL);

  xc->exchange = exchange;

  return PSPIO_SUCCESS;
}

int pspio_xc_set_correlation(pspio_xc_t *xc, int correlation)
{
  assert(xc != NULL);

  xc->correlation = correlation;

  return PSPIO_SUCCESS;
}

int pspio_xc_set_nlcc_prefactors(pspio_xc_t *xc, double nlcc_pfs,
      double nlcc_pfv)
{
  xc->nlcc_pf_scale = nlcc_pfs;
  xc->nlcc_pf_value = nlcc_pfv;

  return PSPIO_SUCCESS;
}

int pspio_xc_set_nlcc_scheme(pspio_xc_t *xc, int nlcc_scheme)
{
  assert(xc != NULL);

  switch (nlcc_scheme) {
  case PSPIO_NLCC_NONE:
  case PSPIO_NLCC_UNKNOWN:
  case PSPIO_NLCC_LOUIE:
  case PSPIO_NLCC_FHI:
  case PSPIO_NLCC_TETER1:
  case PSPIO_NLCC_TETER2:
  case PSPIO_NLCC_ATOM:
    break;    
  default:
    return PSPIO_EVALUE;
  }
  xc->nlcc_scheme = nlcc_scheme;

  return PSPIO_SUCCESS;
}

int pspio_xc_set_nlcc_density(pspio_xc_t *xc, const pspio_mesh_t *mesh,
			      const double *cd, const double *cdp, 
			      const double *cdpp)
{
  int ierr;

  assert(xc != NULL);  
  assert(xc->nlcc_scheme != PSPIO_NLCC_NONE);

  ierr = pspio_meshfunc_alloc(&(xc->nlcc_dens), pspio_mesh_get_np(mesh));
  if ( ierr != PSPIO_SUCCESS ) {
    pspio_meshfunc_free(xc->nlcc_dens);
    xc->nlcc_dens = NULL;
    RETURN_WITH_ERROR( ierr );
  }

  SUCCEED_OR_RETURN( pspio_meshfunc_init(xc->nlcc_dens, mesh, cd, cdp, cdpp) );

  return PSPIO_SUCCESS;
}


/**********************************************************************
 * Getters                                                            *
 **********************************************************************/

int pspio_xc_get_exchange(const pspio_xc_t *xc)
{
  assert(xc != NULL);

  return xc->exchange;
}

int pspio_xc_get_correlation(const pspio_xc_t *xc)
{
  assert(xc != NULL);

  return xc->correlation;
}

int pspio_xc_get_nlcc_pf_scale(const pspio_xc_t *xc)
{
  assert(xc != NULL);

  return xc->nlcc_pf_scale;
}

int pspio_xc_get_nlcc_pf_value(const pspio_xc_t *xc)
{
  assert(xc != NULL);

  return xc->nlcc_pf_value;
}

int pspio_xc_get_nlcc_scheme(const pspio_xc_t *xc)
{
  assert(xc != NULL);

  return xc->nlcc_scheme;
}

const pspio_meshfunc_t *pspio_xc_get_nlcc_density(const pspio_xc_t *xc)
{
  assert(xc != NULL);
  assert(xc->nlcc_scheme != PSPIO_NLCC_NONE);

  return xc->nlcc_dens;
}


/**********************************************************************
 * Utility routines                                                   *
 **********************************************************************/

int pspio_xc_cmp(const pspio_xc_t *xc1, const pspio_xc_t *xc2) {

  if ((xc1->correlation != xc2->correlation) ||
      (xc1->exchange != xc2->exchange) ||
      (xc1->nlcc_scheme != xc2->nlcc_scheme)) {
    return PSPIO_DIFF;
  } else {
    if (xc1->nlcc_scheme != PSPIO_NLCC_NONE) {
      if ( (xc1->nlcc_pf_scale == xc2->nlcc_pf_scale) &&
                   (xc1->nlcc_pf_value == xc2->nlcc_pf_value) ) {
        return pspio_meshfunc_cmp(xc1->nlcc_dens, xc2->nlcc_dens) == PSPIO_EQUAL ? PSPIO_EQUAL : PSPIO_DIFF;
      } else {
        return PSPIO_DIFF;
      }
    } else {
      return PSPIO_EQUAL;
    }
  }
}

double pspio_xc_nlcc_density_eval(const pspio_xc_t *xc, double r)
{
  assert(xc != NULL);

  if (xc->nlcc_scheme != PSPIO_NLCC_NONE) {
    return pspio_meshfunc_eval(xc->nlcc_dens, r);
  } else {
    return 0.0;
  }
}

double pspio_xc_nlcc_density_eval_deriv(const pspio_xc_t *xc, double r)
{
  assert(xc != NULL);

  if (xc->nlcc_scheme != PSPIO_NLCC_NONE) {
    return pspio_meshfunc_eval_deriv(xc->nlcc_dens, r);
  } else {
    return 0.0;
  }
}

double pspio_xc_nlcc_density_eval_deriv2(const pspio_xc_t *xc, double r)
{
  assert(xc != NULL);

  if (xc->nlcc_scheme != PSPIO_NLCC_NONE) {
    return pspio_meshfunc_eval_deriv2(xc->nlcc_dens, r);
  } else {
    return 0.0;
  }
}

int pspio_xc_has_nlcc(const pspio_xc_t *xc)
{
  assert (xc != NULL);

  return (xc->nlcc_scheme != PSPIO_NLCC_NONE);
}
