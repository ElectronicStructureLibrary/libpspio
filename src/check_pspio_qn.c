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
 * @file check_pspsio_qn.c
 * @brief checks pspio_qn.c and pspio_qn.h 
 */

#include <check.h>

#include "pspio_qn.h"

static pspio_qn_t *qn1 = NULL, *qn2 = NULL; 


void qn_setup(void)
{
  pspio_qn_alloc(&qn1);
  pspio_qn_alloc(&qn2);
}

void qn_teardown(void)
{
  pspio_qn_free(qn1);
  pspio_qn_free(qn2);
  qn1 = NULL;
  qn2 = NULL;
}

START_TEST(test_qn_alloc)
{
  ck_assert(pspio_qn_alloc(&qn1) == PSPIO_SUCCESS);
}
END_TEST


START_TEST(test_qn_init_nonrel)
{
  pspio_qn_init(qn1, 1, 2, 0.0);
  ck_assert(pspio_qn_get_n(qn1) == 1);
  ck_assert(pspio_qn_get_l(qn1) == 2);
  ck_assert(pspio_qn_get_j(qn1) == 0.0);
}
END_TEST

START_TEST(test_qn_init_relplus)
{
  ck_assert(pspio_qn_init(qn1, 2, 2, 2.5) == PSPIO_SUCCESS);
  ck_assert(pspio_qn_get_n(qn1) == 2);
  ck_assert(pspio_qn_get_l(qn1) == 2);
  ck_assert(pspio_qn_get_j(qn1) == 2.5);
}
END_TEST

START_TEST(test_qn_init_relminus)
{
  ck_assert(pspio_qn_init(qn1, 1, 1, 0.5) == PSPIO_SUCCESS);
  ck_assert(pspio_qn_get_n(qn1) == 1);
  ck_assert(pspio_qn_get_l(qn1) == 1);
  ck_assert(pspio_qn_get_j(qn1) == 0.5);
}
END_TEST

START_TEST(test_qn_init_bad_l)
{
  ck_assert(pspio_qn_init(qn1, 1, -2, 0.0) == PSPIO_EVALUE);
}
END_TEST

START_TEST(test_qn_init_bad_j)
{
  ck_assert(pspio_qn_init(qn1, 1,  2, 3.0) == PSPIO_EVALUE);
}
END_TEST


START_TEST(test_qn_cmp_diff)
{
  pspio_qn_init(qn1, 1, 2, 0.0);
  pspio_qn_init(qn2, 1, 2, 2.5);
  ck_assert(pspio_qn_cmp(qn1, qn2) == PSPIO_DIFF);
}
END_TEST

START_TEST(test_qn_cmp_mtequal)
{
  pspio_qn_init(qn1, 1, 2, 0.0);
  pspio_qn_init(qn2, 2, 2, 0.0);
  ck_assert(pspio_qn_cmp(qn1, qn2) == PSPIO_MTEQUAL);
}
END_TEST

START_TEST(test_qn_cmp_equal)
{
  pspio_qn_init(qn1, 1, 2, 0.0);
  pspio_qn_init(qn2, 1, 2, 0.0);
  ck_assert(pspio_qn_cmp(qn1, qn2) == PSPIO_EQUAL);
}
END_TEST


START_TEST(test_qn_copy_null)
{
  pspio_qn_init(qn1, 1, 2, 0.0);
  pspio_qn_free(qn2);
  qn2 = NULL;

  ck_assert(pspio_qn_copy(&qn2, qn1) == PSPIO_SUCCESS);
  ck_assert(pspio_qn_cmp(qn1, qn2) == PSPIO_EQUAL);
}
END_TEST

START_TEST(test_qn_copy_nonnull)
{
  pspio_qn_init(qn1, 1, 2, 0.0);
  pspio_qn_init(qn2, 1, 2, 2.5);
  ck_assert(pspio_qn_copy(&qn2, qn1) == PSPIO_SUCCESS);
  ck_assert(pspio_qn_cmp(qn1, qn2) == PSPIO_EQUAL);
}
END_TEST

START_TEST(test_qn_get_n)
{
  pspio_qn_init(qn1, 1, 2, 0.0);
  ck_assert(pspio_qn_get_n(qn1) == 1);
}
END_TEST

START_TEST(test_qn_get_l)
{
  pspio_qn_init(qn1, 1, 2, 0.0);
  ck_assert(pspio_qn_get_l(qn1) == 2);
}
END_TEST

START_TEST(test_qn_get_j)
{
  pspio_qn_init(qn1, 1, 2, 0.0);
  ck_assert(pspio_qn_get_j(qn1) == 0.0);
}
END_TEST

START_TEST(test_qn_label_nonrel)
{
  char label[10];

  pspio_qn_init(qn1, 1, 2, 0.0);
  ck_assert(pspio_qn_label(qn1, label) == PSPIO_SUCCESS);
  ck_assert_str_eq(label, "1d");
}
END_TEST

START_TEST(test_qn_label_rel)
{
  char label[10];

  pspio_qn_init(qn1, 1, 2, 2.5);
  ck_assert(pspio_qn_label(qn1, label) == PSPIO_SUCCESS);
  ck_assert_str_eq(label, "1d2.5");
}
END_TEST

Suite * make_qn_suite(void)
{
  Suite *s;
  TCase *tc_alloc, *tc_init, *tc_cmp, *tc_copy, *tc_get, *tc_labels;

  s = suite_create("Quantum numbers");

  tc_alloc = tcase_create("Allocation");
  tcase_add_checked_fixture(tc_alloc, NULL, qn_teardown);
  tcase_add_test(tc_alloc, test_qn_alloc);
  suite_add_tcase(s, tc_alloc);

  tc_init = tcase_create("Initialization");
  tcase_add_checked_fixture(tc_init, qn_setup, qn_teardown);
  tcase_add_test(tc_init, test_qn_init_nonrel);
  tcase_add_test(tc_init, test_qn_init_relplus);
  tcase_add_test(tc_init, test_qn_init_relminus);
  tcase_add_test(tc_init, test_qn_init_bad_l);
  tcase_add_test(tc_init, test_qn_init_bad_j);
  suite_add_tcase(s, tc_init);

  tc_cmp = tcase_create("Comparison");
  tcase_add_checked_fixture(tc_cmp, qn_setup, qn_teardown);
  tcase_add_test(tc_cmp, test_qn_cmp_equal);
  tcase_add_test(tc_cmp, test_qn_cmp_mtequal);
  tcase_add_test(tc_cmp, test_qn_cmp_diff);
  suite_add_tcase(s, tc_cmp);

  tc_copy = tcase_create("Copy");
  tcase_add_checked_fixture(tc_copy, qn_setup, qn_teardown);
  tcase_add_test(tc_copy, test_qn_copy_null);
  tcase_add_test(tc_copy, test_qn_copy_nonnull);
  suite_add_tcase(s, tc_copy);

  tc_get = tcase_create("Getters");
  tcase_add_checked_fixture(tc_get, qn_setup, qn_teardown);
  tcase_add_test(tc_get, test_qn_get_n);
  tcase_add_test(tc_get, test_qn_get_l);
  tcase_add_test(tc_get, test_qn_get_j);
  suite_add_tcase(s, tc_get);

  tc_labels = tcase_create("Labels");
  tcase_add_checked_fixture(tc_labels, qn_setup, qn_teardown);
  tcase_add_test(tc_labels, test_qn_label_nonrel);
  tcase_add_test(tc_labels, test_qn_label_rel);
  suite_add_tcase(s, tc_labels);
    
  return s;
}
