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


/**********************************************************************
 * Data structures                                                    *
 **********************************************************************/

/**
 * Global error handling structure
 */
typedef struct {
  int id; /**< ID of the error */
  char *filename; /**< name of the file where the error appeared */
  int line; /**< line number in the file where the error appeared */
  struct pspio_error_t *next; /**< next error in the chain */
} pspio_error_t;

/* The following is a VERY bad trick. Whoever finds better is welcome to
 * speak. */
static int pspio_error_tmp_id = 0;
static pspio_error_t *pspio_error_chain = NULL;


/**********************************************************************
 * Routines                                                           *
 **********************************************************************/

/**
 * Add an error to the chain
 * @param[in] new_error: new error to add
 * @return error code
 */
int pspio_error_add(const char *filename, const int line);


/**
 * Clear the error chain
 * @return error code
 */
int pspio_error_free(void);


/**
 * Pop the first available error
 * @return error structure pointer
 */
pspio_error_t *pspio_error_pop(void);


/**
 * Displays an error message.
 * @param[in] error_id: integer identifying the error.
 * @param[in] filename: source filename (use NULL if none).
 * @param[in] line: line number in the source file (ignored if filename
 *            is NULL).
 * @return string with error message.
 */
void pspio_error_show(const int error_id, const char *filename,
       const int line);


/**
 * Returns a string with error description.
 * @param[in] error_id: integer identifying the error.
 * @return string with error message.
 */
const char *pspio_error_str(const int pspio_errorid);


/**********************************************************************
 * Macros                                                             *
 **********************************************************************/

/**
 * Libpspio-specific assert
 * @param[in] condition: condition to check
 * @param[in] error_id: error code to set if condition is false
 */
#define ASSERT(condition, error_id) \
  pspio_error_tmp_id = ( condition ) ? PSPIO_SUCCESS : error_id; \
  HANDLE_ERROR(pspio_error_tmp_id)


/**
 * Error handler macro that frees memory upon error after calling a
 * function
 * @param[in] function_call: the function to be called with all parameters
 * @param[in] type_to_free: type of variable to free when error
 * @param[in] var_to_free: name of the variable to free when error
 */
#define HANDLE_ALLOC_ERROR(function_call, type_to_free, var_to_free) \
  pspio_error_tmp_id = function_call; \
  if ( pspio_error_tmp_id != PSPIO_SUCCESS ) { \
    pspio_ ## type_to_free ## _free(var_to_free); \
    pspio_error_add(__FILE__, __LINE__); \
    return pspio_error_tmp_id; \
  }


/**
 * Error handler macro for fatal errors
 * @param[in] error_id: error code to set before aborting
 */
#define HANDLE_FATAL_ERROR(condition, error_id) \
  pspio_error_tmp_id = ( condition ) ? error_id : PSPIO_SUCCESS; \
  if ( pspio_error_tmp_id != PSPIO_SUCCESS ) { \
    pspio_error_show(pspio_error_tmp_id, __FILE__, __LINE__); \
    exit(1); \
  }


/**
 * Error handler macro for function calls
 * @param[in] function_call: the function to be called with all parameters
 */
#define HANDLE_FUNC_ERROR(function_call) \
  pspio_error_tmp_id = function_call; \
  if ( pspio_error_tmp_id != PSPIO_SUCCESS ) { \
    pspio_error_add(__FILE__, __LINE__); \
    return pspio_error_tmp_id; \
  }


/**
 * Basic error handler macro
 * @param[id] error_id: error code to check
 */
#define HANDLE_ERROR(error_id) \
  if ( error_id != PSPIO_SUCCESS ) { \
    pspio_error_tmp_id = error_id; \
    pspio_error_add(__FILE__, __LINE__); \
    return error_id; \
  }

#endif
