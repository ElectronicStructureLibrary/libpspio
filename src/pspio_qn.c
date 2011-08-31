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

#include "pspio_error.h"
#include "pspio_qn.h"


int pspio_qn_alloc(pspio_qn_t *qn) {
  ASSERT( qn == NULL, PSPIO_ERROR)

  qn = (pspio_qn_t *)malloc(sizeof(qn));
  HANDLE_FATAL_ERROR (qn == NULL, PSPIO_ENOMEM);

  qn->n = 0;
  qn->l = 0;
  qn->j = 0.0;

  return PSPIO_SUCCESS;
}


int pspio_qn_set(pspio_qn_t *qn, const int n, const int l, const double j) {

  ASSERT((n > 0) && (l > 0) && (abs(j - (double)l) - 0.5 > 1.0e-9),
    PSPIO_EVALUE)

  qn->n = n;
  qn->l = l;
  qn->j = j;

  return PSPIO_SUCCESS;
}


int pspio_qn_copy(pspio_qn_t *dst, pspio_qn_t *src) {
  ASSERT(src != NULL, PSPIO_ERROR)

  if ( dst == NULL ) {
    HANDLE_FUNC_ERROR (pspio_qn_alloc(dst));
  }

  dst->n = src->n;
  dst->l = src->l;
  dst->j = src->j;

  return PSPIO_SUCCESS;
}


int pspio_qn_free(pspio_qn_t *qn) {
  if ( qn != NULL ) {
    free(qn);
  }

  return PSPIO_SUCCESS;
}

