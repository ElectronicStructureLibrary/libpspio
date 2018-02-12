/* Copyright (C) 2011-2017 Joseba Alberdi <alberdi@hotmail.es>
 *                         Matthieu Verstraete <matthieu.jean.verstraete@gmail.com>
 *                         Micael Oliveira <micael.oliveira@mpsd.mpg.de>
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
