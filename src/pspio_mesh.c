/*
 Copyright (C) 2011-2012 J. Alberdi, M. Oliveira, Y. Pouillon, and M. Verstraete

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

*/

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "pspio_mesh.h"
#include "pspio_error.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

int pspio_mesh_alloc(pspio_mesh_t **mesh, const int np) {

  assert(mesh != NULL);
  assert(*mesh == NULL);
  assert(np > 1);

  // Memory allocation
  *mesh = (pspio_mesh_t *) malloc (sizeof(pspio_mesh_t));
  FULFILL_OR_EXIT( *mesh != NULL, PSPIO_ENOMEM );

  (*mesh)->r = NULL;
  (*mesh)->rab = NULL;

  (*mesh)->r = (double *) malloc (np * sizeof(double));
  FULFILL_OR_EXIT( (*mesh)->r != NULL, PSPIO_ENOMEM );

  (*mesh)->rab = (double *) malloc (np * sizeof(double));
  FULFILL_OR_EXIT( (*mesh)->rab != NULL, PSPIO_ENOMEM );

  // Presets
  (*mesh)->np = np;
  memset((*mesh)->r, 0, np*sizeof(double));
  memset((*mesh)->rab, 0, np*sizeof(double));
  (*mesh)->a = 0;
  (*mesh)->b = 0;
  (*mesh)->type = PSPIO_MESH_NONE;

  return PSPIO_SUCCESS;
}


int pspio_mesh_set(pspio_mesh_t **mesh, const int type, const double a, 
       const double b, const double *r, const double *rab) {
  assert((*mesh) != NULL);
  assert((*mesh)->r != NULL);
  assert((*mesh)->rab != NULL);

  (*mesh)->type = type;
  (*mesh)->a = a;
  (*mesh)->a = b;
  memcpy((*mesh)->r, r, (*mesh)->np * sizeof(double));
  memcpy((*mesh)->rab, rab, (*mesh)->np * sizeof(double));

  return PSPIO_SUCCESS;
}


int pspio_mesh_copy(pspio_mesh_t **dst, const pspio_mesh_t *src) {
  assert(src != NULL);

  if ( *dst == NULL ) {
    SUCCEED_OR_RETURN(pspio_mesh_alloc(dst, src->np));
  }

  (*dst)->type = src->type;
  (*dst)->a = src->a;
  (*dst)->a = src->b;
  memcpy((*dst)->r, src->r, src->np * sizeof(double));
  memcpy((*dst)->rab, src->rab, src->np * sizeof(double));

  return PSPIO_SUCCESS;
}


