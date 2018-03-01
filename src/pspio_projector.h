/* Copyright (C) 2011-2017 Joseba Alberdi <alberdi@hotmail.es>
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

/**
 * @file pspio_projector.h
 * @brief header file for the handling of the KB projectors
 */

#ifndef PSPIO_PROJECTOR_H
#define PSPIO_PROJECTOR_H

#include "pspio_error.h"
#include "pspio_mesh.h"
#include "pspio_meshfunc.h"
#include "pspio_qn.h"


/**********************************************************************
 * Data structures                                                    *
 **********************************************************************/

/**
 * Projector structure
 */
typedef struct{
  pspio_qn_t *qn;         /**< quantum numbers for present projector */
  double energy;          /**< projector energy */
  pspio_meshfunc_t *proj; /**< projector on a mesh */
} pspio_projector_t;


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

/**
 * Allocates memory and preset projector structure
 * 
 * @param[in,out] projector: projector structure
 * @param[in] np: number of points in mesh
 * @return error code
 * @note np should be larger than 1.
 */
int pspio_projector_alloc(pspio_projector_t **projector, int np);

/**
 * Initializes the projector data.
 * @param[in,out] projector: projector structure to be initialized
 * @param[in] qn: pointer to quantum numbers
 * @param[in] energy: energy
 * @param[in] mesh: pointer to mesh structure
 * @param[in] pofr: values of the projector on the mesh
 * @return error code
 * @note The projector pointer has to be allocated first with the 
 *       pspio_projector_alloc method.
 */
int pspio_projector_init(pspio_projector_t *projector, const pspio_qn_t *qn, 
                         const pspio_mesh_t *mesh, const double *pofr);

/**
 * Duplicates a projector structure.
 * @param[out] dst: destination projector structure pointer
 * @param[in] src: source projector structure pointer 
 * @return error code
 * @note The src pointer has to be allocated first with the pspio_projector_alloc 
 *       method.
 * @note The dst pointer might or might not be allocated first. If it is not,
 *        then it is allocated here.
 */
int pspio_projector_copy(pspio_projector_t **dst, const pspio_projector_t *src);

/**
 * Frees all memory associated with a projector structure
 * 
 * @param[in,out] projector: projector structure
 * @note This function can be safely called even if some or all of the
 *       projector components have not been allocated.
 */
void pspio_projector_free(pspio_projector_t *projector);


/**********************************************************************
 * Setters                                                            *
 **********************************************************************/

/**
 * Specify the energy of the projector
 *
 * @param[in] projector: projector structure
 * @param[in] energy: value of the projector energy
 * @return error code
 */
int pspio_projector_set_energy(pspio_projector_t *projector, double energy);

/**********************************************************************
 * Getters                                                            *
 **********************************************************************/

/**
 * Returns the energy of the projector
 * 
 * @param[in] projector: projector structure
 * @return value of the projector energy
 * @note The projector pointer has to be fully set.
 */
double pspio_projector_get_energy(const pspio_projector_t *projector);

/**
 * Returns a pointer to the quantum numbers of the projector
 * 
 * @param[in] projector: projector structure
 * @return pointer to the projector quantum numbers
 * @note The projector pointer has to be fully set.
 */
const pspio_qn_t *pspio_projector_get_qn(const pspio_projector_t *projector);


/**********************************************************************
 * Utility routines                                                   *
 **********************************************************************/

/**
 * Compares two projectors.
 * @param[in] projector1: first projector to compare
 * @param[in] projector2: second projector to compare
 * @return PSPIO_EQUAL when equal, PSPIO_DIFF when different, PSPIO_ERROR if a
 * problem occured.
 */
int pspio_projector_cmp(const pspio_projector_t *projector1, const
                        pspio_projector_t *projector2);

/**
 * Returns the value of the projector at an arbitrary point
 * 
 * @param[in] projector: projector structure
 * @param[in] r: point were we want to evaluate the projector
 * @return value of the projector at r
 * @note The projector pointer has to be fully set.
 */
double pspio_projector_eval(const pspio_projector_t *projector, double r);

/**
 * Returns the value of the derivative of the projector at an arbitrary point
 * 
 * @param[in] projector: projector structure
 * @param[in] r: point were we want to evaluate the derivative of the projector
 * @return value of the derivative at r
 * @note The projector pointer has to be fully set.
 */
double pspio_projector_eval_deriv(const pspio_projector_t *projector, double r);

/**
 * Returns the value of the second derivative of the projector at an arbitrary point
 * 
 * @param[in] projector: projector structure
 * @param[in] r: point were we want to evaluate the second derivative of the projector
 * @return value of the second derivative at r
 * @note The projector pointer has to be fully set.
 */
double pspio_projector_eval_deriv2(const pspio_projector_t *projector, double r);

/**
 * Return a count of projectors per angular momentum
 *
 * @param[in] projectors: the projectors to count
 * @param[in] nproj: the total number of projectors
 */
int * pspio_projectors_per_l(pspio_projector_t ** const projectors, int nproj);

#endif
