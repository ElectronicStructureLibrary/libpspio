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

 $Id: pspio_error.h 58 2011-08-30 10:44:20Z micael $
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pspio_error.h"


int pspio_error_add(const char *filename, const int line) {
  int s;
  pspio_error_t *last_err;

  if ( pspio_error_chain == NULL ) {
    pspio_error_chain == (pspio_error_t *)malloc(sizeof(pspio_error_t));
    if ( pspio_error_chain == NULL ) {
      return PSPIO_ERROR;
    }

    pspio_error_chain->id = pspio_error_tmp_id;
    pspio_error_chain->line = line;
    pspio_error_chain->next = NULL;
    s = strlen(filename);
    pspio_error_chain->filename = (char *)malloc(s + 1);
    if ( pspio_error_chain->filename == NULL ) {
      return PSPIO_ERROR;
    }
    memcpy(pspio_error_chain->filename,filename,s);
    pspio_error_chain->filename[s] = 0;
    pspio_error_tmp_id = 0;

    return PSPIO_SUCCESS;
  }

  last_err = pspio_error_chain;
  while ( last_err->next != NULL ) {
    last_err = last_err->next;
  }

  last_err->next = (pspio_error_t *)malloc(sizeof(pspio_error_t));
  if ( last_err->next == NULL ) {
    return PSPIO_ERROR;
  }
  last_err = last_err->next;
  last_err->id = pspio_error_tmp_id;
  last_err->line = line;
  last_err->next = NULL;
  s = strlen(filename);
  last_err->filename = (char *)malloc(s + 1);
  if ( last_err->filename == NULL ) {
    return PSPIO_ERROR;
  }
  memcpy(last_err->filename,filename,s);
  last_err->filename[s] = 0;
  pspio_error_tmp_id = 0;

  return PSPIO_SUCCESS;
}

int pspio_error_free(void) {
  pspio_error_t *first_err;

  while ( pspio_error_chain != NULL ) {
    first_err = pspio_error_chain;
    pspio_error_chain = pspio_error_chain->next;
    free(first_err);
  }

  return PSPIO_SUCCESS;
}

pspio_error_t *pspio_error_pop(void) {
  pspio_error_t *first_error = NULL;

  if ( pspio_error_chain != NULL ) {
    first_error = pspio_error_chain;
    pspio_error_chain = pspio_error_chain->next;
  }

  return first_error;
}


void pspio_error_show(const int error_id, const char *filename,
       const int line) {
  printf("libpspio: ERROR:\n  %s\n",pspio_error_str(error_id));
  if ( filename != NULL ) {
    printf("  in file %s:%d\n",filename,line);
  }
}


const char *pspio_error_str(const int pspio_errorid) {
  switch (pspio_errorid)
    {
    case PSPIO_SUCCESS:
      return "success" ;
    case PSPIO_ERROR:
      return "error" ;
    case PSPIO_ENOFILE:
      return "file does not exist" ;
    case PSPIO_EIO:
      return "error in I/O" ;
    case PSPIO_EVALUE:
      return "value error: bad value found";
    case PSPIO_EGSL:
      return "error in GSL";
    case PSPIO_ENOMEM:
      return "malloc failed";
    default:
      return "unknown error code" ;
    }
}
