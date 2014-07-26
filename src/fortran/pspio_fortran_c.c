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
 */

#include <stdlib.h>
#include <string.h>

#include "pspio_pspdata.h"
#include "pspio_info.h"
#include "pspio_error.h"

#include "config.h"
#include "string_f.h"


/**********************************************************************
 * pspio_pspdata                                                      *
 **********************************************************************/

//Global routines
int FC_FUNC_(pspio_f90_pspdata_init, PSPIO_F90_PSPDATA_INIT)
     (void **pspdata)
{
  pspio_pspdata_t *pspdata_p = NULL;
  int ierr;

  ierr = (int) pspio_pspdata_init(&pspdata_p);
  *pspdata = (void *) pspdata_p;

  return ierr;
}

int FC_FUNC_(pspio_f90_pspdata_read, PSPIO_F90_PSPDATA_READ)
     (void **pspdata, const int *format, STR_F_TYPE filename STR_ARG1)
{
  char *filename_c;
  int ierr;

  TO_C_STR1(filename, filename_c);
  ierr = (int) pspio_pspdata_read( (pspio_pspdata_t **)(pspdata), (int *) format, filename_c);
  free(filename_c);

  return ierr;
}

int FC_FUNC_(pspio_f90_pspdata_write, PSPIO_F90_PSPDATA_WRITE)
     (void ** pspdata, int *format, STR_F_TYPE filename STR_ARG1)
{
  char *filename_c;
  int ierr;

  TO_C_STR1(filename, filename_c);
  ierr = (int) pspio_pspdata_write( (pspio_pspdata_t *)(*pspdata), (int) (*format), filename_c);
  free(filename_c);

  return ierr;
}

void FC_FUNC_(pspio_f90_pspdata_free, PSPIO_F90_PSPDATA_FREE)
     (void ** pspdata)
{
  pspio_pspdata_free( (pspio_pspdata_t **)(pspdata));
}

// symbol
void FC_FUNC_(pspio_f90_pspdata_set_symbol, PSPIO_F90_PSPDATA_SET_SYMBOL)
     (void ** pspdata, STR_F_TYPE symbol STR_ARG1)
{
  char *symbol_c;

  TO_C_STR1(symbol, symbol_c);
  strcpy(((pspio_pspdata_t *)(*pspdata))->symbol, symbol_c);
  free(symbol_c);
}

void FC_FUNC_(pspio_f90_pspdata_get_symbol, PSPIO_F90_PSPDATA_GET_SYMBOL)
     (void ** pspdata, STR_F_TYPE symbol STR_ARG1)
{
  TO_F_STR1( ((pspio_pspdata_t *)(*pspdata))->symbol, symbol);
}

// z
void FC_FUNC_(pspio_f90_pspdata_set_z, PSPIO_F90_PSPDATA_SET_Z)
     (void ** pspdata, double *z)
{
  ((pspio_pspdata_t *)(*pspdata))->z = *z;
}

void FC_FUNC_(pspio_f90_pspdata_get_z, PSPIO_F90_PSPDATA_GET_Z)
     (void ** pspdata, double * z)
{
  *z = ((pspio_pspdata_t *)(*pspdata))->z;
}

// zvalence
void FC_FUNC_(pspio_f90_pspdata_set_zvalence, PSPIO_F90_PSPDATA_SET_ZVALENCE)
     (void ** pspdata, double *zvalence)
{
  ((pspio_pspdata_t *)(*pspdata))->zvalence = *zvalence;
}

void FC_FUNC_(pspio_f90_pspdata_get_zvalence, PSPIO_F90_PSPDATA_GET_ZVALENCE)
     (void ** pspdata, double * zvalence)
{
  *zvalence = ((pspio_pspdata_t *)(*pspdata))->zvalence;
}

