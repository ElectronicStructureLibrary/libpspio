/*
 Copyright (C) 2012 M. Oliveira, Y. Pouillon
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

/**
 * @file pspio_fortran_c.c
 * @brief Fortran wrappers
 * 
 * Some implementation notes:
 *
 * - *alloc/init* functions: a pointer of the correct variable type
 *   must be declared in the interface functions, and this is what we
 *   pass back to the Fortran code.
 *
 * - *free* functions: a pointer of the correct variable type must be
 *   declared in the interface functions, and this is what has to be
 *   passed to the C code.
 *
 * - *set/get* functions: all the set/get functions should do a *copy*
 *   of the data.
 *
 * - string arguments: these must come last in the argument list, so
 *   that we can use a couple of tricks to convert Fortran strings to
 *   and from C strings.
 *   
 */

#include <stdlib.h>
#include <string.h>

#include "pspio_pspdata.h"
#include "pspio_info.h"
#include "pspio_error.h"

#include "test_common.h"

#include "config.h"
#include "string_f.h"


/**********************************************************************
 * pspio_pspdata                                                      *
 **********************************************************************/

// pspdata_init
int FC_FUNC_(pspio_f90_pspdata_init, PSPIO_F90_PSPDATA_INIT)
     (void **pspdata)
{
  pspio_pspdata_t *pspdata_p = NULL;

  SUCCEED_OR_RETURN( pspio_pspdata_init(&pspdata_p) );
  *pspdata = (void *) pspdata_p;

  return PSPIO_SUCCESS;
}

// pspdata_read
int FC_FUNC_(pspio_f90_pspdata_read, PSPIO_F90_PSPDATA_READ)
     (void **pspdata, const int *format, STR_F_TYPE filename STR_ARG1)
{
  char *filename_c;

  TO_C_STR1(filename, filename_c);
  DEFER_FUNC_ERROR( pspio_pspdata_read( (pspio_pspdata_t *)(*pspdata), *format, filename_c) );
  free(filename_c);

  RETURN_ON_DEFERRED_ERROR;
  return PSPIO_SUCCESS;
}

// pspdata_write
int FC_FUNC_(pspio_f90_pspdata_write, PSPIO_F90_PSPDATA_WRITE)
     (void ** pspdata, int *format, STR_F_TYPE filename STR_ARG1)
{
  char *filename_c;

  TO_C_STR1(filename, filename_c);
  DEFER_FUNC_ERROR( pspio_pspdata_write( (pspio_pspdata_t *)(*pspdata), *format, filename_c) );
  free(filename_c);

  RETURN_ON_DEFERRED_ERROR;
  return PSPIO_SUCCESS;
}

// pspdata_free
void FC_FUNC_(pspio_f90_pspdata_free, PSPIO_F90_PSPDATA_FREE)
     (void ** pspdata)
{
  pspio_pspdata_t *pspdata_p = NULL;

  pspdata_p = ((pspio_pspdata_t *)(*pspdata));
  pspio_pspdata_free(&pspdata_p);
}

// pspdata_get_format_guessed
void FC_FUNC_(pspio_f90_pspdata_get_format_guessed, PSPIO_F90_PSPDATA_GET_FORMAT_GUESSED)
     (void ** pspdata, int *format)
{
  *format = ((pspio_pspdata_t *)(*pspdata))->format_guessed;
}

// pspdata_set_symbol
void FC_FUNC_(pspio_f90_pspdata_set_symbol, PSPIO_F90_PSPDATA_SET_SYMBOL)
     (void ** pspdata, STR_F_TYPE symbol STR_ARG1)
{
  char *symbol_c;

  TO_C_STR1(symbol, symbol_c);
  
  strcpy(((pspio_pspdata_t *)(*pspdata))->symbol, symbol_c);
  free(symbol_c);
}

// pspdata_get_symbol
void FC_FUNC_(pspio_f90_pspdata_get_symbol, PSPIO_F90_PSPDATA_GET_SYMBOL)
     (void ** pspdata, STR_F_TYPE symbol STR_ARG1)
{
  TO_F_STR1( ((pspio_pspdata_t *)(*pspdata))->symbol, symbol);
}

// pspdata_set_z
void FC_FUNC_(pspio_f90_pspdata_set_z, PSPIO_F90_PSPDATA_SET_Z)
     (void ** pspdata, double *z)
{
  ((pspio_pspdata_t *)(*pspdata))->z = *z;
}

// pspdata_get_z
void FC_FUNC_(pspio_f90_pspdata_get_z, PSPIO_F90_PSPDATA_GET_Z)
     (void ** pspdata, double * z)
{
  *z = ((pspio_pspdata_t *)(*pspdata))->z;
}

// pspdata_set_zvalence
void FC_FUNC_(pspio_f90_pspdata_set_zvalence, PSPIO_F90_PSPDATA_SET_ZVALENCE)
     (void ** pspdata, double *zvalence)
{
  ((pspio_pspdata_t *)(*pspdata))->zvalence = *zvalence;
}

// pspdata_get_zvalence
void FC_FUNC_(pspio_f90_pspdata_get_zvalence, PSPIO_F90_PSPDATA_GET_ZVALENCE)
     (void ** pspdata, double * zvalence)
{
  *zvalence = ((pspio_pspdata_t *)(*pspdata))->zvalence;
}

