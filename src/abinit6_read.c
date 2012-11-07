/*
 Copyright (C) 2012 M. Oliveira, Y. Pouillon

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

 $Id: abinit_read.c 263 2012-11-05 14:53:03Z micael $
*/

/** 
 * @file abinit_read.c
 * @brief routines to read UPF files 
 */
#include <string.h>
#include <ctype.h>
#include "abinit.h"
#include "util.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif

int abinit_read_info(FILE *fp, pspio_pspdata_t **pspdata){
  char line[MAX_STRLEN];
  int il;

  // Allocate memory
  (*pspdata)->info = (char *)malloc(1*sizeof(char));
  CHECK_ERROR((*pspdata)->info != NULL, PSPIO_ENOMEM);

  // Store the first 7 lines
  for (il=0; il<7; il++) {
    CHECK_ERROR(fgets(line, MAX_STRLEN, fp) != NULL, PSPIO_EIO);
    (*pspdata)->info = realloc((*pspdata)->info,
      strlen((*pspdata)->info) + strlen(line));
    CHECK_ERROR((*pspdata)->info != NULL, PSPIO_ENOMEM);
    strncat((*pspdata)->info, line, strlen(line));
  }

  return PSPIO_SUCCESS;
}

int abinit_read_header(FILE *fp, int *np, pspio_pspdata_t **pspdata){
  char line[MAX_STRLEN], buffer[MAX_STRLEN];
  char pspdat[6];
  int pspcod, pspxc, lmax, lloc, mmax, have_nlcc;
  int exchange, correlation, il;
  double zatom, zvalence, r2well, rchrg, fchrg, qchrg;

  // Line 1: skip title
  CHECK_ERROR( fgets(line, MAX_STRLEN, fp) != NULL, PSPIO_EIO);

  // Line 2: read atomic number, Z valence, psp date
  CHECK_ERROR( fgets(line, MAX_STRLEN, fp) != NULL, PSPIO_EIO);
  CHECK_ERROR( sscanf(line, "%f %f %s %s", 
    &zatom, &zval, pspdat, buffer ) == 1, PSPIO_EIO);
  (*pspdata)->z = zatom;
  (*pspdata)->zvalence = zval;
  (*pspdata)->nelvalence = zval;

  // Line 3: read pspcod, pspxc, lmax, lloc, mmax, r2well
  CHECK_ERROR( fgets(line, MAX_STRLEN, fp) != NULL, PSPIO_EIO);
  CHECK_ERROR( sscanf(line, "%d %d %d %d %d %f %s",
    &pspcod, &pspxc, &lmax, &lloc, &mmax, &r2well, buffer) == 1, PSPIO_EIO);
  (*pspdata)->scheme = pspcod;
  (*pspdata)->l_max = lmax;
  (*pspdata)->l_local = lloc;
  *np = mmax;
  if ( pspxc >= 1000 ) {
    exchange = pspxc / 1000;
    correlation = pspxc % 1000;
  }

  // Line 4: read rchrg, fchrg, qchrg if NLCC
  CHECK_ERROR( fgets(line, MAX_STRLEN, fp) != NULL, PSPIO_EIO);
  have_nlcc = 0;
  if ( strcmp("4--", line[0:2]) != 0 ) {
    CHECK_ERROR( sscanf(line, "%d %d %d %d %d %f %s",
      &rchrg, &fchrg, &qchrg, buffer) == 1, PSPIO_EIO );
    if ( abs(fchrg) >= 1.0e-14 ) {
      have_nlcc = 1;
    }
  }

  // Skip lines 5 to 18
  for(il=5; il<=18; il++) {
    CHECK_ERROR( fgets(line, MAX_STRLEN, fp) != NULL, PSPIO_EIO);
  }

  // Initialize xc
  if ( have_nlcc == 0) {
    HANDLE_FUNC_ERROR(pspio_xc_alloc(&(*pspdata)->xc, PSPIO_NLCC_NONE, *np));
  } else {
    HANDLE_FUNC_ERROR(pspio_xc_alloc(&(*pspdata)->xc, PSPIO_NLCC_UNKNOWN, *np));
  }
  HANDLE_FUNC_ERROR(pspio_xc_set(&(*pspdata)->xc, exchange, correlation));

  return PSPIO_SUCCESS;
}


