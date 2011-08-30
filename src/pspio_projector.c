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

#include "pspio_projector.h"
#include <stdlib.h>

int pspio_projector_alloc(pspio_projector_t *projector, pspio_mesh_t *mesh){
  int i, ierr;
  
  projector = (pspio_projector_t *) malloc (sizeof(pspio_projector_t));

  if (projector == NULL) {
    return PSPIO_ENOMEM;
  }

  ierr = pspio_meshfunc_alloc(projector->p, mesh);
  if (!ierr) {
    free(projector);
    return ierr;
  }

  ierr = pspio_qn_alloc(projector->qn);
  if (ierr) {
    pspio_meshfunc_free(projector->p);
    free(projector);
    return ierr;
  }

  return PSPIO_SUCCESS;
}


int pspio_projector_set(pspio_projector_t *projector, pspio_qn_t *qn, double e, double *p){
  int ierr;

  ierr = pspio_qn_copy(qn, projector->qn);
  if (!ierr) {
    return ierr;
  }
  
  ierr = pspio_meshfunc_set(projector->p, p);
  if (!ierr) {
    return ierr;
  }

  return PSPIO_SUCCESS;
}


void pspio_projector_free(pspio_projector_t *projector){

  if (projector == NULL) {
    return;
  }

  pspio_meshfunc_free(projector->p);
  free(projector);

}
