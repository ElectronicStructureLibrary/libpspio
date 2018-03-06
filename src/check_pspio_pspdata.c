/* Copyright (C) 2015-2017 Micael Oliveira <micael.oliveira@mpsd.mpg.de>
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
 * @file test_pspdata.c
 * @brief checks pspio_pspdata.c and pspio_pspdata.h
 */

#include <check.h>

#include "pspio_error.h"
#include "pspio_pspdata.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif

static pspio_pspdata_t *pspdata = NULL;
static pspio_pspinfo_t *pspinfo = NULL;
static pspio_mesh_t *mesh = NULL;
static pspio_qn_t *qn = NULL;
static pspio_state_t *state = NULL;
static pspio_potential_t *potential = NULL;
static pspio_projector_t *projector = NULL;
static pspio_xc_t *xc = NULL;
static pspio_meshfunc_t *rho_valence = NULL;

char filename[200];


void pspdata_minimal_setup(void)
{
  pspio_pspdata_free(pspdata);
  pspio_pspdata_alloc(&pspdata);
}

void pspdata_minimal_teardown(void)
{
  pspio_pspdata_free(pspdata);
  pspdata = NULL;
}

void pspdata_full_setup(void)
{
  int i;
  const double a = 1.0;
  const double b = 2.0;
  double *func = NULL;
  const double *r;

  pspio_pspdata_free(pspdata);
  pspio_pspdata_alloc(&pspdata);

  pspio_pspinfo_free(pspinfo);
  pspio_pspinfo_alloc(&pspinfo);
  pspio_pspinfo_set_author(pspinfo, "A. Author");
  pspio_pspinfo_set_code_version(pspinfo, "1.0");
  pspio_pspinfo_set_generation_day(pspinfo, 1);
  pspio_pspinfo_set_generation_month(pspinfo, 1);
  pspio_pspinfo_set_generation_year(pspinfo, 1900);
  pspio_pspinfo_set_description(pspinfo, "Universal alchemical pseudopotential");

  pspio_mesh_free(mesh);
  pspio_mesh_alloc(&mesh, 8);
  pspio_mesh_init_from_parameters(mesh, PSPIO_MESH_LOG1, a, b);

  func = (double *)malloc(8*sizeof(double));
  r = pspio_mesh_get_r(mesh);
  for (i=0; i<8; i++) {
    func[i] = 2.0*r[i]*r[i];
  }

  pspio_qn_free(qn);
  pspio_qn_alloc(&qn);
  pspio_qn_init(qn, 1, 0, 0.0);

  pspio_state_free(state);
  pspio_state_alloc(&state, 8);
  pspio_state_init(state, 3.0, qn, 2.0, 1.2, mesh, func, NULL);

  pspio_potential_free(potential);
  pspio_potential_alloc(&potential, 8);
  pspio_potential_init(potential, qn, mesh, func);

  pspio_projector_free(projector);
  pspio_projector_alloc(&projector, 8);
  pspio_projector_init(projector, qn, mesh, func);

  pspio_xc_free(xc);
  pspio_xc_alloc(&xc);
  pspio_xc_init(xc, 1, 4, PSPIO_NLCC_FHI, 1.0, 2.0, mesh, func, NULL, NULL);

  pspio_meshfunc_free(rho_valence);
  pspio_meshfunc_alloc(&rho_valence, 8);
  pspio_meshfunc_init(rho_valence, mesh, func, NULL, NULL);

  free(func);
}

void pspdata_full_teardown(void)
{
  pspio_pspinfo_free(pspinfo);
  pspinfo = NULL;

  pspio_mesh_free(mesh);
  mesh = NULL;

  pspio_qn_free(qn);
  qn = NULL;

  pspio_state_free(state);
  state = NULL;

  pspio_potential_free(potential);
  potential = NULL;

  pspio_projector_free(projector);
  projector = NULL;

  pspio_xc_free(xc);
  xc = NULL;

  pspio_meshfunc_free(rho_valence);
  rho_valence = NULL;

  pspio_pspdata_free(pspdata);
  pspdata = NULL;
}


START_TEST(test_pspdata_alloc)
{
  ck_assert(pspio_pspdata_alloc(&pspdata) == PSPIO_SUCCESS);
}
END_TEST


START_TEST(test_pspdata_setget_format_guessed)
{
  /* No set function, so we just check that the default value is correct */
  ck_assert(pspio_pspdata_get_format_guessed(pspdata) == PSPIO_FMT_UNKNOWN);
}
END_TEST

