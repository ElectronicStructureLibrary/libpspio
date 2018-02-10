/* Copyright (C) 2016-2017 Micael Oliveira <micael.oliveira@mpsd.mpg.de>
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
 * @file check_pspsio_pspinfo.c
 * @brief checks pspio_pspinfo.c and pspio_pspinfo.h 
 */

#include <check.h>

#include "pspio_error.h"
#include "pspio_pspinfo.h"


static pspio_pspinfo_t *pspinfo1 = NULL, *pspinfo2 = NULL;
static char *author1 = "A. Author", *author2 = "Another Author";
static char *code_name1 = "XPTO", *code_name2 = "DFT code";
static char *code_version1 = "3.4.0", *code_version2 = "1.0";
static int day1 = 1, day2 = 31, month1 = 1, month2 = 12, year1 = 1900, year2 = 2100;
static char *description1 = "Universal alchemical pseudopotential", *description2 = "Minus one over r";


void pspinfo_setup(void)
{
  pspio_pspinfo_alloc(&pspinfo1);
  pspio_pspinfo_alloc(&pspinfo2);
}

void pspinfo_setup_init(void)
{
  pspinfo_setup();

  pspio_pspinfo_set_author(pspinfo1, author1);
  pspio_pspinfo_set_code_name(pspinfo1, code_name1);
  pspio_pspinfo_set_code_version(pspinfo1, code_version1);
  pspio_pspinfo_set_generation_day(pspinfo1, day1);
  pspio_pspinfo_set_generation_month(pspinfo1, month1);
  pspio_pspinfo_set_generation_year(pspinfo1, year1);
  pspio_pspinfo_set_description(pspinfo1, description1);
}

void pspinfo_teardown(void)
{
  pspio_pspinfo_free(pspinfo1);
  pspio_pspinfo_free(pspinfo2);
  pspinfo1 = NULL;
  pspinfo2 = NULL;
}

START_TEST(test_pspinfo_alloc)
{
  ck_assert(pspio_pspinfo_alloc(&pspinfo1) == PSPIO_SUCCESS);
}
END_TEST

START_TEST(test_pspinfo_setget_author)
{
  ck_assert_str_eq(pspio_pspinfo_get_author(pspinfo1), "Unknown");
  ck_assert(pspio_pspinfo_set_author(pspinfo1, author1) == PSPIO_SUCCESS);
  ck_assert_str_eq(pspio_pspinfo_get_author(pspinfo1), author1);
}
END_TEST

START_TEST(test_pspinfo_setget_code_name)
{
  ck_assert_str_eq(pspio_pspinfo_get_code_name(pspinfo1), "Unknown");
  ck_assert(pspio_pspinfo_set_code_name(pspinfo1, code_name1) == PSPIO_SUCCESS);
  ck_assert_str_eq(pspio_pspinfo_get_code_name(pspinfo1), code_name1);
}
END_TEST

START_TEST(test_pspinfo_setget_code_version)
{
  ck_assert(pspio_pspinfo_set_code_version(pspinfo1, code_version1) == PSPIO_SUCCESS);
  ck_assert_str_eq(pspio_pspinfo_get_code_version(pspinfo1), code_version1);
}
END_TEST

START_TEST(test_pspinfo_setget_generation_day)
{
  ck_assert_int_eq(pspio_pspinfo_get_generation_day(pspinfo1), 1);
  ck_assert(pspio_pspinfo_set_generation_day(pspinfo1, day1) == PSPIO_SUCCESS);
  ck_assert_int_eq(pspio_pspinfo_get_generation_day(pspinfo1), day1);
}
END_TEST

START_TEST(test_pspinfo_setget_generation_month)
{
  ck_assert_int_eq(pspio_pspinfo_get_generation_month(pspinfo1), 1);
  ck_assert(pspio_pspinfo_set_generation_month(pspinfo1, month1) == PSPIO_SUCCESS);
  ck_assert_int_eq(pspio_pspinfo_get_generation_month(pspinfo1), month1);
}
END_TEST

START_TEST(test_pspinfo_setget_generation_year)
{
  ck_assert_int_eq(pspio_pspinfo_get_generation_year(pspinfo1), 1900);
  ck_assert(pspio_pspinfo_set_generation_year(pspinfo1, year1) == PSPIO_SUCCESS);
  ck_assert_int_eq(pspio_pspinfo_get_generation_year(pspinfo1), year1);
}
END_TEST

START_TEST(test_pspinfo_setget_description)
{
  ck_assert_str_eq(pspio_pspinfo_get_description(pspinfo1), "");
  ck_assert(pspio_pspinfo_set_description(pspinfo1, description1) == PSPIO_SUCCESS);
  ck_assert_str_eq(pspio_pspinfo_get_description(pspinfo1), description1);
}
END_TEST

START_TEST(test_pspinfo_copy_null)
{
  pspio_pspinfo_free(pspinfo2);
  pspinfo2 = NULL;

  ck_assert(pspio_pspinfo_copy(&pspinfo2, pspinfo1) == PSPIO_SUCCESS);
  ck_assert(pspio_pspinfo_cmp(pspinfo1, pspinfo2) == PSPIO_EQUAL);
}
END_TEST

START_TEST(test_pspinfo_copy_nonnull)
{
  ck_assert(pspio_pspinfo_copy(&pspinfo2, pspinfo1) == PSPIO_SUCCESS);
  ck_assert(pspio_pspinfo_cmp(pspinfo1, pspinfo2) == PSPIO_EQUAL);
}
END_TEST

START_TEST(test_pspinfo_cmp_equal)
{
  ck_assert(pspio_pspinfo_copy(&pspinfo2, pspinfo1) == PSPIO_SUCCESS);
  ck_assert(pspio_pspinfo_cmp(pspinfo1, pspinfo2) == PSPIO_EQUAL);
}
END_TEST

