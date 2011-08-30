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

#include <stdlib.h>
#include "pspio_qn.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


/**********************************************************************
 * Data structures                                                    *
 **********************************************************************/

/**
 * General information about the state
 */
typedef struct{
  pspio_qn_t *qn; /**< quantum numbers n l j for this wavefunction */
  double occ; /**< occupation of the electronic state */
  double eigenval; /**< eigenvalue of electronic state*/
  char *label; /**< string describing the electronic state - eg 2s or 4d1.5  */
  double rc; /**< cutoff radii used for pseudopotential generation */
  /// The wavefunctions 
  int np; /**< number of points */
  double *wf; /**< Wavefunction */
  double *wfp; /**<Derivative of the wavefunction */
} pspio_state_t;


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

/**
 * Allocates and presets a state structure pointer.
 * @param[out] state: state structure pointer to allocate
 * @return error code
 */
int pspio_state_alloc(pspio_state_t *state);


/**
 * Duplicates a state structure pointer.
 * @param[out] dst: destination state structure pointer
 * @param[in] src: source state structure pointer
 * @return error code
 */
int pspio_state_copy(pspio_state_t *dst,pspio_state_t *src);


/**
 * Deallocates a state structure pointer.
 * @param[in,out] state: state structure pointer to destroy
 * @return error code
 * @note The state pointer is supposed to have been already allocated
 *       with pspio_state_alloc.
 */
int pspio_state_free(pspio_state_t *state);


/**
 * Sets all parameters of a state.
 * @param[in,out] state: state structure pointer to set
 * @param[in] np: number of points
 * @param[in] eigenval: eigenvalue
 * @param[in] label: string describing the state
 * @param[in] occ: occupation number
 * @param[in] rc: cutoff radius
 * @param[in] wf: pointer to the wavefunction
 * @param[in] wfp: pointer to the wavefunction derivative
 * @return error code
 * @note The state pointer is supposed to have been already allocated
 *       with pspio_state_alloc.
 */
int pspio_state_set(pspio_state_t *state, const int np, const double eigenval,
  const char *label, const double occ, const double rc, const double *wf,
  const double *wfp);

#endif
