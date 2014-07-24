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

#include "config.h"
#include "string_f.h"


/**********************************************************************
 * pspio_pspdata                                                      *
 **********************************************************************/

CC_FORTRAN_INT FC_FUNC_(pspio_f90_pspdata_init, PSPIO_F90_PSPDATA_INIT)
     (void **pspdata)
{
  pspio_pspdata_t *pspdata_p = NULL;
  int ierr;

  ierr = (CC_FORTRAN_INT) pspio_pspdata_init(&pspdata_p);
  *pspdata = (void *) pspdata_p;

  return ierr;
}

CC_FORTRAN_INT FC_FUNC_(pspio_f90_pspdata_read, PSPIO_F90_PSPDATA_READ)
     (void **pspdata, const CC_FORTRAN_INT *format, STR_F_TYPE filename STR_ARG1)
{
  char *filename_c;
  int ierr;

  TO_C_STR1(filename, filename_c);
  ierr = (CC_FORTRAN_INT) pspio_pspdata_read( (pspio_pspdata_t **)(pspdata), (int *) format, filename_c);
  free(filename_c);

  return ierr;
}

CC_FORTRAN_INT FC_FUNC_(pspio_f90_pspdata_write, PSPIO_F90_PSPDATA_WRITE)
     (void ** pspdata, CC_FORTRAN_INT *format, STR_F_TYPE filename STR_ARG1)
{
  char *filename_c;
  int ierr;

  TO_C_STR1(filename, filename_c);
  ierr = (CC_FORTRAN_INT) pspio_pspdata_write( (pspio_pspdata_t *)(*pspdata), (int) (*format), filename_c);
  free(filename_c);

  return ierr;
}

void FC_FUNC_(pspio_f90_pspdata_free, PSPIO_F90_PSPDATA_FREE)
     (void ** pspdata)
{

  pspio_pspdata_free( (pspio_pspdata_t **)(pspdata));
     
}

void FC_FUNC_(pspio_f90_pspdata_get_symbol, PSPIO_F90_PSPDATA_GET_SYMBOL)
     (void ** pspdata, STR_F_TYPE symbol STR_ARG1)
{

  TO_F_STR1( ((pspio_pspdata_t *)(*pspdata))->symbol, symbol);

}

void FC_FUNC_(pspio_f90_pspdata_get_z, PSPIO_F90_PSPDATA_GET_Z)
     (void ** pspdata, double *z)
{

  *z = ((pspio_pspdata_t *)(*pspdata))->z;

}

void FC_FUNC_(pspio_f90_pspdata_get_zvalence, PSPIO_F90_PSPDATA_GET_ZVALENCE)
     (void ** pspdata, double *zvalence)
{

  *zvalence = ((pspio_pspdata_t *)(*pspdata))->zvalence;

}

void FC_FUNC_(pspio_f90_pspdata_get_l_max, PSPIO_F90_PSPDATA_GET_L_MAX)
     (void ** pspdata, CC_FORTRAN_INT *l_max)
{

  *l_max = ((pspio_pspdata_t *)(*pspdata))->l_max;

}

void FC_FUNC_(pspio_f90_pspdata_get_wave_eq, PSPIO_F90_PSPDATA_GET_WAVE_EQ)
     (void ** pspdata, CC_FORTRAN_INT *wave_eq)
{

  *wave_eq = ((pspio_pspdata_t *)(*pspdata))->wave_eq;

}

void FC_FUNC_(pspio_f90_pspdata_get_mesh, PSPIO_F90_PSPDATA_GET_MESH)
     (void ** pspdata, void ** mesh)
{

  *mesh = (void *) ((pspio_pspdata_t *)(*pspdata))->mesh;

}

void FC_FUNC_(pspio_f90_pspdata_get_n_states, PSPIO_F90_PSPDATA_GET_N_STATES)
     (void ** pspdata, CC_FORTRAN_INT *n_states)
{

  *n_states = ((pspio_pspdata_t *)(*pspdata))->n_states;

}

void FC_FUNC_(pspio_f90_pspdata_get_state, PSPIO_F90_PSPDATA_GET_STATE)
     (void ** pspdata, CC_FORTRAN_INT *i, void ** state)
{

  *state = (void *) ((pspio_pspdata_t *)(*pspdata))->states[*i-1];

}

