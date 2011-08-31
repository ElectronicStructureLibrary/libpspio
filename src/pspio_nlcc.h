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
 * values for NLCC scheme - could add possibilities for different schemes
 */
#define NLCC_NONE -1
#define NLCC_UNKNOWN 0
#define NLCC_LOUIE 1 // S. G. Louie, S. Froyen, and M. L. Cohen. Nonlinear ionic pseudopotentials in spin-density-functional calculations. Phys. Rev. B, 26:1738-1742, 1982. 
#define NLCC_FHI 2 // M. Fuchs and M. Scheffler. Ab initio pseudopotentials for electronic structure calculations of poly-atomic systems using density-functional theory. Comp. Phys. Comm. 119:67-98, 1999.
#define NLCC_TETER1 3 // 7 May 1992 by M. Teter abinit version for pspcod 4
#define NLCC_TETER2 4 // 5 Nov 1992 by M. Teter abinit version for pspcod 1


/**********************************************************************
 * Data structures                                                    *
 **********************************************************************/

/**
 * nlcc structure
 */
typedef struct{
  int scheme;                  /**< Scheme used to obtain the NLCC */
  pspio_meshfunc_t *core_dens; /**< core density, on a radial mesh */
} pspio_nlcc_t;


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

/**
 * Allocates memory and preset nlcc structure
 * 
 * @param[in,out] nlcc: nlcc structure
 * @param[in] np: number of points in mesh
 * @return error code
 * @note np should be larger than 1.
 */
int pspio_nlcc_alloc(pspio_nlcc_t *nlcc, const int np);


/**
 * Sets the nlcc data.
 * @param[in,out] nlcc: nlcc structure to set
 * @param[in] scheme: scheme used to obtain core density 
 * @param[in] mesh: pointer to radial mesh
 * @param[in] core_dens: values of the core density on the mesh
 * @return error code
 * @note The nlcc pointer has to be allocated first with the pspio_nlcc_alloc
 *       method.
 */
int pspio_nlcc_set(pspio_nlcc_t *nlcc, const int scheme, const pspio_mesh_t *mesh,
		   const double *core_dens);


/**
 * Frees all memory associated with nlcc structure
 * 
 * @param[in,out] nlcc: nlcc structure
 * @return error code
 * @note This function can be safelly called even if some or all of the mesh 
 *       compoments have not been allocated.
 */
int pspio_nlcc_free(pspio_nlcc_t *nlcc);

#endif
