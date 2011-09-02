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
 * @file test_state.c
 * @brief checks pspio_state.c and pspio_state.h 
 */

#include <stdlib.h>
#include <stdio.h>

#include "pspio_error.h"
#include "pspio_mesh.h"
#include "pspio_state.h"
#include "test_common.h"


int main(void) {
  const double wf1[] = {1.0, 0.95, 0.90, 0.80, 0.50, 0.20, 0.10, 0.05, 0.00};
  const double wf2[] = {0.0, 0.80, 0.60, 0.55, 0.50, 0.55, 0.60, 0.80, 0.00};

  int eid = PSPIO_SUCCESS, i, np;
  pspio_mesh_t *mesh;
  pspio_state_t *s1 = NULL, *s2 = NULL, **st;

  /* Display basic information */
  DEBUG_PRINT("%s - test_state\nReport bugs to %s\n\n", PACKAGE_STRING,
    PACKAGE_BUGREPORT);
  DEBUG_PRINT("=== BEGIN test_state ===\n\n");

  /* Init dependent variables */
  DEBUG_PRINT("test_state: creating mesh\n");
  np = sizeof(wf1) / sizeof(double);
  eid = pspio_mesh_alloc(&mesh, np);
  eid = pspio_error_flush();

  /* Check creation and destruction of states */
  DEBUG_PRINT("test_state: creating s1\n");
  eid = pspio_state_alloc(&s1, np);
  eid = pspio_error_flush();
  DEBUG_PRINT("test_state: creating s2\n");
  eid = pspio_state_alloc(&s2, np);
  eid = pspio_error_flush();
  DEBUG_PRINT("test_state: destroying s2\n");
  eid = pspio_state_free(&s2);
  eid = pspio_error_flush();
  DEBUG_PRINT("\n");

  /* Check setting of states */
  DEBUG_PRINT("test_state: setting s1 to (-1.0, '1s2', 2.0, 1.0, ...)\n");
  eid = pspio_state_set(&s1, -1.0, "1s2", 2.0, 1.0, mesh, wf1);
  eid = pspio_error_flush();
  DEBUG_PRINT("\n");

  /* Check copy of states */
  DEBUG_PRINT("test_state: copying s1 to a NULL s2\n");
  eid = pspio_state_copy(&s2, s1);
  eid = pspio_error_flush();
  DEBUG_PRINT("test_state: copying s1 to a non-empty s2\n");
  eid = pspio_state_copy(&s2, s1);
  eid = pspio_error_flush();
  DEBUG_PRINT("test_state: setting s2 to (3.0, '2s1', 1.0, 2.0, ...)\n");
  eid = pspio_state_set(&s1, 3.0, "2s1", 1.0, 2.0, mesh, wf2);
  eid = pspio_error_flush();
  DEBUG_PRINT("test_state: creating st[2]\n");
  st = (pspio_state_t **)malloc(2 * sizeof(pspio_state_t *));
  PTR_STAT_SHOW(st)
  DEBUG_PRINT("test_state: copying s1 and s2 into st\n");
  eid = pspio_state_copy(&st[0], s1);
  eid = pspio_error_flush();
  eid = pspio_state_copy(&st[1], s2);
  eid = pspio_error_flush();
  DEBUG_PRINT("\n");

  /* Destroy states */
  DEBUG_PRINT("test_state: destroying s1\n");
  eid = pspio_state_free(&s1);
  eid = pspio_error_flush();
  DEBUG_PRINT("test_state: destroying s2\n");
  eid = pspio_state_free(&s2);
  eid = pspio_error_flush();
  DEBUG_PRINT("test_state: destroying st\n");
  for (i=0; i<2; i++) {
    eid = pspio_state_free(&st[i]);
    eid = pspio_error_flush();
  }
  free(st);
  DEBUG_PRINT("\n");

  DEBUG_PRINT("=== END test_state ===\n");

  return 0;
}
