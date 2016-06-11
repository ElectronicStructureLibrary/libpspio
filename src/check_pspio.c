/* Copyright (C) 2011-2016 Joseba Alberdi <alberdi@hotmail.es>
 *                         Matthieu Verstraete <matthieu.jean.verstraete@gmail.com>
 *                         Micael Oliveira <micaeljtoliveira@gmail.com>
 *                         Yann Pouillon <notifications@materialsevolution.es>
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

