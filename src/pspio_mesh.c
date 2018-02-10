/* Copyright (C) 2011-2016 Joseba Alberdi <alberdi@hotmail.es>
 *                         Matthieu Verstraete <matthieu.jean.verstraete@gmail.com>
 *                         Micael Oliveira <micael.oliveira@mpsd.mpg.de>
 *                         Yann Pouillon <devops@materialsevolution.es>
 *
 * This file is part of Libpspio.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * version 2.0. If a copy of the MPL was not distributed with this file, You
 * can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Libpspio is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the Mozilla Public License version 2.0 for
 * more details.
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "pspio_mesh.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

int pspio_mesh_alloc(pspio_mesh_t **mesh, int np)
{
  assert(mesh != NULL);
  assert(*mesh == NULL);
  assert(np > 1);

  /* Memory allocation */
  *mesh = (pspio_mesh_t *) malloc (sizeof(pspio_mesh_t));
  FULFILL_OR_EXIT( *mesh != NULL, PSPIO_ENOMEM );

  (*mesh)->r = NULL;
  (*mesh)->rab = NULL;

  (*mesh)->r = (double *) malloc (np * sizeof(double));
  FULFILL_OR_EXIT( (*mesh)->r != NULL, PSPIO_ENOMEM );

  (*mesh)->rab = (double *) malloc (np * sizeof(double));
  FULFILL_OR_EXIT( (*mesh)->rab != NULL, PSPIO_ENOMEM );

  /* Presets */
  (*mesh)->np = np;
  memset((*mesh)->r, 0, np*sizeof(double));
  memset((*mesh)->rab, 0, np*sizeof(double));
  (*mesh)->a = 0;
  (*mesh)->b = 0;
  (*mesh)->type = PSPIO_MESH_NONE;

  return PSPIO_SUCCESS;
}

int pspio_mesh_init(pspio_mesh_t *mesh, int type, double a,
		    double b, const double *r, const double *rab)
{
  assert(mesh != NULL);
  assert(mesh->r != NULL);
  assert(mesh->rab != NULL);

  mesh->type = type;
  mesh->a = a;
  mesh->b = b;
  memcpy(mesh->r, r, mesh->np * sizeof(double));
  memcpy(mesh->rab, rab, mesh->np * sizeof(double));

  return PSPIO_SUCCESS;
}

void pspio_mesh_init_from_parameters(pspio_mesh_t *mesh, int type,
				     double a, double b)
{
  int i;

  assert(mesh != NULL);
  assert(mesh->r != NULL);
  assert(mesh->rab != NULL);
  assert(type == PSPIO_MESH_LINEAR || type == PSPIO_MESH_LOG1 || type == PSPIO_MESH_LOG2);

  mesh->type = type;
  mesh->a = a;
  mesh->b = b;

  for (i=0; i<mesh->np; i++) {
    switch (type) {
    case PSPIO_MESH_LINEAR:
      mesh->r[i] = a*(i+1) + b;
      mesh->rab[i] = a;
      break;
    case PSPIO_MESH_LOG1:
      mesh->r[i] = b*exp(a*(i+1));
      mesh->rab[i] = a*mesh->r[i];
      break;
    case PSPIO_MESH_LOG2:
      mesh->r[i] = b*(exp(a*(i+1)) - 1.0);
      mesh->rab[i] = a*mesh->r[i] + a*b;
      break;
    }
  }
}

