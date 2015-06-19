/*
 Copyright (C) 2011-2012 J. Alberdi, M. Oliveira, Y. Pouillon, and M. Verstraete
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
 * @file check_pspsio_qn.c
 * @brief checks pspio_qn.c and pspio_qn.h 
 */

#include <stdio.h>
#include <check.h>

#include "pspio_qn.h"

pspio_qn_t *qn1 = NULL, *qn2 = NULL; 


void qn_setup(void)
{
  pspio_qn_alloc(&qn1);
  pspio_qn_alloc(&qn2);
  pspio_qn_init(qn1, 1, 2, 0.0);
  pspio_qn_init(qn2, 1, 2, 2.5);
}

void qn_teardown(void)
{
  pspio_qn_free(qn1);
  pspio_qn_free(qn2);
}

START_TEST(test_qn_alloc)
{
  ck_assert(pspio_qn_alloc(&qn1) == PSPIO_SUCCESS);
  ck_assert(pspio_qn_alloc(&qn2) == PSPIO_SUCCESS);
}
END_TEST

START_TEST(test_qn_init)
{
  ck_assert(pspio_qn_init(qn1, 1,  2, 3.0) == PSPIO_EVALUE);
  ck_assert(pspio_qn_init(qn2, 1, -2, 0.0) == PSPIO_EVALUE);

  ck_assert(pspio_qn_init(qn1, 1, 1, 0.5) == PSPIO_SUCCESS);
  ck_assert(pspio_qn_get_n(qn1) == 1);
  ck_assert(pspio_qn_get_l(qn1) == 1);
  ck_assert(pspio_qn_get_j(qn1) == 0.5);

  ck_assert(pspio_qn_init(qn2, 2, 2, 2.5) == PSPIO_SUCCESS);
  ck_assert(pspio_qn_get_n(qn2) == 2);
  ck_assert(pspio_qn_get_l(qn2) == 2);
  ck_assert(pspio_qn_get_j(qn2) == 2.5);
}
END_TEST

START_TEST(test_qn_cmp)
{
  ck_assert(pspio_qn_cmp(qn2, qn1) == PSPIO_QN_DIFF);

  pspio_qn_init(qn2, 2, 2, 0.0);
  ck_assert(pspio_qn_cmp(qn2, qn1) == PSPIO_QN_MTEQUAL);

  pspio_qn_init(qn2, 1, 2, 0.0);
  ck_assert(pspio_qn_cmp(qn2, qn1) == PSPIO_QN_EQUAL);
}
END_TEST

START_TEST(test_qn_copy)
{
  ck_assert(pspio_qn_copy(&qn2, qn1) == PSPIO_SUCCESS);
  ck_assert(pspio_qn_cmp(qn2, qn1) == PSPIO_QN_EQUAL);

  pspio_qn_free(qn2);
  qn2 = NULL;
  ck_assert(pspio_qn_copy(&qn2, qn1) == PSPIO_SUCCESS);
  ck_assert(pspio_qn_cmp(qn2, qn1) == PSPIO_QN_EQUAL);
}
END_TEST

START_TEST(test_qn_labels)
{
  char label[10];

  pspio_qn_label(qn1, label);
  ck_assert_str_eq(label, "1d");

  pspio_qn_label(qn2, label);
  ck_assert_str_eq(label, "1d2.5");
}
END_TEST

Suite * make_qn_suite(void)
{
  Suite *s;
  TCase *tc_alloc, *tc_init, *tc_cmp, *tc_copy, *tc_labels;

  s = suite_create("Quantum numbers");

  tc_alloc = tcase_create("Allocation");
  tcase_add_checked_fixture(tc_alloc, NULL, qn_teardown);
  tcase_add_test(tc_alloc, test_qn_alloc);
  suite_add_tcase(s, tc_alloc);

  tc_init = tcase_create("Initialization");
  tcase_add_checked_fixture(tc_init, qn_setup, qn_teardown);
  tcase_add_test(tc_init, test_qn_init);
  suite_add_tcase(s, tc_init);

  tc_cmp = tcase_create("Comparization");
  tcase_add_checked_fixture(tc_cmp, qn_setup, qn_teardown);
  tcase_add_test(tc_cmp, test_qn_cmp);
  suite_add_tcase(s, tc_cmp);

  tc_copy = tcase_create("Copy");
  tcase_add_checked_fixture(tc_copy, qn_setup, qn_teardown);
  tcase_add_test(tc_copy, test_qn_copy);
  suite_add_tcase(s, tc_copy);

  tc_labels = tcase_create("Labels");
  tcase_add_checked_fixture(tc_labels, qn_setup, qn_teardown);
  tcase_add_test(tc_labels, test_qn_labels);
  suite_add_tcase(s, tc_labels);
    
  return s;
}
