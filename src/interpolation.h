/*
 Copyright (C) 2014 M. Oliveira

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
 * @file interpolation.h
 * @brief header file for the handling of interpolation methods
 */

#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include "pspio_common.h"
#include "pspio_error.h"
#include "pspio_mesh.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_GSL
#include <gsl/gsl_spline.h>
#endif


/**********************************************************************
 * Data structures                                                    *
 **********************************************************************/

/**
* Interpolation data structure
*/
typedef struct{
  int method; /**< Interpolation method */

#ifdef HAVE_GSL
  // Objects to the used with GSL interpolation
  gsl_spline *gsl_spl;       /**< gsl spline structure */
  gsl_interp_accel *gsl_acc; /**< gsl accelerator for interpolation lookups */
#endif

  // Objects to be used with ...


} interpolation_t;


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

/**
 * Allocates memory and preset
 * 
 * @param[in,out] func: function structure
 * @param[in] method: interpolation method
 * @param[in] np: number of points
 * @return error code
 * @note np should be larger than 1.
 */
int interpolation_alloc(interpolation_t **interp, const int method, const int np);


/**
 * Sets the interpolation object.
 * @param[in,out] interp: interpolation structure to set.
 * @param[in] mesh: mesh structure.
 * @param[in] f: values of the function on the mesh.
 * @return error code
 */
int interpolation_set(interpolation_t **interp, const pspio_mesh_t *mesh, const double *f);


/**
 * Frees all memory associated with the interpolation structure
 * 
 * @param[in,out] interp: interpolation structure
 * @note This function can be safelly called even if some or all of the interp
 *       compoments have not been allocated.
 */
void interpolation_free(interpolation_t **interp);


/**********************************************************************
 * Atomic routines                                                    *
 **********************************************************************/


/**
 * Evaluates the interpolated function at a given point
 * 
 * @param[in] interp: interpolation structure
 * @param[in] r:  point were we want to evaluate the function
 * @param[out] *f: value of the function
 */
void interpolation_eval(const interpolation_t *interp, const double r, double *f);


/**
 * Evaluates the derivative of the interpolated function at arbitrary points
 * 
 * @param[in] interp: interpolation structure
 * @param[in] r: point were we want to evaluate the function
 * @param[out] *fp: value of the derivative
 */
void interpolation_eval_deriv(const interpolation_t *interp, const double r, double *fp);


/**
 * Returns the value of the second derivative of a function at arbitrary points
 * 
 * @param[in] interp: interpolation structure
 * @param[in] r: point were we want to evaluate the function
 * @param[out] *fpp: value of the second derivative
 */
void interpolation_eval_deriv2(const interpolation_t *interp, const double r, double *fpp);

#endif
