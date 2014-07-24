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

int pspio_xc_alloc(pspio_xc_t **xc, const int nlcc_scheme, const int np){
  int ierr;

  assert(xc != NULL);
  assert(*xc == NULL);

  switch (nlcc_scheme) {
  case PSPIO_NLCC_NONE:
    break;
  case PSPIO_NLCC_UNKNOWN:
    break;
  case PSPIO_NLCC_LOUIE:
    break;
  case PSPIO_NLCC_FHI:
    break;
  case PSPIO_NLCC_TETER1:
    break;
  case PSPIO_NLCC_TETER2:
    break;
  default:
    return PSPIO_EVALUE;
  }

  *xc = (pspio_xc_t *) malloc (sizeof(pspio_xc_t));
  CHECK_ERROR(*xc != NULL, PSPIO_ENOMEM);

  (*xc)->nlcc_scheme = nlcc_scheme;
  (*xc)->core_dens = NULL;
  if (nlcc_scheme != PSPIO_NLCC_NONE) {
    CHECK_ERROR(np > 1, PSPIO_EVALUE);
    ierr = pspio_meshfunc_alloc(&(*xc)->core_dens, np);
    if (ierr) {
      pspio_xc_free(xc);
      HANDLE_ERROR(ierr);
    }
  }

  return PSPIO_SUCCESS;
}


void pspio_xc_set(pspio_xc_t **xc, const int exchange, const int correlation){
  assert(*xc != NULL);

  (*xc)->exchange = exchange;
  (*xc)->correlation = correlation;
}


int pspio_xc_nlcc_set(pspio_xc_t **xc, const pspio_mesh_t *mesh, const double *cd, const double *cdp, const double *cdpp){

  HANDLE_FUNC_ERROR(pspio_meshfunc_set(&(*xc)->core_dens, mesh, cd, cdp, cdpp));

  return PSPIO_SUCCESS;
}


void pspio_xc_free(pspio_xc_t **xc){

  if (*xc != NULL) {
    pspio_meshfunc_free(&(*xc)->core_dens);
    free(*xc);
    *xc = NULL;
  }
}


/**********************************************************************
 * Atomic routines                                                    *
 **********************************************************************/

void pspio_xc_get_id(const pspio_xc_t *xc, int *exchange, int *correlation){
  assert(xc != NULL);

  *exchange = xc->exchange;
  *correlation = xc->correlation;
}

void pspio_xc_has_nlcc(const pspio_xc_t *xc, int *has_nlcc) {
  assert(xc != NULL);

  *has_nlcc = (xc->nlcc_scheme != PSPIO_NLCC_NONE);
}

void pspio_xc_nlcc_eval(const pspio_xc_t *xc, const int np, const double *r, double *core_dens) {
  assert(xc != NULL);
  assert(r != NULL);
  assert(core_dens != NULL);
  assert(xc->nlcc_scheme != PSPIO_NLCC_NONE);

  pspio_meshfunc_eval(xc->core_dens, np, r, core_dens);
}

void pspio_xc_nlcc_get(const pspio_xc_t *xc, pspio_meshfunc_t **cd_func) {
  assert(xc != NULL);
  assert(xc->nlcc_scheme != PSPIO_NLCC_NONE);

  (*cd_func) = xc->core_dens;
}
