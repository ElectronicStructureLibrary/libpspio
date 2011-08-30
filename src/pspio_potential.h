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
 * @file pspio_potential.h
 * @brief header file for the handling of the potentials
 */

#ifndef PSPIO_POTENTIAL_H
#define PSPIO_POTENTIAL_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "pspio_error.h"
#include "pspio_mesh.h"
#include "pspio_meshfunc.h"
#include "pspio_qn.h"


/**
 * Potential structure
 */
typedef struct{
  pspio_qn_t *qn;      /**< struct with quantum numbers n l j for the potential */
  pspio_meshfunc_t *v;  /**< pseudopotential, on a radial mesh */
} pspio_potential_t;


/**
 * Allocates memory and preset potential structure
 * 
 * @param[in,out] potential: potential structure
 * @param[in] m: pointer to mesh
 * @return error code
 */
int pspio_potential_alloc(pspio_potential_t *potential, pspio_mesh_t *m);


/**
 * Sets the potential datga.
 * @param[in,out] potential: potential structure to set
 * @param[in] qn: pointer to quantum numbers
 * @param[in] v: values of the potential on the mesh
 * @return error code
 */
int pspio_potential_set(pspio_potential_t *potential, pspio_qn_t *qn, double *v);


/**
 * Frees all memory associated with potential structure
 * 
 * @param[in,out] potential: potential structure
 */
void pspio_potential_free(pspio_potential_t *potential);

#endif
