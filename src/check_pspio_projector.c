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
 * @file check_pspio_projector.c
 * @brief checks pspio_projector.c and pspio_projector.h 
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <check.h>

#include "pspio_error.h"
#include "pspio_projector.h"



static pspio_mesh_t *m1 = NULL, *m2 = NULL;
static pspio_qn_t *qn11 = NULL, *qn12 = NULL, *qn2 = NULL;
static pspio_projector_t *proj11 = NULL, *proj12 = NULL, *proj2 = NULL;
const double e11 = 1.0e-2, e12 = 0.5, e2 = 0.25;
static double *p11, *p12, *p2;

void projector_setup(void)
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

  pspio_projector_free(proj11);
  pspio_projector_free(proj12);
  pspio_projector_free(proj2);
  pspio_projector_alloc(&proj11, 8);
  pspio_projector_alloc(&proj12, 8);
  pspio_projector_alloc(&proj2, 6);

  r = pspio_mesh_get_r(m1);
  p11 = (double *)malloc(8*sizeof(double));
  p12 = (double *)malloc(8*sizeof(double));
  for (i=0; i<pspio_mesh_get_np(m1); i++) {
    p11[i] = 20.0*exp(-r[i]*r[i]/10.0);
    p12[i] = 10.0*exp(-r[i]*r[i]/2.0);
  }

  r = pspio_mesh_get_r(m2);
  p2 = (double *)malloc(6*sizeof(double));
  for (i=0; i<pspio_mesh_get_np(m2); i++) {
    p2[i] = 20.0*exp(-r[i]*r[i]/3.0);
  }

}

void projector_teardown(void)
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

  pspio_projector_free(proj11);
  pspio_projector_free(proj12);
  pspio_projector_free(proj2);
  proj11 = NULL;
  proj12 = NULL;
  proj2 = NULL;

  free(p11);
  free(p12);
  free(p2);
}

void projector_compare_values(const pspio_mesh_t *mesh, const pspio_projector_t *proj, const pspio_qn_t *qn, const double energy, const double *p, const double tol)
{
  int i;
  const double *pp;

  ck_assert(pspio_mesh_cmp(pspio_meshfunc_get_mesh(proj->proj), mesh) == PSPIO_EQUAL);
  ck_assert(pspio_qn_cmp(pspio_projector_get_qn(proj), qn) == PSPIO_EQUAL);
  ck_assert( fabs(energy - pspio_projector_get_energy(proj)) < tol);
  pp = pspio_meshfunc_get_function(proj->proj);
  for (i=0; i<pspio_mesh_get_np(mesh); i++) {
    ck_assert_msg( fabs(p[i] - pp[i]) < tol, "projector at point %i: interp= %16.10e expected= %16.10e\n", i, pp[i], p[i]);
  }
}


START_TEST(test_projector_alloc)
{
  ck_assert(pspio_projector_alloc(&proj11, 3) == PSPIO_SUCCESS);
}
END_TEST

START_TEST(test_projector_init)
{
  ck_assert(pspio_projector_init(proj11, qn11, m1, p11) == PSPIO_SUCCESS);
  ck_assert(pspio_projector_set_energy(proj11, e11) == PSPIO_SUCCESS);
  projector_compare_values(m1, proj11, qn11, e11, p11, 1e-10);
}
END_TEST

START_TEST(test_projector_cmp_equal)
{
  ck_assert(pspio_projector_init(proj11, qn11, m1, p11) == PSPIO_SUCCESS);
  ck_assert(pspio_projector_set_energy(proj11, e11) == PSPIO_SUCCESS);
  ck_assert(pspio_projector_init(proj12, qn11, m1, p11) == PSPIO_SUCCESS);
  ck_assert(pspio_projector_set_energy(proj12, e11) == PSPIO_SUCCESS);
  ck_assert(pspio_projector_cmp(proj12, proj11) == PSPIO_EQUAL);
}
END_TEST

START_TEST(test_projector_cmp_diff_qn)
{
  ck_assert(pspio_projector_init(proj11, qn11, m1, p11) == PSPIO_SUCCESS);
  ck_assert(pspio_projector_set_energy(proj11, e11) == PSPIO_SUCCESS);
  ck_assert(pspio_projector_init(proj12, qn12, m1, p11) == PSPIO_SUCCESS);
  ck_assert(pspio_projector_set_energy(proj12, e11) == PSPIO_SUCCESS);
  ck_assert(pspio_projector_cmp(proj12, proj11) == PSPIO_DIFF);
}
END_TEST

START_TEST(test_projector_cmp_diff_energy)
{
  ck_assert(pspio_projector_init(proj11, qn11, m1, p11) == PSPIO_SUCCESS);
  ck_assert(pspio_projector_set_energy(proj11, e11) == PSPIO_SUCCESS);
  ck_assert(pspio_projector_init(proj12, qn11, m1, p11) == PSPIO_SUCCESS);
  ck_assert(pspio_projector_set_energy(proj12, e12) == PSPIO_SUCCESS);
  ck_assert(pspio_projector_cmp(proj12, proj11) == PSPIO_DIFF);
}
END_TEST

START_TEST(test_projector_cmp_diff_proj)
{
  ck_assert(pspio_projector_init(proj11, qn11, m1, p12) == PSPIO_SUCCESS);
  ck_assert(pspio_projector_set_energy(proj11, e11) == PSPIO_SUCCESS);
  ck_assert(pspio_projector_init(proj12, qn11, m1, p11) == PSPIO_SUCCESS);
  ck_assert(pspio_projector_set_energy(proj12, e11) == PSPIO_SUCCESS);
  ck_assert(pspio_projector_cmp(proj12, proj11) == PSPIO_DIFF);
}
END_TEST

