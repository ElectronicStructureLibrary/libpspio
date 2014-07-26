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
 * @file pspio_info.c
 * @brief Libpspio version information 
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "pspio_error.h"
#include "pspio_info.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

void pspio_version(int *major, int *minor, int *micro) {
  assert((major != NULL) && (minor != NULL) && (micro != NULL));
#if !defined HAVE_CONFIG_H
  return PSPIO_ERROR;
#endif

  const char *version_string = PACKAGE_VERSION;

  *major = -1;
  *minor = -1;
  *micro = -1;
  sscanf(version_string,"%d.%d.%d",major,minor,micro);
}


void pspio_info_string(char *info) {
  assert(info == NULL);
#if !defined HAVE_CONFIG_H
  return PSPIO_ERROR;
#endif

  const char *package_string = PACKAGE_STRING;

  int s = strlen(package_string);
  info = (char *) malloc (s + 1);
  assert(info != NULL);
  strncpy(info, package_string, s);
  info[s] = '\0';
}
