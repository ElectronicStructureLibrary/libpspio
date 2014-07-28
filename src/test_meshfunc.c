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
  const double f[] = {0.0, 1.0, 4.0, 16.0, 64.0, 169.0, 289.0, 400.0};
  const double fp[] = {0.0, 20.0, 40.0, 80.0, 160.0, 260.0, 340.0, 400.0};
  const double a = 1.0;
  const double b = 2.0;
  const int np = sizeof(r) / sizeof(double);

  double feval;
  pspio_mesh_t *m1 = NULL, *m2 = NULL;
  pspio_meshfunc_t *f1 = NULL, *f2 = NULL, *f3 = NULL;

  /* Display basic information */
  DEBUG_PRINT("%s - test_meshfunc\nReport bugs to %s\n\n", PACKAGE_STRING,
    PACKAGE_BUGREPORT);
  DEBUG_PRINT("=== BEGIN test_meshfunc ===\n\n");

  /* Check creation and destruction of meshes */
  DEBUG_PRINT("test_meshfunc: creating m1\n");
  CHECK_STAT(pspio_mesh_alloc(&m1, np), PSPIO_SUCCESS);
  DEBUG_PRINT("test_meshfunc: setting m1\n");
  CHECK_STAT(pspio_mesh_set(&m1, PSPIO_MESH_LOG1, a, b, r, rab), PSPIO_SUCCESS);
  DEBUG_PRINT("test_meshfunc: creating m2\n");
  CHECK_STAT(pspio_mesh_alloc(&m2, np), PSPIO_SUCCESS);
  DEBUG_PRINT("test_meshfunc: setting m2\n");
  CHECK_STAT(pspio_mesh_set(&m2, PSPIO_MESH_LINEAR, a, b, r, rab), PSPIO_SUCCESS);
  DEBUG_PRINT("\n");

  /* Check creation and destruction of mesh functions */
  DEBUG_PRINT("test_meshfunc: creating f1\n");
  CHECK_STAT(pspio_meshfunc_alloc(&f1, PSPIO_INTERP_GSL_CSPLINE, np), PSPIO_SUCCESS);
  DEBUG_PRINT("test_meshfunc: creating f2\n");
  CHECK_STAT(pspio_meshfunc_alloc(&f2, PSPIO_INTERP_GSL_CSPLINE, np), PSPIO_SUCCESS);
  DEBUG_PRINT("test_meshfunc: creating f3\n");
  CHECK_STAT(pspio_meshfunc_alloc(&f3, PSPIO_INTERP_GSL_CSPLINE, np), PSPIO_SUCCESS);
  DEBUG_PRINT("test_meshfunc: destroying f3\n");
  pspio_meshfunc_free(&f3);
  DEBUG_PRINT("\n");

  /* Check setting of mesh functions */
  DEBUG_PRINT("test_meshfunc: setting f1\n");
  CHECK_STAT(pspio_meshfunc_set(&f1, m1, f, NULL, NULL), PSPIO_SUCCESS);
  DEBUG_PRINT("test_meshfunc: setting f2 with explicit derivative\n");
  CHECK_STAT(pspio_meshfunc_set(&f2, m1, f, fp, NULL), PSPIO_SUCCESS);
  DEBUG_PRINT("\n");

  /* Check evaluation of mesh functions */
  DEBUG_PRINT("test_meshfunc: evaluating f1 at r=%f\n", r[6]);
  pspio_meshfunc_eval(f1, 1, &r[6], &feval);
  DEBUG_PRINT("test_meshfunc: result=%f\n", feval);
  DEBUG_PRINT("test_meshfunc: evaluating f2 at r=%f\n", r[6]);
  pspio_meshfunc_eval(f2, 1, &r[6], &feval);
  DEBUG_PRINT("test_meshfunc: result=%f\n", feval);
  DEBUG_PRINT("test_meshfunc: evaluating first derivative of f1 at r=%f\n", r[6]);
  pspio_meshfunc_eval_deriv(f1, 1, &r[6], &feval);
  DEBUG_PRINT("test_meshfunc: result=%f\n", feval);
  DEBUG_PRINT("test_meshfunc: evaluating first derivative of f2 at r=%f\n", r[6]);
  pspio_meshfunc_eval_deriv(f2, 1, &r[6], &feval);
  DEBUG_PRINT("test_meshfunc: result=%f\n", feval);
  DEBUG_PRINT("test_meshfunc: evaluating second derivative of f1 at r=%f\n", r[6]);
  pspio_meshfunc_eval_deriv2(f1, 1, &r[6], &feval);
  DEBUG_PRINT("test_meshfunc: result=%f\n", feval);
  DEBUG_PRINT("test_meshfunc: evaluating second derivative of f2 at r=%f\n", r[6]);
  pspio_meshfunc_eval_deriv2(f2, 1, &r[6], &feval);
  DEBUG_PRINT("test_meshfunc: result=%f\n", feval);
  DEBUG_PRINT("\n");


  /* Check copy of mesh functions */
  DEBUG_PRINT("test_meshfunc: copying f1 to a non-empty f2\n");
  CHECK_STAT(pspio_meshfunc_copy(&f2, f1), PSPIO_SUCCESS);
  DEBUG_PRINT("test_meshfunc: copying f1 to a NULL f3\n");
  CHECK_STAT(pspio_meshfunc_copy(&f3, f1), PSPIO_SUCCESS);


  DEBUG_PRINT("test_meshfunc: evaluating f1 at r=%f\n", r[6]);
  pspio_meshfunc_eval(f1, 1, &r[6], &feval);
  DEBUG_PRINT("test_meshfunc: result=%f\n", feval);
  DEBUG_PRINT("test_meshfunc: evaluating f2 at r=%f\n", r[6]);
  pspio_meshfunc_eval(f2, 1, &r[6], &feval);
  DEBUG_PRINT("test_meshfunc: result=%f\n", feval);
  DEBUG_PRINT("test_meshfunc: evaluating f3 at r=%f\n", r[6]);
  pspio_meshfunc_eval(f3, 1, &r[6], &feval);
  DEBUG_PRINT("test_meshfunc: result=%f\n", feval);
  DEBUG_PRINT("test_meshfunc: evaluating first derivative of f1 at r=%f\n", r[6]);
  pspio_meshfunc_eval_deriv(f1, 1, &r[6], &feval);
  DEBUG_PRINT("test_meshfunc: result=%f\n", feval);
  DEBUG_PRINT("test_meshfunc: evaluating first derivative of f2 at r=%f\n", r[6]);
  pspio_meshfunc_eval_deriv(f2, 1, &r[6], &feval);
  DEBUG_PRINT("test_meshfunc: result=%f\n", feval);
  DEBUG_PRINT("test_meshfunc: evaluating first derivative of f3 at r=%f\n", r[6]);
  pspio_meshfunc_eval_deriv(f3, 1, &r[6], &feval);
  DEBUG_PRINT("test_meshfunc: result=%f\n", feval);
  DEBUG_PRINT("test_meshfunc: evaluating second derivative of f1 at r=%f\n", r[6]);
  pspio_meshfunc_eval_deriv2(f1, 1, &r[6], &feval);
  DEBUG_PRINT("test_meshfunc: result=%f\n", feval);
  DEBUG_PRINT("test_meshfunc: evaluating second derivative of f2 at r=%f\n", r[6]);
  pspio_meshfunc_eval_deriv2(f2, 1, &r[6], &feval);
  DEBUG_PRINT("test_meshfunc: result=%f\n", feval);
  DEBUG_PRINT("test_meshfunc: evaluating second derivative of f3 at r=%f\n", r[6]);
  pspio_meshfunc_eval_deriv2(f3, 1, &r[6], &feval);
  DEBUG_PRINT("test_meshfunc: result=%f\n", feval);
  DEBUG_PRINT("\n");

  /* Destroy mesh functions */
  DEBUG_PRINT("test_meshfunc: destroying f1\n");
  PTR_STAT_SHOW(f1);
  pspio_meshfunc_free(&f1);
  DEBUG_PRINT("test_meshfunc: destroying f2\n");
  PTR_STAT_SHOW(f2);
  pspio_meshfunc_free(&f2);
  DEBUG_PRINT("test_meshfunc: destroying f3\n");
  PTR_STAT_SHOW(f3);
  pspio_meshfunc_free(&f3);
  DEBUG_PRINT("\n");

  DEBUG_PRINT("=== END test_meshfunc ===\n");

  return 0;
}
