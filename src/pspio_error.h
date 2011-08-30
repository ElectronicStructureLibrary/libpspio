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
#define PSPIO_ENOFILE   1
#define PSPIO_EIO    2
#define PSPIO_EVALUE 3
#define PSPIO_EFILE_FORMAT 4
#define PSPIO_ETYPE 5
#define PSPIO_EGSL  6
#define PSPIO_ENOMEM 7


/**
 * Returns a string with error description.
 * @param[in] error_id: integer identifying the error.
 * @return string with error message.
 */
const char * pspio_error_str (const int pspio_errorid);

#endif
