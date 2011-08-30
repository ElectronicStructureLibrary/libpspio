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

/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

int pspio_state_alloc(pspio_state_t *state) {
  if ( state != NULL ) {
    return PSPIO_ERROR;
  }
  state = (pspio_state_t *)malloc(sizeof(state));
  if ( state == NULL ) {
    return PSPIO_ERROR;
  }

  state->np = -1;
  state->eigenval = 0.0;
  state->occ = 0.0;
  state->rc = 0.0;
  state->label = NULL;
  state->wf = NULL;
  state->wfp = NULL;

  return PSPIO_SUCCESS;
}


int pspio_state_copy(pspio_state_t *dst, pspio_state_t *src) {
  int s;

  if ( dst != NULL ) {
    return PSPIO_ERROR;
  }
  dst = (pspio_state_t *)malloc(sizeof(dst));
  if ( dst == NULL ) {
    return PSPIO_ERROR;
  }

  if ( (src->wf == NULL) || (dst->wf != NULL) ) {
    return PSPIO_ERROR;
  }
  s = src->np * sizeof(double);
  dst->wf = (double *)malloc(s);
  memcpy(dst->wf, src->wf, s);

  pspio_qn_copy(dst->qn, src->qn);

  dst->eigenval = src->eigenval;
  dst->np = src->np;
  dst->occ = src->occ;
  dst->rc = src->rc;

  if ( (src->label == NULL) || (dst->label != NULL) ) {
    return PSPIO_ERROR;
  }
  s = strlen(src->label);
  dst->label = (char *)malloc(s+1);
  memcpy(dst->label, src->label, s);
  dst->label[s] = 0;

  if ( (src->wfp == NULL) || (dst->wfp != NULL) ) {
    return PSPIO_ERROR;
  }
  s = src->np * sizeof(double);
  dst->wfp = (double *)malloc(s);
  memcpy(dst->wfp, src->wfp, s);

  return PSPIO_SUCCESS;
}


int pspio_state_free(pspio_state_t *state) {
  if ( state != NULL ) {
    free(state);
  }

  return PSPIO_SUCCESS;
}


int pspio_state_set(pspio_state_t *state, const int np, const double eigenval,
      const char *label, const double occ, const double rc, const double *wf,
      const double *wfp) {
  int s;

  if ( state == NULL ) {
    return PSPIO_ERROR;
  }

  state->np = np;
  state->eigenval = eigenval;
  state->occ = occ;
  state->rc = rc;

  if ( (label == NULL) || (state->label != NULL) ) {
    return PSPIO_ERROR;
  }
  s = strlen(label);
  state->label = (char *)malloc(s + 1);
  memcpy(state->label,label,s);
  state->label[s] = 0;

  if ( (wf == NULL) || (state->wf != NULL) ) {
    return PSPIO_ERROR;
  }
  state->wf = (double *)malloc(np);
  if ( state->wf == NULL ) {
    return PSPIO_ERROR;
  }
  memcpy(state->wf,wf,np);

  if ( (wfp == NULL) || (state->wfp != NULL) ) {
    return PSPIO_ERROR;
  }
  state->wfp = (double *)malloc(np);
  if ( state->wfp == NULL ) {
    return PSPIO_ERROR;
  }
  memcpy(state->wfp,wfp,np);

  return PSPIO_SUCCESS;
}
