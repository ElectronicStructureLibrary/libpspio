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


/**********************************************************************
 * Data structures                                                    *
 **********************************************************************/
/**
* Mesh function structure
*/
typedef struct{
  pspio_mesh_t *mesh;    /**< Pointer to mesh */
  double *f;             /**< values of the function on the mesh */
  gsl_spline *spl;       /**< gsl spline structure */
  gsl_interp_accel *acc; /**< accelerator for interpolation lookups */
} pspio_meshfunc_t;


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

/**
 * Allocates memory and preset function structure
 * 
 * @param[in,out] func: function structure
 * @param[in] np: number of points
 * @return error code
 * @note np should be larger than 1.
 */
int pspio_meshfunc_alloc(pspio_meshfunc_t *func, const int np);


/**
 * Sets the function data.
 * @param[in,out] func: function structure to set.
 * @param[in] mesh: mesh structure.
 * @param[in] f: values of the function on the mesh.
 * @return error code
 * @note The func pointer has to be allocated first with the 
 *       pspio_meshfunc_alloc method.
 */
int pspio_meshfunc_set(pspio_meshfunc_t *func, pspio_mesh_t *mesh, double *f);


/**
 * Frees all memory associated with function structure
 * 
 * @param[in,out] func: function structure
 * @return error code
 * @note This function can be safelly called even if some or all of the func 
 *       compoments have not been allocated.
 */
int pspio_meshfunc_free(pspio_meshfunc_t *func);


/**
 * Returns the value of the function at an arbitrary point
 * 
 * @param[in] func: function structure
 * @param[in] r: position were we want to evaluate the function
 * @param[out] f: value of the function at r
 * @return error code
 */
int pspio_meshfunc_eval(pspio_meshfunc_t *func, const double r, double f);


/**
 * Returns the value of the derivative of a function at an arbitrary point
 * 
 * @param[in] func: function structure
 * @param[in] r: position were we want to evaluate the function
 * @param[out] fp: value of the derivative at r
 * @return error code
 */
int pspio_meshfunc_eval_deriv(pspio_meshfunc_t *func, const double r, double fp);


/**
 * Returns the value of the second derivative of a function at an arbitrary point
 * 
 * @param[in] func: function structure
 * @param[in] r: position were we want to evaluate the function
 * @param[out] fpp: value of the second derivative at r
 * @return error code
 */
int pspio_meshfunc_eval_deriv2(pspio_meshfunc_t *func, const double r, double fpp);


#endif
