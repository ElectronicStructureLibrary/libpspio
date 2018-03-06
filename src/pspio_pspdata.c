/* Copyright (C) 2011-2018 Damien Caliste <dcaliste@free.fr>
 *                         Joseba Alberdi <alberdi@hotmail.es>
 *                         Matthieu Verstraete <matthieu.jean.verstraete@gmail.com>
 *                         Micael Oliveira <micael.oliveira@mpsd.mpg.de>
 *                         Yann Pouillon <devops@materialsevolution.es>
 *
 * This file is part of Libpspio.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * version 2.0. If a copy of the MPL was not distributed with this file, You
 * can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Libpspio is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the Mozilla Public License version 2.0 for
 * more details.
 */

#include <stdio.h>
#include <string.h>

#include "pspio_common.h"
#include "pspio_error.h"
#include "pspio_meshfunc.h"
#include "pspio_pspdata.h"
#include "fhi.h"
#include "upf.h"
#include "abinit.h"

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
  (*pspdata)->pspinfo = NULL;
  (*pspdata)->format_guessed = PSPIO_FMT_UNKNOWN;
  strcpy((*pspdata)->symbol, "");
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
  (*pspdata)->n_projectors_per_l = NULL;
  (*pspdata)->projector_energies = NULL;
  (*pspdata)->projectors = NULL;
  (*pspdata)->projectors_l_max = 0;
  (*pspdata)->l_local = 0;
  (*pspdata)->vlocal = NULL;

  (*pspdata)->xc = NULL;

  (*pspdata)->rho_valence = NULL;

  return PSPIO_SUCCESS;
}

int pspio_pspdata_read(pspio_pspdata_t *pspdata, int file_format,
		       const char *file_name) 
{
  int ierr, fmt;
  FILE * fp;

  assert(pspdata != NULL);

  /* Stop if pspdata already contains some information */
  assert(pspdata->format_guessed == PSPIO_FMT_UNKNOWN);

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
  if (pspdata->n_states > 0)
    SUCCEED_OR_RETURN( pspio_states_lookup_table(pspdata->n_states, pspdata->states, &pspdata->qn_to_istate) );

  return PSPIO_SUCCESS;
}

