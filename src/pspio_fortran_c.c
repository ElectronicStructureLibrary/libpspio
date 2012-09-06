/*
 Copyright (C) 2012 M. Oliveira

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

/**
 * @file pspio_fortran_c.c
 * @brief Fortran wrappers
 */

#include "pspio_pspdata.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif

#include "string_f.h"

/**********************************************************************
 * pspio_pspdata                                                      *
 **********************************************************************/

CC_FORTRAN_INT FC_FUNC_(pspio_f90_pspdata_init, PSPIO_F90_PSPDATA_READ)
  (void **pspdata)
{
  pspio_pspdata_t *pspdata_p = NULL;
  int ierr;

  ierr = (CC_FORTRAN_INT) pspio_pspdata_init(&pspdata_p);
  *pspdata = (void *) pspdata_p;

  return ierr;
}

CC_FORTRAN_INT FC_FUNC_(pspio_f90_pspdata_read, PSPIO_F90_PSPDATA_READ)
  (void **pspdata, CC_FORTRAN_INT *format, STR_F_TYPE filename STR_ARG1)
{
  char *filename_c;
  int ierr;

  TO_C_STR1(filename, filename_c);
  ierr = (CC_FORTRAN_INT) pspio_pspdata_read( (pspio_pspdata_t **)(pspdata), filename_c, (int) (*format));
  free(filename_c);

  return ierr;
}

CC_FORTRAN_INT FC_FUNC_(pspio_f90_pspdata_write, PSPIO_F90_PSPDATA_WRITE)
  (void ** pspdata, CC_FORTRAN_INT *format, STR_F_TYPE filename STR_ARG1)
{
  char *filename_c;
  int ierr;

  TO_C_STR1(filename, filename_c);
  ierr = (CC_FORTRAN_INT) pspio_pspdata_write( (pspio_pspdata_t *)(*pspdata), filename_c, (int) (*format));
  free(filename_c);

  return ierr;
}

CC_FORTRAN_INT FC_FUNC_(pspio_f90_pspdata_free, PSPIO_F90_PSPDATA_FREE)
  (void ** pspdata)
{

 return (CC_FORTRAN_INT) pspio_pspdata_free( (pspio_pspdata_t **)(pspdata));
     
}

/**********************************************************************
 * pspio_mesh                                                         *
 **********************************************************************/

CC_FORTRAN_INT FC_FUNC_(pspio_f90_mesh_get_np, PSPIO_F90_MESH_GET_NP)
  (void ** pspdata, CC_FORTRAN_INT *np)
{

  return (CC_FORTRAN_INT) pspio_mesh_get_np( ((pspio_pspdata_t *)(*pspdata))->mesh, np);

}

/**********************************************************************
 * pspio_error                                                        *
 **********************************************************************/

CC_FORTRAN_INT FC_FUNC_(pspio_f90_error_flush, PSPIO_F90_ERROR_FLUSH)
  (void)
{
  return (CC_FORTRAN_INT) pspio_error_flush();
}
