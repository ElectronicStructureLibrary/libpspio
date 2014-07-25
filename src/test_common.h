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

/**
 * @file test_common.h
 * @brief data shared by test programs 
 */

#ifndef TEST_COMMON_H
#define TEST_COMMON_H

#include "pspio_error.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


/**********************************************************************
 * CPP macros                                                         *
 **********************************************************************/

/**
 * Checks an error status
 * @param[in] stat_var: variable used to store the status
 * @param[in] stat_val: expected status value
 * @return 0 on success, 1 on error
 */
#define CHECK_STAT(stat_var, stat_val) \
  if ( stat_var != stat_val ) { \
    pspio_error_flush(stdout); \
    return 1; \
  }


/**
 * Checks the error status and the length of a variable
 * @param[in] stat_var: variable used to store the status
 * @param[in] stat_val: expected status value
 * @param[in] len_var: variable used to store the length
 * @param[in] len_val: expected length value
 * @return 0 on success, 1 on error
 */
#define CHECK_STAT_LEN(stat_var, stat_val, len_var, len_val) \
  if ( (stat_var != stat_val) || ( len_var != len_val) ) { \
    pspio_error_flush(stdout); \
    return 1; \
  }


/**
 * Displays information on stderr when debug level is nonzero
 * @param args...: what to display, as when calling printf
 */
#define DEBUG_PRINT(...) \
  if ( DEBUG_LEVEL > 0 ) { \
    fprintf(stderr, __VA_ARGS__); \
  }


/**
 * Shows the status of a pointer
 * @param[in] ptr: pointer to check
 */
#define PTR_STAT_SHOW(ptr) \
  DEBUG_PRINT("*** Pointer %s is %s ***\n", #ptr, \
    ( ptr == NULL ) ? "NULL" : "not NULL");

#endif
