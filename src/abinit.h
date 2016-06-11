/* Copyright (C) 2011-2016 Joseba Alberdi <alberdi@hotmail.es>
 *                         Micael Oliveira <micaeljtoliveira@gmail.com>
 *                         Yann Pouillon <notifications@materialsevolution.es>
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
 * @file abinit.h
 * @brief header file for ABINIT related routines
 */

#if !defined ABINIT_H
#define ABINIT_H

#include <stdio.h>
#include <assert.h>

#include "pspio_pspdata.h"


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

/**
 * Read the data contained in a Abinit-formatted file and store it in the
 * psp_data structure
 * @param[in] fp a stream of the input file
 * @param[in,out] pspdata the data structure
 * @param[in] format: the Abinit format number
 * @return error code
 */
int pspio_abinit_read(FILE *fp, pspio_pspdata_t *pspdata, int format);

/**
 * Write the data contained in the psp_data structure to a file using an Abinit format
 * @param[in] fp a stream of the input file
 * @param[in] pspdata the data structure
 * @param[in] format: the Abinit format number
 * @return error code
 */
int pspio_abinit_write(FILE *fp, const pspio_pspdata_t *pspdata, int format);


/**********************************************************************
 * ABINIT XC routines                                                 *
 **********************************************************************/

/**
* Converts Abinit pspxc code to libxc codes
*@param[in]  pspxc: abinit xc code
*@param[out] exchange: libxc code for exchange
*@param[out] correlation: libxc code for correlation
* @return error code
*/
int abinit_to_libxc(int pspxc, int *exchange, int *correlation);

/**
 * Converts libxc codes to Abinit codes
 * @param[in] exchange: libxc code for exchange
 * @param[in] correlation: libxc code for correlation
 * @param[out] pspxc: Abinit code
 * @return error code
 */
int libxc_to_abinit(int exchange, int correlation, int *pspxc);


/**********************************************************************
 * ABINIT utility routines                                            *
 **********************************************************************/

/**
 * Read the ABINIT header
 * @param[in] fp: a stream of the input file
 * @param[in] format: the file format
 * @param[out] pspdata: pseudopotential data structure
 * @return error code
 */
int abinit_read_header(FILE *fp, int format, pspio_pspdata_t *pspdata);

/**
 * Write the ABINIT header
 * @param[in] fp: a stream of the input file
 * @param[in] format: the file format
 * @param[in] pspdata: pseudopotential data structure
 * @return error code
 */
int abinit_write_header(FILE *fp, int format, const pspio_pspdata_t *pspdata);

#endif
