/* Copyright (C) 2017      Yann Pouillon <notifications@materialsevolution.es>
 *
 * This file is part of Libpspio.
 *
 * Libpspio is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, version 3 of the License, or (at your option) any later
 * version.
 *
 * Libpspio is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Libpspio.  If not, see <http://www.gnu.org/licenses/> or write to
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301  USA.
 */

/**
 * @file oncv.h
 * @brief header file for the Hamann ONCV routines accessible to the other parts of the library
 */

#if !defined PSPIO_ONCV_H
#define PSPIO_ONCV_H

#include <stdio.h>
#include <assert.h>

#include "pspio_pspdata.h"


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

/**
 * Read the data contained in a Hamann ONCV file and store it in
 * the psp_data structure
 * @param[in] fp a stream of the input file
 * @param[in,out] pspdata the data structure
 * @return error code
 */
int pspio_oncv_read(FILE * fp, pspio_pspdata_t *pspdata);

/**
 * Write the data contained in the psp_data structure to a file using the
 * Hamann ONCV format
 * @param[in] fp a stream of the input file
 * @param[in] pspdata the data structure
 * @return error code
 */
int pspio_oncv_write(FILE * fp, const pspio_pspdata_t *pspdata);

#endif
