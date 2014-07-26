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

#include "pspio_xc.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

int pspio_xc_alloc(pspio_xc_t **xc) {
  assert(xc != NULL);
  assert(*xc == NULL);

  *xc = (pspio_xc_t *) malloc (sizeof(pspio_xc_t));
  FULFILL_OR_RETURN( *xc != NULL, PSPIO_ENOMEM );

  (*xc)->correlation = XC_NONE;
  (*xc)->exchange = XC_NONE;

  (*xc)->nlcc_scheme = PSPIO_NLCC_NONE;
  (*xc)->nlcc_dens = NULL;

  return PSPIO_SUCCESS;
}


void pspio_xc_free(pspio_xc_t **xc){

  if (*xc != NULL) {
    if ( (*xc)->nlcc_dens != NULL) {
      pspio_meshfunc_free(&(*xc)->nlcc_dens);
    }
    free(*xc);
    *xc = NULL;
  }
}


/**********************************************************************
 * Atomic routines                                                    *
 **********************************************************************/

void pspio_xc_set_id(pspio_xc_t **xc, const int exchange,
       const int correlation) {
  assert(*xc != NULL);

  (*xc)->exchange = exchange;
  (*xc)->correlation = correlation;
}

int pspio_xc_set_nlcc_scheme(pspio_xc_t **xc, const int nlcc_scheme) {
  assert(*xc != NULL);

  switch (nlcc_scheme) {
    case PSPIO_NLCC_NONE:
    case PSPIO_NLCC_UNKNOWN:
    case PSPIO_NLCC_LOUIE:
    case PSPIO_NLCC_FHI:
    case PSPIO_NLCC_TETER1:
    case PSPIO_NLCC_TETER2:
      break;

    default:
      return PSPIO_EVALUE;
  }
  (*xc)->nlcc_scheme = nlcc_scheme;

  return PSPIO_SUCCESS;
}

int pspio_xc_set_nlcc_density(pspio_xc_t **xc, const pspio_mesh_t *mesh,
      const double *cd, const double *cdp, const double *cdpp) {
  int ierr;

  assert(*xc != NULL);  
  assert(&(*xc)->nlcc_scheme != PSPIO_NLCC_NONE);

  ierr = pspio_meshfunc_alloc(&(*xc)->nlcc_dens, PSPIO_INTERP_GSL_CSPLINE, pspio_mesh_get_np(mesh));
  if ( ierr != PSPIO_SUCCESS ) {
    pspio_meshfunc_free(&(*xc)->nlcc_dens);
    RETURN_WITH_ERROR(ierr);
  }

  SUCCEED_OR_RETURN( pspio_meshfunc_set(&(*xc)->nlcc_dens, mesh, cd, cdp,
    cdpp) );

  return PSPIO_SUCCESS;
}

void pspio_xc_get_id(const pspio_xc_t *xc, int *exchange, int *correlation) {
  assert(xc != NULL);

  *exchange = xc->exchange;
  *correlation = xc->correlation;
}

void pspio_xc_get_nlcc_scheme(const pspio_xc_t *xc, int *nlcc_scheme) {
  assert(xc != NULL);

  *nlcc_scheme = xc->nlcc_scheme;
}

void pspio_xc_get_nlcc_density(const pspio_xc_t *xc,
       pspio_meshfunc_t **cd_func) {
  assert(xc != NULL);
  assert(xc->nlcc_scheme != PSPIO_NLCC_NONE);

  (*cd_func) = xc->nlcc_dens;
}

void pspio_xc_nlcc_density_eval(const pspio_xc_t *xc, const int np,
       const double *r, double *nlcc_dens) {
  assert(xc != NULL);
  assert(r != NULL);
  assert(nlcc_dens != NULL);
  assert(xc->nlcc_scheme != PSPIO_NLCC_NONE);

  pspio_meshfunc_eval(xc->nlcc_dens, np, r, nlcc_dens);
}

int pspio_xc_has_nlcc(const pspio_xc_t *xc) {
  assert (xc != NULL);

  return (xc->nlcc_scheme != PSPIO_NLCC_NONE);
}
