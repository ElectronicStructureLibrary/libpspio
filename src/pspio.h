/*
 Copyright (C) 2011 J. Alberdi, M. Oliveira, Y. Pouillon, and M. Verstraete

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2, or (at your option)
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 02111-1307, USA.

 $Id$
*/

/**
 * @file pspio.h
 * @brief the include file 
 */

#include <gsl/gsl_spline.h>

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
 * values for NLCC - could add possibilities for different schemes
 */
#define NLCC_NO 0
#define NLCC_YES 1
#define NLCC_LOUIE 2 // S. G. Louie, S. Froyen, and M. L. Cohen. Nonlinear ionic pseudopotentials in spin-density-functional calculations. Phys. Rev. B, 26:1738-1742, 1982. 


/**
 * Main structure for pseudopotential data
 */
struct psp_data_t {
  // general data
  char *title; /**< descriptive string for content of file read in */
  int wave_eq; /**< type of wave equation which was solved: Dirac, Scalar Relativistic, or Schroedinger */
  int exchange; /**< exchange functional id, taken from libxc conventions */
  int correlation; /**< correlation functional id, taken from libxc conventions */
  int scheme; /**< scheme used to generate the pseudopotentials */
  int nlcc; /**< flag for presence of NLCC */
  mesh_t *mesh; /**< Radial mesh */

  // atom data
  double z; /**< Atomic number */
  char *symbol; /**< Atomic symbol */

  // valence data for psp
  double zvalence; /**< charge of pseudopotential ion - valence electrons */
  double nelvalence; /**< number of electrons - normally equal to zion, except for special cases for ions */
  int n_states; /**< number of electronic states */
  states_t *states[]; /**< struct with electronic states */
  int lmax; /**< maximal angular momentum channel */
  int l_local; /**< angular momentum channel of local potential */
  int n_potentials; /**< Number of pseudopotentials */
  potential_t potentials[]; /**< struc with pseudopotentials */ 
  kb_projector_t *kb_projector; /**< Kleinman and Bylander projectors */
};


/**
 * Mesh structure
 */
struct mesh_t{
  int type; /**< Type of mesh */
  int np; /**< Number of points in mesh */
  double a, b; /**< Mesh parameters */
  double r[]; /**< Mesh points */
};


/**
 * Potential structure
 */
struct potential_t{
  qn_t *qn; /**< struct with quantum numbers n l j for the potential */
  double psp[]; /**< pseudopotential, on a radial grid */
  gsl_spline *psp_spl; /**< spline type, for future interpolation before output to calling code */
};

/**
 *General information about the state
 */
struct state_t{
  qn_t *qn; /**< quantum numbers n l j for this wavefunction */
  double *occ; /**< occupation of the electronic state */
  double *eigenval; /**< eigenvalue of electronic state*/
  char *label; /**< string describing the electronic state - eg 2s or 4d1.5  */
  double *rc; /**< cutoff radii used for pseudopotential generation */
  /// The wavefunctions 
  int np; /**< number of points */
  double wf[]; /**< Wavefunction */
  double wfp[]; /**<Derivative of the wavefunction */
};

/** 
 * The quantum numbers
 */
struct qn_t{
  int n; /**< main quantum number */
  int l; /**< angular momentum quantum number */
  double j; /**< total angular momentum quantum number */
};


struct kb_projector_t {
  potential_t *vlocal; /**< local potential */
  int nproj; /**< number of projectors below */
  qn_t qn[]; /**< quantum numbers for present projector */
  double en[]; /**< Kleinman Bylander energies */
  double proj[][]; /**< Kleinman Bylander projectors */
  gsl_spline p_spl[]; /**< spline structures for interpolating projectors*/
};


