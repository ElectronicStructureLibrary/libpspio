/* Copyright (C) 2011-2017 Joseba Alberdi <alberdi@hotmail.es>
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

/** 
 * @file upf_read.c
 * @brief routines to read UPF files 
 */
#include <string.h>

#include "pspio_common.h"
#include "pspio_error.h"
#include "upf.h"
#include "util.h"
#include "pspio.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


int upf_read_info(FILE *fp, pspio_pspdata_t *pspdata)
{
  char line[PSPIO_STRLEN_LINE];
  char *info;
  int il, nlines = 0;

  /* Find init tag */
  SUCCEED_OR_RETURN( upf_tag_init(fp, "PP_INFO", GO_BACK) );

  /* Count how many lines we have */
  while ( upf_tag_check_end(fp, "PP_INFO") != PSPIO_SUCCESS ) {
    nlines += 1;
    FULFILL_OR_RETURN( fp != NULL, PSPIO_EIO );
  }

  /* Go back */
  SUCCEED_OR_RETURN( upf_tag_init(fp,"PP_INFO", GO_BACK) );

  /* Allocate memory */
  SUCCEED_OR_RETURN( pspio_pspinfo_alloc(&pspdata->pspinfo) );
  info = (char *) malloc (1*sizeof(char));
  FULFILL_OR_EXIT(info != NULL, PSPIO_ENOMEM);
  info[0] = '\0';

  /* Store all the lines */
  for (il=0; il<nlines; il++) {
    FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
    info = realloc(info, strlen(info)+strlen(line)+1);
    FULFILL_OR_EXIT(info != NULL, PSPIO_ENOMEM);
    strncat(info, line, strlen(line));
  }
  SUCCEED_OR_RETURN( pspio_pspinfo_set_description(pspdata->pspinfo, info) );
  free(info);

  /* Check end tag */
  SUCCEED_OR_RETURN( upf_tag_check_end(fp,"PP_INFO") );

  return PSPIO_SUCCESS;
}

