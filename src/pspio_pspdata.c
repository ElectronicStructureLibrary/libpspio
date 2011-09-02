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

#include <stdio.h>

#include "pspio_common.h"
#include "pspio_error.h"
#include "pspio_meshfunc.h"
#include "pspio_pspdata.h"
#include "util.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

int pspio_pspdata_init(pspio_pspdata_t *pspdata, const char *file_name,
      const int file_format){
  FILE * fp;
  int ierr;
  
  // open file
  fp = fopen(file_name, "r");
  if(fp == NULL) {
    HANDLE_ERROR(PSPIO_ENOFILE);
  }
  
  //read from file
  switch(file_format) {
  case UNKNOWN:
    break;
  case ABINIT_4:
    HANDLE_FUNC_ERROR(read_abinit4(fp,pspdata));
    break;
  case ABINIT_5:
    HANDLE_FUNC_ERROR(read_abinit5(fp,pspdata));
    break;
  case ABINIT_6:
    HANDLE_FUNC_ERROR(read_abinit6(fp,pspdata));
    break;
  case ABINIT_HGH:
    HANDLE_FUNC_ERROR(read_abinit_hgh(fp,pspdata));
    break;
  case ABINIT_GTH:
    HANDLE_FUNC_ERROR(read_abinit_gth(fp,pspdata));
    break;
  case ATOM:
    break;
  case FHI98PP:
    break;
  case SIESTA:
    break;
  case UPF:
    HANDLE_FUNC_ERROR(pspio_upf_init(fp,pspdata));
    break;
  default:
    return PSPIO_EFILE_FORMAT;
  }
  
  // close file and check for ierr being non 0
  ierr = fclose(fp);
  if(ierr != 0){
    HANDLE_ERROR(PSPIO_EIO);
  }

  return PSPIO_SUCCESS;
}


int pspio_pspdata_free(pspio_pspdata_t *pspdata){
  int i;

  if (pspdata != NULL) {
    if (pspdata->symbol != NULL) free(pspdata->symbol);
    HANDLE_FUNC_ERROR (pspio_mesh_free(pspdata->mesh));

    if (pspdata->states != NULL) {
      for (i=0; i<pspdata->n_states; i++) {
	HANDLE_FUNC_ERROR (pspio_state_free(pspdata->states[i]));
      }
      free(pspdata->states);
    }

    if (pspdata->potentials != NULL) {
      for (i=0; i<pspdata->n_potentials; i++) {
	HANDLE_FUNC_ERROR (pspio_potential_free(pspdata->potentials[i]));
      }
      free(pspdata->potentials);
    }

    if (pspdata->kb_projectors != NULL) {
      for (i=0; i<pspdata->n_kbproj; i++) {
	HANDLE_FUNC_ERROR (pspio_projector_free(pspdata->kb_projectors[i]));
      }
      free(pspdata->kb_projectors);
    }

    if (pspdata->vlocal != NULL) {
      HANDLE_FUNC_ERROR (pspio_potential_free(pspdata->vlocal));
    }

    if (pspdata->nlcc != NULL) {
      HANDLE_FUNC_ERROR (pspio_nlcc_free(pspdata->nlcc));
    }

  }

  return PSPIO_SUCCESS;
}


/**********************************************************************
 * Atomic routines                                                    *
 **********************************************************************/

int pspio_pspdata_potential_get(const pspio_pspdata_t *data, const int l,
      const double j, const double r, double *value) {
  ASSERT(data != NULL, PSPIO_ERROR);
  ASSERT(value != NULL, PSPIO_ERROR);
  ASSERT((l >=0) &&
	 ((j == 0) || (abs(j - (double)l) - 0.5 < 10e-9)), PSPIO_ERROR);

  HANDLE_FUNC_ERROR(pspio_meshfunc_eval(data->potentials[LJ_TO_I(l,j)]->v,r,value));

  return PSPIO_SUCCESS;
}

int pspio_pspdata_kbprojector_get(const pspio_pspdata_t *data, const int l,
      const double j, const double r, double *value) {
  ASSERT(data != NULL, PSPIO_ERROR);
  ASSERT(value != NULL, PSPIO_ERROR);
  ASSERT( (l >=0) &&
	 ((j == 0) || (abs(j - (double)l) - 0.5 < 10e-9)), PSPIO_ERROR);

  HANDLE_FUNC_ERROR(pspio_meshfunc_eval(data->kb_projectors[LJ_TO_I(l,j)]->proj,r,value));

  return PSPIO_SUCCESS;
}

int pspio_pspdata_kbenergy_get(const pspio_pspdata_t *data, const int l,
      const double j, double *value) {
  ASSERT(data != NULL, PSPIO_ERROR);
  ASSERT(value != NULL, PSPIO_ERROR);
  ASSERT( (l >=0) &&
	 ((j == 0) || (abs(j - (double)l) - 0.5 < 10e-9)), PSPIO_ERROR);

  *value = data->kb_projectors[LJ_TO_I(l,j)]->energy;

  return PSPIO_SUCCESS;
}
