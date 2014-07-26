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
 * @file test_xc.c
 * @brief checks pspio_xc.c and pspio_xc.h 
 */

#include <stdlib.h>
#include <stdio.h>

#include "pspio_error.h"
#include "pspio_xc.h"
#include "test_common.h"

int main(void) {
  const double r[]  = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8};
  const double cd[] = {1.0, 0.95, 0.90, 0.80, 0.50, 0.20, 0.10, 0.05, 0.00};
  const int np = sizeof(r) / sizeof(double);

  pspio_mesh_t *mesh = NULL;
  pspio_xc_t *xc = NULL;

  /* Display basic information */
  DEBUG_PRINT("%s - test_xc\nReport bugs to %s\n\n", PACKAGE_STRING,
    PACKAGE_BUGREPORT);
  DEBUG_PRINT("=== BEGIN test_xc ===\n\n");

  /* Check creation and setting of mesh */
  DEBUG_PRINT("test_xc: creating mesh\n");
  CHECK_STAT(pspio_mesh_alloc(&mesh, np), PSPIO_SUCCESS);
  DEBUG_PRINT("test_xc: setting mesh\n");
  pspio_mesh_init_from_points(&mesh, r, NULL);
  DEBUG_PRINT("\n");

  /* Check creation and destruction of xc */
  DEBUG_PRINT("test_xc: creating xc\n");
  CHECK_STAT(pspio_xc_alloc(&xc), PSPIO_SUCCESS);
  DEBUG_PRINT("\n");

  /* Check setting of the xc */
  DEBUG_PRINT("test_xc: setting xc with PW92 LDA\n");
  pspio_xc_set_id(&xc, XC_LDA_X, XC_LDA_C_PW);
  DEBUG_PRINT("test_xc: setting of NLCC scheme of xc\n");
  CHECK_STAT(pspio_xc_set_nlcc_scheme(&xc, PSPIO_NLCC_UNKNOWN), PSPIO_SUCCESS);
  DEBUG_PRINT("test_xc: setting of core density of xc\n");
  CHECK_STAT(pspio_xc_set_nlcc_density(&xc, mesh, cd, NULL, NULL), PSPIO_SUCCESS);
  DEBUG_PRINT("\n");

  /* Destroy xc */
  DEBUG_PRINT("test_xc: destroying xc\n");
  pspio_xc_free(&xc);
  DEBUG_PRINT("\n");

  DEBUG_PRINT("=== END test_xc ===\n");

  return 0;
}