static int upf_read_header_old(FILE *fp, int *np, pspio_pspdata_t *pspdata)
{
  char line[PSPIO_STRLEN_LINE];
  int version_number, i;
  char symbol[4], nlcc_flag[2], xc_string[23];
  int exchange, correlation, l_max, n_states, n_projectors;
  double z, zvalence, total_energy;
  double wfc_cutoff, rho_cutoff;

  /* Find init tag */
  SUCCEED_OR_RETURN( upf_tag_init(fp,"PP_HEADER", GO_BACK) );

  /* Read the version number */
  FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  FULFILL_OR_RETURN( sscanf(line, "%d", &version_number) == 1,
    PSPIO_EFILE_CORRUPT );
 
  /* Read the atomic symbol */
  FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  strncpy(symbol, strtok(line," "), 3);
  symbol[3] = '\0';
  SUCCEED_OR_RETURN( pspio_pspdata_set_symbol(pspdata, symbol) );
  SUCCEED_OR_RETURN( symbol_to_z(symbol, &z) );
  SUCCEED_OR_RETURN( pspio_pspdata_set_z(pspdata, z) );

  /* Read the kind of pseudo-potentials US|NC|PAW */
  FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  /* At the moment LIBPSP_IO can only read norm-conserving pseudo-potentials */
  FULFILL_OR_RETURN( strncmp(strtok(line," "), "NC", 2) == 0, PSPIO_ENOSUPPORT );

  /* Read the nonlinear core correction */
  FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  FULFILL_OR_RETURN( sscanf(line, "%1s", nlcc_flag) == 1, PSPIO_EFILE_CORRUPT );

  /* Exchange-correlation functional */
  /* Note: the xc string should always contain the first 21 chars of the line */
  FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  strncpy(xc_string, line, 22);
  xc_string[22] = '\0';
  SUCCEED_OR_RETURN( upf_to_libxc(xc_string, &exchange, &correlation) );

  /* Read the Z valence */
  FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  FULFILL_OR_RETURN( sscanf(line, "%lf", &zvalence) == 1, PSPIO_EFILE_CORRUPT );
  SUCCEED_OR_RETURN( pspio_pspdata_set_zvalence(pspdata, zvalence) );

  /* Read the total energy */
  FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  FULFILL_OR_RETURN( sscanf(line, "%lf", &total_energy) == 1, PSPIO_EFILE_CORRUPT );
  SUCCEED_OR_RETURN( pspio_pspdata_set_total_energy(pspdata, total_energy) );

  /* Read the suggested cutoff for wfc and rho */
  FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  FULFILL_OR_RETURN( sscanf(line, "%lf %lf", &wfc_cutoff, &rho_cutoff) == 2, PSPIO_EFILE_CORRUPT );
  
  /* Read the max angular momentun component of the KB projectors */
  FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  FULFILL_OR_RETURN( sscanf(line, "%d", &l_max) == 1, PSPIO_EFILE_CORRUPT );
  SUCCEED_OR_RETURN( pspio_pspdata_set_l_max(pspdata, l_max) );

  /* Read the number of points in mesh */
  FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  FULFILL_OR_RETURN( sscanf(line, "%d", np) == 1, PSPIO_EFILE_CORRUPT );
  
  /* Read the number of wavefunctions and projectors */
  FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  FULFILL_OR_RETURN( sscanf(line, "%d %d", &n_states, &n_projectors) == 2, PSPIO_EFILE_CORRUPT );
  SUCCEED_OR_RETURN( pspio_pspdata_set_n_states(pspdata, n_states) );
  SUCCEED_OR_RETURN( pspio_pspdata_set_n_projectors(pspdata, n_projectors) );

  /* Skip info on wavefunctions, as it is repeated in the PP_PSWFC block */
  for (i=0; i<pspdata->n_states+1; i++) {
    FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  }

  /* Initialize xc */
  SUCCEED_OR_RETURN( pspio_xc_alloc(&pspdata->xc) );
  if (strcmp(nlcc_flag, "T") == 0) {
    SUCCEED_OR_RETURN( pspio_xc_set_nlcc_scheme(pspdata->xc, PSPIO_NLCC_UNKNOWN) );
  } else {
    SUCCEED_OR_RETURN( pspio_xc_set_nlcc_scheme(pspdata->xc, PSPIO_NLCC_NONE) );
  }
  SUCCEED_OR_RETURN( pspio_xc_set_exchange(pspdata->xc, exchange) );
  SUCCEED_OR_RETURN( pspio_xc_set_correlation(pspdata->xc, correlation) );

  /* Check end tag */
  SUCCEED_OR_RETURN( upf_tag_check_end(fp, "PP_HEADER") );

  return PSPIO_SUCCESS;
}

