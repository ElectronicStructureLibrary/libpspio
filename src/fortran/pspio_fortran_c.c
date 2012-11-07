/*
 Copyright (C) 2012 M. Oliveira

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

/**
 * @file pspio_fortran_c.c
 * @brief Fortran wrappers
 */

#include "pspio_pspdata.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif

#include "string_f.h"

/**********************************************************************
 * pspio_pspdata                                                      *
 **********************************************************************/

CC_FORTRAN_INT FC_FUNC_(pspio_f90_pspdata_init, PSPIO_F90_PSPDATA_READ)
  (void **pspdata)
{
  pspio_pspdata_t *pspdata_p = NULL;
  int ierr;

  ierr = (CC_FORTRAN_INT) pspio_pspdata_init(&pspdata_p);
  *pspdata = (void *) pspdata_p;

  return ierr;
}

CC_FORTRAN_INT FC_FUNC_(pspio_f90_pspdata_read, PSPIO_F90_PSPDATA_READ)
  (void **pspdata, CC_FORTRAN_INT *format, STR_F_TYPE filename STR_ARG1)
{
  char *filename_c;
  int ierr;

  TO_C_STR1(filename, filename_c);
  ierr = (CC_FORTRAN_INT) pspio_pspdata_read( (pspio_pspdata_t **)(pspdata), filename_c, (int) (*format));
  free(filename_c);

  return ierr;
}

CC_FORTRAN_INT FC_FUNC_(pspio_f90_pspdata_write, PSPIO_F90_PSPDATA_WRITE)
  (void ** pspdata, CC_FORTRAN_INT *format, STR_F_TYPE filename STR_ARG1)
{
  char *filename_c;
  int ierr;

  TO_C_STR1(filename, filename_c);
  ierr = (CC_FORTRAN_INT) pspio_pspdata_write( (pspio_pspdata_t *)(*pspdata), filename_c, (int) (*format));
  free(filename_c);

  return ierr;
}

CC_FORTRAN_INT FC_FUNC_(pspio_f90_pspdata_free, PSPIO_F90_PSPDATA_FREE)
  (void ** pspdata)
{

 return (CC_FORTRAN_INT) pspio_pspdata_free( (pspio_pspdata_t **)(pspdata));
     
}

CC_FORTRAN_INT FC_FUNC_(pspio_f90_pspdata_get_symbol, PSPIO_F90_PSPDATA_GET_SYMBOL)
  (void ** pspdata, STR_F_TYPE symbol STR_ARG1)
{

  TO_F_STR1( ((pspio_pspdata_t *)(*pspdata))->symbol, symbol);

  return PSPIO_SUCCESS;
}

CC_FORTRAN_INT FC_FUNC_(pspio_f90_pspdata_get_z, PSPIO_F90_PSPDATA_GET_Z)
  (void ** pspdata, double *z)
{

  *z = ((pspio_pspdata_t *)(*pspdata))->z;

  return PSPIO_SUCCESS;
}

CC_FORTRAN_INT FC_FUNC_(pspio_f90_pspdata_get_zvalence, PSPIO_F90_PSPDATA_GET_ZVALENCE)
  (void ** pspdata, double *zvalence)
{

  *zvalence = ((pspio_pspdata_t *)(*pspdata))->zvalence;

  return PSPIO_SUCCESS;
}

CC_FORTRAN_INT FC_FUNC_(pspio_f90_pspdata_get_l_max, PSPIO_F90_PSPDATA_GET_L_MAX)
  (void ** pspdata, CC_FORTRAN_INT *l_max)
{

  *l_max = ((pspio_pspdata_t *)(*pspdata))->l_max;

  return PSPIO_SUCCESS;
}

CC_FORTRAN_INT FC_FUNC_(pspio_f90_pspdata_get_wave_eq, PSPIO_F90_PSPDATA_GET_WAVE_EQ)
  (void ** pspdata, CC_FORTRAN_INT *wave_eq)
{

  *wave_eq = ((pspio_pspdata_t *)(*pspdata))->wave_eq;

  return PSPIO_SUCCESS;
}

CC_FORTRAN_INT FC_FUNC_(pspio_f90_pspdata_get_mesh, PSPIO_F90_PSPDATA_GET_MESH)
 (void ** pspdata, void ** mesh)
{

  *mesh = (void *) ((pspio_pspdata_t *)(*pspdata))->mesh;

  return PSPIO_SUCCESS;
}

