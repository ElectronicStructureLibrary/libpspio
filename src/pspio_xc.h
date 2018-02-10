/* Copyright (C) 2011-2017 Micael Oliveira <micael.oliveira@mpsd.mpg.de>
 *                         Yann Pouillon <devops@materialsevolution.es>
 *
 * This file is part of Libpspio.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * version 2.0. If a copy of the MPL was not distributed with this file, You
 * can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Libpspio is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the Mozilla Public License version 2.0 for
 * more details.
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
  int nlcc_scheme; /**< scheme used to obtain the NLCC */
  double nlcc_pf_scale; /**< prefactor for model scale if NLCC (multiplies core-valence crossover radius), ignored otherwise */
  double nlcc_pf_value; /**< prefactor for model amplitude if NLCC (multiplies core-valence crossover value), ignored otherwise */
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
 * Initializes all the parameters of the xc structure.
 * @param[in,out] xc: xc structure pointer to be initialized
 * @param[in] exchange: identifier
 * @param[in] correlation: identifier
 * @param[in] nlcc_scheme:  scheme used to obtain core density
 * @param[in] nlcc_pfs:  NLCC core-valence scale crossover prefactor (radius)
 * @param[in] nlcc_pfv:  NLCC core-valence value crossover prefactor (amplitude)
 * @param[in] mesh: the radial mesh
 * @param[in] cd: values of the core density on the mesh (optional if nlcc_scheme == PSPIO_NLCC_NONE)
 * @param[in] cdp: values of the core density first derivative on the mesh (optional)
 * @param[in] cdpp: values of the core density second derivative on the mesh (optional)
 * @return error code
 * @note The xc pointer is supposed to have been already allocated
 *       with pspio_xc_alloc.
 */
int pspio_xc_init(pspio_xc_t *xc, int exchange, int correlation,
		  int nlcc_scheme, double nlcc_pfs, double nlcc_pfv,
		  const pspio_mesh_t *mesh,
		  const double *cd, const double *cdd, const double *cddd);

/**
 * Duplicates a xc structure.
 * @param[out] dst: destination xc structure pointer
 * @param[in] src: source xc structure pointer 
 * @return error code
 * @note The src pointer has to be allocated first with the pspio_xc_alloc 
 *       method.
 * @note The dst pointer might or might not be allocated first. If it is not,
 *        then it is allocated here.
 */
int pspio_xc_copy(pspio_xc_t **dst, const pspio_xc_t *src);

/**
 * Frees all memory associated with xc structure
 * 
 * @param[in,out] xc: xc structure
 * @note This function can be safelly called even if some or all of the xc 
 *       compoments have not been allocated.
 */
void pspio_xc_free(pspio_xc_t *xc);


/**********************************************************************
 * Setters                                                            *
 **********************************************************************/

/**
 * Sets the exchange id.
 * @param[in,out] xc: xc structure to set
 * @param[in] exchange: identifier
 * @return error code
 */
int pspio_xc_set_exchange(pspio_xc_t *xc, int exchange);

/**
 * Sets the correlation id.
 * @param[in,out] xc: xc structure to set
 * @param[in] correlation: identifier
 * @return error code
 */
int pspio_xc_set_correlation(pspio_xc_t *xc, int correlation);

/**
 * Sets the NLCC core-valence crossover prefactors.
 * @param[in,out] xc: xc structure to set
 * @param[in] nlcc_pfs: prefactor for the crossover scale (radius)
 * @param[in] nlcc_pfv: prefactor for the crossover value (amplitude)
 * @return error code
 */
int pspio_xc_set_nlcc_prefactors(pspio_xc_t *xc, double nlcc_pfs,
      double nlcc_pfv);

/**
 * Sets the NLCC scheme.
 * @param[in,out] xc: xc structure to set
 * @param[in] nlcc_scheme: scheme used to obtain core density
 * @return error code
 */
int pspio_xc_set_nlcc_scheme(pspio_xc_t *xc, int nlcc_scheme);

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
int pspio_xc_set_nlcc_density(pspio_xc_t *xc, const pspio_mesh_t *mesh,
			      const double *cd, const double *cdp, const double *cdpp);


/**********************************************************************
 * Getters                                                            *
 **********************************************************************/

/**
 * Returns the exchange identifier
 * @param[in] xc: xc structure
 * @return exchange identifier
 */
int pspio_xc_get_exchange(const pspio_xc_t *xc);

/**
 * Returns the correlation identifier
 * @param[in] xc: xc structure
 * @return correlation identifier
 */
int pspio_xc_get_correlation(const pspio_xc_t *xc);

/**
 * Returns the NLCC core-valence crossover scale prefactor (multiplies radius)
 * @param[in] xc: xc structure
 * @return the prefactor
 */
int pspio_xc_get_nlcc_pf_scale(const pspio_xc_t *xc);

/**
 * Returns the NLCC core-valence crossover value prefactor (multiplies amplitude)
 * @param[in] xc: xc structure
 * @return the prefactor
 */
int pspio_xc_get_nlcc_pf_value(const pspio_xc_t *xc);

/**
 * Returns the scheme used to generate the NLCC core density
 * @param[in] xc: xc structure
 * @return the scheme used
 */
int pspio_xc_get_nlcc_scheme(const pspio_xc_t *xc);

/**
 * Returns the core density function
 * @param[in] xc: xc structure
 * @return NLCC core density function defined on the mesh
 */
const pspio_meshfunc_t *pspio_xc_get_nlcc_density(const pspio_xc_t *xc);


/**********************************************************************
 * Utility routines                                                   *
 **********************************************************************/

/**
 * Compares two xcs.
 * @param[in] xc1: first xc to compare
 * @param[in] xc2: second xc to compare
 * @return PSPIO_EQUAL when equal, PSPIO_DIFF when different, PSPIO_ERROR if a
 * problem occured.
 */
int pspio_xc_cmp(const pspio_xc_t *xc1, const pspio_xc_t *xc2);

/**
 * Returns the value of the NLCC core density at an arbitrary point
 * @param[in] xc: xc structure
 * @param[in] r: point were we want to evaluate the core density
 * @return value of the core density at r
 * @note The xc pointer has to be fully set.
 */
double pspio_xc_nlcc_density_eval(const pspio_xc_t *xc, double r);

/**
 * Returns the value of the derivative of the NLCC core density at an arbitrary point
 * @param[in] xc: xc structure
 * @param[in] r: point were we want to evaluate the derivative of the core density
 * @return value of the core density derivative at r
 * @note The xc pointer has to be fully set.
 */
double pspio_xc_nlcc_density_eval_deriv(const pspio_xc_t *xc, double r);

/**
 * Returns the value of the second derivative of the NLCC core density at an arbitrary point
 * @param[in] xc: xc structure
 * @param[in] r: point were we want to evaluate the second derivative of the core density
 * @return value of the core density second derivative at r
 * @note The xc pointer has to be fully set.
 */
double pspio_xc_nlcc_density_eval_deriv2(const pspio_xc_t *xc, double r);

/**
 * Returns if xc has non-linear core-corrections
 * @param[in] xc: xc structure
 * @return: true if xc has NLCC, false otherwise
 */
int pspio_xc_has_nlcc(const pspio_xc_t *xc);

#endif
