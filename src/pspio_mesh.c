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

#include "pspio_mesh.h"
#include "pspio_error.h"
#include <stdlib.h>

int pspio_mesh_alloc(pspio_mesh_t *m, const int np){
  int i;

  if (np < 2) {
    return PSPIO_ERROR;
  }

  m = (pspio_mesh_t *) malloc (sizeof(pspio_mesh_t));

  if (m == NULL) {
    return PSPIO_ENOMEM;
  }

  m->r = (double *) malloc (np * sizeof(double));

  if (m->r == NULL) {
    free (m);
    return PSPIO_ENOMEM;
  }

  for (i = 0; i < m->np; i++)
    {
      m->r[i] = 0;
    }
  m->a = 0;
  m->b = 0;
  m->type = MESH_NONE;

  return PSPIO_SUCCESS;
}


int pspio_mesh_set(pspio_mesh_t *m, const int type, const double a, const double b, double *r){
  int i;
  
  m->type = type;  
  m->a = a;
  m->a = b;

  for (i = 0; i < m->np; i++)
    {
      m->r[i] = r[i];
    }

  return PSPIO_SUCCESS;
}


int pspio_mesh_free(pspio_mesh_t *m){

  if (m != NULL) {
    if (m->r != NULL) free (m->r);
    free (m);
  }

  return PSPIO_SUCCESS;
}
