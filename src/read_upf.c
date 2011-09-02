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

/** 
 * @file read_upf.c
 * @brief implementation to read in UPF files 
 */
#include <string.h>
#include <ctype.h>
#include "read_upf.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


/* start the implementations */
/*****************************/
int  pspio_upf_init(FILE * fp, pspio_pspdata_t * psp_data){
  char tmp_filename[256];
  int ierr;
 
  HANDLE_FUNC_ERROR(pspio_upf_file_read(fp,psp_data));
}

int pspio_upf_file_read(FILE * fp, pspio_pspdata_t * psp_data){
  int i,j,ierr, narg, id_left,dummy;
  int version_number,kb_nc;
  char line[MAX_STRLEN],title[] = "UPF file";
  char symbol[2];
  char kind_ps[2],exchange[MAX_STRLEN];
  double zvalence,total_energy,wfc_cutoff,rho_cutoff;
  int l_max,np,n_states,n_proj;
  wavefunction_t * wavefunctions;
  pspio_qn_t * qn;
  pspio_qn_t ** qn_vec;
  //Variables for the mesh
  double *r, *drdi, *core_density;

  //Variables for non-linear core-corrections
  pspio_nlcc_t * nlcc;
  char nlcc_flag;
  int scheme; /**< scheme used to generate the pseudopotentials */


  //Variables for local potential
  double * vlocal_read;
  pspio_qn_t * qn_empty;
  pspio_potential_t * vlocal;

  //Variables for the projections
  pspio_projector_t ** projectors_vec;
  pspio_projector_t * projector;
  pspio_mesh_t * mesh;
  pspio_qn_t * qn_proj;
  pspio_qn_t ** qn_vec_proj;
  int l, proj_np;
  double * projector_read;
  double ** projector_read_vec;
  double * energy_vec;
  int n_dij,ii,jj;

  //Variables for the wavefunctions
  double **wfs;

  //Variables for the valence charge
  double * rho_read;

  //Variables for extra information
  double * proj_j_read;
  
  kb_nc = 1;

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
  narg = sscanf (line, "%2c",&symbol[0]);
  ASSERT(narg==1, PSPIO_EIO);		
  
  // read the kind of pseudo-potentials US|NC|PAW    "Ultrasoft|Norm conserving|Projector-augmented"  
  if(fgets(line, MAX_STRLEN, fp) == NULL) {
    HANDLE_ERROR(PSPIO_EIO);
  }
  narg = sscanf (line, "%2c",&kind_ps[0]); 
  ASSERT(narg==1, PSPIO_EIO);	
  //LIBPSP_IO can only read norm-conserving pseudo-potentials from UPF format.
  if (!strncmp(kind_ps,"NC",2)) {
    HANDLE_ERROR(PSPIO_ENOSUPPORT);
  }
  
  // read the nonlinear core correction. The character
  if(fgets(line, MAX_STRLEN, fp) == NULL) {
    HANDLE_ERROR(PSPIO_EIO);
  }
  narg = sscanf (line, "%c",&nlcc_flag); 
  ASSERT(narg==1, PSPIO_EIO);
  
  if(fgets(line, MAX_STRLEN, fp) == NULL) {
    HANDLE_ERROR(PSPIO_EIO);
  }

  //Store the 2 functionals (4 identifiers) into one string
  id_left = 4;
  i = 0;
  j = 0;
  while ( id_left && &line[i] != NULL){
    if (line[i]!=' '){
      //Save the identifier char by char
      exchange[j] = line[i];
      j++;
      //Find that if it is the last char of the identifier
      if (line[i-1] == ' ')
	id_left--;
      //if the next is whitespace have to included in the output
      if (line[i+1] == ' ' ){
	exchange[j] == ' ';
	j++;
      }
    }
    i++;
  }
    
  if(fgets(line, MAX_STRLEN, fp) == NULL) {
    HANDLE_ERROR(PSPIO_EIO);
  }
  narg = sscanf (line, "%lf",&zvalence); // read the Z valence
  ASSERT(narg==1, PSPIO_EIO);
  
  if(fgets(line, MAX_STRLEN, fp) == NULL) {
    HANDLE_ERROR(PSPIO_EIO);
  }
  narg = sscanf (line, "%lf",&total_energy); // read the total energy
  ASSERT(narg==1, PSPIO_EIO);
  
  if(fgets(line, MAX_STRLEN, fp) == NULL) {
    HANDLE_ERROR(PSPIO_EIO);
  }
  narg = sscanf (line, "%lf %lf",&wfc_cutoff,&rho_cutoff); //read the suggested cutoff for wfc and rho
  ASSERT(narg==2, PSPIO_EIO);
  
  if(fgets(line, MAX_STRLEN, fp) == NULL) {
    HANDLE_ERROR(PSPIO_EIO);
  }
  narg = sscanf (line, "%d",&l_max); // read the max angular momentun component (Note: is not the lmax needed by Octopus)
  ASSERT(narg==1, PSPIO_EIO);
  
  if(fgets(line, MAX_STRLEN, fp) == NULL) {
    HANDLE_ERROR(PSPIO_EIO);
  }
  narg = sscanf (line, "%d",&np); // read the number of points in mesh
  ASSERT(narg==1, PSPIO_EIO);
  
  if(fgets(line, MAX_STRLEN, fp) == NULL) {
    HANDLE_ERROR(PSPIO_EIO);
  }
  narg = sscanf (line, "%d %d",&n_states,&n_proj); // read the number of wavefunctions and projectors 
  ASSERT(narg==2, PSPIO_EIO);
  
  wavefunctions = (wavefunction_t *)malloc(n_states*sizeof(wavefunction_t));
  HANDLE_FATAL_ERROR(wavefunctions == NULL,PSPIO_ENOMEM);
  
  //Read the wavefuntions: "Wavefunctions   nl   l   occ"   
  if(fgets(line, MAX_STRLEN, fp) == NULL) {
    HANDLE_ERROR(PSPIO_EIO);
  } //skip the first line
  
  *(qn_vec) = (pspio_qn_t *)malloc(n_states * sizeof(pspio_qn_t));
  HANDLE_FATAL_ERROR(*(qn_vec) == NULL,PSPIO_ENOMEM);
  for (i=0; i<n_states; i++) {
    if(fgets(line, MAX_STRLEN, fp) == NULL) {
      HANDLE_ERROR(PSPIO_EIO);
    }
    narg = sscanf(line, "%2c %d %lf",&wavefunctions[i].nl[0],&wavefunctions[i].l,
		  &wavefunctions[i].occ); // read the number of wavefunctions and projectors 
    HANDLE_FUNC_ERROR(pspio_qn_alloc(qn));
    HANDLE_FUNC_ERROR(pspio_qn_set(qn,(int)wavefunctions[i].nl[0],wavefunctions[i].l,wavefunctions[i].occ));
    
    qn_vec[i] = qn;
    //HANDLE_FUNC_ERROR(pspio_qn_free(qn)); ??
    ASSERT(narg==3, PSPIO_EIO);
  }
  HANDLE_FUNC_ERROR(check_end_tag(fp, "PP_HEADER"));
  
  // Mesh info /////////////////////////////////////////////////////////////////
  HANDLE_FUNC_ERROR(init_tag(fp, "PP_MESH", GO_BACK));
  HANDLE_FUNC_ERROR(init_tag(fp, "PP_R", NO_GO_BACK));
  r = (double *)malloc(np*sizeof(double));
  HANDLE_FATAL_ERROR(r == NULL,PSPIO_ENOMEM);
  
  for (i=0; i<np; i+4) {
    if(fgets(line, MAX_STRLEN, fp) == NULL) {
      HANDLE_ERROR(PSPIO_EIO);
    }
    narg = sscanf(line,"%lf %lf %lf %lf",&r[i],&r[i+1],&r[i+2],&r[i+3]);
    ASSERT(narg==4, PSPIO_EIO);
  }
  HANDLE_FUNC_ERROR(check_end_tag(fp, "PP_R"));
  
  HANDLE_FUNC_ERROR(init_tag(fp, "PP_RAB", NO_GO_BACK));
  drdi = (double *)malloc(np*sizeof(double));
  HANDLE_FATAL_ERROR(drdi == NULL,PSPIO_ENOMEM);
  
  for (i=0; i<np; i+4) {
    if (fgets(line, MAX_STRLEN, fp) == NULL) {
      HANDLE_ERROR(PSPIO_EIO);
    }
    narg = sscanf(line,"%lf %lf %lf %lf",&drdi[i],&drdi[i+1],&drdi[i+2],&drdi[i+3]);
    ASSERT(narg==4, PSPIO_EIO);
  }                                                
  HANDLE_FUNC_ERROR(check_end_tag(fp, "PP_RAB"));
  HANDLE_FUNC_ERROR(check_end_tag(fp, "PP_MESH")); 

  //Store the mesh in the general pspio_mesh_t format
  HANDLE_FUNC_ERROR(pspio_mesh_alloc(mesh,np));
  HANDLE_FUNC_ERROR(pspio_mesh_init_from_points(mesh,r,drdi));
  // Mesh info end -------------------------------------------------------------
  
  // Non-linear core-corrections ///////////////////////////////////////////////
  if (nlcc_flag) {
    //@todo Initialize scheme
    scheme = NLCC_UNKNOWN;
    HANDLE_FUNC_ERROR(pspio_nlcc_alloc(nlcc,np));
    HANDLE_FUNC_ERROR(init_tag(fp, "PP_NLCC", GO_BACK));
    core_density = (double *)malloc(np*sizeof(double));
    HANDLE_FATAL_ERROR(core_density == NULL,PSPIO_ENOMEM);
    
    for (i=0; i<np; i+4) {
      if (fgets(line, MAX_STRLEN, fp) == NULL) {
	HANDLE_ERROR(PSPIO_EIO);
      }
      narg = sscanf(line,"%lf %lf %lf %lf",&core_density[i],&core_density[i+1],&core_density[i+2],&core_density[i+3]);
      ASSERT(narg==4, PSPIO_EIO);
    }
    HANDLE_FUNC_ERROR(check_end_tag(fp,"PP_NLC"));
    //Fill the pspio_nlcc_t data structure
    HANDLE_FUNC_ERROR(pspio_nlcc_set(nlcc,scheme,mesh,core_density));
  }
  else {
    core_density = NULL;
  }
  // Non-linear core-corrections end -------------------------------------------
  
  // Local component ///////////////////////////////////////////////////////////
  HANDLE_FUNC_ERROR(init_tag(fp, "PP_LOCAL",GO_BACK));
  vlocal_read = (double *)malloc(np*sizeof(double));
  HANDLE_FATAL_ERROR(vlocal_read == NULL,PSPIO_ENOMEM);
  
  for (i=0; i<np; i+4) {
    if (fgets(line, MAX_STRLEN, fp) == NULL) {
      HANDLE_ERROR(PSPIO_EIO);
    }
    narg = sscanf(line,"%lf %lf %lf %lf",&vlocal_read[i],&vlocal_read[i+1],&vlocal_read[i+2],&vlocal_read[i+3]);
    ASSERT(narg==4, PSPIO_EIO);
  }
  HANDLE_FUNC_ERROR(check_end_tag(fp,"PP_LOCAL"));
  //Store the local potential in the general data structure
  HANDLE_FUNC_ERROR(pspio_qn_alloc(qn_empty));
  HANDLE_FUNC_ERROR(pspio_potential_alloc(vlocal,np));
  HANDLE_FUNC_ERROR(pspio_potential_set(vlocal,qn_empty,mesh,vlocal_read));
  // Local component end -------------------------------------------------------
  
  // Non-local component. //////////////////////////////////////////////////////
  HANDLE_FUNC_ERROR(init_tag(fp,"PP_NONLOCAL",GO_BACK));
  *(qn_vec_proj) = (pspio_qn_t *)malloc(n_states * sizeof(pspio_qn_t));
  HANDLE_FATAL_ERROR(*(qn_vec_proj) == NULL,PSPIO_ENOMEM);
  //Alloc memory for all projections
  *(projector_read_vec) = (double *)malloc(n_proj * sizeof(double) * np); 
  HANDLE_FATAL_ERROR(*(projector_read_vec) == NULL,PSPIO_ENOMEM);
  for (i=0;i<n_proj;i++){
    HANDLE_FUNC_ERROR(init_tag(fp,"PP_BETA",NO_GO_BACK));
    if(fgets(line, MAX_STRLEN, fp) == NULL) {
      HANDLE_ERROR(PSPIO_EIO);
    }
    narg = sscanf(line,"%d %d",&dummy,&l);
    ASSERT(narg==2, PSPIO_EIO);

    if(fgets(line, MAX_STRLEN, fp) == NULL) {
      HANDLE_ERROR(PSPIO_EIO);
    }
    //Read the number of points of projections
    narg = sscanf(line,"%d",&proj_np);
    ASSERT(narg==1, PSPIO_EIO);
    
    //In any case; alloc memory for the entire mesh
    projector_read = (double *)malloc(np * sizeof(double));
    HANDLE_FATAL_ERROR(projector_read == NULL,PSPIO_ENOMEM);
    
    //Read the projections
    for (j=0;j<proj_np;j+4){
      if (fgets(line, MAX_STRLEN, fp) == NULL) {
	HANDLE_ERROR(PSPIO_EIO);
      }
      narg = sscanf(line,"%lf %lf %lf %lf",&projector_read[j],&projector_read[j+1],&projector_read[j+2],&projector_read[j+3]);
      ASSERT(narg==4, PSPIO_EIO);
    }
    //Fill with zeros, if any left
    for (j=proj_np;j<np;j++)
      projector_read[i] = 0.0;
   
    //Store in the vector
    projector_read_vec[i] = projector_read;
    HANDLE_FUNC_ERROR(pspio_qn_alloc(qn_proj));
    HANDLE_FUNC_ERROR(pspio_qn_set(qn,0,l,0.0));
    qn_vec_proj[i] = qn_proj;
  }
  HANDLE_FUNC_ERROR(init_tag(fp,"PP_DIJ",NO_GO_BACK));
  energy_vec = (double *)malloc(n_proj*sizeof(double));
  HANDLE_FATAL_ERROR(energy_vec == NULL,PSPIO_ENOMEM);
  
  //Read the number of n_dij 
  if (fgets(line, MAX_STRLEN, fp) == NULL) {
    HANDLE_ERROR(PSPIO_EIO);
  }
  narg == sscanf(line,"%d",&n_dij);
  ASSERT(narg==1, PSPIO_EIO);

  for (i=0;i<n_dij;i++){
    if (fgets(line, MAX_STRLEN, fp) == NULL) {
      HANDLE_ERROR(PSPIO_EIO);
    }
    narg == sscanf(line,"%d %d %lf",&ii,&jj,&energy_vec[i]);
    ASSERT(narg==3, PSPIO_EIO);
    if (ii /= jj) {
      HANDLE_ERROR(PSPIO_EVALUE);
    }
  }
  HANDLE_FUNC_ERROR(check_end_tag(fp,"PP_DIJ"));
  HANDLE_FUNC_ERROR(check_end_tag(fp,"PP_NONLOCAL"));
  
  //Save the projections to the general data structure
  *(projectors_vec) = (pspio_projector_t *)malloc(n_proj * sizeof(pspio_projector_t));
  HANDLE_FATAL_ERROR(*(projectors_vec) == NULL, PSPIO_ENOMEM);
  for (i=0;i<n_proj;i++){ 
    HANDLE_FUNC_ERROR(pspio_projector_alloc(projector,np));
    HANDLE_FUNC_ERROR(pspio_projector_set(projector,qn_vec_proj[i],energy_vec[i],mesh,projector_read_vec[i]));
    projectors_vec[i] = projector;
  }
  // Non-local component. ------------------------------------------------------
  
  // Pseudo wavefunctions //////////////////////////////////////////////////////
  HANDLE_FUNC_ERROR(init_tag(fp,"PP_PSWFC",GO_BACK));
  //allocate(ps_upf%wfs(1:ps_upf%np, 1:ps_upf%n_wfs))
  //allocate memory for rows
  wfs = (double **)malloc(n_states * sizeof(double));
  HANDLE_FATAL_ERROR(wfs == NULL,PSPIO_ENOMEM);
  
  //For each row allocate memory for columns
  //From: http://www.codeproject.com/KB/cpp/pointertopointer.aspx
  for (i=0;i<n_states;i++){
    *(wfs+i) = (double*)malloc(np * sizeof(double));
    HANDLE_FATAL_ERROR(*(wfs) == NULL,PSPIO_ENOMEM);
    
    //Skip the first line for each state
    if (fgets(line, MAX_STRLEN, fp) == NULL) {
      HANDLE_ERROR(PSPIO_EIO);
    }
    for (j=0;j<np;j+4){
      if (fgets(line, MAX_STRLEN, fp) == NULL) {
	HANDLE_ERROR(PSPIO_EIO);
      }
      narg == sscanf(line,"%lf %lf %lf %lf",&wfs[i][j],&wfs[i][j+1],&wfs[i][j+2],&wfs[i][j+3]);
      ASSERT(narg==4, PSPIO_EIO);
    }
  }
  HANDLE_FUNC_ERROR(check_end_tag(fp,"PP_PSWFC"));
  // Pseudo wavefunctions end --------------------------------------------------

  // Valence charge ////////////////////////////////////////////////////////////
  HANDLE_FUNC_ERROR(init_tag(fp,"PP_RHOATOM",GO_BACK));
  //Allocate memory to read
  rho_read = (double *)malloc(np * sizeof(double));
  HANDLE_FATAL_ERROR(rho_read == NULL,PSPIO_ENOMEM);
  for (i=0;i<np;i+4){
    if (fgets(line, MAX_STRLEN, fp) == NULL) {
      HANDLE_ERROR(PSPIO_EIO);
    }
    narg == sscanf(line,"%lf %lf %lf %lf",&rho_read[i],&rho_read[i+1],&rho_read[i+2],&rho_read[i+3]);
    ASSERT(narg==4, PSPIO_EIO);
  }
  HANDLE_FUNC_ERROR(check_end_tag(fp,"PP_RHOATOM"));
  // Valence charge end --------------------------------------------------------

  // Extra information /////////////////////////////////////////////////////////
  //tag_isdef is not error checked!!
  if (tag_isdef(fp,"PP_ADDINFO")){
    kb_nc = 2;
    //I don't know if has to be done (I guess the pointer is already there):
    HANDLE_FUNC_ERROR(init_tag(fp,"PP_ADDINFO",GO_BACK));
    for (i=0;i<n_states;i++){
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
  //Extra information end ------------------------------------------------------

  //Save the rest to the global data structure
  psp_data->title = title;
  psp_data->symbol = symbol;
  psp_data->z = symbol_to_z(symbol) ; //double
  psp_data->zvalence = zvalence;
  //psp_data->nelvalence = ; //double
  psp_data->l_max = l_max;
  psp_data->n_kbproj = kb_nc;
  //psp_data->wave_eq = ; //int
  psp_data->mesh = mesh;
  //psp_data->qn_to_istate = ; //int ** I think is not needed to fill
  //psp_data->states = ; //psp_state_t**
  psp_data->n_states = n_states;
  //psp_data->scheme = ;//int
  psp_data->n_potentials = np; //????
  psp_data->potentials = vlocal; //I think is wrong. pspio_potential_t**
  psp_data->n_kbproj = kb_nc;//????int
  //psp_data->kb_projectors = ;//pspio_projector_t**
  psp_data->l_local = l;//?? int
  psp_data->vlocal = vlocal; //psp_potential_t *
  //psp_data->exchange = exchange; //this is wrong as I read as string exchange and we need int
  //psp_data->correlation = ;//int
  psp_data->has_nlcc = (int)nlcc_flag; //
  psp_data->nlcc = nlcc;

  return PSPIO_SUCCESS;
}

int init_tag(FILE * fp, const char * tag, const int go_back){
  char line[MAX_STRLEN];
  char * compare_string = NULL;
  char * result_string = NULL;
  int i;

  if (go_back) rewind(fp);
  
  //Prepare base string
  compare_string = (char *)malloc((strlen(tag)+2) * sizeof(char));
  HANDLE_FATAL_ERROR(compare_string == NULL,PSPIO_ENOMEM);
  strcat(compare_string,"<");
  strncat(compare_string,tag,strlen(tag));
  strcat(compare_string,">");
  for (i=0;compare_string[i]; i++)
    compare_string[i] = tolower(compare_string[i]);
    
  while (fgets(line, sizeof line, fp) != NULL){
    //Lowercase line
    for (i=0;line[i]; i++)
      line[i] = tolower(line[i]);
    //Find out if the tag is inside the line and if yes, go to the first char *
    result_string = strstr(compare_string,line);
    if (result_string != NULL){
      if (strncmp(result_string,compare_string,strlen(compare_string))) return PSPIO_SUCCESS;
    }
  }
  return PSPIO_EFILE_FORMAT;
}

int check_end_tag(FILE * fp, const char * tag){
  char line[MAX_STRLEN];
  char * compare_string = NULL;
  char * result_string = NULL;
  int i;
  
  //Prepare base string
  compare_string = (char *)malloc((strlen(tag)+3) * sizeof(char));
  HANDLE_FATAL_ERROR(compare_string == NULL,PSPIO_ENOMEM);
  strcat(compare_string,"</");
  strncat(compare_string,tag,strlen(tag));
  strcat(compare_string,">");
  for (i=0;compare_string[i]; i++)
    compare_string[i] = tolower(compare_string[i]);
  
  if(fgets(line, sizeof line, fp) == NULL){
    HANDLE_ERROR(PSPIO_EIO);
  }
  //Lowercase line
  for (i=0;line[i]; i++)
    line[i] = tolower(line[i]);

  //Compare with the ending tag 
  
  //Find out if the tag is inside the line and if yes, go to the first char *
  result_string = strstr(compare_string,line);
  if (strncmp(result_string,compare_string,strlen(compare_string))) return PSPIO_SUCCESS;
  else {
    printf("PSPIO library is not able to find %s ending tag",compare_string);
    return PSPIO_EFILE_FORMAT;
  }
  
}

int tag_isdef(FILE * fp, const char * tag){
  char line[MAX_STRLEN];
  char * compare_string = NULL;
  char * result_string = NULL;
  int i;
  
  //Go to the beginning of the buffer
  rewind(fp);
  
  //Prepare base string
  compare_string = (char *)malloc((strlen(tag)+2) * sizeof(char));
  HANDLE_FATAL_ERROR(compare_string == NULL,PSPIO_ENOMEM);
  
  strcat(compare_string,"<");
  strncat(compare_string,tag,strlen(tag));
  strcat(compare_string,">");
  for (i=0;compare_string[i]; i++)
    compare_string[i] = tolower(compare_string[i]);
  
  while (fgets(line, sizeof line, fp) != NULL){
    //Lowercase line
    for (i=0;line[i]; i++)
      line[i] = tolower(line[i]);
    //Find out if the tag is inside the line and if yes, go to the first char *
    result_string = strstr(compare_string,line);
    if (result_string != NULL)
      if (strncmp(result_string,compare_string,strlen(compare_string))) return PSPIO_SUCCESS;
  }
  //End of the buffer reached; so return false
  return 1;
}