// pspdata_set_l_max
void FC_FUNC_(pspio_f90_pspdata_set_l_max, PSPIO_F90_PSPDATA_SET_L_MAX)
     (void ** pspdata, int *l_max)
{
  ((pspio_pspdata_t *)(*pspdata))->l_max = *l_max;
}

// pspdata_get_l_max
void FC_FUNC_(pspio_f90_pspdata_get_l_max, PSPIO_F90_PSPDATA_GET_L_MAX)
     (void ** pspdata, int * l_max)
{
  *l_max = ((pspio_pspdata_t *)(*pspdata))->l_max;
}

// pspdata_set_wave_eq
void FC_FUNC_(pspio_f90_pspdata_set_wave_eq, PSPIO_F90_PSPDATA_SET_WAVE_EQ)
     (void ** pspdata, int *wave_eq)
{
  ((pspio_pspdata_t *)(*pspdata))->wave_eq = *wave_eq;
}

// pspdata_get_wave_eq
void FC_FUNC_(pspio_f90_pspdata_get_wave_eq, PSPIO_F90_PSPDATA_GET_WAVE_EQ)
     (void ** pspdata, int * wave_eq)
{
  *wave_eq = ((pspio_pspdata_t *)(*pspdata))->wave_eq;
}

// pspdata_set_mesh
int FC_FUNC_(pspio_f90_pspdata_set_mesh, PSPIO_F90_PSPDATA_SET_MESH)
     (void ** pspdata, void ** mesh)
{
  return pspio_mesh_copy( &((pspio_pspdata_t *)(*pspdata))->mesh, ((pspio_mesh_t *) (*mesh)));
}

// pspdata_get_mesh
int FC_FUNC_(pspio_f90_pspdata_get_mesh, PSPIO_F90_PSPDATA_GET_MESH)
     (void ** pspdata, void ** mesh)
{
  pspio_mesh_t *mesh_p = NULL;

  SUCCEED_OR_RETURN(  pspio_mesh_copy(&mesh_p, ((pspio_pspdata_t *)(*pspdata))->mesh) );
  *mesh = (void *) mesh_p;

  return PSPIO_SUCCESS;
}

// pspdata_set_n_states
void FC_FUNC_(pspio_f90_pspdata_set_n_states, PSPIO_F90_PSPDATA_SET_N_STATES)
     (void ** pspdata, int *n_states)
{
  ((pspio_pspdata_t *)(*pspdata))->n_states = *n_states;
}

// pspdata_get_n_states
void FC_FUNC_(pspio_f90_pspdata_get_n_states, PSPIO_F90_PSPDATA_GET_N_STATES)
     (void ** pspdata, int * n_states)
{
  *n_states = ((pspio_pspdata_t *)(*pspdata))->n_states;
}

// pspdata_set_states
int FC_FUNC_(pspio_f90_pspdata_set_states, PSPIO_F90_PSPDATA_SET_STATES)
     (void ** pspdata, void ** states)
{
  int i;
  pspio_state_t **states_p = NULL;

  states_p = (pspio_state_t **) malloc ( ((pspio_pspdata_t *)(*pspdata))->n_states*sizeof(pspio_state_t *));
  FULFILL_OR_EXIT(states_p != NULL, PSPIO_ENOMEM);

  for (i=0; i<((pspio_pspdata_t *)(*pspdata))->n_states; i++) {
    states_p[i] = NULL;
    SUCCEED_OR_RETURN( pspio_state_copy(&(states_p[i]), ((pspio_state_t *)(states[i]))) );
  }

  ((pspio_pspdata_t *)(*pspdata))->states = states_p;

  SUCCEED_OR_RETURN( pspio_states_lookup_table(((pspio_pspdata_t *)(*pspdata))->n_states, 
					       ((pspio_pspdata_t *)(*pspdata))->states, 
					       &((pspio_pspdata_t *)(*pspdata))->qn_to_istate) );

  return PSPIO_SUCCESS;
}

// pspdata_get_states
int FC_FUNC_(pspio_f90_pspdata_get_states, PSPIO_F90_PSPDATA_GET_STATES)
     (void ** pspdata, void ** states)
{
  int i;
  pspio_state_t **states_p = NULL;

  states_p = (pspio_state_t **) malloc ( ((pspio_pspdata_t *)(*pspdata))->n_states*sizeof(pspio_state_t *));
  FULFILL_OR_EXIT(states_p != NULL, PSPIO_ENOMEM);

  for (i=0; i<((pspio_pspdata_t *)(*pspdata))->n_states; i++) {
    states_p[i] = NULL;
    SUCCEED_OR_RETURN( pspio_state_copy(&(states_p[i]), ((pspio_pspdata_t *)(*pspdata))->states[i]) );
  }

  *states = (void *) states_p;

  return PSPIO_SUCCESS;
}

