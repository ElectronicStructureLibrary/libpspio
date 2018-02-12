/* Copyright (C) 2011-2016 Joseba Alberdi <alberdi@hotmail.es>
 *                         Matthieu Verstraete <matthieu.jean.verstraete@gmail.com>
 *                         Micael Oliveira <micael.oliveira@mpsd.mpg.de>
 *                         Yann Pouillon <devops@materialsevolution.es>
 *
 * This file is part of Libpspio.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * version 2.0. If a copy of the MPL was not distributed with this file, You
 * can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Libpspio is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the Mozilla Public License version 2.0 for
 * more details.
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
 * Macro to break loops when there are deferred errors, in order to propagate
 * error conditions from inner loops to outer loops
 */
#define BREAK_ON_DEFERRED_ERROR \
  if ( pspio_error_get_last(__func__) != PSPIO_SUCCESS ) { \
    break; \
  }

/**
 * Deferred error handler macro for function calls, when it is necessary
 * to continue executing the code despite the error
 * @param[in] function_call: the function to be called with all its parameters
 */
#define DEFER_FUNC_ERROR(function_call) \
  pspio_error_add(function_call, __FILE__, __LINE__, __func__);

/**
 * Deferred error handler macro for unsatisfied conditions, when it is
 * necessary to continue executing the code despite the error
 * @param[in] condition: condition to check
 * @param[in] error_id: error code to set
 */
#define DEFER_TEST_ERROR(condition, error_id) \
  if ( !(condition) ) {		    \
    pspio_error_add(error_id, __FILE__, __LINE__, __func__); \
  } 

/**
 * Macro to break out of a loop when a condition is unsatisfied
 * @param[in] condition: condition to check
 * @param[in] error_id: error code to set before breaking out of the loop
 */
#define FULFILL_OR_BREAK(condition, error_id) \
  if (!(condition)) { \
    pspio_error_add(error_id, __FILE__, __LINE__, __func__); \
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
    return pspio_error_add(error_id, __FILE__, __LINE__, __func__); \
  }

/**
 * Error handler macro for deferred errors
 */
#define RETURN_ON_DEFERRED_ERROR \
  if ( pspio_error_get_last(__func__) != PSPIO_SUCCESS ) { \
    return pspio_error_get_last(__func__); \
  }

/**
 * Error handler macro ensuring that errors are properly registered
 * before returning from a function
 * @param[in] error_id: error code
 */
#define RETURN_WITH_ERROR(error_id) \
    return pspio_error_add(error_id, __FILE__, __LINE__, __func__);

/**
 * Break the current loop when a pspio function call fails
 * @param[in] function_call: the function to be called with all its parameters
 */
#define SUCCEED_OR_BREAK(function_call) \
  if ( pspio_error_add(function_call, __FILE__, __LINE__, __func__) != PSPIO_SUCCESS ) { \
    break; \
  }

/**
 * Return when a pspio function call fails
 * @param[in] function_call: the function to be called with all its parameters
 */
#define SUCCEED_OR_RETURN(function_call) \
  if ( pspio_error_add(function_call, __FILE__, __LINE__, __func__) != PSPIO_SUCCESS ) { \
    return pspio_error_get_last(NULL); \
  }

/**
 * Macro to skip routine execution on error
 * @param[in] routine_call: the routine to be called with all its parameters
 */
#define SKIP_CALL_ON_ERROR(routine_call) \
  if ( pspio_error_get_last(__func__) == PSPIO_SUCCESS ) { \
    routine_call; \
  }

/**
 * Macro to skip function execution on error
 * @param[in] function_call: the function to be called with all its parameters
 */
#define SKIP_FUNC_ON_ERROR(function_call) \
  if ( pspio_error_get_last(__func__) == PSPIO_SUCCESS ) { \
    pspio_error_add(function_call, __FILE__, __LINE__, __func__); \
  }

/**
 * Macro to skip a test on error
 * @param[in] function_call: the function to be called with all its parameters
 */
#define SKIP_TEST_ON_ERROR(condition, error_id) \
  if ( (pspio_error_get_last(__func__) == PSPIO_SUCCESS) && !(condition) ) { \
    pspio_error_add(error_id, __FILE__, __LINE__, __func__); \
  }

#endif
