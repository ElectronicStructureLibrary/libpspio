/* Copyright (C) 2015-2016 Micael Oliveira <micael.oliveira@mpsd.mpg.de>
 *                         Yann Pouillon <devops@materialsevolution.es>
 *
 * This file is part of Libpspio.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * version 2.0. If a copy of the MPL was not distributed with this file, You
 * can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Libpspio is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the Mozilla Public License version 2.0 for
 * more details.
 */

/**
 * @file check_pspio_mesh.c
 * @brief checks pspio_mesh.c and pspio_mesh.h 
 */

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
  m1 = NULL;
  m2 = NULL;
}


void mesh_compare_values(pspio_mesh_t *mesh, const double a, const double b, const double r[8], const double rab[8], const double tol)
{
  int i;
  const double *r2, *rab2;

  ck_assert(pspio_mesh_get_np(mesh) == 8);
  ck_assert(pspio_mesh_get_a(mesh) == a);
  ck_assert(pspio_mesh_get_b(mesh) == b);
  r2 = pspio_mesh_get_r(mesh);
  rab2 = pspio_mesh_get_rab(mesh);
  for (i=0; i<pspio_mesh_get_np(mesh); i++) {
    ck_assert( fabs(r2[i] - r[i]) <= tol);
    ck_assert( fabs(rab2[i] - rab[i]) <= tol);
  }
}


START_TEST(test_mesh_alloc)
{
  ck_assert(pspio_mesh_alloc(&m1, 10) == PSPIO_SUCCESS);
}
END_TEST

START_TEST(test_mesh_init)
{
  const double r_init[8] = {0.0, 0.05, 0.10, 0.20, 0.40, 0.65, 0.85, 1.00};
  const double rab_init[8] = {0.05, 0.05, 0.20, 0.20, 0.20, 0.20, 0.05, 0.05};
  const double a = 1.0, b = 2.0;

  ck_assert(pspio_mesh_init(m1, PSPIO_MESH_LOG1, a, b, r_init, rab_init) == PSPIO_SUCCESS);
  mesh_compare_values(m1, a, b, r_init, rab_init, 0.0);
}
END_TEST

START_TEST(test_mesh_init_parameters_linear)
{
  int i;
  double r_lin[8], rab_lin[8];
  const double a = 1.0, b = 2.0;

  for (i=0; i<pspio_mesh_get_np(m1); i++) {
    r_lin[i] = a*(i+1) + b;
    rab_lin[i] = a;
  }
  pspio_mesh_init_from_parameters(m1, PSPIO_MESH_LINEAR, a, b);
  mesh_compare_values(m1, a, b, r_lin, rab_lin, 0.0);
}
END_TEST

START_TEST(test_mesh_init_parameters_log1)
{
  int i;
  double r_log1[8], rab_log1[8];
  const double a = 1.0, b = 2.0;

  for (i=0; i<pspio_mesh_get_np(m1); i++) {
    r_log1[i] = b*exp(a*(i+1));
    rab_log1[i] = a*r_log1[i];
  }
  pspio_mesh_init_from_parameters(m1, PSPIO_MESH_LOG1, a, b);
  mesh_compare_values(m1, a, b, r_log1, rab_log1, 0.0);
}
END_TEST

START_TEST(test_mesh_init_parameters_log2)
{
  int i;
  double r_log2[8], rab_log2[8];
  const double a = 1.0, b = 2.0;

  for (i=0; i<pspio_mesh_get_np(m1); i++) {
    r_log2[i] = b*(exp(a*(i+1)) - 1.0);
    rab_log2[i] = a*r_log2[i] + a*b;
  }
  pspio_mesh_init_from_parameters(m1, PSPIO_MESH_LOG2, a, b);
  mesh_compare_values(m1, a, b, r_log2, rab_log2, 0.0);
}
END_TEST

START_TEST(test_mesh_init_points_linear)
{
  int i;
  double r_lin[8], rab_lin[8];
  const double a = 1.0, b = 2.0;

  for (i=0; i<pspio_mesh_get_np(m1); i++) {
    r_lin[i] = a*(i+1) + b;
    rab_lin[i] = a;
  }
  pspio_mesh_init_from_points(m1, r_lin, rab_lin);
  mesh_compare_values(m1, a, b, r_lin, rab_lin, 5.0e-10);
}
END_TEST

