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

#include <stdlib.h>

#include "pspio_projector.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

int pspio_projector_alloc(pspio_projector_t *projector, const int np){
  int ierr;

  ASSERT (np > 1, PSPIO_EVALUE);

  projector = (pspio_projector_t *) malloc (sizeof(pspio_projector_t));
  HANDLE_FATAL_ERROR (projector == NULL, PSPIO_ENOMEM);

  ierr = pspio_meshfunc_alloc(projector->proj, np);
  if (ierr) {
    pspio_projector_free(projector);
    HANDLE_ERROR (ierr);
  }
   
  ierr = pspio_qn_alloc(projector->qn);
  if (ierr) {
    pspio_projector_free(projector);
    HANDLE_ERROR (ierr);
  }

  return PSPIO_SUCCESS;
}


int pspio_projector_set(pspio_projector_t *projector, const pspio_qn_t *qn, 
			const double e, const pspio_mesh_t *mesh, const double *p){

  HANDLE_FUNC_ERROR (pspio_qn_copy(projector->qn, qn));
  projector->energy = e;
  HANDLE_FUNC_ERROR (pspio_meshfunc_set(projector->proj, mesh, p));

  return PSPIO_SUCCESS;
}


int pspio_projector_free(pspio_projector_t *projector){

  if (projector != NULL) {
    pspio_meshfunc_free(projector->proj);
    pspio_qn_free(projector->qn);
    free(projector);
  }

  return PSPIO_SUCCESS;
}



/**********************************************************************
 * Atomic routines                                                    *
 **********************************************************************/

int pspio_projector_get_qn(pspio_projector_t *projector, pspio_qn_t *qn){
  ASSERT((projector != NULL) && (projector->qn != NULL), PSPIO_ERROR);

  HANDLE_FUNC_ERROR(pspio_qn_copy(projector->qn, qn));

  return PSPIO_SUCCESS;
}

