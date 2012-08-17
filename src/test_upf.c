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

 $Id: test_error.c 229 2011-09-05 13:17:03Z micael $
*/

/**
 * @file test_upf.c
 * @brief checks read_upf.c and upf.h 
 */

#include <stdlib.h>
#include <stdio.h>

#include "upf.h"
#include "pspio_pspdata.h"
#include "test_common.h"


int main(int argc, char *argv[]) {
  int eid;
  pspio_pspdata_t *pspdata = NULL;

  if(argc != 2){
    printf("Usage:\n%s file\n", argv[0]);
    return 1;
  }

  /* Display basic information */
  DEBUG_PRINT("%s - test_upf\nReport bugs to %s\n\n", PACKAGE_STRING,
    PACKAGE_BUGREPORT);
  DEBUG_PRINT("=== BEGIN test_upf ===\n\n");

  /* check parsing of UPF file */
  DEBUG_PRINT("test_upf: parsing file %s\n", argv[1]);
  eid = pspio_pspdata_init(&pspdata, argv[1], UPF);
  eid = pspio_error_flush();
  DEBUG_PRINT("\n");
  
  /* Destroy pspdata structures */
  DEBUG_PRINT("test_upf: destroying pspdata structure\n");
  eid = pspio_pspdata_free(&pspdata);
  eid = pspio_error_flush();
  DEBUG_PRINT("\n");

  DEBUG_PRINT("=== END test_upf ===\n")

  return 0;
}
