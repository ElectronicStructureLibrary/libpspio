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
 
 $Id$
 */

/**
 * @file upf.h
 * @brief header file for all UPF related routines
 */

#if !defined PSPIO_UPF_H
#define PSPIO_UPF_H

#include <stdio.h>

#include "pspio_pspdata.h"


/**********************************************************************
 * Defines                                                            *
 **********************************************************************/

#define GO_BACK    1 /**< Defines that it has to go at the beginning of the file */
#define NO_GO_BACK 0 /**< Defines that it has to stay there in the file*/


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

/**
 * Init the UPF file read
 * @param[in] fp a stream of the input file
 * @param[in,out] psp_data the data structure
 * @return error code
 */
int pspio_upf_init(FILE * fp, pspio_pspdata_t **psp_data);


/**
 * Finalize the file reading
 * @param[in,out] pspdata the data structure
 * @return error code
 */
int pspio_upf_end(pspio_pspdata_t **psp_data);


/**
 * Read the UPF file
 * @param[in] fp a stream of the input file
 * @param[in,out] pspdata the data structure
 * @return error code
 */
int pspio_upf_file_read(FILE * fp, pspio_pspdata_t **pspdata);


/** 
 * Evaluates if a tag is defined
 * @param[in] fp a stream of the input file
 * @param[in] tag the tag. It is case-insensitive
 * @return 0 if defined, 1 otherwise
 */
int tag_isdef(FILE * fp, const char * tag);


/** 
 * Goes to the point just after the tag 
 * @param[in] fp a stream of the input file
 * @param[in] tag the tag. It is case-insensitive
 * @param[in] go_back decides if it has to go to the beginning of the file
 * @return error code
 */
int init_tag(FILE * fp, const char * tag, const int go_back);

/** 
 * Evaluates if a tag is correctly closed
 * @param[in] fp a stream of the input file
 * @param[in] tag the tag. It is case-insensitive
 * @return 0 if correct, 1 otherwise
 */
int check_end_tag(FILE * fp, const char * tag);


/**
* subroutine converts pwcf xc string to libxc codes
*@param[in]  xc_string: pwscf string
*@param[out] exchange: libxc code for exchange
*@param[out] correlation: libxc code for correlation
*/
int upf_to_libxc (const char *xc_string, int *exchange, int *correlation);


/**
* subroutine converts libxc codes to pwcf xc string
*@param[in] exchange: libxc code for exchange
*@param[in] correlation: libxc code for correlation
*@param[out]  xc_string: pwscf string
*/
int upf_from_libxc (const int exchange, const int correlation, char *xc_string);


#endif
