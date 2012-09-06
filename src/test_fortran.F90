!! Copyright (C) 2012 M. Oliveira
!!
!! This program is free software; you can redistribute it and/or modify
!! it under the terms of the GNU Lesser General Public License as published by
!! the Free Software Foundation; either version 3 of the License, or 
!! (at your option) any later version.
!!
!! This program is distributed in the hope that it will be useful,
!! but WITHOUT ANY WARRANTY; without even the implied warranty of
!! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
!! GNU Lesser General Public License for more details.
!!
!! You should have received a copy of the GNU Lesser General Public License
!! along with this program; if not, write to the Free Software
!! Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
!!
!! $Id$

program test_fortran
  use pspio_f90_types_m
  use pspio_f90_lib_m
  implicit none

  integer :: ierr, np
  character(len=100) :: file_in, file_out
  type(pspio_f90_pointer_t) :: pspdata
  
  ierr = pspio_f90_pspdata_init(pspdata)
  if (ierr /= 0) ierr = pspio_f90_error_flush()

  write(*,'("UPF filename to read:")')
  read(*,'(A)') file_in
  ierr = pspio_f90_pspdata_read(pspdata, PSPIO_UPF, file_in)
  if (ierr /= 0) ierr = pspio_f90_error_flush()

  ierr = pspio_f90_mesh_get_np(pspdata, np)
  if (ierr /= 0) ierr = pspio_f90_error_flush()
  write(*,'("Mesh number of points: ", I6)') np

  write(*,'("UPF filename to write:")')
  read(*,'(A)') file_out
  ierr = pspio_f90_pspdata_write(pspdata, PSPIO_UPF, file_out)
  if (ierr /= 0) ierr = pspio_f90_error_flush()

  ierr = pspio_f90_pspdata_free(pspdata)
  if (ierr /= 0) ierr = pspio_f90_error_flush()

end program test_fortran