// l_max
void FC_FUNC_(pspio_f90_pspdata_set_l_max, PSPIO_F90_PSPDATA_SET_L_MAX)
     (void ** pspdata, int *l_max)
{
  ((pspio_pspdata_t *)(*pspdata))->l_max = *l_max;
}

void FC_FUNC_(pspio_f90_pspdata_get_l_max, PSPIO_F90_PSPDATA_GET_L_MAX)
     (void ** pspdata, int * l_max)
{
  *l_max = ((pspio_pspdata_t *)(*pspdata))->l_max;
}

// wave_eq
void FC_FUNC_(pspio_f90_pspdata_set_wave_eq, PSPIO_F90_PSPDATA_SET_WAVE_EQ)
     (void ** pspdata, int *wave_eq)
{
  ((pspio_pspdata_t *)(*pspdata))->wave_eq = *wave_eq;
}

int FC_FUNC_(pspio_f90_pspdata_get_wave_eq, PSPIO_F90_PSPDATA_GET_WAVE_EQ)
     (void ** pspdata)
{
  return ((pspio_pspdata_t *)(*pspdata))->wave_eq;
}

// mesh
void FC_FUNC_(pspio_f90_pspdata_set_mesh, PSPIO_F90_PSPDATA_SET_MESH)
     (void ** pspdata, void ** mesh)
{
  ((pspio_pspdata_t *)(*pspdata))->mesh = ((pspio_mesh_t *) (*mesh));
}

void FC_FUNC_(pspio_f90_pspdata_get_mesh, PSPIO_F90_PSPDATA_GET_MESH)
     (void ** pspdata, void ** mesh)
{
  *mesh = (void *) ((pspio_pspdata_t *)(*pspdata))->mesh;
}

// n_states
void FC_FUNC_(pspio_f90_pspdata_set_n_states, PSPIO_F90_PSPDATA_SET_N_STATES)
     (void ** pspdata, int *n_states)
{
  ((pspio_pspdata_t *)(*pspdata))->n_states = *n_states;
}

void FC_FUNC_(pspio_f90_pspdata_get_n_states, PSPIO_F90_PSPDATA_GET_N_STATES)
     (void ** pspdata, int * n_states)
{
  *n_states = ((pspio_pspdata_t *)(*pspdata))->n_states;
}

// states
void FC_FUNC_(pspio_f90_pspdata_set_states, PSPIO_F90_PSPDATA_SET_STATES)
     (void ** pspdata, void *** states)
{
  ((pspio_pspdata_t *)(*pspdata))->states = ((pspio_state_t **) (*states));
}

void FC_FUNC_(pspio_f90_pspdata_get_states, PSPIO_F90_PSPDATA_GET_STATES)
     (void ** pspdata, void *** states)
{
  *states = (void *) ((pspio_pspdata_t *)(*pspdata))->states;
}

// n_potentials
void FC_FUNC_(pspio_f90_pspdata_set_n_potentials, PSPIO_F90_PSPDATA_SET_N_POTENTIALS)
     (void ** pspdata, int *n_potentials)
{
  ((pspio_pspdata_t *)(*pspdata))->n_potentials = *n_potentials;
}

void FC_FUNC_(pspio_f90_pspdata_get_n_potentials, PSPIO_F90_PSPDATA_GET_N_POTENTIALS)
     (void ** pspdata, int * n_potentials)
{
  *n_potentials = ((pspio_pspdata_t *)(*pspdata))->n_potentials;
}

// potentials
void FC_FUNC_(pspio_f90_pspdata_set_potentials, PSPIO_F90_PSPDATA_SET_POTENTIALS)
     (void ** pspdata, void *** potentials)
{
  ((pspio_pspdata_t *)(*pspdata))->potentials = ((pspio_potential_t **) (*potentials));
}

void FC_FUNC_(pspio_f90_pspdata_get_potentials, PSPIO_F90_PSPDATA_GET_POTENTIALS)
     (void ** pspdata, void *** potentials)
{
  *potentials = (void *) ((pspio_pspdata_t *)(*pspdata))->potentials;
}

