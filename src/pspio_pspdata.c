/*
 Copyright (C) 2011-2012 J. Alberdi, M. Oliveira, Y. Pouillon, and M. Verstraete

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
#include "pspio_upf.h"
#include "util.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

int pspio_pspdata_init(pspio_pspdata_t **pspdata, const char *file_name,
      const int file_format){
  FILE * fp;

  ASSERT(pspdata != NULL, PSPIO_EVALUE);
  ASSERT(*pspdata == NULL, PSPIO_EVALUE);
  
  // Memory allocation
  *pspdata = (pspio_pspdata_t *) malloc (sizeof(pspio_pspdata_t));
  ASSERT(*pspdata != NULL, PSPIO_ENOMEM);

  // Nullify pointers
  (*pspdata)->info = NULL;
  (*pspdata)->symbol = NULL;
  (*pspdata)->mesh = NULL;
  (*pspdata)->states = NULL;
  (*pspdata)->qn_to_istate = NULL;
  (*pspdata)->potentials = NULL;
  (*pspdata)->kb_projectors = NULL;
  (*pspdata)->vlocal = NULL;
  (*pspdata)->xc = NULL;
  (*pspdata)->rho_valence = NULL;

  // open file
  fp = fopen(file_name, "r");
  if(fp == NULL) {
    HANDLE_ERROR(PSPIO_ENOFILE);
  }

  //read from file
  switch(file_format) {
  case UNKNOWN:
    break;
  /* case ABINIT_4: */
  /*   HANDLE_FUNC_ERROR(read_abinit4(fp,pspdata)); */
  /*   break; */
  /* case ABINIT_5: */
  /*   HANDLE_FUNC_ERROR(read_abinit5(fp,pspdata)); */
  /*   break; */
  /* case ABINIT_6: */
  /*   HANDLE_FUNC_ERROR(read_abinit6(fp,pspdata)); */
  /*   break; */
  /* case ABINIT_HGH: */
  /*   HANDLE_FUNC_ERROR(read_abinit_hgh(fp,pspdata)); */
  /*   break; */
  /* case ABINIT_GTH: */
  /*   HANDLE_FUNC_ERROR(read_abinit_gth(fp,pspdata)); */
  /*   break; */
  case ATOM:
    break;
  case FHI98PP:
    break;
  case SIESTA:
    break;
  case UPF:
    HANDLE_FUNC_ERROR(pspio_upf_read(fp, pspdata));
    break;
  default:
    return PSPIO_EFILE_FORMAT;
  }
  
  // close file and check for ierr being non 0
  ASSERT(fclose(fp) == 0, PSPIO_EIO);

  return PSPIO_SUCCESS;
}


int pspio_pspdata_write(const pspio_pspdata_t *pspdata, const char *file_name,
      const int file_format){
  FILE * fp;

  ASSERT(pspdata != NULL, PSPIO_EVALUE);
  
  // open file
  fp = fopen(file_name, "w");
  if(fp == NULL) {
    HANDLE_ERROR(PSPIO_ENOFILE);
  }

  //write to file
  switch(file_format) {
  case UNKNOWN:
    break;
  case UPF:
    HANDLE_FUNC_ERROR(pspio_upf_write(fp, pspdata));
    break;
  default:
    return PSPIO_EFILE_FORMAT;
  }
  
  // close file and check for ierr being non 0
  ASSERT(fclose(fp) == 0, PSPIO_EIO);

  return PSPIO_SUCCESS;
}


int pspio_pspdata_free(pspio_pspdata_t **pspdata){
  int i;

  if (*pspdata != NULL) {
    // Free info
    if ((*pspdata)->info != NULL) free((*pspdata)->info);

    // Free symbol
    if ((*pspdata)->symbol != NULL) free((*pspdata)->symbol);

    // Free mesh
    HANDLE_FUNC_ERROR (pspio_mesh_free(&(*pspdata)->mesh));

    // Free states and lookup table
    if ((*pspdata)->states != NULL) {
      for (i=0; i<(*pspdata)->n_states; i++) {
	HANDLE_FUNC_ERROR (pspio_state_free(&(*pspdata)->states[i]));
      }
      free((*pspdata)->states);
    }
    if ((*pspdata)->qn_to_istate != NULL) {
      for (i=0; (*pspdata)->qn_to_istate[i]!=NULL; i++) {
	free((*pspdata)->qn_to_istate[i]);
      }
    }

    // Free potentials
    if ((*pspdata)->potentials != NULL) {
      for (i=0; i<(*pspdata)->n_potentials; i++) {
	HANDLE_FUNC_ERROR (pspio_potential_free(&(*pspdata)->potentials[i]));
      }
      free((*pspdata)->potentials);
    }

    // Free KB projectors
    if ((*pspdata)->kb_projectors != NULL) {
      for (i=0; i<(*pspdata)->n_kbproj; i++) {
	HANDLE_FUNC_ERROR (pspio_projector_free(&(*pspdata)->kb_projectors[i]));
      }
      free((*pspdata)->kb_projectors);
    }
    if ((*pspdata)->vlocal != NULL) {
      HANDLE_FUNC_ERROR (pspio_potential_free(&(*pspdata)->vlocal));
    }

    // Free xc
    if ((*pspdata)->xc != NULL) {
      HANDLE_FUNC_ERROR (pspio_xc_free(&(*pspdata)->xc));
    }

    // Free valence density
    if ((*pspdata)->rho_valence != NULL) {
      HANDLE_FUNC_ERROR (pspio_meshfunc_free(&(*pspdata)->rho_valence));
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
