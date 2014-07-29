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

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


program test_fortran

  use pspio_f90_types_m
  use pspio_f90_lib_m

  implicit none

  character(len=1024) :: pio_program, pio_file_inp, pio_file_out
  character(len=4) :: pio_fmt_str
  integer :: pio_fmt, ierr, np
  type(pspio_f90_mesh_t) :: pspmesh
  type(pspio_f90_pspdata_t) :: pspdata
 
  ! Get command-line arguments
  call get_command_argument(0, pio_program)
  if ( command_argument_count() < 2 ) then
    write(*,'(A,A,A,1X,A)') "Usage:", achar(10), trim(pio_program), &
&     "input_file output_file [format]"
    stop 1
  end if

  call get_command_argument(1, pio_file_inp)
  call get_command_argument(2, pio_file_out)
  if ( command_argument_count() < 3 ) then
    pio_fmt = PSPIO_FMT_UNKNOWN
  else
    call get_command_argument(3, pio_fmt_str)
    read(pio_fmt_str, '(I4)') pio_fmt
  end if

  ! Display basic information
  write(*,'(A,1X,A)') PACKAGE_STRING, "- test_fortran_io"
  write(*,'(A,1X,A,A)') "Report bugs to ", PACKAGE_BUGREPORT, achar(10)
  write(*,'(A,A)') "=== BEGIN test_fortran_io ===", achar(10)

  ! Init pspdata
  write(*,'(A)') "test_fortran_io: initializing pspdata"
  ierr = pspio_f90_pspdata_init(pspdata)
  if ( ierr /= 0 ) then
    call pspio_f90_error_flush()
    stop 1
  end if

  ! Check parsing of file
  write(*,'(A)') "test_fortran_io: parsing file"
  write(*,'("test_fortran_io: before pspio_f90_pspdata_read, status = ", &
&   I3, ", format = ", I3)') ierr, pio_fmt
  ierr = pspio_f90_pspdata_read(pspdata, pio_fmt, trim(pio_file_inp))
  if ( ierr == 0 ) then
    pio_fmt = pspio_f90_format_guessed(pspdata)
    write(*,'("test_fortran_io: after  pspio_f90_pspdata_read, status = ", &
&     I3, ", format = ", I3,A)') ierr, pio_fmt, achar(10)
  else
    call pspio_f90_error_flush()
    stop 1
  end if

  ! Extract some random information
  write(*,'(A)') "test_fortran_io: extracting some data"
  call pspio_f90_pspdata_get_mesh(pspdata, pspmesh)
  call pspio_f90_mesh_get_np(pspmesh, np)
  write(*,'("test_fortran_io: number of points in the mesh = ", I6,A)') np, &
&   achar(10)

  ! Check writing of file
  write(*,'(A)') "test_fortran_io: writing file"
  ierr = pspio_f90_pspdata_write(pspdata, pio_fmt, trim(pio_file_out))
  if ( ierr == 0 ) then
    write(*,'("test_fortran_io: after pspio_f90_pspdata_write, status = ", &
&     I3,",format = ",I3,A)') ierr, pio_fmt, achar(10)
  else
    call pspio_f90_error_flush()
    stop 1
  end if

  ! Destroy pspdata structures
  write(*,'(A,A)') "test_fortran_io: destroying pspdata structure", achar(10)
  call pspio_f90_pspdata_free(pspdata)

  write(*,'(A)') "=== END test_fortran_io ==="

end program test_fortran
