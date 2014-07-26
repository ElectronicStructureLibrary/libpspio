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

*/

/**
 * @file pspio_xc.h
 * @brief header file for the handling of exchange and correlation
 */

#ifndef PSPIO_XC_H
#define PSPIO_XC_H

#include "pspio_common.h"
#include "pspio_error.h"
#include "pspio_mesh.h"
#include "pspio_meshfunc.h"
#include "pspio_xc_funcs.h"


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
  pspio_meshfunc_t *nlcc_dens; /**< core density, on a radial mesh */
} pspio_xc_t;


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

/**
 * Allocates memory and preset xc structure
 * 
 * @param[in,out] xc: xc structure
 * @return error code
 */
int pspio_xc_alloc(pspio_xc_t **xc);


/**
 * Frees all memory associated with xc structure
 * 
 * @param[in,out] xc: xc structure
 * @note This function can be safelly called even if some or all of the xc 
 *       compoments have not been allocated.
 */
void pspio_xc_free(pspio_xc_t **xc);


/**********************************************************************
 * Atomic routines                                                    *
 **********************************************************************/

/**
 * Sets the exchange-correlation id.
 * @param[in,out] xc: xc structure to set
 * @param[in] exchange: identifier
 * @param[in] correlation: identifier
 */
void pspio_xc_set_id(pspio_xc_t **xc, const int exchange, const int correlation);


/**
 * Sets the xc data.
 * @param[in,out] xc: xc structure to set
 * @param[in] nlcc_scheme: scheme used to obtain core density
 * @return error code
 */
int pspio_xc_set_nlcc_scheme(pspio_xc_t **xc, const int nlcc_scheme);


/**
 * Sets the core density for NLCC.
 * @param[in,out] xc: xc structure to set
 * @param[in] mesh: pointer to radial mesh
 * @param[in] cd: values of the core density on the mesh
 * @param[in] cdp: values of the core density first derivative on the mesh (optional)
 * @param[in] cdpp: values of the core density second derivative on the mesh (optional)
 * @return error code
 *      
 */
int pspio_xc_set_nlcc_density(pspio_xc_t **xc, const pspio_mesh_t *mesh,
			      const double *cd, const double *cdp, const double *cdpp);


/**
 * Returns the xc identifiers
 * @param[in] xc: xc structure
 * @param[out] exchange: identifier
 * @param[out] correlation: identifier
 */
void pspio_xc_get_id(const pspio_xc_t *xc, int *exchange, int *correlation);


/**
 * Returns the scheme used to generate the NLCC core density
 * @param[in] xc: xc structure
 * @param[out] nlcc_scheme: the scheme used
 */
void pspio_xc_get_nlcc_scheme(const pspio_xc_t *xc, int *nlcc_scheme);


/**
 * Returns the core density function
 * @param[in] xc: xc structure
 * @param[out] *cd_func: NLCC core density function defined on the mesh
 */
void pspio_xc_get_nlcc_density(const pspio_xc_t *xc, pspio_meshfunc_t **cd_func);


/**
 * Returns the value of the NLCC core density at an array of arbitrary points
 * @param[in] xc: xc structure
 * @param[in] np: number of points
 * @param[in] *r: positions were we want to evaluate the core density
 * @param[out] *nlcc_dens: values of the core density at r
 * @note The xc pointer has to be fully set.
 */
void pspio_xc_nlcc_density_eval(const pspio_xc_t *xc, const int np, 
				const double *r, double *nlcc_dens);

/**
 * Returns if xc has non-linear core-corrections
 * @param[in] xc: xc structure
 * @return: true if xc has NLCC, false otherwise
 */
int pspio_xc_has_nlcc(const pspio_xc_t *xc);

#endif
