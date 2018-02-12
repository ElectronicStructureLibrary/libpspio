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
 * @file check_pspio_potential.c
 * @brief checks pspio_potential.c and pspio_potential.h 
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <check.h>

#include "pspio_error.h"
#include "pspio_potential.h"


static pspio_mesh_t *m1 = NULL, *m2 = NULL;
static pspio_qn_t *qn11 = NULL, *qn12 = NULL, *qn2 = NULL;
static pspio_potential_t *pot11 = NULL, *pot12 = NULL, *pot2 = NULL;

static double *v11, *v12, *v2;

void potential_setup(void)
{
  int i;
  const double *r;
  const double a = 1.0;
  const double b = 2.0;

  pspio_mesh_free(m1);
  pspio_mesh_alloc(&m1, 8);
  pspio_mesh_init_from_parameters(m1, PSPIO_MESH_LOG1, a, b);
  pspio_mesh_free(m2);
  pspio_mesh_alloc(&m2, 6);
  pspio_mesh_init_from_parameters(m2, PSPIO_MESH_LOG2, a, b);

  pspio_qn_free(qn11);
  pspio_qn_alloc(&qn11);
  pspio_qn_init(qn11, 1, 0, 0);
  pspio_qn_free(qn12);
  pspio_qn_alloc(&qn12);
  pspio_qn_init(qn12, 2, 1, 0);
  pspio_qn_free(qn2);
  pspio_qn_alloc(&qn2);
  pspio_qn_init(qn2, 3, 2, 0.5);

  pspio_potential_free(pot11);
  pspio_potential_free(pot12);
  pspio_potential_free(pot2);
  pspio_potential_alloc(&pot11, 8);
  pspio_potential_alloc(&pot12, 8);
  pspio_potential_alloc(&pot2, 6);

  r = pspio_mesh_get_r(m1);
  v11 = (double *)malloc(8*sizeof(double));
  v12 = (double *)malloc(8*sizeof(double));
  for (i=0; i<pspio_mesh_get_np(m1); i++) {
    v11[i] = 20.0*exp(-r[i]*r[i]/10.0);
    v12[i] = 10.0*exp(-r[i]*r[i]/2.0);
  }

  r = pspio_mesh_get_r(m2);
  v2 = (double *)malloc(6*sizeof(double));
  for (i=0; i<pspio_mesh_get_np(m2); i++) {
    v2[i] = 20.0*exp(-r[i]*r[i]/3.0);
  }

}

void potential_teardown(void)
{
  pspio_mesh_free(m1);
  pspio_mesh_free(m2);
  m1 = NULL;
  m2 = NULL;

  pspio_qn_free(qn11);
  pspio_qn_free(qn12);
  pspio_qn_free(qn2);
  qn11 = NULL;
  qn12 = NULL;
  qn2 = NULL;

  pspio_potential_free(pot11);
  pspio_potential_free(pot12);
  pspio_potential_free(pot2);
  pot11 = NULL;
  pot12 = NULL;
  pot2 = NULL;

  free(v11);
  free(v12);
  free(v2);
}

void potential_compare_values(const pspio_mesh_t *mesh, const pspio_potential_t *pot, const pspio_qn_t *qn, const double *v, const double tol)
{
  int i;
  const double *vv;

  ck_assert(pspio_mesh_cmp(pspio_meshfunc_get_mesh(pot->v), mesh) == PSPIO_EQUAL);
  ck_assert(pspio_qn_cmp(pspio_potential_get_qn(pot), qn) == PSPIO_EQUAL);
  vv = pspio_meshfunc_get_function(pot->v);
  for (i=0; i<pspio_mesh_get_np(mesh); i++) {
    ck_assert_msg( fabs(v[i] - vv[i]) < tol, "potential at point %i: interp= %16.10e expected= %16.10e\n", i, vv[i], v[i]);
  }
}


START_TEST(test_potential_alloc)
{
  ck_assert(pspio_potential_alloc(&pot11, 3) == PSPIO_SUCCESS);
}
END_TEST

START_TEST(test_potential_init)
{
  ck_assert(pspio_potential_init(pot11, qn11, m1, v11) == PSPIO_SUCCESS);
  potential_compare_values(m1, pot11, qn11, v11, 1e-10);
}
END_TEST

START_TEST(test_potential_cmp_equal)
{
  ck_assert(pspio_potential_init(pot11, qn11, m1, v11) == PSPIO_SUCCESS);
  ck_assert(pspio_potential_init(pot12, qn11, m1, v11) == PSPIO_SUCCESS);
  ck_assert(pspio_potential_cmp(pot11, pot12) == PSPIO_EQUAL);
}
END_TEST

START_TEST(test_potential_cmp_diff_v)
{
  ck_assert(pspio_potential_init(pot11, qn11, m1, v11) == PSPIO_SUCCESS);
  ck_assert(pspio_potential_init(pot12, qn11, m1, v12) == PSPIO_SUCCESS);
  ck_assert(pspio_potential_cmp(pot11, pot12) == PSPIO_DIFF);
}
END_TEST

