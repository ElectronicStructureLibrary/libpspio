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

*/

/** 
 * @file upf_read.c
 * @brief routines to read UPF files 
 */
#include <string.h>
#include <ctype.h>

#include "pspio_common.h"
#include "pspio_error.h"
#include "upf.h"
#include "util.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif

int upf_read_info(FILE *fp, pspio_pspdata_t **pspdata){
  char line[PSPIO_STRLEN_LINE];
  int il, nlines = 0;

  //Find init tag
  SUCCEED_OR_RETURN( upf_tag_init(fp,"PP_INFO", GO_BACK) );

  //Count how many lines we have
  while ( upf_tag_check_end(fp, "PP_INFO") != PSPIO_SUCCESS ) {
    nlines += 1;
    FULFILL_OR_RETURN( fp != NULL, PSPIO_EIO );
  }

  //Go back
  SUCCEED_OR_RETURN( upf_tag_init(fp,"PP_INFO", GO_BACK) );

  //Allocate memory
  (*pspdata)->info = (char *) malloc (1*sizeof(char));
  FULFILL_OR_EXIT((*pspdata)->info != NULL, PSPIO_ENOMEM);
  (*pspdata)->info[0] = '\0';

  //Store all the lines
  for (il=0; il<nlines; il++) {
    FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
    (*pspdata)->info = realloc((*pspdata)->info, strlen((*pspdata)->info)+strlen(line)+1);
    FULFILL_OR_EXIT((*pspdata)->info != NULL, PSPIO_ENOMEM);
    strncat((*pspdata)->info, line, strlen(line));
  }

  //Check end tag
  SUCCEED_OR_RETURN( upf_tag_check_end(fp,"PP_INFO") );

  return PSPIO_SUCCESS;
}

int upf_read_header(FILE *fp, int *np, pspio_pspdata_t **pspdata){
  char line[PSPIO_STRLEN_LINE];
  int version_number, i;
  char nlcc_flag[2];
  char xc_string[23];
  int exchange, correlation;
  double wfc_cutoff, rho_cutoff;

  //Find init tag
  SUCCEED_OR_RETURN( upf_tag_init(fp,"PP_HEADER", GO_BACK) );

  //Read the version number
  FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  FULFILL_OR_RETURN( sscanf(line, "%d", &version_number) == 1, PSPIO_EFILE_CORRUPT );
 
  //Read the atomic symbol
  FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  (*pspdata)->symbol = (char *) malloc (3*sizeof(char));
  FULFILL_OR_EXIT((*pspdata)->symbol != NULL, PSPIO_ENOMEM);
  strcpy((*pspdata)->symbol, strtok(line," "));
  SUCCEED_OR_RETURN( symbol_to_z((*pspdata)->symbol, &(*pspdata)->z) );

  //Read the kind of pseudo-potentials US|NC|PAW
  FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  //At the moment LIBPSP_IO can only read norm-conserving pseudo-potentials
  FULFILL_OR_RETURN( strncmp(strtok(line," "),"NC",2) == 0, PSPIO_ENOSUPPORT );

  // read the nonlinear core correction
  FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  FULFILL_OR_RETURN( sscanf(line, "%1s", nlcc_flag) == 1, PSPIO_EFILE_CORRUPT );

  //Exchange-correlation functional
  FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  strncpy(xc_string, line, 22); //the xc string should always the first 21 chars of the line
  xc_string[22] = '\0';
  SUCCEED_OR_RETURN( upf_to_libxc(xc_string, &exchange, &correlation) );

  // read the Z valence
  FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  FULFILL_OR_RETURN( sscanf(line, "%lf",&(*pspdata)->zvalence) == 1, PSPIO_EFILE_CORRUPT );

  // read the total energy
  FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  FULFILL_OR_RETURN( sscanf(line, "%lf",&(*pspdata)->total_energy) == 1, PSPIO_EFILE_CORRUPT );
  
  //read the suggested cutoff for wfc and rho
  FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  FULFILL_OR_RETURN( sscanf(line, "%lf %lf",&wfc_cutoff,&rho_cutoff) == 2, PSPIO_EFILE_CORRUPT );
  
  // read the max angular momentun component of the KB projectors
  FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  FULFILL_OR_RETURN( sscanf(line, "%d",&(*pspdata)->kb_l_max) == 1, PSPIO_EFILE_CORRUPT );
  
  // read the number of points in mesh
  FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  FULFILL_OR_RETURN( sscanf(line, "%d",np) == 1, PSPIO_EFILE_CORRUPT );
  
  // read the number of wavefunctions and projectors 
  FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  FULFILL_OR_RETURN( sscanf(line, "%d %d", &(*pspdata)->n_states, &(*pspdata)->n_kbproj) == 2, PSPIO_EFILE_CORRUPT );

  //Skip info on wavefunctions, as it is repeated in the PP_PSWFC block
  for (i=0; i<(*pspdata)->n_states+1; i++) {
    FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  }

  // Initialize xc
  SUCCEED_OR_RETURN( pspio_xc_alloc(&(*pspdata)->xc) );
  if (strcmp(nlcc_flag, "T") == 0) {
    SUCCEED_OR_RETURN( pspio_xc_set_nlcc_scheme(&(*pspdata)->xc, PSPIO_NLCC_UNKNOWN) );
  } else {
    SUCCEED_OR_RETURN( pspio_xc_set_nlcc_scheme(&(*pspdata)->xc, PSPIO_NLCC_NONE) );
  }
  pspio_xc_set_id(&(*pspdata)->xc, exchange, correlation);

  //Check end tag
  SUCCEED_OR_RETURN( upf_tag_check_end(fp, "PP_HEADER") );

  return PSPIO_SUCCESS;
}