START_TEST(test_projector_copy_null)
{
  pspio_projector_init(proj11, qn11, m1, p11);
  pspio_projector_set_energy(proj11, e11);
  pspio_projector_free(proj12);
  proj12 = NULL;
  ck_assert(pspio_projector_copy(&proj12, proj11) == PSPIO_SUCCESS);
  ck_assert(pspio_projector_cmp(proj12, proj11) == PSPIO_EQUAL);
}
END_TEST

START_TEST(test_projector_copy_nonnull)
{
  pspio_projector_init(proj11, qn11, m1, p11);
  pspio_projector_set_energy(proj11, e11);
  pspio_projector_init(proj12, qn12, m1, p12);
  pspio_projector_set_energy(proj11, e12);
  ck_assert(pspio_projector_copy(&proj12, proj11) == PSPIO_SUCCESS);
  ck_assert(pspio_projector_cmp(proj12, proj11) == PSPIO_EQUAL);
}
END_TEST

START_TEST(test_projector_copy_nonnull_size)
{
  pspio_projector_init(proj11, qn11, m1, p11);
  pspio_projector_set_energy(proj11, e11);
  pspio_projector_init(proj2, qn2, m2, p2);
  pspio_projector_set_energy(proj2, e2);
  ck_assert(pspio_projector_copy(&proj2, proj11) == PSPIO_SUCCESS);
  ck_assert(pspio_projector_cmp(proj2, proj11) == PSPIO_EQUAL);
}
END_TEST

START_TEST(test_projector_get_energy)
{
  pspio_projector_init(proj11, qn11, m1, p11);
  pspio_projector_set_energy(proj11, e11);
  ck_assert(pspio_projector_get_energy(proj11) == e11);
}
END_TEST

START_TEST(test_projector_get_qn)
{
  pspio_projector_init(proj11, qn11, m1, p11);
  ck_assert(pspio_qn_cmp(pspio_projector_get_qn(proj11), qn11) == PSPIO_EQUAL);
}
END_TEST

START_TEST(test_projector_eval)
{
  double eval, expect;

  pspio_projector_init(proj11, qn11, m1, p11);
  eval = pspio_projector_eval(proj11, 0.01);
  expect = 1.6456049569e+00;
  ck_assert_msg(fabs(eval - expect) <= 1e-10, "projector eval returned= %16.10e expected= %16.10e\n", eval, expect);
}
END_TEST

START_TEST(test_projector_eval_deriv)
{
  double eval, expect;

  pspio_projector_init(proj11, qn11, m1, p11);
  eval = pspio_projector_eval_deriv(proj11, 0.01);
  expect = -1.5477352024e-01;
  ck_assert_msg(fabs(eval - expect) <= 1e-10, "projector eval deriv returned= %16.10e expected= %16.10e\n", eval, expect);
}
END_TEST

START_TEST(test_projector_eval_deriv2)
{
  double eval, expect;

  pspio_projector_init(proj11, qn11, m1, p11);
  eval = pspio_projector_eval_deriv2(proj11, 0.01);
  expect = -5.8963771072e-03;
  ck_assert_msg(fabs(eval - expect) <= 1e-10, "projector eval deriv2 returned= %16.10e expected= %16.10e\n", eval, expect);
}
END_TEST


Suite * make_projector_suite(void)
{
  Suite *s;
  TCase *tc_alloc, *tc_init, *tc_cmp, *tc_copy, *tc_get, *tc_eval;

  s = suite_create("Projector");

  tc_alloc = tcase_create("Allocation");
  tcase_add_checked_fixture(tc_alloc, NULL, projector_teardown);
  tcase_add_test(tc_alloc, test_projector_alloc);
  suite_add_tcase(s, tc_alloc);

  tc_init = tcase_create("Initialization");
  tcase_add_checked_fixture(tc_init, projector_setup, projector_teardown);
  tcase_add_test(tc_init, test_projector_init);
  suite_add_tcase(s, tc_init);

  tc_cmp = tcase_create("Comparison");
  tcase_add_checked_fixture(tc_cmp, projector_setup, projector_teardown);
  tcase_add_test(tc_cmp, test_projector_cmp_equal);
  tcase_add_test(tc_cmp, test_projector_cmp_diff_qn);
  tcase_add_test(tc_cmp, test_projector_cmp_diff_energy);
  tcase_add_test(tc_cmp, test_projector_cmp_diff_proj);
  suite_add_tcase(s, tc_cmp);

  tc_copy = tcase_create("Copy");
  tcase_add_checked_fixture(tc_copy, projector_setup, projector_teardown);
  tcase_add_test(tc_copy, test_projector_copy_null);
  tcase_add_test(tc_copy, test_projector_copy_nonnull);
  tcase_add_test(tc_copy, test_projector_copy_nonnull_size);
  suite_add_tcase(s, tc_copy);

  tc_get = tcase_create("Getters");
  tcase_add_checked_fixture(tc_get, projector_setup, projector_teardown);
  tcase_add_test(tc_get, test_projector_get_energy);
  tcase_add_test(tc_get, test_projector_get_qn);
  suite_add_tcase(s, tc_get);

  tc_eval = tcase_create("Evaluation");
  tcase_add_checked_fixture(tc_eval, projector_setup, projector_teardown);
  tcase_add_test(tc_eval, test_projector_eval);
  tcase_add_test(tc_eval, test_projector_eval_deriv);
  tcase_add_test(tc_eval, test_projector_eval_deriv2);
  suite_add_tcase(s, tc_eval);
    
  return s;
}
