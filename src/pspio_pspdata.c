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
#include "pspio_fhi.h"
#include "util.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

int pspio_pspdata_init(pspio_pspdata_t **pspdata) {
  ASSERT(pspdata != NULL, PSPIO_EVALUE);
  ASSERT(*pspdata == NULL, PSPIO_EVALUE);
  
  // Memory allocation
  *pspdata = (pspio_pspdata_t *) malloc (sizeof(pspio_pspdata_t));
  CHECK_ERROR(*pspdata != NULL, PSPIO_ENOMEM);

  // Nullify pointers and initialize all values to 0
  (*pspdata)->info = NULL;
  (*pspdata)->symbol = NULL;
  (*pspdata)->z = 0.0;
  (*pspdata)->zvalence = 0.0;
  (*pspdata)->nelvalence = 0.0;
  (*pspdata)->l_max = 0;
  (*pspdata)->wave_eq = 0;
  (*pspdata)->total_energy = 0.0;

  (*pspdata)->mesh = NULL;

  (*pspdata)->qn_to_istate = NULL;
  (*pspdata)->n_states = 0;
  (*pspdata)->states = NULL;

  (*pspdata)->scheme = 0;
  (*pspdata)->n_potentials = 0;
  (*pspdata)->potentials = NULL;

  (*pspdata)->n_kbproj = 0;
  (*pspdata)->kb_projectors = NULL;
  (*pspdata)->l_local = 0;
  (*pspdata)->kb_l_max = 0;
  (*pspdata)->vlocal = NULL;

  (*pspdata)->xc = NULL;

  (*pspdata)->rho_valence = NULL;

  return PSPIO_SUCCESS;
}

int pspio_pspdata_read(pspio_pspdata_t **pspdata, const char *file_name,
      const int file_format){
  int ierr;
  FILE * fp;

  ASSERT(pspdata != NULL, PSPIO_EVALUE);
  ASSERT(*pspdata != NULL, PSPIO_EVALUE);

  // open file
  fp = fopen(file_name, "r");
  CHECK_ERROR(fp != NULL, PSPIO_ENOFILE);

  //read from file (NOTE: should always rewind the file before trying to read it)
  ierr = PSPIO_EFILE_FORMAT;
  if (ierr && (file_format == ABINIT_4   || file_format == UNKNOWN) ) ierr = PSPIO_ENOSUPPORT;
  if (ierr && (file_format == ABINIT_5   || file_format == UNKNOWN) ) ierr = PSPIO_ENOSUPPORT;
  if (ierr && (file_format == ABINIT_6   || file_format == UNKNOWN) ) ierr = PSPIO_ENOSUPPORT;
  if (ierr && (file_format == ABINIT_HGH || file_format == UNKNOWN) ) ierr = PSPIO_ENOSUPPORT;
  if (ierr && (file_format == ABINIT_GTH || file_format == UNKNOWN) ) ierr = PSPIO_ENOSUPPORT;
  if (ierr && (file_format == ATOM       || file_format == UNKNOWN) ) ierr = PSPIO_ENOSUPPORT;
  if (ierr && (file_format == SIESTA     || file_format == UNKNOWN) ) ierr = PSPIO_ENOSUPPORT;
  if (ierr && (file_format == FHI98PP    || file_format == UNKNOWN) ) {
    CHECK_ERROR(fp != NULL, PSPIO_ENOFILE);
    rewind(fp);
    ierr = pspio_fhi_read(fp, pspdata);
  }
  if (ierr && (file_format == UPF        || file_format == UNKNOWN) ) {
    CHECK_ERROR(fp != NULL, PSPIO_ENOFILE);
    rewind(fp);
    ierr = pspio_upf_read(fp, pspdata);
  }

  // close file
  CHECK_ERROR(fclose(fp) == 0, PSPIO_EIO);

  // create states lookup table
  if ( ierr == PSPIO_SUCCESS ) {
    HANDLE_FUNC_ERROR(pspio_states_lookup_table((*pspdata)->n_states, (*pspdata)->states, &(*pspdata)->qn_to_istate));
  }

  // make sure ierr is not silently ignored
  HANDLE_ERROR(ierr);

  return PSPIO_SUCCESS;
}


int pspio_pspdata_write(const pspio_pspdata_t *pspdata, const char *file_name,
      const int file_format){
  FILE * fp;

  ASSERT(pspdata != NULL, PSPIO_EVALUE);
  
  // open file
  fp = fopen(file_name, "w");
  CHECK_ERROR(fp != NULL, PSPIO_ENOFILE);

  //write to file
  switch(file_format) {
  case FHI98PP:
    HANDLE_FUNC_ERROR(pspio_fhi_write(fp, pspdata));
    break;
  case UPF:
    HANDLE_FUNC_ERROR(pspio_upf_write(fp, pspdata));
    break;
  default:
    fclose(fp);
    return PSPIO_EFILE_FORMAT;
  }
  
  // close file and check for ierr being non 0
  CHECK_ERROR(fclose(fp) == 0, PSPIO_EIO);

  return PSPIO_SUCCESS;
}


void pspio_pspdata_free(pspio_pspdata_t **pspdata){
  int i;

  if (*pspdata != NULL) {

    // General data
    if ((*pspdata)->info != NULL) free((*pspdata)->info);
    if ((*pspdata)->symbol != NULL) free((*pspdata)->symbol);
    (*pspdata)->z = 0.0;
    (*pspdata)->zvalence = 0.0;
    (*pspdata)->nelvalence = 0.0;
    (*pspdata)->l_max = 0;
    (*pspdata)->wave_eq = 0;
    (*pspdata)->total_energy = 0.0;

    // Mesh
    pspio_mesh_free(&(*pspdata)->mesh);

    // States
    if ((*pspdata)->qn_to_istate != NULL) {
      for (i=0; (*pspdata)->qn_to_istate[i]!=NULL; i++) {
	free((*pspdata)->qn_to_istate[i]);
      }
    }
    (*pspdata)->n_states = 0;
    if ((*pspdata)->states != NULL) {
      for (i=0; i<(*pspdata)->n_states; i++) {
	pspio_state_free(&(*pspdata)->states[i]);
      }
      free((*pspdata)->states);
    }

    // Potentials
    (*pspdata)->scheme = 0;
    (*pspdata)->n_potentials = 0;
    if ((*pspdata)->potentials != NULL) {
      for (i=0; i<(*pspdata)->n_potentials; i++) {
	pspio_potential_free(&(*pspdata)->potentials[i]);
      }
      free((*pspdata)->potentials);
    }

    // KB projectors
    if ((*pspdata)->kb_projectors != NULL) {
      for (i=0; i<(*pspdata)->n_kbproj; i++) {
	pspio_projector_free(&(*pspdata)->kb_projectors[i]);
      }
      free((*pspdata)->kb_projectors);
    }
    (*pspdata)->l_local = 0;
    (*pspdata)->kb_l_max = 0;
    if ((*pspdata)->vlocal != NULL) {
      pspio_potential_free(&(*pspdata)->vlocal);
    }

    // XC
    if ((*pspdata)->xc != NULL) {
      pspio_xc_free(&(*pspdata)->xc);
    }

    // Valence density
    if ((*pspdata)->rho_valence != NULL) {
      pspio_meshfunc_free(&(*pspdata)->rho_valence);
    }

  }
}


