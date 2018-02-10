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
 * @file check_pspio_meshfunc.c
 * @brief checks pspio_meshfunc.c and pspio_meshfunc.h 
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <check.h>

#include "pspio_error.h"
#include "pspio_mesh.h"
#include "pspio_meshfunc.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif

static pspio_mesh_t *m1 = NULL, *m2 = NULL;
static pspio_meshfunc_t *mf11 = NULL, *mf12 = NULL, *mf2 = NULL;
static double *f11, *f11p, *f11pp;
static double *f12, *f12p, *f12pp;
static double *f2,  *f2p,  *f2pp;

void meshfunc_setup(void)
{
  int i;
  const double *r2;
  const double r1[] = {0.0, 0.05, 0.10, 0.20, 0.40, 0.65, 0.85, 1.00};
  const double r1ab[] = {0.05, 0.05, 0.20, 0.20, 0.20, 0.20, 0.05, 0.05};
  const double a = 1.0;
  const double b = 2.0;

  pspio_mesh_free(m1);
  pspio_mesh_alloc(&m1, 8);
  pspio_mesh_init(m1, PSPIO_MESH_UNKNOWN, a, b, r1, r1ab);

  pspio_mesh_free(m2);
  pspio_mesh_alloc(&m2, 6);
  pspio_mesh_init_from_parameters(m2, PSPIO_MESH_LOG1, a, b);
  r2 = pspio_mesh_get_r(m2);

  pspio_meshfunc_free(mf11);
  pspio_meshfunc_free(mf12);
  pspio_meshfunc_free(mf2);
  pspio_meshfunc_alloc(&mf11, 8);
  pspio_meshfunc_alloc(&mf12, 8);
  pspio_meshfunc_alloc(&mf2, 6);

  f11 = (double *)malloc(8*sizeof(double));
  f11p = (double *)malloc(8*sizeof(double));
  f11pp = (double *)malloc(8*sizeof(double));
  for (i=0; i<8; i++) {
    f11[i] = r1[i]*r1[i];
    f11p[i] = 2.0*r1[i];
    f11pp[i] = 2.0;
  }
  f12 = (double *)malloc(8*sizeof(double));
  f12p = (double *)malloc(8*sizeof(double));
  f12pp = (double *)malloc(8*sizeof(double));
  for (i=0; i<8; i++) {
    f12[i] = r1[i]*r1[i]*r1[i];
    f12p[i] = 3.0*r1[i]*r1[i];
    f12pp[i] = 6.0*r1[i];
  }
  f2 = (double *)malloc(6*sizeof(double));
  f2p = (double *)malloc(6*sizeof(double));
  f2pp = (double *)malloc(6*sizeof(double));
  for (i=0; i<pspio_mesh_get_np(m2); i++) {
    f2[i] = r2[i]*r2[i];
    f2p[i] = 2.0*r2[i];
    f2pp[i] = 2.0;
  }

}

void meshfunc_teardown(void)
{
  pspio_mesh_free(m1);
  pspio_mesh_free(m2);
  m1 = NULL;
  m2 = NULL;

  pspio_meshfunc_free(mf11);
  pspio_meshfunc_free(mf12);
  pspio_meshfunc_free(mf2);
  mf11 = NULL;
  mf12 = NULL;
  mf2 = NULL;

  free(f11);
  free(f11p);
  free(f11pp);
  free(f12);
  free(f12p);
  free(f12pp);
  free(f2);
  free(f2p);
  free(f2pp);
}

void meshfunc_compare_values(const pspio_mesh_t *mesh, const pspio_meshfunc_t *meshfunc, const double *f, const double *fp, const double *fpp, const double tol)
{
  int i;
  const double *ff, *ffp, *ffpp;

  ck_assert(pspio_mesh_cmp(pspio_meshfunc_get_mesh(meshfunc), mesh) == PSPIO_EQUAL);
  ff   = pspio_meshfunc_get_function(meshfunc);
  ffp  = pspio_meshfunc_get_deriv1(meshfunc);
  ffpp = pspio_meshfunc_get_deriv2(meshfunc);
  for (i=0; i<pspio_mesh_get_np(mesh); i++) {
    ck_assert_msg( fabs(f[i] - ff[i]) < tol, "function at point %i: interp= %16.10e expected=%16.10e\n", i, ff[i], f[i]);
    ck_assert_msg( fabs(fp[i] - ffp[i]) < tol, "deriv at point %i: interp= %16.10e expected=%16.10e\n", i, ffp[i], fp[i]);
    ck_assert_msg( fabs(fpp[i] - ffpp[i]) < tol, "deriv2 at point %i: interp= %16.10e expected=%16.10e\n", i, ffpp[i], fpp[i]);
  }
}

