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


/**
 * Main structure for pseudopotential data
 */
typedef struct{
  // general data
  char title[STRLEN_TITLE]; /**< descriptive string for content of file read in */
  int wave_eq; /**< type of wave equation which was solved: Dirac, Scalar Relativisticof the License, or Schroedinger */
  int exchange; /**< exchange functional id, taken from libxc conventions */
  int correlation; /**< correlation functional id, taken from libxc conventions */
  int scheme; /**< scheme used to generate the pseudopotentials */
  int has_nlcc; /**< flag for presence of NLCC */
  pspio_nlcc_t *nlcc; /**< NLCC structure */
  pspio_mesh_t *mesh; /**< Radial mesh */

  // atom data
  double z; /**< Atomic number */
  char *symbol; /**< Atomic symbol */

  // valence data for psp
  double zvalence; /**< charge of pseudopotential ion - valence electrons */
  double nelvalence; /**< number of electrons - normally equal to zion, except for special cases for ions */
  int n_states; /**< number of electronic states */
  pspio_state_t *states; /**< struct with electronic states */
  int l_max; /**< maximal angular momentum channel */
  int l_local; /**< angular momentum channel of local potential */
  int n_potentials; /**< Number of pseudopotentials */
  pspio_potential_t *potentials; /**< struc with pseudopotentials */ 
  int n_kbproj; /**< Number of Kleinman and Bylander projectors */
  pspio_projector_t *kb_projectors; /**< Kleinman and Bylander projectors */
  pspio_potential_t *vlocal; /**< local potential for the KB form of the pseudopotentials */ 

} pspio_pspdata_t;

#endif