int upf_read_mesh(FILE *fp, const int np, pspio_pspdata_t **pspdata) {
  char line[PSPIO_STRLEN_LINE];
  int i, j, nargs;
  double tmp[4];
  double *r, *drdi;

  // Find init tag
  SUCCEED_OR_RETURN( upf_tag_init(fp, "PP_MESH", GO_BACK) );

  // Allocate memory
  r = (double *) malloc (np*sizeof(double));
  FULFILL_OR_EXIT(r != NULL, PSPIO_ENOMEM);

  drdi = (double *) malloc (np*sizeof(double));
  FULFILL_OR_EXIT(drdi != NULL, PSPIO_ENOMEM);

  // Read mesh points
  // Note: need to treat some errors without helper macros, for sake of
  //       feasibility.
  DEFER_FUNC_ERROR( upf_tag_init(fp, "PP_R", NO_GO_BACK) );
  if ( pspio_error_get_last(__func__) == PSPIO_SUCCESS ) {
    for (i=0; i<np; i+=4) {
      FULFILL_OR_BREAK( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
      nargs = sscanf(line,"%lf %lf %lf %lf",&tmp[0],&tmp[1],&tmp[2],&tmp[3]);
      FULFILL_OR_BREAK( nargs < 5 && nargs > 0, PSPIO_EFILE_CORRUPT );
      for (j=0; j<nargs; j++) r[i+j] = tmp[j];
    }
    SKIP_FUNC_ON_ERROR( upf_tag_check_end(fp, "PP_R") );
    
    // Read Rab
    SKIP_FUNC_ON_ERROR( upf_tag_init(fp, "PP_RAB", NO_GO_BACK) );
    for (i=0; i<np; i+=4) {
      FULFILL_OR_BREAK( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
      nargs = sscanf(line,"%lf %lf %lf %lf",&tmp[0],&tmp[1],&tmp[2],&tmp[3]);
      FULFILL_OR_BREAK( nargs < 5 && nargs > 0, PSPIO_EFILE_CORRUPT );
      for (j=0; j<nargs; j++) drdi[i+j] = tmp[j];
    }
    SKIP_FUNC_ON_ERROR( upf_tag_check_end(fp, "PP_RAB") );

    // Store the mesh in the pspdata structure
    SKIP_FUNC_ON_ERROR( pspio_mesh_alloc(&(*pspdata)->mesh, np) );
    SKIP_CALL_ON_ERROR( pspio_mesh_init_from_points(&(*pspdata)->mesh,
      r, drdi) );
  }

  // Free memory
  free(r);
  free(drdi);

  // Make sure no error is left unhandled
  RETURN_ON_DEFERRED_ERROR;

  // Check end tag
  SUCCEED_OR_RETURN( upf_tag_check_end(fp, "PP_MESH") ); 

  return PSPIO_SUCCESS;
}


int upf_read_nlcc(FILE *fp, const int np, pspio_pspdata_t **pspdata) {
  char line[PSPIO_STRLEN_LINE];
  int i, j, nargs;
  double tmp[4];
  double *rho;

  //Find init tag
  SUCCEED_OR_RETURN( upf_tag_init(fp, "PP_NLCC", GO_BACK) );

  //Allocate memory
  rho = (double *) malloc (np*sizeof(double));
  FULFILL_OR_EXIT(rho != NULL, PSPIO_ENOMEM);

  //Read core rho
  for (i=0; i<np; i+=4) {
    FULFILL_OR_BREAK( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
    nargs = sscanf(line,"%lf %lf %lf %lf",&tmp[0],&tmp[1],&tmp[2],&tmp[3]);
    FULFILL_OR_BREAK( nargs < 5 && nargs > 0, PSPIO_EFILE_CORRUPT );
    for (j=0; j<nargs; j++) rho[i+j] = tmp[j];
  }

  //Store the non-linear core corrections in the pspdata structure
  SKIP_FUNC_ON_ERROR( pspio_xc_set_nlcc_density(&(*pspdata)->xc,
    (*pspdata)->mesh, rho, NULL, NULL) );

  //Free memory
  free(rho);

  // Make sure no error is left unhandled
  RETURN_ON_DEFERRED_ERROR;

  //Check end tag
  SUCCEED_OR_RETURN( upf_tag_check_end(fp,"PP_NLCC") );

  return PSPIO_SUCCESS;
}


int upf_read_nonlocal(FILE *fp, const int np, pspio_pspdata_t **pspdata) {
  char line[PSPIO_STRLEN_LINE];
  int proj_np = 0;
  int i, j, k, l, n_dij, ii, jj, nargs;
  double tmp[4];
  double *proj_j;
  double energy;
  double *ekb, *projector_read;
  pspio_qn_t *qn = NULL;

  // Find init tag
  SUCCEED_OR_RETURN( upf_tag_init(fp,"PP_NONLOCAL",GO_BACK) );

  // Allocate memory
  SUCCEED_OR_RETURN( pspio_qn_alloc(&qn) );

  proj_j = (double *) malloc ((*pspdata)->n_kbproj * sizeof(double));
  FULFILL_OR_EXIT(proj_j != NULL, PSPIO_ENOMEM);

  projector_read = (double *) malloc (np * sizeof(double));
  FULFILL_OR_EXIT(projector_read != NULL, PSPIO_ENOMEM);

  (*pspdata)->kb_projectors = (pspio_projector_t **) malloc ( (*pspdata)->n_kbproj*sizeof(pspio_projector_t *));
  FULFILL_OR_EXIT((*pspdata)->kb_projectors != NULL, PSPIO_ENOMEM);
  for (i=0; i<(*pspdata)->n_kbproj; i++) (*pspdata)->kb_projectors[i] = NULL;

  ekb = (double *) malloc ((*pspdata)->n_kbproj*sizeof(double));
  FULFILL_OR_EXIT(ekb != NULL, PSPIO_ENOMEM);
  memset(ekb, 0, (*pspdata)->n_kbproj*sizeof(double));

  // We start by reading the KB energies, as it is more convinient this way
  DEFER_FUNC_ERROR( upf_tag_init(fp,"PP_DIJ",NO_GO_BACK) );

  // Read the number of n_dij
  SKIP_TEST_ON_ERROR( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  SKIP_TEST_ON_ERROR( sscanf(line,"%d",&n_dij) == 1, PSPIO_EFILE_CORRUPT );

  for (i=0; i<n_dij; i++){
    FULFILL_OR_BREAK( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
    FULFILL_OR_BREAK( sscanf(line,"%d %d %lf", &ii, &jj, &energy) == 3,
      PSPIO_EFILE_CORRUPT );
    FULFILL_OR_BREAK( ii == jj, PSPIO_EVALUE );
    ekb[ii-1] = energy*2.0;
  }

  // Check end tag
  SKIP_FUNC_ON_ERROR( upf_tag_check_end(fp,"PP_DIJ") );
  SKIP_FUNC_ON_ERROR( upf_tag_check_end(fp,"PP_NONLOCAL") );

  // In the case of a fully-relativistic calculation, there is
  // extra information in the ADDINFO tag
  if ((*pspdata)->wave_eq == PSPIO_EQN_DIRAC) {
    SKIP_FUNC_ON_ERROR( upf_tag_init(fp,"PP_ADDINFO",GO_BACK) );
    // Skip the lines with the wavefunctions info
    for (i=0; i<(*pspdata)->n_states; i++) {
      FULFILL_OR_BREAK( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
    }
    // Read j quantum numbers
    for (i=0; i<(*pspdata)->n_kbproj; i++) {
      FULFILL_OR_BREAK( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
      FULFILL_OR_BREAK( sscanf(line,"%d %lf",&j, &proj_j[i]) == 2, PSPIO_EFILE_CORRUPT );
    }
  } else {
    for (i=0; i<(*pspdata)->n_kbproj; i++) proj_j[i] = 0.0;
  }

  // Now we go back and read the projector functions
  SKIP_FUNC_ON_ERROR( upf_tag_init(fp, "PP_NONLOCAL", GO_BACK) );

  for (i=0; i<(*pspdata)->n_kbproj; i++){
    SUCCEED_OR_BREAK( upf_tag_init(fp, "PP_BETA", NO_GO_BACK) );

    FULFILL_OR_BREAK( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
    FULFILL_OR_BREAK( sscanf(line, "%d %d", &ii, &l) == 2,
      PSPIO_EFILE_CORRUPT );

    // Read the number of points of projections
    FULFILL_OR_BREAK( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
    FULFILL_OR_BREAK( sscanf(line, "%d", &proj_np) == 1, PSPIO_EFILE_CORRUPT );

    // Read the projector function
    for (j=0; j<proj_np; j+=4){
      FULFILL_OR_BREAK( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
      nargs = sscanf(line,"%lf %lf %lf %lf", &tmp[0], &tmp[1], &tmp[2],
        &tmp[3]);
      FULFILL_OR_BREAK( nargs < 5 && nargs > 0, PSPIO_EFILE_CORRUPT );

      for (k=0; k<nargs; k++) projector_read[k+j] = tmp[k];
    }
    BREAK_ON_DEFERRED_ERROR;

    // Fill with zeros, if any left
    for (j=proj_np; j<np; j++) projector_read[j] = 0.0;

    // Convert units
    for (j=0; j<np; j++) projector_read[j] /= 2.0*(*pspdata)->mesh->r[j];

    // Store the projectors in the pspdata structure
    SUCCEED_OR_BREAK( pspio_qn_set(&qn, 0, l, proj_j[i]) );
    SUCCEED_OR_BREAK(
      pspio_projector_alloc( &((*pspdata)->kb_projectors[i]), np) );
    SUCCEED_OR_BREAK( pspio_projector_set( &(*pspdata)->kb_projectors[i], qn,
      ekb[i], (*pspdata)->mesh, projector_read) );

    // Check end tag
    if ( pspio_error_get_last(__func__) == PSPIO_SUCCESS ) {
      if (upf_tag_check_end(fp,"PP_BETA") != PSPIO_SUCCESS) {
        // We may have a "new" version of the format with two extra lines at
        // the end of this block, so we will try to skip them. Note that one
        // line was already read by upf_tag_check_end.
        FULFILL_OR_BREAK( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL,
          PSPIO_EIO );
        SUCCEED_OR_BREAK( upf_tag_check_end(fp,"PP_BETA") );
      }
    }
  }

  // Free memory
  free(projector_read);
  free(ekb);
  free(proj_j);
  pspio_qn_free(&qn);

  // Make sure no error is left unhandled
  RETURN_ON_DEFERRED_ERROR;

  return PSPIO_SUCCESS;
}


int upf_read_local(FILE *fp, const int np, pspio_pspdata_t **pspdata) {
  char line[PSPIO_STRLEN_LINE];
  int i, j, nargs, n;
  double *vlocal;
  double tmp[4];
  pspio_qn_t *qn = NULL;

  // Find init tag
  SUCCEED_OR_RETURN( upf_tag_init(fp, "PP_LOCAL",GO_BACK) );

  // Allocate memory
  SUCCEED_OR_RETURN( pspio_qn_alloc(&qn) );

  vlocal = (double *) malloc (np*sizeof(double));
  FULFILL_OR_EXIT(vlocal != NULL, PSPIO_ENOMEM);
  for (i=0; i<np; i++) vlocal[i] = 0.0;

  // Deduce l local (this is done in a very crude way and it should
  // probably be made more robust)
  (*pspdata)->l_local = -1;
  for (i=0; i<(*pspdata)->l_max+1; i++) {
    n = 0;
    for (j=0; j<(*pspdata)->n_kbproj; j++) {
      if ( pspio_projector_get_l((*pspdata)->kb_projectors[j]) == i ) n++;
    }
    if ( n == 0 ) (*pspdata)->l_local = i;
  }

  // Read local potential
  for (i=0; i<np; i+=4) {
    FULFILL_OR_BREAK( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL,  PSPIO_EIO );
    nargs = sscanf(line,"%lf %lf %lf %lf",&tmp[0],&tmp[1],&tmp[2],&tmp[3]);
    FULFILL_OR_BREAK( nargs < 5 && nargs > 0, PSPIO_EFILE_CORRUPT );
    for (j=0; j<nargs; j++) vlocal[i+j] = tmp[j];
  }

  // Convert from Rydbergs
  for (i=0; i<np; i++) vlocal[i] /= 2.0;

  // Store the local potential in the pspdata structure
  SKIP_FUNC_ON_ERROR( pspio_qn_set(&qn, 0, (*pspdata)->l_local, 0.0) );
  SKIP_FUNC_ON_ERROR( pspio_potential_alloc(&(*pspdata)->vlocal, np) );
  SKIP_FUNC_ON_ERROR( pspio_potential_set(&(*pspdata)->vlocal, qn, (*pspdata)->mesh, vlocal) );

  // Free memory
  free(vlocal);
  pspio_qn_free(&qn);

  // Make sure no error is left unhandled
  RETURN_ON_DEFERRED_ERROR;

  // Check end tag
  SUCCEED_OR_RETURN( upf_tag_check_end(fp,"PP_LOCAL") );

  return PSPIO_SUCCESS;
}


int upf_read_pswfc(FILE *fp, const int np, pspio_pspdata_t **pspdata) {
  char line[PSPIO_STRLEN_LINE];
  int is, ir, i;
  char ll, label[3];
  int nargs, n, l, lmax;
  double occ;
  double *j, *wf;
  double tmp[4];
  pspio_qn_t *qn = NULL;

  // Allocate memory
  SUCCEED_OR_RETURN( pspio_qn_alloc(&qn) );

  wf = (double *) malloc (np * sizeof(double));
  FULFILL_OR_EXIT(wf != NULL, PSPIO_ENOMEM);

  j = (double *) malloc ((*pspdata)->n_states*sizeof(double));
  FULFILL_OR_EXIT(j != NULL, PSPIO_ENOMEM);

  (*pspdata)->states = (pspio_state_t **) malloc ( (*pspdata)->n_states*sizeof(pspio_state_t *));
  FULFILL_OR_EXIT((*pspdata)->states != NULL, PSPIO_ENOMEM);
  for (is=0; is<(*pspdata)->n_states; is++) {
    (*pspdata)->states[is] = NULL;
  }

  // In the case of a fully-relativistic calculation, there is extra
  // information in the ADDINFO tag
  if ( (*pspdata)->wave_eq == PSPIO_EQN_DIRAC ) {
    DEFER_FUNC_ERROR( upf_tag_init(fp,"PP_ADDINFO",GO_BACK) );
    for (is=0; is<(*pspdata)->n_states; is++) {
      FULFILL_OR_BREAK( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
      FULFILL_OR_BREAK( sscanf(line,"%1d%1c %d %d %lf %lf",
        &n, &ll, &i, &l, &j[is], &occ) == 6, PSPIO_EFILE_CORRUPT );
    }
  } else {
    for (is=0; is<(*pspdata)->n_states; is++) j[is] = 0.0;
  }

  // Find init tag
  SKIP_FUNC_ON_ERROR( upf_tag_init(fp,"PP_PSWFC",GO_BACK) );

  // Read states
  lmax = -1;
  l = -1;
  for (is=0; is<(*pspdata)->n_states; is++) {
    // Read the quantum numbers and occupations
    FULFILL_OR_BREAK( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
    FULFILL_OR_BREAK( sscanf(line, "%1d%1c %d %lf", &n, &ll, &l, &occ) == 4, PSPIO_EFILE_CORRUPT );
    FULFILL_OR_BREAK( sprintf(label, "%1d%1c", n, ll) > 0, PSPIO_EIO );
    SUCCEED_OR_BREAK( pspio_qn_set(&qn, n, l, j[is]) );
    lmax = max(l, lmax);

    // Read wavefunction
    for (ir=0; ir<np; ir+=4) {
      FULFILL_OR_BREAK( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
      nargs = sscanf(line, "%lf %lf %lf %lf",
        &tmp[0], &tmp[1], &tmp[2], &tmp[3]);
      FULFILL_OR_BREAK( nargs < 5 && nargs > 0, PSPIO_EFILE_CORRUPT );
      for (i=0; i<nargs; i++) wf[ir+i] = tmp[i];
    }
    BREAK_ON_DEFERRED_ERROR;

    // UPF uses the wavefunctions multiplied by r
    for (ir=0; ir<np; ir++) wf[ir] /= (*pspdata)->mesh->r[ir];

    // Store the state in the pspdata structure
    SUCCEED_OR_BREAK( pspio_state_alloc(&(*pspdata)->states[is], np) );
    SUCCEED_OR_BREAK( pspio_state_set(&(*pspdata)->states[is], 0.0,
      label, qn, occ, 0.0, (*pspdata)->mesh, wf) );
  }
  (*pspdata)->l_max = lmax;

  // Free memory
  free(wf);
  free(j);
  pspio_qn_free(&qn);

  // Make sure no error is left unhandled
  RETURN_ON_DEFERRED_ERROR;

  // Check end tag
  SUCCEED_OR_RETURN( upf_tag_check_end(fp,"PP_PSWFC") );

  return PSPIO_SUCCESS;
}


int upf_read_rhoatom(FILE *fp, const int np, pspio_pspdata_t **pspdata) {
  char line[PSPIO_STRLEN_LINE];
  int i, j, nargs;
  double *rho_read;
  double tmp[4];

  // Find init tag
  SUCCEED_OR_RETURN( upf_tag_init(fp,"PP_RHOATOM",GO_BACK) );

  // Allocate memory
  rho_read = (double *) malloc (np * sizeof(double));
  FULFILL_OR_EXIT(rho_read != NULL, PSPIO_ENOMEM);

  // Read valence density
  for (i=0; i<np; i+=4){
    FULFILL_OR_BREAK( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
    nargs = sscanf(line,"%lf %lf %lf %lf", &tmp[0], &tmp[1],&tmp[2], &tmp[3]);
    FULFILL_OR_BREAK( nargs < 5 && nargs > 0, PSPIO_EFILE_CORRUPT );
    for (j=0; j<nargs; j++) rho_read[i+j] = tmp[j];
  }

  // UPF uses the density multiplied by 4*Pi*r*r
  for (i=0; i<np; i++) {
    rho_read[i] /= 4.0*M_PI*(*pspdata)->mesh->r[i]*(*pspdata)->mesh->r[i];
  }

  // Store the density in the pspdata structure
  SKIP_FUNC_ON_ERROR( pspio_meshfunc_alloc(&(*pspdata)->rho_valence, PSPIO_INTERP_GSL_CSPLINE, np) );
  SKIP_FUNC_ON_ERROR( pspio_meshfunc_set(&(*pspdata)->rho_valence,
    (*pspdata)->mesh, rho_read, NULL, NULL) );

  // Free memory
  free(rho_read);

  // Make sure no error is left unhandled
  RETURN_ON_DEFERRED_ERROR;

  // Check end tag
  SUCCEED_OR_RETURN( upf_tag_check_end(fp,"PP_RHOATOM") );

  return PSPIO_SUCCESS;
}
