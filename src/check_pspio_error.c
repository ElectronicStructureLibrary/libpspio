/* Copyright (C) 2011-2016 Micael Oliveira <micael.oliveira@mpsd.mpg.de>
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
 * @file check_pspio_error.c
 * @brief checks pspio_error.c and pspio_error.h 
 */

#include <stdio.h>
#include <check.h>

#include "pspio_error.h"

static char *err_str;

void error_setup(void)
{
  pspio_error_free();
}

void error_teardown(void)
{
  pspio_error_free();
}

START_TEST(test_error_fetchall)
{
  ck_assert(pspio_error_add(PSPIO_EVALUE, "test_1_1.c", 1234, "dummy1") == PSPIO_EVALUE);
  err_str = pspio_error_fetchall();
  ck_assert_str_eq(err_str, "libpspio: ERROR:\n"
          "  * in test_1_1.c(dummy1):1234:\n"
          "      value error: bad value found (PSPIO_EVALUE)\n");
  free(err_str);
}
END_TEST

START_TEST(test_error_empty)
{
  ck_assert_int_eq(pspio_error_get_last(NULL), PSPIO_SUCCESS);
  ck_assert_int_eq(pspio_error_len(), 0);

  err_str = pspio_error_fetchall();
  ck_assert(err_str == NULL);
  free(err_str);

  ck_assert_int_eq(pspio_error_get_last(NULL), PSPIO_SUCCESS);
  ck_assert_int_eq(pspio_error_len(), 0);

}
END_TEST

START_TEST(test_error_single)
{
  ck_assert(pspio_error_add(PSPIO_EVALUE, "test_1_1.c", 1234, "dummy1") == PSPIO_EVALUE);
  ck_assert(pspio_error_get_last(NULL) == PSPIO_EVALUE);
  ck_assert(pspio_error_len() == 1);

  err_str = pspio_error_fetchall();
  ck_assert_str_eq(err_str, "libpspio: ERROR:\n"
          "  * in test_1_1.c(dummy1):1234:\n"
          "      value error: bad value found (PSPIO_EVALUE)\n");
  free(err_str);

  ck_assert(pspio_error_get_last(NULL) == PSPIO_SUCCESS);
  ck_assert(pspio_error_len() == 0);
}
END_TEST

START_TEST(test_error_double)
{
  ck_assert(pspio_error_add(PSPIO_EGSL, "test_2_1.c", 1234, "dummy21") == PSPIO_EGSL);
  ck_assert(pspio_error_get_last(NULL) == PSPIO_EGSL);
  ck_assert(pspio_error_len() == 1);

  ck_assert(pspio_error_add(PSPIO_ENOSUPPORT, "test_2_2.c", 202, "dummy22") == PSPIO_ENOSUPPORT);
  ck_assert(pspio_error_get_last(NULL) == PSPIO_ENOSUPPORT);
  ck_assert(pspio_error_len() == 2);

  err_str = pspio_error_fetchall();
  ck_assert_str_eq(err_str, "libpspio: ERROR:\n"
          "  * in test_2_1.c(dummy21):1234:\n"
          "      error in GSL (PSPIO_EGSL)\n"
          "  * in test_2_2.c(dummy22):202:\n"
          "      unsupported option in the pseudopotential file (PSPIO_ENOSUPPORT)\n");
  free(err_str);

  ck_assert(pspio_error_get_last(NULL) == PSPIO_SUCCESS);
  ck_assert(pspio_error_len() == 0);
}
END_TEST

