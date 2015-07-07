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
 * @file test_pspdata.c
 * @brief checks pspio_pspdata.c and pspio_pspdata.h
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <check.h>

#include "pspio_error.h"
#include "pspio_pspdata.h"


static pspio_pspdata_t *pspdata;
static pspio_mesh_t *mesh = NULL;


void pspdata_setup(void)
{
  const double a = 1.0;
  const double b = 2.0;

  pspio_mesh_free(mesh);
  pspio_mesh_alloc(&mesh, 8);
  pspio_mesh_init_from_parameters(mesh, PSPIO_MESH_LOG1, a, b);

  pspio_pspdata_alloc(&pspdata);

}

void pspdata_teardown(void)
{
  pspio_mesh_free(mesh);
  mesh = NULL;

  pspio_pspdata_free(pspdata);
  pspdata = NULL;
}


START_TEST(test_pspdata_alloc)
{
  ck_assert(pspio_pspdata_alloc(&pspdata) == PSPIO_SUCCESS);
}
END_TEST


START_TEST(test_pspdata_format_guessed)
{
  /* Not set function, so we just check that the default value is correct */
  ck_assert(pspio_pspdata_get_format_guessed(pspdata) == PSPIO_FMT_UNKNOWN);
}
END_TEST

START_TEST(test_pspdata_symbol)
{
  ck_assert(pspio_pspdata_set_symbol(pspdata, "Al") == PSPIO_SUCCESS);
  ck_assert_str_eq(pspio_pspdata_get_symbol(pspdata), "Al");
}
END_TEST

START_TEST(test_pspdata_z)
{
  ck_assert(pspio_pspdata_set_z(pspdata, 2.0) == PSPIO_SUCCESS);
  ck_assert(pspio_pspdata_get_z(pspdata) == 2.0);
}
END_TEST

START_TEST(test_pspdata_zvalence)
{
  ck_assert(pspio_pspdata_set_zvalence(pspdata, 3.0) == PSPIO_SUCCESS);
  ck_assert(pspio_pspdata_get_zvalence(pspdata) == 3.0);
}
END_TEST

START_TEST(test_pspdata_nelvalence)
{
  ck_assert(pspio_pspdata_set_nelvalence(pspdata, 4.0) == PSPIO_SUCCESS);
  ck_assert(pspio_pspdata_get_nelvalence(pspdata) == 4.0);
}
END_TEST

START_TEST(test_pspdata_l_max)
{
  ck_assert(pspio_pspdata_set_l_max(pspdata, 2) == PSPIO_SUCCESS);
  ck_assert(pspio_pspdata_get_l_max(pspdata) == 2);
}
END_TEST

START_TEST(test_pspdata_wave_eq)
{
  ck_assert(pspio_pspdata_set_wave_eq(pspdata, PSPIO_EQN_SCHRODINGER) == PSPIO_SUCCESS);
  ck_assert(pspio_pspdata_get_wave_eq(pspdata) == PSPIO_EQN_SCHRODINGER);
}
END_TEST

START_TEST(test_pspdata_total_energy)
{
  ck_assert(pspio_pspdata_set_total_energy(pspdata, 4.5) == PSPIO_SUCCESS);
  ck_assert(pspio_pspdata_get_total_energy(pspdata) == 4.5);
}
END_TEST

START_TEST(test_pspdata_mesh)
{
  ck_assert(pspio_pspdata_set_mesh(pspdata, mesh) == PSPIO_SUCCESS);
  ck_assert(pspio_mesh_cmp(pspio_pspdata_get_mesh(pspdata), mesh) == PSPIO_MESH_EQUAL);
}
END_TEST

START_TEST(test_pspdata_n_states)
{
  ck_assert(pspio_pspdata_set_n_states(pspdata, 3) == PSPIO_SUCCESS);
  ck_assert(pspio_pspdata_get_n_states(pspdata) == 3);
}
END_TEST

Suite * make_pspdata_suite(void)
{
  Suite *s;
  TCase *tc_alloc, *tc_getset;

  s = suite_create("Pspdata");

  tc_alloc = tcase_create("Allocation");
  tcase_add_checked_fixture(tc_alloc, NULL, pspdata_teardown);
  tcase_add_test(tc_alloc, test_pspdata_alloc);
  suite_add_tcase(s, tc_alloc);

  tc_getset = tcase_create("Setters and getters");
  tcase_add_checked_fixture(tc_getset, pspdata_setup, pspdata_teardown);
  tcase_add_test(tc_getset, test_pspdata_format_guessed);
  tcase_add_test(tc_getset, test_pspdata_symbol);
  tcase_add_test(tc_getset, test_pspdata_z);
  tcase_add_test(tc_getset, test_pspdata_zvalence);
  tcase_add_test(tc_getset, test_pspdata_nelvalence);
  tcase_add_test(tc_getset, test_pspdata_l_max);
  tcase_add_test(tc_getset, test_pspdata_wave_eq);
  tcase_add_test(tc_getset, test_pspdata_total_energy);
  tcase_add_test(tc_getset, test_pspdata_mesh);
  suite_add_tcase(s, tc_getset);
    
  return s;
}