// pspdata_get_state
int FC_FUNC_(pspio_f90_pspdata_get_state, PSPIO_F90_PSPDATA_GET_STATE)
     (void ** pspdata, int *i, void ** state)
{
  pspio_state_t *state_p = NULL;

  FULFILL_OR_EXIT(*i>0 && *i<=((pspio_pspdata_t *)(*pspdata))->n_states, PSPIO_EVALUE);

  SUCCEED_OR_RETURN( pspio_state_copy(&state_p, ((pspio_pspdata_t *)(*pspdata))->states[*i-1]) );

  *state = (void *) state_p;

  return PSPIO_SUCCESS;
}

// pspdata_set_scheme
void FC_FUNC_(pspio_f90_pspdata_set_scheme, PSPIO_F90_PSPDATA_SET_SCHEME)
     (void ** pspdata, int *scheme)
{
  ((pspio_pspdata_t *)(*pspdata))->scheme = *scheme;
}

// pspdata_get_scheme
void FC_FUNC_(pspio_f90_pspdata_get_scheme, PSPIO_F90_PSPDATA_GET_SCHEME)
     (void ** pspdata, int * scheme)
{
  *scheme = ((pspio_pspdata_t *)(*pspdata))->scheme;
}

// pspdata_set_n_potentials
void FC_FUNC_(pspio_f90_pspdata_set_n_potentials, PSPIO_F90_PSPDATA_SET_N_POTENTIALS)
     (void ** pspdata, int *n_potentials)
{
  ((pspio_pspdata_t *)(*pspdata))->n_potentials = *n_potentials;
}

// pspdata_get_n_potentials
void FC_FUNC_(pspio_f90_pspdata_get_n_potentials, PSPIO_F90_PSPDATA_GET_N_POTENTIALS)
     (void ** pspdata, int * n_potentials)
{
  *n_potentials = ((pspio_pspdata_t *)(*pspdata))->n_potentials;
}

// pspdata_set_potentials
int FC_FUNC_(pspio_f90_pspdata_set_potentials, PSPIO_F90_PSPDATA_SET_POTENTIALS)
     (void ** pspdata, void *** potentials)
{
  int i;
  pspio_potential_t **potentials_p = NULL;

  potentials_p = (pspio_potential_t **) malloc ( ((pspio_pspdata_t *)(*pspdata))->n_potentials*sizeof(pspio_potential_t *));
  FULFILL_OR_EXIT(potentials_p != NULL, PSPIO_ENOMEM);

  for (i=0; i<((pspio_pspdata_t *)(*pspdata))->n_potentials; i++) {
    potentials_p[i] = NULL;
    SUCCEED_OR_RETURN( pspio_potential_copy(&(potentials_p[i]), ((pspio_potential_t *)(potentials[i]))) );
  }

  ((pspio_pspdata_t *)(*pspdata))->potentials = potentials_p;

  return PSPIO_SUCCESS;
}

// pspdata_get_potentials
int FC_FUNC_(pspio_f90_pspdata_get_potentials, PSPIO_F90_PSPDATA_GET_POTENTIALS)
     (void ** pspdata, void *** potentials)
{
  int i;
  pspio_potential_t **potentials_p = NULL;

  potentials_p = (pspio_potential_t **) malloc ( ((pspio_pspdata_t *)(*pspdata))->n_potentials*sizeof(pspio_potential_t *));
  FULFILL_OR_EXIT(potentials_p != NULL, PSPIO_ENOMEM);

  for (i=0; i<((pspio_pspdata_t *)(*pspdata))->n_potentials; i++) {
    potentials_p[i] = NULL;
    SUCCEED_OR_RETURN( pspio_potential_copy(&(potentials_p[i]), ((pspio_pspdata_t *)(*pspdata))->potentials[i]) );
  }

  *potentials = (void *) potentials_p;

  return PSPIO_SUCCESS;
}

// pspdata_get_potential
int FC_FUNC_(pspio_f90_pspdata_get_potential, PSPIO_F90_PSPDATA_GET_POTENTIAL)
     (void ** pspdata, int *i, void *** potential)
{
  pspio_potential_t *potential_p = NULL;

  FULFILL_OR_EXIT(*i>0 && *i<=((pspio_pspdata_t *)(*pspdata))->n_potentials, PSPIO_EVALUE);

  SUCCEED_OR_RETURN( pspio_potential_copy(&potential_p, ((pspio_pspdata_t *)(*pspdata))->potentials[*i-1]) );
  *potential = (void *) potential_p;

  return PSPIO_SUCCESS;
}

// pspdata_set_n_kbproj
void FC_FUNC_(pspio_f90_pspdata_set_n_kbproj, PSPIO_F90_PSPDATA_SET_N_KBPROJ)
     (void ** pspdata, int *n_kbproj)
{
  ((pspio_pspdata_t *)(*pspdata))->n_kbproj = *n_kbproj;
}

// pspdata_get_n_kbproj
void FC_FUNC_(pspio_f90_pspdata_get_n_kbproj, PSPIO_F90_PSPDATA_GET_N_KBPROJ)
     (void ** pspdata, int * n_kbproj)
{
  *n_kbproj = ((pspio_pspdata_t *)(*pspdata))->n_kbproj;
}

