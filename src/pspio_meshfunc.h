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

/**
 * @file pspio_meshfunc.h
 * @brief header file for the handling of functions defined on a mesh
 */

#ifndef PSPIO_MESHFUNC_H
#define PSPIO_MESHFUNC_H

#include "pspio_error.h"
#include "pspio_mesh.h"
#include "interpolation.h"


/**********************************************************************
 * Data structures                                                    *
 **********************************************************************/

/**
* Mesh function structure
*/
typedef struct{
  pspio_mesh_t *mesh;    /**< Pointer to mesh */
  int interp_method;

  // Function
  double *f;                  /**< function values on the mesh */
  interpolation_t *f_interp;  /**< function interpolation object */

  // Function first derivative
  double *fp;                 /**< first derivative values on the mesh */
  interpolation_t *fp_interp; /**< first derivative interpolation object */

  // Function second derivative
  double *fpp;                 /**< second derivative on the mesh */
  interpolation_t *fpp_interp; /**< second derivative interpolation object */

} pspio_meshfunc_t;


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

/**
 * Allocates memory and preset function structure
 * 
 * @param[in,out] func: function structure
 * @param[in] interp_method: method used for interpolation
 * @param[in] np: number of points
 * @return error code
 * @note np should be larger than 1.
 */
int pspio_meshfunc_alloc(pspio_meshfunc_t **func, const int interp_method, const int np);


/**
 * Sets the function data.
 * @param[in,out] func: function structure to set.
 * @param[in] mesh: mesh structure.
 * @param[in] f: values of the function on the mesh.
 * @param[in] fp: values of the functions first derivative on the mesh (optional)
 * @param[in] fpp: values of the functions second derivative on the mesh (optional)
 * @return error code
 * @note The func pointer has to be allocated first with the 
 *       pspio_meshfunc_alloc method.
 */
int pspio_meshfunc_set(pspio_meshfunc_t **func, const pspio_mesh_t *mesh, 
		       const double *f, const double *fp, const double *fpp);


/**
 * Duplicates a mesh function structure
 * 
 * @param[out] dst: destination mesh function structure pointer
 * @param[in] src: source mesh function structure pointer
 * @return error code
 * @note The src pointer has to be allocated first with the pspio_meshfunc_alloc 
 *       method.
 * @note The dst pointer might or might not be allocated. If it is not, then it
 *       is allocate here.
 */
int pspio_meshfunc_copy(pspio_meshfunc_t **dst, const pspio_meshfunc_t *src);


/**
 * Frees all memory associated with function structure
 * 
 * @param[in,out] func: function structure
 * @note This function can be safelly called even if some or all of the func 
 *       compoments have not been allocated.
 */
void pspio_meshfunc_free(pspio_meshfunc_t **func);


/**********************************************************************
 * Atomic routines                                                    *
 **********************************************************************/

/**
 * Returns the value of the function at arbitrary points
 * 
 * @param[in] func: function structure
 * @param[in] np: number of points
 * @param[in] *r:  array of points were we want to evaluate the function
 * @param[out] *f: values of the function
 */
void pspio_meshfunc_eval(const pspio_meshfunc_t *func, const int np, const double *r,
      double *f);


/**
 * Returns the value of the derivative of a function at arbitrary points
 * 
 * @param[in] func: function structure
 * @param[in] np: number of points
 * @param[in] *r: array of points were we want to evaluate the function
 * @param[out] *fp: values of the derivative
 */
void pspio_meshfunc_eval_deriv(const pspio_meshfunc_t *func, const int np, 
			       const double *r, double *fp);


/**
 * Returns the value of the second derivative of a function at arbitrary points
 * 
 * @param[in] func: function structure
 * @param[in] np: number of points
 * @param[in] *r: array of points were we want to evaluate the function
 * @param[out] *fpp: values of the second derivative
 */
void pspio_meshfunc_eval_deriv2(const pspio_meshfunc_t *func, const int np, 
				const double *r, double *fpp);

#endif
