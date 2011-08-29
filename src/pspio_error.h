/*
 Copyright (C) 2011 J. Alberdi, M. Oliveira, Y. Pouillon, and M. Verstraete

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2, or (at your option)
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 02111-1307, USA.

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

const char * pspio_errorstr (const int pspio_errorid);

#endif PSPIO_ERROR_H
