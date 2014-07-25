/*
 Copyright (C) 2011 J. Alberdi, M. Oliveira, Y. Pouillon, and M. Verstraete
 Copyright (C) 2014 M. Oliveira

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
 * Data structures                                                    *
 **********************************************************************/

/**
 * Main structure for pseudopotential data
 */
typedef struct{
  // general data
  int format;        /**< Format of the file. */
  char *info;        /**< descriptive string for content of file read in. Nothing should ever be assumed about its content. */
  char *symbol;      /**< Atomic symbol */
  double z;          /**< Atomic number */
  double zvalence;   /**< charge of pseudopotential ion - valence electrons */
  double nelvalence; /**< number of electrons - normally equal to zion, except for special cases for ions */
  int l_max;         /**< maximal angular momentum channel */
  int wave_eq;       /**< type of wave equation which was solved: Dirac, Scalar Relativistic, or Schroedinger */
  double total_energy; /**< the total energy of the pseudo atom */

  // The radial mesh.
  pspio_mesh_t *mesh; /**< Radial mesh - all functions should be discretized on this mesh */

  // The states
  int **qn_to_istate;     /**< lookup table giving the position of the state from the quantum numbers */ 
  int n_states;           /**< number of electronic states */
  pspio_state_t **states; /**< struct with electronic states */

  // The pseudopotentials
  int scheme;                    /**< scheme used to generate the pseudopotentials */
  int n_potentials;              /**< number of pseudopotentials */
  pspio_potential_t **potentials; /**< struc with pseudopotentials */ 

  // Kleinman and Bylander non-local projectors
  int n_kbproj;                      /**< number of Kleinman and Bylander projectors */
  pspio_projector_t **kb_projectors; /**< Kleinman and Bylander projectors */
  int l_local;                       /**< angular momentum channel of local potential */
  int kb_l_max;                      /**< maximum angular momentum of KB projectors*/
  pspio_potential_t *vlocal;         /**< local potential for the KB form of the pseudopotentials */ 

  // Exchange and correlation data, including non-linear core corrections
  pspio_xc_t *xc; /**< xc structure */

  // Valence density
  pspio_meshfunc_t *rho_valence; /**< valence density */

} pspio_pspdata_t;


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

/**
 * Initialize pspdata with null vales.
 * @param[in,out] pspdata: pointer to pspdata structure to be initialized
 * @return error code.
 */
int pspio_pspdata_init(pspio_pspdata_t **pspdata);

/**
 * Fill pspdata with the data read from a given file.
 * @param[in,out] pspdata: pointer to pspdata structure to be filled
 * @param[in] file_format: the format of the file.
 * @param[in] file_name: file to be parsed.
 * @return error code.
 * @note The file format might be UNKNOWN. In that case all the other
 *       formats are tried until the correct one is found.
 */
int pspio_pspdata_read(pspio_pspdata_t **pspdata, const int *file_format, 
      const char *file_name);

/**
 * Writes the pspdata to a given file. If the specified file format is equal
 * to PSPIO_FMT_UNKNOWN, the routine will set it to the actual format value
 * before returning.
 * @param[in] pspdata: pointer to pspdata structure
 * @param[out] file_name: file write to.
 * @param[in,out] file_format: the format of file_name.
 * @return error code.
 */
int pspio_pspdata_write(const pspio_pspdata_t *pspdata, const int file_format,
      const char *file_name);

/**
 * Reset all the pspdata structure data
 * @param[in,out] pspdata: pointer to pspdata structure to be
 */
void pspio_pspdata_reset(pspio_pspdata_t **pspdata);


/**
 * Frees all memory associated with pspdata structure
 * @param[in,out] pspdata: pointer to pspdata structure to be
 */
void pspio_pspdata_free(pspio_pspdata_t **pspdata);

#endif
