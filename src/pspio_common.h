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

#ifndef PSPIO_COMMON_H
#define PSPIO_COMMON_H

/**
 * @file pspio_common.h
 * @brief include file with low-level stuff 
 */

/**
 * File formats
 */
#define UNKNOWN -1
#define ABINIT_4 1
#define ABINIT_5 2
#define ABINIT_6 3
#define ABINIT_HGH 4
#define ABINIT_GTH 5
#define ATOM 6
#define FHI98PP 7
#define SIESTA 8
#define UPF 9

#define MAX_LEN 256
#define STRLEN_TITLE 80

#endif