void pspio_mesh_init_from_points(pspio_mesh_t *mesh, const double *r, const double *rab)
{
  int i;
  double tol = 1.0e-10;

  assert(r != NULL);
  assert(mesh != NULL);
  assert(mesh->r != NULL);
  assert(mesh->rab != NULL);

  /* Init mesh */
  memcpy(mesh->r, r, mesh->np * sizeof(double));
  if ( rab != NULL ) {
    memcpy(mesh->rab, rab, mesh->np * sizeof(double));
  }
  mesh->type = PSPIO_MESH_UNKNOWN;

  /* Try linear mesh */
  if ( mesh->type == PSPIO_MESH_UNKNOWN ) {
    mesh->a = r[1] - r[0];
    mesh->b = r[0] - mesh->a;
    mesh->type = PSPIO_MESH_LINEAR;
    for (i=0; i<mesh->np; i++) {
      if ( fabs( (r[i] - (mesh->a*(i+1) + mesh->b))/r[i] ) > tol ) {
      	mesh->type = PSPIO_MESH_UNKNOWN;
        break;
      }
    }
    if ( rab != NULL ) {
      for (i=0; i<mesh->np; i++) {
	      if ( fabs( (rab[i] - mesh->a)/rab[i] ) > tol ) {
	        mesh->type = PSPIO_MESH_UNKNOWN;
	        break;
	      }
      }
    } else if (mesh->type == PSPIO_MESH_LINEAR) {
      for (i=0; i<mesh->np; i++) {
	      mesh->rab[i] = mesh->a;
      }
    }
  }

  /* Try log1 mesh */
  if ( mesh->type == PSPIO_MESH_UNKNOWN ) {
    mesh->a = log(r[1]/r[0]);
    mesh->b = r[0]/exp(mesh->a);
    mesh->type = PSPIO_MESH_LOG1;
    for (i=0; i<mesh->np; i++) {
      if ( fabs( (r[i] - (mesh->b*exp(mesh->a*(i+1))))/r[i] ) > tol ) {
      	mesh->type = PSPIO_MESH_UNKNOWN;
	      break;
      }
    }
    if ( rab != NULL ) {
      for (i=0; i<mesh->np; i++) {
	      if ( fabs( (rab[i] - mesh->a*mesh->r[i])/rab[i] ) > tol ) {
	        mesh->type = PSPIO_MESH_UNKNOWN;
	        break;
	      }
      }
    } else if ( mesh->type == PSPIO_MESH_LOG1 ) {
      for (i=0; i<mesh->np; i++) {
	      mesh->rab[i] = mesh->a*mesh->r[i];
      }
    }
  }

  /* Try log2 mesh */
  if ( mesh->type == PSPIO_MESH_UNKNOWN ) {
    mesh->b = (r[1]*r[1] - r[2]*r[0])/(r[0] + r[2] - 2*r[1]);
    mesh->a = log( (r[1] + mesh->b)/(r[0] + mesh->b) );
    mesh->type = PSPIO_MESH_LOG2;
    for (i=0; i<mesh->np; i++) {
      if ( fabs( (r[i] - (mesh->b*(exp(mesh->a*(i+1)) - 1.0)))/r[i] ) > tol ) {
      	mesh->type = PSPIO_MESH_UNKNOWN;
	      break;
      }
    }
    if ( rab != NULL ) {
      for (i=0; i<mesh->np; i++) {
	      if ( fabs( (rab[i] - (mesh->a*mesh->r[i] + mesh->a*mesh->b))/rab[i] ) > tol ) {
	        mesh->type = PSPIO_MESH_UNKNOWN;
	        break;
	      }
      }
    } else if ( mesh->type == PSPIO_MESH_LOG2 ) {
      for (i=0; i<mesh->np; i++) {
	      mesh->rab[i] = mesh->a*mesh->r[i] + mesh->a*mesh->b;
      }
    }
  }

  /* Unable to determine mesh type */
  if ( mesh->type == PSPIO_MESH_UNKNOWN ) {
    mesh->a = 0.0;
    mesh->b = 0.0;
  }
}

int pspio_mesh_copy(pspio_mesh_t **dst, const pspio_mesh_t *src)
{
  assert(src != NULL);

  if ( *dst == NULL ) {
    SUCCEED_OR_RETURN(pspio_mesh_alloc(dst, src->np));
  }

  /* The destination mesh must have the same number of points as the source mesh */
  if ( (*dst)->np != src->np ) {
    pspio_mesh_free(*dst);
    *dst = NULL;
    SUCCEED_OR_RETURN(pspio_mesh_alloc(dst, src->np));
  }

  (*dst)->type = src->type;
  (*dst)->a = src->a;
  (*dst)->b = src->b;
  memcpy((*dst)->r, src->r, src->np * sizeof(double));
  memcpy((*dst)->rab, src->rab, src->np * sizeof(double));

  return PSPIO_SUCCESS;
}

void pspio_mesh_free(pspio_mesh_t *mesh)
{
  if (mesh != NULL) {
    free (mesh->r);
    free (mesh->rab);
    free (mesh);
  }
}


/**********************************************************************
 * Getters                                                            *
 **********************************************************************/

int pspio_mesh_get_np(const pspio_mesh_t *mesh)
{
  assert(mesh != NULL);

  return mesh->np;
}

double pspio_mesh_get_a(const pspio_mesh_t *mesh)
{
  assert(mesh != NULL);

  return mesh->a;
}

double pspio_mesh_get_b(const pspio_mesh_t *mesh)
{
  assert(mesh != NULL);

  return mesh->b;
}

const double *pspio_mesh_get_r(const pspio_mesh_t *mesh)
{
  assert(mesh != NULL);

  return mesh->r;
}

const double *pspio_mesh_get_rab(const pspio_mesh_t *mesh)
{
  assert(mesh != NULL);

  return mesh->rab;
}


/**********************************************************************
 * Utility routines                                                   *
 **********************************************************************/

int pspio_mesh_cmp(const pspio_mesh_t *mesh1, const pspio_mesh_t *mesh2)
{
  int i, np, result;

  assert((mesh1 != NULL) && (mesh2 != NULL));

  result = mesh1->np == mesh2->np ? PSPIO_EQUAL : PSPIO_MTEQUAL;

  if ( (mesh1->type == PSPIO_MESH_UNKNOWN) && (mesh2->type == PSPIO_MESH_UNKNOWN) ) {
    /* We need to compare the points explicitly */
    np = mesh1->np < mesh2->np ? mesh2->np : mesh1->np;
    for (i=0; i<np; i++) {
      if ( mesh1->r[i] != mesh2->r[i] ) return PSPIO_DIFF;
    }
    return result;
  } else if ( (mesh1->type == mesh2->type) &&
	      (fabs(mesh2->a - mesh1->a) < 1.0e-10) && 
	      (fabs(mesh2->b - mesh1->b) < 1.0e-10) ) {
    return result;
  } else {
    return PSPIO_DIFF;
  }
}
