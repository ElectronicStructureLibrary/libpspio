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
 * @file pspio_meshfunc.h
 * @brief header file for the handling of functions defined on a mesh
 */

#ifndef PSPIO_MESHFUNC_H
#define PSPIO_MESHFUNC_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gsl/gsl_spline.h>

#include "pspio_error.h"
#include "pspio_mesh.h"


/**
* Mesh function structure
*/
typedef struct{
  pspio_mesh_t *mesh;    /**< Pointer to mesh */
  double *f;             /**< values of the function on the mesh */
  gsl_spline *spl;       /**< gsl spline structure */
  gsl_interp_accel *acc; /**< accelerator for interpolation lookups */
} pspio_meshfunc_t;


/**
 * Allocates memory and preset function structure
 * 
 * @param[in,out] func: function structure
 * @param[in] mesh: mesh structure
 * @return error code
 */
int pspio_meshfunc_alloc(pspio_meshfunc_t *func, pspio_mesh_t *mesh);


/**
 * Sets the function data.
 * @param[in,out] func: function structure to set.
 * @param[in] f: values of the function on the mesh.
 * @return error code
 */
int pspio_meshfunc_set(pspio_meshfunc_t *func, double *f);


/**
 * Frees all memory associated with function structure
 * 
 * @param[in,out] func: function structure
 * @return error code
 */
int pspio_meshfunc_free(pspio_meshfunc_t *func);

#endif