int upf_read_header(FILE *fp, int *np, pspio_pspdata_t *pspdata)
{
  char line[PSPIO_STRLEN_LINE];
  char *attr, *at;
  char symbol[4];
  int exchange, correlation, l_max, n_states, n_projectors;
  double z, zvalence, total_energy;

  attr = upf_tag_read_attr(fp, "PP_HEADER", "pseudo_type", line);
  if (!attr)
    return upf_read_header_old(fp, np, pspdata);
  /* At the moment LIBPSP_IO can only read norm-conserving pseudo-potentials */
  FULFILL_OR_RETURN( strncmp(attr, "NC", 2) == 0, PSPIO_ENOSUPPORT );

  FULFILL_OR_RETURN( attr = upf_tag_read_attr(fp, "PP_HEADER", "element", line),
                     PSPIO_EFILE_CORRUPT);
  SUCCEED_OR_RETURN( pspio_pspdata_set_symbol(pspdata, attr) );
  SUCCEED_OR_RETURN( symbol_to_z(attr, &z) );
  SUCCEED_OR_RETURN( pspio_pspdata_set_z(pspdata, z) );

  FULFILL_OR_RETURN( attr = upf_tag_read_attr(fp, "PP_HEADER", "core_correction", line),
                     PSPIO_EFILE_CORRUPT);
  /* Initialize xc */
  SUCCEED_OR_RETURN( pspio_xc_alloc(&pspdata->xc) );
  if (strcmp(attr, ".T.") == 0 || strcmp(attr, "T") == 0) {
    SUCCEED_OR_RETURN( pspio_xc_set_nlcc_scheme(pspdata->xc, PSPIO_NLCC_UNKNOWN) );
  } else {
    SUCCEED_OR_RETURN( pspio_xc_set_nlcc_scheme(pspdata->xc, PSPIO_NLCC_NONE) );
  }
  FULFILL_OR_RETURN( attr = upf_tag_read_attr(fp, "PP_HEADER", "functional", line),
                     PSPIO_EFILE_CORRUPT);
  at = attr;
  while (*at ++) if (*at == '-') *at = ' ';
  SUCCEED_OR_RETURN( upf_to_libxc(attr, &exchange, &correlation) );
  SUCCEED_OR_RETURN( pspio_xc_set_exchange(pspdata->xc, exchange) );
  SUCCEED_OR_RETURN( pspio_xc_set_correlation(pspdata->xc, correlation) );

  FULFILL_OR_RETURN( attr = upf_tag_read_attr(fp, "PP_HEADER", "z_valence", line),
                     PSPIO_EFILE_CORRUPT);
  FULFILL_OR_RETURN( sscanf(attr, "%lf", &zvalence) == 1, PSPIO_EFILE_CORRUPT );
  SUCCEED_OR_RETURN( pspio_pspdata_set_zvalence(pspdata, zvalence) );

  FULFILL_OR_RETURN( attr = upf_tag_read_attr(fp, "PP_HEADER", "total_psenergy", line),
                     PSPIO_EFILE_CORRUPT);
  FULFILL_OR_RETURN( sscanf(attr, "%lg", &total_energy) == 1, PSPIO_EFILE_CORRUPT );
  SUCCEED_OR_RETURN( pspio_pspdata_set_total_energy(pspdata, total_energy) );

  FULFILL_OR_RETURN( attr = upf_tag_read_attr(fp, "PP_HEADER", "l_max", line),
                     PSPIO_EFILE_CORRUPT);
  FULFILL_OR_RETURN( sscanf(attr, "%d", &l_max) == 1, PSPIO_EFILE_CORRUPT );
  SUCCEED_OR_RETURN( pspio_pspdata_set_l_max(pspdata, l_max) );

  FULFILL_OR_RETURN( attr = upf_tag_read_attr(fp, "PP_HEADER", "mesh_size", line),
                     PSPIO_EFILE_CORRUPT);
  FULFILL_OR_RETURN( sscanf(attr, "%d", np) == 1, PSPIO_EFILE_CORRUPT );

  FULFILL_OR_RETURN( attr = upf_tag_read_attr(fp, "PP_HEADER", "number_of_wfc", line),
                     PSPIO_EFILE_CORRUPT);
  FULFILL_OR_RETURN( sscanf(attr, "%d", &n_states) == 1, PSPIO_EFILE_CORRUPT );
  SUCCEED_OR_RETURN( pspio_pspdata_set_n_states(pspdata, n_states) );
  FULFILL_OR_RETURN( attr = upf_tag_read_attr(fp, "PP_HEADER", "number_of_proj", line),
                     PSPIO_EFILE_CORRUPT);
  FULFILL_OR_RETURN( sscanf(attr, "%d", &n_projectors) == 1, PSPIO_EFILE_CORRUPT );
  SUCCEED_OR_RETURN( pspio_pspdata_set_n_projectors(pspdata, n_projectors) );

  return PSPIO_SUCCESS;
}

int upf_read_mesh(FILE *fp, int np, pspio_pspdata_t *pspdata)
{
  double *r, *drdi;

  /* Find init tag */
  SUCCEED_OR_RETURN( upf_tag_init(fp, "PP_MESH", GO_BACK) );

  /* Allocate memory */
  r = (double *) malloc (np*sizeof(double));
  FULFILL_OR_EXIT(r != NULL, PSPIO_ENOMEM);

  drdi = (double *) malloc (np*sizeof(double));
  FULFILL_OR_EXIT(drdi != NULL, PSPIO_ENOMEM);

  /* Read mesh points */
  /* Note: need to treat some errors without helper macros, for sake of
     feasibility. */
  DEFER_FUNC_ERROR( upf_tag_init(fp, "PP_R", NO_GO_BACK) );
  if ( pspio_error_get_last(__func__) == PSPIO_SUCCESS ) {
    SKIP_FUNC_ON_ERROR( read_array_4by4(fp, r, np) );
    SKIP_FUNC_ON_ERROR( upf_tag_check_end(fp, "PP_R") );
    
    /* Read Rab */
    SKIP_FUNC_ON_ERROR( upf_tag_init(fp, "PP_RAB", NO_GO_BACK) );
    SKIP_FUNC_ON_ERROR( read_array_4by4(fp, drdi, np) );
    SKIP_FUNC_ON_ERROR( upf_tag_check_end(fp, "PP_RAB") );

    /* Store the mesh in the pspdata structure */
    SKIP_FUNC_ON_ERROR( pspio_mesh_alloc(&pspdata->mesh, np) );
    SKIP_CALL_ON_ERROR( pspio_mesh_init_from_points(pspdata->mesh, r, drdi) );
  }

  /* Free memory */
  free(r);
  free(drdi);

  /* Make sure no error is left unhandled */
  RETURN_ON_DEFERRED_ERROR;

  /* Check end tag */
  SUCCEED_OR_RETURN( upf_tag_check_end(fp, "PP_MESH") ); 

  return PSPIO_SUCCESS;
}

