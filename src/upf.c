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

/** 
 * @file read_upf.c
 * @brief implementation to read in UPF files 
 */
#include <string.h>
#include <ctype.h>
#include "upf.h"
#include "util.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


int pspio_upf_init(FILE *fp, pspio_pspdata_t **pspdata){
 
  HANDLE_FUNC_ERROR(pspio_upf_file_read(fp, pspdata));

  return PSPIO_SUCCESS;
}

int pspio_upf_file_read(FILE *fp, pspio_pspdata_t **pspdata){
  char line[MAX_STRLEN];
  int i, j, narg;
  pspio_qn_t *qn = NULL;
  HANDLE_FUNC_ERROR(pspio_qn_alloc(&qn));


  /****************************************************************************
   *                                 HEADER                                   *
   ****************************************************************************/
  int version_number;
  char *kind_ps;
  char nlcc_flag;
  char xc_string[MAX_STRLEN];
  int id_left;
  int exchange, correlation;
  double total_energy, wfc_cutoff, rho_cutoff;
  int np;
  char label[2];
  int l;

  HANDLE_FUNC_ERROR(init_tag(fp,"PP_HEADER", GO_BACK));
  
  //Read the version number
  if(fgets(line, MAX_STRLEN, fp) == NULL) {
    HANDLE_ERROR(PSPIO_EIO);
  }
  narg = sscanf (line, "%d", &version_number);
  ASSERT(narg==1, PSPIO_EIO);
 
  //Read the atomic symbol
  if(fgets(line, MAX_STRLEN, fp) == NULL) {
    HANDLE_ERROR(PSPIO_EIO);
  }
  (*pspdata)->symbol = (char *)malloc(2*sizeof(char));
  HANDLE_FATAL_ERROR((*pspdata)->symbol == NULL,PSPIO_ENOMEM);
  strcpy((*pspdata)->symbol, strtok(line," "));
  HANDLE_FUNC_ERROR( symbol_to_z((*pspdata)->symbol, (*pspdata)->z) );

  //Read the kind of pseudo-potentials US|NC|PAW
  if(fgets(line, MAX_STRLEN, fp) == NULL) {
    HANDLE_ERROR(PSPIO_EIO);
  }
  kind_ps = (char *)malloc(3*sizeof(char));
  HANDLE_FATAL_ERROR(kind_ps == NULL, PSPIO_ENOMEM);
  kind_ps = strtok(line," ");
  if (strncmp(kind_ps,"NC",2)) {
    //At the moment LIBPSP_IO can only read norm-conserving pseudo-potentials
    HANDLE_ERROR(PSPIO_ENOSUPPORT);
  }
  
  // read the nonlinear core correction
  if(fgets(line, MAX_STRLEN, fp) == NULL) {
    HANDLE_ERROR(PSPIO_EIO);
  }
  narg = sscanf (line, "%1s", &nlcc_flag);
  ASSERT(narg==1, PSPIO_EIO);

  //Exchange-correlation functional
  if(fgets(line, MAX_STRLEN, fp) == NULL) {
    HANDLE_ERROR(PSPIO_EIO);
  }
  strncpy(xc_string, line, 21); //the xc string should always the first 21 chars of the line
  HANDLE_FUNC_ERROR (upf_to_libxc(xc_string, &exchange, &correlation));

  // read the Z valence
  if(fgets(line, MAX_STRLEN, fp) == NULL) {
    HANDLE_ERROR(PSPIO_EIO);
  }
  narg = sscanf (line, "%lf",&(*pspdata)->zvalence);
  ASSERT(narg==1, PSPIO_EIO);

  // read the total energy
  if(fgets(line, MAX_STRLEN, fp) == NULL) {
    HANDLE_ERROR(PSPIO_EIO);
  }
  narg = sscanf (line, "%lf",&total_energy);
  ASSERT(narg==1, PSPIO_EIO);
  
  //read the suggested cutoff for wfc and rho
  if(fgets(line, MAX_STRLEN, fp) == NULL) {
    HANDLE_ERROR(PSPIO_EIO);
  }
  narg = sscanf (line, "%lf %lf",&wfc_cutoff,&rho_cutoff);
  ASSERT(narg==2, PSPIO_EIO);
  
  // read the max angular momentun component
  if(fgets(line, MAX_STRLEN, fp) == NULL) {
    HANDLE_ERROR(PSPIO_EIO);
  }
  narg = sscanf (line, "%d",&(*pspdata)->l_max);
  ASSERT(narg==1, PSPIO_EIO);
  
  // read the number of points in mesh
  if(fgets(line, MAX_STRLEN, fp) == NULL) {
    HANDLE_ERROR(PSPIO_EIO);
  }
  narg = sscanf (line, "%d",&np);
  ASSERT(narg==1, PSPIO_EIO);
  
  // read the number of wavefunctions and projectors 
  if(fgets(line, MAX_STRLEN, fp) == NULL) {
    HANDLE_ERROR(PSPIO_EIO);
  }
  narg = sscanf (line, "%d %d", &(*pspdata)->n_states, &(*pspdata)->n_kbproj);
  ASSERT(narg==2, PSPIO_EIO);

  //Skip info on wavefunctions, as it is repeated in the PP_PSWFC block
  for (i=0; i<(*pspdata)->n_states+1; i++) {
    if(fgets(line, MAX_STRLEN, fp) == NULL) {
      HANDLE_ERROR(PSPIO_EIO);
    }
  }
  HANDLE_FUNC_ERROR(check_end_tag(fp, "PP_HEADER"));
  
  // Initialize xc
  HANDLE_FUNC_ERROR(pspio_xc_alloc(&(*pspdata)->xc, PSPIO_NLCC_UNKNOWN, np));
  HANDLE_FUNC_ERROR(pspio_xc_set(&(*pspdata)->xc, exchange, correlation));


  /****************************************************************************
   *                                  MESH                                    *
   ****************************************************************************/
  double *r, *drdi;

  //Allocate memory
  r = (double *)malloc(np*sizeof(double));
  HANDLE_FATAL_ERROR(r == NULL,PSPIO_ENOMEM);

  drdi = (double *)malloc(np*sizeof(double));
  HANDLE_FATAL_ERROR(drdi == NULL,PSPIO_ENOMEM);


  HANDLE_FUNC_ERROR(init_tag(fp, "PP_MESH", GO_BACK));
  HANDLE_FUNC_ERROR(init_tag(fp, "PP_R", NO_GO_BACK));

  for (i=0; i<np; i+=4) {
    if(fgets(line, MAX_STRLEN, fp) == NULL) {
      HANDLE_ERROR(PSPIO_EIO);
    }
    narg = sscanf(line,"%lf %lf %lf %lf",&r[i],&r[i+1],&r[i+2],&r[i+3]);
    ASSERT(narg==4, PSPIO_EIO);
  }
  HANDLE_FUNC_ERROR(check_end_tag(fp, "PP_R"));
  
  HANDLE_FUNC_ERROR(init_tag(fp, "PP_RAB", NO_GO_BACK));
  
  for (i=0; i<np; i+=4) {
    if (fgets(line, MAX_STRLEN, fp) == NULL) {
      HANDLE_ERROR(PSPIO_EIO);
    }
    narg = sscanf(line,"%lf %lf %lf %lf",&drdi[i],&drdi[i+1],&drdi[i+2],&drdi[i+3]);
    ASSERT(narg==4, PSPIO_EIO);
  }                                                
  HANDLE_FUNC_ERROR(check_end_tag(fp, "PP_RAB"));
  HANDLE_FUNC_ERROR(check_end_tag(fp, "PP_MESH")); 

  //Store the mesh in the pspdata structure
  HANDLE_FUNC_ERROR(pspio_mesh_alloc(&(*pspdata)->mesh, np));
  HANDLE_FUNC_ERROR(pspio_mesh_init_from_points(&(*pspdata)->mesh, r, drdi));

  free(r);
  free(drdi);


  /****************************************************************************
   *                                 PP_NLCC                                  *
   ****************************************************************************/
  double *core_density;

  if (strcmp(&nlcc_flag, "F") == 0) {
    //No core corrections
    core_density = NULL;
  } else {

    //Allocate memory
    core_density = (double *)malloc(np*sizeof(double));
    HANDLE_FATAL_ERROR(core_density == NULL,PSPIO_ENOMEM);

    HANDLE_FUNC_ERROR(init_tag(fp, "PP_NLCC", GO_BACK));
    for (i=0; i<np; i+=4) {
      if (fgets(line, MAX_STRLEN, fp) == NULL) {
	HANDLE_ERROR(PSPIO_EIO);
      }
      narg = sscanf(line,"%lf %lf %lf %lf",&core_density[i],&core_density[i+1],&core_density[i+2],&core_density[i+3]);
      ASSERT(narg==4, PSPIO_EIO);
    }
    HANDLE_FUNC_ERROR(check_end_tag(fp,"PP_NLCC"));

    //Store the non-linear core corrections in the pspdata structure
    HANDLE_FUNC_ERROR(pspio_xc_nlcc_set(&(*pspdata)->xc, (*pspdata)->mesh, core_density));

    free(core_density);
  }


  /****************************************************************************
   *                                PP_ADDINFO                                *
   *                                                                          *
   * PP_ADDINFO should be read before PP_NONLOCAL and PP_PSWFC                *
   ****************************************************************************/
  double * proj_j_read;

  /*
  //tag_isdef is not error checked!!
  if (tag_isdef(fp,"PP_ADDINFO")){
    kb_nc = 2;
    //I don't know if has to be done (I guess the pointer is already there):
    HANDLE_FUNC_ERROR(init_tag(fp,"PP_ADDINFO",GO_BACK));
    for (i=0; i<(*pspdata)->n_states; i++){
      //dummy

      if (fgets(line, MAX_STRLEN, fp) == NULL) {
	HANDLE_ERROR(PSPIO_EIO);
      }
    }
    //Allocate memory for the reading of projection j
    proj_j_read = (double *)malloc(n_proj * sizeof(double *));
    HANDLE_FATAL_ERROR(proj_j_read == NULL,PSPIO_ENOMEM);
    for (i=0;i<n_proj;i++){
      if (fgets(line, MAX_STRLEN, fp) == NULL) {
	HANDLE_ERROR(PSPIO_EIO);
      }
      narg == sscanf(line,"%d %lf",&dummy,&proj_j_read[i]);
      ASSERT(narg==2, PSPIO_EIO); 
    }  
    
    //Skip the last line
    if (fgets(line, MAX_STRLEN, fp) == NULL) {
      HANDLE_ERROR(PSPIO_EIO);
    }
  }
  HANDLE_FUNC_ERROR(check_end_tag(fp,"PP_ADDINFO"));

  free(proj_j);
  */


  /****************************************************************************
   *                              PP_NONLOCAL                                 *
   ****************************************************************************/  
  int proj_np, n_dij, ii, jj;
  double energy;
  double *ekb, *projector_read;

  //Allocate memory
  projector_read = (double *)malloc(np * sizeof(double));
  HANDLE_FATAL_ERROR(projector_read == NULL, PSPIO_ENOMEM);

  (*pspdata)->kb_projectors = (pspio_projector_t **)malloc( (*pspdata)->n_kbproj*sizeof(pspio_projector_t *));
  HANDLE_FATAL_ERROR( (*pspdata)->kb_projectors == NULL, PSPIO_ENOMEM);
  for (i=0; i<(*pspdata)->n_kbproj; i++) (*pspdata)->kb_projectors[i] = NULL;

  ekb = (double *)malloc((*pspdata)->n_kbproj*sizeof(double));
  HANDLE_FATAL_ERROR(ekb == NULL, PSPIO_ENOMEM);
  memset(ekb, 0, (*pspdata)->n_kbproj*sizeof(double));

  // We start by reading the KB energies, as it is more convinient this way
  HANDLE_FUNC_ERROR(init_tag(fp,"PP_NONLOCAL",GO_BACK));
  HANDLE_FUNC_ERROR(init_tag(fp,"PP_DIJ",NO_GO_BACK));

  //Read the number of n_dij
  if (fgets(line, MAX_STRLEN, fp) == NULL) {
    HANDLE_ERROR(PSPIO_EIO);
  }
  narg = sscanf(line,"%d",&n_dij);
  ASSERT(narg==1, PSPIO_EIO);

  for (i=0; i<n_dij; i++){
    if (fgets(line, MAX_STRLEN, fp) == NULL) {
      HANDLE_ERROR(PSPIO_EIO);
    }
    narg = sscanf(line,"%d %d %lf", &ii, &jj, &energy);
    ASSERT(narg==3, PSPIO_EIO);
    if (ii != jj) {
      HANDLE_ERROR(PSPIO_EVALUE);
    }
    ekb[ii-1] = energy;

  }
  HANDLE_FUNC_ERROR(check_end_tag(fp,"PP_DIJ"));
  HANDLE_FUNC_ERROR(check_end_tag(fp,"PP_NONLOCAL"));


  // Now we go back and read the projector functions
  HANDLE_FUNC_ERROR(init_tag(fp,"PP_NONLOCAL",GO_BACK));

  for (i=0; i<(*pspdata)->n_kbproj; i++){

    HANDLE_FUNC_ERROR(init_tag(fp,"PP_BETA",NO_GO_BACK));

    if(fgets(line, MAX_STRLEN, fp) == NULL) {
      HANDLE_ERROR(PSPIO_EIO);
    }
    narg = sscanf(line,"%d %d", &ii, &l);
    ASSERT(narg==2, PSPIO_EIO);

    //Read the number of points of projections
    if(fgets(line, MAX_STRLEN, fp) == NULL) {
      HANDLE_ERROR(PSPIO_EIO);
    }
    narg = sscanf(line,"%d",&proj_np);
    ASSERT(narg==1, PSPIO_EIO);
    
    //Read the projector function
    for (j=0; j<proj_np; j+=4){
      if (fgets(line, MAX_STRLEN, fp) == NULL) {
	HANDLE_ERROR(PSPIO_EIO);
      }
      narg = sscanf(line,"%lf %lf %lf %lf",&projector_read[j],&projector_read[j+1],&projector_read[j+2],&projector_read[j+3]);
      ASSERT(narg==4, PSPIO_EIO);
    }
    //Fill with zeros, if any left
    for (j=proj_np; j<np; j++) projector_read[i] = 0.0;

    //Store the projectors in the pspdata structure
    HANDLE_FUNC_ERROR(pspio_qn_set(&qn, 0, l, 0.0));
    HANDLE_FUNC_ERROR(pspio_projector_alloc( &((*pspdata)->kb_projectors[i]), np));
    HANDLE_FUNC_ERROR(pspio_projector_set( &(*pspdata)->kb_projectors[i], qn, ekb[i], (*pspdata)->mesh, projector_read));

  }

  free(projector_read);
  free(ekb);


  /****************************************************************************
   *                                PP_LOCAL                                  *
   *                                                                          *
   * PP_LOCAL has to be read after PP_NONLOCAL because we will only be able   *  
   * to deduce the value of l_local from the data stored in PP_NONLOCAL       *
   ****************************************************************************/
  double * vlocal_read;
  int n, l_local;

  HANDLE_FUNC_ERROR(init_tag(fp, "PP_LOCAL",GO_BACK));

  //Allocate memory
  vlocal_read = (double *)malloc(np*sizeof(double));
  HANDLE_FATAL_ERROR(vlocal_read == NULL,PSPIO_ENOMEM);

  //Deduce l local (this is done in a very crude way and it should probably be made more robust)
  l_local = -1;
  for (i=0; i<(*pspdata)->l_max+1; i++) {
    n = 0;
    for (j=0; j<(*pspdata)->n_kbproj; j++) {
      HANDLE_FUNC_ERROR(pspio_projector_l( (*pspdata)->kb_projectors[j], &l));
      if (l == i) n++;
    }
    if (n == 0) l_local = i;
  }

  //Read local potential
  for (i=0; i<np; i+=4) {
    if (fgets(line, MAX_STRLEN, fp) == NULL) {
      HANDLE_ERROR(PSPIO_EIO);
    }
    narg = sscanf(line,"%lf %lf %lf %lf",&vlocal_read[i],&vlocal_read[i+1],&vlocal_read[i+2],&vlocal_read[i+3]);
    ASSERT(narg==4, PSPIO_EIO);
  }
  HANDLE_FUNC_ERROR(check_end_tag(fp,"PP_LOCAL"));

  //Store the local potential in the pspdata structure
  HANDLE_FUNC_ERROR(pspio_qn_set(&qn, 0, (*pspdata)->l_local, 0.0));
  HANDLE_FUNC_ERROR(pspio_potential_alloc(&(*pspdata)->vlocal, np));
  HANDLE_FUNC_ERROR(pspio_potential_set(&(*pspdata)->vlocal, qn, (*pspdata)->mesh, vlocal_read));



  /****************************************************************************
   *                                PP_PSWFC                                  *
   ****************************************************************************/
  double occ;
  double *wf;

  //Allocate memory
  wf = (double *)malloc(np * sizeof(double));
  HANDLE_FATAL_ERROR(wf == NULL, PSPIO_ENOMEM);

  (*pspdata)->states = (pspio_state_t **)malloc( (*pspdata)->n_states*sizeof(pspio_state_t *));
  HANDLE_FATAL_ERROR( (*pspdata)->states == NULL, PSPIO_ENOMEM);
  for (j=0; j<(*pspdata)->n_states; j++) (*pspdata)->states[j] = NULL;

  HANDLE_FUNC_ERROR(init_tag(fp,"PP_PSWFC",GO_BACK));
  for (j=0; j<(*pspdata)->n_states; j++) {

    //Read the quantum numbers and occupations
    if(fgets(line, MAX_STRLEN, fp) == NULL) {
      HANDLE_ERROR(PSPIO_EIO);
    }
    narg = sscanf(line, "%2c %d %lf", &label[0], &l, &occ);
    ASSERT(narg==3, PSPIO_EIO);

    HANDLE_FUNC_ERROR(pspio_qn_set(&qn, (int)label[0], l, 0.0));

    //Read wavefunction
    for (i=0; i<np; i+=4) {
      if (fgets(line, MAX_STRLEN, fp) == NULL) {
	HANDLE_ERROR(PSPIO_EIO);
      }
      narg = sscanf(line,"%lf %lf %lf %lf", &wf[i], &wf[i+1], &wf[i+2], &wf[i+3]);
      ASSERT(narg==4, PSPIO_EIO);
    }

    //Store the state in the pspdata structure
    HANDLE_FUNC_ERROR (pspio_state_alloc( &(*pspdata)->states[j], np));
    HANDLE_FUNC_ERROR (pspio_state_set( &(*pspdata)->states[j], 0.0, label, qn, occ, 0.0, (*pspdata)->mesh, wf));

  }
  HANDLE_FUNC_ERROR(check_end_tag(fp,"PP_PSWFC"));

  free(wf);


  /****************************************************************************
   *                                PP_RHOATOM                                *
   ****************************************************************************/
  double * rho_read;

  HANDLE_FUNC_ERROR(init_tag(fp,"PP_RHOATOM",GO_BACK));

  //Allocate memory
  rho_read = (double *)malloc(np * sizeof(double));
  HANDLE_FATAL_ERROR(rho_read == NULL, PSPIO_ENOMEM);

  //Read valence density
  for (i=0; i<np; i+=4){
    if (fgets(line, MAX_STRLEN, fp) == NULL) {
      HANDLE_ERROR(PSPIO_EIO);
    }
    narg = sscanf(line,"%lf %lf %lf %lf",&rho_read[i],&rho_read[i+1],&rho_read[i+2],&rho_read[i+3]);
    ASSERT(narg==4, PSPIO_EIO);
  }
  HANDLE_FUNC_ERROR(check_end_tag(fp,"PP_RHOATOM"));
  
  //Store the density in the pspdata structure
  HANDLE_FUNC_ERROR(pspio_meshfunc_alloc(&(*pspdata)->rho_valence, np));
  HANDLE_FUNC_ERROR(pspio_meshfunc_set(&(*pspdata)->rho_valence, (*pspdata)->mesh, rho_read));

  free(rho_read);




  // Final deallocations
  HANDLE_FUNC_ERROR (pspio_qn_free( &(qn)));

  return PSPIO_SUCCESS;
}

