/*
 Copyright (C) 2011 J. Alberdi, M. Oliveira, Y. Pouillon, and M. Verstraete

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2, or (at your option)
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 02111-1307, USA.

 $Id: pspio.h 26 2011-08-29 14:59:42Z micael $
*/

/**
 * @file pspio_init.c
 * @brief the main file 
 */

#include "pspio.h"

int psp_init(char * filename, int fileformat, psp_data_t * pspio){
  FILE * fp;
  int ierr;
  
  // open file
  fp = fopen(filename, "r");
  if(fp == NULL) return PSPIO_NOFILE;
  
  //read from file
  switch(fileformat) {
  case UNKNOWN:
    break;
  case ABINIT_4:
    break;
  case ABINIT_5:
    break;
  case ABINIT_6:
    break;
  case ABINIT_HGH:
    break;
  case ABINIT_GTH:
    break;
  case ATOM:
    break;
  case FHI98PP:
    break;
  case SIESTA:
    break;
  case UPF:
    pspio_upf_init(pspio,fp)
      break;
  default:
    return PSPIO_FILE_FORMAT_ERROR;
  }
  
  // close file and check for ierr being non 0
  ierr = close(fp);
  if(ierr != 0) return ierr;

  return 0;
}
