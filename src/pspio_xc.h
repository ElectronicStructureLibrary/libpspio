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
 * @file pspio_xc.h
 * @brief header file for the handling of exchange and correlation
 */

#ifndef PSPIO_XC_H
#define PSPIO_XC_H

#include "pspio_error.h"
#include "pspio_mesh.h"
#include "pspio_meshfunc.h"
#include "pspio_xc_funcs.h"

/** 
 * values for NLCC scheme - could add possibilities for different schemes
 */
#define PSPIO_NLCC_NONE -1
#define PSPIO_NLCC_UNKNOWN 0
#define PSPIO_NLCC_LOUIE 1 // S. G. Louie, S. Froyen, and M. L. Cohen. Nonlinear ionic pseudopotentials in spin-density-functional calculations. Phys. Rev. B, 26:1738-1742, 1982. 
#define PSPIO_NLCC_FHI 2 // M. Fuchs and M. Scheffler. Ab initio pseudopotentials for electronic structure calculations of poly-atomic systems using density-functional theory. Comp. Phys. Comm. 119:67-98, 1999.
#define PSPIO_NLCC_TETER1 3 // 7 May 1992 by M. Teter abinit version for pspcod 4
#define PSPIO_NLCC_TETER2 4 // 5 Nov 1992 by M. Teter abinit version for pspcod 1


/**********************************************************************
 * Data structures                                                    *
 **********************************************************************/

/**
 * xc structure
 */
typedef struct{
  int exchange;    /**< exchange functional id, taken from libxc conventions */
  int correlation; /**< correlation functional id, taken from libxc conventions */  
  int nlcc_scheme; /**< Scheme used to obtain the NLCC */
  pspio_meshfunc_t *core_dens; /**< core density, on a radial mesh */
} pspio_xc_t;


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

/**
 * Allocates memory and preset xc structure
 * 
 * @param[in,out] xc: xc structure
 * @param[in] nlcc_scheme: scheme used to obtain core density 
 * @param[in] np: number of points
 * @return error code
 * @note the number of points is only relevant if the nlcc scheme is different 
 *       from PSPIO_NLCC_NONE. In that case it should be larger than 1.
 */
int pspio_xc_alloc(pspio_xc_t **xc, const int nlcc_scheme, const int np);


/**
 * Sets the xc data.
 * @param[in,out] xc: xc structure to set
 * @param[in] exchange: exchange functional id, taken from libxc conventions
 * @param[in] correlation: correlation functional id, taken from libxc conventions
 * @return error code
 */
int pspio_xc_set(pspio_xc_t **xc, const int exchange, const int correlation);


/**
 * Sets the xc data related to NLCC.
 * @param[in,out] xc: xc structure to set
 * @param[in] mesh: pointer to radial mesh
 * @param[in] core_dens: values of the core density on the mesh
 * @return error code
 * @note The xc pointer has to be allocated first with the pspio_xc_alloc
 *       method.
 */
int pspio_xc_nlcc_set(pspio_xc_t **xc, const pspio_mesh_t *mesh,
		      const double *core_dens);


/**
 * Frees all memory associated with xc structure
 * 
 * @param[in,out] xc: xc structure
 * @return error code
 * @note This function can be safelly called even if some or all of the xc 
 *       compoments have not been allocated.
 */
int pspio_xc_free(pspio_xc_t **xc);


/**********************************************************************
 * Atomic routines                                                    *
 **********************************************************************/

/**
 * Returns the xc identifiers
 * @param[in] xc: xc structure
 * @param[out] exchange: identifier
 * @param[out] correlation: identifier
 * @return error code
 */
int pspio_xc_get_id(pspio_xc_t *xc, int *exchange, int *correlation);

/**
 * Returns if xc has non-linear core-corrections
 * @param[in] xc: xc structure
 * @param[out] has_nlcc: true if xc has nlcc, false otherwise
 * @return error code
 */
int pspio_xc_has_nlcc(pspio_xc_t *xc, int *has_nlcc);

/**
 * Returns the value of the core density at an arbitrary point
 * @param[in] xc: xc structure
 * @param[in] r: position were we want to evaluate the core density
 * @param[out] *core_dens: value of the core density at r
 * @return error code
 * @note The xc pointer has to be fully set.
 */
int pspio_xc_nlcc_eval(const pspio_xc_t *xc, const double r, double *core_dens);


/**
 * Returns the core density function
 * @param[in] xc: xc structure
 * @param[out] *cd_func: core density function defined on the mesh
 * @return error code
 * @note The xc pointer has to be fully set.
 */
int pspio_xc_nlcc_get(const pspio_xc_t *xc, pspio_meshfunc_t **cd_func);


#endif
