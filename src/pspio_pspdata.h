/* Copyright (C) 2011-2017 Joseba Alberdi <alberdi@hotmail.es>
 *                         Matthieu Verstraete <matthieu.jean.verstraete@gmail.com>
 *                         Micael Oliveira <micael.oliveira@mpsd.mpg.de>
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
#include "pspio_pspinfo.h"


/**********************************************************************
 * Data structures                                                    *
 **********************************************************************/

/**
 * Main structure for pseudopotential data
 */
typedef struct{
  /* general data */
  pspio_pspinfo_t *pspinfo; /**< Generic information about the pseudopotential. */
  int format_guessed;/**< Format of the file guessed by pspio_pspdata_read. */
  char symbol[4];    /**< Atomic symbol */
  double z;          /**< Atomic number */
  double zvalence;   /**< charge of pseudopotential ion */
  double nelvalence; /**< number of electrons; normally equal to zvalence, except for special cases for ions */
  int l_max;         /**< maximal angular momentum channel */
  int wave_eq;       /**< type of wave equation which was solved: Dirac, Scalar Relativistic, or Schroedinger */
  double total_energy; /**< the total energy of the pseudo atom */

  /* The radial mesh. */
  pspio_mesh_t *mesh; /**< Radial mesh - all functions should be discretized on this mesh */

  /* The states */
  int **qn_to_istate;     /**< lookup table giving the position of the state from the quantum numbers */ 
  int n_states;           /**< number of electronic states */
  pspio_state_t **states; /**< struct with electronic states */

  /* The pseudopotentials */
  int scheme;                    /**< scheme used to generate the pseudopotentials */
  int n_potentials;              /**< number of pseudopotentials */
  pspio_potential_t **potentials; /**< struc with pseudopotentials */ 

  /* Non-local projectors */
  int n_projectors;               /**< number of projectors */
  int *n_projectors_per_l;        /**< number of projectors per angular momentum */
  double *projector_energies;     /**< Dij terms for interactions between projectors */
  pspio_projector_t **projectors; /**< projectors */
  int projectors_l_max;           /**< maximum angular momentum of considered in the projectors */
  int l_local;                    /**< angular momentum channel of local potential */
  pspio_potential_t *vlocal;      /**< local potential for the non-local form of the pseudopotentials */ 

  /* Exchange and correlation data, including non-linear core corrections */
  pspio_xc_t *xc; /**< xc structure */

  /* Valence density */
  pspio_meshfunc_t *rho_valence; /**< valence density */

} pspio_pspdata_t;


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

/**
 * Allocates memory and preset pspdata structure
 * 
 * @param[in,out] pspdata: pspdata structure
 * @return error code
 */
int pspio_pspdata_alloc(pspio_pspdata_t **pspdata);

/**
 * Frees all memory associated with pspdata structure
 * @param[in,out] pspdata: pointer to pspdata structure to be
 */
void pspio_pspdata_free(pspio_pspdata_t *pspdata);

/**
 * Fill pspdata with the data read from a given file.
 * @param[in,out] pspdata: pointer to pspdata structure to be filled
 * @param[in] file_format: the format of the file.
 * @param[in] file_name: file to be parsed.
 * @return error code.
 * @note The file format might be UNKNOWN. In that case all the other
 *       formats are tried until the correct one is found.
 */
int pspio_pspdata_read(pspio_pspdata_t *pspdata, int file_format, const char *file_name);

/**
 * Writes the pspdata to a given file. If the specified file format is equal
 * to PSPIO_FMT_UNKNOWN, the routine will set it to the actual format value
 * before returning.
 * @param[in] pspdata: pointer to pspdata structure
 * @param[out] file_name: file write to.
 * @param[in,out] file_format: the format of file_name.
 * @return error code.
 */
int pspio_pspdata_write(pspio_pspdata_t *pspdata, int file_format, const char *file_name);

/**
 * Reset all the pspdata structure data
 * @param[in,out] pspdata: pointer to pspdata structure to be
 */
void pspio_pspdata_reset(pspio_pspdata_t *pspdata);


/**********************************************************************
 * Setters                                                            *
 **********************************************************************/

/**
 * @param[in,out] pspdata: pointer to pspdata structure
 * @param[in] pspinfo: pointer to the pseudopotential information
 * @return error code
 */
int pspio_pspdata_set_pspinfo(pspio_pspdata_t *pspdata, const pspio_pspinfo_t *pspinfo);

/**
 * @param[in,out] pspdata: pointer to pspdata structure
 * @param[in] symbol: pointer to atomic symbol
 * @return error code
 */
int pspio_pspdata_set_symbol(pspio_pspdata_t *pspdata, const char symbol[]);

/**
 * @param[in,out] pspdata: pointer to pspdata structure
 * @param[in] z: atomic number
 * @return error code
 */
int pspio_pspdata_set_z(pspio_pspdata_t *pspdata, double z);

/**
 * @param[in,out] pspdata: pointer to pspdata structure
 * @param[in] zvalence: valence charge
 * @return error code
 */
int pspio_pspdata_set_zvalence(pspio_pspdata_t *pspdata, double zvalence);

