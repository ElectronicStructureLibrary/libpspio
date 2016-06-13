/* Copyright (C) 2016 Micael Oliveira <micael.oliveira@mpsd.mpg.de>
 *                    Yann Pouillon <notifications@materialsevolution.es>
 *
 * This file is part of Libpspio.
 *
 * Libpspio is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, version 3 of the License, or (at your option) any later
 * version.
 *
 * Libpspio is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Libpspio.  If not, see <http://www.gnu.org/licenses/> or write to
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301  USA.
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
static char *code1 = "XPTO", *code2 = "DFT code";
static char *date1 = "99/99/99", *date2 = "00/00/00";
static char *description1 = "Universal alchemical pseudopotential", *description2 = "Minus one over r";
static char *scheme_name1 = "Hamann scheme", *scheme_name2 = "Troullier-Martins scheme";


void pspinfo_setup(void)
{
  pspio_pspinfo_alloc(&pspinfo1);
  pspio_pspinfo_alloc(&pspinfo2);
}

void pspinfo_teardown(void)
{
  pspio_pspinfo_free(pspinfo1);
  pspio_pspinfo_free(pspinfo2);
  pspinfo1 = NULL;
  pspinfo2 = NULL;
}

void pspinfo_compare_values(pspio_pspinfo_t *pspinfo, const char *author, const char *code, const char *date,
                            const char *description, const char *scheme_name)
{
  ck_assert_str_eq(pspio_pspinfo_get_author(pspinfo), author);
  ck_assert_str_eq(pspio_pspinfo_get_code(pspinfo), code);
  ck_assert_str_eq(pspio_pspinfo_get_date(pspinfo), date);
  ck_assert_str_eq(pspio_pspinfo_get_description(pspinfo), description);
  ck_assert_str_eq(pspio_pspinfo_get_scheme_name(pspinfo), scheme_name);
}


START_TEST(test_pspinfo_alloc)
{
  ck_assert(pspio_pspinfo_alloc(&pspinfo1) == PSPIO_SUCCESS);
}
END_TEST

START_TEST(test_pspinfo_init)
{
  ck_assert(pspio_pspinfo_init(pspinfo1, author1, code1, date1, description1, scheme_name1) == PSPIO_SUCCESS);
  pspinfo_compare_values(pspinfo1, author1, code1, date1, description1, scheme_name1);
}
END_TEST

START_TEST(test_pspinfo_cmp_equal)
{
  ck_assert(pspio_pspinfo_init(pspinfo1, author1, code1, date1, description1, scheme_name1) == PSPIO_SUCCESS);
  ck_assert(pspio_pspinfo_init(pspinfo2, author1, code1, date1, description1, scheme_name1) == PSPIO_SUCCESS);
  ck_assert(pspio_pspinfo_cmp(pspinfo1, pspinfo2) == PSPIO_EQUAL);
}
END_TEST

START_TEST(test_pspinfo_cmp_diff_author)
{
  ck_assert(pspio_pspinfo_init(pspinfo1, author1, code1, date1, description1, scheme_name1) == PSPIO_SUCCESS);
  ck_assert(pspio_pspinfo_init(pspinfo2, author2, code1, date1, description1, scheme_name1) == PSPIO_SUCCESS);
  ck_assert(pspio_pspinfo_cmp(pspinfo1, pspinfo2) == PSPIO_DIFF);
}
END_TEST

START_TEST(test_pspinfo_cmp_diff_code)
{
  ck_assert(pspio_pspinfo_init(pspinfo1, author1, code1, date1, description1, scheme_name1) == PSPIO_SUCCESS);
  ck_assert(pspio_pspinfo_init(pspinfo2, author1, code2, date1, description1, scheme_name1) == PSPIO_SUCCESS);
  ck_assert(pspio_pspinfo_cmp(pspinfo1, pspinfo2) == PSPIO_DIFF);
}
END_TEST

START_TEST(test_pspinfo_cmp_diff_date)
{
  ck_assert(pspio_pspinfo_init(pspinfo1, author1, code1, date1, description1, scheme_name1) == PSPIO_SUCCESS);
  ck_assert(pspio_pspinfo_init(pspinfo2, author1, code1, date2, description1, scheme_name1) == PSPIO_SUCCESS);
  ck_assert(pspio_pspinfo_cmp(pspinfo1, pspinfo2) == PSPIO_DIFF);
}
END_TEST

START_TEST(test_pspinfo_cmp_diff_description)
{
  ck_assert(pspio_pspinfo_init(pspinfo1, author1, code1, date1, description1, scheme_name1) == PSPIO_SUCCESS);
  ck_assert(pspio_pspinfo_init(pspinfo2, author1, code1, date1, description2, scheme_name1) == PSPIO_SUCCESS);
  ck_assert(pspio_pspinfo_cmp(pspinfo1, pspinfo2) == PSPIO_DIFF);
}
END_TEST

START_TEST(test_pspinfo_cmp_diff_scheme_name)
{
  ck_assert(pspio_pspinfo_init(pspinfo1, author1, code1, date1, description1, scheme_name1) == PSPIO_SUCCESS);
  ck_assert(pspio_pspinfo_init(pspinfo2, author1, code1, date1, description1, scheme_name2) == PSPIO_SUCCESS);
  ck_assert(pspio_pspinfo_cmp(pspinfo1, pspinfo2) == PSPIO_DIFF);
}
END_TEST


START_TEST(test_pspinfo_copy_null)
{
  ck_assert(pspio_pspinfo_init(pspinfo1, author1, code1, date1, description1, scheme_name1) == PSPIO_SUCCESS);
  pspio_pspinfo_free(pspinfo2);
  pspinfo2 = NULL;

  ck_assert(pspio_pspinfo_copy(&pspinfo2, pspinfo1) == PSPIO_SUCCESS);
  ck_assert(pspio_pspinfo_cmp(pspinfo1, pspinfo2) == PSPIO_EQUAL);
}
END_TEST

START_TEST(test_pspinfo_copy_nonnull)
{
  ck_assert(pspio_pspinfo_init(pspinfo1, author1, code1, date1, description1, scheme_name1) == PSPIO_SUCCESS);
  ck_assert(pspio_pspinfo_init(pspinfo2, author2, code2, date2, description2, scheme_name2) == PSPIO_SUCCESS);
  ck_assert(pspio_pspinfo_copy(&pspinfo2, pspinfo1) == PSPIO_SUCCESS);
  ck_assert(pspio_pspinfo_cmp(pspinfo1, pspinfo2) == PSPIO_EQUAL);
}
END_TEST

START_TEST(test_pspinfo_setget_author)
{
  ck_assert(pspio_pspinfo_set_author(pspinfo1, author1) == PSPIO_SUCCESS);
  ck_assert_str_eq(pspio_pspinfo_get_author(pspinfo1), author1);
}
END_TEST

START_TEST(test_pspinfo_setget_code)
{
  ck_assert(pspio_pspinfo_set_code(pspinfo1, code1) == PSPIO_SUCCESS);
  ck_assert_str_eq(pspio_pspinfo_get_code(pspinfo1), code1);
}
END_TEST

START_TEST(test_pspinfo_setget_date)
{
  ck_assert(pspio_pspinfo_set_date(pspinfo1, date1) == PSPIO_SUCCESS);
  ck_assert_str_eq(pspio_pspinfo_get_date(pspinfo1), date1);
}
END_TEST

START_TEST(test_pspinfo_setget_description)
{
  ck_assert(pspio_pspinfo_set_description(pspinfo1, description1) == PSPIO_SUCCESS);
  ck_assert_str_eq(pspio_pspinfo_get_description(pspinfo1), description1);
}
END_TEST

START_TEST(test_pspinfo_setget_scheme_name)
{
  ck_assert(pspio_pspinfo_set_scheme_name(pspinfo1, scheme_name1) == PSPIO_SUCCESS);
  ck_assert_str_eq(pspio_pspinfo_get_scheme_name(pspinfo1), scheme_name1);
}
END_TEST


Suite * make_pspinfo_suite(void)
{
  Suite *s;
  TCase *tc_alloc, *tc_init, *tc_cmp, *tc_copy, *tc_setget;

  s = suite_create("Pseudopotential information");

  tc_alloc = tcase_create("Allocation");
  tcase_add_checked_fixture(tc_alloc, NULL, pspinfo_teardown);
  tcase_add_test(tc_alloc, test_pspinfo_alloc);
  suite_add_tcase(s, tc_alloc);

  tc_init = tcase_create("Initialization");
  tcase_add_checked_fixture(tc_init, pspinfo_setup, pspinfo_teardown);
  tcase_add_test(tc_init, test_pspinfo_init);
  suite_add_tcase(s, tc_init);

  tc_cmp = tcase_create("Comparison");
  tcase_add_checked_fixture(tc_cmp, pspinfo_setup, pspinfo_teardown);
  tcase_add_test(tc_cmp, test_pspinfo_cmp_equal);
  tcase_add_test(tc_cmp, test_pspinfo_cmp_diff_author);
  tcase_add_test(tc_cmp, test_pspinfo_cmp_diff_code);
  tcase_add_test(tc_cmp, test_pspinfo_cmp_diff_date);
  tcase_add_test(tc_cmp, test_pspinfo_cmp_diff_description);
  tcase_add_test(tc_cmp, test_pspinfo_cmp_diff_scheme_name);
  suite_add_tcase(s, tc_cmp);

  tc_copy = tcase_create("Copy");
  tcase_add_checked_fixture(tc_copy, pspinfo_setup, pspinfo_teardown);
  tcase_add_test(tc_copy, test_pspinfo_copy_null);
  tcase_add_test(tc_copy, test_pspinfo_copy_nonnull);
  suite_add_tcase(s, tc_copy);

  tc_setget = tcase_create("Setters and getters");
  tcase_add_checked_fixture(tc_setget, pspinfo_setup, pspinfo_teardown);
  tcase_add_test(tc_setget, test_pspinfo_setget_author);
  tcase_add_test(tc_setget, test_pspinfo_setget_code);
  tcase_add_test(tc_setget, test_pspinfo_setget_date);
  tcase_add_test(tc_setget, test_pspinfo_setget_description);
  tcase_add_test(tc_setget, test_pspinfo_setget_scheme_name);
  suite_add_tcase(s, tc_setget);

  return s;
}
