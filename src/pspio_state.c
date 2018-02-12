/* Copyright (C) 2011-2016 Micael Oliveira <micael.oliveira@mpsd.mpg.de>
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

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include "pspio_state.h"
#include "util.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

int pspio_state_alloc(pspio_state_t **state, int np)
{
  int ierr;

  assert(state != NULL);
  assert(*state == NULL);
  assert(np > 1);

  *state = (pspio_state_t *) malloc (sizeof(pspio_state_t));
  FULFILL_OR_EXIT( *state != NULL, PSPIO_ENOMEM );

  (*state)->wf = NULL;
  ierr = pspio_meshfunc_alloc(&(*state)->wf, np);
  if ( ierr != PSPIO_SUCCESS ) {
    pspio_state_free(*state);
    *state = NULL;
    RETURN_WITH_ERROR( ierr );
  }

  (*state)->qn = NULL;
  ierr = pspio_qn_alloc(&(*state)->qn);
  if ( ierr != PSPIO_SUCCESS ) {
    pspio_state_free(*state);
    *state = NULL;
    RETURN_WITH_ERROR( ierr );
  }

  (*state)->eigenval = 0.0;
  (*state)->occ = 0.0;
  (*state)->rc = 0.0;
  (*state)->label = NULL;

  return PSPIO_SUCCESS;
}

int pspio_state_init(pspio_state_t *state, double eigenval,
		     const pspio_qn_t *qn, double occ, double rc,
		     const pspio_mesh_t *mesh, const double *wf, const char *label)
{
  char qn_label[6];
  size_t s;

  assert(state != NULL);
  assert(state->label == NULL);

  state->eigenval = eigenval;
  state->occ = occ;
  state->rc = rc;
  if (label == NULL) {
    SUCCEED_OR_RETURN( pspio_qn_label(qn, qn_label) );
    s = strlen(qn_label);
    state->label = (char *) malloc((s+1)*sizeof(char));
    memcpy(state->label, qn_label, s);
  } else {
    s = strlen(label);
    state->label = (char *) malloc((s+1)*sizeof(char));
    memcpy(state->label, label, s);
  }
  state->label[s] = '\0';

  SUCCEED_OR_RETURN( pspio_qn_copy(&state->qn, qn) );
  SUCCEED_OR_RETURN( pspio_meshfunc_init(state->wf, mesh, wf, NULL, NULL) );

  return PSPIO_SUCCESS;
}

int pspio_state_copy(pspio_state_t **dst, const pspio_state_t *src)
{
  size_t s;
  int np;

  assert(src != NULL);
  assert((src->label != NULL));

  np = pspio_mesh_get_np(src->wf->mesh);

  if ( *dst == NULL ) {
    SUCCEED_OR_RETURN( pspio_state_alloc(dst, np) );
  }

  /* 
   * The mesh of the destination wavefunction must have the same
   * number of points as the mesh of the source wavefunction
   */
  if ( pspio_mesh_get_np((*dst)->wf->mesh) != np ) {
    pspio_state_free(*dst);
    *dst = NULL;
    SUCCEED_OR_RETURN(pspio_state_alloc(dst, np));
  }

  SUCCEED_OR_RETURN( pspio_meshfunc_copy(&(*dst)->wf, src->wf) );
  SUCCEED_OR_RETURN( pspio_qn_copy(&(*dst)->qn, src->qn) );
  (*dst)->eigenval = src->eigenval;
  (*dst)->occ = src->occ;
  (*dst)->rc = src->rc;
  free((*dst)->label);
  s = strlen(src->label);
  (*dst)->label = (char *) malloc((s+1)*sizeof(char));
  memcpy((*dst)->label, src->label, s);
  (*dst)->label[s] = '\0';

  return PSPIO_SUCCESS;
}

int pspio_states_lookup_table(int n_states, pspio_state_t **states,
			      int ***table_ptr)
{
  int i, nmax, lmax, rel, lsize;
  pspio_qn_t *qn;
  int **table;

  assert(n_states > 0);
  assert(states != NULL);
  assert(table_ptr != NULL);

  /* Determine dimensions of the table */
  nmax = 0; lmax = 0, rel = 0;
  for (i=0; i<n_states; i++) {
    qn = states[i]->qn;
    rel = (qn->j != 0.0) ? 1 : 0;
    nmax = max(nmax, qn->n);
    lmax = max(lmax, qn->l);
  }
  nmax++;
  lsize = rel ? lmax*2+1 : lmax+1;

  /* Allocate memory and preset table */
  table = malloc ( (nmax+1) * sizeof(int*));
  FULFILL_OR_EXIT( table != NULL, PSPIO_ENOMEM );
  for(i=0; i<nmax; i++) {
    table[i] = malloc(lsize*sizeof(int));
    FULFILL_OR_EXIT( table[i] != NULL, PSPIO_ENOMEM );
    memset(table[i], -1, lsize*sizeof(int));
  }
  table[nmax] = NULL;

  /* Build lookup table */
  for (i=0; i<n_states; i++) {
    qn = states[i]->qn;    
    table[qn->n][LJ_TO_I(qn->l,qn->j)] = i;
  }

  *table_ptr = table;
  return PSPIO_SUCCESS;
}

void pspio_state_free(pspio_state_t *state)
{
  if ( state != NULL ) {
    pspio_meshfunc_free(state->wf);
    pspio_qn_free(state->qn);
    free(state->label);
    free(state);
  }
}


/**********************************************************************
 * Getters                                                            *
 **********************************************************************/

const pspio_qn_t *pspio_state_get_qn(const pspio_state_t *state)
{
  assert(state != NULL);

  return state->qn;
}

double pspio_state_get_occ(const pspio_state_t *state)
{
  assert(state != NULL);
  
  return state->occ;
}

double pspio_state_get_ev(const pspio_state_t *state)
{
  assert(state != NULL);

  return state->eigenval;
}

double pspio_state_get_rc(const pspio_state_t *state)
{
  assert(state != NULL);

  return state->rc;
}

const char *pspio_state_get_label(const pspio_state_t *state)
{
  assert(state != NULL);

  return state->label;
}

const pspio_meshfunc_t *pspio_state_get_wf(const pspio_state_t *state)
{
  assert(state != NULL);

  return state->wf;
}

/**********************************************************************
 * Utility routines                                                   *
 **********************************************************************/

int pspio_state_cmp(const pspio_state_t *state1, const pspio_state_t *state2) {

  if ((pspio_qn_cmp(state1->qn, state2->qn) == PSPIO_DIFF) ||
      (fabs(state1->occ - state2->occ) > 1e-12) ||
      (fabs(state1->eigenval - state2->eigenval) > 1e-12) ||
      strcmp(state1->label, state2->label) ||
      (fabs(state1->rc - state2->rc) > 1e-12) ||
      (pspio_meshfunc_cmp(state1->wf, state2->wf) == PSPIO_DIFF)) {
    return PSPIO_DIFF;
  } else {
    return PSPIO_EQUAL;
  }
}

double pspio_state_wf_eval(const pspio_state_t *state, double r)
{
  assert(state != NULL);
  
  return pspio_meshfunc_eval(state->wf, r);
}

double pspio_state_wf_eval_deriv(const pspio_state_t *state, double r)
{
  assert(state != NULL);
  
  return pspio_meshfunc_eval_deriv(state->wf, r);
}

double pspio_state_wf_eval_deriv2(const pspio_state_t *state, double r)
{
  assert(state != NULL);
  
  return pspio_meshfunc_eval_deriv2(state->wf, r);
}
