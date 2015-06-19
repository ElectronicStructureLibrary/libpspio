/*
 Copyright (C) 2011 J. Alberdi, M. Oliveira, Y. Pouillon, and M. Verstraete
 Copyright (C) 2015 M. Oliveira

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
 * @file check_pspio_mesh.c
 * @brief checks pspio_mesh.c and pspio_mesh.h 
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <check.h>

#include "pspio_error.h"
#include "pspio_mesh.h"

static pspio_mesh_t *m1 = NULL, *m2 = NULL;

void mesh_setup(void)
{
  pspio_mesh_free(m1);
  pspio_mesh_free(m2);
  pspio_mesh_alloc(&m1, 8);
  pspio_mesh_alloc(&m2, 8);
}

void mesh_teardown(void)
{
  pspio_mesh_free(m1);
  pspio_mesh_free(m2);
}

START_TEST(test_mesh_alloc)
{
  ck_assert(pspio_mesh_alloc(&m1, 2) == PSPIO_SUCCESS);
  ck_assert(pspio_mesh_alloc(&m2, 10) == PSPIO_SUCCESS);
}
END_TEST

START_TEST(test_mesh_init)
{
  const double r_init[] = {0.0, 0.05, 0.10, 0.20, 0.40, 0.65, 0.85, 1.00};
  const double rab_init[] = {0.05, 0.05, 0.20, 0.20, 0.20, 0.20, 0.05, 0.05};

  double r_log1[8];
  double rab_log1[8];

  double r_log2[8];
  double rab_log2[8];

  double r_lin[8];
  double rab_lin[8];

  const double a = 1.0;
  const double b = 2.0;
  const double tol = 5.0e-10;

  int i;
  double *r, *rab;

  /* Generate values of auxiliary vectors */
  for (i=0; i<pspio_mesh_get_np(m1); i++) {
    r_lin[i] = a*(i+1) + b;
    rab_lin[i] = a;

    r_log1[i] = b*exp(a*(i+1));
    rab_log1[i] = a*r_log1[i];

    r_log2[i] = b*(exp(a*(i+1)) - 1.0);
    rab_log2[i] = a*r_log2[i] + a*b;
  }

  /* mesh_init */
  ck_assert(pspio_mesh_init(m1, PSPIO_MESH_LOG1, a, b, r_init, rab_init) == PSPIO_SUCCESS);
  ck_assert(pspio_mesh_get_np(m1) == 8);
  ck_assert(pspio_mesh_get_a(m1) == a);
  ck_assert(pspio_mesh_get_b(m1) == b);
  r = pspio_mesh_get_r(m1);
  rab = pspio_mesh_get_rab(m1);
  for (i=0; i<pspio_mesh_get_np(m1); i++) {
    ck_assert(r[i] == r_init[i]);
    ck_assert(rab[i] == rab_init[i]);
  }

  /* mesh_from_parameters with LINEAR mesh */
  pspio_mesh_init_from_parameters(m1, PSPIO_MESH_LINEAR, a, b);
  ck_assert(pspio_mesh_get_np(m1) == 8);
  ck_assert(pspio_mesh_get_a(m1) == a);
  ck_assert(pspio_mesh_get_b(m1) == b);
  r = pspio_mesh_get_r(m1);
  rab = pspio_mesh_get_rab(m1);
  for (i=0; i<pspio_mesh_get_np(m1); i++) {
    ck_assert(r[i] == r_lin[i]);
    ck_assert(rab[i] == rab_lin[i]);
  }

  /* mesh_from_parameters with LOG1 mesh */
  pspio_mesh_init_from_parameters(m1, PSPIO_MESH_LOG1, a, b);
  ck_assert(pspio_mesh_get_np(m1) == 8);
  ck_assert(pspio_mesh_get_a(m1) == a);
  ck_assert(pspio_mesh_get_b(m1) == b);
  r = pspio_mesh_get_r(m1);
  rab = pspio_mesh_get_rab(m1);
  for (i=0; i<pspio_mesh_get_np(m1); i++) {
    ck_assert(r[i] == r_log1[i]);
    ck_assert(rab[i] == rab_log1[i]);
  }

  /* mesh_from_parameters with LOG2 mesh */
  pspio_mesh_init_from_parameters(m1, PSPIO_MESH_LOG2, a, b);
  ck_assert(pspio_mesh_get_np(m1) == 8);
  ck_assert(pspio_mesh_get_a(m1) == a);
  ck_assert(pspio_mesh_get_b(m1) == b);
  r = pspio_mesh_get_r(m1);
  rab = pspio_mesh_get_rab(m1);
  for (i=0; i<pspio_mesh_get_np(m1); i++) {
    ck_assert(r[i] == r_log2[i]);
    ck_assert(rab[i] == rab_log2[i]);
  }

  /* mesh_from_points with LINEAR mesh */
  pspio_mesh_init_from_points(m1, r_lin, rab_lin);
  ck_assert(pspio_mesh_get_np(m1) == 8);
  ck_assert(fabs(pspio_mesh_get_a(m1) - a) < tol);
  ck_assert(fabs(pspio_mesh_get_b(m1) - b) < tol);
  r = pspio_mesh_get_r(m1);
  rab = pspio_mesh_get_rab(m1);
  for (i=0; i<pspio_mesh_get_np(m1); i++) {
    ck_assert(r[i] == r_lin[i]);
    ck_assert(rab[i] == rab_lin[i]);
  }

  /* mesh_from_points with LOG1 mesh */
  pspio_mesh_init_from_points(m1, r_log1, rab_log1);
  ck_assert(pspio_mesh_get_np(m1) == 8);
  ck_assert(fabs(pspio_mesh_get_a(m1) - a) < tol);
  ck_assert(fabs(pspio_mesh_get_b(m1) - b) < tol);
  r = pspio_mesh_get_r(m1);
  rab = pspio_mesh_get_rab(m1);
  for (i=0; i<pspio_mesh_get_np(m1); i++) {
    ck_assert(r[i] == r_log1[i]);
    ck_assert(rab[i] == rab_log1[i]);
  }

  /* mesh_from_points with LOG2 mesh */
  pspio_mesh_init_from_points(m1, r_log2, rab_log2);
  ck_assert(pspio_mesh_get_np(m1) == 8);
  ck_assert(fabs(pspio_mesh_get_a(m1) - a) < tol);
  ck_assert(fabs(pspio_mesh_get_b(m1) - b) < tol);
  r = pspio_mesh_get_r(m1);
  rab = pspio_mesh_get_rab(m1);
  for (i=0; i<pspio_mesh_get_np(m1); i++) {
    ck_assert(r[i] == r_log2[i]);
    ck_assert(rab[i] == rab_log2[i]);
  }

}
END_TEST

