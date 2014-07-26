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
 * @file test_projector.c
 * @brief checks pspio_projector.c and pspio_projector.h 
 */

#include <stdlib.h>
#include <stdio.h>

#include "pspio_error.h"
#include "pspio_projector.h"
#include "test_common.h"


int main(void) {
  const double r[] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8};
  const double p[] = {1.0, 0.95, 0.90, 0.80, 0.50, 0.20, 0.10, 0.05, 0.00};
  const int np = sizeof(r) / sizeof(double);

  int l;
  double pr, e, j;
  pspio_mesh_t *mesh = NULL;
  pspio_qn_t *qn = NULL;
  pspio_projector_t *proj1 = NULL, *proj2 = NULL;

  /* Display basic information */
  DEBUG_PRINT("%s - test_projector\nReport bugs to %s\n\n", PACKAGE_STRING,
    PACKAGE_BUGREPORT);
  DEBUG_PRINT("=== BEGIN test_projector ===\n\n");

  /* Check creation and setting of mesh */
  DEBUG_PRINT("test_projector: creating mesh\n");
  CHECK_STAT(pspio_mesh_alloc(&mesh, np), PSPIO_SUCCESS);
  DEBUG_PRINT("test_projector: setting mesh\n");
  pspio_mesh_init_from_points(&mesh, r, NULL);
  DEBUG_PRINT("\n");

  /* Check creation and setting of quantum numbers */
  DEBUG_PRINT("test_projector: creating quantum numbers\n");
  CHECK_STAT(pspio_qn_alloc(&qn), PSPIO_SUCCESS);
  DEBUG_PRINT("test_projector: setting quantum numbers to (1, 2, 0.0)\n");
  CHECK_STAT(pspio_qn_set(&qn, 1, 2, 0.0), PSPIO_SUCCESS);
  DEBUG_PRINT("\n");

  /* Check creation and destruction of projector */
  DEBUG_PRINT("test_projector: creating proj1\n");
  CHECK_STAT(pspio_projector_alloc(&proj1, np), PSPIO_SUCCESS);
  DEBUG_PRINT("test_projector: creating proj2\n");
  CHECK_STAT(pspio_projector_alloc(&proj2, np), PSPIO_SUCCESS);
  DEBUG_PRINT("test_projector: destroying proj2\n");
  pspio_projector_free(&proj2);
  DEBUG_PRINT("\n");

  /* Check setting of the projectors */
  DEBUG_PRINT("test_projector: setting proj1\n");
  CHECK_STAT(pspio_projector_set(&proj1, qn, 2.0, mesh, p), PSPIO_SUCCESS);
  DEBUG_PRINT("\n");

  /* Check evaluation of the projector */
  DEBUG_PRINT("test_projector: evaluating proj1 at r=%f\n", r[6]);
  pspio_projector_eval(proj1, 1, &r[6], &pr);
  DEBUG_PRINT("test_projector: result=%f\n", pr);
  DEBUG_PRINT("test_projector: evaluating energy of proj1 (should be 2.0)\n");
  e = pspio_projector_get_energy(proj1);
  DEBUG_PRINT("test_projector: result=%f\n", e);
  DEBUG_PRINT("test_projector: evaluating angular momentum of proj1 (should be 2)\n");
  l = pspio_projector_get_l(proj1);
  DEBUG_PRINT("test_projector: result=%d\n", l);
  DEBUG_PRINT("test_projector: evaluating angular momentum of proj1 (should be 0.0)\n");
  j = pspio_projector_get_j(proj1);
  DEBUG_PRINT("test_projector: result=%3.1f\n", j);
  DEBUG_PRINT("\n");

  /* Destroy projector */
  DEBUG_PRINT("test_projector: destroying proj1\n");
  pspio_projector_free(&proj1);
  DEBUG_PRINT("\n");

  DEBUG_PRINT("=== END test_projector ===\n");

  return 0;
}
