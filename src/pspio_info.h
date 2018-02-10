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

/**
 * @file pspio_info.h
 * @brief Libpspio version information 
 */

#if !defined PSPIO_INFO_H
#define PSPIO_INFO_H

/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

/**
 * Provide the version number of the library.
 * @param[out] major major version number, -1 if not found
 * @param[out] minor minor version number, -1 if not found
 * @param[out] micro micro version number, -1 if not found
 */
void pspio_info_version(int *major, int *minor, int *micro);

/**
 * Provide the package string of the library.
 * @param[out] info: package name and version.
 */
void pspio_info_string(char *info);

#endif
