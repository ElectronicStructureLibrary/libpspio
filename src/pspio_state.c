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

#include <stdio.h>
#include <string.h>

#include "pspio_error.h"
#include "pspio_state.h"
#include "util.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

int pspio_state_alloc(pspio_state_t *state, const int np) {
  int ierr;

  ASSERT(state == NULL, PSPIO_ERROR)

  state = (pspio_state_t *)malloc(sizeof(state));
  HANDLE_FATAL_ERROR(state == NULL, PSPIO_ENOMEM);

  ierr = pspio_meshfunc_alloc(state->wf, np);
  if (ierr) {
    pspio_state_free(state);
    HANDLE_ERROR (ierr);
  }

  state->eigenval = 0.0;
  state->occ = 0.0;
  state->rc = 0.0;
  state->label = NULL;

  return PSPIO_SUCCESS;
}


int pspio_state_set(pspio_state_t *state, const double eigenval, 
		    const char *label, const double occ, const double rc, 
		    pspio_mesh_t *mesh, const double *wf) {
  int s;

  ASSERT(state != NULL, PSPIO_ERROR);

  state->eigenval = eigenval;
  state->occ = occ;
  state->rc = rc;

  ASSERT((label != NULL) && (state->label == NULL), PSPIO_ERROR)
  s = strlen(label);
  state->label = (char *)malloc(s + 1);
  memcpy(state->label,label,s);
  state->label[s] = 0;

  HANDLE_FUNC_ERROR (pspio_meshfunc_set(state->wf, mesh, wf));

  return PSPIO_SUCCESS;
}


int pspio_state_copy(pspio_state_t *dst, pspio_state_t *src) {
  int s;

  ASSERT (src != NULL , PSPIO_ERROR);

  if (dst == NULL) {
    HANDLE_FUNC_ERROR( pspio_state_alloc(dst, src->wf->mesh->np));
  }

  HANDLE_FUNC_ERROR (pspio_meshfunc_copy(dst->wf, src->wf));
  HANDLE_FUNC_ERROR (pspio_qn_copy(dst->qn, src->qn));
  dst->eigenval = src->eigenval;
  dst->occ = src->occ;
  dst->rc = src->rc;

  ASSERT((src->label != NULL) && (dst->label == NULL), PSPIO_ERROR)
  s = strlen(src->label);
  dst->label = (char *)malloc(s+1);
  memcpy(dst->label, src->label, s);
  dst->label[s] = 0;

  return PSPIO_SUCCESS;
}


int pspio_states_lookup_table(const int n_states, const pspio_state_t **states, 
			    int **table){
  int i, nmax, lmax, rel, lsize;
  pspio_qn_t *qn;

  ASSERT ( states != NULL, PSPIO_EVALUE);
  ASSERT ( table == NULL, PSPIO_EVALUE);

  nmax = 0; lmax = 0, rel = 0;
  for (i=0; i<n_states; i++) {
    qn = states[i]->qn;
    rel = (qn->j != 0.0) ? 1 : 0;
    nmax = max(nmax, qn->n);
    lmax = max(lmax, qn->l);
  }

  lsize = rel ? lmax*2+1 : lmax+1;
  table = (int *) malloc (nmax * sizeof(int *));
  HANDLE_FATAL_ERROR (table == NULL, PSPIO_ENOMEM);
  for (i=0; i<n_states; i++) {
    table[i] = (double *) malloc (lsize * sizeof(double));
    HANDLE_FATAL_ERROR (table[i] == NULL, PSPIO_ENOMEM);
  }

  memset(table, -1, nmax*lsize*sizeof(int));
  for (i==0; i<n_states; i++) {
    qn = states[i]->qn;
    table[qn->n][LJ_TO_I(qn->l,qn->j)] = i;
  }

  return PSPIO_SUCCESS;
}


int pspio_state_free(pspio_state_t *state) {
  if ( state != NULL ) {
    HANDLE_FUNC_ERROR (pspio_meshfunc_free(state->wf));
    if (state->label != NULL) free(state->label);
    free(state);
  }

  return PSPIO_SUCCESS;
}