// pspdata_set_kb_projectors
int FC_FUNC_(pspio_f90_pspdata_set_kb_projectors, PSPIO_F90_PSPDATA_SET_KB_PROJECTORS)
     (void ** pspdata, void ** kb_projectors)
{
  int i;
  pspio_projector_t **kb_projectors_p = NULL;

  kb_projectors_p = (pspio_projector_t **) malloc ( ((pspio_pspdata_t *)(*pspdata))->n_kbproj*sizeof(pspio_projector_t *));
  FULFILL_OR_EXIT(kb_projectors_p != NULL, PSPIO_ENOMEM);

  for (i=0; i<((pspio_pspdata_t *)(*pspdata))->n_kbproj; i++) {
    kb_projectors_p[i] = NULL;
    SUCCEED_OR_RETURN( pspio_projector_copy(&(kb_projectors_p[i]), ((pspio_projector_t *)(kb_projectors[i]))) );
  }

  ((pspio_pspdata_t *)(*pspdata))->kb_projectors = kb_projectors_p;

  return PSPIO_SUCCESS;
}

// pspdata_set_kb_projectors
int FC_FUNC_(pspio_f90_pspdata_get_kb_projectors, PSPIO_F90_PSPDATA_GET_KB_PROJECTORS)
     (void ** pspdata, void *** kb_projectors)
{
  int i;
  pspio_projector_t **kb_projectors_p = NULL;

  kb_projectors_p = (pspio_projector_t **) malloc ( ((pspio_pspdata_t *)(*pspdata))->n_kbproj*sizeof(pspio_projector_t *));
  FULFILL_OR_EXIT(kb_projectors_p != NULL, PSPIO_ENOMEM);

  for (i=0; i<((pspio_pspdata_t *)(*pspdata))->n_kbproj; i++) {
    kb_projectors_p[i] = NULL;
    SUCCEED_OR_RETURN( pspio_projector_copy(&(kb_projectors_p[i]), ((pspio_pspdata_t *)(*pspdata))->kb_projectors[i]) );
  }

  *kb_projectors = (void *) kb_projectors_p;

  return PSPIO_SUCCESS;
}

// pspdata_get_kb_projector
int FC_FUNC_(pspio_f90_pspdata_get_kb_projector, PSPIO_F90_PSPDATA_GET_KB_PROJECTOR)
     (void ** pspdata, int *i, void ** kb_projector)
{
  pspio_projector_t *kb_projector_p = NULL;

  FULFILL_OR_EXIT(*i>0 && *i<=((pspio_pspdata_t *)(*pspdata))->n_kbproj, PSPIO_EVALUE);

  SUCCEED_OR_RETURN( pspio_projector_copy(&kb_projector_p, ((pspio_pspdata_t *)(*pspdata))->kb_projectors[*i-1]) );

  *kb_projector = (void *) kb_projector_p;

  return PSPIO_SUCCESS;
}

// pspdata_set_l_local
void FC_FUNC_(pspio_f90_pspdata_set_l_local, PSPIO_F90_PSPDATA_SET_L_LOCAL)
     (void ** pspdata, int * l_local)
{
  ((pspio_pspdata_t *)(*pspdata))->l_local = *l_local;
}

// pspdata_get_l_local
void FC_FUNC_(pspio_f90_pspdata_get_l_local, PSPIO_F90_PSPDATA_GET_L_LOCAL)
     (void ** pspdata, int * l_local)
{
  *l_local = ((pspio_pspdata_t *)(*pspdata))->l_local;
}

// pspdata_set_kb_l_max
void FC_FUNC_(pspio_f90_pspdata_set_kb_l_max, PSPIO_F90_PSPDATA_SET_KB_L_MAX)
     (void ** pspdata, int * kb_l_max)
{
  ((pspio_pspdata_t *)(*pspdata))->kb_l_max = *kb_l_max;
}

// pspdata_get_kb_l_max
void FC_FUNC_(pspio_f90_pspdata_get_kb_l_max, PSPIO_F90_PSPDATA_GET_KB_L_MAX)
     (void ** pspdata, int * kb_l_max)
{
  *kb_l_max = ((pspio_pspdata_t *)(*pspdata))->kb_l_max;
}

// pspdata_set_vlocal
int FC_FUNC_(pspio_f90_pspdata_set_vlocal, PSPIO_F90_PSPDATA_SET_VLOCAL)
     (void ** pspdata, void ** vlocal)
{
  pspio_potential_t *vlocal_p = NULL;

  SUCCEED_OR_RETURN( pspio_potential_copy(&vlocal_p, ((pspio_potential_t *)(*vlocal))) );
 
  ((pspio_pspdata_t *)(*pspdata))->vlocal = vlocal_p; 

  return PSPIO_SUCCESS;
}

// pspdata_get_vlocal
int FC_FUNC_(pspio_f90_pspdata_get_vlocal, PSPIO_F90_PSPDATA_GET_VLOCAL)
     (void ** pspdata, void ** vlocal)
{
  pspio_potential_t *vlocal_p = NULL;

  SUCCEED_OR_RETURN( pspio_potential_copy(&vlocal_p, ((pspio_pspdata_t *)(*pspdata))->vlocal) );
 
  *vlocal = (void *) vlocal_p;

  return PSPIO_SUCCESS;
}

