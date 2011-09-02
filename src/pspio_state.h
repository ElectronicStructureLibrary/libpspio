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
 * @file pspio_state.h
 * @brief header file for the handling of states 
 */

#if !defined PSPIO_STATES_H
#define PSPIO_STATES_H

#include "pspio_qn.h"
#include "pspio_mesh.h"
#include "pspio_meshfunc.h"


/**********************************************************************
 * Data structures                                                    *
 **********************************************************************/

/**
 * General information about the state
 */
typedef struct{
  pspio_qn_t *qn;       /**< quantum numbers n l j for this wavefunction */
  double occ;           /**< occupation of the electronic state */
  double eigenval;      /**< eigenvalue of electronic state*/
  char *label;          /**< string describing the electronic state - eg 2s or 4d1.5 */
  double rc;            /**< cutoff radii used for pseudopotential generation */
  pspio_meshfunc_t *wf; /**< Wavefunction */
  
} pspio_state_t;


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

/**
 * Allocates and presets a state structure pointer.
 * @param[out] state: state structure pointer to allocate
 * @param[in] np: number of points in the mesh
 * @return error code
  * @note np should be larger than 1.
 */
int pspio_state_alloc(pspio_state_t *state, const int np);


/**
 * Sets all parameters of a state.
 * @param[in,out] state: state structure pointer to set
 * @param[in] eigenval: eigenvalue
 * @param[in] label: string describing the state
 * @param[in] occ: occupation number
 * @param[in] rc: cutoff radius
 * @param[in] mesh: the radial mesh
 * @param[in] wf: pointer to the wavefunction values
 * @return error code
 * @note The state pointer is supposed to have been already allocated
 *       with pspio_state_alloc.
 */
int pspio_state_set(pspio_state_t *state, const double eigenval,
		    const char *label, const double occ, const double rc, 
		    pspio_mesh_t *mesh, const double *wf);


/**
 * Duplicates a state structure pointer.
 * @param[out] dst: destination state structure pointer
 * @param[in] src: source state structure pointer
 * @return error code
 * @note The src pointer has to be allocated first with the pspio_state_alloc 
 *       method.
 * @note The dst pointer might or might not be allocated. If it is not, then it
 *       is allocate here.
 */
int pspio_state_copy(pspio_state_t *dst, pspio_state_t *src);


/**
 * Creates a lookup table where are stored the indexes of the states corresponding to 
 * some quantum numbers.
 * @param[in] n_states: the number of states
 * @param[in] states: the list of states
 * @param[out] table: lookup table with indexes of the states.
 * @return error code
 * @note The index of a state in the list of states is given by 
 * table[state->qn->n][state->qn->l + (int)states->qn->j]
 * @note The table is allocated inside this function.
 */
int pspio_states_lookup_table(const int n_states, const pspio_state_t **states,
			      int **table);


/**
 * Deallocates a state structure pointer.
 * @param[in,out] state: state structure pointer to destroy
 * @return error code
 * @note This function can be safelly called even if some or all of the state 
 *       compoments have not been allocated.
 */
int pspio_state_free(pspio_state_t *state);


#endif