START_TEST(test_pspdata_setget_pspinfo)
{
  ck_assert(pspio_pspdata_set_pspinfo(pspdata, pspinfo) == PSPIO_SUCCESS);
  ck_assert(pspio_pspinfo_cmp(pspio_pspdata_get_pspinfo(pspdata), pspinfo) == PSPIO_EQUAL);
}
END_TEST

START_TEST(test_pspdata_setget_symbol)
{
  ck_assert(pspio_pspdata_set_symbol(pspdata, "Al") == PSPIO_SUCCESS);
  ck_assert_str_eq(pspio_pspdata_get_symbol(pspdata), "Al");
}
END_TEST

START_TEST(test_pspdata_setget_z)
{
  ck_assert(pspio_pspdata_set_z(pspdata, 2.0) == PSPIO_SUCCESS);
  ck_assert(pspio_pspdata_get_z(pspdata) == 2.0);
}
END_TEST

START_TEST(test_pspdata_setget_zvalence)
{
  ck_assert(pspio_pspdata_set_zvalence(pspdata, 3.0) == PSPIO_SUCCESS);
  ck_assert(pspio_pspdata_get_zvalence(pspdata) == 3.0);
}
END_TEST

START_TEST(test_pspdata_setget_nelvalence)
{
  ck_assert(pspio_pspdata_set_nelvalence(pspdata, 4.0) == PSPIO_SUCCESS);
  ck_assert(pspio_pspdata_get_nelvalence(pspdata) == 4.0);
}
END_TEST

START_TEST(test_pspdata_setget_l_max)
{
  ck_assert(pspio_pspdata_set_l_max(pspdata, 2) == PSPIO_SUCCESS);
  ck_assert(pspio_pspdata_get_l_max(pspdata) == 2);
}
END_TEST

START_TEST(test_pspdata_setget_wave_eq)
{
  ck_assert(pspio_pspdata_set_wave_eq(pspdata, PSPIO_EQN_SCHRODINGER) == PSPIO_SUCCESS);
  ck_assert(pspio_pspdata_get_wave_eq(pspdata) == PSPIO_EQN_SCHRODINGER);
}
END_TEST

START_TEST(test_pspdata_setget_total_energy)
{
  ck_assert(pspio_pspdata_set_total_energy(pspdata, 4.5) == PSPIO_SUCCESS);
  ck_assert(pspio_pspdata_get_total_energy(pspdata) == 4.5);
}
END_TEST

START_TEST(test_pspdata_setget_mesh)
{
  ck_assert(pspio_pspdata_set_mesh(pspdata, mesh) == PSPIO_SUCCESS);
  ck_assert(pspio_mesh_cmp(pspio_pspdata_get_mesh(pspdata), mesh) == PSPIO_EQUAL);
}
END_TEST

START_TEST(test_pspdata_setget_n_states)
{
  ck_assert(pspio_pspdata_set_n_states(pspdata, 3) == PSPIO_SUCCESS);
  ck_assert(pspio_pspdata_get_n_states(pspdata) == 3);
}
END_TEST

START_TEST(test_pspdata_setget_state)
{
  ck_assert(pspio_pspdata_set_n_states(pspdata, 1) == PSPIO_SUCCESS);
  ck_assert(pspio_pspdata_set_state(pspdata, 0, state) == PSPIO_SUCCESS);
  ck_assert(pspio_state_cmp(pspio_pspdata_get_state(pspdata, 0), state) == PSPIO_EQUAL);
}
END_TEST

START_TEST(test_pspdata_setget_scheme)
{
  ck_assert(pspio_pspdata_set_scheme(pspdata, PSPIO_SCM_MRPP) == PSPIO_SUCCESS);
  ck_assert(pspio_pspdata_get_scheme(pspdata) == PSPIO_SCM_MRPP);
}
END_TEST

START_TEST(test_pspdata_setget_n_potentials)
{
  ck_assert(pspio_pspdata_set_n_potentials(pspdata, 2) == PSPIO_SUCCESS);
  ck_assert(pspio_pspdata_get_n_potentials(pspdata) == 2);
}
END_TEST

