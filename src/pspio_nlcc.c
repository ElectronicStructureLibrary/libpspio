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

#include "pspio_nlcc.h"
#include <stdlib.h>

int pspio_nlcc_alloc(pspio_nlcc_t *nlcc, pspio_mesh_t *mesh){
  int i, ierr;
  
  nlcc = (pspio_nlcc_t *) malloc (sizeof(pspio_nlcc_t));

  if (nlcc == NULL) {
    return PSPIO_ENOMEM;
  }

  ierr = pspio_meshfunc_alloc(nlcc->core_dens, mesh);
  if (!ierr) {
    free(nlcc);
    return ierr;
  }

  return PSPIO_SUCCESS;
}


int pspio_nlcc_set(pspio_nlcc_t *nlcc, int scheme, double *core_dens){
  int ierr;

  nlcc->scheme = scheme;

  ierr = pspio_meshfunc_set(nlcc->core_dens, core_dens);
  if (!ierr) {
    return ierr;
  }

  return PSPIO_SUCCESS;
}


int pspio_nlcc_free(pspio_nlcc_t *nlcc){

  if (nlcc != NULL) {
    pspio_meshfunc_free(nlcc->core_dens);
    free(nlcc);
  }

  pspio_meshfunc_free(nlcc->core_dens);
  free(nlcc);

}
