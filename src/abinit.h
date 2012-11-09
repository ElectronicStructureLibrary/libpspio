/*
 Copyright (C) 2012 M. Oliveira, Y. Pouillon
 
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
 * @file abinit.h
 * @brief header file for ABINIT related routines
 */

#if !defined ABINIT_H
#define ABINIT_H

#include <stdio.h>

#include "pspio_pspdata.h"


/**********************************************************************
 * Defines                                                            *
 **********************************************************************/


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

/**
 * Read the ABINIT header
 * @param[in] fp: a stream of the input file
 * @param[out] format: the file format
 * @param[out] np: the number of radial mesh points
 * @param[out] have_nlcc: whether there are non-linear core-corrections
 * @return error code
 */
int abinit_read_header(FILE *fp, int *format, int *np, int *have_nlcc);


/**********************************************************************
 * Format 6 routines                                                  *
 **********************************************************************/

/**
 * Read an ABINIT format 6 file
 * @param[in] fp: a stream of the input file
 * @param[inout] pspdata: a pseudopotential data structure
 * @return error code
 */
int abinit6_read(FILE *fp, pspio_pspdata_t **pspdata);


/**
 * Write an ABINIT format 6 file
 * @param[in] fp: a stream of the input file
 * @param[in] pspdata: a pseudopotential data structure
 * @return error code
 */
int abinit6_write(FILE *fp, const pspio_pspdata_t *pspdata);


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
int abinit_to_libxc(const char *xc_string, int *exchange, int *correlation);

/**
 * Converts libxc codes to Abinit codes
 * @param[in] exchange: libxc code for exchange
 * @param[in] correlation: libxc code for correlation
 * @param[out] pspxc: Abinit code
 * @return error code
 */
int libxc_to_abinit(const int exchange, const int correlation, int *pspxc);

#endif