START_TEST(test_mesh_copy)
{
  int i;
  double *r1, *r2, *rab1, *rab2;

  pspio_mesh_init_from_parameters(m1, PSPIO_MESH_LOG1, 1.0, 2.0);

  /* Copy m1 to NULL m2 */
  ck_assert(pspio_mesh_copy(&m2, m1) == PSPIO_SUCCESS);
  ck_assert(pspio_mesh_get_np(m1) == pspio_mesh_get_np(m2));
  ck_assert(pspio_mesh_get_a(m1) == pspio_mesh_get_a(m2));
  ck_assert(pspio_mesh_get_b(m1) == pspio_mesh_get_b(m2));
  r1 = pspio_mesh_get_r(m1);
  r2 = pspio_mesh_get_r(m2);
  for (i=0; i<pspio_mesh_get_np(m1); i++) {
    ck_assert(r1[i] == r2[i]);
  }
  rab1 = pspio_mesh_get_rab(m1);
  rab2 = pspio_mesh_get_rab(m2);
  for (i=0; i<pspio_mesh_get_np(m1); i++) {
    ck_assert(rab1[i] == rab2[i]);
  }

  /* Copy m1 to non-NULL m2 */
  ck_assert(pspio_mesh_copy(&m2, m1) == PSPIO_SUCCESS);
  ck_assert(pspio_mesh_get_np(m1) == pspio_mesh_get_np(m2));
  ck_assert(pspio_mesh_get_a(m1) == pspio_mesh_get_a(m2));
  ck_assert(pspio_mesh_get_b(m1) == pspio_mesh_get_b(m2));
  r1 = pspio_mesh_get_r(m1);
  r2 = pspio_mesh_get_r(m2);
  for (i=0; i<pspio_mesh_get_np(m1); i++) {
    ck_assert(r1[i] == r2[i]);
  }
  rab1 = pspio_mesh_get_rab(m1);
  rab2 = pspio_mesh_get_rab(m2);
  for (i=0; i<pspio_mesh_get_np(m1); i++) {
    ck_assert(rab1[i] == rab2[i]);
  }

}
END_TEST

Suite * make_mesh_suite(void)
{
  Suite *s;
  TCase *tc_alloc, *tc_init, *tc_copy;

  s = suite_create("Mesh");

  tc_alloc = tcase_create("Allocation");
  tcase_add_checked_fixture(tc_alloc, NULL, mesh_teardown);
  tcase_add_test(tc_alloc, test_mesh_alloc);
  suite_add_tcase(s, tc_alloc);

  tc_init = tcase_create("Initialization");
  tcase_add_checked_fixture(tc_init, mesh_setup, mesh_teardown);
  tcase_add_test(tc_init, test_mesh_init);
  suite_add_tcase(s, tc_init);

  tc_copy = tcase_create("Copy");
  tcase_add_checked_fixture(tc_copy, mesh_setup, mesh_teardown);
  tcase_add_test(tc_copy, test_mesh_copy);
  suite_add_tcase(s, tc_copy);
    
  return s;
}
