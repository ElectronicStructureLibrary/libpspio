/* Copyright (C) 2011-2016 Joseba Alberdi <alberdi@hotmail.es>
 *                         Matthieu Verstraete <matthieu.jean.verstraete@gmail.com>
 *                         Micael Oliveira <micael.oliveira@mpsd.mpg.de>
 *                         Yann Pouillon <notifications@materialsevolution.es>
 *
 * This file is part of Libpspio.
 *
 * Libpspio is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, version 3 of the License, or (at your option) any later
 * version.
 *
 * Libpspio is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Libpspio.  If not, see <http://www.gnu.org/licenses/> or write to
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301  USA.
 */

#ifndef PSPIO_ERROR_H
#define PSPIO_ERROR_H

/**
 * @file pspio_error.h 
 * @brief Error handlers
 */

#include <stdlib.h>
#include <stdio.h>

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
  char *routine; /**< routine where the error appeared */
  struct pspio_error_type *next; /**< next error in the chain */
};
typedef struct pspio_error_type pspio_error_t;


/**********************************************************************
 * Routines                                                           *
 **********************************************************************/

/**
 * Add an error to the chain.
 * @param[in] error_id: error code.
 * @param[in] filename: source filename (use NULL if none).
 * @param[in] line: line number in the source file (ignored if filename
 *            is NULL).
 * @param[in] routine: current routine in the source file.
 * @return the error code provided as input (for automation purposes).
 */
int pspio_error_add(int error_id, const char *filename, int line,
		    const char *routine);

/**
 * Fetch and clear the error chain.
 * @return string pointer describing the chain of errors.
 */
char *pspio_error_fetchall(void);

/**
 * Flush and clear the error chain.
 * @param[in] fd: file descriptor for the output.
 * @return error code
 */
void pspio_error_flush(FILE *fd);

/**
 * Clear the error chain.
 */
void pspio_error_free(void);

/**
 * Get the current error status.
 * @param[in] routine: return last error of the specified routine, or the very 
 *            last error if NULL.
 * @return error code
 */
int pspio_error_get_last(const char *routine);

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
 * @param[in] routine: current routine in the source file.
 * @return string with error message.
 */
void pspio_error_show(int error_id, const char *filename, int line,
		      const char *routine);

/**
 * Returns a string with error description.
 * @param[in] pspio_errorid: integer identifying the error.
 * @return string with error message.
 */
const char *pspio_error_string(const int error_id);


/**********************************************************************
 * Macros                                                             *
 **********************************************************************/

/**
 * Switchable macro to log errors to stderr
 * @note: must be defined first
 */
#if ( DEBUG_LEVEL >= 2 )
#define DEBUG_LOG fprintf(stderr, "DEBUG: %s(%s):%d - error_id = %d\n", __FILE__, __func__, __LINE__, pspio_error_get_last(__func__));
#else
#define DEBUG_LOG
#endif

/**
 * Macro to break loops when there are deferred errors, in order to propagate
 * error conditions from inner loops to outer loops
 */
#define BREAK_ON_DEFERRED_ERROR \
  DEBUG_LOG \
  if ( pspio_error_get_last(__func__) != PSPIO_SUCCESS ) { \
    break; \
  }

/**
 * Deferred error handler macro for function calls, when it is necessary
 * to continue executing the code despite the error
 * @param[in] function_call: the function to be called with all its parameters
 */
#define DEFER_FUNC_ERROR(function_call) \
  pspio_error_add(function_call, __FILE__, __LINE__, __func__); \
  DEBUG_LOG

/**
 * Deferred error handler macro for unsatisfied conditions, when it is
 * necessary to continue executing the code despite the error
 * @param[in] condition: condition to check
 * @param[in] error_id: error code to set
 */
#define DEFER_TEST_ERROR(condition, error_id) \
  if ( !(condition) ) {		    \
    pspio_error_add(error_id, __FILE__, __LINE__, __func__); \
  } \
  DEBUG_LOG

