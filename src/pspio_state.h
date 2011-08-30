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

#include "pspio.h"

/**********************************************************************
 * Data structures                                                    *
 **********************************************************************/

/**
 * General information about the state
 */
typedef struct{
  pspio_qn_t *qn; /**< quantum numbers n l j for this wavefunction */
  double *occ; /**< occupation of the electronic state */
  double *eigenval; /**< eigenvalue of electronic state*/
  char *label; /**< string describing the electronic state - eg 2s or 4d1.5  */
  double *rc; /**< cutoff radii used for pseudopotential generation */
  /// The wavefunctions 
  int np; /**< number of points */
  double *wf; /**< Wavefunction */
  double *wfp; /**<Derivative of the wavefunction */
} pspio_state_t;


/**********************************************************************
 * Routines                                                           *
 **********************************************************************/

/**
 * Sets the eigenvalues of a state.
 * @param[inout] state: state structure to set
 * @param[in] eigenval: pointer to the eigenvalues
 * @return error code
 */
int pspio_state_eigenval_set(pspio_state_t state, const double *eigenval);


/**
 * Sets the labels of a state.
 * @param[inout] state: state structure to set
 * @param[in] label: pointer to the labels
 * @return error code
 */
int pspio_state_label_set(pspio_state_t state, const char *label);


/**
 * Sets the number of points of a state.
 * @param[inout] state: state structure to set
 * @param[in] np: number of points
 * @return error code
 */
int pspio_state_np_set(pspio_state_t state, const int np);


/**
 * Sets the occupation numbers of a state.
 * @param[inout] state: state structure to set
 * @param[in] occ: pointer to the occupation numbers
 * @return error code
 */
int pspio_state_occ_set(pspio_state_t state, const double *occ);


/**
 * Sets the cutoff radii of a state.
 * @param[inout] state: state structure to set
 * @param[in] rc: pointer to the cutoff radii
 * @return error code
 */
int pspio_state_rc_set(pspio_state_t state, const double *rc);


/**
 * Sets the wavefunction of a state.
 * @param[inout] state: state structure to set
 * @param[in] wf: pointer to the wavefunction
 * @return error code
 */
int pspio_state_wf_set(pspio_state_t state, const double *wf);


/**
 * Sets the wavefunction derivative of a state.
 * @param[inout] state: state structure to set
 * @param[in] wfp: pointer to the wavefunction derivative
 * @return error code
 */
int pspio_state_wfp_set(pspio_state_t state, const double *wfp);

#endif