// pspdata_set_xc
int FC_FUNC_(pspio_f90_pspdata_set_xc, PSPIO_F90_PSPDATA_SET_XC)
     (void ** pspdata, void ** xc)
{
  pspio_xc_t *xc_p = NULL;

  SUCCEED_OR_RETURN( pspio_xc_copy(&xc_p, ((pspio_xc_t *)(*xc))) );

  ((pspio_pspdata_t *)(*pspdata))->xc = xc_p;

  return PSPIO_SUCCESS;
}

// pspdata_get_xc
int FC_FUNC_(pspio_f90_pspdata_get_xc, PSPIO_F90_PSPDATA_GET_XC)
     (void ** pspdata, void ** xc)
{
  pspio_xc_t *xc_p = NULL;

  SUCCEED_OR_RETURN( pspio_xc_copy(&xc_p, ((pspio_pspdata_t *)(*pspdata))->xc) );

  *xc = (void *) xc_p;

  return PSPIO_SUCCESS;
}

// pspdata_set_rho_valence
int FC_FUNC_(pspio_f90_pspdata_set_rho_valence, PSPIO_F90_PSPDATA_SET_RHO_VALENCE)
     (void ** pspdata, double *rho)
{
  SUCCEED_OR_RETURN( pspio_meshfunc_alloc(&((pspio_pspdata_t *)(*pspdata))->rho_valence, ((pspio_pspdata_t *)(*pspdata))->mesh->np) );
  SUCCEED_OR_RETURN( pspio_meshfunc_init(((pspio_pspdata_t *)(*pspdata))->rho_valence, ((pspio_pspdata_t *)(*pspdata))->mesh, rho, NULL, NULL) );

  return PSPIO_SUCCESS;
}

// pspdata_set_rho_valence_eval_s
void FC_FUNC_(pspio_f90_pspdata_rho_valence_eval_s, PSPIO_F90_PSPDATA_RHO_VALENCE_EVAL_S)
     (void ** pspdata, double *r, double *rho)
{
  pspio_meshfunc_eval(((pspio_pspdata_t *)(*pspdata))->rho_valence, 1, r, rho);
}

// pspdata_set_rho_valence_eval_v
void FC_FUNC_(pspio_f90_pspdata_rho_valence_eval_v, PSPIO_F90_PSPDATA_RHO_VALENCE_EVAL_V)
     (void ** pspdata, int *np, double *r, double *rho)
{
  pspio_meshfunc_eval(((pspio_pspdata_t *)(*pspdata))->rho_valence, *np, r, rho);
}

/**********************************************************************
 * pspio_mesh                                                         *
 **********************************************************************/

// mesh_alloc
int FC_FUNC_(pspio_f90_mesh_alloc, PSPIO_F90_MESH_ALLOC)
     (void ** mesh, int * np)
{
  pspio_mesh_t *mesh_p = NULL;
  int ierr;

  ierr = (int) pspio_mesh_alloc(&mesh_p, *np);
  *mesh = (void *) mesh_p;

  return ierr;
}

// mesh_init
int FC_FUNC_(pspio_f90_mesh_init, PSPIO_F90_MESH_SET)
     (void ** mesh, int *type, double *a, double *b, double *r, double *rab)
{
  return pspio_mesh_init(((pspio_mesh_t *)(*mesh)), *type, *a, *b, r, rab);
}

// mesh_init_from_points
void FC_FUNC_(pspio_f90_mesh_init_from_points, PSPIO_F90_MESH_INIT_FROM_POINTS)
     (pspio_mesh_t **mesh, double *r, double *rab)
{
  pspio_mesh_init_from_points( ((pspio_mesh_t *)(*mesh)), r, rab);
}

// mesh_init_from_parameters
void FC_FUNC_(pspio_f90_mesh_init_from_parameters, PSPIO_F90_MESH_INIT_FROM_PARAMETERS)
     (pspio_mesh_t **mesh, int *type, double *a, double *b)
{
  pspio_mesh_init_from_parameters(  ((pspio_mesh_t *)(*mesh)), *type, *a, *b);
}

// mesh_free
void FC_FUNC_(pspio_f90_mesh_free, PSPIO_F90_MESH_FREE)
     (pspio_mesh_t **mesh)
{
  pspio_mesh_t *mesh_p = NULL;

  mesh_p = ((pspio_mesh_t *)(*mesh));
  pspio_mesh_free(&mesh_p);
}

// mesh_get_np
void FC_FUNC_(pspio_f90_mesh_get_np, PSPIO_F90_MESH_GET_NP)
     (void ** mesh, int * np)
{
  *np = pspio_mesh_get_np( ((pspio_mesh_t *)(*mesh)));
}

// mesh_get_info
void FC_FUNC_(pspio_f90_mesh_get_info, PSPIO_F90_MESH_GET_INFO)
  (void **mesh, int *type, int *np, double *a, double *b){

  *type = ((pspio_mesh_t *)(*mesh))->type;
  *np = ((pspio_mesh_t *)(*mesh))->np;
  *a = ((pspio_mesh_t *)(*mesh))->a;
  *b = ((pspio_mesh_t *)(*mesh))->b;
}

// mesh_get_r
void FC_FUNC_(pspio_f90_mesh_get_r, PSPIO_F90_MESH_GET_R)
     (void ** mesh, double *r)
{
  pspio_mesh_get_r( ((pspio_mesh_t *)(*mesh)), r);
}

