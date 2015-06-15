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

#include "pspio_error.h"
#include "pspio_pspdata.h"
#include "test_common.h"


int main(void) {
  pspio_pspdata_t *pspdata = NULL;

  /* Display basic information */
  DEBUG_PRINT("%s - test_pspdata\nReport bugs to %s\n\n", PACKAGE_STRING,
    PACKAGE_BUGREPORT);
  DEBUG_PRINT("=== BEGIN test_pspdata ===\n\n");

  /* Init pspdata */
  DEBUG_PRINT("test_pspdata: initializing pspdata\n");
  CHECK_STAT(pspio_pspdata_init(&pspdata), PSPIO_SUCCESS);
  DEBUG_PRINT("\n");

  /* Check all setters and getters */
  DEBUG_PRINT("test_pspdata: checking get_format_guessed\n");
  CHECK_STAT(pspio_pspdata_get_format_guessed(pspdata), PSPIO_FMT_UNKNOWN);

  DEBUG_PRINT("test_pspdata: checking set_symbol\n");
  CHECK_STAT(pspio_pspdata_set_symbol(pspdata, "He"), PSPIO_SUCCESS);
  DEBUG_PRINT("test_pspdata: get_symbol = '%s' (should be 'He')\n", pspio_pspdata_get_symbol(pspdata));

  DEBUG_PRINT("test_pspdata: checking set_z\n");
  CHECK_STAT(pspio_pspdata_set_z(pspdata, 3.0), PSPIO_SUCCESS);
  DEBUG_PRINT("test_pspdata: get_z = '%3.1f' (should be 3.0)\n", pspio_pspdata_get_z(pspdata));

  DEBUG_PRINT("test_pspdata: checking set_zvalence\n");
  CHECK_STAT(pspio_pspdata_set_zvalence(pspdata, 4.0), PSPIO_SUCCESS);
  DEBUG_PRINT("test_pspdata: get_zvalence = '%3.1f' (should be 4.0)\n", pspio_pspdata_get_zvalence(pspdata));

  DEBUG_PRINT("test_pspdata: checking set_nelvalence\n");
  CHECK_STAT(pspio_pspdata_set_nelvalence(pspdata, 2.0), PSPIO_SUCCESS);
  DEBUG_PRINT("test_pspdata: get_nelvalence = '%3.1f' (should be 2.0)\n", pspio_pspdata_get_nelvalence(pspdata));

  DEBUG_PRINT("test_pspdata: checking set_l_max\n");
  CHECK_STAT(pspio_pspdata_set_l_max(pspdata, 2), PSPIO_SUCCESS);
  DEBUG_PRINT("test_pspdata: get_l_max = '%d' (should be 2)\n", pspio_pspdata_get_l_max(pspdata));

  DEBUG_PRINT("test_pspdata: checking set_wave_eq\n");
  CHECK_STAT(pspio_pspdata_set_wave_eq(pspdata, PSPIO_EQN_SCHRODINGER), PSPIO_SUCCESS);
  DEBUG_PRINT("test_pspdata: get_wave_eq = '%d' (should be '%d')\n", pspio_pspdata_get_wave_eq(pspdata), PSPIO_EQN_SCHRODINGER);

  DEBUG_PRINT("test_pspdata: checking set_total_energy\n");
  CHECK_STAT(pspio_pspdata_set_total_energy(pspdata, 1.234), PSPIO_SUCCESS);
  DEBUG_PRINT("test_pspdata: get_total_energy = '%5.3f' (should be 1.234)\n", pspio_pspdata_get_total_energy(pspdata));




  DEBUG_PRINT("\n");

  
  /* Destroy pspdata structures */
  DEBUG_PRINT("test_pspdata: destroying pspdata structure\n");
  pspio_pspdata_free(pspdata);
  DEBUG_PRINT("\n");

  DEBUG_PRINT("=== END test_pspdata ===\n")

  return 0;
}
