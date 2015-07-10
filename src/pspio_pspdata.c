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
#include <string.h>

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

int pspio_pspdata_alloc(pspio_pspdata_t **pspdata)
{
  assert(pspdata != NULL);
  assert(*pspdata == NULL);
  
  /* Memory allocation */
  *pspdata = (pspio_pspdata_t *) malloc (sizeof(pspio_pspdata_t));
  FULFILL_OR_EXIT(*pspdata != NULL, PSPIO_ENOMEM);

  /* Nullify pointers and initialize all values to 0 */
  (*pspdata)->format_guessed = PSPIO_FMT_UNKNOWN;
  (*pspdata)->info = NULL;
  (*pspdata)->symbol = (char *) malloc (3*sizeof(char)); /* This string has a fixed lenght, so we allocate the memory here */
  FULFILL_OR_EXIT((*pspdata)->symbol != NULL, PSPIO_ENOMEM);
  sprintf((*pspdata)->symbol, "   ");
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

  (*pspdata)->n_projectors = 0;
  (*pspdata)->projectors = NULL;
  (*pspdata)->projectors_l_max = 0;
  (*pspdata)->l_local = 0;
  (*pspdata)->vlocal = NULL;

  (*pspdata)->xc = NULL;

  (*pspdata)->rho_valence = NULL;

  return PSPIO_SUCCESS;
}

int pspio_pspdata_read(pspio_pspdata_t *pspdata, const int file_format, 
		       const char *file_name) 
{
  int ierr, fmt;
  FILE * fp;

  assert(pspdata != NULL);

  /* Open file */
  fp = fopen(file_name, "r");
  FULFILL_OR_RETURN(fp != NULL, PSPIO_ENOFILE);

  /* Read from file */
  ierr = PSPIO_ERROR;
  for (fmt=0; fmt<PSPIO_FMT_NFORMATS; fmt++) {
    if ( (file_format != PSPIO_FMT_UNKNOWN) && (fmt != file_format) ) {
      continue;
    }
    
    /* The error chain should be reset, as some errors might have been
       set in the previous iteration of this loop */
    pspio_error_free();

    /* Always rewind the file to allow for multiple reads */
    FULFILL_OR_RETURN(fp != NULL, PSPIO_ENOFILE);
    rewind(fp);

    fflush(stdout);
    switch (fmt) {
    case PSPIO_FMT_ABINIT_6:
      ierr = pspio_abinit_read(fp, pspdata, fmt);
      break;
    case PSPIO_FMT_FHI98PP:
      ierr = pspio_fhi_read(fp, pspdata);
      break;
    case PSPIO_FMT_UPF:
      ierr = pspio_upf_read(fp, pspdata);
      break;

    default:
      ierr = PSPIO_ENOSUPPORT;
    }

    if (ierr != PSPIO_SUCCESS) pspio_pspdata_reset(pspdata);

    /* Store the format */
    if ( (ierr == PSPIO_SUCCESS) || (file_format != PSPIO_FMT_UNKNOWN) ) {
      pspdata->format_guessed = fmt;
      break;
    }
  }

  if ( ierr == PSPIO_SUCCESS ) {
  }

  /* Close file */
  FULFILL_OR_RETURN( fclose(fp) == 0, PSPIO_EIO );

  /* Make sure ierr is not silently ignored */
  FULFILL_OR_RETURN(ierr == PSPIO_SUCCESS, ierr);

  /* Create states lookup table */
  SUCCEED_OR_RETURN( pspio_states_lookup_table(pspdata->n_states, pspdata->states, &pspdata->qn_to_istate) );

  return PSPIO_SUCCESS;
}

int pspio_pspdata_write(pspio_pspdata_t *pspdata, const int file_format, 
			const char *file_name) 
{
  FILE * fp;
  int ierr;

  assert(pspdata != NULL);
  assert(pspdata->qn_to_istate != NULL);

  /* Open file */
  fp = fopen(file_name, "w");
  FULFILL_OR_RETURN(fp != NULL, PSPIO_ENOFILE);

  /* Write to file in the selected format */
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
  
  /* Close file and check for ierr being non 0 */
  FULFILL_OR_RETURN( fclose(fp) == 0, PSPIO_EIO );

  /* Make sure ierr is not silently ignored */
  RETURN_WITH_ERROR( ierr );

  return PSPIO_SUCCESS;
}

