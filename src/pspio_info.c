/* Copyright (C) 2011-2016 Joseba Alberdi <alberdi@hotmail.es>
 *                         Micael Oliveira <micaeljtoliveira@gmail.com>
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

/**
 * @file pspio_info.c
 * @brief Libpspio version information 
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "pspio_info.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#else
#define PACKAGE_VERSION ""
#define PACKAGE_STRING ""
#endif


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

void pspio_info_version(int *major, int *minor, int *micro)
{
  const char *version_string = PACKAGE_VERSION;

  assert((major != NULL) && (minor != NULL) && (micro != NULL));

  *major = -1;
  *minor = -1;
  *micro = -1;
  if ( strlen(version_string) > 0 ) {
    sscanf(version_string,"%d.%d.%d", major, minor, micro);
  }
}

void pspio_info_string(char *info)
{
  size_t s = strlen(PACKAGE_STRING);

  assert(info != NULL);

  strncpy(info, PACKAGE_STRING, s);
  info[s] = '\0';
}
