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
 * @file pspio_init.c
 * @brief the main file 
 */

#include "pspio.h"

int psp_init(char * file_name, int file_format, psp_data_t * psp_data){
  FILE * fp;
  int ierr;
  
  // open file
  fp = fopen(file_name, "r");
  if(fp == NULL) return PSPIO_NOFILE;
  
  //read from file
  switch(fileformat) {
  case UNKNOWN:
    break;
  case ABINIT_4:
    read_abinit_header(fp,psp_data,file_format);
    break;
  case ABINIT_5:
    read_abinit_header(fp,psp_data,file_format);
    break;
  case ABINIT_6:
    read_abinit_header(fp,psp_data,file_format);
    break;
  case ABINIT_HGH:
    read_abinit_header(fp,psp_data,file_format);
    break;
  case ABINIT_GTH:
    read_abinit_header(fp,psp_data,file_format);
    break;
  case ATOM:
    break;
  case FHI98PP:
    break;
  case SIESTA:
    break;
  case UPF:
    pspio_upf_init(fp,psp_data);
    break;
  default:
    return PSPIO_FILE_FORMAT_ERROR;
  }
  
  // close file and check for ierr being non 0
  ierr = close(fp);
  if(ierr != 0) return ierr;

  return 0;
}
