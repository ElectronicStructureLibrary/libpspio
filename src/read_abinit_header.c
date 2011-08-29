/*
 Copyright (C) 2011 J. Alberdi, M. Oliveira, Y. Pouillon, and M. Verstraete

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2, or (at your option)
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 02111-1307, USA.

 $Id$
*/


/** subroutine to read in abinit header which is generic to all psp files for abinit */

#include <stdio.h>
#include <string.h>
#include "pspio.h"
#include "pspio_error.h"

/**
* subroutine reads in full abinit header file and initializes psp_data structure with available stuff
*@param[in]  fname      file name to be read in 
*@param[out] psp_data   pseudopotential info is partially filled in present routine 
*@param[out] pspcod     pseudopotential file type (HGH GTH fhi etc...)
*/
int read_abinit_header (FILE *fp, psp_data_t *psp_data, int pspcod){

  /// local variables
  int ierr;
  int idum;
  int minlen;
  int ncharead = 1000;
  int pspxc; 
  char line[ncharead];
  char *testread;

  /**< read in title */
  if (fgets(line, ncharead, fp) == NULL) return PSPIO_IOERR;
  minlen = strlen(line) > STRLEN_TITLE ? STRLEN_TITLE : strlen(line);
  if (strncpy(&psp_data.title, line, minlen) == NULL){
    return PSPIO_IOERR;
  }
 
  /**< read in atomic number, pseudopotential ion charge (= num of valence electrons), and abinit data flag (not used) */
  if(fgets(line, ncharead, fp) == NULL) return PSPIO_IOERR;
  narg = sscanf (line, "%f %f %d", &psp_data.z, &psp_data.zvalence, &idum);
  ///check narg is equal to 2
 
  /**< read in psp code and xc code*/
  if(fgets(line, ncharead, fp) == NULL) return PSPIO_IOERR;
  narg = sscanf (line, "%d %d %d %d %d", &pspcod, &pspxc, &psp_data.lmax, &idum, &psp_data.mesh.np);
  ///check narg is equal to 5

  // inferred from abinit web site http://www.abinit.org/documentation/helpfiles/for-v6.8/users/abinit_help.html#5
  /* NOTE: pspcod = 5 is used also for new psp generated with the Martins code, including 2 projectors for spin orbit coupling */ 
  switch(pspcod){
    case 1:
    case 4:
    case 5:
    case 6:
      &psp_data.scheme = TM2;
      break;
    case 2:
      &psp_data.scheme = GTH;
      break;
    case 3:
      &psp_data.scheme = HGH;
      break;
    default:
      return PSPIO_VALUE_ERROR;
  }


  ierr = pspxc2libxc (pspxc, psp_data)
  if (ierr != PSPIO_SUCCESS) return ierr

  return PSPIO_SUCCESS;

}