START_TEST(test_pspdata_setget_potential)
{
  ck_assert(pspio_pspdata_set_n_potentials(pspdata, 1) == PSPIO_SUCCESS);
  ck_assert(pspio_pspdata_set_potential(pspdata, 0, potential) == PSPIO_SUCCESS);
  ck_assert(pspio_potential_cmp(pspio_pspdata_get_potential(pspdata, 0), potential) == PSPIO_EQUAL);
}
END_TEST

START_TEST(test_pspdata_setget_n_projectors)
{
  ck_assert(pspio_pspdata_set_n_projectors(pspdata, 4) == PSPIO_SUCCESS);
  ck_assert(pspio_pspdata_get_n_projectors(pspdata) == 4);
}
END_TEST

START_TEST(test_pspdata_setget_projector)
{
  ck_assert(pspio_pspdata_set_n_projectors(pspdata, 1) == PSPIO_SUCCESS);
  ck_assert(pspio_pspdata_set_projector(pspdata, 0, projector) == PSPIO_SUCCESS);
  ck_assert(pspio_projector_cmp(pspio_pspdata_get_projector(pspdata, 0), projector) == PSPIO_EQUAL);
}
END_TEST

START_TEST(test_pspdata_setget_projectors_l_max)
{
  ck_assert(pspio_pspdata_set_projectors_l_max(pspdata, 1) == PSPIO_SUCCESS);
  ck_assert(pspio_pspdata_get_projectors_l_max(pspdata) == 1);
}
END_TEST

START_TEST(test_pspdata_setget_l_local)
{
  ck_assert(pspio_pspdata_set_l_local(pspdata, -1) == PSPIO_SUCCESS);
  ck_assert(pspio_pspdata_get_l_local(pspdata) == -1);
}
END_TEST

START_TEST(test_pspdata_setget_vlocal)
{
  ck_assert(pspio_pspdata_set_vlocal(pspdata, potential) == PSPIO_SUCCESS);
  ck_assert(pspio_potential_cmp(pspio_pspdata_get_vlocal(pspdata), potential) == PSPIO_EQUAL);
}
END_TEST

START_TEST(test_pspdata_setget_xc)
{
  ck_assert(pspio_pspdata_set_xc(pspdata, xc) == PSPIO_SUCCESS);
  ck_assert(pspio_xc_cmp(pspio_pspdata_get_xc(pspdata), xc) == PSPIO_EQUAL);
}
END_TEST

START_TEST(test_pspdata_setget_rho_valence)
{
  ck_assert(pspio_pspdata_set_rho_valence(pspdata, rho_valence) == PSPIO_SUCCESS);
  ck_assert(pspio_meshfunc_cmp(pspio_pspdata_get_rho_valence(pspdata),rho_valence) == PSPIO_EQUAL);
}
END_TEST


START_TEST(test_pspdata_fhi_io)
{
  sprintf(filename, "%s/%s", PSPIO_CHK_DATADIR, "fhi/Li.cpi");
  ck_assert(pspio_pspdata_read(pspdata, PSPIO_FMT_FHI98PP, filename) == PSPIO_SUCCESS);
  sprintf(filename, "test_io_%d.tmp", PSPIO_FMT_FHI98PP);
  ck_assert(pspio_pspdata_write(pspdata, PSPIO_FMT_FHI98PP, filename) == PSPIO_SUCCESS);
}
END_TEST

START_TEST(test_pspdata_fhi_guess)
  {
    sprintf(filename, "%s/%s", PSPIO_CHK_DATADIR, "fhi/Li.cpi");
    ck_assert(pspio_pspdata_read(pspdata, PSPIO_FMT_UNKNOWN, filename) == PSPIO_SUCCESS);
    ck_assert(pspio_pspdata_get_format_guessed(pspdata) == PSPIO_FMT_FHI98PP);
  }
END_TEST

START_TEST(test_pspdata_abinit6_io)
{
  sprintf(filename, "%s/%s", PSPIO_CHK_DATADIR, "abinit6/03-Li.LDA.fhi");
  ck_assert(pspio_pspdata_read(pspdata, PSPIO_FMT_ABINIT_6, filename) == PSPIO_SUCCESS);
  sprintf(filename, "test_io_%d.tmp", PSPIO_FMT_ABINIT_6);
  ck_assert(pspio_pspdata_write(pspdata, PSPIO_FMT_ABINIT_6, filename) == PSPIO_SUCCESS);
}
END_TEST

