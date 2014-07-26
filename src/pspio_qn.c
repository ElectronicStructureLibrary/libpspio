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
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include "pspio_error.h"
#include "pspio_qn.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

int pspio_qn_alloc(pspio_qn_t **qn) {
  assert( *qn == NULL);

  *qn = (pspio_qn_t *) malloc (sizeof(pspio_qn_t));
  FULFILL_OR_EXIT(*qn != NULL, PSPIO_ENOMEM);

  (*qn)->n = 0;
  (*qn)->l = 0;
  (*qn)->j = 0.0;

  return PSPIO_SUCCESS;
}


int pspio_qn_copy(pspio_qn_t **dst, const pspio_qn_t *src) {
  assert(src != NULL);

  if ( *dst == NULL ) {
    SUCCEED_OR_RETURN( pspio_qn_alloc(dst) );
  }

  (*dst)->n = src->n;
  (*dst)->l = src->l;
  (*dst)->j = src->j;

  return PSPIO_SUCCESS;
}


void pspio_qn_free(pspio_qn_t **qn) {

  if ( qn != NULL ) {
    free(*qn);
    *qn = NULL;
  }
}


void pspio_qn_get(const pspio_qn_t *qn, int *n, int *l, double *j) {
  assert(qn != NULL);

  *n = qn->n;
  *l = qn->l;
  *j = qn->j;
}


int pspio_qn_set(pspio_qn_t **qn, const int n, const int l, const double j) {
  assert((qn != NULL) && (*qn != NULL));

  FULFILL_OR_RETURN( l >= 0, PSPIO_EVALUE );
  FULFILL_OR_RETURN( (j == 0.0) || (fabs(j - (double)l) - 0.5 < 1.0e-8), PSPIO_EVALUE);

  (*qn)->n = n;
  (*qn)->l = l;
  (*qn)->j = j;

  return PSPIO_SUCCESS;
}


/**********************************************************************
 * Atomic routines                                                    *
 **********************************************************************/

int pspio_qn_get_n(const pspio_qn_t *qn) {
  assert(qn != NULL);

  return qn->n;
}

int pspio_qn_get_l(const pspio_qn_t *qn) {
  assert(qn != NULL);

  return qn->l;
}

double pspio_qn_get_j(const pspio_qn_t *qn) {
  assert(qn != NULL);

  return qn->j;
}


/**********************************************************************
 * Utility routines                                                   *
 **********************************************************************/

int pspio_qn_cmp(const pspio_qn_t *qn1, const pspio_qn_t *qn2) {
  assert((qn1 != NULL) && (qn2 != NULL));

  if ( (qn1->n == qn2->n) && (qn1->l == qn2->l) &&
       (fabs(qn2->j - qn1->j) < 1.0e-9) ) {
    return PSPIO_QN_EQUAL;
  } else if ( (qn1->l == qn2->l) && (fabs(qn2->j - qn1->j) < 1.0e-9) ) {
    return PSPIO_QN_MTEQUAL;
  } else {
    return PSPIO_QN_DIFF;
  }
}

void pspio_qn_label(const pspio_qn_t *qn, char *s) {
  char llabel[4] = "spdf";

  assert(qn != NULL);

  if ( qn->j == 0.0 ) {
    sprintf(s, "%1u%1c", qn->n, llabel[qn->l]);
  } else {
    sprintf(s, "%1d%1c%3.1f", qn->n, llabel[qn->l], qn->j);
  }
}
