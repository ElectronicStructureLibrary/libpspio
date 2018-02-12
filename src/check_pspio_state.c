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
 * @file check_pspio_state.c
 * @brief checks pspio_state.c and pspio_state.h 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <check.h>

#include "pspio_error.h"
#include "pspio_state.h"


static pspio_mesh_t *m1 = NULL, *m2 = NULL;
static pspio_qn_t *qn11 = NULL, *qn12 = NULL, *qn2 = NULL;
static pspio_state_t *state11 = NULL, *state12 = NULL, *state2 = NULL;

static double e11 = 1.0e-2, e12 = 0.5, e2 = 0.25;
static double occ11 = 2.0, occ12 = 1.0, occ2 = 0.0;
static double rc11 = 1.5, rc12 = 1.0, rc2 = 2.0;
static double *wf11, *wf12, *wf2;


void state_setup(void)
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
  pspio_qn_init(qn11, 2, 0, 0.5);
  pspio_qn_free(qn12);
  pspio_qn_alloc(&qn12);
  pspio_qn_init(qn12, 3, 1, 0.5);
  pspio_qn_free(qn2);
  pspio_qn_alloc(&qn2);
  pspio_qn_init(qn2, 3, 1, 1.5);

  pspio_state_free(state11);
  pspio_state_free(state12);
  pspio_state_free(state2);
  pspio_state_alloc(&state11, 8);
  pspio_state_alloc(&state12, 8);
  pspio_state_alloc(&state2, 6);

  r = pspio_mesh_get_r(m1);
  wf11 = (double *)malloc(8*sizeof(double));
  wf12 = (double *)malloc(8*sizeof(double));
  for (i=0; i<pspio_mesh_get_np(m1); i++) {
    wf11[i] = 20.0*exp(-r[i]*r[i]/10.0);
    wf12[i] = 10.0*exp(-r[i]*r[i]/2.0);
  }

  r = pspio_mesh_get_r(m2);
  wf2 = (double *)malloc(6*sizeof(double));
  for (i=0; i<pspio_mesh_get_np(m2); i++) {
    wf2[i] = 20.0*exp(-r[i]*r[i]/3.0);
  }

}

void state_teardown(void)
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

  pspio_state_free(state11);
  pspio_state_free(state12);
  pspio_state_free(state2);
  state11 = NULL;
  state12 = NULL;
  state2 = NULL;

  free(wf11);
  free(wf12);
  free(wf2);
}

void state_compare_values(const pspio_state_t *state, const double eigenval, const pspio_qn_t *qn, const double occ, 
			  const double rc, const pspio_mesh_t *mesh, const double *wf, const char *label, const double tol)
{
  int i;
  const double *wf2;

  ck_assert( fabs(eigenval - pspio_state_get_ev(state)) < tol );
  ck_assert( pspio_qn_cmp(pspio_state_get_qn(state), qn) == PSPIO_EQUAL );
  ck_assert( fabs(occ - pspio_state_get_occ(state)) < tol );
  ck_assert( fabs(rc - pspio_state_get_rc(state)) < tol );
  ck_assert( pspio_mesh_cmp(pspio_meshfunc_get_mesh(state->wf), mesh) == PSPIO_EQUAL );
  wf2 = pspio_meshfunc_get_function(state->wf);
  for (i=0; i<pspio_mesh_get_np(mesh); i++) {
    ck_assert_msg( fabs(wf[i] - wf2[i]) < tol, "wavefunction at point %i: interp= %16.10e expected= %16.10e\n", i, wf2[i], wf[i]);
  }
  ck_assert_str_eq( label, pspio_state_get_label(state) );
}


START_TEST(test_state_alloc)
{
  ck_assert(pspio_state_alloc(&state11, 3) == PSPIO_SUCCESS);
}
END_TEST

START_TEST(test_state_init)
{
  ck_assert(pspio_state_init(state11, e11, qn11, occ11, rc11, m1, wf11, "1s0.5") == PSPIO_SUCCESS);
  state_compare_values(state11, e11, qn11, occ11, rc11, m1, wf11, "1s0.5", 1e-10);
}
END_TEST

START_TEST(test_state_init_label)
{
  ck_assert(pspio_state_init(state11, e11, qn11, occ11, rc11, m1, wf11, NULL) == PSPIO_SUCCESS);
  state_compare_values(state11, e11, qn11, occ11, rc11, m1, wf11, "2s0.5", 1e-10);
}
END_TEST

