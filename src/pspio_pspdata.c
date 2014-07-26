/*
 Copyright (C) 2011-2012 J. Alberdi, M. Oliveira, Y. Pouillon, and M. Verstraete
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

#include <stdio.h>
#include <assert.h>

#include "pspio_common.h"
#include "pspio_error.h"
#include "pspio_meshfunc.h"
#include "pspio_pspdata.h"
#include "pspio_abinit.h"
#include "pspio_fhi.h"
#include "pspio_upf.h"
#include "util.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

int pspio_pspdata_init(pspio_pspdata_t **pspdata) {
  assert(pspdata != NULL);
  assert(*pspdata == NULL);
  
  // Memory allocation
  *pspdata = (pspio_pspdata_t *) malloc (sizeof(pspio_pspdata_t));
  FULFILL_OR_EXIT(*pspdata != NULL, PSPIO_ENOMEM);

  // Nullify pointers and initialize all values to 0
  (*pspdata)->format = PSPIO_FMT_UNKNOWN;
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


int pspio_pspdata_read(pspio_pspdata_t **pspdata, const int *file_format, 
      const char *file_name) {
  int ierr, fmt, psp_fmt;
  FILE * fp;

  assert(file_format != NULL);
  assert(pspdata != NULL);
  assert(*pspdata != NULL);

  psp_fmt = PSPIO_FMT_UNKNOWN;

  // Open file
  fp = fopen(file_name, "r");
  FULFILL_OR_RETURN(fp != NULL, PSPIO_ENOFILE);

  // Read from file
  ierr = PSPIO_ERROR;
  for (fmt=0; fmt<PSPIO_FMT_NFORMATS; fmt++) {
    if ( (*file_format != PSPIO_FMT_UNKNOWN) && (fmt != *file_format) ) {
      continue;
    }
    
    // The error chain should be reset, as some errors might have been
    // set in the previous iteration of this loop
    pspio_error_free();

    // Always rewind the file to allow for multiple reads
    FULFILL_OR_RETURN(fp != NULL, PSPIO_ENOFILE);
    rewind(fp);

    switch (fmt) {
    case PSPIO_FMT_ABINIT_6:
      ierr = pspio_abinit_read(fp, pspdata, fmt);
      psp_fmt = PSPIO_FMT_ABINIT_6;
      break;
    case PSPIO_FMT_FHI98PP:
      ierr = pspio_fhi_read(fp, pspdata);
      psp_fmt = PSPIO_FMT_FHI98PP;
      break;
    case PSPIO_FMT_UPF:
      ierr = pspio_upf_read(fp, pspdata);
      psp_fmt = PSPIO_FMT_UPF;
      break;

    default:
      ierr = PSPIO_ENOSUPPORT;
    }

    if (ierr != PSPIO_SUCCESS) pspio_pspdata_reset(pspdata);
    if ( (ierr == PSPIO_SUCCESS) || (*file_format != PSPIO_FMT_UNKNOWN) ) break;
  }

  // Store the format
  (*pspdata)->format = psp_fmt;

  // Close file
  FULFILL_OR_RETURN( fclose(fp) == 0, PSPIO_EIO );

  // Make sure ierr is not silently ignored
  RETURN_WITH_ERROR(ierr);

  // Create states lookup table
  SUCCEED_OR_RETURN( pspio_states_lookup_table((*pspdata)->n_states,
    (*pspdata)->states, &(*pspdata)->qn_to_istate) );

  return PSPIO_SUCCESS;
}


int pspio_pspdata_write(const pspio_pspdata_t *pspdata, const int file_format, 
      const char *file_name) {
  FILE * fp;
  int ierr;

  assert(pspdata != NULL);
  
  // Open file
  fp = fopen(file_name, "w");
  FULFILL_OR_RETURN(fp != NULL, PSPIO_ENOFILE);

  // Write to file in the selected format
  switch(file_format) {
    case PSPIO_FMT_ABINIT_5:
    case PSPIO_FMT_ABINIT_6:
      ierr = pspio_abinit_write(fp, pspdata, file_format);
      break;
    case PSPIO_FMT_FHI98PP:
      ierr = pspio_fhi_write(fp, pspdata);
      break;
    case PSPIO_FMT_UPF:
      ierr = pspio_upf_write(fp, pspdata);
      break;
    default:
      ierr = PSPIO_EFILE_FORMAT;
  }
  
  // Close file and check for ierr being non 0
  FULFILL_OR_RETURN( fclose(fp) == 0, PSPIO_EIO );

  // Make sure ierr is not silently ignored
  RETURN_WITH_ERROR(ierr);

  return PSPIO_SUCCESS;
}


void pspio_pspdata_reset(pspio_pspdata_t **pspdata) {
  int i;

  assert(*pspdata != NULL);

  // General data
  free((*pspdata)->info);
  (*pspdata)->info = NULL;
  free((*pspdata)->symbol);
  (*pspdata)->symbol = NULL;
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
    free((*pspdata)->qn_to_istate);
  }
  if ((*pspdata)->states != NULL) {
    for (i=0; i<(*pspdata)->n_states; i++) {
      pspio_state_free(&(*pspdata)->states[i]);
    }
    free((*pspdata)->states);
  }
  (*pspdata)->n_states = 0;

  // Potentials
  if ((*pspdata)->potentials != NULL) {
    for (i=0; i<(*pspdata)->n_potentials; i++) {
      pspio_potential_free(&(*pspdata)->potentials[i]);
    }
    free((*pspdata)->potentials);
  }
  (*pspdata)->n_potentials = 0;
  (*pspdata)->scheme = 0;
  
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

void pspio_pspdata_free(pspio_pspdata_t **pspdata) {
  if (*pspdata != NULL) {
    pspio_pspdata_reset(pspdata);
    free(*pspdata);
    *pspdata = NULL;
  }
}