// n_kbproj
void FC_FUNC_(pspio_f90_pspdata_set_n_kbproj, PSPIO_F90_PSPDATA_SET_N_KBPROJ)
     (void ** pspdata, int *n_kbproj)
{
  ((pspio_pspdata_t *)(*pspdata))->n_kbproj = *n_kbproj;
}

void FC_FUNC_(pspio_f90_pspdata_get_n_kbproj, PSPIO_F90_PSPDATA_GET_N_KBPROJ)
     (void ** pspdata, int * n_kbproj)
{
  *n_kbproj = ((pspio_pspdata_t *)(*pspdata))->n_kbproj;
}

// kb_projectors
void FC_FUNC_(pspio_f90_pspdata_set_kb_projectors, PSPIO_F90_PSPDATA_SET_KB_PROJECTORS)
     (void ** pspdata, void *** kb_projectors)
{
  ((pspio_pspdata_t *)(*pspdata))->kb_projectors = ((pspio_projector_t **) (*kb_projectors));
}

void FC_FUNC_(pspio_f90_pspdata_get_kb_projectors, PSPIO_F90_PSPDATA_GET_KB_PROJECTORS)
     (void ** pspdata, void *** kb_projectors)
{
  *kb_projectors = (void *) ((pspio_pspdata_t *)(*pspdata))->kb_projectors;
}

// l_local
void FC_FUNC_(pspio_f90_pspdata_set_l_local, PSPIO_F90_PSPDATA_SET_L_LOCAL)
     (void ** pspdata, int * l_local)
{
  ((pspio_pspdata_t *)(*pspdata))->l_local = *l_local;
}

void FC_FUNC_(pspio_f90_pspdata_get_l_local, PSPIO_F90_PSPDATA_GET_L_LOCAL)
     (void ** pspdata, int * l_local)
{
  *l_local = ((pspio_pspdata_t *)(*pspdata))->l_local;
}

// kb_l_max
void FC_FUNC_(pspio_f90_pspdata_set_kb_l_max, PSPIO_F90_PSPDATA_SET_KB_L_MAX)
     (void ** pspdata, int * kb_l_max)
{
  ((pspio_pspdata_t *)(*pspdata))->kb_l_max = *kb_l_max;
}

void FC_FUNC_(pspio_f90_pspdata_get_kb_l_max, PSPIO_F90_PSPDATA_GET_KB_L_MAX)
     (void ** pspdata, int * kb_l_max)
{
  *kb_l_max = ((pspio_pspdata_t *)(*pspdata))->kb_l_max;
}

// vlocal
void FC_FUNC_(pspio_f90_pspdata_set_vlocal, PSPIO_F90_PSPDATA_SET_VLOCAL)
     (void ** pspdata, void ** vlocal)
{
  ((pspio_pspdata_t *)(*pspdata))->vlocal = ((pspio_potential_t *)(*vlocal));
}

void FC_FUNC_(pspio_f90_pspdata_get_vlocal, PSPIO_F90_PSPDATA_GET_VLOCAL)
     (void ** pspdata, void ** vlocal)
{
  *vlocal = (void *) ((pspio_pspdata_t *)(*pspdata))->vlocal;
}

// xc
void FC_FUNC_(pspio_f90_pspdata_set_xc, PSPIO_F90_PSPDATA_SET_XC)
     (void ** pspdata, void ** xc)
{
  ((pspio_pspdata_t *)(*pspdata))->xc = ((pspio_xc_t *)(*xc));
}

void FC_FUNC_(pspio_f90_pspdata_get_xc, PSPIO_F90_PSPDATA_GET_XC)
     (void ** pspdata, void ** xc)
{
  *xc = (void *) ((pspio_pspdata_t *)(*pspdata))->xc;
}