START_TEST(test_potential_cmp_diff_qn)
{
  ck_assert(pspio_potential_init(pot11, qn11, m1, v11) == PSPIO_SUCCESS);
  ck_assert(pspio_potential_init(pot12, qn12, m1, v11) == PSPIO_SUCCESS);
  ck_assert(pspio_potential_cmp(pot11, pot12) == PSPIO_DIFF);
}
END_TEST

START_TEST(test_potential_copy_null)
{
  pspio_potential_init(pot11, qn11, m1, v11);
  pspio_potential_free(pot12);
  pot12 = NULL;
  ck_assert(pspio_potential_copy(&pot12, pot11) == PSPIO_SUCCESS);
  ck_assert(pspio_potential_cmp(pot11, pot12) == PSPIO_EQUAL);
}
END_TEST

START_TEST(test_potential_copy_nonnull)
{
  pspio_potential_init(pot11, qn11, m1, v11);
  pspio_potential_init(pot12, qn12, m1, v12);
  ck_assert(pspio_potential_copy(&pot12, pot11) == PSPIO_SUCCESS);
  ck_assert(pspio_potential_cmp(pot11, pot12) == PSPIO_EQUAL);
}
END_TEST

START_TEST(test_potential_copy_nonnull_size)
{
  pspio_potential_init(pot11, qn11, m1, v11);
  pspio_potential_init(pot2, qn2, m2, v2);
  ck_assert(pspio_potential_copy(&pot2, pot11) == PSPIO_SUCCESS);
  ck_assert(pspio_potential_cmp(pot2, pot11) == PSPIO_EQUAL);
}
END_TEST

START_TEST(test_potential_get_qn)
{
  pspio_potential_init(pot11, qn11, m1, v11);
  ck_assert(pspio_qn_cmp(pspio_potential_get_qn(pot11), qn11) == PSPIO_EQUAL);
}
END_TEST

START_TEST(test_potential_eval)
{
  double eval, expect;

  pspio_potential_init(pot11, qn11, m1, v11);
  eval = pspio_potential_eval(pot11, 0.01);
  expect = 1.6456049569e+00;
  ck_assert_msg(fabs(eval - expect) <= 1e-10, "potential eval returned= %16.10e expected= %16.10e\n", eval, expect);
}
END_TEST

START_TEST(test_potential_eval_deriv)
{
  double eval, expect;

  pspio_potential_init(pot11, qn11, m1, v11);
  eval = pspio_potential_eval_deriv(pot11, 0.01);
  expect = -1.5477352024e-01;
  ck_assert_msg(fabs(eval - expect) <= 1e-10, "potential eval deriv returned= %16.10e expected= %16.10e\n", eval, expect);
}
END_TEST

START_TEST(test_potential_eval_deriv2)
{
  double eval, expect;

  pspio_potential_init(pot11, qn11, m1, v11);
  eval = pspio_potential_eval_deriv2(pot11, 0.01);
  expect = -5.8963771072e-03;
  ck_assert_msg(fabs(eval - expect) <= 1e-10, "potential eval deriv2 returned= %16.10e expected= %16.10e\n", eval, expect);
}
END_TEST


Suite * make_potential_suite(void)
{
  Suite *s;
  TCase *tc_alloc, *tc_init, *tc_cmp, *tc_copy, *tc_get, *tc_eval;

  s = suite_create("Potential");

  tc_alloc = tcase_create("Allocation");
  tcase_add_checked_fixture(tc_alloc, NULL, potential_teardown);
  tcase_add_test(tc_alloc, test_potential_alloc);
  suite_add_tcase(s, tc_alloc);

  tc_init = tcase_create("Initialization");
  tcase_add_checked_fixture(tc_init, potential_setup, potential_teardown);
  tcase_add_test(tc_init, test_potential_init);
  suite_add_tcase(s, tc_init);

  tc_cmp = tcase_create("Comparison");
  tcase_add_checked_fixture(tc_cmp, potential_setup, potential_teardown);
  tcase_add_test(tc_cmp, test_potential_cmp_equal);
  tcase_add_test(tc_cmp, test_potential_cmp_diff_v);
  tcase_add_test(tc_cmp, test_potential_cmp_diff_qn);
  suite_add_tcase(s, tc_cmp);

  tc_copy = tcase_create("Copy");
  tcase_add_checked_fixture(tc_copy, potential_setup, potential_teardown);
  tcase_add_test(tc_copy, test_potential_copy_null);
  tcase_add_test(tc_copy, test_potential_copy_nonnull);
  tcase_add_test(tc_copy, test_potential_copy_nonnull_size);
  suite_add_tcase(s, tc_copy);

  tc_get = tcase_create("Getters");
  tcase_add_checked_fixture(tc_get, potential_setup, potential_teardown);
  tcase_add_test(tc_get, test_potential_get_qn);
  suite_add_tcase(s, tc_get);

  tc_eval = tcase_create("Evaluation");
  tcase_add_checked_fixture(tc_eval, potential_setup, potential_teardown);
  tcase_add_test(tc_eval, test_potential_eval);
  tcase_add_test(tc_eval, test_potential_eval_deriv);
  tcase_add_test(tc_eval, test_potential_eval_deriv2);
  suite_add_tcase(s, tc_eval);
    
  return s;
}