START_TEST(test_state_cmp_equal)
{
  ck_assert(pspio_state_init(state11, e11, qn11, occ11, rc11, m1, wf11, NULL) == PSPIO_SUCCESS);
  ck_assert(pspio_state_init(state12, e11, qn11, occ11, rc11, m1, wf11, NULL) == PSPIO_SUCCESS);
  ck_assert(pspio_state_cmp(state11, state12) == PSPIO_EQUAL);
}
END_TEST

START_TEST(test_state_cmp_diff_qn)
{
  ck_assert(pspio_state_init(state11, e11, qn11, occ11, rc11, m1, wf11, NULL) == PSPIO_SUCCESS);
  ck_assert(pspio_state_init(state12, e11, qn12, occ11, rc11, m1, wf11, NULL) == PSPIO_SUCCESS);
  ck_assert(pspio_state_cmp(state11, state12) == PSPIO_DIFF);
}
END_TEST

START_TEST(test_state_cmp_diff_occ)
{
  ck_assert(pspio_state_init(state11, e11, qn11, occ11, rc11, m1, wf11, NULL) == PSPIO_SUCCESS);
  ck_assert(pspio_state_init(state12, e11, qn11, occ12, rc11, m1, wf11, NULL) == PSPIO_SUCCESS);
  ck_assert(pspio_state_cmp(state11, state12) == PSPIO_DIFF);
}
END_TEST

START_TEST(test_state_cmp_diff_eigenval)
{
  ck_assert(pspio_state_init(state11, e11, qn11, occ11, rc11, m1, wf11, NULL) == PSPIO_SUCCESS);
  ck_assert(pspio_state_init(state12, e12, qn11, occ11, rc11, m1, wf11, NULL) == PSPIO_SUCCESS);
  ck_assert(pspio_state_cmp(state11, state12) == PSPIO_DIFF);
}
END_TEST

START_TEST(test_state_cmp_diff_label)
{
  ck_assert(pspio_state_init(state11, e11, qn11, occ11, rc11, m1, wf11, "1s") == PSPIO_SUCCESS);
  ck_assert(pspio_state_init(state12, e11, qn11, occ11, rc11, m1, wf11, "2s") == PSPIO_SUCCESS);
  ck_assert(pspio_state_cmp(state11, state12) == PSPIO_DIFF);
}
END_TEST

START_TEST(test_state_cmp_diff_rc)
{
  ck_assert(pspio_state_init(state11, e11, qn11, occ11, rc11, m1, wf11, NULL) == PSPIO_SUCCESS);
  ck_assert(pspio_state_init(state12, e11, qn11, occ11, rc12, m1, wf11, NULL) == PSPIO_SUCCESS);
  ck_assert(pspio_state_cmp(state11, state12) == PSPIO_DIFF);
}
END_TEST

START_TEST(test_state_cmp_diff_wf)
{
  ck_assert(pspio_state_init(state11, e11, qn11, occ11, rc11, m1, wf11, NULL) == PSPIO_SUCCESS);
  ck_assert(pspio_state_init(state12, e11, qn11, occ11, rc11, m1, wf12, NULL) == PSPIO_SUCCESS);
  ck_assert(pspio_state_cmp(state11, state12) == PSPIO_DIFF);
}
END_TEST

START_TEST(test_state_copy_null)
{
  ck_assert(pspio_state_init(state11, e11, qn11, occ11, rc11, m1, wf11, NULL) == PSPIO_SUCCESS);
  pspio_state_free(state12);
  state12 = NULL;
  ck_assert(pspio_state_copy(&state12, state11) == PSPIO_SUCCESS);
  ck_assert(pspio_state_cmp(state11, state12));
}
END_TEST

START_TEST(test_state_copy_nonnull)
{
  ck_assert(pspio_state_init(state11, e11, qn11, occ11, rc11, m1, wf11, NULL) == PSPIO_SUCCESS);
  ck_assert(pspio_state_init(state12, e12, qn12, occ12, rc12, m1, wf12, NULL) == PSPIO_SUCCESS);
  ck_assert(pspio_state_copy(&state12, state11) == PSPIO_SUCCESS);
  ck_assert(pspio_state_cmp(state11, state12));
}
END_TEST

START_TEST(test_state_copy_nonnull_size)
{
  ck_assert(pspio_state_init(state11, e11, qn11, occ11, rc11, m1, wf11, NULL) == PSPIO_SUCCESS);
  ck_assert(pspio_state_init(state2, e2, qn2, occ2, rc2, m2, wf2, NULL) == PSPIO_SUCCESS);
  ck_assert(pspio_state_copy(&state2, state11) == PSPIO_SUCCESS);
  ck_assert(pspio_state_cmp(state11, state2));
}
END_TEST