// rho_valence
int FC_FUNC_(pspio_f90_pspdata_set_rho_valence, PSPIO_F90_PSPDATA_SET_RHO_VALENCE)
     (void ** pspdata, double *rho)
{
  pspio_meshfunc_t *rho_valence;

  SUCCEED_OR_RETURN(pspio_meshfunc_alloc(&rho_valence, PSPIO_INTERP_GSL_CSPLINE, ((pspio_pspdata_t *)(*pspdata))->mesh->np));
  SUCCEED_OR_RETURN(pspio_meshfunc_set(&rho_valence, ((pspio_pspdata_t *)(*pspdata))->mesh, rho, NULL, NULL));

  return PSPIO_SUCCESS;
}

void FC_FUNC_(pspio_f90_pspdata_rho_valence_eval_s, PSPIO_F90_PSPDATA_RHO_VALENCE_EVAL_S)
     (void ** pspdata, double *r, double *rho)
{
  pspio_meshfunc_eval(((pspio_pspdata_t *)(*pspdata))->rho_valence, 1, r, rho);
}

void FC_FUNC_(pspio_f90_pspdata_rho_valence_eval_v, PSPIO_F90_PSPDATA_RHO_VALENCE_EVAL_V)
     (void ** pspdata, int *np, double *r, double *rho)
{
  pspio_meshfunc_eval(((pspio_pspdata_t *)(*pspdata))->rho_valence, *np, r, rho);
}

/**********************************************************************
 * pspio_mesh                                                         *
 **********************************************************************/

//
void FC_FUNC_(pspio_f90_mesh_get_np, PSPIO_F90_MESH_GET_NP)
     (void ** mesh, int * np)
{

  *np = pspio_mesh_get_np( ((pspio_mesh_t *)(*mesh)));

}

//
void FC_FUNC_(pspio_f90_mesh_get_info, PSPIO_F90_MESH_GET_INFO)
  (void **mesh, int *type, int *np, double *a, double *b){

  *type = ((pspio_mesh_t *)(*mesh))->type;
  *np = ((pspio_mesh_t *)(*mesh))->np;
  *a = ((pspio_mesh_t *)(*mesh))->a;
  *b = ((pspio_mesh_t *)(*mesh))->b;
}

//
void FC_FUNC_(pspio_f90_mesh_get_r, PSPIO_F90_MESH_GET_R)
     (void ** mesh, double *r)
{

  pspio_mesh_get_r( ((pspio_mesh_t *)(*mesh)), r);

}

//
void FC_FUNC_(pspio_f90_mesh_get_rab, PSPIO_F90_MESH_GET_RAB)
     (void ** mesh, double *rab)
{

  pspio_mesh_get_rab( ((pspio_mesh_t *)(*mesh)), rab);

}


/**********************************************************************
 * pspio_state                                                        *
 **********************************************************************/

//
void FC_FUNC_(pspio_f90_state_wf_eval_s, PSPIO_F90_STATE_WF_EVAL_S)
     (void ** state, double *r, double *wf)
{

  pspio_state_wf_eval(((pspio_state_t *)(*state)), 1, r, wf);

}

//
void FC_FUNC_(pspio_f90_state_wf_eval_v, PSPIO_F90_STATE_WF_EVAL_V)
     (void ** state, int *np, double *r, double *wf)
{

  pspio_state_wf_eval(((pspio_state_t *)(*state)), *np, r, wf);

}

//
void FC_FUNC_(pspio_f90_state_get_qn, PSPIO_F90_STATE_GET_QN)
     (void ** state, int *n, int *l, double *j)
{
  *n = pspio_state_get_n(((pspio_state_t *)(*state)));
  *l = pspio_state_get_l(((pspio_state_t *)(*state)));
  *j = pspio_state_get_j(((pspio_state_t *)(*state)));
}

//
void FC_FUNC_(pspio_f90_state_get_occ, PSPIO_F90_STATE_GET_OCC)
     (void ** state, int * occ)
{

  *occ = pspio_state_get_occ((pspio_state_t *)(*state));

}


