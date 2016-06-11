/* Copyright (C) 2011-2016 Micael Oliveira <micaeljtoliveira@gmail.com>
 *                         Yann Pouillon <notifications@materialsevolution.es>
 *
 * This file is part of Libpspio.
 *
 * Libpspio is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, version 3 of the License, or (at your option) any later
 * version.
 *
 * Libpspio is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Libpspio.  If not, see <http://www.gnu.org/licenses/> or write to
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301  USA.
 */

/**
 * @file pspio_meshfunc.h
 * @brief header file for the handling of functions defined on a mesh
 */

#ifndef PSPIO_MESHFUNC_H
#define PSPIO_MESHFUNC_H

#include "pspio_error.h"
#include "pspio_mesh.h"
#include "pspio_interp.h"


/**********************************************************************
 * Data structures                                                    *
 **********************************************************************/

/**
* Mesh function structure
*/
typedef struct{
  pspio_mesh_t *mesh;    /**< Pointer to mesh */
  int interp_method;

  /* Function */
  double *f;                  /**< function values on the mesh */
  pspio_interp_t *f_interp;  /**< function interpolation object */

  /* Function first derivative */
  double *fp;                 /**< first derivative values on the mesh */
  pspio_interp_t *fp_interp; /**< first derivative interpolation object */

  /* Function second derivative */
  double *fpp;                 /**< second derivative on the mesh */
  pspio_interp_t *fpp_interp; /**< second derivative interpolation object */

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
int pspio_meshfunc_alloc(pspio_meshfunc_t **func, int np);

/**
 * Initializes the function data.
 * @param[in,out] func: function structure to be initialized.
 * @param[in] mesh: mesh structure.
 * @param[in] f: values of the function on the mesh.
 * @param[in] fp: values of the functions first derivative on the mesh (optional)
 * @param[in] fpp: values of the functions second derivative on the mesh (optional)
 * @return error code
 * @note The func pointer has to be allocated first with the 
 *       pspio_meshfunc_alloc method.
 */
int pspio_meshfunc_init(pspio_meshfunc_t *func, const pspio_mesh_t *mesh, 
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
void pspio_meshfunc_free(pspio_meshfunc_t *func);


/**********************************************************************
 * Getters                                                            *
 **********************************************************************/

/**
 * Returns a pointer to the function.
 * 
 * @param[in] func: function structure
 * @return pointer to the function
 */
const double *pspio_meshfunc_get_function(const pspio_meshfunc_t *func);

/**
 * Returns a pointer to the first derivative of the function.
 * 
 * @param[in] func: function structure
 * @return pointer to the first derivative
 */
const double *pspio_meshfunc_get_deriv1(const pspio_meshfunc_t *func);

/**
 * Returns a pointer to the second derivative of the function.
 * 
 * @param[in] func: function structure
 * @return pointer to the second derivative
 */
const double *pspio_meshfunc_get_deriv2(const pspio_meshfunc_t *func);

/**
 * Returns the interpolation method.
 * 
 * @param[in] func: function structure
 * @return interpolation method
 */
int pspio_meshfunc_get_interp_method(const pspio_meshfunc_t *func);

/**
 * Returns a pointer to the mesh.
 * 
 * @param[in] func: function structure
 * @return pointer to the mesh
 */
const pspio_mesh_t *pspio_meshfunc_get_mesh(const pspio_meshfunc_t *func);


/**********************************************************************
 * Utility routines                                                   *
 **********************************************************************/

/**
 * Compares two mesh functions.
 * @param[in] meshfunc1: first mesh function to compare
 * @param[in] meshfunc2: second mesh function to compare
 * @return PSPIO_EQUAL when equal, PSPIO_DIFF when different, PSPIO_ERROR if a
 * problem occured.
 */
int pspio_meshfunc_cmp(const pspio_meshfunc_t *meshfunc1, const
                       pspio_meshfunc_t *meshfunc2);

/**
 * Returns the value of the function at an arbitrary point.
 * 
 * @param[in] func: function structure
 * @param[in] r: point were we want to evaluate the function
 * @return value of the function
 */
double pspio_meshfunc_eval(const pspio_meshfunc_t *func, double r);

/**
 * Returns the value of the derivative of the function at an arbitrary
 * point.
 * 
 * @param[in] func: function structure
 * @param[in] r: point were we want to evaluate the function
 * @return value of the derivative
 */
double pspio_meshfunc_eval_deriv(const pspio_meshfunc_t *func, double r);

/**
 * Returns the value of the second derivative of the function at an
 * arbitrary point.
 * 
 * @param[in] func: function structure
 * @param[in] r: point were we want to evaluate the function
 * @return value of the second derivative
 */
double pspio_meshfunc_eval_deriv2(const pspio_meshfunc_t *func, double r);


#endif
