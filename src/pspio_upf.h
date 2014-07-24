/*
 Copyright (C) 2011-2012 J. Alberdi, M. Oliveira, Y. Pouillon, and M. Verstraete
 
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
 * @file pspio_upf.h
 * @brief header file for the UPF routines accessible to the other parts of the library
 */

#if !defined PSPIO_UPF_H
#define PSPIO_UPF_H

#include <stdio.h>
#include <assert.h>

#include "pspio_pspdata.h"


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

/**
 * Read the data contained in a UPF file and store it in the psp_data structure
 * @param[in] fp a stream of the input file
 * @param[in,out] psp_data the data structure
 * @return error code
 */
int pspio_upf_read(FILE * fp, pspio_pspdata_t **psp_data);


/**
 * Write the data contained in the psp_data structure to a file using the UPF format
 * @param[in] fp a stream of the input file
 * @param[in] psp_data the data structure
 * @return error code
 */
int pspio_upf_write(FILE * fp, const pspio_pspdata_t *psp_data);


#endif