/**********************************************************************
 * pspio_potential                                                    *
 **********************************************************************/

//
void FC_FUNC_(pspio_f90_potential_eval_s, PSPIO_F90_POTENTIAL_EVAL_S)
     (void ** potential, double *r, double *v)
{

  pspio_potential_eval(((pspio_potential_t *)(*potential)), 1, r, v);

}

void FC_FUNC_(pspio_f90_potential_eval_v, PSPIO_F90_POTENTIAL_EVAL_V)
     (void ** potential, int *np, double *r, double *v)
{

  pspio_potential_eval(((pspio_potential_t *)(*potential)), *np, r, v);

}


/**********************************************************************
 * pspio_projector                                                    *
 **********************************************************************/

//
void FC_FUNC_(pspio_f90_projector_eval_s, PSPIO_F90_PROJECTOR_EVAL_S)
     (void ** projector, double *r, double *p)
{

  pspio_projector_eval(((pspio_projector_t *)(*projector)), 1, r, p);

}

void FC_FUNC_(pspio_f90_projector_eval_v, PSPIO_F90_PROJECTOR_EVAL_V)
     (void ** projector, int *np, double *r, double *p)
{

  pspio_projector_eval(((pspio_projector_t *)(*projector)), *np, r, p);

}

//
void FC_FUNC_(pspio_f90_projector_get_energy, PSPIO_F90_PROJECTOR_GET_ENERGY)
     (void ** projector, double * energy)
{

  *energy = pspio_projector_get_energy((pspio_projector_t *)(*projector));

}

//
void FC_FUNC_(pspio_f90_projector_get_l, PSPIO_F90_PROJECTOR_GET_L)
     (void ** projector, int * l)
{

  *l = pspio_projector_get_l((pspio_projector_t *)(*projector));

}

//
void FC_FUNC_(pspio_f90_projector_get_j, PSPIO_F90_PROJECTOR_GET_J)
     (void ** projector, double * j)
{

  *j = pspio_projector_get_j((pspio_projector_t *)(*projector));

}


/**********************************************************************
 * pspio_xc                                                           *
 **********************************************************************/

//
void FC_FUNC_(pspio_f90_xc_get_correlation, PSPIO_F90_XC_GET_CORRELATION)
     (void **xc, int * correlation)
{

  *correlation = ((pspio_xc_t *)(xc))->correlation;

}

//
void FC_FUNC_(pspio_f90_xc_get_exchange, PSPIO_F90_XC_GET_EXCHANGE)
     (void **xc, int * exchange)
{

  *exchange = ((pspio_xc_t *)(xc))->exchange;

}

//
void FC_FUNC_(pspio_f90_xc_has_nlcc_int, PSPIO_F90_XC_HAS_NLCC_INT)
     (void ** xc, int * has_nlcc)
{

  *has_nlcc = pspio_xc_has_nlcc(((pspio_xc_t *)(*xc)));

}

//
void FC_FUNC_(pspio_f90_xc_nlcc_density_eval_s, PSPIO_F90_XC_NLCC_DENSITY_EVAL_S)
     (void ** xc, double *r, double *nlcc_dens)
{

  pspio_xc_nlcc_density_eval(((pspio_xc_t *)(*xc)), 1, r, nlcc_dens);

}

void FC_FUNC_(pspio_f90_xc_nlcc_density_eval_v, PSPIO_F90_XC_NLCC_EVAL_V)
     (void ** xc, int *np, double *r, double *nlcc_dens)
{

  pspio_xc_nlcc_density_eval(((pspio_xc_t *)(*xc)), *np, r, nlcc_dens);

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


/**********************************************************************
 * pspio_info                                                         *
 **********************************************************************/

void FC_FUNC_(pspio_f90_version, PSPIO_F90_VERSION)
 (int *major, int *minor, int *micro)
{
  pspio_version(major, minor, micro);
}
