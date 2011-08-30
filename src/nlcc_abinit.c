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

/** subroutine to make NLCC core charge in abinit psp format 4  */

#include <stdio.h>
#include <string.h>

#include "pspio_error.h"
#include "pspio.h"
#include "pspio_nlcc.h"


/**
* function returns value of polynomial in NLCC core charge density for abinit format pspcod 4
*@param[in]  x          value of input - should be between 0 and 1 in normal usage
*@param[out] y          output value of polynomial
*/
int nlcc_ab4(double x, double y){

  const double a2 =-0.8480751097855989e1;
  const double a4 =+0.9684600878284791e2;
  const double a6 =-0.7490894651588015e3;
  const double a8 =+0.3670890998130434e4;
  const double a10=-0.1156854803757563e5;
  const double a12=+0.2371534625455588e5;
  const double a14=-0.3138755797827918e5;
  const double a16=+0.2582842713241039e5;
  const double a18=-0.1200356429115204e5;
  const double a20=+0.2405099057118771e4;

  if (x > 1.0 || x < 0.0) return PSPIO_EVALUE;

/// NLCC Expression of 7 May 1992
//  y = (1.e0+pow(x,2)*(a2 +pow(x,2)*(a4 +pow(x,2)*(a6 +pow(x,2)*(a8 + pow(x,2)*(a10+pow(x,2)*(a12+pow(x,2)*(a14+pow(x,2)*(a16+pow(x,2)*(a18+pow(x,2)*(a20))))))))))) * pow((1.0e0-pow(x,2)),2);
  y =              a20;
  y = y*pow(x,2) + a18;
  y = y*pow(x,2) + a18;
  y = y*pow(x,2) + a16;
  y = y*pow(x,2) + a14;
  y = y*pow(x,2) + a12;
  y = y*pow(x,2) + a10;
  y = y*pow(x,2) + a8 ;
  y = y*pow(x,2) + a6 ;
  y = y*pow(x,2) + a4 ;
  y = y*pow(x,2) + a2 ;
  y = y*pow(x,2) + 1.e0;
  y *= pow((1.0e0-pow(x,2)),2);

  return PSPIO_SUCCESS;
}

/**
* subroutine accepts fchrg and rchrg and makes the non linear core correction model density on the corresponding grid
*  for abinit4 we have a 24th order polynomial
*@param[in]  rchrg      radius of core charge
*@param[in]  fchrg      amplitude of core charge
*@param[out] psp_data   pseudopotential info is filled in present routine and subroutines
*/
int nlcc_abinit4 (pspio_nlcc_t *nlcc, double rchrg, double fchrg){

  /// local variables
  int ierr;
  int ir;
  int np;
  double fftmp;
  double a;
  double b;
  double *rr;
  double *ff;
  pspio_mesh_t nlccmesh;

  /// init the mesh
  np = 2501;  // this is the abinit default, used with analytical core charges. Could be modified to use the potential's mesh, but there can be numerical problems with not using a linear mesh.

  ierr = pspio_mesh_alloc(nlccmesh, np);
  
/**< the following could become a new linear init mode for mesh */
  a = 1.0e0 / ((double) np - 1);
  b = 0.0e0;
  for (ir = 0; ir < ; ir++){
    rr[ir]
  }
  ierr = pspio_mesh_set(nlccmesh, MESH_LINEAR, a, b, rr);
  free(rr)

  /// allocate the nlcc object
  ierr = pspio_nlcc_alloc(nlcc, np);

  ff = (double *) malloc(sizeof(nlcc->cdens->f))

  if (np < 2) return PSPIO_EVALUE;

  /// fill the nlcc core density
  for (ir=0;  ir < nlcc->cdens->mesh->np; ir++){
    ierr = nlcc_ab4 (((double)ir)/((double)nlcc->cdens->mesh->np-1), fftmp);
    if (ierr) return ierr;
    ff[ir]=fchrg * fftmp;
  }

  ierr = pspio_nlcc_set(pspio_nlcc_t *nlcc, mesh, ff);
  if (ierr) return ierr;
  
  free(ff);

  return PSPIO_SUCCESS;

}



