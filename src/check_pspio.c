/*
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

  sr = srunner_create(make_error_suite());
  srunner_add_suite(sr, make_qn_suite());
  srunner_add_suite(sr, make_mesh_suite());
  srunner_add_suite(sr, make_meshfunc_suite());
  srunner_add_suite(sr, make_potential_suite());

  srunner_run_all(sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