START_TEST(test_pspinfo_cmp_diff_author)
{
  ck_assert(pspio_pspinfo_copy(&pspinfo2, pspinfo1) == PSPIO_SUCCESS);
  ck_assert(pspio_pspinfo_set_author(pspinfo2, author2) == PSPIO_SUCCESS);
  ck_assert(pspio_pspinfo_cmp(pspinfo1, pspinfo2) == PSPIO_DIFF);
}
END_TEST

START_TEST(test_pspinfo_cmp_diff_code_name)
{
  ck_assert(pspio_pspinfo_copy(&pspinfo2, pspinfo1) == PSPIO_SUCCESS);
  ck_assert(pspio_pspinfo_set_code_name(pspinfo2, code_name2) == PSPIO_SUCCESS);
  ck_assert(pspio_pspinfo_cmp(pspinfo1, pspinfo2) == PSPIO_DIFF);
}
END_TEST

START_TEST(test_pspinfo_cmp_diff_code_version)
{
  ck_assert(pspio_pspinfo_copy(&pspinfo2, pspinfo1) == PSPIO_SUCCESS);
  ck_assert(pspio_pspinfo_set_code_version(pspinfo2, code_version2) == PSPIO_SUCCESS);
  ck_assert(pspio_pspinfo_cmp(pspinfo1, pspinfo2) == PSPIO_DIFF);
}
END_TEST

START_TEST(test_pspinfo_cmp_diff_generation_day)
{
  ck_assert(pspio_pspinfo_copy(&pspinfo2, pspinfo1) == PSPIO_SUCCESS);
  ck_assert(pspio_pspinfo_set_generation_day(pspinfo2, day2) == PSPIO_SUCCESS);
  ck_assert(pspio_pspinfo_cmp(pspinfo1, pspinfo2) == PSPIO_DIFF);
}
END_TEST

START_TEST(test_pspinfo_cmp_diff_generation_month)
{
  ck_assert(pspio_pspinfo_copy(&pspinfo2, pspinfo1) == PSPIO_SUCCESS);
  ck_assert(pspio_pspinfo_set_generation_month(pspinfo2, month2) == PSPIO_SUCCESS);
  ck_assert(pspio_pspinfo_cmp(pspinfo1, pspinfo2) == PSPIO_DIFF);
}
END_TEST

START_TEST(test_pspinfo_cmp_diff_generation_year)
{
  ck_assert(pspio_pspinfo_copy(&pspinfo2, pspinfo1) == PSPIO_SUCCESS);
  ck_assert(pspio_pspinfo_set_generation_year(pspinfo2, year2) == PSPIO_SUCCESS);
  ck_assert(pspio_pspinfo_cmp(pspinfo1, pspinfo2) == PSPIO_DIFF);
}
END_TEST

START_TEST(test_pspinfo_cmp_diff_description)
{
  ck_assert(pspio_pspinfo_copy(&pspinfo2, pspinfo1) == PSPIO_SUCCESS);
  ck_assert(pspio_pspinfo_set_description(pspinfo2, description2) == PSPIO_SUCCESS);
  ck_assert(pspio_pspinfo_cmp(pspinfo1, pspinfo2) == PSPIO_DIFF);
}
END_TEST


Suite * make_pspinfo_suite(void)
{
  Suite *s;
  TCase *tc_alloc, *tc_cmp, *tc_copy, *tc_setget;

  s = suite_create("Pseudopotential information");

  tc_alloc = tcase_create("Allocation");
  tcase_add_checked_fixture(tc_alloc, NULL, pspinfo_teardown);
  tcase_add_test(tc_alloc, test_pspinfo_alloc);
  suite_add_tcase(s, tc_alloc);

  tc_setget = tcase_create("Setters and getters");
  tcase_add_checked_fixture(tc_setget, pspinfo_setup, pspinfo_teardown);
  tcase_add_test(tc_setget, test_pspinfo_setget_author);
  tcase_add_test(tc_setget, test_pspinfo_setget_code_name);
  tcase_add_test(tc_setget, test_pspinfo_setget_code_version);
  tcase_add_test(tc_setget, test_pspinfo_setget_generation_day);
  tcase_add_test(tc_setget, test_pspinfo_setget_generation_month);
  tcase_add_test(tc_setget, test_pspinfo_setget_generation_year);
  tcase_add_test(tc_setget, test_pspinfo_setget_description);
  suite_add_tcase(s, tc_setget);

  tc_copy = tcase_create("Copy");
  tcase_add_checked_fixture(tc_copy, pspinfo_setup_init, pspinfo_teardown);
  tcase_add_test(tc_copy, test_pspinfo_copy_null);
  tcase_add_test(tc_copy, test_pspinfo_copy_nonnull);
  suite_add_tcase(s, tc_copy);

  tc_cmp = tcase_create("Comparison");
  tcase_add_checked_fixture(tc_cmp, pspinfo_setup_init, pspinfo_teardown);
  tcase_add_test(tc_cmp, test_pspinfo_cmp_equal);
  tcase_add_test(tc_cmp, test_pspinfo_cmp_diff_author);
  tcase_add_test(tc_cmp, test_pspinfo_cmp_diff_code_name);
  tcase_add_test(tc_cmp, test_pspinfo_cmp_diff_code_version);
  tcase_add_test(tc_cmp, test_pspinfo_cmp_diff_generation_day);
  tcase_add_test(tc_cmp, test_pspinfo_cmp_diff_generation_month);
  tcase_add_test(tc_cmp, test_pspinfo_cmp_diff_generation_year);
  tcase_add_test(tc_cmp, test_pspinfo_cmp_diff_description);
  suite_add_tcase(s, tc_cmp);

  return s;
}
