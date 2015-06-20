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

static pspio_mesh_t *m = NULL;

void mesh_setup(void) 
{
  pspio_mesh_free(m);
  pspio_mesh_alloc(&m, 8);
}

void mesh_teardown(void)
{
  pspio_mesh_free(m);
  m = NULL;
}

void mesh_compare(const pspio_mesh_t *m1, const pspio_mesh_t *m2)
{
  int i;
  double *r1, *r2, *rab1, *rab2;

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


void mesh_compare_values(const double a, const double b, const double r[8], const double rab[8], const double tol)
{
  int i;
  double *r2, *rab2;

  ck_assert(pspio_mesh_get_np(m) == 8);
  ck_assert(pspio_mesh_get_a(m) == a);
  ck_assert(pspio_mesh_get_b(m) == b);
  r2 = pspio_mesh_get_r(m);
  rab2 = pspio_mesh_get_rab(m);
  for (i=0; i<pspio_mesh_get_np(m); i++) {
    ck_assert( fabs(r2[i] - r[i]) <= tol);
    ck_assert( fabs(rab2[i] - rab[i]) <= tol);
  }
}


START_TEST(test_mesh_alloc)
{
  ck_assert(pspio_mesh_alloc(&m, 10) == PSPIO_SUCCESS);
}
END_TEST

START_TEST(test_mesh_init)
{
  const double r_init[8] = {0.0, 0.05, 0.10, 0.20, 0.40, 0.65, 0.85, 1.00};
  const double rab_init[8] = {0.05, 0.05, 0.20, 0.20, 0.20, 0.20, 0.05, 0.05};
  const double a = 1.0, b = 2.0;

  ck_assert(pspio_mesh_init(m, PSPIO_MESH_LOG1, a, b, r_init, rab_init) == PSPIO_SUCCESS);
  mesh_compare_values(a, b, r_init, rab_init, 0.0);
}
END_TEST

START_TEST(test_mesh_init_parameters_linear)
{
  int i;
  double r_lin[8], rab_lin[8];
  const double a = 1.0, b = 2.0;

  for (i=0; i<pspio_mesh_get_np(m); i++) {
    r_lin[i] = a*(i+1) + b;
    rab_lin[i] = a;
  }
  pspio_mesh_init_from_parameters(m, PSPIO_MESH_LINEAR, a, b);
  mesh_compare_values(a, b, r_lin, rab_lin, 0.0);
}
END_TEST

START_TEST(test_mesh_init_parameters_log1)
{
  int i;
  double r_log1[8], rab_log1[8];
  const double a = 1.0, b = 2.0;

  for (i=0; i<pspio_mesh_get_np(m); i++) {
    r_log1[i] = b*exp(a*(i+1));
    rab_log1[i] = a*r_log1[i];
  }
  pspio_mesh_init_from_parameters(m, PSPIO_MESH_LOG1, a, b);
  mesh_compare_values(a, b, r_log1, rab_log1, 0.0);
}
END_TEST

START_TEST(test_mesh_init_parameters_log2)
{
  int i;
  double r_log2[8], rab_log2[8];
  const double a = 1.0, b = 2.0;

  for (i=0; i<pspio_mesh_get_np(m); i++) {
    r_log2[i] = b*(exp(a*(i+1)) - 1.0);
    rab_log2[i] = a*r_log2[i] + a*b;
  }
  pspio_mesh_init_from_parameters(m, PSPIO_MESH_LOG2, a, b);
  mesh_compare_values(a, b, r_log2, rab_log2, 0.0);
}
END_TEST

START_TEST(test_mesh_init_points_linear)
{
  int i;
  double r_lin[8], rab_lin[8];
  const double a = 1.0, b = 2.0;

  for (i=0; i<pspio_mesh_get_np(m); i++) {
    r_lin[i] = a*(i+1) + b;
    rab_lin[i] = a;
  }
  pspio_mesh_init_from_points(m, r_lin, rab_lin);
  mesh_compare_values(a, b, r_lin, rab_lin, 5.0e-10);
}
END_TEST

START_TEST(test_mesh_init_points_log1)
{
  int i;
  double r_log1[8], rab_log1[8];
  const double a = 1.0, b = 2.0;

  for (i=0; i<pspio_mesh_get_np(m); i++) {
    r_log1[i] = b*exp(a*(i+1));
    rab_log1[i] = a*r_log1[i];
  }
  pspio_mesh_init_from_points(m, r_log1, rab_log1);
  mesh_compare_values(a, b, r_log1, rab_log1, 5.0e-10);
}
END_TEST

START_TEST(test_mesh_init_points_log2)
{
  int i;
  double r_log2[8], rab_log2[8];
  const double a = 1.0, b = 2.0;

  for (i=0; i<pspio_mesh_get_np(m); i++) {
    r_log2[i] = b*(exp(a*(i+1)) - 1.0);
    rab_log2[i] = a*r_log2[i] + a*b;
  }
  pspio_mesh_init_from_points(m, r_log2, rab_log2);
  mesh_compare_values(a, b, r_log2, rab_log2, 5.0e-10);
}
END_TEST

START_TEST(test_mesh_copy_null) 
{
  pspio_mesh_t *m2 = NULL;

  pspio_mesh_init_from_parameters(m, PSPIO_MESH_LOG1, 1.0, 2.0);
  ck_assert(pspio_mesh_copy(&m2, m) == PSPIO_SUCCESS);
  mesh_compare(m, m2);
}
END_TEST

START_TEST(test_mesh_copy_nonnull) 
{
  pspio_mesh_t *m2 = NULL;

  pspio_mesh_init_from_parameters(m, PSPIO_MESH_LOG1, 1.0, 2.0);
  pspio_mesh_alloc(&m2, 8);
  pspio_mesh_init_from_parameters(m2, PSPIO_MESH_LOG2, 1.0, 2.0);

  ck_assert(pspio_mesh_copy(&m2, m) == PSPIO_SUCCESS);
  mesh_compare(m, m2);
}
END_TEST

START_TEST(test_mesh_copy_nonnull_size) 
{
  pspio_mesh_t *m2 = NULL;

  pspio_mesh_init_from_parameters(m, PSPIO_MESH_LOG1, 1.0, 2.0);
  pspio_mesh_alloc(&m2, 10);
  pspio_mesh_init_from_parameters(m2, PSPIO_MESH_LOG2, 1.0, 2.0);

  ck_assert(pspio_mesh_copy(&m2, m) == PSPIO_SUCCESS);
  mesh_compare(m, m2);
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
  tcase_add_test(tc_init, test_mesh_init_parameters_linear);
  tcase_add_test(tc_init, test_mesh_init_parameters_log1);
  tcase_add_test(tc_init, test_mesh_init_parameters_log2);
  tcase_add_test(tc_init, test_mesh_init_points_linear);
  tcase_add_test(tc_init, test_mesh_init_points_log1);
  tcase_add_test(tc_init, test_mesh_init_points_log2);
  suite_add_tcase(s, tc_init);

  tc_copy = tcase_create("Copy");
  tcase_add_checked_fixture(tc_copy, mesh_setup, mesh_teardown);
  tcase_add_test(tc_copy, test_mesh_copy_null);
  tcase_add_test(tc_copy, test_mesh_copy_nonnull);
  tcase_add_test(tc_copy, test_mesh_copy_nonnull_size);
  suite_add_tcase(s, tc_copy);
    
  return s;
}
