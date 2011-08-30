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

 $Id: nlcc_abinit.c 60 2011-08-30 10:48:58Z mjv500 $
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/** subroutine to make NLCC core charge in abinit psp format 4  */

#include <stdio.h>
#include <string.h>

#include "pspio_error.h"
#include "pspio.h"

/**
* subroutine accepts fchrg and rchrg and makes the non linear core correction model density on the corresponding grid
*  for abinit4 we have a 24th order polynomial
*@param[in]  rchrg      radius of core charge
*@param[in]  fchrg      amplitude of core charge
*@param[out] psp_data   pseudopotential info is filled in present routine and subroutines
*/
int nlcc_abinit4 (FILE *fp, pspio_pspdata_t *psp_data, ){

  /// local variables
  int ierr;
  int idum;
  int narg;
  int pspcod; 
  int pspxc; 
  char line[MAX_STRLEN];
  char *testread;


 

  return PSPIO_SUCCESS;

}