START_TEST(test_meshfunc_alloc)
{
  ck_assert(pspio_meshfunc_alloc(&mf11, 3) == PSPIO_SUCCESS);
}
END_TEST

START_TEST(test_meshfunc_init1)
{
  const double fp[] = {2.8981947526e-02, 9.2036104948e-02, 2.0287363268e-01, 3.9868599401e-01,
                       8.0213677058e-01, 1.2920270399e+00, 1.7269932400e+00, 1.9115033800e+00};
  const double fpp[] = {0.0, 2.5221662969e+00, 1.9113348125e+00, 2.0049124140e+00,
                        2.0295953518e+00, 1.8895268024e+00, 2.4601351993e+00, 0.0};

  ck_assert(pspio_meshfunc_init(mf11, m1, f11, NULL, NULL) == PSPIO_SUCCESS);
  meshfunc_compare_values(m1, mf11, f11, fp, fpp, 1e-10);
}
END_TEST

START_TEST(test_meshfunc_init2)
{
  const double fpp[] = {0.0, 2.5221662969e+00, 1.9113348125e+00, 2.0049124140e+00,
                        2.0295953518e+00, 1.8895268024e+00, 2.4601351993e+00, 0.0};

  ck_assert(pspio_meshfunc_init(mf11, m1, f11, f11p, NULL) == PSPIO_SUCCESS);
  meshfunc_compare_values(m1, mf11, f11, f11p, fpp, 1e-10);
}
END_TEST

START_TEST(test_meshfunc_init3)
{
  ck_assert(pspio_meshfunc_init(mf11, m1, f11, f11p, f11pp) == PSPIO_SUCCESS);
  meshfunc_compare_values(m1, mf11, f11, f11p, f11pp, 1e-10);
}
END_TEST

START_TEST(test_meshfunc_cmp_equal)
{
  pspio_meshfunc_init(mf11, m1, f11, f11p, f11pp);
  pspio_meshfunc_init(mf12, m1, f11, f11p, f11pp);
  ck_assert(pspio_meshfunc_cmp(mf11, mf12) == PSPIO_EQUAL);
}
END_TEST

START_TEST(test_meshfunc_cmp_diff_mesh)
{
  pspio_meshfunc_init(mf11, m1, f11, f11p, f11pp);
  pspio_meshfunc_init(mf2, m2, f2, f2p, f2pp);
  ck_assert(pspio_meshfunc_cmp(mf11, mf2) == PSPIO_DIFF);
}
END_TEST

START_TEST(test_meshfunc_cmp_diff_f)
{
  pspio_meshfunc_init(mf11, m1, f11, f11p, f11pp);
  pspio_meshfunc_init(mf12, m1, f12, f11p, f11pp);
  ck_assert(pspio_meshfunc_cmp(mf11, mf12) == PSPIO_DIFF);
}
END_TEST

START_TEST(test_meshfunc_cmp_diff_fp)
{
  pspio_meshfunc_init(mf11, m1, f11, f11p, f11pp);
  pspio_meshfunc_init(mf12, m1, f11, f12p, f11pp);
  ck_assert(pspio_meshfunc_cmp(mf11, mf12) == PSPIO_DIFF);
}
END_TEST

START_TEST(test_meshfunc_cmp_diff_fpp)
{
  pspio_meshfunc_init(mf11, m1, f11, f11p, f11pp);
  pspio_meshfunc_init(mf12, m1, f11, f11p, f12pp);
  ck_assert(pspio_meshfunc_cmp(mf11, mf12) == PSPIO_DIFF);
}
END_TEST

