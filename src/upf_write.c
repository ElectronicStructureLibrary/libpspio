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
 * @file upf_write.c
 * @brief routines to write UPF files 
 */
#include <string.h>
#include <ctype.h>
#include "upf.h"
#include "util.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


int upf_write_header(FILE *fp, const pspio_pspdata_t *pspdata){

  //Write init tag
  fprintf(fp, "<PP_HEADER>\n");
  
  //Write the version number
 
  //Write the atomic symbol

  //Write the kind of pseudo-potentials US|NC|PAW
  
  //Write the nonlinear core correction

  //Write exchange-correlation functional

  //Write the Z valence

  //Write the total energy
  
  //Write the suggested cutoff for wfc and rho
  
  //Write the max angular momentun component
 
  //Write the number of points in mesh
  
  //Write the number of wavefunctions and projectors 

  //Write wavefunctions info

  //Write end tag
  fprintf(fp, "</PP_HEADER>\n");

  return PSPIO_SUCCESS;
}


int upf_write_mesh(FILE *fp, const pspio_pspdata_t *pspdata){
  int i;

  //Write init tag
  fprintf(fp, "<PP_MESH>\n");

  //Print mesh points
  fprintf(fp, "  <PP_R>\n");
  for (i=0; i<pspdata->mesh->np; i+=4) {
    fprintf(fp, " %18.11E %18.11E %18.11E %18.11E\n", pspdata->mesh->r[i], pspdata->mesh->r[i+1], pspdata->mesh->r[i+2], pspdata->mesh->r[i+3]);
  }
  fprintf(fp, "  </PP_R>\n");

  //Print Rab
  fprintf(fp, "  <PP_RAB>\n");
  for (i=0; i<pspdata->mesh->np; i+=4) {
    fprintf(fp, " %18.11E %18.11E %18.11E %18.11E\n", pspdata->mesh->rab[i], pspdata->mesh->rab[i+1], pspdata->mesh->rab[i+2], pspdata->mesh->rab[i+3]);
  }
  fprintf(fp, "  </PP_RAB>\n");

  //Write end tag
  fprintf(fp, "</PP_MESH>\n");

  return PSPIO_SUCCESS;
}


int upf_write_nlcc(FILE *fp, const pspio_pspdata_t *pspdata){

  //Write init tag
  fprintf(fp, "<PP_NLCC>\n");

  //Write end tag
  fprintf(fp, "</PP_NLCC>\n");

  return PSPIO_SUCCESS;
}


int upf_write_nonlocal(FILE *fp, const pspio_pspdata_t *pspdata){

  //Write init tag
  fprintf(fp, "<PP_NONLOCAL>\n");

  //Write end tag
  fprintf(fp, "</PP_NONLOCAL>\n");

  return PSPIO_SUCCESS;
}


int upf_write_local(FILE *fp, const pspio_pspdata_t *pspdata){

  //Write init tag
  fprintf(fp, "<PP_LOCAL>\n");

  //Write end tag
  fprintf(fp, "</PP_LOCAL>\n");

  return PSPIO_SUCCESS;
}


int upf_write_pswfc(FILE *fp, const pspio_pspdata_t *pspdata){

  //Write init tag
  fprintf(fp, "<PP_PSWFC>\n");

  //Write end tag
  fprintf(fp, "</PP_PSWFC>\n");

  return PSPIO_SUCCESS;
}


int upf_write_rhoatom(FILE *fp, const pspio_pspdata_t *pspdata){

  //Write init tag
  fprintf(fp, "<PP_RHOATOM>\n");

  //Write end tag
  fprintf(fp, "</PP_RHOATOM>\n");

  return PSPIO_SUCCESS;
}
