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

#ifndef UTIL_H
#define UTIL_H

/**
 * @file util.h
 * @brief include file with some useful macros and defines 
 */


/**********************************************************************
 * Defines                                                            *
 **********************************************************************/

# define M_PI 3.14159265358979323846  /* pi */


/**********************************************************************
 * Macros                                                             *
 **********************************************************************/

#define min(x,y)  ((x<y) ? (x) : (y))
#define max(x,y)  ((x<y) ? (y) : (x))

#define LJ_TO_I(x,y)  (x+(int)y)


/**********************************************************************
 * Routines                                                           *
 **********************************************************************/

/**
 * Given the atomic number, returns the symbol
 *
 * @param[in] z: atomic number
 * @param[out] symbol: the symbol
 * @note z should be larger than 0 and smaller than 113.
 * @return error code
 */
int z_to_symbol(double z, char symbol[4]);

/**
 * Given the symbol, returns the atomic number
 *
 * @param[in] symbol: the symbol
 * @param[out] z: atomic number
 * @return error code
 */
int symbol_to_z(const char symbol[4], double *z);

/**
 * Given the values of a function at two points, uses a linear
 * extrapolation to evaluate the function at a third point
 *
 * @param[in] x1, x2: points where we know the values of the function
 * @param[in] f1, f2: known values of the function at x1 and x2
 * @param[in] x: point where we want to evaluate the function
 * @return value of the function at x
 */
double linear_extrapolation(double x1, double x2, double f1,  double f2, double x);

/**
 * Given the pseudopotential generation scheme, returns a string with the scheme name
 *
 * @param[in] scheme: the pseudopotential generation scheme id
 * @return scheme name
 */
char * psp_scheme_name(int scheme);

#endif
