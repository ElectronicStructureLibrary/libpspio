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

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


/* The following is a VERY bad trick. Whoever finds something better
 * is welcome to speak. */
static int pspio_error_tmp_id = 0;
static pspio_error_t *pspio_error_chain = NULL;


int pspio_error_add(const char *filename, const int line) {
  int error_id, s;
  pspio_error_t *last_err;

  /* Save current status, as it will be overwritten when allocating memory */
  error_id = pspio_error_tmp_id;

  if ( pspio_error_chain == NULL ) {
    pspio_error_chain = (pspio_error_t *)malloc(sizeof(pspio_error_t));
    HANDLE_FATAL_ERROR(pspio_error_chain == NULL, PSPIO_ENOMEM)

    pspio_error_chain->id = error_id;
    pspio_error_chain->line = line;
    pspio_error_chain->next = NULL;
    s = strlen(filename);
    pspio_error_chain->filename = (char *)malloc(s + 1);
    HANDLE_FATAL_ERROR(pspio_error_chain->filename == NULL, PSPIO_ENOMEM)
    memcpy(pspio_error_chain->filename,filename,s);
    pspio_error_chain->filename[s] = 0;

    pspio_error_tmp_id = PSPIO_SUCCESS;
    return PSPIO_SUCCESS;
  }

  last_err = pspio_error_chain;
  while ( last_err->next != NULL ) {
    last_err = last_err->next;
  }

  last_err->next = (pspio_error_t *)malloc(sizeof(pspio_error_t));
  HANDLE_FATAL_ERROR(last_err->next == NULL, PSPIO_ENOMEM)
  last_err = last_err->next;
  last_err->id = error_id;
  last_err->line = line;
  last_err->next = NULL;
  s = strlen(filename);
  last_err->filename = (char *)malloc(s + 1);
  HANDLE_FATAL_ERROR(last_err->filename == NULL, PSPIO_ENOMEM)
  memcpy(last_err->filename,filename,s);
  last_err->filename[s] = 0;

  pspio_error_tmp_id = PSPIO_SUCCESS;
  return PSPIO_SUCCESS;
}


int pspio_error_flush(void) {
  pspio_error_t *cursor = pspio_error_chain;

  while ( cursor != NULL ) {
    printf("libpspio: error: %s\n          at %s:%d\n",
      pspio_error_str(cursor->id), cursor->filename, cursor->line);
    cursor = cursor->next;
  }

  return pspio_error_free();
}


int pspio_error_free(void) {
  pspio_error_t *first_err;

  while ( pspio_error_chain != NULL ) {
    first_err = pspio_error_chain;
    pspio_error_chain = pspio_error_chain->next;
    free(first_err->filename);
    free(first_err);
  }

  return PSPIO_SUCCESS;
}


int pspio_error_get(void) {
  return pspio_error_tmp_id;
}


int pspio_error_len(void) {
  int n = 0;
  pspio_error_t *cursor = pspio_error_chain;

  while ( cursor != NULL ) {
    n++;
    cursor = cursor->next;
  }

  return n;
}


pspio_error_t *pspio_error_pop(void) {
  pspio_error_t *first_error = NULL;

  if ( pspio_error_chain != NULL ) {
    first_error = pspio_error_chain;
    pspio_error_chain = pspio_error_chain->next;
  }

  return first_error;
}


void pspio_error_set(const int error_id) {
  pspio_error_tmp_id = error_id;
}


void pspio_error_show(const int error_id, const char *filename,
       const int line) {
  printf("libpspio: ERROR: %s\n",pspio_error_str(error_id));
  if ( filename != NULL ) {
    printf("  at %s:%d\n",filename,line);
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
    case PSPIO_ENOSUPPORT:
      return "No supported option in the pseudo-potential file";
    default:
      return "unknown error code" ;
    }
}
