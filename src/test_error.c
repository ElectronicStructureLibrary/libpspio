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

*/

/**
 * @file test_error.c
 * @brief checks pspio_error.c and pspio_error.h 
 */

#include <stdlib.h>
#include <stdio.h>

#include "pspio_error.h"
#include "test_common.h"


int test_assert(const int i) {
  ASSERT(i == 0, PSPIO_EVALUE);
  return PSPIO_SUCCESS;
}


int main(void) {
  int eid = pspio_error_get_last();

  /* Display basic information */
  DEBUG_PRINT("%s - test_error\nReport bugs to %s\n\n", PACKAGE_STRING,
    PACKAGE_BUGREPORT);
  DEBUG_PRINT("=== BEGIN test_error ===\n\n");

  /* check empty error chain */
  DEBUG_PRINT("test_error: checking empty error chain\n");
  DEBUG_PRINT("test_error: at the beginning, status = %d, length = %d\n",
    eid, pspio_error_len());
  CHECK_STAT_LEN(eid, PSPIO_SUCCESS, pspio_error_len(), 0);
  DEBUG_PRINT("test_error: BEGIN FLUSH\n");
  eid = pspio_error_flush();
  DEBUG_PRINT("test_error: END FLUSH\n");
  DEBUG_PRINT("test_error: after pspio_error_flush, status = %d, length = %d\n",
    eid, pspio_error_len());
  CHECK_STAT_LEN(eid, PSPIO_SUCCESS, pspio_error_len(), 0);
  DEBUG_PRINT("\n");

  /* Check single error */
  DEBUG_PRINT("test_error: checking single error (EVALUE)\n");
  pspio_error_add(PSPIO_EVALUE, "test_1_1.c", 1234);
  DEBUG_PRINT("test_error: after pspio_error_add, status = %d, length = %d\n",
    eid, pspio_error_len());
  CHECK_STAT_LEN(eid, PSPIO_SUCCESS, pspio_error_len(), 1);
  DEBUG_PRINT("test_error: BEGIN FLUSH\n");
  pspio_error_flush();
  DEBUG_PRINT("test_error: END FLUSH\n");
  DEBUG_PRINT("test_error: after pspio_error_flush, status = %d, length = %d\n",
    eid, pspio_error_len());
  CHECK_STAT_LEN(eid, PSPIO_SUCCESS, pspio_error_len(), 0);
  DEBUG_PRINT("\n");

  /* Check double error */
  DEBUG_PRINT("test_error: checking double error (EGSL, ENOSUPPORT)\n");
  pspio_error_add(PSPIO_EGSL, "test_2_1.c", 201);
  DEBUG_PRINT("test_error: after pspio_error_add, status = %d, length = %d\n",
    eid, pspio_error_len());
  CHECK_STAT_LEN(eid, PSPIO_SUCCESS, pspio_error_len(), 1);
  pspio_error_add(PSPIO_ENOSUPPORT, "test_2_2.c", 202);
  DEBUG_PRINT("test_error: after pspio_error_add, status = %d, length = %d\n",
    eid, pspio_error_len());
  CHECK_STAT_LEN(eid, PSPIO_SUCCESS, pspio_error_len(), 2);
  DEBUG_PRINT("test_error: BEGIN FLUSH\n");
  pspio_error_flush();
  DEBUG_PRINT("test_error: END FLUSH\n");
  DEBUG_PRINT("test_error: after pspio_error_flush, status = %d, length = %d\n",
    eid, pspio_error_len());
  CHECK_STAT_LEN(eid, PSPIO_SUCCESS, pspio_error_len(), 0);
  DEBUG_PRINT("\n");

  /* Check triple error */
  DEBUG_PRINT("test_error: checking triple error (EVALUE, ENOFILE, ERROR)\n");
  pspio_error_add(PSPIO_EVALUE, "test_3_1.c", 311);
  DEBUG_PRINT("test_error: after pspio_error_add, status = %d, length = %d\n",
    eid, pspio_error_len());
  CHECK_STAT_LEN(eid, PSPIO_SUCCESS, pspio_error_len(), 1);
  pspio_error_add(PSPIO_ENOFILE, "test_3_2.c", 322);
  DEBUG_PRINT("test_error: after pspio_error_add, status = %d, length = %d\n",
    eid, pspio_error_len());
  CHECK_STAT_LEN(eid, PSPIO_SUCCESS, pspio_error_len(), 2);
  pspio_error_add(PSPIO_ERROR, "test_3_3.c", 333);
  DEBUG_PRINT("test_error: after pspio_error_add, status = %d, length = %d\n",
    eid, pspio_error_len());
  CHECK_STAT_LEN(eid, PSPIO_SUCCESS, pspio_error_len(), 3);
  DEBUG_PRINT("test_error: BEGIN FLUSH\n");
  pspio_error_flush();
  DEBUG_PRINT("test_error: END FLUSH\n");
  DEBUG_PRINT("test_error: after pspio_error_flush, status = %d, length = %d\n",
    eid, pspio_error_len());
  CHECK_STAT_LEN(eid, PSPIO_SUCCESS, pspio_error_len(), 0);
  DEBUG_PRINT("\n");

  /* Check ASSERT macro */
  DEBUG_PRINT("test_error: checking successful ASSERT macro (0==0)\n");
  eid = test_assert(0);
  DEBUG_PRINT("test_error: BEGIN FLUSH\n");
  eid = pspio_error_flush();
  DEBUG_PRINT("test_error: END FLUSH\n");
  CHECK_STAT_LEN(eid, PSPIO_SUCCESS, pspio_error_len(), 0);
  DEBUG_PRINT("test_error: after ASSERT, status = %d, length = %d\n",
    eid, pspio_error_len());
  DEBUG_PRINT("\n");

  /* Destroy error-handling structures */
  DEBUG_PRINT("test_error: destroying error-handling structures\n");
  eid = pspio_error_free();
  CHECK_STAT_LEN(eid, PSPIO_SUCCESS, pspio_error_len(), 0);
  DEBUG_PRINT("\n");

  DEBUG_PRINT("=== END test_error ===\n");

  return 0;
}