CC_FORTRAN_INT FC_FUNC_(pspio_f90_pspdata_get_n_states, PSPIO_F90_PSPDATA_GET_N_STATES)
  (void ** pspdata, CC_FORTRAN_INT *n_states)
{

  *n_states = ((pspio_pspdata_t *)(*pspdata))->n_states;

  return PSPIO_SUCCESS;
}

CC_FORTRAN_INT FC_FUNC_(pspio_f90_pspdata_get_state, PSPIO_F90_PSPDATA_GET_STATE)
  (void ** pspdata, CC_FORTRAN_INT *i, void ** state)
{

  *state = (void *) ((pspio_pspdata_t *)(*pspdata))->states[*i-1];

  return PSPIO_SUCCESS;
}

CC_FORTRAN_INT FC_FUNC_(pspio_f90_pspdata_get_n_kbproj, PSPIO_F90_PSPDATA_GET_N_KBPROJ)
  (void ** pspdata, CC_FORTRAN_INT *n_kbproj)
{

  *n_kbproj = ((pspio_pspdata_t *)(*pspdata))->n_kbproj;

  return PSPIO_SUCCESS;
}

CC_FORTRAN_INT FC_FUNC_(pspio_f90_pspdata_get_kb_projector, PSPIO_F90_PSPDATA_GET_KB_PROJECTOR)
  (void ** pspdata, CC_FORTRAN_INT *i, void ** kb_projector)
{

  *kb_projector = (void *) ((pspio_pspdata_t *)(*pspdata))->kb_projectors[*i-1];

  return PSPIO_SUCCESS;
}

CC_FORTRAN_INT FC_FUNC_(pspio_f90_pspdata_get_l_local, PSPIO_F90_PSPDATA_GET_L_LOCAL)
  (void ** pspdata, CC_FORTRAN_INT *l_local)
{

  *l_local = ((pspio_pspdata_t *)(*pspdata))->l_local;

  return PSPIO_SUCCESS;
}

CC_FORTRAN_INT FC_FUNC_(pspio_f90_pspdata_get_kb_l_max, PSPIO_F90_PSPDATA_GET_KB_L_MAX)
  (void ** pspdata, CC_FORTRAN_INT *kb_l_max)
{

  *kb_l_max = ((pspio_pspdata_t *)(*pspdata))->kb_l_max;

  return PSPIO_SUCCESS;
}

CC_FORTRAN_INT FC_FUNC_(pspio_f90_pspdata_get_vlocal, PSPIO_F90_PSPDATA_GET_VLOCAL)
 (void ** pspdata, void ** vlocal)
{

  *vlocal = (void *) ((pspio_pspdata_t *)(*pspdata))->vlocal;

  return PSPIO_SUCCESS;
}

CC_FORTRAN_INT FC_FUNC_(pspio_f90_pspdata_get_xc, PSPIO_F90_PSPDATA_GET_XC)
 (void ** pspdata, void ** xc)
{

  *xc = (void *) ((pspio_pspdata_t *)(*pspdata))->xc;

  return PSPIO_SUCCESS;
}


/**********************************************************************
 * pspio_mesh                                                         *
 **********************************************************************/

CC_FORTRAN_INT FC_FUNC_(pspio_f90_mesh_get_np, PSPIO_F90_MESH_GET_NP)
  (void ** mesh, CC_FORTRAN_INT *np)
{

  return (CC_FORTRAN_INT) pspio_mesh_get_np( ((pspio_mesh_t *)(*mesh)), np);

}

CC_FORTRAN_INT FC_FUNC_(pspio_f90_mesh_get_r, PSPIO_F90_MESH_GET_R)
  (void ** mesh, double *r)
{

  return (CC_FORTRAN_INT) pspio_mesh_get_r( ((pspio_mesh_t *)(*mesh)), r);

}


/**********************************************************************
 * pspio_state                                                        *
 **********************************************************************/

CC_FORTRAN_INT FC_FUNC_(pspio_f90_state_wf_eval, PSPIO_F90_STATE_WF_EVAL)
 (void ** state, double *r, double *wf)
{

  return (CC_FORTRAN_INT) pspio_state_wf_eval(((pspio_state_t *)(*state)), *r, wf);

}

