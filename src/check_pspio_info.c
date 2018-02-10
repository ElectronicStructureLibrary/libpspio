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
 * @file check_pspsio_info.c
 * @brief checks pspio_info.c and pspio_info.h
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <check.h>

#include "pspio_info.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#else
#define PACKAGE_STRING ""
#define PACKAGE_VERSION ""
#endif

START_TEST(test_info_version)
{
  int major, minor, micro;
  char *version_string;
  size_t s = strlen(PACKAGE_VERSION);

  version_string = (char *) malloc (s+1);
  ck_assert(version_string != NULL);

  pspio_info_version(&major, &minor, &micro);
  sprintf(version_string,"%d.%d.%d",major,minor,micro);

  ck_assert_str_eq(version_string, PACKAGE_VERSION);
  free(version_string);
}
END_TEST

START_TEST(test_info_string)
{
  char *info_string;
  size_t s = strlen(PACKAGE_STRING);

  info_string = (char *) malloc (s+1);
  ck_assert(info_string != NULL);

  pspio_info_string(info_string);
  ck_assert_str_eq(info_string, PACKAGE_STRING);
  free(info_string);
}
END_TEST


Suite * make_info_suite(void)
{
  Suite *s;
  TCase *tc_info;

  s = suite_create("Info");

  tc_info = tcase_create("Version");
  tcase_add_test(tc_info, test_info_version);
  tcase_add_test(tc_info, test_info_string);
  suite_add_tcase(s, tc_info);

  return s;
}