int abinit_read_mesh(FILE *fp, const int np, pspio_pspdata_t **pspdata){
  char line[MAX_STRLEN];
  int il, ip, jl, lloc, lmax, mmax;
  double amesh, ri, di, vnli, wrci;
  double *r, *drdi, *vnl, *vloc, *wrc;

  // Init parameters for Abinit
  (*pspdata)->mesh->type = PSPIO_MESH_LINEAR;
  lloc = (*pspdata)->l_local;
  lmax = (*pspdata)->l_max;
  di = 0.0;

  // Allocate memory
  r = (double *)malloc(np*sizeof(double)); // Mesh
  CHECK_ERROR(r != NULL, PSPIO_ENOMEM);
  drdi = (double *)malloc(np*sizeof(double)); // Mesh derivative
  CHECK_ERROR(drdi != NULL, PSPIO_ENOMEM);
  vloc = (double *)malloc(np*sizeof(double)); // Local part
  CHECK_ERROR(vloc != NULL, PSPIO_ENOMEM);
  vnl = (double *)malloc((lmax+1)*np*sizeof(double)); // Non-local part
  CHECK_ERROR(vnl != NULL, PSPIO_ENOMEM);
  wrc = (double *)malloc((lmax+1)*np*sizeof(double)); // Wavefunctions
  CHECK_ERROR(wrc != NULL, PSPIO_ENOMEM);

  //Read mesh points, non-local part, and wavefunctions
  for (jl=0; jl<=lmax; jl++) {
    CHECK_ERROR( fgets(line, MAX_STRLEN, fp) != NULL, PSPIO_EIO);
    CHECK_ERROR( sscanf(line, "%d %f", &mmax, &amesh) == 1, PSPIO_EIO );
    for (il=0; il<np; il++) {
      CHECK_ERROR( fgets(line, MAX_STRLEN, fp) != NULL, PSPIO_EIO);
      CHECK_ERROR( sscanf(line, "%d %f %f %f",
        &ip, &ri, &wrci, &vnli) == 1, PSPIO_EIO );
      if ( j == 0 ) {
        r[il] = ri;
        if ( il == 1 ) {
          di = ri - r[0];
        }
        drdi[il] = di;
      }
      wrc[jl][il] = wrci;
      vnl[jl][il] = vnli;
      if ( j == lloc ) {
        vloc[il] = vnli;
      }
    }
  }
  drdi[0] = di;

  // Store the mesh in the pspdata structure
  HANDLE_FUNC_ERROR(pspio_mesh_alloc(&(*pspdata)->mesh, np));
  HANDLE_FUNC_ERROR(pspio_mesh_init_from_points(&(*pspdata)->mesh, r, drdi));

  // Store the local potential in the pspdata structure
  //HANDLE_FUNC_ERROR(pspio_mesh_alloc(&(*pspdata)->mesh, np));
  //HANDLE_FUNC_ERROR(pspio_mesh_init_from_points(&(*pspdata)->mesh, r, drdi));

  // Store the non-local potential in the pspdata structure
  //HANDLE_FUNC_ERROR(pspio_mesh_alloc(&(*pspdata)->mesh, np));
  //HANDLE_FUNC_ERROR(pspio_mesh_init_from_points(&(*pspdata)->mesh, r, drdi));

  // Store the reference wavefunctions in the pspdata structure
  //HANDLE_FUNC_ERROR(pspio_mesh_alloc(&(*pspdata)->mesh, np));
  //HANDLE_FUNC_ERROR(pspio_mesh_init_from_points(&(*pspdata)->mesh, r, drdi));

  //Free memory
  free(r);
  free(drdi);
  free(vloc);
  free(vnl);
  free(wrc);

  return PSPIO_SUCCESS;
}
