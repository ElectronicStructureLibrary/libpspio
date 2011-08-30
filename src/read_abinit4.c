/*
 Copyright (C) 2011 J. Alberdi, M. Oliveira, Y. Pouillon, and M. Verstraete

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

 $Id$
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/** subroutine to read in abinit psp format 4  */

#include <stdio.h>
#include <string.h>

#include "pspio_error.h"
#include "pspio.h"

/**
* subroutine reads in full abinit file and initializes psp_data structure with available stuff
*@param[in]  fp         pointer to open file
*@param[out] psp_data   pseudopotential info is filled in present routine and subroutines
*/
int read_abinit (FILE *fp, pspio_pspdata_t *psp_data){

  /// local variables
  int ierr;
  int idum;
  int narg;
  int ncharead = 1000;
  int pspcod; 
  int pspxc; 
  char line[ncharead];
  char *testread;


  /// read in header
  ierr = read_abinit_header(fp, psp_data, pspcod);
  if (ierr != PSPIO_SUCCESS) return ierr;

 
  /**< read in psp code and xc code*/
  if(fgets(line, ncharead, fp) == NULL) return PSPIO_EIO;
  narg = sscanf (line, "%d %d %d %d %d", &pspcod, &pspxc, &((*psp_data).lmax), &idum, &((*(*psp_data).mesh).np) );
  ///check narg is equal to 5



  return PSPIO_SUCCESS;

}