START_TEST(test_state_get_label)
{
  ck_assert(pspio_state_init(state11, e11, qn11, occ11, rc11, m1, wf11, NULL) == PSPIO_SUCCESS);
  ck_assert_str_eq(pspio_state_get_label(state11), "2s0.5");
}
END_TEST

START_TEST(test_state_get_qn)
{
  ck_assert(pspio_state_init(state11, e11, qn11, occ11, rc11, m1, wf11, NULL) == PSPIO_SUCCESS);
  ck_assert(pspio_qn_cmp(pspio_state_get_qn(state11), qn11) == PSPIO_EQUAL);
}
END_TEST

START_TEST(test_state_get_occ)
{
  ck_assert(pspio_state_init(state11, e11, qn11, occ11, rc11, m1, wf11, NULL) == PSPIO_SUCCESS);
  ck_assert(pspio_state_get_occ(state11) == occ11);
}
END_TEST

START_TEST(test_state_get_ev)
{
  ck_assert(pspio_state_init(state11, e11, qn11, occ11, rc11, m1, wf11, NULL) == PSPIO_SUCCESS);
  ck_assert(pspio_state_get_ev(state11) == e11);
}
END_TEST

START_TEST(test_state_get_rc)
{
  ck_assert(pspio_state_init(state11, e11, qn11, occ11, rc11, m1, wf11, NULL) == PSPIO_SUCCESS);
  ck_assert(pspio_state_get_rc(state11) == rc11);
}
END_TEST

START_TEST(test_state_lookup_table)
{
  int i;
  const int n_states = 3;
  pspio_state_t **st;
  int **table = NULL;

  st = (pspio_state_t **) malloc (n_states * sizeof(pspio_state_t *));
  memset(st, 0, n_states * sizeof(pspio_state_t *));

  pspio_state_alloc(&(st[0]), 8);
  pspio_state_alloc(&(st[1]), 8);
  pspio_state_alloc(&(st[2]), 6);
  pspio_state_init(st[0], e11, qn11, occ11, rc11, m1, wf11, NULL);
  pspio_state_init(st[1], e12, qn12, occ12, rc12, m1, wf12, NULL);
  pspio_state_init(st[2], e2, qn2, occ2, rc2, m2, wf2, NULL);

  ck_assert( pspio_states_lookup_table(n_states, st, &table) == PSPIO_SUCCESS);
  ck_assert_msg( table[0][0] == -1, "table[%i][%i] = %i, expected = %i\n", 0, 0, table[0][0], -1);
  ck_assert_msg( table[0][1] == -1, "table[%i][%i] = %i, expected = %i\n", 0, 1, table[0][1], -1);
  ck_assert_msg( table[0][2] == -1, "table[%i][%i] = %i, expected = %i\n", 0, 2, table[0][2], -1);
  ck_assert_msg( table[1][0] == -1, "table[%i][%i] = %i, expected = %i\n", 1, 0, table[1][0], -1);
  ck_assert_msg( table[1][1] == -1, "table[%i][%i] = %i, expected = %i\n", 1, 1, table[1][1], -1);
  ck_assert_msg( table[1][2] == -1, "table[%i][%i] = %i, expected = %i\n", 1, 2, table[1][2], -1);
  ck_assert_msg( table[2][0] ==  0, "table[%i][%i] = %i, expected = %i\n", 2, 0, table[2][0],  0);
  ck_assert_msg( table[2][1] == -1, "table[%i][%i] = %i, expected = %i\n", 2, 1, table[2][1], -1);
  ck_assert_msg( table[2][2] == -1, "table[%i][%i] = %i, expected = %i\n", 2, 2, table[2][2], -1);
  ck_assert_msg( table[3][0] == -1, "table[%i][%i] = %i, expected = %i\n", 3, 0, table[3][0], -1);
  ck_assert_msg( table[3][1] ==  1, "table[%i][%i] = %i, expected = %i\n", 3, 1, table[3][1],  1);
  ck_assert_msg( table[3][2] ==  2, "table[%i][%i] = %i, expected = %i\n", 3, 2, table[3][2],  2);

  for (i=0; i<n_states; i++) {
    pspio_state_free(st[i]);
  }
  free(st);

  for (i=0; table[i]!=NULL; i++) {
    free(table[i]);
  }
  free(table);
}
END_TEST