// mesh_get_rab
void FC_FUNC_(pspio_f90_mesh_get_rab, PSPIO_F90_MESH_GET_RAB)
     (void ** mesh, double *rab)
{
  pspio_mesh_get_rab( ((pspio_mesh_t *)(*mesh)), rab);
}


/**********************************************************************
 * pspio_state                                                        *
 **********************************************************************/

// state_alloc
int FC_FUNC_(pspio_f90_state_alloc, PSPIO_F90_STATE_ALLOC)
     (void ** state, int * np)
{
  pspio_state_t *state_p = NULL;
  int ierr;

  ierr = (int) pspio_state_alloc(&state_p, *np);
  *state = (void *) state_p;

  return ierr;
}

// state_init
int FC_FUNC_(pspio_f90_state_init, PSPIO_F90_STATE_SET)
     (void ** state, double * eigenval, void ** qn, double * occ, double * rc, void ** mesh, double *wf)
{
  return pspio_state_init(((pspio_state_t *)(*state)), *eigenval, "", ((pspio_qn_t *)(*qn)), *occ, *rc, ((pspio_mesh_t*)(*mesh)), wf);
}

// state_free
void FC_FUNC_(pspio_f90_state_free, PSPIO_F90_STATE_FREE)
     (pspio_state_t **state)
{
  pspio_state_t *state_p;

  state_p = ((pspio_state_t *)(*state));
  pspio_state_free(&state_p);
}

// state_wf_eval_s
void FC_FUNC_(pspio_f90_state_wf_eval_s, PSPIO_F90_STATE_WF_EVAL_S)
     (void ** state, double *r, double *wf)
{
  pspio_state_wf_eval(((pspio_state_t *)(*state)), 1, r, wf);
}

// state_wf_eval_v
void FC_FUNC_(pspio_f90_state_wf_eval_v, PSPIO_F90_STATE_WF_EVAL_V)
     (void ** state, int *np, double *r, double *wf)
{
  pspio_state_wf_eval(((pspio_state_t *)(*state)), *np, r, wf);
}

// state_get_qn
void FC_FUNC_(pspio_f90_state_get_qn, PSPIO_F90_STATE_GET_QN)
     (void ** state, int *n, int *l, double *j)
{
  *n = pspio_state_get_n(((pspio_state_t *)(*state)));
  *l = pspio_state_get_l(((pspio_state_t *)(*state)));
  *j = pspio_state_get_j(((pspio_state_t *)(*state)));
}

// state_get_occ
void FC_FUNC_(pspio_f90_state_get_occ, PSPIO_F90_STATE_GET_OCC)
     (void ** state, double * occ)
{
  *occ = pspio_state_get_occ((pspio_state_t *)(*state));
}

// state_get_ev
void FC_FUNC_(pspio_f90_state_get_ev, PSPIO_F90_STATE_GET_EV)
     (void ** state, double * ev)
{
  *ev = pspio_state_get_ev((pspio_state_t *)(*state));
}

// state_get_rc
void FC_FUNC_(pspio_f90_state_get_rc, PSPIO_F90_STATE_GET_RC)
     (void ** state, double * rc)
{
  *rc = pspio_state_get_rc((pspio_state_t *)(*state));
}


/**********************************************************************
 * pspio_potential                                                    *
 **********************************************************************/

// potential_alloc
int FC_FUNC_(pspio_f90_potential_alloc, PSPIO_F90_POTENTIAL_ALLOC)
     (void ** potential, int * np)
{
  pspio_potential_t *potential_p = NULL;
  int ierr;

  ierr = (int) pspio_potential_alloc(&potential_p, *np);
  *potential = (void *) potential_p;

  return ierr;
}

// potential_init
int FC_FUNC_(pspio_f90_potential_init, PSPIO_F90_POTENTIAL_SET)
     (void ** potential, void ** qn, void ** mesh, double * v)
{
  return pspio_potential_init(((pspio_potential_t *)(*potential)), ((pspio_qn_t *)(*qn)), ((pspio_mesh_t *)(*mesh)), v);
}

// potential_free
void FC_FUNC_(pspio_f90_potential_free, PSPIO_F90_POTENTIAL_FREE)
     (pspio_potential_t **potential)
{
  pspio_potential_t *potential_p;

  potential_p = ((pspio_potential_t *)(*potential));
  pspio_potential_free(&potential_p);
}

// potential_eval_s
void FC_FUNC_(pspio_f90_potential_eval_s, PSPIO_F90_POTENTIAL_EVAL_S)
     (void ** potential, double *r, double *v)
{
  pspio_potential_eval(((pspio_potential_t *)(*potential)), 1, r, v);
}

// potential_eval_v
void FC_FUNC_(pspio_f90_potential_eval_v, PSPIO_F90_POTENTIAL_EVAL_V)
     (void ** potential, int *np, double *r, double *v)
{
  pspio_potential_eval(((pspio_potential_t *)(*potential)), *np, r, v);
}


/**********************************************************************
 * pspio_projector                                                    *
 **********************************************************************/

