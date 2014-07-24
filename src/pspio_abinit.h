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
 
 */

/**
 * @file pspio_abinit.h
 * @brief header file for the Abinit routines accessible to the other parts of the library
 */

#if !defined PSPIO_ABINIT_H
#define PSPIO_ABINIT_H

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
int pspio_abinit_read(FILE *fp, pspio_pspdata_t **pspdata, const int format);


/**
 * Write the data contained in the psp_data structure to a file using an Abinit format
 * @param[in] fp a stream of the input file
 * @param[in] pspdata the data structure
 * @param[in] format: the Abinit format number
 * @return error code
 */
int pspio_abinit_write(FILE *fp, const pspio_pspdata_t *pspdata, const int format);


#endif
