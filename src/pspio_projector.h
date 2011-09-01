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
int pspio_projector_alloc(pspio_projector_t *projector, const int np);


/**
 * Sets the projector data.
 * @param[in,out] projector: projector structure to set
 * @param[in] qn: pointer to quantum numbers
 * @param[in] e: energy
 * @param[in] mesh: pointer to mesh structure
 * @param[in] p: values of the projector on the mesh
 * @return error code
 * @note The projector pointer has to be allocated first with the 
 *       pspio_projector_alloc method.
 */
int pspio_projector_set(pspio_projector_t *projector, const pspio_qn_t *qn, 
			const double e, const pspio_mesh_t *mesh, const double *p);


/**
 * Frees all memory associated with a projector structure
 * 
 * @param[in,out] projector: projector structure
 * @return error code
 * @note This function can be safelly called even if some or all of the
 *       projector components have not been allocated.
 */
int pspio_projector_free(pspio_projector_t *projector);


/**********************************************************************
 * Atomic routines                                                    *
 **********************************************************************/

/**
 * Return the quantum numbers associated with a projector.
 * @param[in] projector: pointer to projector structure
 * @param[out] qn: pointer to quantum numbers
 * @return error code
 * @note The projector pointer has to be fully set.
 */
int pspio_projector_get_qn(pspio_projector_t *projector, pspio_qn_t *qn);


#endif
