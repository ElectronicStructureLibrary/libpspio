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
static int pspio_error_tmp_id = PSPIO_SUCCESS;
static pspio_error_t *pspio_error_chain = NULL;


void pspio_error_add(const char *filename, const int line) {
  int s;
  pspio_error_t *last_err;

  /* Notes:
       * errors in this routine must be fatal, in order to avoid
         infinite loops;
       * the error status must be left untouched.
   */
  if ( pspio_error_chain == NULL ) {
    pspio_error_chain = (pspio_error_t *) malloc (sizeof(pspio_error_t));
    ASSERT(pspio_error_chain != NULL, PSPIO_ENOMEM)

    pspio_error_chain->id = pspio_error_tmp_id;
    pspio_error_chain->line = line;
    pspio_error_chain->next = NULL;
    s = strlen(filename);
    pspio_error_chain->filename = (char *) malloc (s + 1);
    ASSERT(pspio_error_chain->filename != NULL, PSPIO_ENOMEM)
    memcpy(pspio_error_chain->filename, filename, s);
    pspio_error_chain->filename[s] = '\0';
  } else {
    last_err = pspio_error_chain;
    while ( last_err->next != NULL ) {
      last_err = last_err->next;
    }

    last_err->next = (pspio_error_t *) malloc (sizeof(pspio_error_t));
    ASSERT(last_err->next != NULL, PSPIO_ENOMEM)
    last_err = last_err->next;
    last_err->id = pspio_error_tmp_id;
    last_err->line = line;
    last_err->next = NULL;
    s = strlen(filename);
    last_err->filename = (char *) malloc (s + 1);
    ASSERT(last_err->filename != NULL, PSPIO_ENOMEM)
    memcpy(last_err->filename, filename, s);
    last_err->filename[s] = '\0';
  }
}


int pspio_error_fetchall(char **err_str) {
  char buf[8];
  char *tmp_str;
  int err_len;
  pspio_error_t *cursor = pspio_error_chain;

  if ( cursor != NULL ) {
    *err_str  = (char *) malloc (20*sizeof(char));
    ASSERT(*err_str != NULL, PSPIO_ENOMEM);
    sprintf(*err_str, "%s", "libpspio: ERROR:\n");
  }

  while ( cursor != NULL ) {
    err_len  = 16;
    err_len += strlen(pspio_error_str(cursor->id));
    err_len += strlen(cursor->filename);
    sprintf(buf, "%d", cursor->line);
    err_len += strlen(buf);

    tmp_str  = (char *) malloc ((err_len+1)*sizeof(char));
    ASSERT(tmp_str != NULL, PSPIO_ENOMEM);
    sprintf(tmp_str, "  * %s\n      at %s:%d\n",
      pspio_error_str(cursor->id), cursor->filename, cursor->line);
    fflush(stdout);
    *err_str  = realloc(*err_str, strlen(*err_str)+err_len+1);
    strcat(*err_str, tmp_str);
    free(tmp_str);

    cursor = cursor->next;
  }

  return pspio_error_free();
}


int pspio_error_flush(void) {
  char *err_str = NULL;
  int eid;

  eid = pspio_error_fetchall(&err_str);
  printf("%s", err_str);

  if ( eid != PSPIO_SUCCESS ) {
    printf("libpspio: FATAL: pspio_error_free() failed.\n");
  }

  return eid;
}


int pspio_error_free(void) {
  pspio_error_t *first_err;

  while ( pspio_error_chain != NULL ) {
    first_err = pspio_error_chain;
    pspio_error_chain = pspio_error_chain->next;
    free(first_err->filename);
    free(first_err);
  }

  pspio_error_tmp_id = PSPIO_SUCCESS;
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
  printf("libpspio: ERROR: %s\n", pspio_error_str(error_id));
  if ( filename != NULL ) {
    printf("  at %s:%d\n", filename, line);
  }
}


const char *pspio_error_str(const int pspio_errorid) {
  switch (pspio_errorid)
    {
    case PSPIO_SUCCESS:
      return "success (PSPIO_SUCCESS)" ;
    case PSPIO_ERROR:
      return "error (PSPIO_ERROR)" ;
    case PSPIO_EFILE_CORRUPT:
      return "file corrupted (PSPIO_EFILE_CORRUPT)";
    case PSPIO_EFILE_FORMAT:
      return "unknown file format (PSPIO_EFILE_FORMAT)";
    case PSPIO_EGSL:
      return "error in GSL (PSPIO_EGSL)";
    case PSPIO_EIO:
      return "input/output error (PSPIO_EIO)" ;
    case PSPIO_ENOFILE:
      return "file does not exist (PSPIO_ENOFILE)" ;
    case PSPIO_ENOMEM:
      return "malloc failed (PSPIO_ENOMEM)";
    case PSPIO_ENOSUPPORT:
      return "Unsupported option in the pseudo-potential file (PSPIO_ENOSUPPORT)";
    case PSPIO_EVALUE:
      return "value error: bad value found (PSPIO_EVALUE)";
    default:
      return "unknown error code" ;
    }
}