void FC_FUNC_(pspio_f90_pspdata_get_n_kbproj, PSPIO_F90_PSPDATA_GET_N_KBPROJ)
     (void ** pspdata, CC_FORTRAN_INT *n_kbproj)
{

  *n_kbproj = ((pspio_pspdata_t *)(*pspdata))->n_kbproj;

}

void FC_FUNC_(pspio_f90_pspdata_get_kb_projector, PSPIO_F90_PSPDATA_GET_KB_PROJECTOR)
     (void ** pspdata, CC_FORTRAN_INT *i, void ** kb_projector)
{

  *kb_projector = (void *) ((pspio_pspdata_t *)(*pspdata))->kb_projectors[*i-1];

}

void FC_FUNC_(pspio_f90_pspdata_get_l_local, PSPIO_F90_PSPDATA_GET_L_LOCAL)
     (void ** pspdata, CC_FORTRAN_INT *l_local)
{

  *l_local = ((pspio_pspdata_t *)(*pspdata))->l_local;

}

void FC_FUNC_(pspio_f90_pspdata_get_kb_l_max, PSPIO_F90_PSPDATA_GET_KB_L_MAX)
     (void ** pspdata, CC_FORTRAN_INT *kb_l_max)
{

  *kb_l_max = ((pspio_pspdata_t *)(*pspdata))->kb_l_max;

}

void FC_FUNC_(pspio_f90_pspdata_get_vlocal, PSPIO_F90_PSPDATA_GET_VLOCAL)
     (void ** pspdata, void ** vlocal)
{

  *vlocal = (void *) ((pspio_pspdata_t *)(*pspdata))->vlocal;

}

void FC_FUNC_(pspio_f90_pspdata_get_xc, PSPIO_F90_PSPDATA_GET_XC)
     (void ** pspdata, void ** xc)
{

  *xc = (void *) ((pspio_pspdata_t *)(*pspdata))->xc;

}


/**********************************************************************
 * pspio_mesh                                                         *
 **********************************************************************/

void FC_FUNC_(pspio_f90_mesh_get_np, PSPIO_F90_MESH_GET_NP)
     (void ** mesh, CC_FORTRAN_INT *np)
{

  pspio_mesh_get_np( ((pspio_mesh_t *)(*mesh)), np);

}

void FC_FUNC_(pspio_f90_mesh_get_info, PSPIO_F90_MESH_GET_INFO)
  (void **mesh, int *type, int *np, double *a, double *b){

  *type = ((pspio_mesh_t *)(*mesh))->type;
  *np = ((pspio_mesh_t *)(*mesh))->np;
  *a = ((pspio_mesh_t *)(*mesh))->a;
  *b = ((pspio_mesh_t *)(*mesh))->b;
}


void FC_FUNC_(pspio_f90_mesh_get_r, PSPIO_F90_MESH_GET_R)
     (void ** mesh, double *r)
{

  pspio_mesh_get_r( ((pspio_mesh_t *)(*mesh)), r);

}

void FC_FUNC_(pspio_f90_mesh_get_rab, PSPIO_F90_MESH_GET_RAB)
     (void ** mesh, double *rab)
{

  pspio_mesh_get_rab( ((pspio_mesh_t *)(*mesh)), rab);

}


/**********************************************************************
 * pspio_state                                                        *
 **********************************************************************/

void FC_FUNC_(pspio_f90_state_wf_eval_s, PSPIO_F90_STATE_WF_EVAL_S)
     (void ** state, double *r, double *wf)
{

  pspio_state_wf_eval(((pspio_state_t *)(*state)), 1, r, wf);

}

void FC_FUNC_(pspio_f90_state_wf_eval_v, PSPIO_F90_STATE_WF_EVAL_V)
     (void ** state, int *np, double *r, double *wf)
{

  pspio_state_wf_eval(((pspio_state_t *)(*state)), *np, r, wf);

}

void FC_FUNC_(pspio_f90_state_get_qn, PSPIO_F90_STATE_GET_QN)
     (void ** state, CC_FORTRAN_INT *n, CC_FORTRAN_INT *l, double *j)
{
  pspio_state_get_n(((pspio_state_t *)(*state)), n);
  pspio_state_get_l(((pspio_state_t *)(*state)), l);
  pspio_state_get_j(((pspio_state_t *)(*state)), j);
}

void FC_FUNC_(pspio_f90_state_get_occ, PSPIO_F90_STATE_GET_OCC)
     (void ** state, double *occ)
{

  pspio_state_get_occ(((pspio_state_t *)(*state)), occ);

}


