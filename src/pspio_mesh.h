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

#ifndef PSPIO_MESH_H
#define PSPIO_MESH_H

#include "pspio_error.h"

/**
* Mesh types
*/
#define LOG1   1
#define LOG2   2
#define LINEAR 3

/**
* Mesh structure
*/
typedef struct{
  int type;    /**< Type of mesh */
  int np;      /**< Number of points in mesh */
  double a, b; /**< Mesh parameters */
  double *r;  /**< Mesh points */
} pspio_mesh_t;

//typedef struct pspio_mesh_struct pspio_mesh_t;

int pspio_mesh_set_type(pspio_mesh_t *m, int type);

int pspio_mesh_set_parameters(pspio_mesh_t *m, const double a, const double b);

int pspio_mesh_set_points(pspio_mesh_t *m, const int np, double r[]);

#endif
