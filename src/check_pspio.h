/*
 Copyright (C) 2015 M. Oliveira

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

*/

#ifndef CHECK_PSPIO_H
#define CHECK_PSPIO_H

/**
 * @file check_pspio.h 
 * @brief pspsio check testsuites
 */


Suite *make_error_suite(void);
Suite *make_qn_suite(void);
Suite *make_mesh_suite(void);
Suite *make_meshfunc_suite(void);
Suite *make_potential_suite(void);
Suite *make_projector_suite(void);

#endif
