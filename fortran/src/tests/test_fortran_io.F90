!! Copyright (C) 2012-2017 Micael Oliveira <micael.oliveira@mpsd.mpg.de>
!!                         Yann Pouillon <devops@materialsevolution.es>
!!
!! This file is part of Libpspio.
!!
!! This Source Code Form is subject to the terms of the Mozilla Public License,
!! version 2.0. If a copy of the MPL was not distributed with this file, You
!! can obtain one at https://mozilla.org/MPL/2.0/.
!!
!! Libpspio is distributed in the hope that it will be useful, but WITHOUT ANY
!! WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
!! FOR A PARTICULAR PURPOSE. See the Mozilla Public License version 2.0 for
!! more details.

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


program test_fortran
  use pspiof_m
  implicit none

  character(len=1024) :: pio_program, pio_file_inp, pio_file_out
  character(len=4) :: pio_fmt_str
  integer :: pio_fmt, ierr, np
  type(pspiof_mesh_t) :: pspmesh
  type(pspiof_pspdata_t) :: pspdata
 
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
  ierr = pspiof_pspdata_alloc(pspdata)
  if ( ierr /= 0 ) then
    call pspiof_error_flush()
    stop 1
  end if

  ! Check parsing of file
  write(*,'(A)') "test_fortran_io: parsing file"
  write(*,'("test_fortran_io: before pspiof_pspdata_read, status = ", &
&   I3, ", format = ", I3)') ierr, pio_fmt
  ierr = pspiof_pspdata_read(pspdata, pio_fmt, trim(pio_file_inp))
  if ( ierr == 0 ) then
    pio_fmt = pspiof_pspdata_get_format_guessed(pspdata)
    write(*,'("test_fortran_io: after  pspiof_pspdata_read, status = ", &
&     I3, ", format = ", I3,A)') ierr, pio_fmt, achar(10)
  else
    call pspiof_error_flush()
    stop 1
  end if

  ! Extract some random information
  write(*,'(A)') "test_fortran_io: extracting some data"
  pspmesh = pspiof_pspdata_get_mesh(pspdata)
  np = pspiof_mesh_get_np(pspmesh)
  write(*,'("test_fortran_io: number of points in the mesh = ", I6,A)') np, &
       &   achar(10)

  ! Check writing of file
  write(*,'(A)') "test_fortran_io: writing file"
  ierr = pspiof_pspdata_write(pspdata, pio_fmt, trim(pio_file_out))
  if ( ierr == 0 ) then
    write(*,'("test_fortran_io: after pspiof_pspdata_write, status = ", &
&     I3,",format = ",I3,A)') ierr, pio_fmt, achar(10)
  else
    call pspiof_error_flush()
    stop 1
  end if

  ! Destroy pspdata structures
  write(*,'(A,A)') "test_fortran_io: destroying pspdata structure", achar(10)
  call pspiof_pspdata_free(pspdata)

  write(*,'(A)') "=== END test_fortran_io ==="

end program test_fortran
