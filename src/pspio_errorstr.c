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

/** @file pspio_errorstr.c
 * @brief returns a string describing an error
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "pspio_error.h"

const char * pspio_errorstr (const int pspio_errorid){
  switch (pspio_errorid)
    {
    case PSPIO_SUCCESS:
      return "success" ;
    case PSPIO_ERROR:
      return "error" ;
    case PSPIO_ENOFILE:
      return "file does not exist" ;
    case PSPIO_EIO:
      return "error in I/O" ;
    case PSPIO_EVALUE:
      return "value error: bad value found";
    case PSPIO_EGSL:
      return "error in GSL";
    case PSPIO_ENOMEM:
      return "malloc failed";
    default:
      return "unknown error code" ;
    }
}
