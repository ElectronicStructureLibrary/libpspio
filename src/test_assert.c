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
 * @file test_assert.c
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
  int eid;

  /* Display basic information */
  DEBUG_PRINT("%s - test_assert\nReport bugs to %s\n\n", PACKAGE_STRING,
    PACKAGE_BUGREPORT);
  DEBUG_PRINT("=== BEGIN test_assert ===\n\n");

  DEBUG_PRINT("test_assert: will now check fatal error\n");
  DEBUG_PRINT("test_assert: checking ASSERT macro (1==0)\n");
  eid = test_assert(1);
  eid = pspio_error_flush();

  DEBUG_PRINT("=== END test_assert ===\n");

  return 0;
}
