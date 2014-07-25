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
 * @file test_state.c
 * @brief checks pspio_state.c and pspio_state.h 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pspio_error.h"
#include "pspio_mesh.h"
#include "pspio_state.h"
#include "test_common.h"


int main(void) {
  const double wf[] = {1.0, 0.95, 0.90, 0.80, 0.50, 0.20, 0.10, 0.05, 0.00};
  const int np = sizeof(wf) / sizeof(double);

  int i, j, lsize;
  int **table = NULL;
  pspio_mesh_t *mesh = NULL;
  pspio_qn_t *qn1 = NULL, *qn2 = NULL, *qn3 = NULL;
  pspio_state_t *s1 = NULL, *s2 = NULL, **st;

  /* Display basic information */
  DEBUG_PRINT("%s - test_state\nReport bugs to %s\n\n", PACKAGE_STRING,
    PACKAGE_BUGREPORT);
  DEBUG_PRINT("=== BEGIN test_state ===\n\n");

  /* Check creation and setting of mesh */
  DEBUG_PRINT("test_state: creating mesh\n");
  CHECK_STAT(pspio_mesh_alloc(&mesh, np), PSPIO_SUCCESS);
  DEBUG_PRINT("test_state: setting mesh\n");
  pspio_mesh_init_from_parameters(&mesh, PSPIO_MESH_LINEAR, 0.1, 0.05);
  DEBUG_PRINT("\n");

  /* Check creation and setting of quantum numbers */
  DEBUG_PRINT("test_state: creating qn1\n");
  CHECK_STAT(pspio_qn_alloc(&qn1), PSPIO_SUCCESS);
  DEBUG_PRINT("test_state: creating qn2\n");
  CHECK_STAT(pspio_qn_alloc(&qn2), PSPIO_SUCCESS);
  DEBUG_PRINT("test_state: creating qn3\n");
  CHECK_STAT(pspio_qn_alloc(&qn3), PSPIO_SUCCESS);
  DEBUG_PRINT("test_state: setting qn1 to (2, 0, 0.5)\n");
  CHECK_STAT(pspio_qn_set(&qn1, 2, 0, 0.5), PSPIO_SUCCESS);
  DEBUG_PRINT("test_state: setting qn2 to (3, 1, 0.5)\n");
  CHECK_STAT(pspio_qn_set(&qn2, 3, 1, 0.5), PSPIO_SUCCESS);
  DEBUG_PRINT("test_state: setting qn3 to (3, 1, 1.5)\n");
  CHECK_STAT(pspio_qn_set(&qn3, 3, 1, 1.5), PSPIO_SUCCESS);
  DEBUG_PRINT("\n");

  /* Check creation and destruction of states */
  DEBUG_PRINT("test_state: creating s1\n");
  CHECK_STAT(pspio_state_alloc(&s1, np), PSPIO_SUCCESS);
  DEBUG_PRINT("test_state: creating s2\n");
  CHECK_STAT(pspio_state_alloc(&s2, np), PSPIO_SUCCESS);
  DEBUG_PRINT("test_state: destroying s2\n");
  pspio_state_free(&s2);
  DEBUG_PRINT("\n");

  /* Check setting of states */
  DEBUG_PRINT("test_state: setting s1 to (-1.0, '2s', qn1, 2.0, 1.0, ...)\n");
  CHECK_STAT(pspio_state_set(&s1, -1.0, "1s2", qn1, 2.0, 1.0, mesh, wf), PSPIO_SUCCESS);
  DEBUG_PRINT("\n");

  /* Check creation of lookup table */
  DEBUG_PRINT("test_state: creating st[3]\n");
  st = (pspio_state_t **) malloc (3 * sizeof(pspio_state_t *));
  memset(st, 0, 3 * sizeof(pspio_state_t *));
  PTR_STAT_SHOW(st);
  DEBUG_PRINT("test_state: creating st[0]\n");
  CHECK_STAT(pspio_state_alloc(&st[0], np), PSPIO_SUCCESS);
  DEBUG_PRINT("test_state: creating st[1]\n");
  CHECK_STAT(pspio_state_alloc(&st[1], np), PSPIO_SUCCESS);
  DEBUG_PRINT("test_state: creating st[2]\n");
  CHECK_STAT(pspio_state_alloc(&st[2], np), PSPIO_SUCCESS);
  DEBUG_PRINT("test_state: setting st[0] to (-1.00, '2s0.5', qn1, 2.0, 1.0, ...)\n");
  CHECK_STAT(pspio_state_set(&st[0], -1.0, "2s0.5", qn1, 2.0, 1.0, mesh, wf), PSPIO_SUCCESS);
  DEBUG_PRINT("test_state: setting st[1] to (-0.50, '3p0.5', qn2, 2.0, 1.0, ...)\n");
  CHECK_STAT(pspio_state_set(&st[1], -1.0, "2s0.5", qn2, 2.0, 1.0, mesh, wf), PSPIO_SUCCESS);
  DEBUG_PRINT("test_state: setting st[2] to (-0.25, '3p1.5', qn3, 4.0, 1.0, ...)\n");
  CHECK_STAT(pspio_state_set(&st[2], -1.0, "2s0.5", qn3, 4.0, 1.0, mesh, wf), PSPIO_SUCCESS);
  DEBUG_PRINT("test_state: Creating lookup table\n");
  CHECK_STAT(pspio_states_lookup_table(3, st, &table), PSPIO_SUCCESS);
  lsize = 3;
  for (i=0; table[i]!=NULL; i++) {
    for (j=0; j<lsize ; j++) {
      DEBUG_PRINT("test_state:   table[%d][%d] = %d \n", i, j, table[i][j]);
    }
  }
  DEBUG_PRINT("\n");

  /* Destroy states */
  DEBUG_PRINT("test_state: destroying s1\n");
  pspio_state_free(&s1);
  for (i=0; i<3; i++) {
    DEBUG_PRINT("test_state: destroying st[%d]\n", i);
    pspio_state_free(&st[i]);
  }
  DEBUG_PRINT("\n");

  DEBUG_PRINT("=== END test_state ===\n");

  return 0;
}