void pspio_mesh_init_from_points(pspio_mesh_t **mesh, const double *r, 
      const double *rab) {
  int i;
  double tol = 5.0e-10;

  assert(*mesh != NULL);
  assert((*mesh)->r != NULL);
  assert((*mesh)->rab != NULL);

  // Init mesh
  memcpy((*mesh)->r, r, (*mesh)->np * sizeof(double));
  if ( rab != NULL ) {
    memcpy((*mesh)->rab, rab, (*mesh)->np * sizeof(double));
  }
  (*mesh)->type = PSPIO_MESH_UNKNOWN;

  // Try linear mesh
  if ( (*mesh)->type == PSPIO_MESH_UNKNOWN ) {
    (*mesh)->a = r[1] - r[2];
    (*mesh)->b = r[0];
    if ( fabs(r[2] - (3.0*(*mesh)->a + (*mesh)->b)) < tol ) {
      (*mesh)->type = PSPIO_MESH_LINEAR;
      if ( rab != NULL ) {
        for (i=0; i<(*mesh)->np; i++) {
          if ( fabs(rab[i] - (*mesh)->a) > tol ) {
            (*mesh)->type = PSPIO_MESH_UNKNOWN;
            break;
          }
        }
      } else {
        for (i=0; i<(*mesh)->np; i++) {
          (*mesh)->rab[i] = (*mesh)->a;
        }
      }
    }
  }

  // Try log1 mesh
  if ( (*mesh)->type == PSPIO_MESH_UNKNOWN ) {
    (*mesh)->a = log(r[1]/r[0]);
    (*mesh)->b = r[0]/exp((*mesh)->a);
    if ( fabs(r[2] - ((*mesh)->b*exp((*mesh)->a*3.0))) < tol ) {
      (*mesh)->type = PSPIO_MESH_LOG1;
      if ( rab != NULL ) {
        for (i=0; i<(*mesh)->np; i++) {
          if ( fabs(rab[i] - (*mesh)->a) > tol ) {
            (*mesh)->type = PSPIO_MESH_UNKNOWN;
            break;
          }
        }
      } else {
        for (i=0; i<(*mesh)->np; i++) {
          (*mesh)->rab[i] = (*mesh)->a;
        }
      }
    }
  }

  // Try log2 mesh
  if ( (*mesh)->type == PSPIO_MESH_UNKNOWN ) {
    (*mesh)->a = log(r[1]/r[0] - 1.0);
    (*mesh)->b = r[0]/(exp((*mesh)->a) - 1.0);
    if ( fabs(r[2] - (*mesh)->b*(exp((*mesh)->a*3.0 - 1.0))) < tol ) {
      (*mesh)->type = PSPIO_MESH_LOG2;
      if ( rab != NULL ) {
        for (i=0; i<(*mesh)->np; i++) {
          if ( fabs(rab[i] - (*mesh)->a) > tol ) {
            (*mesh)->type = PSPIO_MESH_UNKNOWN;
            break;
          }
        }
      } else {
        for (i=0; i<(*mesh)->np; i++) {
          (*mesh)->rab[i] = (*mesh)->a;
        }
      }
    }
  }

  // Unable to determine mesh type
  if ( (*mesh)->type == PSPIO_MESH_UNKNOWN ) {
    (*mesh)->a = 0.0;
    (*mesh)->b = 0.0;
  }
}


void pspio_mesh_init_from_parameters(pspio_mesh_t **mesh, const int type, 
       const double a, const double b) {
  int i;

  assert(*mesh != NULL);
  assert((*mesh)->r != NULL);
  assert((*mesh)->rab != NULL);
  assert(type == PSPIO_MESH_LINEAR || type == PSPIO_MESH_LOG1 || type == PSPIO_MESH_LOG2);

  (*mesh)->type = type;
  (*mesh)->a = a;
  (*mesh)->b = b;

  for (i=0; i<(*mesh)->np; i++) {
    switch (type) {
    case PSPIO_MESH_LINEAR:
      (*mesh)->r[i] = a*(i+1) + b;
      (*mesh)->rab[i] = a;
      break;
    case PSPIO_MESH_LOG1:
      (*mesh)->r[i] = b*exp(a*(i+1));
      (*mesh)->rab[i] = a*(*mesh)->r[i];
      break;
    case PSPIO_MESH_LOG2:
      (*mesh)->r[i] = b*exp(a*(i+1) - 1.0);
      (*mesh)->rab[i] = a*(*mesh)->r[i];
      break;
    }
  }
}


void pspio_mesh_free(pspio_mesh_t **mesh) {

  if (*mesh != NULL) {
    free ((*mesh)->r);
    free ((*mesh)->rab);
    free (*mesh);
    *mesh = NULL;
  }
}


/**********************************************************************
 * Atomic routines                                                    *
 **********************************************************************/

int pspio_mesh_get_np(const pspio_mesh_t *mesh) {
  assert(mesh != NULL);

  return mesh->np;
}


void pspio_mesh_get_r(const pspio_mesh_t *mesh, double *r) {
  int i;

  assert(mesh != NULL);
  assert(r != NULL);

  for (i=0; i<mesh->np; i++) r[i] = mesh->r[i];
}


void pspio_mesh_get_rab(const pspio_mesh_t *mesh, double *rab) {
  int i;

  assert(mesh != NULL);
  assert(rab != NULL);

  for (i=0; i<mesh->np; i++) rab[i] = mesh->rab[i];
}