int upf_read_nlcc(FILE *fp, int np, pspio_pspdata_t *pspdata)
{
  double *rho;

  /* Find init tag */
  SUCCEED_OR_RETURN( upf_tag_init(fp, "PP_NLCC", GO_BACK) );

  /* Allocate memory */
  rho = (double *) malloc (np*sizeof(double));
  FULFILL_OR_EXIT(rho != NULL, PSPIO_ENOMEM);

  /* Read core rho */
  SKIP_FUNC_ON_ERROR( read_array_4by4(fp, rho, np) );

  /* Store the non-linear core corrections in the pspdata structure */
  SKIP_FUNC_ON_ERROR( pspio_xc_set_nlcc_density(pspdata->xc, pspdata->mesh, rho, NULL, NULL) );

  /* Free memory */
  free(rho);

  /* Make sure no error is left unhandled */
  RETURN_ON_DEFERRED_ERROR;

  /* Check end tag */
  SUCCEED_OR_RETURN( upf_tag_check_end(fp,"PP_NLCC") );

  return PSPIO_SUCCESS;
}

static int upf_read_dij_old(FILE *fp, pspio_pspdata_t *pspdata)
{
  char line[PSPIO_STRLEN_LINE];
  int i, n_dij, ii, jj;
  double *dij, energy;

  /* Find init tag */
  SUCCEED_OR_RETURN( upf_tag_init(fp,"PP_NONLOCAL",GO_BACK) );

  /* We start by reading the KB energies, as it is more convinient this way */
  SUCCEED_OR_RETURN( upf_tag_init(fp,"PP_DIJ",NO_GO_BACK) );

  /* Read the number of n_dij */
  FULFILL_OR_EXIT( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  FULFILL_OR_EXIT( sscanf(line,"%d", &n_dij) == 1, PSPIO_EFILE_CORRUPT );
  FULFILL_OR_EXIT( n_dij == pspdata->n_projectors, PSPIO_EFILE_CORRUPT );

  dij = (double *) malloc (n_dij * n_dij * sizeof(double));
  memset(dij, '\0', n_dij * n_dij * sizeof(double));
  FULFILL_OR_EXIT(dij != NULL, PSPIO_ENOMEM);

  for (i=0; i<n_dij; i++){
    FULFILL_OR_BREAK( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
    FULFILL_OR_BREAK( sscanf(line,"%d %d %lf", &ii, &jj, &energy) == 3,
      PSPIO_EFILE_CORRUPT );
    FULFILL_OR_BREAK( ii == jj, PSPIO_EVALUE );
    dij[(ii - 1) * n_dij + jj - 1] = energy*2.0;
  }
  SKIP_FUNC_ON_ERROR( pspio_pspdata_set_projector_energies(pspdata, dij) );

  /* Check end tag */
  SKIP_FUNC_ON_ERROR( upf_tag_check_end(fp,"PP_DIJ") );
  SKIP_FUNC_ON_ERROR( upf_tag_check_end(fp,"PP_NONLOCAL") );

  free(dij);

  RETURN_ON_DEFERRED_ERROR;

  return PSPIO_SUCCESS;
}

static int upf_read_dij(FILE *fp, pspio_pspdata_t *pspdata)
{
  char line[PSPIO_STRLEN_LINE];
  char *attr;
  int i, n_dij;
  double *dij;

  attr = upf_tag_read_attr(fp, "PP_DIJ", "size", line);
  if (!attr)
    return upf_read_dij_old(fp, pspdata);
  FULFILL_OR_EXIT( sscanf(attr,"%d", &n_dij) == 1, PSPIO_EFILE_CORRUPT );
  FULFILL_OR_EXIT( n_dij == pspdata->n_projectors * pspdata->n_projectors, PSPIO_EFILE_CORRUPT );

  dij = (double *) malloc (n_dij * sizeof(double));
  FULFILL_OR_EXIT(dij != NULL, PSPIO_ENOMEM);

  SKIP_FUNC_ON_ERROR( upf_tag_init(fp,"PP_NONLOCAL",GO_BACK) );
  SKIP_FUNC_ON_ERROR( upf_tag_init(fp,"PP_DIJ",NO_GO_BACK) );

  /* Read the projector function */
  SKIP_FUNC_ON_ERROR( read_array_4by4(fp, dij, n_dij) );

  SKIP_FUNC_ON_ERROR( upf_tag_check_end(fp,"PP_DIJ") );
  SKIP_FUNC_ON_ERROR( upf_tag_check_end(fp,"PP_NONLOCAL") );

  for (i = 0; i < n_dij; i++)
    dij[i] *= 2.;
  SKIP_FUNC_ON_ERROR( pspio_pspdata_set_projector_energies(pspdata, dij) );

  free(dij);

  RETURN_ON_DEFERRED_ERROR;

  return PSPIO_SUCCESS;
}

static int upf_read_beta_old(FILE *fp, int np, int index, double *projector_read, int *l)
{
  char line[PSPIO_STRLEN_LINE];
  int ii, proj_np, j;

  SUCCEED_OR_RETURN( upf_tag_init(fp, "PP_BETA", NO_GO_BACK) );

  FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  FULFILL_OR_RETURN( sscanf(line, "%d %d", &ii, l) == 2,
                     PSPIO_EFILE_CORRUPT );

  /* Read the number of points of projections */
  FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  FULFILL_OR_RETURN( sscanf(line, "%d", &proj_np) == 1, PSPIO_EFILE_CORRUPT );

  /* Read the projector function */
  SUCCEED_OR_RETURN( read_array_4by4(fp, projector_read, (proj_np < np) ? proj_np : np) );

  /* Fill with zeros, if any left */
  for (j=proj_np; j<np; j++) projector_read[j] = 0.0;

  /* Check end tag */
  if (upf_tag_check_end(fp,"PP_BETA") != PSPIO_SUCCESS) {
    /*
      We may have a "new" version of the format with two extra lines at
      the end of this block, so we will try to skip them. Note that one
      line was already read by upf_tag_check_end.
    */
    FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL,
                       PSPIO_EIO );
    SUCCEED_OR_RETURN( upf_tag_check_end(fp,"PP_BETA") );
  }

  return PSPIO_SUCCESS;
}

static int upf_read_beta(FILE *fp, int np, int index, double *projector_read, int *l)
{
  char line[PSPIO_STRLEN_LINE];
  char element[12];
  char *attr;
  int ii, proj_np, j;
  long at;

  /* Store position because read_attr is rewinding. */
  at = ftell(fp);

  sprintf(element, "PP_BETA.%d", index + 1);
  attr = upf_tag_read_attr(fp, element, "size", line);
  if (!attr) {
    fseek(fp, at, SEEK_SET);
    return upf_read_beta_old(fp, np, index, projector_read, l);
  }
  FULFILL_OR_RETURN( sscanf(attr, "%d", &proj_np) == 1, PSPIO_EFILE_CORRUPT );

  FULFILL_OR_RETURN( attr = upf_tag_read_attr(fp, element, "angular_momentum", line), PSPIO_EFILE_CORRUPT );
  FULFILL_OR_RETURN( sscanf(attr, "%d", l) == 1, PSPIO_EFILE_CORRUPT );

  /* Read the projector function */
  SUCCEED_OR_RETURN( read_array_4by4(fp, projector_read, (proj_np < np) ? proj_np : np) );

  /* Fill with zeros, if any left */
  for (j=proj_np; j<np; j++) projector_read[j] = 0.0;

  /* Check end tag */
  SUCCEED_OR_RETURN( upf_tag_check_end(fp, element) );

  return PSPIO_SUCCESS;
}

int upf_read_nonlocal(FILE *fp, int np, pspio_pspdata_t *pspdata)
{
  char line[PSPIO_STRLEN_LINE];
  int proj_np = 0;
  int i, j, l, ii, jj;
  double *proj_j;
  double *projector_read;
  pspio_qn_t qn;

  /* Allocate memory */
  proj_j = (double *) malloc (pspdata->n_projectors * sizeof(double));
  FULFILL_OR_EXIT(proj_j != NULL, PSPIO_ENOMEM);

  projector_read = (double *) malloc (np * sizeof(double));
  FULFILL_OR_EXIT(projector_read != NULL, PSPIO_ENOMEM);

  /* In the case of a fully-relativistic calculation, there is 
     extra information in the ADDINFO tag */
  if (pspdata->wave_eq == PSPIO_EQN_DIRAC) {
    SKIP_FUNC_ON_ERROR( upf_tag_init(fp,"PP_ADDINFO",GO_BACK) );
    /* Skip the lines with the wavefunctions info */
    for (i=0; i<pspdata->n_states; i++) {
      FULFILL_OR_BREAK( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
    }
    /* Read j quantum numbers */
    for (i=0; i<pspdata->n_projectors; i++) {
      FULFILL_OR_BREAK( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
      FULFILL_OR_BREAK( sscanf(line,"%d %lf",&j, &proj_j[i]) == 2, PSPIO_EFILE_CORRUPT );
    }
  } else {
    for (i=0; i<pspdata->n_projectors; i++) proj_j[i] = 0.0;
  }

  /* Now we go back and read the projector functions */
  SKIP_FUNC_ON_ERROR( upf_tag_init(fp, "PP_NONLOCAL", GO_BACK) );

  for (i=0; i<pspdata->n_projectors; i++){
    SUCCEED_OR_BREAK( upf_read_beta(fp, np, i, projector_read, &l) );

    /* Convert units */
    for (j=0; j<np; j++) projector_read[j] /= 2.0*pspdata->mesh->r[j];

    /* Store the projectors in the pspdata structure */
    SUCCEED_OR_BREAK( pspio_qn_init(&qn, 0, l, proj_j[i]) );
    SUCCEED_OR_BREAK(
      pspio_projector_alloc( &(pspdata->projectors[i]), np) );
    SUCCEED_OR_BREAK( pspio_projector_init(pspdata->projectors[i], &qn, pspdata->mesh, projector_read) );
  }

  SKIP_FUNC_ON_ERROR( upf_read_dij(fp, pspdata) );

  /* Free memory */
  free(projector_read);
  free(proj_j);

  /* Make sure no error is left unhandled */
  RETURN_ON_DEFERRED_ERROR;

  return PSPIO_SUCCESS;
}

int upf_read_local(FILE *fp, int np, pspio_pspdata_t *pspdata)
{
  int i, j, n;
  double *vlocal;
  pspio_qn_t *qn = NULL;

  /* Find init tag */
  SUCCEED_OR_RETURN( upf_tag_init(fp, "PP_LOCAL",GO_BACK) );

  /* Allocate memory */
  SUCCEED_OR_RETURN( pspio_qn_alloc(&qn) );

  vlocal = (double *) malloc (np*sizeof(double));
  FULFILL_OR_EXIT(vlocal != NULL, PSPIO_ENOMEM);
  for (i=0; i<np; i++) vlocal[i] = 0.0;

  /* Deduce l local (this is done in a very crude way and it should
     probably be made more robust) */
  pspdata->l_local = -1;
  for (i=0; i<pspdata->l_max+1; i++) {
    n = 0;
    for (j=0; j<pspdata->n_projectors; j++) {
      if ( pspio_qn_get_l(pspio_projector_get_qn(pspdata->projectors[j])) == i ) n++;
    }
    if ( n == 0 ) pspdata->l_local = i;
  }

  /* Read local potential */
  SKIP_FUNC_ON_ERROR( read_array_4by4(fp, vlocal, np) );

  /* Convert from Rydbergs */
  for (i=0; i<np; i++) vlocal[i] /= 2.0;

  /* Store the local potential in the pspdata structure */
  SKIP_FUNC_ON_ERROR( pspio_qn_init(qn, 0, pspdata->l_local, 0.0) );
  SKIP_FUNC_ON_ERROR( pspio_potential_alloc(&pspdata->vlocal, np) );
  SKIP_FUNC_ON_ERROR( pspio_potential_init(pspdata->vlocal, qn, pspdata->mesh, vlocal) );

  /* Free memory */
  free(vlocal);
  pspio_qn_free(qn);

  /* Make sure no error is left unhandled */
  RETURN_ON_DEFERRED_ERROR;

  /* Check end tag */
  SUCCEED_OR_RETURN( upf_tag_check_end(fp,"PP_LOCAL") );

  return PSPIO_SUCCESS;
}

static int upf_read_pswfc_one_old(FILE *fp, int np, int index, double *wf,
                                  char label[3], double *occ, int *n, int *l)
{
  char line[PSPIO_STRLEN_LINE];
  int nargs;
  char ll;

  /* Read the quantum numbers and occupations */
  FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  // TODO: ask QE developers whether they still accept the old format
  //       like in the He pseudo
  //FULFILL_OR_BREAK( sscanf(line, "%1d%1c %d %lf", &n, &ll, &l, &occ) == 4, PSPIO_EFILE_CORRUPT );
  nargs = sscanf(line, "%1d%1c %d %lf", n, &ll, l, occ);
  if ( nargs != 4 ) {
    *n = 1;
    nargs = sscanf(line, "%1c %d %lf", &ll, l, occ);
    FULFILL_OR_RETURN( nargs == 3, PSPIO_EFILE_CORRUPT );
  }
  FULFILL_OR_RETURN( sprintf(label, "%1d%1c", *n, ll) > 0, PSPIO_EIO );

  /* Read wavefunction */
  SUCCEED_OR_RETURN( read_array_4by4(fp, wf, np) );

  return PSPIO_SUCCESS;
}

static int upf_read_pswfc_one(FILE *fp, int np, int index, double *wf,
                              char label[3], double *occ, int *n, int *l)
{
  char line[PSPIO_STRLEN_LINE];
  char *attr;
  char element[10];
  long at;
  int wf_np;
  char ll;

  /* Store position because read_attr is rewinding. */
  at = ftell(fp);

  sprintf(element, "PP_CHI.%d", index + 1);
  attr = upf_tag_read_attr(fp, element, "size", line);
  if (!attr) {
    fseek(fp, at, SEEK_SET);
    return upf_read_pswfc_one_old(fp, np, index, wf, label, occ, n, l);
  }
  FULFILL_OR_RETURN( sscanf(attr, "%d", &wf_np) == 1, PSPIO_EFILE_CORRUPT );

  FULFILL_OR_RETURN( attr = upf_tag_read_attr(fp, element, "label", line), PSPIO_EFILE_CORRUPT );
  label[0] = attr[0], label[1] = attr[1], label[2] = '\0';

  FULFILL_OR_RETURN( attr = upf_tag_read_attr(fp, element, "occupation", line), PSPIO_EFILE_CORRUPT );
  FULFILL_OR_RETURN( sscanf(attr, "%lf", occ) == 1, PSPIO_EFILE_CORRUPT );

  FULFILL_OR_RETURN( attr = upf_tag_read_attr(fp, element, "n", line), PSPIO_EFILE_CORRUPT );
  FULFILL_OR_RETURN( sscanf(attr, "%d", n) == 1, PSPIO_EFILE_CORRUPT );

  FULFILL_OR_RETURN( attr = upf_tag_read_attr(fp, element, "l", line), PSPIO_EFILE_CORRUPT );
  FULFILL_OR_RETURN( sscanf(attr, "%d", l) == 1, PSPIO_EFILE_CORRUPT );

  /* Read wavefunction */
  SUCCEED_OR_RETURN( read_array_4by4(fp, wf, np) );

  /* Check end tag */
  SUCCEED_OR_RETURN( upf_tag_check_end(fp, element) );

  return PSPIO_SUCCESS;
}

int upf_read_pswfc(FILE *fp, int np, pspio_pspdata_t *pspdata)
{
  char line[PSPIO_STRLEN_LINE];
  int is, ir, i;
  char ll, label[3];
  int nargs, n, l, lmax;
  double occ;
  double *j, *wf;
  pspio_qn_t *qn = NULL;

  /* Allocate memory */
  SUCCEED_OR_RETURN( pspio_qn_alloc(&qn) );

  wf = (double *) malloc (np * sizeof(double));
  FULFILL_OR_EXIT(wf != NULL, PSPIO_ENOMEM);

  j = (double *) malloc (pspdata->n_states*sizeof(double));
  FULFILL_OR_EXIT(j != NULL, PSPIO_ENOMEM);

  /* In the case of a fully-relativistic calculation, there is extra
     information in the ADDINFO tag */
  if ( pspdata->wave_eq == PSPIO_EQN_DIRAC ) {
    DEFER_FUNC_ERROR( upf_tag_init(fp,"PP_ADDINFO",GO_BACK) );
    for (is=0; is<pspdata->n_states; is++) {
      FULFILL_OR_BREAK( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
      FULFILL_OR_BREAK( sscanf(line,"%1d%1c %d %d %lf %lf",
        &n, &ll, &i, &l, &j[is], &occ) == 6, PSPIO_EFILE_CORRUPT );
    }
  } else {
    for (is=0; is<pspdata->n_states; is++) j[is] = 0.0;
  }

  /* Find init tag */
  SKIP_FUNC_ON_ERROR( upf_tag_init(fp,"PP_PSWFC",GO_BACK) );

  /* Read states */
  pspdata->nelvalence = 0.0;
  lmax = -1;
  l = -1;
  for (is=0; is<pspdata->n_states; is++) {
    SUCCEED_OR_BREAK( upf_read_pswfc_one(fp, np, is, wf, label, &occ, &n, &l) );
    SUCCEED_OR_BREAK( pspio_qn_init(qn, n, l, j[is]) );
    lmax = max(l, lmax);

    /* UPF uses the wavefunctions multiplied by r */
    for (ir=0; ir<np; ir++) wf[ir] /= pspdata->mesh->r[ir];

    /* Store the state in the pspdata structure */
    SUCCEED_OR_BREAK( pspio_state_alloc(&pspdata->states[is], np) );
    SUCCEED_OR_BREAK( pspio_state_init(pspdata->states[is], 0.0,
      qn, occ, 0.0, pspdata->mesh, wf, label) );
    pspdata->nelvalence += occ;
  }
  pspdata->l_max = lmax;

  /* Free memory */
  free(wf);
  free(j);
  pspio_qn_free(qn);

  /* Make sure no error is left unhandled */
  RETURN_ON_DEFERRED_ERROR;

  /* Check end tag */
  SUCCEED_OR_RETURN( upf_tag_check_end(fp,"PP_PSWFC") );

  return PSPIO_SUCCESS;
}

int upf_read_rhoatom(FILE *fp, int np, pspio_pspdata_t *pspdata)
{
  int i;
  double *rho_read;

  /* Find init tag */
  SUCCEED_OR_RETURN( upf_tag_init(fp,"PP_RHOATOM",GO_BACK) );

  /* Allocate memory */
  rho_read = (double *) malloc (np * sizeof(double));
  FULFILL_OR_EXIT(rho_read != NULL, PSPIO_ENOMEM);

  /* Read valence density */
  SKIP_FUNC_ON_ERROR( read_array_4by4(fp, rho_read, np) );

  /* UPF uses the density multiplied by 4*Pi*r*r */
  for (i=0; i<np; i++) {
    rho_read[i] /= 4.0*M_PI*pspdata->mesh->r[i]*pspdata->mesh->r[i];
  }

  /* Store the density in the pspdata structure */
  SKIP_FUNC_ON_ERROR( pspio_meshfunc_alloc(&pspdata->rho_valence, np) );
  SKIP_FUNC_ON_ERROR( pspio_meshfunc_init(pspdata->rho_valence,
    pspdata->mesh, rho_read, NULL, NULL) );

  /* Free memory */
  free(rho_read);

  /* Make sure no error is left unhandled */
  RETURN_ON_DEFERRED_ERROR;

  /* Check end tag */
  SUCCEED_OR_RETURN( upf_tag_check_end(fp,"PP_RHOATOM") );

  return PSPIO_SUCCESS;
}
