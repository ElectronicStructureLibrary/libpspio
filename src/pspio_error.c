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
#include <string.h>
#include "pspio_error.h"


int pspio_error_add(const char *filename, const long line) {
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
