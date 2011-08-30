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

#ifndef PSPIO_ERROR_H
#define PSPIO_ERROR_H

/**
 * @file pspio_error.h 
 * @brief Error codes and handlers
 */

#include <stdlib.h>

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


#define PSPIO_SUCCESS  0 
#define PSPIO_ERROR   -1
#define PSPIO_ENOFILE   1
#define PSPIO_EIO    2
#define PSPIO_EVALUE 3
#define PSPIO_EFILE_FORMAT 4
#define PSPIO_ETYPE 5
#define PSPIO_EGSL  6
#define PSPIO_ENOMEM 7


/**
 * Returns a string with error description.
 * @param[in] error_id: integer identifying the error.
 * @return string with error message.
 */
const char * pspio_error_str (const int pspio_errorid);


/**
 * Global error handling structure
 */
typedef struct {
  int id; /**< ID of the error */
  char *filename; /**< name of the file where the error appeared */
  long line; /**< line number in the file where the error appeared */
  struct pspio_error_t *next; /**< next error in the chain */
} pspio_error_t;

/* The following is a VERY bad trick. Whoever finds better is welcome to
 * speak. */
static int pspio_error_tmp_id = 0;
static pspio_error_t *pspio_error_chain = NULL;


/**
 * Add an error to the chain
 * @param[in] new_error: new error to add
 */
int pspio_error_add(const char *filename, const long line);


/**
 * Clear the error chain
 */
int pspio_error_free(void);


/**
 * Error handler macro
 * @param[in] FUNCTION_CALL: the function to be called with all parameters
 * @param[in] TYPE_TO_FREE: type of variable to free when error
 * @param[in] VAR_TO_FREE: name of the variable to free when error
 */
#define HANDLE_ERROR(FUNCTION_CALL, TYPE_TO_FREE, VAR_TO_FREE) \
  pspio_error_tmp_id = FUNCTION_CALL; \
  if ( pspio_error_tmp_id != PSPIO_SUCCESS ) { \
    pspio_ ## TYPE_TO_FREE ## _free(VAR_TO_FREE); \
    pspio_error_add(pspio_error_tmp_id, __FILE__, __LINE__); \
    return pspio_error_tmp_id; \
  }
#endif