/**
 * @param[in,out] pspdata: pointer to pspdata structure
 * @param[in] nelvalence: number of electrons
 * @return error code
 */
int pspio_pspdata_set_nelvalence(pspio_pspdata_t *pspdata, double nelvalence);

/**
 * @param[in,out] pspdata: pointer to pspdata structure
 * @param[in] l_max: maximal angular momentum channel
 * @return error code
 */
int pspio_pspdata_set_l_max(pspio_pspdata_t *pspdata, int l_max);

/**
 * @param[in,out] pspdata: pointer to pspdata structure
 * @param[in] wave_eq: type of wave-equation solved
 * @return error code
 */
int pspio_pspdata_set_wave_eq(pspio_pspdata_t *pspdata, int wave_eq);

/**
 * @param[in,out] pspdata: pointer to pspdata structure
 * @param[in] total_energy: total energy of pseudo-atom
 * @return error code
 */
int pspio_pspdata_set_total_energy(pspio_pspdata_t *pspdata, double total_energy);

/**
 * @param[in,out] pspdata: pointer to pspdata structure
 * @param[in] mesh: pointer to mesh
 * @return error code
 */
int pspio_pspdata_set_mesh(pspio_pspdata_t *pspdata, const pspio_mesh_t *mesh);

/**
 * @param[in,out] pspdata: pointer to pspdata structure
 * @param[in] n_states: number of electronic states
 * @return error code
 */
int pspio_pspdata_set_n_states(pspio_pspdata_t *pspdata, int n_states);

/**
 * @param[in,out] pspdata: pointer to pspdata structure
 * @param[in] index: index of state to be set
 * @param[in] state: pointer to state
 * @return error code
 */
int pspio_pspdata_set_state(pspio_pspdata_t *pspdata, int index, const pspio_state_t *state);

/**
 * @param[in,out] pspdata: pointer to pspdata structure
 * @param[in] n_states: scheme used to generate the pseudopotentials
 * @return error code
 */
int pspio_pspdata_set_scheme(pspio_pspdata_t *pspdata, int scheme);

/**
 * @param[in,out] pspdata: pointer to pspdata structure
 * @param[in] n_potentials: number of potentials
 * @return error code
 */
int pspio_pspdata_set_n_potentials(pspio_pspdata_t *pspdata, int n_potentials);

/**
 * @param[in,out] pspdata: pointer to pspdata structure
 * @param[in] index: index of potential to be set
 * @param[in] potential: pointer to potential
 * @return error code
 */
int pspio_pspdata_set_potential(pspio_pspdata_t *pspdata, int index, const pspio_potential_t *potential);

/**
 * @param[in,out] pspdata: pointer to pspdata structure
 * @param[in] n_projectors: number of projectors
 * @return error code
 */
int pspio_pspdata_set_n_projectors(pspio_pspdata_t *pspdata, int n_projectors);

/**
 * @param[in,out] pspdata: pointer to pspdata structure
 * @param[in] n_ppl: array storing the number of projectors for each angular momentum
 * @note array length must be less than 7
 * @return error code
 */
int pspio_pspdata_set_n_projectors_per_l(pspio_pspdata_t *pspdata, int *n_ppl);

/**
 * @param[in,out] pspdata: pointer to pspdata structure
 * @param[in] index: index of projector to be set
 * @param[in] projector: pointer to projector
 * @return error code
 */
int pspio_pspdata_set_projector(pspio_pspdata_t *pspdata, int index, const pspio_projector_t *projector);

/**
 * @param[in,out] pspdata: pointer to pspdata structure
 * @param[in] projectors_l_max: maximal angular momentum of projectors
 * @return error code
 */
int pspio_pspdata_set_projectors_l_max(pspio_pspdata_t *pspdata, int l_max);

/**
 * @param[in,out] pspdata: pointer to pspdata structure
 * @param[in] l_local: angular momentum channel of local potential
 * @return error code
 */
int pspio_pspdata_set_l_local(pspio_pspdata_t *pspdata, int l_local);

/**
 * @param[in,out] pspdata: pointer to pspdata structure
 * @param[in] vlocal: pointer to the local potential
 * @return error code
 */
int pspio_pspdata_set_vlocal(pspio_pspdata_t *pspdata, const pspio_potential_t *vlocal);

/**
 * @param[in,out] pspdata: pointer to pspdata structure
 * @param[in] xc: pointer to the xc data
 * @return error code
 */
int pspio_pspdata_set_xc(pspio_pspdata_t *pspdata, const pspio_xc_t *xc);

/**
 * @param[in,out] pspdata: pointer to pspdata structure
 * @param[in] rho_valence: pointer to the valence density
 * @return error code
 */
int pspio_pspdata_set_rho_valence(pspio_pspdata_t *pspdata, const pspio_meshfunc_t *rho_valence);

/**
 * @param[in,out] pspdata: pointer to pspdata structure
 * @param[in] energies: matrix of projector energies (ordered [i * n + j])
 * @return error code
 */
int pspio_pspdata_set_projector_energies(pspio_pspdata_t *pspdata,
                                         const double *energies);

/**********************************************************************
 * Getters                                                            *
 **********************************************************************/

