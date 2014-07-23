! Copyright (C) 2011 J. Alberdi, M. Oliveira, Y. Pouillon, and M. Verstraete
!
! This program is free software; you can redistribute it and/or modify
! it under the terms of the GNU Lesser General Public License as published by
! the Free Software Foundation; either version 3 of the License, or
! (at your option) any later version.
!
! This program is distributed in the hope that it will be useful,
! but WITHOUT ANY WARRANTY; without even the implied warranty of
! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
! GNU Lesser General Public License for more details.
!
! You should have received a copy of the GNU Lesser General Public License
! along with this program; if not, write to the Free Software
! Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
!

!! @file test_fortran_error.F90
!! @brief checks pspio_f90_error.F90 and pspio_f90_error.h

program test_fortran_error

  use pspio_f90_lib_m

  implicit none

  character, parameter :: ch10 = char(10)

  character(len=PSPIO_STRLEN_ERROR) :: err_msg
  character(len=64) :: filename
  integer :: eid

  eid = pspio_f90_error_get()

  ! Display basic information
  write(*,'(a,a,a,a,a)') "PACKAGE_STRING - test_fortran_error", ch10, &
&   "Report bugs to PACKAGE_BUGREPORT", ch10, ch10
  write(*, '(a)') "=== BEGIN test_fortran_error ==="

  ! check empty error chain
  write(*, '(a)') "test_fortran_error: checking empty error chain"
  call check_status("at the beginning", eid, PSPIO_SUCCESS, &
&   pspio_f90_error_len(), 0)
  write(*, '(a)') "test_fortran_error: BEGIN FLUSH"
  eid = pspio_f90_error_flush()
  write(*, '(a)') "test_fortran_error: END FLUSH"
  call check_status("after pspio_f90_error_flush", eid, PSPIO_SUCCESS, &
&   pspio_f90_error_len(), 0)
  write(*, '(a)') ""

  ! Check single error
  write(*, '(a)') "test_fortran_error: checking single error (EVALUE)"
  call pspio_f90_error_set(PSPIO_EVALUE)
  write(filename, '(a)') "test_1_1.F90"
  call pspio_f90_error_add(filename, 1234)
  call check_status("after pspio_f90_error_add", eid, PSPIO_SUCCESS, &
&   pspio_f90_error_len(), 1)
  write(*, '(a)') "test_fortran_error: BEGIN FLUSH"
  eid = pspio_f90_error_flush()
  write(*, '(a)') "test_fortran_error: END FLUSH"
  call check_status("after pspio_f90_error_flush", eid, PSPIO_SUCCESS, &
&   pspio_f90_error_len(), 0)
  write(*, '(a)') ""

  ! Check double error
  write(*, '(a)') "test_fortran_error: checking double error (EGSL, ENOSUPPORT)"
  call pspio_f90_error_set(PSPIO_EGSL)
  write(filename, '(a)') "test_2_1.F90"
  call pspio_f90_error_add(filename, 201)
  call check_status("after pspio_f90_error_add", eid, PSPIO_SUCCESS, &
&    pspio_f90_error_len(), 1)
  call pspio_f90_error_set(PSPIO_ENOSUPPORT)
  write(filename, '(a)') "test_2_2.F90"
  call pspio_f90_error_add(filename, 202)
  call check_status("after pspio_f90_error_add", eid, PSPIO_SUCCESS, &
&    pspio_f90_error_len(), 2)
  write(*, '(a)') "test_fortran_error: BEGIN FLUSH"
  eid = pspio_f90_error_flush()
  write(*, '(a)') "test_fortran_error: END FLUSH"
  call check_status("after pspio_f90_error_flush", eid, PSPIO_SUCCESS, &
&    pspio_f90_error_len(), 0)
  write(*, '(a)') ""

  ! Check triple error
  write(*, '(a)') "test_fortran_error: checking triple error (EVALUE, ENOFILE, ERROR)"
  call pspio_f90_error_set(PSPIO_EVALUE)
  write(filename, '(a)') "test_3_1.F90"
  call pspio_f90_error_add(filename, 311)
  call check_status("after pspio_f90_error_add", eid, PSPIO_SUCCESS, &
&    pspio_f90_error_len(), 1)
  call pspio_f90_error_set(PSPIO_ENOFILE)
  write(filename, '(a)') "test_3_2.F90"
  call pspio_f90_error_add(filename, 322)
  call check_status("after pspio_f90_error_add", eid, PSPIO_SUCCESS, &
&    pspio_f90_error_len(), 2)
  call pspio_f90_error_set(PSPIO_ERROR)
  write(filename, '(a)') "test_3_3.F90"
  call pspio_f90_error_add(filename, 333)
  call check_status("after pspio_f90_error_add", eid, PSPIO_SUCCESS, &
&    pspio_f90_error_len(), 3)
  write(*, '(a)') "test_fortran_error: BEGIN FLUSH"
  eid = pspio_f90_error_flush()
  write(*, '(a)') "test_fortran_error: END FLUSH"
  call check_status("after pspio_f90_error_flush", eid, PSPIO_SUCCESS, &
&    pspio_f90_error_len(), 0)
  write(*, '(a)') ""

  ! Check fetchall on triple error
  write(*, '(a)') "test_fortran_error: checking pspio_f90_error_fetchall"
  call pspio_f90_error_set(PSPIO_EVALUE)
  write(filename, '(a)') "test_4_1.F90"
  call pspio_f90_error_add(filename, 411)
  call check_status("after pspio_f90_error_add", eid, PSPIO_SUCCESS, &
&    pspio_f90_error_len(), 1)
  call pspio_f90_error_set(PSPIO_ENOFILE)
  write(filename, '(a)') "test_4_2.F90"
  call pspio_f90_error_add(filename, 422)
  call check_status("after pspio_f90_error_add", eid, PSPIO_SUCCESS, &
&    pspio_f90_error_len(), 2)
  call pspio_f90_error_set(PSPIO_ERROR)
  write(filename, '(a)') "test_4_3.F90"
  call pspio_f90_error_add(filename, 433)
  call check_status("after pspio_f90_error_add", eid, PSPIO_SUCCESS, &
&    pspio_f90_error_len(), 3)
  eid = pspio_f90_error_fetchall(err_msg)
  write(*, '(a)') "test_fortran_error: === BEGIN ERROR STRING ==="
  write(*, '(a)') trim(err_msg)
  write(*, '(a)') "test_fortran_error: === END ERROR STRING ==="
  call check_status("after pspio_f90_error_fetchall", eid, PSPIO_SUCCESS, &
&    pspio_f90_error_len(), 0)
  write(*, '(a)') ""

  ! Destroy error-handling structures
  write(*, '(a)') "test_fortran_error: destroying error-handling structures"
  eid = pspio_f90_error_free()
  call check_status("after pspio_f90_error_free", eid, PSPIO_SUCCESS, &
&    pspio_f90_error_len(), 0)
  write(*, '(a)') ""

  write(*, '(a)') "=== END test_fortran_error ==="

contains

  subroutine check_status(title, stat_var, stat_val, len_var, len_val)

    implicit none

    character(len=*), intent(in) :: title
    integer, intent(in) :: stat_var, stat_val, len_var, len_val
    integer :: eid

    write(*, '(a,1x,a,a,i4.4,a,i4.4)') "test_fortran_error:", title, &
&     ", status = ", stat_var, ", length = ", len_var

    if ( (stat_var /= stat_val) .or. ( len_var /= len_val) ) then
      eid = pspio_f90_error_flush()
      write(*, '(a)') "libpspio_fortran: internal error: status mismatch"
      stop 1
    end if

  end subroutine check_status

end program test_fortran_error