START_TEST(test_mesh_init_points_log1)
{
  int i;
  double r_log1[8], rab_log1[8];
  const double a = 1.0, b = 2.0;

  for (i=0; i<pspio_mesh_get_np(m1); i++) {
    r_log1[i] = b*exp(a*(i+1));
    rab_log1[i] = a*r_log1[i];
  }
  pspio_mesh_init_from_points(m1, r_log1, rab_log1);
  mesh_compare_values(m1, a, b, r_log1, rab_log1, 5.0e-10);
}
END_TEST

START_TEST(test_mesh_init_points_log2)
{
  int i;
  double r_log2[8], rab_log2[8];
  const double a = 1.0, b = 2.0;

  for (i=0; i<pspio_mesh_get_np(m1); i++) {
    r_log2[i] = b*(exp(a*(i+1)) - 1.0);
    rab_log2[i] = a*r_log2[i] + a*b;
  }
  pspio_mesh_init_from_points(m1, r_log2, rab_log2);
  mesh_compare_values(m1, a, b, r_log2, rab_log2, 5.0e-10);
}
END_TEST


START_TEST(test_mesh_cmp_diff_type)
{
  pspio_mesh_init_from_parameters(m1, PSPIO_MESH_LINEAR, 1.0, 2.0);
  pspio_mesh_init_from_parameters(m2, PSPIO_MESH_LOG1, 1.0, 2.0);
  ck_assert(pspio_mesh_cmp(m1, m2) == PSPIO_DIFF);
}
END_TEST

START_TEST(test_mesh_cmp_diff_a)
{
  pspio_mesh_init_from_parameters(m1, PSPIO_MESH_LINEAR, 1.0, 2.0);
  pspio_mesh_init_from_parameters(m2, PSPIO_MESH_LINEAR, 2.0, 2.0);
  ck_assert(pspio_mesh_cmp(m1, m2) == PSPIO_DIFF);
}
END_TEST

START_TEST(test_mesh_cmp_diff_b)
{
  pspio_mesh_init_from_parameters(m1, PSPIO_MESH_LINEAR, 2.0, 3.0);
  pspio_mesh_init_from_parameters(m2, PSPIO_MESH_LINEAR, 2.0, 2.0);
  ck_assert(pspio_mesh_cmp(m1, m2) == PSPIO_DIFF);
}
END_TEST

START_TEST(test_mesh_cmp_diff_r)
{
  const double r1[8] = {0.0, 0.05, 0.10, 0.20, 0.40, 0.65, 0.85, 1.00};
  const double r2[8] = {0.0, 0.05, 0.10, 0.25, 0.40, 0.65, 0.85, 1.00};

  pspio_mesh_init_from_points(m1, r1, NULL);
  pspio_mesh_init_from_points(m2, r2, NULL);
  ck_assert(pspio_mesh_cmp(m1, m2) == PSPIO_DIFF);
}
END_TEST

START_TEST(test_mesh_cmp_mtequal)
{
  pspio_mesh_init_from_parameters(m1, PSPIO_MESH_LINEAR, 1.0, 2.0);
  pspio_mesh_free(m2);
  m2 = NULL;
  pspio_mesh_alloc(&m2, 10);
  pspio_mesh_init_from_parameters(m2, PSPIO_MESH_LINEAR, 1.0, 2.0);
  ck_assert(pspio_mesh_cmp(m1, m2) == PSPIO_MTEQUAL);
}
END_TEST

START_TEST(test_mesh_cmp_equal)
{
  pspio_mesh_init_from_parameters(m1, PSPIO_MESH_LINEAR, 1.0, 2.0);
  pspio_mesh_init_from_parameters(m2, PSPIO_MESH_LINEAR, 1.0, 2.0);
  ck_assert(pspio_mesh_cmp(m1, m2) == PSPIO_EQUAL);
}
END_TEST

START_TEST(test_mesh_copy_null) 
{
  pspio_mesh_init_from_parameters(m1, PSPIO_MESH_LOG1, 1.0, 2.0);
  ck_assert(pspio_mesh_copy(&m2, m1) == PSPIO_SUCCESS);
  ck_assert(pspio_mesh_cmp(m1, m2) == PSPIO_EQUAL);
}
END_TEST

START_TEST(test_mesh_copy_nonnull) 
{
  pspio_mesh_init_from_parameters(m1, PSPIO_MESH_LOG1, 1.0, 2.0);
  pspio_mesh_init_from_parameters(m2, PSPIO_MESH_LOG2, 1.0, 2.0);

  ck_assert(pspio_mesh_copy(&m2, m1) == PSPIO_SUCCESS);
  ck_assert(pspio_mesh_cmp(m1, m2) == PSPIO_EQUAL);
}
END_TEST