START_TEST(test_state_wf_eval)
{
  double eval, expect;

  pspio_state_init(state11, e11, qn11, occ11, rc11, m1, wf11, NULL);
  eval = pspio_state_wf_eval(state11, 0.01);
  expect = 1.6456049569e+00;
  ck_assert_msg(fabs(eval - expect) <= 1e-10, "wavefunction eval returned= %16.10e expected= %16.10e\n", eval, expect);
}
END_TEST

START_TEST(test_state_wf_eval_deriv)
{
  double eval, expect;

  pspio_state_init(state11, e11, qn11, occ11, rc11, m1, wf11, NULL);
  eval = pspio_state_wf_eval_deriv(state11, 0.01);
  expect = -1.5477352024e-01;
  ck_assert_msg(fabs(eval - expect) <= 1e-10, "wavefunction eval deriv returned= %16.10e expected= %16.10e\n", eval, expect);
}
END_TEST

START_TEST(test_state_wf_eval_deriv2)
{
  double eval, expect;

  pspio_state_init(state11, e11, qn11, occ11, rc11, m1, wf11, NULL);
  eval = pspio_state_wf_eval_deriv2(state11, 0.01);
  expect = -5.8963771072e-03;
  ck_assert_msg(fabs(eval - expect) <= 1e-10, "wavefunction eval deriv2 returned= %16.10e expected= %16.10e\n", eval, expect);
}
END_TEST


Suite * make_state_suite(void)
{
  Suite *s;
  TCase *tc_alloc, *tc_init, *tc_cmp, *tc_copy, *tc_get, *tc_lookup, *tc_eval;

  s = suite_create("State");

  tc_alloc = tcase_create("Allocation");
  tcase_add_checked_fixture(tc_alloc, NULL, state_teardown);
  tcase_add_test(tc_alloc, test_state_alloc);
  suite_add_tcase(s, tc_alloc);

  tc_init = tcase_create("Initialization");
  tcase_add_checked_fixture(tc_init, state_setup, state_teardown);
  tcase_add_test(tc_init, test_state_init);
  tcase_add_test(tc_init, test_state_init_label);
  suite_add_tcase(s, tc_init);

  tc_cmp = tcase_create("Comparison");
  tcase_add_checked_fixture(tc_cmp, state_setup, state_teardown);
  tcase_add_test(tc_cmp, test_state_cmp_equal);
  tcase_add_test(tc_cmp, test_state_cmp_diff_qn);
  tcase_add_test(tc_cmp, test_state_cmp_diff_occ);
  tcase_add_test(tc_cmp, test_state_cmp_diff_eigenval);
  tcase_add_test(tc_cmp, test_state_cmp_diff_label);
  tcase_add_test(tc_cmp, test_state_cmp_diff_rc);
  tcase_add_test(tc_cmp, test_state_cmp_diff_wf);
  suite_add_tcase(s, tc_cmp);

  tc_copy = tcase_create("Copy");
  tcase_add_checked_fixture(tc_copy, state_setup, state_teardown);
  tcase_add_test(tc_copy, test_state_copy_null);
  tcase_add_test(tc_copy, test_state_copy_nonnull);
  tcase_add_test(tc_copy, test_state_copy_nonnull_size);
  suite_add_tcase(s, tc_copy);

  tc_get = tcase_create("Getters");
  tcase_add_checked_fixture(tc_get, state_setup, state_teardown);
  tcase_add_test(tc_get, test_state_get_label);
  tcase_add_test(tc_get, test_state_get_qn);
  tcase_add_test(tc_get, test_state_get_occ);
  tcase_add_test(tc_get, test_state_get_ev);
  tcase_add_test(tc_get, test_state_get_rc);
  suite_add_tcase(s, tc_get);

  tc_lookup = tcase_create("Lookup table");
  tcase_add_checked_fixture(tc_lookup, state_setup, state_teardown);
  tcase_add_test(tc_lookup, test_state_lookup_table);
  suite_add_tcase(s, tc_lookup);

  tc_eval = tcase_create("Evaluation");
  tcase_add_checked_fixture(tc_eval, state_setup, state_teardown);
  tcase_add_test(tc_eval, test_state_wf_eval);
  tcase_add_test(tc_eval, test_state_wf_eval_deriv);
  tcase_add_test(tc_eval, test_state_wf_eval_deriv2);
  suite_add_tcase(s, tc_eval);
    
  return s;
}

