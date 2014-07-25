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

/**
 * @file test_qn.c
 * @brief checks pspio_qn.c and pspio_qn.h 
 */

#include <stdlib.h>
#include <stdio.h>

#include "pspio_error.h"
#include "pspio_qn.h"
#include "test_common.h"


int main(void) {
  pspio_qn_t *qn1 = NULL, *qn2 = NULL;
  char label[10];

  /* Display basic information */
  DEBUG_PRINT("%s - test_qn\nReport bugs to %s\n\n", PACKAGE_STRING,
    PACKAGE_BUGREPORT);
  DEBUG_PRINT("=== BEGIN test_qn ===\n\n");

  /* Check creation and destruction of quantum numbers */
  DEBUG_PRINT("test_qn: creating qn1\n");
  CHECK_STAT(pspio_qn_alloc(&qn1), PSPIO_SUCCESS);
  DEBUG_PRINT("test_qn: creating qn2\n");
  CHECK_STAT(pspio_qn_alloc(&qn2), PSPIO_SUCCESS);
  DEBUG_PRINT("test_qn: destroying qn2\n");
  pspio_qn_free(&qn2);
  DEBUG_PRINT("\n");

  /* Check setting of quantum numbers */
  DEBUG_PRINT("test_qn: setting qn1 to (1, 2, 3.0) = wrong\n");
  CHECK_STAT(pspio_qn_set(&qn1, 1, 2, 3.0), PSPIO_EVALUE);
  DEBUG_PRINT("test_qn: setting qn1 to (1, 2, 0.0) = correct\n");
  CHECK_STAT(pspio_qn_set(&qn1, 1, 2, 0.0), PSPIO_SUCCESS);
  DEBUG_PRINT("test_qn: setting qn1 to (1, 2, 1.5) = correct\n");
  CHECK_STAT(pspio_qn_set(&qn1, 1, 2, 1.5), PSPIO_SUCCESS);
  DEBUG_PRINT("test_qn: setting qn1 to (1, 2, 2.5) = correct\n");
  CHECK_STAT(pspio_qn_set(&qn1, 1, 2, 2.5), PSPIO_SUCCESS);
  DEBUG_PRINT("\n");

  /* Check copy of quantum numbers */
  DEBUG_PRINT("test_qn: copying qn1 to a NULL qn2\n");
  CHECK_STAT(pspio_qn_copy(&qn2, qn1), PSPIO_SUCCESS);
  DEBUG_PRINT("test_qn: copying qn1 to a non-empty qn2\n");
  CHECK_STAT(pspio_qn_copy(&qn2, qn1), PSPIO_SUCCESS);
  DEBUG_PRINT("\n");

  /* Check comparison of quantum numbers */
  DEBUG_PRINT("test_qn: comparing identical qn1 and qn2\n");
  CHECK_STAT(pspio_qn_cmp(qn2, qn1), PSPIO_QN_EQUAL);
  DEBUG_PRINT("test_qn: comparing qn1 and qn2 with identical momenta\n");
  CHECK_STAT(pspio_qn_set(&qn2, 2, 2, 2.5), PSPIO_SUCCESS);
  CHECK_STAT(pspio_qn_cmp(qn2, qn1), PSPIO_QN_MTEQUAL);
  DEBUG_PRINT("test_qn: comparing different qn1 and qn2\n");
  CHECK_STAT(pspio_qn_set(&qn2, 2, 3, 2.5), PSPIO_SUCCESS);
  CHECK_STAT(pspio_qn_cmp(qn1, qn2), PSPIO_QN_DIFF);
  CHECK_STAT(pspio_qn_set(&qn2, 1, 2, 1.5), PSPIO_SUCCESS);
  CHECK_STAT(pspio_qn_cmp(qn2, qn1), PSPIO_QN_DIFF);
  DEBUG_PRINT("\n");

  /* Check quantum numbers labels */
  DEBUG_PRINT("test_qn: printing quantum number label\n");
  CHECK_STAT(pspio_qn_set(&qn1, 1, 0, 0.0), PSPIO_SUCCESS);
  pspio_qn_label(qn1, label);
  DEBUG_PRINT("test_qn:  label 1 = '%s' (should be '1s')\n", label);
  CHECK_STAT(pspio_qn_set(&qn1, 2, 1, 1.5), PSPIO_SUCCESS);
  pspio_qn_label(qn1, label);
  DEBUG_PRINT("test_qn:  label 2 = '%s' (should be '2p1.5')\n", label);
  DEBUG_PRINT("\n");

  /* Destroy quantum numbers */
  DEBUG_PRINT("test_qn: destroying qn1\n");
  pspio_qn_free(&qn1);
  DEBUG_PRINT("test_qn: destroying qn2\n");
  pspio_qn_free(&qn2);
  DEBUG_PRINT("\n");

  DEBUG_PRINT("=== END test_qn ===\n");

  return 0;
}
