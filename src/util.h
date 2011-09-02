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

#ifndef UTIL_H
#define UTIL_H

/**
 * @file util.h
 * @brief include file with some useful macros and defines 
 */

/**********************************************************************
 * Macros                                                             *
 **********************************************************************/

#define min(x,y)  ((x<y) ? (x) : (y))
#define max(x,y)  ((x<y) ? (y) : (x))

#define LJ_TO_I(l,j)  (l+(int)j)


/**********************************************************************
 * Routines                                                           *
 **********************************************************************/

/**
 * Given the atomic number, returns the symbol
 *
 * @param[in] z: atomic number
 * @param[out] symbol: the symbol
 * @return error code
 * @note z should be larger than 0 and smaller than 113.
 */
int z_to_symbol(const double z, char *symbol);


/**
 * Given the symbol, returns the atomic number
 *
 * @param[in] symbol: the symbol
 * @param[out] z: atomic number
 * @return error code
 */
int symbol_to_z(const char *symbol, double z);


#endif
