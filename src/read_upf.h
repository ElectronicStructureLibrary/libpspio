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
 * @file read_upf.h
 * @brief header file for reading the UPF files
 */

#if !defined PSPIO_READIUPF_H
#define PSPIO_READUPF_H

#include <stdio.h>
#include "pspio_pspdata.h"

#define GO_BACK 1 /**< Defines that it has to go at the beginning of the file */
#define NO_GO_BACK 0 /**< Defines that it has to stay there in the file*/

typedef struct{
	char nl[2];
	int l;
	double occ;
} wavefunction_t;

/**
 * Init the UPF file read
 * @param[in] fp a stream of the input file
 * @param[in,out] psp_data the data structure
 * @return error code
 */
int pspio_upf_init(FILE * fp, pspio_pspdata_t * psp_data);


/**
 * Finalize the file reading
 * @param[in,out] psp_data the data structure
 * @return error code
 */
int pspio_upf_end(pspio_pspdata_t * psp_data);


/**
 * Read the UPF file
 * @param[in] fp a stream of the input file
 * @param[in,out] psp_data the data structure
 * @return error code
 */
int pspio_upf_file_read(FILE * fp, pspio_pspdata_t * psp_data);


/** 
 * Evaluates is a tag is defined
 * @param[in] fp a stream of the input file
 * @param[in] tag the tag
 * @return 0 if defined. 1 else
 */
int tag_isdef(FILE * fp, const char * tag);


/** 
 * Goes to the point just after the tag 
 * @param[in] fp a stream of the input file
 * @param[in] tag the tag
 * @param[in] go_back decides it has to go to the beginning of the file
 * @return error code
 */
int init_tag(FILE * fp, const char * tag, const int go_back);

/** 
 * Evaluates if a tag is correctly closed
 * @param[in] fp a stream of the input file
 * @param[in] tag the tag
 * @return 0 if correct. 1 else
 */
int check_end_tag(FILE * fp, const char * tag);

/**
 * Description
 * @param[in,out] psp_data the data structure
 */
int pspio_upf_cutoff_radii(pspio_pspdata_t * psp_data);

/**
 * checks normalization of the pseudo wavefunctions
 * @param[in,out] psp_data the data structure
 */   
int pspio_upf_check_rphi(pspio_pspdata_t * psp_data);

#endif
