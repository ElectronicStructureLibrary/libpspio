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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "pspio_error.h"
#include "pspio_mesh.h"
#include "pspio_meshfunc.h"
#include "pspio_qn.h"


/**
 * Projector structure
 */
typedef struct{
  pspio_qn_t *qn;      /**< quantum numbers for present projector */
  double e;            /**< energy */
  pspio_meshfunc_t *p; /**< projector on a mesh */
} pspio_projector_t;


/**
 * Allocates memory and preset projector structure
 * 
 * @param[in,out] projector: projector structure
 * @param[in] m: pointer to mesh
 * @return error code
 */
int pspio_projector_alloc(pspio_projector_t *projector, pspio_mesh_t *mesh);


/**
 * Sets the projector data.
 * @param[in,out] projector: projector structure to set
 * @param[in] qn: pointer to quantum numbers
 * @param[in] e: energy
 * @param[in] p: values of the projector on the mesh
 * @return error code
 */
int pspio_projector_set(pspio_projector_t *projector, pspio_qn_t *qn, double e, double *p);


/**
 * Frees all memory associated with a projector structure
 * 
 * @param[in,out] projector: projector structure
 * @return error code
 */
int pspio_projector_free(pspio_projector_t *projector);

#endif
