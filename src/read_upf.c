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

/** @file read_upf.c
 * @brief implementation to read in UPF files 
 */

#include "read_upf.h"

/* start the implementations */
/*****************************/
int  pspio_upf_init(FILE * fp, pspio_pspdata_t * psp_data){
  char tmp_filename[256];
  int ierr;
 
  HANDLE_FUNC_ERROR(pspio_upf_file_read(fp,psp_data));
}

int pspio_upf_file_read(FILE * fp, pspio_pspdata_t * psp_data){
  int i,ierr, narg;
  int version_number;
  char line[MAX_STRLEN];
  char symbol[2];
  char nlcc,kind_ps[2],exchange[MAX_STRLEN];
  double zvalence,total_energy,wfc_cutoff,rho_cutoff;
  int lmax,np,n_states,n_proj;
  wavefunction_t * wavefunctions;
  
  //variables for the mesh
  double *r, *drdi, *core_density;
  
  HANDLE_FUNC_ERROR(init_tag(fp,"PP_HEADER", GO_BACK));
  
  if(fgets(line, MAX_STRLEN, fp) == NULL) {
    HANDLE_ERROR(PSPIO_EIO);
  }
  narg = sscanf (line, "%d", &version_number);
  PSPIO_ASSERT(narg==1, PSPIO_EIO);
  
  if(fgets(line, MAX_STRLEN, fp) == NULL) {
    HANDLE_ERROR(PSPIO_EIO);
  }
  narg = sscanf (line, "%2c",&symbol[0]);
  PSPIO_ASSERT(narg==1, PSPIO_EIO);		

  if(fgets(line, MAX_STRLEN, fp) == NULL) {
    HANDLE_ERROR(PSPIO_EIO);
  }
  narg = sscanf (line, "%2c",&kind_ps[0]); // read the kind of pseudo-potentials US|NC|PAW    "Ultrasoft|Norm conserving|Projector-augmented" 
  PSPIO_ASSERT(narg==1, PSPIO_EIO);	
  if (!strncpm(kind_ps,"NC",2)) {
    printf("LIBPSP_IO can only read norm-conserving pseudo-potentials from UPF format.");
    return PSPIO_ETYPE;
  }
	
  if(fgets(line, MAX_STRLEN, fp) == NULL) {
    HANDLE_ERROR(PSPIO_EIO);
  }
  narg = sscanf (line, "%c",&nlcc); // read the nonlinear core correction. The character
  PSPIO_ASSERT(narg==1, PSPIO_EIO);
	
  if(fgets(line, MAX_STRLEN, fp) == NULL) {
    HANDLE_ERROR(PSPIO_EIO);
  }
  narg = sscanf (line, "%s %s %s %s",&exchange); // read the exchange-correlation functional!!CORRECTION NEEDED!!!!!!! either we have to skipt or read it like in LIBXC
  PSPIO_ASSERT(narg==3, PSPIO_EIO);
	
  if(fgets(line, MAX_STRLEN, fp) == NULL) {
    HANDLE_ERROR(PSPIO_EIO);
  }
  narg = sscanf (line, "%lf",&zvalence); // read the Z valence
  PSPIO_ASSERT(narg==1, PSPIO_EIO);

  if(fgets(line, MAX_STRLEN, fp) == NULL) {
    HANDLE_ERROR(PSPIO_EIO);
  }
  narg = sscanf (line, "%lf",&total_energy); // read the total energy
  PSPIO_ASSERT(narg==1, PSPIO_EIO);
  
  if(fgets(line, MAX_STRLEN, fp) == NULL) {
    HANDLE_ERROR(PSPIO_EIO);
  }
  narg = sscanf (line, "%lf %lf",&wfc_cutoff,&rho_cutoff); //read the suggested cutoff for wfc and rho
  PSPIO_ASSERT(narg==2, PSPIO_EIO);
  
  if(fgets(line, MAX_STRLEN, fp) == NULL) {
    HANDLE_ERROR(PSPIO_EIO);
  }
  narg = sscanf (line, "%d",&lmax); // read the max angular momentun component (Note: is not the lmax needed by Octopus)
  PSPIO_ASSERT(narg==1, PSPIO_EIO);
  
  if(fgets(line, MAX_STRLEN, fp) == NULL) {
    HANDLE_ERROR(PSPIO_EIO);
  }
  narg = sscanf (line, "%d",&np); // read the number of points in mesh
  PSPIO_ASSERT(narg==1, PSPIO_EIO);
	
  if(fgets(line, MAX_STRLEN, fp) == NULL) {
    HANDLE_ERROR(PSPIO_EIO);
  }
  narg = sscanf (line, "%d %d",&n_states,&n_proj); // read the number of wavefunctions and projectors 
  PSPIO_ASSERT(narg==2, PSPIO_EIO);

  wavefunctions = (wavefunction_t *)malloc(n_states*sizeof(wavefunction_t));
  if(wavefunctions == NULL){
    HANDLE_FATAL_ERROR(PSPIO_ENOMEM);
  }

  //read the wavefuntions: "Wavefunctions   nl   l   occ"   
  if(fgets(line, MAX_STRLEN, fp) == NULL) {
    HANDLE_ERROR(PSPIO_EIO);
  } //skip the first line
  for (i=0; i<n_states; i++) {
    if(fgets(line, MAX_STRLEN, fp) == NULL) {
      HANDLE_ERROR(PSPIO_EIO);
    }
    narg = sscanf(line, "%2c %d %lf",&wavefunctions[i].nl[0],&wavefunctions[i].l,
		  &wavefunctions[i].occ); // read the number of wavefunctions and projectors 
    PSPIO_ASSERT(narg==3, PSPIO_EIO);
  }
  HANDLE_FUNC_ERROR(check_end_tag(fp, "PP_HEADER"));
  
  // Mesh info                                                                                         
  HANDLE_FUNC_ERROR(init_tag(fp, "PP_MESH", GO_BACK));
  HANDLE_FUNC_ERROR(init_tag(fp, "PP_R", NO_GO_BACK));
  r = (double *)malloc(np*sizeof(double));
  if( r == NULL){
    HANDLE_FATAL_ERROR(PSPIO_ENOMEM);
  }
  for (i=0; i<np; i+4) {
    if(fgets(line, MAX_STRLEN, fp) == NULL) {
      HANDLE_ERROR(PSPIO_EIO);
    }
    narg = sscanf(line,"%lf %lf %lf %lf",&r[i],&r[i+1],&r[i+2],&r[i+3]);
    PSPIO_ASSERT(narg==4, PSPIO_EIO);
  }
  
  HANDLE_FUNC_ERROR(check_end_tag(fp, "PP_R"));
  
  HANDLE_FUNC_ERROR(init_tag(fp, "PP_RAB", NO_GO_BACK));
  drdi = (double *)malloc(np*sizeof(double));
  if (drdi == NULL) {
    HANDLE_FATAL_ERROR(PSPIO_ENOMEM);
  }
  for (i=0; i<np; i+4) {
    if (fgets(line, MAX_STRLEN, fp) == NULL) {
      HANDLE_ERROR(PSPIO_EIO);
    }
    narg = sscanf(line,"%lf %lf %lf %lf",&drdi[i],&drdi[i+1],&drdi[i+2],&drdi[i+3]);
    PSPIO_ASSERT(narg==4, PSPIO_EIO);
  }                                                
  HANDLE_FUNC_ERROR(check_end_tag(fp, "PP_RAB"));
  HANDLE_FUNC_ERROR(check_end_tag(fp, "PP_MESH"));

  //Non-linear core-corrections
  if (nlcc) {
    HANDLE_FUNC_ERROR(init_tag(fp, "PP_NLCC", GO_BACK));
    core_density = (double *)malloc(np*sizeof(double));
    if (core_density == NULL) {
      HANDLE_FATAL_ERROR(PSPIO_ENOMEM);
    }
    for (i=0; i<np; i+4) {
      if (fgets(line, MAX_STRLEN, fp) == NULL) {
	HANDLE_ERROR(PSPIO_EIO);
      }
      narg = sscanf(line,"%lf %lf %lf %lf",&core_density[i],&core_density[i+1],&core_density[i+2],&core_density[i+3]);
      PSPIO_ASSERT(narg==4, PSPIO_EIO);
    }
  }
  else {
    
  }
  return PSPIO_SUCCESS;
}

int init_tag(FILE * fp, char * tag, int go_back){
  char line[MAX_STRLEN];
  char * compare_string = NULL;
  if (go_back) rewind(fp);
  
  strcat(compare_string,"<");
  strncat(compare_string,tag,strlen(tag));
  strcat(compare_string,">");
  
  while (fgets(line, sizeof line, fp) != NULL){
    if (strcmp(line,compare_string)) return PSPIO_SUCCESS;
  }
  return PSPIO_EFILE_FORMAT;
}

int check_end_tag(FILE * fp, char * tag){
  char line[MAX_STRLEN];
  char * compare_string = NULL;
  
  strcat(compare_string,"</");
  strncat(compare_string,tag,strlen(tag));
  strcat(compare_string,">");
  
  if(fgets(line, sizeof line, fp) == NULL) return PSPIO_EIO;

  if (strcmp(line,compare_string)) return PSPIO_SUCCESS;
  else {
    printf("PSPIO library is not able to find %s ending tag",compare_string);
    return PSPIO_EFILE_FORMAT;
  }
  
}
