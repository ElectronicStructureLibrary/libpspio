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

#include <stdlib.h>
#include <stdio.h>
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
  ASSERT( *qn == NULL, PSPIO_ERROR);

  *qn = (pspio_qn_t *)malloc(sizeof(pspio_qn_t));
  HANDLE_FATAL_ERROR(*qn == NULL, PSPIO_ENOMEM);

  (*qn)->n = 0;
  (*qn)->l = 0;
  (*qn)->j = 0.0;

  return PSPIO_SUCCESS;
}


int pspio_qn_copy(pspio_qn_t **dst, const pspio_qn_t *src) {
  ASSERT(src != NULL, PSPIO_ERROR);

  if ( *dst == NULL ) {
    HANDLE_FUNC_ERROR(pspio_qn_alloc(dst));
  }

  (*dst)->n = src->n;
  (*dst)->l = src->l;
  (*dst)->j = src->j;

  return PSPIO_SUCCESS;
}


int pspio_qn_free(pspio_qn_t **qn) {
  if ( qn != NULL ) {
    free(*qn);
    *qn = NULL;
  }

  return PSPIO_SUCCESS;
}


int pspio_qn_get(const pspio_qn_t *qn, int *n, int *l, double *j) {
  ASSERT(qn != NULL, PSPIO_EVALUE);

  *n = qn->n;
  *l = qn->l;
  *j = qn->j;

  return PSPIO_SUCCESS;
}


int pspio_qn_set(pspio_qn_t **qn, const int n, const int l, const double j) {
  ASSERT((qn != NULL) && (*qn != NULL), PSPIO_ERROR);
  ASSERT( (n > 0) && (l >= 0), PSPIO_EVALUE);
  ASSERT( (j == 0.0) || (fabs(j - (double)l) - 0.5 < 1.0e-8), PSPIO_EVALUE);

  (*qn)->n = n;
  (*qn)->l = l;
  (*qn)->j = j;

  return PSPIO_SUCCESS;
}


/**********************************************************************
 * Utility routines                                                   *
 **********************************************************************/

int pspio_qn_cmp(const pspio_qn_t *qn1, const pspio_qn_t *qn2) {
  ASSERT((qn1 != NULL) && (qn2 != NULL), PSPIO_ERROR);

  if ( (qn1->n == qn2->n) && (qn1->l == qn2->l) &&
       (fabs(qn2->j - qn1->j) < 1.0e-9) ) {
    return PSPIO_QN_EQUAL;
  } else if ( (qn1->l == qn2->l) && (fabs(qn2->j - qn1->j) < 1.0e-9) ) {
    return PSPIO_QN_MTEQUAL;
  } else {
    return PSPIO_QN_DIFF;
  }
}