START_TEST(test_meshfunc_copy_null)
{
  pspio_meshfunc_init(mf11, m1, f11, f11p, f11pp);
  pspio_meshfunc_free(mf12);
  mf12 = NULL;
  ck_assert(pspio_meshfunc_copy(&mf12, mf11) == PSPIO_SUCCESS);
  ck_assert(pspio_meshfunc_cmp(mf11, mf12) == PSPIO_EQUAL);
}
END_TEST

START_TEST(test_meshfunc_copy_nonnull)
{
  pspio_meshfunc_init(mf11, m1, f11, f11p, f11pp);
  pspio_meshfunc_init(mf12, m1, f12, f12p, f12pp);
  ck_assert(pspio_meshfunc_copy(&mf12, mf11) == PSPIO_SUCCESS);
  ck_assert(pspio_meshfunc_cmp(mf11, mf12) == PSPIO_EQUAL);
}
END_TEST

START_TEST(test_meshfunc_copy_nonnull_size)
{
  pspio_meshfunc_init(mf11, m1, f11, f11p, f11pp);
  pspio_meshfunc_init(mf2, m2, f2, f2p, f2pp);
  ck_assert(pspio_meshfunc_copy(&mf2, mf11) == PSPIO_SUCCESS);
  ck_assert(pspio_meshfunc_cmp(mf11, mf2) == PSPIO_EQUAL);
}
END_TEST

START_TEST(test_meshfunc_get_func)
{
  int i;
  const double *f;

  pspio_meshfunc_init(mf11, m1, f11, f11p, f11pp);

  f = pspio_meshfunc_get_function(mf11);
  for (i=0; i<pspio_mesh_get_np(m1); i++) {
    ck_assert(f[i] == f11[i]);
  }
}
END_TEST

START_TEST(test_meshfunc_get_deriv1)
{
  int i;
  const double *fp;

  pspio_meshfunc_init(mf11, m1, f11, f11p, f11pp);

  fp = pspio_meshfunc_get_deriv1(mf11);
  for (i=0; i<pspio_mesh_get_np(m1); i++) {
    ck_assert(fp[i] == f11p[i]);
  }
}
END_TEST

START_TEST(test_meshfunc_get_deriv2)
{
  int i;
  const double *fpp;

  pspio_meshfunc_init(mf11, m1, f11, f11p, f11pp);

  fpp = pspio_meshfunc_get_deriv2(mf11);
  for (i=0; i<pspio_mesh_get_np(m1); i++) {
    ck_assert(fpp[i] == f11pp[i]);
  }
}
END_TEST

START_TEST(test_meshfunc_get_interp_method)
{
  pspio_meshfunc_init(mf11, m1, f11, f11p, f11pp);

#ifdef HAVE_GSL
  ck_assert(pspio_meshfunc_get_interp_method(mf11) == PSPIO_INTERP_GSL_CSPLINE);
#else
  ck_assert(pspio_meshfunc_get_interp_method(mf11) == PSPIO_INTERP_JB_CSPLINE);
#endif
}
END_TEST

START_TEST(test_meshfunc_get_mesh)
  {
    pspio_meshfunc_init(mf11, m1, f11, f11p, f11pp);

    ck_assert(pspio_mesh_cmp(pspio_meshfunc_get_mesh(mf11), m1) == PSPIO_EQUAL);
  }
END_TEST

START_TEST(test_meshfunc_eval)
{
  pspio_meshfunc_init(mf11, m1, f12, f12p, f12pp);
  /* Interpolation */
  ck_assert(fabs(pspio_meshfunc_eval(mf11, 0.001) - 2.688989e-08) <= 1e-10);
  /* Extrapolation */
  ck_assert(fabs(pspio_meshfunc_eval(mf11, -0.1) + 2.500000e-04) <= 1e-10);
  ck_assert(fabs(pspio_meshfunc_eval(mf11, 1.1) - 1.257250e+00) <= 1e-10);
}
END_TEST

