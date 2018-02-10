/* Copyright (C) 2011-2016 Joseba Alberdi <alberdi@hotmail.es>
 *                         Matthieu Verstraete <matthieu.jean.verstraete@gmail.com>
 *                         Micael Oliveira <micael.oliveira@mpsd.mpg.de>
 *                         Yann Pouillon <devops@materialsevolution.es>
 *
 * This file is part of Libpspio.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * version 2.0. If a copy of the MPL was not distributed with this file, You
 * can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Libpspio is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the Mozilla Public License version 2.0 for
 * more details.
 */

/**
 * @file pspio_mesh.h
 * @brief header file for the handling of the mesh 
 */

#ifndef PSPIO_MESH_H
#define PSPIO_MESH_H

#include "pspio_common.h"
#include "pspio_error.h"


/**********************************************************************
 * Data structures                                                    *
 **********************************************************************/

/**
* Mesh structure
*/
typedef struct{
  int type;    /**< Type of mesh */
  double a, b; /**< Mesh parameters */
  int np;      /**< Number of points in mesh */
  double *r;   /**< Mesh points */
  double *rab; /**< Factor required for discrete integration: rab(i) = (dr(x)/dx)_{x=i} */
} pspio_mesh_t;


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

/**
 * Allocates memory and preset mesh structure
 * 
 * @param[in,out] mesh: mesh structure
 * @param[in] np: number of points in the mesh
 * @return error code
 * @note np should be larger than 1.
 */
int pspio_mesh_alloc(pspio_mesh_t **mesh, int np);

/**
 * Initializes the mesh data.
 * @param[in,out] mesh: mesh structure to be initialized
 * @param[in] type: type of mesh. Can be LOG1, LOG2, or LINEAR.
 * @param[in] a: parameter a. The meaning depends on the type of mesh.
 * @param[in] b: parameter b. The meaning depends on the type of mesh.
 * @param[in] r: pointer to mesh radial points.
 * @param[in] rab: pointer to discrete integration factors.
 * @return error code
 * @note The mesh pointer has to be allocated first with the pspio_mesh_alloc
 *       method.
 * @note r and rab should be of size mesh->np.
 */
int pspio_mesh_init(pspio_mesh_t *mesh, int type, double a,
       double b, const double *r, const double *rab);

/**
 * Sets the mesh data from a list of points. The function will try to determine
 * the type of mesh. If it is not able to do it it will set it to
 * PSPIO_MESH_UNKNOWN. It uses a hard-coded tolerance of 5.0e-10 to
 * identify the mesh types.
 * @param[in,out] mesh: mesh structure to set
 * @param[in] r: pointer to mesh radial points
 * @param[in] rab: pointer to discrete integration factors.
 * @note The mesh pointer has to be allocated first with the pspio_mesh_alloc
 *       method.
 * @note r and rab should be of size mesh->np.
 * @note If rab is null it will be determined automatically, otherwise 
 *       consistency will be checked between r and rab.
 */
void pspio_mesh_init_from_points(pspio_mesh_t *mesh, const double *r, const double *rab);

/**
 * Generates the mesh from the mesh type and parameters.
 * @param[in,out] mesh: mesh structure to set
 * @param[in] type: type of mesh. Can be LOG1, LOG2, or LINEAR.
 * @param[in] a: parameter a. The meaning depends on the type of mesh.
 * @param[in] b: parameter b. The meaning depends on the type of mesh.
 * @note The mesh pointer has to be allocated first with the pspio_mesh_alloc
 *       method.
 */
void pspio_mesh_init_from_parameters(pspio_mesh_t *mesh, int type, double a, double b);

/**
 * Duplicates a mesh structure
 * 
 * @param[out] dst: destination mesh structure pointer
 * @param[in] src: source mesh structure pointer
 * @return error code
 * @note The src pointer has to be allocated first with the pspio_mesh_alloc 
 *       method.
 * @note The dst pointer might or might not be allocated. If it is not, then it
 *       is allocate here.
 */
int pspio_mesh_copy(pspio_mesh_t **dst, const pspio_mesh_t *src);

/**
 * Frees all memory associated with mesh structure
 * 
 * @param[in,out] mesh: mesh structure
 * @note This function can be safelly called even if some or all of the mesh 
 *       compoments have not been allocated.
 */
void pspio_mesh_free(pspio_mesh_t *mesh);


/**********************************************************************
 * Getters                                                            *
 **********************************************************************/

/**
 * Returns the number of points in the mesh
 * 
 * @param[in] mesh: mesh structure
 * @return np: the number of points
 */
int pspio_mesh_get_np(const pspio_mesh_t *mesh);

/**
 * Returns the a parameter of the mesh
 * 
 * @param[in] mesh: mesh structure
 * @return the a parameter (depends on mesh type)
 */
double pspio_mesh_get_a(const pspio_mesh_t *mesh);

/**
 * Returns the b parameter of the mesh
 * 
 * @param[in] mesh: mesh structure
 * @param[out] a: the a parameter (depends on mesh type)
 * @return the b parameter (depends on mesh type)
 */
double pspio_mesh_get_b(const pspio_mesh_t *mesh);

/**
 * Returns a pointer to the points of the mesh
 * 
 * @param[in] mesh: mesh structure
 * @return pointer to array of points
 */
const double *pspio_mesh_get_r(const pspio_mesh_t *mesh);

/**
 * Returns a pointer to the di/dr of the mesh
 * 
 * @param[in] mesh: mesh structure
 * @return pointer to array of di/dr points
 */
const double *pspio_mesh_get_rab(const pspio_mesh_t *mesh);


/**********************************************************************
 * Utility routines                                                   *
 **********************************************************************/

/**
 * Compares two meshes.
 * @param[in] mesh1: first mesh to compare
 * @param[in] mesh2: second mesh to compare
 * @return PSPIO_EQUAL for strict equality, PSPIO_MTEQUAL
 *         when the mesh are of the same type and have the same
 *         parameters, but have a different number of points,
 *         PSPIO_DIFF when different, PSPIO_ERROR if a problem
 *         occured.
 */
int pspio_mesh_cmp(const pspio_mesh_t *mesh1, const pspio_mesh_t *mesh2);

#endif