/**********************************************************************
 * pspio_potential                                                    *
 **********************************************************************/

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

void FC_FUNC_(pspio_f90_projector_get_energy, PSPIO_F90_PROJECTOR_GET_ENERGY)
 (void ** projector, double *e)
{

  pspio_projector_get_energy(((pspio_projector_t *)(*projector)), e);

}

void FC_FUNC_(pspio_f90_projector_get_l, PSPIO_F90_PROJECTOR_GET_L)
 (void ** projector, int *l)
{

  pspio_projector_get_l(((pspio_projector_t *)(*projector)), l);

}

void FC_FUNC_(pspio_f90_projector_get_j, PSPIO_F90_PROJECTOR_GET_J)
 (void ** projector, double *j)
{

  pspio_projector_get_j(((pspio_projector_t *)(*projector)), j);

}


/**********************************************************************
 * pspio_xc                                                           *
 **********************************************************************/

void FC_FUNC_(pspio_f90_xc_get_correlation, PSPIO_F90_XC_GET_CORRELATION)
     (void **xc, int *correlation)
{

  *correlation = ((pspio_xc_t *)(xc))->correlation;

}
void FC_FUNC_(pspio_f90_xc_get_exchange, PSPIO_F90_XC_GET_EXCHANGE)
     (void **xc, int *exchange)
{

  *exchange = ((pspio_xc_t *)(xc))->exchange;

}

void FC_FUNC_(pspio_f90_xc_has_nlcc_int, PSPIO_F90_XC_HAS_NLCC_INT)
     (void ** xc, int *has_nlcc)
{

  pspio_xc_has_nlcc(((pspio_xc_t *)(*xc)), has_nlcc);

}

void FC_FUNC_(pspio_f90_xc_nlcc_eval_s, PSPIO_F90_XC_NLCC_EVAL_S)
     (void ** xc, double *r, double *core_dens)
{

  pspio_xc_nlcc_eval(((pspio_xc_t *)(*xc)), 1, r, core_dens);

}

void FC_FUNC_(pspio_f90_xc_nlcc_eval_v, PSPIO_F90_XC_NLCC_EVAL_V)
     (void ** xc, int *np, double *r, double *core_dens)
{

  pspio_xc_nlcc_eval(((pspio_xc_t *)(*xc)), *np, r, core_dens);

}


/**********************************************************************
 * pspio_error                                                        *
 **********************************************************************/

void FC_FUNC_(pspio_f90_error_add, PSPIO_F90_ERROR_ADD)
  (int *error_id, STR_F_TYPE filename, int *lineno STR_ARG1)
{
  char *tmp_name;

  TO_C_STR1(filename, tmp_name);
  pspio_error_add(*error_id, tmp_name, *lineno);
  free(tmp_name);
}


CC_FORTRAN_INT FC_FUNC_(pspio_f90_error_fetchall, PSPIO_F90_ERROR_FETCHALL)
  (STR_F_TYPE err_msg STR_ARG1)
{
  char *tmp_msg;
  int eid;

  eid = pspio_error_fetchall(&tmp_msg);
  TO_F_STR1(tmp_msg, err_msg);
  free(tmp_msg);

  return (CC_FORTRAN_INT) eid;
}


CC_FORTRAN_INT FC_FUNC_(pspio_f90_error_flush, PSPIO_F90_ERROR_FLUSH)
  (void)
{
  return (CC_FORTRAN_INT) pspio_error_flush();
}


CC_FORTRAN_INT FC_FUNC_(pspio_f90_error_free, PSPIO_F90_ERROR_FREE)
  (void)
{
  return (CC_FORTRAN_INT) pspio_error_free();
}


CC_FORTRAN_INT FC_FUNC_(pspio_f90_error_get_last, PSPIO_F90_ERROR_GET_LAST)
  (void)
{
  return (CC_FORTRAN_INT) pspio_error_get_last();
}


CC_FORTRAN_INT FC_FUNC_(pspio_f90_error_len, PSPIO_F90_ERROR_LEN)
  (void)
{
  return (CC_FORTRAN_INT) pspio_error_len();
}


/**********************************************************************
 * pspio_info                                                         *
 **********************************************************************/

CC_FORTRAN_INT FC_FUNC_(pspio_f90_version, PSPIO_F90_VERSION)
 (int *major, int *minor, int *micro)
{
  return (CC_FORTRAN_INT)  pspio_version(major, minor, micro);
}