START_TEST(test_meshfunc_eval_deriv)
{
  pspio_meshfunc_init(mf11, m1, f12, f12p, f12pp);
  /* Interpolation */
  ck_assert(fabs(pspio_meshfunc_eval_deriv(mf11, 0.001) - 8.697106e-05) <= 1e-10);
  /* Extrapolation */
  ck_assert(fabs(pspio_meshfunc_eval_deriv(mf11, -0.1) + 1.500000e-02) <= 1e-10);
  ck_assert(fabs(pspio_meshfunc_eval_deriv(mf11, 1.1) - 3.555000e+00) <= 1e-10);
}
END_TEST

START_TEST(test_meshfunc_eval_deriv2)
{
  pspio_meshfunc_init(mf11, m1, f12, f12p, f12pp);
  /* Interpolation */
  ck_assert(fabs(pspio_meshfunc_eval_deriv2(mf11, 0.001) - 6.000000e-03) <= 1e-10);
  /* Extrapolation */
  ck_assert(fabs(pspio_meshfunc_eval_deriv2(mf11, -0.1) + 6.000000e-01) <= 1e-10);
  ck_assert(fabs(pspio_meshfunc_eval_deriv2(mf11, 1.1) - 6.600000e+00) <= 1e-10);
}
END_TEST


Suite * make_meshfunc_suite(void)
{
  Suite *s;
  TCase *tc_alloc, *tc_init, *tc_cmp, *tc_copy, *tc_get, *tc_eval;

  s = suite_create("Mesh function");

  tc_alloc = tcase_create("Allocation");
  tcase_add_checked_fixture(tc_alloc, NULL, meshfunc_teardown);
  tcase_add_test(tc_alloc, test_meshfunc_alloc);
  suite_add_tcase(s, tc_alloc);

  tc_init = tcase_create("Initialization");
  tcase_add_checked_fixture(tc_init, meshfunc_setup, meshfunc_teardown);
  tcase_add_test(tc_init, test_meshfunc_init1);
  tcase_add_test(tc_init, test_meshfunc_init2);
  tcase_add_test(tc_init, test_meshfunc_init3);
  suite_add_tcase(s, tc_init);

  tc_cmp = tcase_create("Comparison");
  tcase_add_checked_fixture(tc_cmp, meshfunc_setup, meshfunc_teardown);
  tcase_add_test(tc_cmp, test_meshfunc_cmp_equal);
  tcase_add_test(tc_cmp, test_meshfunc_cmp_diff_mesh);
  tcase_add_test(tc_cmp, test_meshfunc_cmp_diff_f);
  tcase_add_test(tc_cmp, test_meshfunc_cmp_diff_fp);
  tcase_add_test(tc_cmp, test_meshfunc_cmp_diff_fpp);
  suite_add_tcase(s, tc_cmp);

  tc_copy = tcase_create("Copy");
  tcase_add_checked_fixture(tc_copy, meshfunc_setup, meshfunc_teardown);
  tcase_add_test(tc_copy, test_meshfunc_copy_null);
  tcase_add_test(tc_copy, test_meshfunc_copy_nonnull);
  tcase_add_test(tc_copy, test_meshfunc_copy_nonnull_size);
  suite_add_tcase(s, tc_copy);

  tc_get = tcase_create("Getters");
  tcase_add_checked_fixture(tc_get, meshfunc_setup, meshfunc_teardown);
  tcase_add_test(tc_get, test_meshfunc_get_func);
  tcase_add_test(tc_get, test_meshfunc_get_deriv1);
  tcase_add_test(tc_get, test_meshfunc_get_deriv2);
  tcase_add_test(tc_get, test_meshfunc_get_interp_method);
  tcase_add_test(tc_get, test_meshfunc_get_mesh);
  suite_add_tcase(s, tc_get);

  tc_eval = tcase_create("Evaluation");
  tcase_add_checked_fixture(tc_eval, meshfunc_setup, meshfunc_teardown);
  tcase_add_test(tc_eval, test_meshfunc_eval);
  tcase_add_test(tc_eval, test_meshfunc_eval_deriv);
  tcase_add_test(tc_eval, test_meshfunc_eval_deriv2);
  suite_add_tcase(s, tc_eval);
    
  return s;
}
