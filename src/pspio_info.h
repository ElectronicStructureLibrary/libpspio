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
