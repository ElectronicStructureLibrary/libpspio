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
int pspio_state_alloc(pspio_state_t **state, const int np);


/**
 * Sets all parameters of a state.
 * @param[in,out] state: state structure pointer to set
 * @param[in] eigenval: eigenvalue
 * @param[in] label: string describing the state
 * @param[in] qn: pointer to quatum numbers
 * @param[in] occ: occupation number
 * @param[in] rc: cutoff radius
 * @param[in] mesh: the radial mesh
 * @param[in] wf: pointer to the wavefunction values
 * @return error code
 * @note The state pointer is supposed to have been already allocated
 *       with pspio_state_alloc.
 */
int pspio_state_set(pspio_state_t **state, const double eigenval,
		    const char *label, const pspio_qn_t *qn, const double occ, 
		    const double rc, const pspio_mesh_t *mesh, const double *wf);


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
int pspio_states_lookup_table(const int n_states, pspio_state_t **states,
			      int ***table);


/**
 * Deallocates a state structure pointer.
 * @param[in,out] state: state structure pointer to destroy
 * @return error code
 * @note This function can be safelly called even if some or all of the state 
 *       compoments have not been allocated.
 */
int pspio_state_free(pspio_state_t **state);


/**********************************************************************
 * Atomic routines                                                    *
 **********************************************************************/

/**
 * Returns the value of the states wavefunction at an arbitrary point
 * @param[in] state: state structure
 * @param[in] r: position were we want to evaluate the wavefunction
 * @param[out] *wf: value of the wavefunction at r
 * @return error code
 */
int pspio_state_wf_eval(const pspio_state_t *state, const double r, double *wf);

/**
 * Returns the state label
 * @param[in] state: state structure
 * @param[out] label
 * @return error code
 */
int pspio_state_get_label(const pspio_state_t *state, char *label);

/**
 * Returns the angular momentum quantum number
 * @param[in] state: state structure
 * @param[out] l: the angular momentum quantum number
 * @return error code
 */
int pspio_state_get_l(const pspio_state_t *state, int *l);

/**
 * Returns the state occupancies
 * @param[in] state: state structure
 * @param[out] occ: the occupancies
 * @return error code
 */
int pspio_state_get_occ(const pspio_state_t *state, double *occ);


#endif
