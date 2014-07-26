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

program test_fortran

  use pspio_f90_types_m
  use pspio_f90_lib_m

  implicit none

  character(len=*), parameter :: file_inp = "test_fortran_input.tmp"
  character(len=*), parameter :: file_out = "test_fortran_output.tmp"

  integer :: pio_fmt, ierr, np
  type(pspio_f90_mesh_t) :: pspmesh
  type(pspio_f90_pspdata_t) :: pspdata
  
  ierr = pspio_f90_pspdata_init(pspdata)
  if (ierr /= 0) ierr = pspio_f90_error_flush()

  pio_fmt = PSPIO_FMT_UPF
  ierr = pspio_f90_pspdata_read(pspdata, pio_fmt, file_inp)
  if (ierr /= 0) then
    ierr = pspio_f90_error_flush()
    stop 1
  end if

  call pspio_f90_pspdata_get_mesh(pspdata, pspmesh)
  ierr = pspio_f90_error_flush()
  call pspio_f90_mesh_get_np(pspmesh, np)
  write(*,'("Mesh number of points: ", I6)') np

  ierr = pspio_f90_pspdata_write(pspdata, pio_fmt, file_out)
  if (ierr /= 0) then
    ierr = pspio_f90_error_flush()
    stop 1
  end if

  call pspio_f90_pspdata_free(pspdata)

end program test_fortran
