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

/**
 * @file pspio_mesh.h
 * @brief header file for the handling of the mesh 
 */

#ifndef PSPIO_MESH_H
#define PSPIO_MESH_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "pspio_error.h"


/**
* Mesh types
*/
#define MESH_NONE   0
#define MESH_LOG1   1
#define MESH_LOG2   2
#define MESH_LINEAR 3

/**
* Mesh structure
*/
typedef struct{
  int type;    /**< Type of mesh */
  double a, b; /**< Mesh parameters */
  int np;      /**< Number of points in mesh */
  double *r;   /**< Mesh points */
} pspio_mesh_t;


/**
 * Allocates memory and preset mesh structure
 * 
 * @param[in,out] mesh: mesh structure
 * @param[in] np: number of points in the mesh
 * @return error code
 */
int pspio_mesh_alloc(pspio_mesh_t *m, int np);


/**
 * Sets the mesh data.
 * @param[in,out] mesh: mesh structure to set
 * @param[in] type: type of mesh. Can be LOG1, LOG2, or LINEAR.
 * @param[in] a: parameter a. The meaning depends on the type of mesh.
 * @param[in] b: parameter b. The meaning depends on the type of mesh.
 * @param[in] r: pointer to mesh radial points. Should be of size m->np.
 * @return error code
 */
int pspio_mesh_set(pspio_mesh_t *m, const int type, const double a, const double b, double *r);


/**
 * Frees all memory associated with mesh structure
 * 
 * @param[in,out] mesh: mesh structure
 */
void pspio_mesh_free(pspio_mesh_t *m);

#endif
