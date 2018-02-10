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
 * @file pspio_qn.h
 * @brief header file for the handling of quantum numbers 
 */

#if !defined PSPIO_QN_H
#define PSPIO_QN_H

#include "pspio_common.h"
#include "pspio_error.h"


/**********************************************************************
 * Data structures                                                    *
 **********************************************************************/

/** 
 * The quantum numbers
 */
typedef struct{
  int n;    /**< main quantum number */
  int l;    /**< angular momentum quantum number */
  double j; /**< total angular momentum quantum number */
} pspio_qn_t;


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

/**
 * Allocates and presets a quantum number structure.
 * @param[out] qn: quantum number structure pointer to allocate
 * @return error code
 */
int pspio_qn_alloc(pspio_qn_t **qn);

/**
 * Sets the quantum numbers of a state.
 * @param[in,out] qn: quantum number structure pointer to set
 * @param[in] n: main quantum number
 * @param[in] l: angular momentum
 * @param[in] j: total angular momentum
 * @return error code
 * @note The qn pointer has to be allocated first with the pspio_qn_alloc
 *       method.
 */
int pspio_qn_init(pspio_qn_t *qn, int n, int l, double j);

/**
 * Duplicates a quantum number structure.
 * @param[out] dst: destination quantum number structure pointer
 * @param[in] src: source quantum number structure pointer 
 * @return error code
 * @note The src pointer has to be allocated first with the pspio_qn_alloc 
 *       method.
 * @note The dst pointer might or might not be allocated first. If it is not,
 *        then it is allocated here.
 */
int pspio_qn_copy(pspio_qn_t **dst, const pspio_qn_t *src);

/**
 * Frees the memory occupied by a quantum number structure.
 * @param[in,out] qn: quantum number structure pointer to destroy
 * @note This function can be safely called even if some or all of the qn
 *       components have not been allocated.
 */
void pspio_qn_free(pspio_qn_t *qn);


/**********************************************************************
 * Getters                                                            *
 **********************************************************************/

/**
 * Returns the main quantum number
 * @param[in] qn: set of quantum numbers
 * @return main quantum number
 */
int pspio_qn_get_n(const pspio_qn_t *qn);

/**
 * Returns the angular momentum quantum number
 * @param[in] qn: set of quantum numbers
 * @return angular momentum quantum number
 */
int pspio_qn_get_l(const pspio_qn_t *qn);

/**
 * Returns the total angular momentum quantum number
 * @param[in] qn: set of quantum numbers
 * @return total angular momentum quantum number
 */
double pspio_qn_get_j(const pspio_qn_t *qn);


/**********************************************************************
 * Utility routines                                                   *
 **********************************************************************/

/**
 * Compares two sets of quantum numbers.
 * @param[in] qn1: first set to compare
 * @param[in] qn2: second set to compare
 * @return PSPIO_EQUAL for strict equality, PSPIO_MTEQUAL when the
 *         angular momenta are equal, PSPIO_DIFF when different,
 *         PSPIO_ERROR if a problem occured
 */
int pspio_qn_cmp(const pspio_qn_t *qn1, const pspio_qn_t *qn2);

/**
 * Returns a label that identifies the set of quantum numbers
 * @param[in] qn: set of quantum numbers
 * @param[out] label:
 * @return error code
 */
int pspio_qn_label(const pspio_qn_t *qn, char *s);

#endif