int pspio_pspdata_write(pspio_pspdata_t *pspdata, int file_format,
			const char *file_name) 
{
  FILE * fp;
  int ierr;

  assert(pspdata != NULL);

  if (pspdata->qn_to_istate == NULL) {
    SUCCEED_OR_RETURN(pspio_states_lookup_table(pspdata->n_states, pspdata->states, &pspdata->qn_to_istate));
  }

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
  if (pspdata->pspinfo != NULL) {
    pspio_pspinfo_free(pspdata->pspinfo);
    pspdata->pspinfo = NULL;
  }
  strcpy(pspdata->symbol, "");
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
  if (pspdata->projector_energies != NULL) {
    free(pspdata->projector_energies);
    pspdata->projector_energies = NULL;
  }
  pspdata->n_projectors = 0;
  if (pspdata->n_projectors_per_l != NULL) {
    free(pspdata->n_projectors_per_l);
  }
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

int pspio_pspdata_set_pspinfo(pspio_pspdata_t *pspdata, const pspio_pspinfo_t *pspinfo)
{
  assert(pspdata != NULL);

  SUCCEED_OR_RETURN(pspio_pspinfo_copy(&pspdata->pspinfo, pspinfo));

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_symbol(pspio_pspdata_t *pspdata, const char symbol[])
{
  assert(pspdata != NULL);

  if (strlen(symbol) >= 4)
    return PSPIO_STRLEN_ERROR;
  else
    strcpy(pspdata->symbol, symbol);

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_z(pspio_pspdata_t *pspdata, double z)
{
  assert(pspdata != NULL);

  pspdata->z = z;

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_zvalence(pspio_pspdata_t *pspdata, double zvalence)
{
  assert(pspdata != NULL);

  pspdata->zvalence = zvalence;

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_nelvalence(pspio_pspdata_t *pspdata, double nelvalence)
{
  assert(pspdata != NULL);

  pspdata->nelvalence = nelvalence;

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_l_max(pspio_pspdata_t *pspdata, int l_max)
{
  assert(pspdata != NULL);

  pspdata->l_max = l_max;

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_wave_eq(pspio_pspdata_t *pspdata, int wave_eq)
{
  assert(pspdata != NULL);

  pspdata->wave_eq = wave_eq;

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_total_energy(pspio_pspdata_t *pspdata, double total_energy)
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

int pspio_pspdata_set_n_states(pspio_pspdata_t *pspdata, int n_states)
{
  int is;

  assert(pspdata != NULL);

  if (pspdata->n_states >= 0) {
    for (is=0; is<pspdata->n_states; is++) {
      pspio_state_free(pspdata->states[is]);
    }
    free(pspdata->states);
  }

  pspdata->n_states = n_states;

  pspdata->states = (pspio_state_t **) malloc ( pspdata->n_states*sizeof(pspio_state_t *));
  FULFILL_OR_EXIT(pspdata->states != NULL, PSPIO_ENOMEM);
  for (is=0; is<pspdata->n_states; is++) {
    pspdata->states[is] = NULL;
  }

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_state(pspio_pspdata_t *pspdata, int index, const pspio_state_t *state)
{
  assert(pspdata != NULL);

  SUCCEED_OR_RETURN( pspio_state_copy(&(pspdata->states[index]), state) )

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_scheme(pspio_pspdata_t *pspdata, int scheme)
{
  assert(pspdata != NULL);

  pspdata->scheme = scheme;

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_n_potentials(pspio_pspdata_t *pspdata, int n_potentials)
{
  int ip;

  assert(pspdata != NULL);

  if (pspdata->n_potentials >= 0) {
    for (ip=0; ip<pspdata->n_potentials; ip++) {
      pspio_potential_free(pspdata->potentials[ip]);
    }
    free(pspdata->potentials);
  }

  pspdata->n_potentials = n_potentials;

  pspdata->potentials = (pspio_potential_t **) malloc ( pspdata->n_potentials*sizeof(pspio_potential_t *));
  FULFILL_OR_EXIT(pspdata->potentials != NULL, PSPIO_ENOMEM);
  for (ip=0; ip<pspdata->n_potentials; ip++) {
    pspdata->potentials[ip] = NULL;
  }

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_potential(pspio_pspdata_t *pspdata, int index, const pspio_potential_t *potential)
{
  assert(pspdata != NULL);

  SUCCEED_OR_RETURN( pspio_potential_copy(&(pspdata->potentials[index]),
                                          potential) )

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_n_projectors(pspio_pspdata_t *pspdata, int n_projectors)
{
  int ip;

  assert(pspdata != NULL);

  if (pspdata->n_projectors >= 0) {
    for (ip=0; ip<pspdata->n_projectors; ip++) {
      pspio_projector_free(pspdata->projectors[ip]);
    }
    free(pspdata->projectors);
    free(pspdata->projector_energies);
  }

  pspdata->n_projectors = n_projectors;

  pspdata->projectors = (pspio_projector_t **) malloc ( pspdata->n_projectors*sizeof(pspio_projector_t *));
  FULFILL_OR_EXIT(pspdata->projectors != NULL, PSPIO_ENOMEM);
  for (ip=0; ip<pspdata->n_projectors; ip++) {
    pspdata->projectors[ip] = NULL;
  }
  pspdata->projector_energies = (double*) malloc ( pspdata->n_projectors * pspdata->n_projectors * sizeof(double));
  FULFILL_OR_EXIT(pspdata->projector_energies != NULL, PSPIO_ENOMEM);
  memset(pspdata->projector_energies, '\0', pspdata->n_projectors * pspdata->n_projectors * sizeof(double));

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_n_projectors_per_l(pspio_pspdata_t *pspdata, int *n_ppl)
{
  assert(pspdata != NULL);
  assert(pspdata->n_projectors_per_l == NULL);

  if ( n_ppl == NULL ) {
    pspdata->n_projectors_per_l = pspio_projectors_per_l(pspdata->projectors,
      pspdata->n_projectors);
  } else {
    pspdata->n_projectors_per_l = n_ppl;
  }

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_projector(pspio_pspdata_t *pspdata, int index, const pspio_projector_t *projector)
{
  assert(pspdata != NULL);
  assert(index >= 0 && index < pspdata->n_projectors);

  SUCCEED_OR_RETURN( pspio_projector_copy(&(pspdata->projectors[index]), projector) )

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_projectors_l_max(pspio_pspdata_t *pspdata, int l_max)
{
  assert(pspdata != NULL);

  pspdata->projectors_l_max = l_max;

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_l_local(pspio_pspdata_t *pspdata, int l_local)
{
  assert(pspdata != NULL);

  pspdata->l_local = l_local;

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_vlocal(pspio_pspdata_t *pspdata, const pspio_potential_t *vlocal)
{
  assert(pspdata != NULL);

  SUCCEED_OR_RETURN( pspio_potential_copy(&(pspdata->vlocal), vlocal) )

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_xc(pspio_pspdata_t *pspdata, const pspio_xc_t *xc)
{
  assert(pspdata != NULL);

  SUCCEED_OR_RETURN( pspio_xc_copy(&(pspdata->xc), xc) );

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_rho_valence(pspio_pspdata_t *pspdata, const pspio_meshfunc_t *rho_valence)
{
  assert(pspdata != NULL);

  SUCCEED_OR_RETURN( pspio_meshfunc_copy(&(pspdata->rho_valence), rho_valence) )

  return PSPIO_SUCCESS;
}

int pspio_pspdata_set_projector_energies(pspio_pspdata_t *pspdata,
                                         const double *energies)
{
  int i, j;

  assert(pspdata != NULL);
  assert(pspdata->projector_energies != NULL && energies != NULL);

  for (i = 0; i < pspdata->n_projectors; i++) {
    pspio_projector_set_energy(pspdata->projectors[i],
                               energies[i * pspdata->n_projectors + i]);
    pspdata->projector_energies[i * pspdata->n_projectors + i] =
      energies[i * pspdata->n_projectors + i];
    for (j = i + 1; j < pspdata->n_projectors; j++) {
      pspdata->projector_energies[i * pspdata->n_projectors + j] = 0.5 *
        (energies[i * pspdata->n_projectors + j] +
         energies[j * pspdata->n_projectors + i]);
      pspdata->projector_energies[j * pspdata->n_projectors + i] =
        pspdata->projector_energies[i * pspdata->n_projectors + j];
    }
  }

  return PSPIO_SUCCESS;
}


/**********************************************************************
 * Getters                                                            *
 **********************************************************************/

int pspio_pspdata_get_format_guessed(const pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->format_guessed;
}

const pspio_pspinfo_t * pspio_pspdata_get_pspinfo(const pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->pspinfo;
}

const char * pspio_pspdata_get_symbol(const pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->symbol;
}

double pspio_pspdata_get_z(const pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->z;
}

double pspio_pspdata_get_zvalence(const pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->zvalence;
}

double pspio_pspdata_get_nelvalence(const pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->nelvalence;
}

int pspio_pspdata_get_l_max(const pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->l_max;
}

int pspio_pspdata_get_wave_eq(const pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->wave_eq;
}

double pspio_pspdata_get_total_energy(const pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->total_energy;
}

const pspio_mesh_t * pspio_pspdata_get_mesh(const pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->mesh;
}

int pspio_pspdata_get_n_states(const pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->n_states;
}

const pspio_state_t * pspio_pspdata_get_state(const pspio_pspdata_t *pspdata, int index)
{
  assert(pspdata != NULL);
  assert(index >= 0 && index < pspdata->n_states);

  return pspdata->states[index];
}

int pspio_pspdata_get_scheme(const pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->scheme;
}

int pspio_pspdata_get_n_potentials(const pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->n_potentials;
}

const pspio_potential_t * pspio_pspdata_get_potential(const pspio_pspdata_t *pspdata, int index)
{
  assert(pspdata != NULL);
  assert(index >= 0 && index < pspdata->n_potentials);

  return pspdata->potentials[index];
}

int pspio_pspdata_get_n_projectors(const pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->n_projectors;
}

int * pspio_pspdata_get_n_projectors_per_l(const pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->n_projectors_per_l;
}

const pspio_projector_t * pspio_pspdata_get_projector(const pspio_pspdata_t *pspdata, int index)
{
  assert(pspdata != NULL);
  assert(index >= 0 && index < pspdata->n_projectors);

  return pspdata->projectors[index];
}

int pspio_pspdata_get_projectors_l_max(const pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->projectors_l_max;
}

int pspio_pspdata_get_l_local(const pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->l_local;
}

const pspio_potential_t * pspio_pspdata_get_vlocal(const pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->vlocal;
}

const pspio_xc_t * pspio_pspdata_get_xc(const pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->xc;
}

const pspio_meshfunc_t * pspio_pspdata_get_rho_valence(const pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  return pspdata->rho_valence;
}

double pspio_pspdata_get_projector_energy(const pspio_pspdata_t *pspdata,
                                          int i, int j)
{
  assert(pspdata != NULL);
  assert(i >= 0 && i < pspdata->n_projectors);
  assert(j >= 0 && j < pspdata->n_projectors);

  return pspdata->projector_energies[i * pspdata->n_projectors + j];
}