int init_tag(FILE * fp, const char * tag, const int go_back){
  char line[MAX_STRLEN];
  char * init_tag = NULL;
  char * read_string = NULL;
  int i;

  if (go_back) rewind(fp);
  
  //Prepare base string
  init_tag = (char *)malloc((strlen(tag)+2) * sizeof(char));
  HANDLE_FATAL_ERROR(init_tag == NULL,PSPIO_ENOMEM);
  strcpy(init_tag, "");
  strcat(init_tag,"<");
  strncat(init_tag,tag,strlen(tag));
  strcat(init_tag,">");
  for (i=0;init_tag[i]; i++)
    init_tag[i] = tolower(init_tag[i]);

  while (fgets(line, sizeof line, fp) != NULL){
    //Skip white spaces
    if (line[0] == ' ')
      read_string = strtok(line," ");
    else 
      read_string = line;
    //Lowercase line
    for (i=0;read_string[i]; i++)
      read_string[i] = tolower(read_string[i]);
    
    if (strncmp(read_string,init_tag,strlen(init_tag))==0) return PSPIO_SUCCESS;
  }
  return PSPIO_EFILE_FORMAT;
}

int check_end_tag(FILE * fp, const char * tag){
  char line[MAX_STRLEN];
  char * ending_tag = NULL;
  char * read_string = NULL;
  int i;
  
  //Prepare base string
  ending_tag = (char *)malloc((strlen(tag)+3) * sizeof(char));
  HANDLE_FATAL_ERROR(ending_tag == NULL,PSPIO_ENOMEM);
  strcpy(ending_tag, "");
  strcat(ending_tag,"</");
  strncat(ending_tag,tag,strlen(tag));
  strcat(ending_tag,">");
  for (i=0;ending_tag[i]; i++)
    ending_tag[i] = tolower(ending_tag[i]);
  
  if(fgets(line, sizeof line, fp) == NULL){
    HANDLE_ERROR(PSPIO_EIO);
  }

  //Skip white spaces
  if (line[0] == ' ')
    read_string = strtok(line," ");
  else 
    read_string = line;
  //Lowercase line
  for (i=0;line[i]; i++)
    read_string[i] = tolower(read_string[i]);

  //Compare with the ending tag
  if (strncmp(read_string,ending_tag,strlen(ending_tag)) == 0) return PSPIO_SUCCESS;
  else {
    printf("PSPIO library is not able to find %s ending tag",ending_tag);
    return PSPIO_EFILE_FORMAT;
  }
}

int tag_isdef(FILE * fp, const char * tag){
  char line[MAX_STRLEN];
  char * init_tag = NULL; 
  char * read_string = NULL;
  int i;
  
  //Go to the beginning of the buffer
  rewind(fp);
  
  //Prepare base string
  init_tag = (char *)malloc((strlen(tag)+2) * sizeof(char));
  HANDLE_FATAL_ERROR(init_tag == NULL,PSPIO_ENOMEM);
  strcat(init_tag,"<");
  strncat(init_tag,tag,strlen(tag));
  strcat(init_tag,">");
  for (i=0;init_tag[i]; i++)
    init_tag[i] = tolower(init_tag[i]);
  
  while (fgets(line, sizeof line, fp) != NULL){ 
    //Skip white spaces
    if (line[0] == ' ')
      read_string = strtok(line," ");
    else 
      read_string = line;
    //Lowercase line
    for (i=0;line[i]; i++)
      line[i] = tolower(line[i]);
    
    if (strncmp(read_string,init_tag,strlen(init_tag))) return PSPIO_SUCCESS;
  }
  //End of the buffer reached; so return false
  return 1;
}