void pspio_pspdata_reset(pspio_pspdata_t *pspdata)
{
  int i;

  assert(pspdata != NULL);

  /* General data */
  if (pspdata->info != NULL) {
    free(pspdata->info);
    pspdata->info = NULL;
  }
  sprintf(pspdata->symbol, "   ");
  pspdata->z = 0.0;
  pspdata->zvalence = 0.0;
  pspdata->nelvalence = 0.0;
  pspdata->l_max = 0;
  pspdata->wave_eq = 0;
  pspdata->total_energy = 0.0;

  /* Mesh */
  if (pspdata->mesh != NULL) {
    pspio_mesh_free(pspdata->mesh);
    pspdata->mesh = NULL;
  }
  
  /* States */
  if (pspdata->qn_to_istate != NULL) {
    for (i=0; pspdata->qn_to_istate[i]!=NULL; i++) {
      free(pspdata->qn_to_istate[i]);
    }
    free(pspdata->qn_to_istate);
    pspdata->qn_to_istate = NULL;
  }
  if (pspdata->states != NULL) {
    for (i=0; i<pspdata->n_states; i++) {
      pspio_state_free(pspdata->states[i]);
    }
    free(pspdata->states);
    pspdata->states = NULL;
  }
  pspdata->n_states = 0;

  /* Potentials */
  if (pspdata->potentials != NULL) {
    for (i=0; i<pspdata->n_potentials; i++) {
      pspio_potential_free(pspdata->potentials[i]);
    }
    free(pspdata->potentials);
    pspdata->potentials = NULL;
  }
  pspdata->n_potentials = 0;
  pspdata->scheme = 0;
  
  /* Projectors */
  if (pspdata->projectors != NULL) {
    for (i=0; i<pspdata->n_projectors; i++) {
      pspio_projector_free(pspdata->projectors[i]);
    }
    free(pspdata->projectors);
    pspdata->projectors = NULL;
  }
  pspdata->n_projectors = 0;
  pspdata->l_local = 0;
  pspdata->projectors_l_max = 0;
  if (pspdata->vlocal != NULL) {
    pspio_potential_free(pspdata->vlocal);
    pspdata->vlocal = NULL;
  }

  /* XC */
  if (pspdata->xc != NULL) {
    pspio_xc_free(pspdata->xc);
    pspdata->xc = NULL;
  }

  /* Valence density */
  if (pspdata->rho_valence != NULL) {
    pspio_meshfunc_free(pspdata->rho_valence);
    pspdata->rho_valence = NULL;
  }
}

void pspio_pspdata_free(pspio_pspdata_t *pspdata)
{
  if (pspdata != NULL) {
    pspio_pspdata_reset(pspdata);
    free(pspdata);
  }
}


/**********************************************************************
 * Setters                                                            *
 **********************************************************************/

