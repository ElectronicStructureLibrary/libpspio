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

 $Id: pspio_state_eigenval_set.c 49 2011-08-29 23:42:09Z micael $
*/

#include <stdio.h>
#include <string.h>
#include "pspio_error.h"
#include "pspio_state.h"

int pspio_state_eigenval_set(pspio_state_t state, const double *eigenval) {
  int eigenval_size;

  if ( state.eigenval != NULL ) {
    return PSPIO_ERROR;
  }

  eigenval_size = sizeof(eigenval);

  state.eigenval = (double *)malloc(eigenval_size);
  if ( state.eigenval == NULL ) {
    return PSPIO_ERROR;
  } else {
    memcpy(state.eigenval,eigenval,eigenval_size);
  }

  return PSPIO_SUCCESS;
}


int pspio_state_label_set(pspio_state_t state, const char *label) {
  int label_size;

  if ( state.label != NULL ) {
    return PSPIO_ERROR;
  }

  label_size = sizeof(label);

  state.label = (char *)malloc(label_size);
  if ( state.label == NULL ) {
    return PSPIO_ERROR;
  } else {
    memcpy(state.label,label,label_size);
  }

  return PSPIO_SUCCESS;
}


int pspio_state_np_set(pspio_state_t state, const int np) {

  state.np = np;

  return PSPIO_SUCCESS;
}


int pspio_state_occ_set(pspio_state_t state, const double *occ) {
  int occ_size;

  if ( state.occ != NULL ) {
    return PSPIO_ERROR;
  }

  occ_size = sizeof(occ);

  state.occ = (double *)malloc(occ_size);
  if ( state.occ == NULL ) {
    return PSPIO_ERROR;
  } else {
    memcpy(state.occ,occ,occ_size);
  }

  return PSPIO_SUCCESS;
}


int pspio_state_rc_set(pspio_state_t state, const double *rc) {
  int rc_size;

  if ( state.rc != NULL ) {
    return PSPIO_ERROR;
  }

  rc_size = sizeof(rc);

  state.rc = (double *)malloc(rc_size);
  if ( state.rc == NULL ) {
    return PSPIO_ERROR;
  } else {
    memcpy(state.rc,rc,rc_size);
  }

  return PSPIO_SUCCESS;
}


int pspio_state_wf_set(pspio_state_t state, const double *wf) {
  int wf_size;

  if ( state.wf != NULL ) {
    return PSPIO_ERROR;
  }

  wf_size = sizeof(wf);

  state.wf = (double *)malloc(wf_size);
  if ( state.wf == NULL ) {
    return PSPIO_ERROR;
  } else {
    memcpy(state.wf,wf,wf_size);
  }

  return PSPIO_SUCCESS;
}


int pspio_state_wfp_set(pspio_state_t state, const double *wfp) {
  int wfp_size;

  if ( state.wfp != NULL ) {
    return PSPIO_ERROR;
  }

  wfp_size = sizeof(wfp);

  state.wfp = (double *)malloc(wfp_size);
  if ( state.wfp == NULL ) {
    return PSPIO_ERROR;
  } else {
    memcpy(state.wfp,wfp,wfp_size);
  }

  return PSPIO_SUCCESS;
}