/**
 * @param[in] pspdata: pointer to pspdata structure
 * @return format guessed
 */
int pspio_pspdata_get_format_guessed(const pspio_pspdata_t *pspdata);

/**
 * @param[in] pspdata: pointer to pspdata structure
 * @return pointer to pseudopotential information
 */
const pspio_pspinfo_t * pspio_pspdata_get_pspinfo(const pspio_pspdata_t *pspdata);

/**
 * @param[in] pspdata: pointer to pspdata structure
 * @return pointer to atomic symbol
 */
const char * pspio_pspdata_get_symbol(const pspio_pspdata_t *pspdata);

/**
 * @param[in] pspdata: pointer to pspdata structure
 * @return atomic number
 */
double pspio_pspdata_get_z(const pspio_pspdata_t *pspdata);

/**
 * @param[in] pspdata: pointer to pspdata structure
 * @return valence charge
 */
double pspio_pspdata_get_zvalence(const pspio_pspdata_t *pspdata);

/**
 * @param[in] pspdata: pointer to pspdata structure
 * @return number of electrons
 */
double pspio_pspdata_get_nelvalence(const pspio_pspdata_t *pspdata);

/**
 * @param[in] pspdata: pointer to pspdata structure
 * @return maximal angular momentum channel.
 */
int pspio_pspdata_get_l_max(const pspio_pspdata_t *pspdata);

/**
 * @param[in] pspdata: pointer to pspdata structure
 * @return type of wave-equation solved
 */
int pspio_pspdata_get_wave_eq(const pspio_pspdata_t *pspdata);

/**
 * @param[in] pspdata: pointer to pspdata structure
 * @return total energy of pseudo-atom
 */
double pspio_pspdata_get_total_energy(const pspio_pspdata_t *pspdata);

/**
 * @param[in] pspdata: pointer to pspdata structure
 * @return pointer to mesh
 */
const pspio_mesh_t * pspio_pspdata_get_mesh(const pspio_pspdata_t *pspdata);

/**
 * @param[in] pspdata: pointer to pspdata structure
 * @return number of states
 */
int pspio_pspdata_get_n_states(const pspio_pspdata_t *pspdata);

/**
 * @param[in] pspdata: pointer to pspdata structure
 * @param[in] index: index of state to get
 * @return pointer to state
 */
const pspio_state_t * pspio_pspdata_get_state(const pspio_pspdata_t *pspdata, int index);

/**
 * @param[in] pspdata: pointer to pspdata structure
 * @return scheme used to generate the pseudopotentials
 */
int pspio_pspdata_get_scheme(const pspio_pspdata_t *pspdata);

/**
 * @param[in] pspdata: pointer to pspdata structure
 * @return number of potentials
 */
int pspio_pspdata_get_n_potentials(const pspio_pspdata_t *pspdata);

/**
 * @param[in] pspdata: pointer to pspdata structure
 * @param[in] index: index of potential to get
 * @return pointer to potential
 */
const pspio_potential_t * pspio_pspdata_get_potential(const pspio_pspdata_t *pspdata, int index);

/**
 * @param[in] pspdata: pointer to pspdata structure
 * @return number of projectors
 */
int pspio_pspdata_get_n_projectors(const pspio_pspdata_t *pspdata);

/**
 * @param[in] pspdata: pointer to pspdata structure
 * @return number of projectors per angular momentum (maximum up to l=6)
 */
int * pspio_pspdata_get_n_projectors_per_l(const pspio_pspdata_t *pspdata);

/**
 * @param[in] pspdata: pointer to pspdata structure
 * @param[in] index: index of projector to get
 * @return pointer to projector
 */
const pspio_projector_t * pspio_pspdata_get_projector(const pspio_pspdata_t *pspdata, int index);

/**
 * @param[in] pspdata: pointer to pspdata structure
 * @return maximal angular momentum of projectors.
 */
int pspio_pspdata_get_projectors_l_max(const pspio_pspdata_t *pspdata);

/**
 * @param[in] pspdata: pointer to pspdata structure
 * @return angular momentum channel of local potential
 */
int pspio_pspdata_get_l_local(const pspio_pspdata_t *pspdata);

/**
 * @param[in] pspdata: pointer to pspdata structure
 * @return pointer to the local potential
 */
const pspio_potential_t * pspio_pspdata_get_vlocal(const pspio_pspdata_t *pspdata);

/**
 * @param[in] pspdata: pointer to pspdata structure
 * @return pointer to the xc data
 */
const pspio_xc_t * pspio_pspdata_get_xc(const pspio_pspdata_t *pspdata);

/**
 * @param[in] pspdata: pointer to pspdata structure
 * @return pointer to the valence density
 */
const pspio_meshfunc_t * pspio_pspdata_get_rho_valence(const pspio_pspdata_t *pspdata);

/**
 * @param[in] pspdata: pointer to pspdata structure
 * @param[in] i: index of first projector
 * @param[in] j: index of second projector
 * @return the interaction energy between projector i and projector j
 */
double pspio_pspdata_get_projector_energy(const pspio_pspdata_t *pspdata,
                                          int i, int j);

#endif