int pspio_pspdata_set_symbol(pspio_pspdata_t *pspdata, char * symbol)
{
  assert(pspdata != NULL);

  FULFILL_OR_RETURN(symbol != NULL, PSPIO_EVALUE);

  pspdata->symbol = (char *) malloc (3*sizeof(char));
  FULFILL_OR_EXIT( pspdata->symbol != NULL, PSPIO_ENOMEM );
  strncpy(pspdata->symbol, symbol, 3);

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_z(pspio_pspdata_t *pspdata, const double z)
{
  assert(pspdata != NULL);

  pspdata->z = z;

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_zvalence(pspio_pspdata_t *pspdata, const double zvalence)
{
  assert(pspdata != NULL);

  pspdata->zvalence = zvalence;

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_nelvalence(pspio_pspdata_t *pspdata, const double nelvalence)
{
  assert(pspdata != NULL);

  pspdata->nelvalence = nelvalence;

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_l_max(pspio_pspdata_t *pspdata, const int l_max)
{
  assert(pspdata != NULL);

  pspdata->l_max = l_max;

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_wave_eq(pspio_pspdata_t *pspdata, const int wave_eq)
{
  assert(pspdata != NULL);

  pspdata->wave_eq = wave_eq;

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_total_energy(pspio_pspdata_t *pspdata, const double total_energy)
{
  assert(pspdata != NULL);

  pspdata->total_energy = total_energy;

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_mesh(pspio_pspdata_t *pspdata, const pspio_mesh_t *mesh)
{
  assert(pspdata != NULL);

  SUCCEED_OR_RETURN(pspio_mesh_copy(&pspdata->mesh, mesh));

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_n_states(pspio_pspdata_t *pspdata, const int n_states)
{
  int is;

  assert(pspdata != NULL);

  pspdata->n_states = n_states;

  pspdata->states = (pspio_state_t **) malloc ( pspdata->n_states*sizeof(pspio_state_t *));
  FULFILL_OR_EXIT(pspdata->states != NULL, PSPIO_ENOMEM);
  for (is=0; is<pspdata->n_states; is++) {
    pspdata->states[is] = NULL;
  }

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_state(pspio_pspdata_t *pspdata, const int index, const pspio_state_t *state)
{
  assert(pspdata != NULL);

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_scheme(pspio_pspdata_t *pspdata, const int scheme)
{
  assert(pspdata != NULL);

  pspdata->scheme = scheme;

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_n_potentials(pspio_pspdata_t *pspdata, const int n_potentials)
{
  int ip;

  assert(pspdata != NULL);

  pspdata->n_potentials = n_potentials;

  pspdata->potentials = (pspio_potential_t **) malloc ( pspdata->n_potentials*sizeof(pspio_potential_t *));
  FULFILL_OR_EXIT(pspdata->potentials != NULL, PSPIO_ENOMEM);
  for (ip=0; ip<pspdata->n_potentials; ip++) {
    pspdata->potentials[ip] = NULL;
  }

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_potential(pspio_pspdata_t *pspdata, const int index, const pspio_potential_t *potential)
{
  assert(pspdata != NULL);

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_n_projectors(pspio_pspdata_t *pspdata, const int n_projectors)
{
  int ip;

  assert(pspdata != NULL);

  pspdata->n_projectors = n_projectors;

  pspdata->projectors = (pspio_projector_t **) malloc ( pspdata->n_projectors*sizeof(pspio_projector_t *));
  FULFILL_OR_EXIT(pspdata->projectors != NULL, PSPIO_ENOMEM);
  for (ip=0; ip<pspdata->n_projectors; ip++) {
    pspdata->projectors[ip] = NULL;
  }

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_projector(pspio_pspdata_t *pspdata, const int index, const pspio_projector_t *projector)
{
  assert(pspdata != NULL);

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_projectors_l_max(pspio_pspdata_t *pspdata, const int l_max)
{
  assert(pspdata != NULL);

  pspdata->projectors_l_max = l_max;

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_l_local(pspio_pspdata_t *pspdata, const int l_local)
{
  assert(pspdata != NULL);

  pspdata->l_local = l_local;

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_vlocal(pspio_pspdata_t *pspdata, const pspio_potential_t *vlocal)
{
  assert(pspdata != NULL);

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_xc(pspio_pspdata_t *pspdata, const pspio_xc_t *xc)
{
  assert(pspdata != NULL);

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_rho_valence(pspio_pspdata_t *pspdata, const pspio_meshfunc_t *rho_valence)
{
  assert(pspdata != NULL);

  return PSPIO_SUCCESS;
}


/**********************************************************************
 * Getters                                                            *
 **********************************************************************/

int pspio_pspdata_get_format_guessed(pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->format_guessed;
}

char * pspio_pspdata_get_symbol(pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->symbol;
}

double pspio_pspdata_get_z(pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->z;
}

double pspio_pspdata_get_zvalence(pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->zvalence;
}

double pspio_pspdata_get_nelvalence(pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->nelvalence;
}

int pspio_pspdata_get_l_max(pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->l_max;
}

int pspio_pspdata_get_wave_eq(pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->wave_eq;
}

double pspio_pspdata_get_total_energy(pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->total_energy;
}

pspio_mesh_t * pspio_pspdata_get_mesh(pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->mesh;
}

int pspio_pspdata_get_n_states(pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->n_states;
}

pspio_state_t * pspio_pspdata_get_state(pspio_pspdata_t *pspdata, const int index)
{
  assert(pspdata != NULL);

  return pspdata->states[index];
}

int pspio_pspdata_get_scheme(pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->scheme;
}

int pspio_pspdata_get_n_potentials(pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->n_potentials;
}

pspio_potential_t * pspio_pspdata_get_potential(pspio_pspdata_t *pspdata, const int index)
{
  assert(pspdata != NULL);

  return pspdata->potentials[index];
}

int pspio_pspdata_get_n_projectors(pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->n_projectors;
}

pspio_projector_t * pspio_pspdata_get_projector(pspio_pspdata_t *pspdata, const int index)
{
  assert(pspdata != NULL);

  return pspdata->projectors[index];
}

int pspio_pspdata_get_projectors_l_max(pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->projectors_l_max;
}

int pspio_pspdata_get_l_local(pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->l_local;
}

pspio_potential_t * pspio_pspdata_get_vlocal(pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->vlocal;
}

pspio_xc_t * pspio_pspdata_get_xc(pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->xc;
}

pspio_meshfunc_t * pspio_pspdata_get_rho_valence(pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->rho_valence;
}