// projector_alloc
int FC_FUNC_(pspio_f90_projector_alloc, PSPIO_F90_PROJECTOR_ALLOC)
     (void ** projector, int * np)
{
  pspio_projector_t *projector_p = NULL;
  int ierr;

  ierr = (int) pspio_projector_alloc(&projector_p, *np);
  *projector = (void *) projector_p;

  return ierr;
}

// projector_init
int FC_FUNC_(pspio_f90_projector_init, PSPIO_F90_PROJECTOR_SET)
     (void ** projector, void ** qn, double * energy, void ** mesh, double * proj)
{
  return pspio_projector_init(((pspio_projector_t *)(*projector)), ((pspio_qn_t *)(*qn)), *energy, ((pspio_mesh_t *)(*mesh)), proj);
}

// projector_free
void FC_FUNC_(pspio_f90_projector_free, PSPIO_F90_PROJECTOR_FREE)
     (pspio_projector_t **projector)
{
  pspio_projector_t *projector_p;

  projector_p = ((pspio_projector_t *)(*projector));

  pspio_projector_free(&projector_p);
}

// projector_eval_s
void FC_FUNC_(pspio_f90_projector_eval_s, PSPIO_F90_PROJECTOR_EVAL_S)
     (void ** projector, double *r, double *p)
{
  pspio_projector_eval(((pspio_projector_t *)(*projector)), 1, r, p);
}

// projector_eval_v
void FC_FUNC_(pspio_f90_projector_eval_v, PSPIO_F90_PROJECTOR_EVAL_V)
     (void ** projector, int *np, double *r, double *p)
{
  pspio_projector_eval(((pspio_projector_t *)(*projector)), *np, r, p);
}

// projector_eval_energy
void FC_FUNC_(pspio_f90_projector_get_energy, PSPIO_F90_PROJECTOR_GET_ENERGY)
     (void ** projector, double * energy)
{
  *energy = pspio_projector_get_energy((pspio_projector_t *)(*projector));
}

// projector_get_l
void FC_FUNC_(pspio_f90_projector_get_l, PSPIO_F90_PROJECTOR_GET_L)
     (void ** projector, int * l)
{
  *l = pspio_projector_get_l((pspio_projector_t *)(*projector));
}

// projector_get_j
void FC_FUNC_(pspio_f90_projector_get_j, PSPIO_F90_PROJECTOR_GET_J)
     (void ** projector, double * j)
{
  *j = pspio_projector_get_j((pspio_projector_t *)(*projector));
}


/**********************************************************************
 * pspio_xc                                                           *
 **********************************************************************/

// xc_alloc
int FC_FUNC_(pspio_f90_xc_alloc, PSPIO_F90_XC_ALLOC)
     (void ** xc)
{
  pspio_xc_t *xc_p = NULL;
  int ierr;

  ierr = (int) pspio_xc_alloc(&xc_p);
  *xc = (void *) xc_p;

  return ierr;
}

// xc_free
void FC_FUNC_(pspio_f90_xc_free, PSPIO_F90_XC_FREE)
     (void ** xc)
{
  pspio_xc_t *xc_p;

  xc_p = ((pspio_xc_t *)(*xc));

  pspio_xc_free(&xc_p);
}

// xc_set_id
void FC_FUNC_(pspio_f90_xc_set_id, PSPIO_F90_XC_SET_ID)
     (void ** xc, int * exchange, int * correlation)
{
  pspio_xc_set_id(((pspio_xc_t *)(*xc)), *exchange, *correlation);
}

// xc_set_nlcc_scheme
int FC_FUNC_(pspio_f90_xc_set_nlcc_scheme, PSPIO_F90_XC_SET_NLCC_SCHEME)
     (void ** xc, int * nlcc_scheme)
{
  return pspio_xc_set_nlcc_scheme(((pspio_xc_t *)(*xc)), *nlcc_scheme);
}

// xc_set_nlcc_density
int FC_FUNC_(pspio_f90_xc_set_nlcc_density1, PSPIO_F90_XC_SET_NLCC_DENSITY1)
     (void ** xc, void ** mesh, double * cd)
{
  return pspio_xc_set_nlcc_density(((pspio_xc_t *)(*xc)), ((pspio_mesh_t *)(*mesh)), cd, NULL, NULL);
}

int FC_FUNC_(pspio_f90_xc_set_nlcc_density2, PSPIO_F90_XC_SET_NLCC_DENSITY2)
     (void ** xc, void ** mesh, double * cd, double * cdp)
{
  return pspio_xc_set_nlcc_density(((pspio_xc_t *)(*xc)), ((pspio_mesh_t *)(*mesh)), cd, cdp, NULL);
}

int FC_FUNC_(pspio_f90_xc_set_nlcc_density3, PSPIO_F90_XC_SET_NLCC_DENSITY3)
     (void ** xc, void ** mesh, double * cd, double * cdp, double * cdpp)
{
  return pspio_xc_set_nlcc_density(((pspio_xc_t *)(*xc)), ((pspio_mesh_t *)(*mesh)), cd, cdp, cdpp);
}

// xc_get_correlation
void FC_FUNC_(pspio_f90_xc_get_correlation, PSPIO_F90_XC_GET_CORRELATION)
     (void **xc, int * correlation)
{
  *correlation = ((pspio_xc_t *)(xc))->correlation;
}

