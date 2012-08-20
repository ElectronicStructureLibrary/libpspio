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

#ifndef PSPIO_PSPDATA_H
#define PSPIO_PSPDATA_H

/**
 * @file pspio_pspdata.h
 * @brief header file for handling the to pseudopotential data structure
 */

#include "pspio_mesh.h"
#include "pspio_meshfunc.h"
#include "pspio_potential.h"
#include "pspio_state.h"
#include "pspio_projector.h"
#include "pspio_xc.h"


/**********************************************************************
 * Defines                                                            *
 **********************************************************************/

/**
 * File formats
 */
#define UNKNOWN -1
#define ABINIT_4 1
#define ABINIT_5 2
#define ABINIT_6 3
#define ABINIT_HGH 4
#define ABINIT_GTH 5
#define ATOM 6
#define FHI98PP 7
#define SIESTA 8
#define UPF 9
#define OCTOPUS_HGH 10

/**
 * Wave-equations 
 */
#define SCHRODINGER 1
#define SCALAR_REL 2
#define DIRAC 3

/**
 * Pseudopotential generation schemes
 */
#define BHS 1
#define HSC 2
#define HAMANN 3
#define KERKER 4
#define TM 5
#define TM2 6
#define MRPP 7
#define HGH 8
#define GTH 9
#define RRKJ 10


/**********************************************************************
 * Data structures                                                    *
 **********************************************************************/

/**
 * Main structure for pseudopotential data
 */
typedef struct{
  // general data
  char * title; /**< descriptive string for content of file read in */
  char *symbol;      /**< Atomic symbol */
  double z;          /**< Atomic number */
  double zvalence;   /**< charge of pseudopotential ion - valence electrons */
  double nelvalence; /**< number of electrons - normally equal to zion, except for special cases for ions */
  int l_max;         /**< maximal angular momentum channel */
  int wave_eq;       /**< type of wave equation which was solved: Dirac, Scalar Relativistic, or Schroedinger */

  // The radial mesh.
  pspio_mesh_t *mesh; /**< Radial mesh - all functions should be discretized on this mesh */

  // The states
  int **qn_to_istate;     /**< lookup table giving the position of the state from the quantum numbers */ 
  int n_states;           /**< number of electronic states */
  pspio_state_t **states; /**< struct with electronic states */

  // The pseudopotentials
  int scheme;                    /**< scheme used to generate the pseudopotentials */
  int n_potentials;              /**< Number of pseudopotentials */
  pspio_potential_t **potentials; /**< struc with pseudopotentials */ 

  // Kleinman and Bylander non-local projectors
  int n_kbproj;                     /**< Number of Kleinman and Bylander projectors */
  pspio_projector_t **kb_projectors; /**< Kleinman and Bylander projectors */
  int l_local;                      /**< angular momentum channel of local potential */
  pspio_potential_t *vlocal;        /**< local potential for the KB form of the pseudopotentials */ 

  // Exchange and correlation data, including non-linear core corrections
  pspio_xc_t *xc; /**< xc structure */

  // Valence density
  pspio_meshfunc_t *rho_valence; /**< Valence density */

} pspio_pspdata_t;


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

/**
 * Initialize the psp_data with the data read from a given input file.
 * @param[in,out] psp_data: pointer to psp_data structure to be initialized
 * @param[in] file_name: file to be parsed.
 * @param[in] file_format: the format of file_name.
 * @return error code.
 * @note The file format might be UNKNOWN. In that case all the other
 *       formats are tried until the correct one is found.
 */
int pspio_pspdata_init(pspio_pspdata_t **psp_data, const char *file_name, 
		       const int file_format);

/**
 * Writes the psp_data to a given file.
 * @param[in] psp_data: pointer to psp_data structure
 * @param[out] file_name: file write to.
 * @param[in] file_format: the format of file_name.
 * @return error code.
 */
int pspio_pspdata_write(const pspio_pspdata_t *psp_data, const char *file_name, 
		       const int file_format);

/**
 * Frees all memory associated with psp_data structure
 * @param[in,out] psp_data: pointer to psp_data structure to be
 * @return error code.
 */
int pspio_pspdata_free(pspio_pspdata_t **psp_data);


/**********************************************************************
 * Atomic routines                                                    *
 **********************************************************************/

/**
 * Gets a potential associated with the psp_data structure for given
 * radius and angular momenta.
 * @param[in] data: pointer to psp_data structure to be probed
 * @param[in] l: angular momentum
 * @param[in] j: total angular momentum
 * @param[in] r: radius
 * @param[out] value: value of the specified potential at r
 * @return error code.
 * @note This routine is called many times when using Libpspio. Please
 *       keep performance in mind when tweaking it.
 */
int pspio_pspdata_potential_get(const pspio_pspdata_t *data, const int l, 
     const double j, const double r, double *value);

/**
 * Gets a KB projector associated with the psp_data structure for given
 * radius and angular momenta.
 * @param[in] data: pointer to psp_data structure to be probed
 * @param[in] l: angular momentum
 * @param[in] j: total angular momentum
 * @param[in] r: radius
 * @param[out] value: value of the specified projector at r
 * @return error code.
 * @note This routine is called many times when using Libpspio. Please
 *       keep performance in mind when tweaking it.
 */
int pspio_pspdata_kbprojector_get(const pspio_pspdata_t *data, const int l,
      const double j, const double r, double *value);


/**
 * Gets a KB energy associated with the psp_data structure for given
 * angular momenta.
 * @param[in] data: pointer to psp_data structure to be probed
 * @param[in] l: angular momentum
 * @param[in] j: total angular momentum
 * @param[out] value: value of the energy of the specified projector
 * @return error code.
 */
int pspio_pspdata_kbenergy_get(const pspio_pspdata_t *data, const int l,
      const double j, double *value);

#endif
