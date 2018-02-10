/* Copyright (C) 2015-2017 Micael Oliveira <micael.oliveira@mpsd.mpg.de>
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
 * @file check_pspio_xc.c
 * @brief checks pspio_xc.c and pspio_xc.h 
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <check.h>

#include "pspio_error.h"
#include "pspio_xc.h"


static pspio_mesh_t *m1 = NULL, *m2 = NULL;
static pspio_xc_t *xc11 = NULL, *xc12 = NULL, *xc2 = NULL;

static double *cd11, *cd12, *cd2;

static int xid11 = 1, xid12 = 2, xid2 = 3;
static int cid11 = 4, cid12 = 5, cid2 = 6;
static int nlcc11 = PSPIO_NLCC_FHI, nlcc12 = PSPIO_NLCC_LOUIE, nlcc2 = PSPIO_NLCC_TETER1;
static double nlccpfv11 = 1.0, nlccpfv12 = 2.0, nlccpfv2 = 3.0;
static double nlccpfs11 = 4.0, nlccpfs12 = 5.0, nlccpfs2 = 6.0;


void xc_setup(void)
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

  pspio_xc_free(xc11);
  pspio_xc_free(xc12);
  pspio_xc_free(xc2);
  pspio_xc_alloc(&xc11);
  pspio_xc_alloc(&xc12);
  pspio_xc_alloc(&xc2);

  r = pspio_mesh_get_r(m1);
  cd11 = (double *)malloc(8*sizeof(double));
  cd12 = (double *)malloc(8*sizeof(double));
  for (i=0; i<pspio_mesh_get_np(m1); i++) {
    cd11[i] = 20.0*exp(-r[i]*r[i]/10.0);
    cd12[i] = 10.0*exp(-r[i]*r[i]/2.0);
  }

  r = pspio_mesh_get_r(m2);
  cd2 = (double *)malloc(6*sizeof(double));
  for (i=0; i<pspio_mesh_get_np(m2); i++) {
    cd2[i] = 20.0*exp(-r[i]*r[i]/3.0);
  }

}

void xc_teardown(void)
{
  pspio_mesh_free(m1);
  pspio_mesh_free(m2);
  m1 = NULL;
  m2 = NULL;

  pspio_xc_free(xc11);
  pspio_xc_free(xc12);
  pspio_xc_free(xc2);
  xc11 = NULL;
  xc12 = NULL;
  xc2 = NULL;

  free(cd11);
  free(cd12);
  free(cd2);
}

void xc_compare_values(const pspio_xc_t *xc,
  const int exchange, const int correlation,
  const int nlcc_scheme, const int nlcc_pfs, const int nlcc_pfv,
  const pspio_mesh_t *mesh, const double *cd, const double tol)
{
  int i;
  const double *cdp;

  ck_assert( pspio_xc_get_exchange(xc) == exchange );
  ck_assert( pspio_xc_get_correlation(xc) == correlation );
  ck_assert( pspio_xc_get_nlcc_scheme(xc) == nlcc_scheme );
  ck_assert( pspio_xc_get_nlcc_pf_scale(xc) == nlcc_pfs );
  ck_assert( pspio_xc_get_nlcc_pf_value(xc) == nlcc_pfv );
  ck_assert( pspio_mesh_cmp(pspio_meshfunc_get_mesh(pspio_xc_get_nlcc_density(xc)), mesh) == PSPIO_EQUAL );
  cdp = pspio_meshfunc_get_function(pspio_xc_get_nlcc_density(xc));
  for (i=0; i<pspio_mesh_get_np(mesh); i++) {
    ck_assert_msg( fabs(cd[i] - cdp[i]) < tol, "nlcc density at point %i: interp= %16.10e expected= %16.10e\n", i, cdp[i], cd[i]);
  }
}


START_TEST(test_xc_alloc)
{
  ck_assert(pspio_xc_alloc(&xc11) == PSPIO_SUCCESS);
}
END_TEST

START_TEST(test_xc_setget_exchange)
{
  ck_assert(pspio_xc_set_exchange(xc11, xid11) == PSPIO_SUCCESS);
  ck_assert(pspio_xc_get_exchange(xc11) == xid11);
}
END_TEST

START_TEST(test_xc_setget_correlation)
{
  ck_assert(pspio_xc_set_correlation(xc11, cid11) == PSPIO_SUCCESS);
  ck_assert(pspio_xc_get_correlation(xc11) == cid11);
}
END_TEST

START_TEST(test_xc_setget_nlcc_scheme)
{
  ck_assert(pspio_xc_set_nlcc_scheme(xc11, -1000) == PSPIO_EVALUE);
  ck_assert(pspio_xc_set_nlcc_scheme(xc11, nlcc11) == PSPIO_SUCCESS);
  ck_assert(pspio_xc_get_nlcc_scheme(xc11) == nlcc11);
}
END_TEST

START_TEST(test_xc_setget_nlcc_density)
{
  int i;
  const double *cd;

  ck_assert(pspio_xc_set_nlcc_scheme(xc11, PSPIO_NLCC_UNKNOWN) == PSPIO_SUCCESS);
  ck_assert(pspio_xc_set_nlcc_density(xc11, m1, cd11, NULL, NULL) == PSPIO_SUCCESS);
  cd = pspio_meshfunc_get_function(pspio_xc_get_nlcc_density(xc11));
  for (i=0; i<pspio_mesh_get_np(m1); i++) {
    ck_assert( fabs(cd[i] - cd11[i]) < 1e-10);
  }
}
END_TEST

START_TEST(test_xc_init)
{
  ck_assert(pspio_xc_init(xc11, xid11, cid11, nlcc11, nlccpfs11, nlccpfv11, m1, cd11, NULL, NULL) == PSPIO_SUCCESS);
  xc_compare_values(xc11, xid11, cid11, nlcc11, nlccpfs11, nlccpfv11, m1, cd11, 1e-10);
}
END_TEST

START_TEST(test_xc_cmp_equal)
{
  ck_assert(pspio_xc_init(xc11, xid11, cid11, nlcc11, nlccpfs11, nlccpfv11, m1, cd11, NULL, NULL) == PSPIO_SUCCESS);
  ck_assert(pspio_xc_init(xc12, xid11, cid11, nlcc11, nlccpfs11, nlccpfv11, m1, cd11, NULL, NULL) == PSPIO_SUCCESS);
  ck_assert(pspio_xc_cmp(xc11, xc12) == PSPIO_EQUAL);
}
END_TEST

START_TEST(test_xc_cmp_diff_xid)
{
  ck_assert(pspio_xc_init(xc11, xid11, cid11, nlcc11, nlccpfs11, nlccpfv11, m1, cd11, NULL, NULL) == PSPIO_SUCCESS);
  ck_assert(pspio_xc_init(xc12, xid12, cid11, nlcc11, nlccpfs11, nlccpfv11, m1, cd11, NULL, NULL) == PSPIO_SUCCESS);
  ck_assert(pspio_xc_cmp(xc11, xc12) == PSPIO_DIFF);
}
END_TEST

START_TEST(test_xc_cmp_diff_cid)
  {
    ck_assert(pspio_xc_init(xc11, xid11, cid11, nlcc11, nlccpfs11, nlccpfv11, m1, cd11, NULL, NULL) == PSPIO_SUCCESS);
    ck_assert(pspio_xc_init(xc12, xid11, cid12, nlcc11, nlccpfs11, nlccpfv11, m1, cd11, NULL, NULL) == PSPIO_SUCCESS);
    ck_assert(pspio_xc_cmp(xc11, xc12) == PSPIO_DIFF);
  }
END_TEST

START_TEST(test_xc_cmp_diff_nlcc_scheme)
  {
    ck_assert(pspio_xc_init(xc11, xid11, cid11, nlcc11, nlccpfs11, nlccpfv11, m1, cd11, NULL, NULL) == PSPIO_SUCCESS);
    ck_assert(pspio_xc_init(xc12, xid11, cid11, nlcc12, nlccpfs12, nlccpfv12, m1, cd11, NULL, NULL) == PSPIO_SUCCESS);
    ck_assert(pspio_xc_cmp(xc11, xc12) == PSPIO_DIFF);
  }
END_TEST

START_TEST(test_xc_cmp_diff_nlcc_density)
  {
    ck_assert(pspio_xc_init(xc11, xid11, cid11, nlcc11, nlccpfs11, nlccpfv11, m1, cd11, NULL, NULL) == PSPIO_SUCCESS);
    ck_assert(pspio_xc_init(xc12, xid11, cid11, nlcc11, nlccpfs11, nlccpfv11, m1, cd12, NULL, NULL) == PSPIO_SUCCESS);
    ck_assert(pspio_xc_cmp(xc11, xc12) == PSPIO_DIFF);
  }
END_TEST

START_TEST(test_xc_copy_null)
{
  ck_assert(pspio_xc_init(xc11, xid11, cid11, nlcc11, nlccpfs11, nlccpfv11, m1, cd11, NULL, NULL) == PSPIO_SUCCESS);
  pspio_xc_free(xc12);
  xc12 = NULL;
  ck_assert(pspio_xc_copy(&xc12, xc11) == PSPIO_SUCCESS);
  ck_assert(pspio_xc_cmp(xc11, xc12));
}
END_TEST

START_TEST(test_xc_copy_nonnull)
{
  ck_assert(pspio_xc_init(xc11, xid11, cid11, nlcc11, nlccpfs11, nlccpfv11, m1, cd11, NULL, NULL) == PSPIO_SUCCESS);
  ck_assert(pspio_xc_init(xc12, xid12, cid12, nlcc12, nlccpfs12, nlccpfv12, m1, cd12, NULL, NULL) == PSPIO_SUCCESS);
  ck_assert(pspio_xc_copy(&xc12, xc11) == PSPIO_SUCCESS);
  ck_assert(pspio_xc_cmp(xc11, xc12));
}
END_TEST

START_TEST(test_xc_copy_nonnull_size)
{
  ck_assert(pspio_xc_init(xc11, xid11, cid11, nlcc11, nlccpfs11, nlccpfv11, m1, cd11, NULL, NULL) == PSPIO_SUCCESS);
  ck_assert(pspio_xc_init(xc2, xid2, cid2, nlcc2, nlccpfs2, nlccpfv2, m2, cd2, NULL, NULL) == PSPIO_SUCCESS);
  ck_assert(pspio_xc_copy(&xc2, xc11) == PSPIO_SUCCESS);
  ck_assert(pspio_xc_cmp(xc11, xc2));
}
END_TEST

START_TEST(test_xc_nlcc_density_eval)
{
  double eval, expect;

  pspio_xc_init(xc11, xid11, cid11, nlcc11, nlccpfs11, nlccpfv11, m1, cd11, NULL, NULL);
  eval = pspio_xc_nlcc_density_eval(xc11, 0.01);
  expect = 1.6456049569e+00;
  ck_assert_msg(fabs(eval - expect) <= 1e-10, "nlcc density eval returned= %16.10e expected= %16.10e\n", eval, expect);
}
END_TEST

START_TEST(test_xc_nlcc_density_eval_deriv)
{
  double eval, expect;

  pspio_xc_init(xc11, xid11, cid11, nlcc11, nlccpfs11, nlccpfv11, m1, cd11, NULL, NULL);
  eval = pspio_xc_nlcc_density_eval_deriv(xc11, 0.01);
  expect = -1.5477352024e-01;
  ck_assert_msg(fabs(eval - expect) <= 1e-10, "nlcc density eval deriv returned= %16.10e expected= %16.10e\n", eval, expect);
}
END_TEST

START_TEST(test_xc_nlcc_density_eval_deriv2)
{
  double eval, expect;

  pspio_xc_init(xc11, xid11, cid11, nlcc11, nlccpfs11, nlccpfv11, m1, cd11, NULL, NULL);
  eval = pspio_xc_nlcc_density_eval_deriv2(xc11, 0.01);
  expect = -5.8963771072e-03;
  ck_assert_msg(fabs(eval - expect) <= 1e-10, "nlcc density eval deriv2 returned= %16.10e expected= %16.10e\n", eval, expect);
}
END_TEST


Suite * make_xc_suite(void)
{
  Suite *s;
  TCase *tc_alloc, *tc_setget, *tc_init, *tc_cmp, *tc_copy, *tc_eval;

  s = suite_create("XC");

  tc_alloc = tcase_create("Allocation");
  tcase_add_checked_fixture(tc_alloc, NULL, xc_teardown);
  tcase_add_test(tc_alloc, test_xc_alloc);
  suite_add_tcase(s, tc_alloc);

  tc_setget = tcase_create("Setters and getters");
  tcase_add_checked_fixture(tc_setget, xc_setup, xc_teardown);
  tcase_add_test(tc_setget, test_xc_setget_exchange);
  tcase_add_test(tc_setget, test_xc_setget_correlation);
  tcase_add_test(tc_setget, test_xc_setget_nlcc_scheme);
  tcase_add_test(tc_setget, test_xc_setget_nlcc_density);
  suite_add_tcase(s, tc_setget);

  tc_init = tcase_create("Initialization");
  tcase_add_checked_fixture(tc_init, xc_setup, xc_teardown);
  tcase_add_test(tc_init, test_xc_init);
  suite_add_tcase(s, tc_init);

  tc_cmp = tcase_create("Comparison");
  tcase_add_checked_fixture(tc_cmp, xc_setup, xc_teardown);
  tcase_add_test(tc_cmp, test_xc_cmp_equal);
  tcase_add_test(tc_cmp, test_xc_cmp_diff_xid);
  tcase_add_test(tc_cmp, test_xc_cmp_diff_cid);
  tcase_add_test(tc_cmp, test_xc_cmp_diff_nlcc_scheme);
  tcase_add_test(tc_cmp, test_xc_cmp_diff_nlcc_density);
  suite_add_tcase(s, tc_cmp);

  tc_copy = tcase_create("Copy");
  tcase_add_checked_fixture(tc_copy, xc_setup, xc_teardown);
  tcase_add_test(tc_copy, test_xc_copy_null);
  tcase_add_test(tc_copy, test_xc_copy_nonnull);
  tcase_add_test(tc_copy, test_xc_copy_nonnull_size);
  suite_add_tcase(s, tc_copy);

  tc_eval = tcase_create("Evaluation");
  tcase_add_checked_fixture(tc_eval, xc_setup, xc_teardown);
  tcase_add_test(tc_eval, test_xc_nlcc_density_eval);
  tcase_add_test(tc_eval, test_xc_nlcc_density_eval_deriv);
  tcase_add_test(tc_eval, test_xc_nlcc_density_eval_deriv2);
  suite_add_tcase(s, tc_eval);
    
  return s;
}

