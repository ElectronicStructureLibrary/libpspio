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

#ifndef PSPIO_ERROR_H
#define PSPIO_ERROR_H

/**
 * @file pspio_error.h 
 * @brief Error codes
 */
#define PSPIO_SUCCESS  0 
#define PSPIO_ERROR   -1
#define PSPIO_NOFILE   1
#define PSPIO_IOERR    2
#define PSPIO_VALUE_ERROR 3
#define PSPIO_FILE_FORMAT_ERROR 4
#define PSPIO_TYPE_ERROR 5

const char * pspio_errorstr (const int pspio_errorid);

#endif
