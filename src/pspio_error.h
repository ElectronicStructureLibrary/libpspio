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

*/

#ifndef PSPIO_ERROR_H
#define PSPIO_ERROR_H

/**
 * @file pspio_error.h 
 * @brief Error handlers
 */

#include <stdlib.h>
#include "pspio_common.h"


/**********************************************************************
 * Data structures                                                    *
 **********************************************************************/

/**
 * Global error handling structure
 */
struct pspio_error_type {
  int id; /**< ID of the error */
  char *filename; /**< name of the file where the error appeared */
  int line; /**< line number in the file where the error appeared */
  struct pspio_error_type *next; /**< next error in the chain */
};
typedef struct pspio_error_type pspio_error_t;


/**********************************************************************
 * Routines                                                           *
 **********************************************************************/

/**
 * Add an error to the chain.
 * @param[in] filename: source filename (use NULL if none).
 * @param[in] line: line number in the source file (ignored if filename
 *            is NULL).
 */
void pspio_error_add(const int error_id, const char *filename, const int line);


/**
 * Fetch and clear the error chain.
 * @param[out] err_str: string pointer describing the chain of errors.
 * @return error code
 */
int pspio_error_fetchall(char **err_str);


/**
 * Flush and clear the error chain.
 * @return error code
 */
int pspio_error_flush(void);


/**
 * Clear the error chain.
 * @return error code
 */
int pspio_error_free(void);


/**
 * Get the current error status.
 * @return error code
 */
int pspio_error_get_last(void);


/**
 * Get the length of the error chain.
 * @return length of the error chain
 */
int pspio_error_len(void);


/**
 * Pop the first available error.
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
 * @param[in] pspio_errorid: integer identifying the error.
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
  if ( !(condition) ) { \
    pspio_error_add(error_id, __FILE__, __LINE__); \
    pspio_error_show(pspio_error_get_last(), __FILE__, __LINE__); \
    exit(1); \
  }


/**
 * Error checker macro
 * @param[in] condition: condition to check
 * @param[in] error_id: error code to set before aborting
 */
#define CHECK_ERROR(condition, error_id) \
  if (!(condition)) {		    \
    pspio_error_add(error_id, __FILE__, __LINE__); \
    return error_id; \
  } 


/**
 * Error handler macro for function calls
 * @param[in] function_call: the function to be called with all parameters
 */
#define HANDLE_FUNC_ERROR(function_call) \
  pspio_error_add(function_call, __FILE__, __LINE__); \
  if ( pspio_error_get_last() != PSPIO_SUCCESS ) { \
    return pspio_error_get_last(); \
  }


/**
 * Basic error handler macro
 * @param[in] error_id: error code to check
 */
#define HANDLE_ERROR(error_id) \
  if ( error_id != PSPIO_SUCCESS ) { \
    pspio_error_add(error_id, __FILE__, __LINE__); \
    return error_id; \
  }


/**
 * Basic error handler without return call
 */
#define TRIGGER_ERROR(condition, error_id) \
  if ( !(condition) ) { \
    pspio_error_add(error_id, __FILE__, __LINE__); \
  }

#endif