/**
 * Macro to break out of a loop when a condition is unsatisfied
 * @param[in] condition: condition to check
 * @param[in] error_id: error code to set before breaking out of the loop
 */
#define FULFILL_OR_BREAK(condition, error_id) \
  if (!(condition)) { \
    pspio_error_add(error_id, __FILE__, __LINE__, __func__); \
    DEBUG_LOG \
    break; \
  }

/**
 * Macro to exit the program when a condition is unsatisfied
 * @param[in] condition: condition to check
 * @param[in] error_id: error code to set before aborting
 */
#define FULFILL_OR_EXIT(condition, error_id) \
  if ( !(condition) ) {		    \
    pspio_error_add(error_id, __FILE__, __LINE__, __func__); \
    DEBUG_LOG \
    pspio_error_flush(stderr); \
    exit(1); \
  } 

/**
 * Macro to return from a routine when a condition is unsatisfied
 * @param[in] condition: condition to check
 * @param[in] error_id: error code to set before returning
 */
#define FULFILL_OR_RETURN(condition, error_id) \
  if (!(condition)) {		    \
    pspio_error_add(error_id, __FILE__, __LINE__, __func__); \
    DEBUG_LOG \
    return pspio_error_get_last(__func__); \
  }

/**
 * Error handler macro for deferred errors
 */
#define RETURN_ON_DEFERRED_ERROR \
  DEBUG_LOG \
  if ( pspio_error_get_last(__func__) != PSPIO_SUCCESS ) { \
    return pspio_error_get_last(__func__); \
  }

/**
 * Error handler macro ensuring that errors are properly registered
 * before returning from a function
 * @param[in] error_id: error code
 */
#define RETURN_WITH_ERROR(error_id) \
  pspio_error_add(error_id, __FILE__, __LINE__, __func__); \
  DEBUG_LOG \
  return pspio_error_get_last(__func__);

/**
 * Break the current loop when a pspio function call fails
 * @param[in] function_call: the function to be called with all its parameters
 */
#define SUCCEED_OR_BREAK(function_call) \
  if ( pspio_error_add(function_call, __FILE__, __LINE__, __func__) != PSPIO_SUCCESS ) { \
    DEBUG_LOG \
    break; \
  } else { \
    DEBUG_LOG \
  }

/**
 * Return when a pspio function call fails
 * @param[in] function_call: the function to be called with all its parameters
 */
#define SUCCEED_OR_RETURN(function_call) \
  if ( pspio_error_add(function_call, __FILE__, __LINE__, __func__) != PSPIO_SUCCESS ) { \
    DEBUG_LOG \
    return pspio_error_get_last(NULL); \
  } else { \
    DEBUG_LOG \
  }

/**
 * Macro to skip routine execution on error
 * @param[in] routine_call: the routine to be called with all its parameters
 */
#define SKIP_CALL_ON_ERROR(routine_call) \
  DEBUG_LOG \
  if ( pspio_error_get_last(__func__) == PSPIO_SUCCESS ) { \
    routine_call; \
  }

/**
 * Macro to skip function execution on error
 * @param[in] function_call: the function to be called with all its parameters
 */
#define SKIP_FUNC_ON_ERROR(function_call) \
  DEBUG_LOG \
  if ( pspio_error_get_last(__func__) == PSPIO_SUCCESS ) { \
    pspio_error_add(function_call, __FILE__, __LINE__, __func__); \
  }

/**
 * Macro to skip a test on error
 * @param[in] function_call: the function to be called with all its parameters
 */
#define SKIP_TEST_ON_ERROR(condition, error_id) \
  DEBUG_LOG \
  if ( (pspio_error_get_last(__func__) == PSPIO_SUCCESS) && !(condition) ) { \
    pspio_error_add(error_id, __FILE__, __LINE__, __func__); \
  }

#endif
