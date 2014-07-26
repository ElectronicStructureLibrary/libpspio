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
 * @file test_potential.c
 * @brief checks pspio_potential.c and pspio_potential.h 
 */

#include <stdlib.h>
#include <stdio.h>

#include "pspio_error.h"
#include "pspio_potential.h"
#include "test_common.h"


int main(void) {
  const double r[] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8};
  const double v[] = {1.0, 0.95, 0.90, 0.80, 0.50, 0.20, 0.10, 0.05, 0.00};
  const int np = sizeof(r) / sizeof(double);

  double vr;
  pspio_mesh_t *mesh = NULL;
  pspio_qn_t *qn = NULL;
  pspio_potential_t *pot1 = NULL, *pot2 = NULL;

  /* Display basic information */
  DEBUG_PRINT("%s - test_potential\nReport bugs to %s\n\n", PACKAGE_STRING,
    PACKAGE_BUGREPORT);
  DEBUG_PRINT("=== BEGIN test_potential ===\n\n");

  /* Check creation and setting of mesh */
  DEBUG_PRINT("test_potential: creating mesh\n");
  CHECK_STAT(pspio_mesh_alloc(&mesh, np), PSPIO_SUCCESS);
  DEBUG_PRINT("test_potential: setting mesh\n");
  pspio_mesh_init_from_points(&mesh, r, NULL);
  DEBUG_PRINT("\n");

  /* Check creation and setting of quantum numbers */
  DEBUG_PRINT("test_potential: creating quantum numbers\n");
  CHECK_STAT(pspio_qn_alloc(&qn), PSPIO_SUCCESS);
  DEBUG_PRINT("test_potential: setting quantum numbers to (1, 2, 0.0)\n");
  CHECK_STAT(pspio_qn_set(&qn, 1, 2, 0.0), PSPIO_SUCCESS);
  DEBUG_PRINT("\n");

  /* Check creation and destruction of potential */
  DEBUG_PRINT("test_potential: creating pot1\n");
  CHECK_STAT(pspio_potential_alloc(&pot1, np), PSPIO_SUCCESS);
  DEBUG_PRINT("test_potential: creating pot2\n");
  CHECK_STAT(pspio_potential_alloc(&pot2, np), PSPIO_SUCCESS);
  DEBUG_PRINT("test_potential: destroying pot2\n");
  pspio_potential_free(&pot2);
  DEBUG_PRINT("\n");

  /* Check setting of the potentials */
  DEBUG_PRINT("test_potential: setting pot1\n");
  CHECK_STAT(pspio_potential_set(&pot1, qn, mesh, v), PSPIO_SUCCESS);
  DEBUG_PRINT("\n");

  /* Check evaluation of the potential */
  DEBUG_PRINT("test_potential: evaluating  pot1 at r=%f\n", r[6]);
  pspio_potential_eval(pot1, 1, &r[6], &vr);
  DEBUG_PRINT("test_potential: result=%f\n", vr);
  DEBUG_PRINT("\n");

  /* Destroy potential */
  DEBUG_PRINT("test_potential: destroying pot1\n");
  pspio_potential_free(&pot1);
  DEBUG_PRINT("\n");

  DEBUG_PRINT("=== END test_potential ===\n");

  return 0;
}