START_TEST(test_mesh_copy_nonnull_size) 
{
  pspio_mesh_init_from_parameters(m1, PSPIO_MESH_LOG1, 1.0, 2.0);
  pspio_mesh_free(m2);
  m2 = NULL;
  pspio_mesh_alloc(&m2, 10);
  pspio_mesh_init_from_parameters(m2, PSPIO_MESH_LOG2, 1.0, 2.0);

  ck_assert(pspio_mesh_copy(&m2, m1) == PSPIO_SUCCESS);
  ck_assert(pspio_mesh_cmp(m1, m2) == PSPIO_EQUAL);
}
END_TEST

START_TEST(test_mesh_get_np)
{
  pspio_mesh_init_from_parameters(m1, PSPIO_MESH_LOG1, 1.0, 2.0);

  ck_assert(pspio_mesh_get_np(m1) == 8);
}
END_TEST

START_TEST(test_mesh_get_a)
{
  pspio_mesh_init_from_parameters(m1, PSPIO_MESH_LOG1, 1.0, 2.0);

  ck_assert(pspio_mesh_get_a(m1) == 1.0);
}
END_TEST

START_TEST(test_mesh_get_b)
{
  pspio_mesh_init_from_parameters(m1, PSPIO_MESH_LOG1, 1.0, 2.0);

  ck_assert(pspio_mesh_get_b(m1) == 2.0);
}
END_TEST

START_TEST(test_mesh_get_r)
{
  int i;
  double r[8] = {0.0, 0.05, 0.10, 0.20, 0.40, 0.65, 0.85, 1.00};
  const double *r_get;

  pspio_mesh_init_from_points(m1, r, NULL);

  r_get = pspio_mesh_get_r(m1);
  for (i=0; i<pspio_mesh_get_np(m1); i++) {
    ck_assert(r_get[i] == r[i]);
  }
}
END_TEST

START_TEST(test_mesh_get_rab)
{
  int i;
  double r[8] = {0.0, 0.05, 0.10, 0.20, 0.40, 0.65, 0.85, 1.00};
  double rab[8] = {0.05, 0.05, 0.20, 0.20, 0.20, 0.20, 0.05, 0.05};
  const double *rab_get;

  pspio_mesh_init_from_points(m1, r, rab);

  rab_get = pspio_mesh_get_rab(m1);
  for (i=0; i<pspio_mesh_get_np(m1); i++) {
    ck_assert(rab_get[i] == rab[i]);
  }
}
END_TEST


Suite * make_mesh_suite(void)
{
  Suite *s;
  TCase *tc_alloc, *tc_init, *tc_cmp, *tc_copy, *tc_get;

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

  tc_cmp = tcase_create("Comparison");
  tcase_add_checked_fixture(tc_cmp, mesh_setup, mesh_teardown);
  tcase_add_test(tc_cmp, test_mesh_cmp_equal);
  tcase_add_test(tc_cmp, test_mesh_cmp_mtequal);
  tcase_add_test(tc_cmp, test_mesh_cmp_diff_type);
  tcase_add_test(tc_cmp, test_mesh_cmp_diff_a);
  tcase_add_test(tc_cmp, test_mesh_cmp_diff_b);
  tcase_add_test(tc_cmp, test_mesh_cmp_diff_r);
  suite_add_tcase(s, tc_cmp);

  tc_copy = tcase_create("Copy");
  tcase_add_checked_fixture(tc_copy, mesh_setup, mesh_teardown);
  tcase_add_test(tc_copy, test_mesh_copy_null);
  tcase_add_test(tc_copy, test_mesh_copy_nonnull);
  tcase_add_test(tc_copy, test_mesh_copy_nonnull_size);
  suite_add_tcase(s, tc_copy);

  tc_get = tcase_create("Getters");
  tcase_add_checked_fixture(tc_get, mesh_setup, mesh_teardown);
  tcase_add_test(tc_get, test_mesh_get_np);
  tcase_add_test(tc_get, test_mesh_get_a);
  tcase_add_test(tc_get, test_mesh_get_b);
  tcase_add_test(tc_get, test_mesh_get_r);
  tcase_add_test(tc_get, test_mesh_get_rab);
  suite_add_tcase(s, tc_get);

  return s;
}
