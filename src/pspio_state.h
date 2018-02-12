/* Copyright (C) 2011-2016 Micael Oliveira <micael.oliveira@mpsd.mpg.de>
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
  double rc;            /**< cutoff radii used for pseudostate generation */
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
int pspio_state_alloc(pspio_state_t **state, int np);

/**
 * Initializes all the parameters of a state.
 * @param[in,out] state: state structure pointer to be initialized
 * @param[in] eigenval: eigenvalue
 * @param[in] qn: pointer to quatum numbers
 * @param[in] occ: occupation number
 * @param[in] rc: cutoff radius
 * @param[in] mesh: the radial mesh
 * @param[in] wf: pointer to the wavefunction values
 * @param[in] label: string describing the state (optional)
 * @return error code
 * @note The state pointer is supposed to have been already allocated
 *       with pspio_state_alloc.
 * @note If label is set to NULL, then the quantum numbers are used to construct the state label.
 */
int pspio_state_init(pspio_state_t *state, double eigenval, const pspio_qn_t *qn, double occ,
		     double rc, const pspio_mesh_t *mesh, const double *wf, const char *label);

/**
 * Duplicates a state structure.
 * @param[out] dst: destination state structure pointer
 * @param[in] src: source state structure pointer 
 * @return error code
 * @note The src pointer has to be allocated first with the pspio_state_alloc 
 *       method.
 * @note The dst pointer might or might not be allocated first. If it is not,
 *        then it is allocated here.
 */
int pspio_state_copy(pspio_state_t **dst, const pspio_state_t *src);

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
int pspio_states_lookup_table(int n_states, pspio_state_t **states, int ***table);

/**
 * Deallocates a state structure pointer.
 * @param[in,out] state: state structure pointer to destroy
 * @note This function can be safelly called even if some or all of the state 
 *       compoments have not been allocated.
 */
void pspio_state_free(pspio_state_t *state);


/**********************************************************************
 * Getters                                                            *
 **********************************************************************/

/**
 * Returns the state label
 * @param[in] state: state structure
 * @return pointer to label
 */
const char *pspio_state_get_label(const pspio_state_t *state);

/**
 * Returns the quantum numbers of the state
 * 
 * @param[in] state: state structure
 * @return pointer to the state quantum numbers
 * @note The state pointer has to be fully set.
 */
const pspio_qn_t *pspio_state_get_qn(const pspio_state_t *state);

/**
 * Returns the state occupancies
 * @param[in] state: state structure
 * @return the occupancies
 */
double pspio_state_get_occ(const pspio_state_t *state);

/**
 * Returns the state eigenvalue
 * @param[in] state: state structure
 * @return the eigenvalue
 */
double pspio_state_get_ev(const pspio_state_t *state);

/**
 * Returns the state cut-off radius
 * @param[in] state: state structure
 * @return the cut-off radius
 */
double pspio_state_get_rc(const pspio_state_t *state);

/**
 * Returns the state wavefuction
 * @param[in] state: state structure
 * @return the wavefunction
 */
const pspio_meshfunc_t *pspio_state_get_wf(const pspio_state_t *state);


/**********************************************************************
 * Utility routines                                                   *
 **********************************************************************/

/**
 * Compares two states.
 * @param[in] state1: first state to compare
 * @param[in] state2: second state to compare
 * @return PSPIO_EQUAL when equal, PSPIO_DIFF when different, PSPIO_ERROR if a
 * problem occured.
 */
int pspio_state_cmp(const pspio_state_t *state1, const pspio_state_t *state2);

/**
 * Returns the value of the states wavefunction at an arbitrary point
 *
 * @param[in] state: state structure
 * @param[in] r: point were we want to evaluate the wavefunction
 * @return value of the wavefunction at r
 */
double pspio_state_wf_eval(const pspio_state_t *state, double r);

/**
 * Returns the value of derivative of the states wavefunction at an arbitrary point
 *
 * @param[in] state: state structure
 * @param[in] r: point were we want to evaluate the derivative of the wavefunction
 * @return value of the wavefunction derivative at r
 */
double pspio_state_wf_eval_deriv(const pspio_state_t *state, double r);

/**
 * Returns the value of second derivative of the states wavefunction at an arbitrary point
 *
 * @param[in] state: state structure
 * @param[in] r: point were we want to evaluate the second derivative of the wavefunction
 * @return value of the wavefunction second derivative at r
 */
double pspio_state_wf_eval_deriv2(const pspio_state_t *state, double r);

#endif
