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

*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "pspio_error.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif

/* Store successive errors in a chain */
static pspio_error_t *pspio_error_chain = NULL;

int pspio_error_add(const int error_id, const char *filename,
       const int line, const char *routine) {
  int s;
  pspio_error_t *last_err;

  /* Notes:
       * this routine cannot call any error macro, in order to avoid
         infinite loops;
       * errors in this routine must be fatal, in order to avoid
         infinite loops;
       * PSPIO_SUCCESS must always be ignored;
       * this routine returns the submitted error ID for automation
         purposes.
   */

  if ( error_id == PSPIO_SUCCESS ) return error_id;

  if ( pspio_error_chain == NULL ) {
    pspio_error_chain = malloc (sizeof(pspio_error_t));
    if ( pspio_error_chain == NULL ) {
      pspio_error_show(PSPIO_ENOMEM, __FILE__, __LINE__, __func__);
      exit(1);
    }

    pspio_error_chain->id = error_id;
    pspio_error_chain->line = line;
    pspio_error_chain->next = NULL;
    s = strlen(filename);
    pspio_error_chain->filename = (char *) malloc (s + 1);
    if ( pspio_error_chain->filename == NULL ) {
      pspio_error_show(PSPIO_ENOMEM, __FILE__, __LINE__, __func__);
      exit(1);
    }
    memcpy(pspio_error_chain->filename, filename, s);
    pspio_error_chain->filename[s] = '\0';
    s = strlen(routine);
    pspio_error_chain->routine = (char *) malloc (s + 1);
    if ( pspio_error_chain->routine == NULL ) {
      pspio_error_show(PSPIO_ENOMEM, __FILE__, __LINE__, __func__);
      exit(1);
    }
    memcpy(pspio_error_chain->routine, routine, s);
    pspio_error_chain->routine[s] = '\0';
  } else {
    last_err = pspio_error_chain;
    while ( last_err->next != NULL ) {
      last_err = last_err->next;
    }

    last_err->next = (pspio_error_t *) malloc (sizeof(pspio_error_t));
    if ( last_err->next == NULL ) {
      pspio_error_show(PSPIO_ENOMEM, __FILE__, __LINE__, __func__);
      exit(1);
    }
    last_err = last_err->next;
    last_err->id = error_id;
    last_err->line = line;
    last_err->next = NULL;
    s = strlen(filename);
    last_err->filename = (char *) malloc (s + 1);
    if ( last_err->filename == NULL ) {
      pspio_error_show(PSPIO_ENOMEM, __FILE__, __LINE__, __func__);
      exit(1);
    }
    memcpy(last_err->filename, filename, s);
    last_err->filename[s] = '\0';
    s = strlen(routine);
    last_err->routine = (char *) malloc (s + 1);
    if ( last_err->routine == NULL ) {
      pspio_error_show(PSPIO_ENOMEM, __FILE__, __LINE__, __func__);
      exit(1);
    }
    memcpy(last_err->routine, routine, s);
    last_err->routine[s] = '\0';
  }

  return error_id;
}


void pspio_error_fetchall(char **err_str) {
  char buf[8];
  char *tmp_str;
  int err_len;
  pspio_error_t *cursor = pspio_error_chain;

  *err_str = NULL;

  if ( cursor != NULL ) {
    *err_str  = (char *) malloc (20*sizeof(char));
    assert(*err_str != NULL);
    sprintf(*err_str, "%s\n", "libpspio: ERROR:");
  }

  while ( cursor != NULL ) {
    assert(cursor->filename != NULL);
    assert(cursor->routine != NULL);

    err_len  = 18;
    err_len += strlen(pspio_error_str(cursor->id));
    err_len += strlen(cursor->filename);
    err_len += strlen(cursor->routine);
    sprintf(buf, "%d", cursor->line);
    err_len += strlen(buf);

    tmp_str  = (char *) malloc ((err_len+1)*sizeof(char));
    assert(tmp_str != NULL);
    sprintf(tmp_str, "  * in %s(%s):%d:\n      %s\n",
      cursor->filename, cursor->routine, cursor->line,
      pspio_error_str(cursor->id));
    *err_str = realloc(*err_str, strlen(*err_str)+err_len+1);
    if ( *err_str == NULL ) {
      fprintf(stderr,
        "libpspio: FATAL:\n      could not build error message.\n");
      exit(1);
    }
    strcat(*err_str, tmp_str);
    free(tmp_str);

    cursor = cursor->next;
  }

  pspio_error_free();
}


void pspio_error_flush(FILE *fd) {
  char *err_str = NULL;

  assert(fd != NULL);

  pspio_error_fetchall(&err_str);
  if ( err_str != NULL ) {
    fprintf(fd, "%s", err_str);
    fflush(fd);
  }
}


void pspio_error_free(void) {
  pspio_error_t *first_err;

  while ( pspio_error_chain != NULL ) {
    first_err = pspio_error_chain;
    pspio_error_chain = pspio_error_chain->next;
    free(first_err->filename);
    free(first_err->routine);
    free(first_err);
  }
}


int pspio_error_get_last(const char *routine) {
  int eid = PSPIO_SUCCESS;
  pspio_error_t *cursor = pspio_error_chain;

  while ( cursor != NULL ) {
    if ( routine == NULL ) {
      eid = cursor->id;
    } else {
      if ( strcmp(cursor->routine, routine) == 0 ) {
        eid = cursor->id;
      }
    }
    cursor = cursor->next;
  }

  return eid;
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


void pspio_error_show(const int error_id, const char *filename,
       const int line, const char *routine) {
  fprintf(stderr, "libpspio: ERROR:\n");
  if ( (filename != NULL) && (routine != NULL) ) {
    fprintf(stderr, "  * in %s(%s):%d:\n", filename, routine, line);
  }
  fprintf(stderr, "      %s\n", pspio_error_str(error_id));
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
      return "unsupported option in the pseudopotential file (PSPIO_ENOSUPPORT)";
    case PSPIO_EVALUE:
      return "value error: bad value found (PSPIO_EVALUE)";
    default:
      return "unknown error code";
    }
}
