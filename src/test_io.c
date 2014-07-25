/*
 Copyright (C) 2011-2012 J. Alberdi, M. Oliveira, Y. Pouillon, and M. Verstraete

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
 * @file test_io.c
 * @brief checks functions that read and write the pseudopotentials
 */

#include <stdlib.h>
#include <stdio.h>

#include "pspio_pspdata.h"
#include "test_common.h"


int main(int argc, char *argv[]) {
  int format;
  pspio_pspdata_t *pspdata = NULL;

  if(argc != 4){
    printf("Usage:\n%s input_file output_file format\n", argv[0]);
    return 1;
  }
  format = atoi(argv[3]);

  /* Display basic information */
  DEBUG_PRINT("%s - test_io\nReport bugs to %s\n\n", PACKAGE_STRING,
    PACKAGE_BUGREPORT);
  DEBUG_PRINT("=== BEGIN test_io ===\n\n");

  /* init pspdata */
  DEBUG_PRINT("test_io: initializing pspdata\n");
  CHECK_STAT(pspio_pspdata_init(&pspdata), PSPIO_SUCCESS);
  DEBUG_PRINT("\n");

  /* check parsing of file */
  DEBUG_PRINT("test_io: parsing file %s\n", argv[1]);
  CHECK_STAT(pspio_pspdata_read(&pspdata, &format, argv[1]), PSPIO_SUCCESS);
  if ( pspio_error_get_last(NULL) == PSPIO_SUCCESS ) {
    DEBUG_PRINT("test_io: file parsing successful\n");
  }
  DEBUG_PRINT("\n");

  /* check writing of file */
  DEBUG_PRINT("test_io: writing file %s\n", argv[2]);
  CHECK_STAT(pspio_pspdata_write(pspdata, format, argv[2]), PSPIO_SUCCESS);
  if ( pspio_error_get_last(NULL) == PSPIO_SUCCESS ) {
    DEBUG_PRINT("test_io: file writing successful\n");
  }
  DEBUG_PRINT("\n");
  
  /* Destroy pspdata structures */
  DEBUG_PRINT("test_io: destroying pspdata structure\n");
  pspio_pspdata_free(&pspdata);
  DEBUG_PRINT("\n");

  DEBUG_PRINT("=== END test_io ===\n")

  return 0;
}
