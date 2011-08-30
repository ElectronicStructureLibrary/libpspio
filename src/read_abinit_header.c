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

/** subroutine to read in abinit header which is generic to all psp files for abinit */

#include <stdio.h>
#include <string.h>
#include "pspio_error.h"
#include "pspio.h"

/**
* subroutine reads in full abinit header file and initializes psp_data structure with available stuff
*@param[in]  fname      file name to be read in 
*@param[out] psp_data   pseudopotential info is partially filled in present routine 
*@param[out] pspcod     pseudopotential file type (HGH GTH fhi etc...)
*/
int read_abinit_header (FILE *fp, pspio_pspdata_t *psp_data, int pspcod){

  /// local variables
  int ierr;
  int idum;
  int narg;
  int minlen;
  int ncharead = 1000;
  int pspxc; 
  char line[ncharead];
  char *testread;

  /**< read in title */
  if (fgets(line, ncharead, fp) == NULL) return PSPIO_EIO;
  minlen = strlen(line) > STRLEN_TITLE ? STRLEN_TITLE : strlen(line);
  if (strncpy((*psp_data).title, line, minlen) == NULL){
    return PSPIO_EIO;
  }
 
  /**< read in atomic number, pseudopotential ion charge (= num of valence electrons), and abinit data flag (not used) */
  if(fgets(line, ncharead, fp) == NULL) return PSPIO_EIO;
  narg = sscanf (line, "%f %f %d", &((*psp_data).z), &((*psp_data).zvalence), &idum);
  ///check narg is equal to 2
 
  /**< read in psp code and xc code*/
  if(fgets(line, ncharead, fp) == NULL) return PSPIO_EIO;
  narg = sscanf (line, "%d %d %d %d %d", &pspcod, &pspxc, &((*psp_data).l_max), &((*psp_data).l_local), &((*(*psp_data).mesh).np));
  ///check narg is equal to 5

  // inferred from abinit web site http://www.abinit.org/documentation/helpfiles/for-v6.8/users/abinit_help.html#5
  /* NOTE: pspcod = 5 is used also for new psp generated with the Martins code, including 2 projectors for spin orbit coupling */ 
  switch(pspcod){
    case 1:
    case 4:
    case 5:
    case 6:
      (*psp_data).scheme = TM2;
      break;
    case 2:
      (*psp_data).scheme = GTH;
      break;
    case 3:
      (*psp_data).scheme = HGH;
      break;
    default:
      return PSPIO_EVALUE;
  }


  ierr = ab2libxc (pspxc, psp_data);
  if (ierr != PSPIO_SUCCESS) return ierr;


  return PSPIO_SUCCESS;

}




//  double rchrg;
//  double fchrg;
//  double qchrg;
  /**< read in NLCC parameters */
//  if(fgets(line, ncharead, fp) == NULL) return PSPIO_EIO;
//  narg = sscanf (line, "%f %f %f", &rchrg, &fchrg, &qchrg);
  ///check narg is equal to 3
  
 /**< initialize nlcc according to pspcod */
