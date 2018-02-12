/* Copyright (C) 2011-2016 Joseba Alberdi <alberdi@hotmail.es>
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
