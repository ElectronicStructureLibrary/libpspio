/* Copyright (C) 2014-2016 Micael Oliveira <micael.oliveira@mpsd.mpg.de>
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

/**
 * @file interpolation.h
 * @brief header file for the handling of interpolation methods
 */

#ifndef PSPIO_INTERP__H
#define PSPIO_INTERP_H

#include "pspio_common.h"
#include "pspio_error.h"
#include "pspio_jb_spline.h"
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
typedef struct pspio_interp_t pspio_interp_t;


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
int pspio_interp_alloc(pspio_interp_t **interp, int method, int np);

/**
 * Duplicates an interpolation structure
 * 
 * @param[out] dst: destination interpolation structure pointer
 * @param[in] src: source interpolation structure pointer
 * @return error code
 * @note The src pointer has to be allocated first with the
 *       pspio_interp_alloc  method.
 * @note The dst pointer might or might not be allocated. If it is not, then it
 *       is allocate here.
 */
int pspio_interp_copy(pspio_interp_t **dst, const pspio_interp_t *src);

/**
 * Initializes the interpolation object.
 * @param[in,out] interp: interpolation structure to be initialized.
 * @param[in] mesh: mesh structure.
 * @param[in] f: values of the function on the mesh.
 * @return error code
 */
int pspio_interp_init(pspio_interp_t *interp, const pspio_mesh_t *mesh, const double *f);

/**
 * Frees all memory associated with the interpolation structure
 * 
 * @param[in,out] interp: interpolation structure
 * @note This function can be safelly called even if some or all of the interp
 *       compoments have not been allocated.
 */
void pspio_interp_free(pspio_interp_t *interp);


/**********************************************************************
 * Utility routines                                                   *
 **********************************************************************/

/**
 * Evaluates the interpolated function at a given point
 * 
 * @param[in] interp: interpolation structure
 * @param[in] r:  point were we want to evaluate the function
 * @return value of the function
 */
double pspio_interp_eval(const pspio_interp_t *interp, double r);

/**
 * Evaluates the derivative of the interpolated function at arbitrary points
 * 
 * @param[in] interp: interpolation structure
 * @param[in] r: point were we want to evaluate the function
 * @return value of the derivative
 */
double pspio_interp_eval_deriv(const pspio_interp_t *interp, double r);

/**
 * Returns the value of the second derivative of a function at arbitrary points
 * 
 * @param[in] interp: interpolation structure
 * @param[in] r: point were we want to evaluate the function
 * @return value of the second derivative
 */
double pspio_interp_eval_deriv2(const pspio_interp_t *interp, double r);

#endif
