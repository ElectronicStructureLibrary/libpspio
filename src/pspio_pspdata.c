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

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

int pspio_pspdata_init(pspio_pspdata_t *psp_data, const char *file_name,
      const int file_format){
  FILE * fp;
  int ierr;
  int fileformat;
  
  // open file
  fp = fopen(file_name, "r");
  if(fp == NULL) {
    HANDLE_ERROR(PSPIO_ENOFILE);
  }
  
  fileformat = UNKNOWN;

  //read from file
  switch(fileformat) {
  case UNKNOWN:
    break;
  case ABINIT_4:
    HANDLE_FUNC_ERROR(read_abinit4(fp,psp_data));
    break;
  case ABINIT_5:
    HANDLE_FUNC_ERROR(read_abinit5(fp,psp_data));
    break;
  case ABINIT_6:
    HANDLE_FUNC_ERROR(read_abinit6(fp,psp_data));
    break;
  case ABINIT_HGH:
    HANDLE_FUNC_ERROR(read_abinit_hgh(fp,psp_data));
    break;
  case ABINIT_GTH:
    HANDLE_FUNC_ERROR(read_abinit_gth(fp,psp_data));
    break;
  case ATOM:
    break;
  case FHI98PP:
    break;
  case SIESTA:
    break;
  case UPF:
    HANDLE_FUNC_ERROR(pspio_upf_init(fp,psp_data));
    break;
  default:
    return PSPIO_EFILE_FORMAT;
  }
  
  // close file and check for ierr being non 0
  ierr = close(fp);
  if(ierr != 0){
    HANDLE_ERROR(PSPIO_EIO);
  }

  return PSPIO_SUCCESS;
}


int pspio_pspdata_free(pspio_pspdata_t *psp_data){
  int i;

  if (psp_data != NULL) {
    if (psp_data->symbol != NULL) free(psp_data->symbol);
    HANDLE_FUNC_ERROR (pspio_mesh_free(psp_data->mesh));

    if (psp_data->states != NULL) {
      for (i=0; i<psp_data->n_states; i++) {
	HANDLE_FUNC_ERROR (pspio_state_free(psp_data->states[i]));
      }
      free(psp_data->states);
    }

    if (psp_data->potentials != NULL) {
      for (i=0; i<psp_data->n_potentials; i++) {
	HANDLE_FUNC_ERROR (pspio_potential_free(psp_data->potentials[i]));
      }
      free(psp_data->potentials);
    }

    if (psp_data->kb_projectors != NULL) {
      for (i=0; i<psp_data->n_kbproj; i++) {
	HANDLE_FUNC_ERROR (pspio_projector_free(psp_data->kb_projectors[i]));
      }
      free(psp_data->kb_projectors);
    }

    if (psp_data->vlocal != NULL) {
      HANDLE_FUNC_ERROR (pspio_potential_free(psp_data->vlocal));
    }

    if (psp_data->nlcc != NULL) {
      HANDLE_FUNC_ERROR (pspio_nlcc_free(psp_data->nlcc));
    }

  }

  return PSPIO_SUCCESS;
}


/**********************************************************************
 * Atomic routines                                                    *
 **********************************************************************/

int pspio_pspdata_potential_get(const pspio_pspdata_t *data, const int n,
      const int l, const double j, const double r, double *value) {
  ASSERT(data != NULL, PSPIO_ERROR)
  ASSERT(value != NULL, PSPIO_ERROR)
  ASSERT((n >= 0) && (l >=0) &&
    ((j == 0) || (abs(j - (double)l) - 0.5 < 10e-9)), PSPIO_ERROR)

  HANDLE_FUNC_ERROR(pspio_meshfunc_eval(data->potentials[l+(int)j]->v,r,value))

  return PSPIO_SUCCESS;
}