CC_FORTRAN_INT FC_FUNC_(pspio_f90_state_get_qn, PSPIO_F90_STATE_GET_QN)
 (void ** state, CC_FORTRAN_INT *n, CC_FORTRAN_INT *l, double *j)
{
  int ierr;

  ierr = pspio_state_get_n(((pspio_state_t *)(*state)), n);
  if (ierr != PSPIO_SUCCESS) return ierr;

  ierr = pspio_state_get_l(((pspio_state_t *)(*state)), l);
  if (ierr != PSPIO_SUCCESS) return ierr;

  ierr = pspio_state_get_j(((pspio_state_t *)(*state)), j);
  if (ierr != PSPIO_SUCCESS) return ierr;

  return PSPIO_SUCCESS;
}

CC_FORTRAN_INT FC_FUNC_(pspio_f90_state_get_occ, PSPIO_F90_STATE_GET_OCC)
 (void ** state, double *occ)
{

  return (CC_FORTRAN_INT) pspio_state_get_occ(((pspio_state_t *)(*state)), occ);

}


/**********************************************************************
 * pspio_potential                                                    *
 **********************************************************************/

CC_FORTRAN_INT FC_FUNC_(pspio_f90_potential_eval, PSPIO_F90_POTENTIAL_EVAL)
 (void ** potential, double *r, double *v)
{

  return (CC_FORTRAN_INT) pspio_potential_eval(((pspio_potential_t *)(*potential)), *r, v);

}


/**********************************************************************
 * pspio_projector                                                    *
 **********************************************************************/

CC_FORTRAN_INT FC_FUNC_(pspio_f90_projector_eval, PSPIO_F90_PROJECTOR_EVAL)
 (void ** projector, double *r, double *p)
{

  return (CC_FORTRAN_INT) pspio_projector_eval(((pspio_projector_t *)(*projector)), *r, p);

}

CC_FORTRAN_INT FC_FUNC_(pspio_f90_projector_get_energy, PSPIO_F90_PROJECTOR_GET_ENERGY)
 (void ** projector, double *e)
{

  return (CC_FORTRAN_INT) pspio_projector_get_energy(((pspio_projector_t *)(*projector)), e);

}

CC_FORTRAN_INT FC_FUNC_(pspio_f90_projector_get_l, PSPIO_F90_PROJECTOR_GET_L)
 (void ** projector, int *l)
{

  return (CC_FORTRAN_INT) pspio_projector_get_l(((pspio_projector_t *)(*projector)), l);

}

CC_FORTRAN_INT FC_FUNC_(pspio_f90_projector_get_j, PSPIO_F90_PROJECTOR_GET_J)
 (void ** projector, double *j)
{

  return (CC_FORTRAN_INT) pspio_projector_get_j(((pspio_projector_t *)(*projector)), j);

}


/**********************************************************************
 * pspio_xc                                                           *
 **********************************************************************/

CC_FORTRAN_INT FC_FUNC_(pspio_f90_xc_has_nlcc_int, PSPIO_F90_XC_HAS_NLCC_INT)
 (void ** xc, int *has_nlcc)
{

  return (CC_FORTRAN_INT) pspio_xc_has_nlcc(((pspio_xc_t *)(*xc)), has_nlcc);

}

CC_FORTRAN_INT FC_FUNC_(pspio_f90_xc_nlcc_eval, PSPIO_F90_XC_NLCC_EVAL)
 (void ** xc, double *r, double *core_dens)
{

  return (CC_FORTRAN_INT) pspio_xc_nlcc_eval(((pspio_xc_t *)(*xc)), *r, core_dens);

}


/**********************************************************************
 * pspio_error                                                        *
 **********************************************************************/

CC_FORTRAN_INT FC_FUNC_(pspio_f90_error_flush, PSPIO_F90_ERROR_FLUSH)
  (void)
{
  return (CC_FORTRAN_INT) pspio_error_flush();
}


/**********************************************************************
 * pspio_info                                                         *
 **********************************************************************/

CC_FORTRAN_INT FC_FUNC_(pspio_f90_version, PSPIO_F90_VERSION)
 (int *major, int *minor, int *micro)
{
  return (CC_FORTRAN_INT)  pspio_version(major, minor, micro);
}