START_TEST(test_pspdata_abinit6_guess)
{
  sprintf(filename, "%s/%s", PSPIO_CHK_DATADIR, "abinit6/03-Li.LDA.fhi");
  ck_assert(pspio_pspdata_read(pspdata, PSPIO_FMT_UNKNOWN, filename) == PSPIO_SUCCESS);
  ck_assert(pspio_pspdata_get_format_guessed(pspdata) == PSPIO_FMT_ABINIT_6);
}
END_TEST

START_TEST(test_pspdata_upf_io)
{
  sprintf(filename, "%s/%s", PSPIO_CHK_DATADIR, "UPF/Li.UPF");
  ck_assert(pspio_pspdata_read(pspdata, PSPIO_FMT_UPF, filename) == PSPIO_SUCCESS);
  sprintf(filename, "test_io_%d.tmp", PSPIO_FMT_UPF);
  ck_assert(pspio_pspdata_write(pspdata, PSPIO_FMT_UPF, filename) == PSPIO_SUCCESS);
}
END_TEST

START_TEST(test_pspdata_upf_guess)
{
  sprintf(filename, "%s/%s", PSPIO_CHK_DATADIR, "UPF/Li.UPF");
  ck_assert(pspio_pspdata_read(pspdata, PSPIO_FMT_UNKNOWN, filename) == PSPIO_SUCCESS);
  ck_assert(pspio_pspdata_get_format_guessed(pspdata) == PSPIO_FMT_UPF);
}
END_TEST


Suite * make_pspdata_suite(void)
{
  Suite *s;
  TCase *tc_alloc, *tc_setget, *tc_io;

  s = suite_create("Pspdata");

  tc_alloc = tcase_create("Allocation");
  tcase_add_checked_fixture(tc_alloc, NULL, pspdata_minimal_teardown);
  tcase_add_test(tc_alloc, test_pspdata_alloc);
  suite_add_tcase(s, tc_alloc);

  tc_setget = tcase_create("Setters and getters");
  tcase_add_checked_fixture(tc_setget, pspdata_full_setup, pspdata_full_teardown);
  tcase_add_test(tc_setget, test_pspdata_setget_format_guessed);
  tcase_add_test(tc_setget, test_pspdata_setget_pspinfo);
  tcase_add_test(tc_setget, test_pspdata_setget_symbol);
  tcase_add_test(tc_setget, test_pspdata_setget_z);
  tcase_add_test(tc_setget, test_pspdata_setget_zvalence);
  tcase_add_test(tc_setget, test_pspdata_setget_nelvalence);
  tcase_add_test(tc_setget, test_pspdata_setget_l_max);
  tcase_add_test(tc_setget, test_pspdata_setget_wave_eq);
  tcase_add_test(tc_setget, test_pspdata_setget_total_energy);
  tcase_add_test(tc_setget, test_pspdata_setget_mesh);
  tcase_add_test(tc_setget, test_pspdata_setget_n_states);
  tcase_add_test(tc_setget, test_pspdata_setget_state);
  tcase_add_test(tc_setget, test_pspdata_setget_scheme);
  tcase_add_test(tc_setget, test_pspdata_setget_n_potentials);
  tcase_add_test(tc_setget, test_pspdata_setget_potential);
  tcase_add_test(tc_setget, test_pspdata_setget_n_projectors);
  tcase_add_test(tc_setget, test_pspdata_setget_projector);
  tcase_add_test(tc_setget, test_pspdata_setget_projectors_l_max);
  tcase_add_test(tc_setget, test_pspdata_setget_l_local);
  tcase_add_test(tc_setget, test_pspdata_setget_vlocal);
  tcase_add_test(tc_setget, test_pspdata_setget_xc);
  tcase_add_test(tc_setget, test_pspdata_setget_rho_valence);
  suite_add_tcase(s, tc_setget);

  tc_io = tcase_create("File parsing and writing");
  tcase_add_checked_fixture(tc_io, pspdata_minimal_setup, pspdata_minimal_teardown);
  tcase_add_test(tc_io, test_pspdata_fhi_io);
  tcase_add_test(tc_io, test_pspdata_fhi_guess);
  tcase_add_test(tc_io, test_pspdata_abinit6_io);
  tcase_add_test(tc_io, test_pspdata_abinit6_guess);
  tcase_add_test(tc_io, test_pspdata_upf_io);
  tcase_add_test(tc_io, test_pspdata_upf_guess);
  suite_add_tcase(s, tc_io);

  return s;
}
