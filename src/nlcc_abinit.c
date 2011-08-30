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
#include "pspio_nlcc.h"

#define A2 =-0.8480751097855989e1
#define A4 =+0.9684600878284791e2
#define A6 =-0.7490894651588015e3
#define A8 =+0.3670890998130434e4
#define A10=-0.1156854803757563e5
#define A12=+0.2371534625455588e5
#define A14=-0.3138755797827918e5
#define A16=+0.2582842713241039e5
#define A18=-0.1200356429115204e5
#define A20=+0.2405099057118771e4

/// NLCC Expression of 7 May 1992
  gg(x)=(1.d0+x**2*(a2 +x**2*(a4 +x**2*(a6 +x**2*(a8 + x**2*(a10+x**2*(a12+x**2*(a14+x**2*(a16+x**2*(a18+x**2*(a20)))))))))))          *(1.0e0-x**2)**2

/**
* subroutine accepts fchrg and rchrg and makes the non linear core correction model density on the corresponding grid
*  for abinit4 we have a 24th order polynomial
*@param[in]  rchrg      radius of core charge
*@param[in]  fchrg      amplitude of core charge
*@param[out] psp_data   pseudopotential info is filled in present routine and subroutines
*/
int nlcc_abinit4 (pspio_nlcc_t *nlcc, rchrg, fchrg){

  /// local variables
  int ierr;
  int ir;
  double *ff;

  /// init the mesh
  

  /// allocate the nlcc object
  pspio_nlcc_alloc(pspio_nlcc_t *nlcc, pspio_mesh_t *m);

  /// fill the nlcc core density
  ff = (double *) malloc(sizeof(nlcc->cdens->f))
  for (ir=0;  ir < nlcc->cdens->mesh->np; ir++){
    ff[ir]=fchrg * gg(((double)ir)/((double)());
  }
  
  free(ff);

  return PSPIO_SUCCESS;

}