// xc_get_exchange
void FC_FUNC_(pspio_f90_xc_get_exchange, PSPIO_F90_XC_GET_EXCHANGE)
     (void **xc, int * exchange)
{
  *exchange = ((pspio_xc_t *)(xc))->exchange;
}

// xc_has_nlcc
void FC_FUNC_(pspio_f90_xc_has_nlcc_int, PSPIO_F90_XC_HAS_NLCC_INT)
     (void ** xc, int * has_nlcc)
{
  *has_nlcc = pspio_xc_has_nlcc(((pspio_xc_t *)(*xc)));
}

// xc_nlcc_densiyt_eval_s
void FC_FUNC_(pspio_f90_xc_nlcc_density_eval_s, PSPIO_F90_XC_NLCC_DENSITY_EVAL_S)
     (void ** xc, double *r, double *nlcc_dens)
{
  pspio_xc_nlcc_density_eval(((pspio_xc_t *)(*xc)), 1, r, nlcc_dens);
}

// xc_nlcc_densiyt_eval_v
void FC_FUNC_(pspio_f90_xc_nlcc_density_eval_v, PSPIO_F90_XC_NLCC_EVAL_V)
     (void ** xc, int *np, double *r, double *nlcc_dens)
{
  pspio_xc_nlcc_density_eval(((pspio_xc_t *)(*xc)), *np, r, nlcc_dens);
}


/**********************************************************************
 * pspio_qn                                                           *
 **********************************************************************/

// qn_alloc
int FC_FUNC_(pspio_f90_qn_alloc, PSPIO_F90_QN_ALLOC)
     (void ** qn)
{
  pspio_qn_t *qn_p = NULL;
  int ierr;

  ierr = (int) pspio_qn_alloc(&qn_p);
  *qn = (void *) qn_p;

  return ierr;
}

// qn_init
int FC_FUNC_(pspio_f90_qn_init, PSPIO_F90_QN_SET)
     (void ** qn, int *n, int *l, double *j)
{
  return pspio_qn_init(((pspio_qn_t *)(*qn)), *n, *l, *j);
}

// qn_free
void FC_FUNC_(pspio_f90_qn_free, PSPIO_F90_QN_FREE)
     (void ** qn)
{
  pspio_qn_t *qn_p;

  qn_p = ((pspio_qn_t *)(*qn));

  pspio_qn_free(&qn_p);
}

// qn_get_n
void FC_FUNC_(pspio_f90_qn_get_n, PSPIO_F90_QN_GET_N)
     (void ** qn, int *n)
{
  *n = pspio_qn_get_n(((pspio_qn_t *)(*qn)));
}

// qn_get_l
void FC_FUNC_(pspio_f90_qn_get_l, PSPIO_F90_QN_GET_L)
     (void ** qn, int *l)
{
  *l = pspio_qn_get_l(((pspio_qn_t *)(*qn)));
}

// qn_get_j
void FC_FUNC_(pspio_f90_qn_get_j, PSPIO_F90_QN_GET_J)
     (void ** qn, double *j)
{
  *j = pspio_qn_get_j(((pspio_qn_t *)(*qn)));
}



/**********************************************************************
 * pspio_error                                                        *
 **********************************************************************/

void FC_FUNC_(pspio_f90_error_fetchall, PSPIO_F90_ERROR_FETCHALL)
  (STR_F_TYPE err_msg STR_ARG1)
{
  char *tmp_msg;

  pspio_error_fetchall(&tmp_msg);
  TO_F_STR1(tmp_msg, err_msg);
  free(tmp_msg);
}


void FC_FUNC_(pspio_f90_error_flush, PSPIO_F90_ERROR_FLUSH)
  (void)
{
  pspio_error_flush(stdout);
}


void FC_FUNC_(pspio_f90_error_free, PSPIO_F90_ERROR_FREE)
  (void)
{
  pspio_error_free();
}


int FC_FUNC_(pspio_f90_error_get_last, PSPIO_F90_ERROR_GET_LAST)
  (const STR_F_TYPE routine STR_ARG1)
{
  char *tmp_str;
  int ierr;

  TO_C_STR1(routine, tmp_str);
  if ( strlen(tmp_str) == 0 ) {
    ierr = pspio_error_get_last(NULL);
  } else {
    ierr = pspio_error_get_last(tmp_str);
  }
  free(tmp_str);

  return ierr;
}


void FC_FUNC_(pspio_f90_error_string, PSPIO_F90_ERROR_STRING)
  (int *error_id, STR_F_TYPE err_str STR_ARG1)
{
  char *tmp_str;

  tmp_str = (char *) malloc (PSPIO_STRLEN_ERROR + 1);
  strncpy(tmp_str, pspio_error_string(*error_id), PSPIO_STRLEN_ERROR);
  TO_F_STR1(tmp_str, err_str);
  free(tmp_str);
}


/**********************************************************************
 * pspio_info                                                         *
 **********************************************************************/

void FC_FUNC_(pspio_f90_version, PSPIO_F90_VERSION)
 (int *major, int *minor, int *micro)
{
  pspio_version(major, minor, micro);
}
