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
 * @brief subroutine to read in UPF files 
 */

#include <pspio.h>
#include "stdio.h"

/**
 * Init the UPF file read
 * @param[in,out] psp_data the data structure
 * @param[in] fp a stream of the input file
 * @return error code
 */
int pspio_upf_init(char FILE * fp, pspio_pspdata_t * psp_data);
int pspio_upf_end(pspio_pspdata_t * psp_data);
int pspio_upf_file_read(FILE * fp, pspio_pspdata_t * psp_data);

int tag_isdef(FILE * fp, char * string);
int init_tag(FILE * fp, char * string, int go_back);
int check_end_tag(char * tag);

int pspio_upf_cutoff_radii(pspio_pspdata_t * psp_data);

/**
 * checks normalization of the pseudo wavefunctions
 */   
int pspio_upf_check_rphi(pspio_pspdata_t * psp_data);



 /* start the implementations */
/*****************************/
int  pspio_upf_init(char FILE * fp, pspio_pspdata_t * psp_data){
  char tmp_filename[256];
  int ierr;
  
  pspio_upf_file_read(fp,psp_data);
}

int pspio_upf_file_read(FILE * fp, pspio_pspdata_t * psp_data){
  int ierr;
  int version_number;
  char line[MAX_LINE + 1];
  
  init_tag(fp,"PP_HEADER", 1);
  fscanf(fp,"%d",version_number); //read the version number
  fgets(line, sizeof line, fp); //skip the rest of the line
  fgets(psp_data->symbol, sizeof line, fp); //read the symbol
  fgets(line, 2, fp);  // kind of pseudo-potentials US|NC|PAW    "Ultrasoft|Norm conserving|Projector-augmented" 
  if (!strncpm(line,"NC",2)) {
    printf("LIBPSP_IO can only read norm-conserving pseudo-potentials from UPF format.");
    return PSPIO_TYPE_ERROR;
  }
  fscanf(fp,"%d",psp_data->nlcc); // read the nonlinear core correction
  fgets(line, sizeof line, fp); //skip the rest of the line
  fscanf(fp,"%d",psp_data->exchange); //read the exchange-correlation functional !!CORRECTION NEEDED!!!!!!!!!!!!!!!!! either we have to skipt or read it like in LIBXC
  //read(unit,*) dummy                ! dft      "Exch-Corr"                                                                                                                                                          
  fgets(line, sizeof line, fp); //skip the rest of the line
  fscanf(fp,"%4.11f",psp_data->zvalence); //read the Z valence
  fgets(line, sizeof line, fp); //skip the rest of the line
  fgets(line, sizeof line, fp); //skip the total energy
  fscanf(fp,"4.7f",psp_data->rc); //read the suggested cutoff for wfc
  //fscanf ??? read the suggested cutoff for rho
  fgets(line, sizeof line, fp); //skip the rest of the line
  fscanf(fp,"%d",psp_data->lmax); //read the max angular momentun component (Note: is not the lmax needed by Octopus
  fgets(line, sizeof line, fp); //skip the rest of the line
  fscanf(fp,"%d",psp_data->states->np); //read the number of points in mesh
  fgets(line, sizeof line, fp); //skip the rest of the line
  fscanf(fp,"%d %d",psp_data->n_states,psp_data->pspio_kb_projectors_t->nproj);  //read the number of wavefunctions, projector 

  //read the wavefuntions: @todo
  
}

int init_tag(FILE * fp, char * string, int go_back){
  char line[MAX_LINE + 1];
  char * compare_string = NULL;
  if (go_back) rewind(fp);
  
  strcat(compare_string,"<");
  strncat(compare_string,string,strlen(string));
  strcat(compare_string,">");
  
  while(fgets(line, sizeof line, fp) != NULL){
    if( strcamp(line,compare_string) ) return;
  }
}

