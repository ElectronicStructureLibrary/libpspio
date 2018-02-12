/* Copyright (C) 2011-2016 Joseba Alberdi <alberdi@hotmail.es>
 *                         Matthieu Verstraete <matthieu.jean.verstraete@gmail.com>
 *                         Micael Oliveira <micael.oliveira@mpsd.mpg.de>
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
 * @file check_pspsio.c
 * @brief Runs all the pspio unit tests
 */

#include <stdlib.h>
#include <check.h>

#include "check_pspio.h"

int main(void)
{
  int number_failed;
  SRunner *sr;

  sr = srunner_create(make_info_suite());
  srunner_add_suite(sr, make_error_suite());
  srunner_add_suite(sr, make_qn_suite());
  srunner_add_suite(sr, make_mesh_suite());
  srunner_add_suite(sr, make_meshfunc_suite());
  srunner_add_suite(sr, make_potential_suite());
  srunner_add_suite(sr, make_projector_suite());
  srunner_add_suite(sr, make_state_suite());
  srunner_add_suite(sr, make_xc_suite());
  srunner_add_suite(sr, make_pspinfo_suite());
  srunner_add_suite(sr, make_pspdata_suite());

  srunner_run_all(sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

