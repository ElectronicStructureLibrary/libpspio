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

 $Id: pspio_nlcc.h 71 2011-08-30 13:22:28Z micael $
*/

/**
 * @file pspio_nlcc.h
 * @brief header file for the handling of the nlccs
 */

#ifndef PSPIO_NLCC_H
#define PSPIO_NLCC_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "pspio_error.h"
#include "pspio_mesh.h"
#include "pspio_meshfunc.h"

/** 
 * values for NLCC - could add possibilities for different schemes
 */
#define NLCC_NO 0
#define NLCC_YES 1
#define NLCC_LOUIE 2 // S. G. Louie, S. Froyen, and M. L. Cohen. Nonlinear ionic pseudopotentials in spin-density-functional calculations. Phys. Rev. B, 26:1738-1742, 1982. 


/**
 * nlcc structure
 */
typedef struct{
  pspio_meshfunc_t *cdens  /**< pseudonlcc, on a radial mesh */
} pspio_nlcc_t;


/**
 * Allocates memory and preset nlcc structure
 * 
 * @param[in,out] nlcc: nlcc structure
 * @param[in] m: pointer to mesh
 * @return error code
 */
int pspio_nlcc_alloc(pspio_nlcc_t *nlcc, pspio_mesh_t *m);


/**
 * Sets the nlcc data.
 * @param[in,out] nlcc: nlcc structure to set
 * @param[in] cdens: values of the nlcc on the mesh
 * @return error code
 */
int pspio_nlcc_set(pspio_nlcc_t *nlcc, double *cdens);


/**
 * Frees all memory associated with nlcc structure
 * 
 * @param[in,out] nlcc: nlcc structure
 */
void pspio_nlcc_free(pspio_nlcc_t *nlcc);

#endif
