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

#ifndef PSPIO_H
#define PSPIO_H

/**
 * @file pspio.h
 * @brief high-level include file 
 */

#include "pspio_mesh.h"
#include "pspio_meshfunc.h"
#include "pspio_potential.h"
#include "pspio_state.h"
#include "pspio_projector.h"
#include "pspio_nlcc.h"


/**********************************************************************
 * Defines                                                            *
 **********************************************************************/

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
  char title[STRLEN_TITLE]; /**< descriptive string for content of file read in */
  char *symbol;      /**< Atomic symbol */
  double z;          /**< Atomic number */
  double zvalence;   /**< charge of pseudopotential ion - valence electrons */
  double nelvalence; /**< number of electrons - normally equal to zion, except for special cases for ions */
  int l_max;         /**< maximal angular momentum channel */
  int wave_eq;       /**< type of wave equation which was solved: Dirac, Scalar Relativisticof the License, or Schroedinger */

  // The radial mesh.
  pspio_mesh_t *mesh; /**< Radial mesh - all functions should be discretized on this mesh */

  // The states
  int n_states;          /**< number of electronic states */
  pspio_state_t *states; /**< struct with electronic states */

  // The pseudopotentials
  int scheme;                    /**< scheme used to generate the pseudopotentials */
  int n_potentials;              /**< Number of pseudopotentials */
  pspio_potential_t *potentials; /**< struc with pseudopotentials */ 

  // Kleinman and Bylander non-local projectors
  int n_kbproj;                     /**< Number of Kleinman and Bylander projectors */
  pspio_projector_t *kb_projectors; /**< Kleinman and Bylander projectors */
  int l_local;                      /**< angular momentum channel of local potential */
  pspio_potential_t *vlocal;        /**< local potential for the KB form of the pseudopotentials */ 

  // Exchange and correlation data, including non-linear core corrections
  int exchange;       /**< exchange functional id, taken from libxc conventions */
  int correlation;    /**< correlation functional id, taken from libxc conventions */
  int has_nlcc;       /**< flag for presence of NLCC */
  pspio_nlcc_t *nlcc; /**< NLCC structure */

} pspio_pspdata_t;


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

/**
 * 
 * @param[in] file_name: file to be parsed.
 * @param[in] file_format: the format of file_name.
 * @param[in,out] psp_data: pointer to psp_data structure to be initialized
 */
int psp_init(char * file_name, int file_format, pspio_pspdata_t * psp_data);

#endif