START_TEST(test_error_triple)
{
  char result[PSPIO_STRLEN_ERROR];

  ck_assert(pspio_error_add(PSPIO_EVALUE, "test_3_1.c", 311, "dummy31") == PSPIO_EVALUE);
  ck_assert(pspio_error_get_last(NULL) == PSPIO_EVALUE);
  ck_assert(pspio_error_len() == 1);

  ck_assert(pspio_error_add(PSPIO_ENOFILE, "test_3_2.c", 322, "dummy32") == PSPIO_ENOFILE); 
  ck_assert(pspio_error_get_last(NULL) == PSPIO_ENOFILE);
  ck_assert(pspio_error_len() == 2);

  ck_assert(pspio_error_add(PSPIO_ERROR, "test_3_3.c", 333, "dummy33") == PSPIO_ERROR); 
  ck_assert(pspio_error_get_last(NULL) == PSPIO_ERROR);
  ck_assert(pspio_error_len() == 3);

  sprintf(result, "libpspio: ERROR:\n");
  sprintf(result+strlen(result), "  * in test_3_1.c(dummy31):311:\n"
          "      value error: bad value found (PSPIO_EVALUE)\n");
  sprintf(result+strlen(result), "  * in test_3_2.c(dummy32):322:\n"
          "      file does not exist (PSPIO_ENOFILE)\n");
  sprintf(result+strlen(result), "  * in test_3_3.c(dummy33):333:\n"
          "      error (PSPIO_ERROR)\n");
  err_str = pspio_error_fetchall();
  ck_assert_str_eq(err_str, result);
  free(err_str);

  ck_assert(pspio_error_get_last(NULL) == PSPIO_SUCCESS);
  ck_assert(pspio_error_len() == 0);
}
END_TEST

START_TEST(test_error_get_last)
{
  ck_assert(pspio_error_add(PSPIO_EGSL,    "test_4_1.c", 411, "dummy41") == PSPIO_EGSL);
  ck_assert(pspio_error_add(PSPIO_ENOFILE, "test_4_2.c", 422, "dummy42") == PSPIO_ENOFILE);
  ck_assert(pspio_error_add(PSPIO_ERROR,   "test_4_3.c", 433, "dummy43") == PSPIO_ERROR);
  ck_assert(pspio_error_len() == 3);

  ck_assert(pspio_error_get_last("dummy41") == PSPIO_EGSL);
  ck_assert(pspio_error_get_last("dummy42") == PSPIO_ENOFILE);
  ck_assert(pspio_error_get_last("dummy43") == PSPIO_ERROR);
  ck_assert(pspio_error_get_last("dummy44") == PSPIO_SUCCESS);
}
END_TEST


Suite * make_error_suite(void)
{
  Suite *s;
  TCase *tc_fetch, *tc_empty, *tc_single, *tc_double, *tc_triple, *tc_last;

  s = suite_create("Error");

  tc_fetch = tcase_create("Fetch all");
  tcase_add_checked_fixture(tc_fetch, error_setup, error_teardown);
  tcase_add_test(tc_fetch, test_error_fetchall);
  suite_add_tcase(s, tc_fetch);

  tc_empty = tcase_create("Empty chain");
  tcase_add_checked_fixture(tc_empty, error_setup, error_teardown);
  tcase_add_test(tc_empty, test_error_empty);
  suite_add_tcase(s, tc_empty);

  tc_single = tcase_create("Single error chain");
  tcase_add_checked_fixture(tc_single, error_setup, error_teardown);
  tcase_add_test(tc_single, test_error_single);
  suite_add_tcase(s, tc_single);

  tc_double = tcase_create("Double error chain");
  tcase_add_checked_fixture(tc_double, error_setup, error_teardown);
  tcase_add_test(tc_double, test_error_double);
  suite_add_tcase(s, tc_double);

  tc_triple = tcase_create("Triple error chain");
  tcase_add_checked_fixture(tc_triple, error_setup, error_teardown);
  tcase_add_test(tc_triple, test_error_triple);
  suite_add_tcase(s, tc_triple);

  tc_last = tcase_create("Get last");
  tcase_add_checked_fixture(tc_last, error_setup, error_teardown);
  tcase_add_test(tc_last, test_error_get_last);
  suite_add_tcase(s, tc_last);

  return s;
}
