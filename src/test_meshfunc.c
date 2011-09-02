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

/**
 * @file test_meshfunc.c
 * @brief checks pspio_meshfunc.c and pspio_meshfunc.h 
 */

#include <stdlib.h>
#include <stdio.h>

#include "pspio_error.h"
#include "pspio_meshfunc.h"
#include "test_common.h"


int main(void) {
  const double r[] = {0.0, 0.05, 0.10, 0.20, 0.40, 0.65, 0.85, 1.00};
  const double rab[] = {0.05, 0.05, 0.20, 0.20, 0.20, 0.20, 0.05, 0.05};
  const double a = 1.0;
  const double b = 2.0;
  const int np = sizeof(r) / sizeof(double);

  int eid = PSPIO_SUCCESS;
  pspio_mesh_t *m1 = NULL, *m2 = NULL;
  pspio_meshfunc_t *f1 = NULL, *f2 = NULL;

  /* Display basic information */
  DEBUG_PRINT("%s - test_meshfunc\nReport bugs to %s\n\n", PACKAGE_STRING,
    PACKAGE_BUGREPORT);
  DEBUG_PRINT("=== BEGIN test_meshfunc ===\n\n");

  /* Check creation and destruction of meshes */
  DEBUG_PRINT("test_mesh: creating m1\n");
  eid = pspio_mesh_alloc(&m1, np);
  eid = pspio_error_flush();
  DEBUG_PRINT("test_mesh: setting m1\n");
  eid = pspio_mesh_set(&m1, PSPIO_MESH_LOG1, a, b, r, rab);
  eid = pspio_error_flush();
  DEBUG_PRINT("test_mesh: creating m2\n");
  eid = pspio_mesh_alloc(&m2, np);
  eid = pspio_error_flush();
  DEBUG_PRINT("test_mesh: setting m2\n");
  eid = pspio_mesh_set(&m2, PSPIO_MESH_LINEAR, a, b, r, rab);
  eid = pspio_error_flush();
  DEBUG_PRINT("\n");

  /* Check creation and destruction of mesh functions */
  DEBUG_PRINT("test_meshfunc: creating f1\n");
  eid = pspio_meshfunc_alloc(&f1, np);
  eid = pspio_error_flush();
  DEBUG_PRINT("test_meshfunc: creating f2\n");
  eid = pspio_meshfunc_alloc(&f2, np);
  eid = pspio_error_flush();
  DEBUG_PRINT("test_meshfunc: destroying f2\n");
  eid = pspio_meshfunc_free(&f2);
  eid = pspio_error_flush();
  DEBUG_PRINT("\n");

  /* Check setting of mesh functions */
  DEBUG_PRINT("test_meshfunc: setting f1\n");
  eid = pspio_meshfunc_set(&f1, m1, r);
  eid = pspio_error_flush();
  DEBUG_PRINT("\n");

  /* Check copy of mesh functions */
  DEBUG_PRINT("test_meshfunc: copying f1 to a NULL f2\n");
  eid = pspio_meshfunc_copy(&f2, f1);
  eid = pspio_error_flush();
  DEBUG_PRINT("test_meshfunc: copying f1 to a non-empty f2\n");
  eid = pspio_meshfunc_copy(&f2, f1);
  eid = pspio_error_flush();
  DEBUG_PRINT("test_meshfunc: setting f2\n");
  eid = pspio_meshfunc_set(&f2, &m2, rab);
  eid = pspio_error_flush();
  DEBUG_PRINT("\n");

  /* Destroy mesh functions */
  DEBUG_PRINT("test_meshfunc: destroying f1\n");
  PTR_STAT_SHOW(f1);
  eid = pspio_meshfunc_free(&f1);
  eid = pspio_error_flush();
  DEBUG_PRINT("test_meshfunc: destroying f2\n");
  PTR_STAT_SHOW(f2);
  eid = pspio_meshfunc_free(&f2);
  eid = pspio_error_flush();
  DEBUG_PRINT("\n");

  DEBUG_PRINT("=== END test_meshfunc ===\n");

  return 0;
}
