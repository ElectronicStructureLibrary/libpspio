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
 * @file test_mesh.c
 * @brief checks pspio_mesh.c and pspio_mesh.h 
 */

#include <stdlib.h>
#include <stdio.h>

#include "pspio_error.h"
#include "pspio_mesh.h"
#include "test_common.h"


int main(void) {
  const double r[] = {0.0, 0.05, 0.10, 0.20, 0.40, 0.65, 0.85, 1.00};
  const double rab[] = {0.05, 0.05, 0.20, 0.20, 0.20, 0.20, 0.05, 0.05};
  const double a = 1.0;
  const double b = 2.0;
  const int np = sizeof(r) / sizeof(double);

  pspio_mesh_t *m1 = NULL, *m2 = NULL;

  /* Display basic information */
  DEBUG_PRINT("%s - test_mesh\nReport bugs to %s\n\n", PACKAGE_STRING,
    PACKAGE_BUGREPORT);
  DEBUG_PRINT("=== BEGIN test_mesh ===\n\n");

  /* Check creation and destruction of meshes */
  DEBUG_PRINT("test_mesh: creating m1\n");
  CHECK_STAT(pspio_mesh_alloc(&m1, np), PSPIO_SUCCESS);
  DEBUG_PRINT("test_mesh: creating m2\n");
  CHECK_STAT(pspio_mesh_alloc(&m2, np), PSPIO_SUCCESS);
  DEBUG_PRINT("test_mesh: destroying m2\n");
  pspio_mesh_free(&m2);
  DEBUG_PRINT("\n");

  /* Check setting of meshes */
  DEBUG_PRINT("test_mesh: setting m1\n");
  CHECK_STAT(pspio_mesh_set(&m1, PSPIO_MESH_LOG1, a, b, r, rab), PSPIO_SUCCESS);
  DEBUG_PRINT("test_mesh: initializing m1 from parameters\n");
  pspio_mesh_init_from_parameters(&m1, PSPIO_MESH_LOG1, a, b);
  DEBUG_PRINT("test_mesh: initializing m1 from points\n");
  pspio_mesh_init_from_points(&m1, r, rab);
  DEBUG_PRINT("\n");

  /* Check copy of meshes */
  DEBUG_PRINT("test_mesh: copying m1 to a NULL m2\n");
  CHECK_STAT(pspio_mesh_copy(&m2, m1), PSPIO_SUCCESS);
  DEBUG_PRINT("test_mesh: copying m1 to a non-empty m2\n");
  CHECK_STAT(pspio_mesh_copy(&m2, m1), PSPIO_SUCCESS);
  DEBUG_PRINT("test_mesh: setting m2\n");
  CHECK_STAT(pspio_mesh_set(&m2, PSPIO_MESH_LINEAR, a, b, r, rab), PSPIO_SUCCESS);
  DEBUG_PRINT("\n");

  /* Destroy meshes */
  DEBUG_PRINT("test_mesh: destroying m1\n");
  PTR_STAT_SHOW(m1);
  pspio_mesh_free(&m1);
  DEBUG_PRINT("test_mesh: destroying m2\n");
  PTR_STAT_SHOW(m2);
  pspio_mesh_free(&m2);
  DEBUG_PRINT("\n");

  DEBUG_PRINT("=== END test_mesh ===\n");

  return 0;
}
