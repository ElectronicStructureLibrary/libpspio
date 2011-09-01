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
#include <string.h>
#include <math.h>

#include "pspio_mesh.h"
#include "pspio_error.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


int pspio_mesh_alloc(pspio_mesh_t *mesh, const int np){
  int i;

  ASSERT(np > 1, PSPIO_EVALUE);

  // Memory allocation
  mesh = (pspio_mesh_t *) malloc (sizeof(pspio_mesh_t));
  HANDLE_FATAL_ERROR (mesh == NULL, PSPIO_ENOMEM);

  mesh->r = (double *) malloc (np * sizeof(double));
  HANDLE_FATAL_ERROR (mesh->r == NULL, PSPIO_ENOMEM);

  // Presets
  mesh->np = np;
  for (i = 0; i < mesh->np; i++)
    {
      mesh->r[i] = 0;
    }
  mesh->a = 0;
  mesh->b = 0;
  mesh->type = MESH_NONE;

  return PSPIO_SUCCESS;
}


int pspio_mesh_set(pspio_mesh_t *mesh, const int type, const double a, 
		   const double b, const double *r){
  ASSERT( mesh != NULL, PSPIO_ERROR);
  
  mesh->type = type;
  mesh->a = a;
  mesh->a = b;
  memcpy(mesh->r, r, mesh->np * sizeof(double));

  return PSPIO_SUCCESS;
}


int pspio_mesh_copy(pspio_mesh_t *dst, const pspio_mesh_t *src){
  ASSERT (src != NULL, PSPIO_ERROR);

  if (dst == NULL) {
    HANDLE_FUNC_ERROR (pspio_mesh_alloc(dst, src->np));
  }

  dst->type = src->type;
  dst->a = src->a;
  dst->a = src->b;
  memcpy(dst->r, src->r, src->np * sizeof(double));

  return PSPIO_SUCCESS;
}


int pspio_mesh_init_from_points(pspio_mesh_t *mesh, const double *r) {
  ASSERT( mesh != NULL, PSPIO_ERROR);

  memcpy(mesh->r, r, mesh->np * sizeof(double));

  // Try linear mesh
  mesh->a = r[1] - r[2];
  mesh->b = r[0];
  if (abs(r[mesh->np] - mesh->np * mesh->a + mesh->b) < 1e-16) {
    mesh->type = MESH_LINEAR;
    return PSPIO_SUCCESS;
  }

  // Try log1 mesh
  mesh->a = log(r[1]/r[0]);
  mesh->b = r[0]/exp(mesh->a);
  if (abs(r[mesh->np] - mesh->b*exp(mesh->a*mesh->np)) < 1e-16 ) {
    mesh->type = MESH_LOG1;
    return PSPIO_SUCCESS;
  }

  // Try log2 mesh
  mesh->a = log(r[1]/r[0] - 1.0);
  mesh->b = r[0]/(exp(mesh->a) - 1.0);
  if (abs(r[mesh->np] - mesh->b*(exp(mesh->a*mesh->np) - 1.0)) < 1e-16 ) {
    mesh->type = MESH_LOG2;
    return PSPIO_SUCCESS;
  }

  // Unable to determine mesh type
  mesh->type = MESH_UNKNOWN;
  mesh->a = 0.0;
  mesh->b = 0.0;
  return PSPIO_SUCCESS;
}


int pspio_mesh_init_from_parameters(pspio_mesh_t *mesh, const int type, 
				    const double a, const double b) {
  int i;

  ASSERT(mesh != NULL, PSPIO_ERROR);
  ASSERT(type != MESH_UNKNOWN, PSPIO_EVALUE);

  mesh->type = type;
  mesh->a = a;
  mesh->b = b;

  for (i=0; i<mesh->np; i++) {
    switch (type) {
    case MESH_LINEAR:
      mesh->r[i] = a*(i+1) + b;
      break;
    case MESH_LOG1:
      mesh->r[i] = b*exp(a*(i+1));
      break;
    case MESH_LOG2:
      mesh->r[i] = b*exp(a*(i+1) - 1.0);
      break;
    }
  }

  return PSPIO_SUCCESS;
}


int pspio_mesh_free(pspio_mesh_t *mesh){

  if (mesh != NULL) {
    if (mesh->r != NULL) free (mesh->r);
    free (mesh);
  }

  return PSPIO_SUCCESS;
}

