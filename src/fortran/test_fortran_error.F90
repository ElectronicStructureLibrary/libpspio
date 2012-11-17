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
! $Id: test_fortran_error.c 298 2012-11-09 08:40:03Z pouillon $

!! @file test_fortran_error.c
!! @brief checks pspio_f90_error.c and pspio_f90_error.h

program test_fortran_error

  use pspio_f90_lib_m

  implicit none

  character, parameter :: ch10 = char(10)

  character(len=:), pointer :: err_msg
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
  eid = pspio_f90_error_add("test_1_1.c", 1234)
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
  eid = pspio_f90_error_add("test_2_1.c", 201)
  call check_status("after pspio_f90_error_add", eid, PSPIO_SUCCESS, &
&    pspio_f90_error_len(), 1)
  call pspio_f90_error_set(PSPIO_ENOSUPPORT)
  eid = pspio_f90_error_add("test_2_2.c", 202)
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
  eid = pspio_f90_error_add("test_3_1.c", 311)
  call check_status("after pspio_f90_error_add", eid, PSPIO_SUCCESS, &
&    pspio_f90_error_len(), 1)
  call pspio_f90_error_set(PSPIO_ENOFILE)
  eid = pspio_f90_error_add("test_3_2.c", 322)
  call check_status("after pspio_f90_error_add", eid, PSPIO_SUCCESS, &
&    pspio_f90_error_len(), 2)
  call pspio_f90_error_set(PSPIO_ERROR)
  eid = pspio_f90_error_add("test_3_3.c", 333)
  call check_status("after pspio_f90_error_add", eid, PSPIO_SUCCESS, &
&    pspio_f90_error_len(), 3)
  write(*, '(a)') "test_fortran_error: BEGIN FLUSH"
  eid = pspio_f90_error_flush()
  write(*, '(a)') "test_fortran_error: END FLUSH"
  call check_status("after pspio_f90_error_flush", eid, PSPIO_SUCCESS, &
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
