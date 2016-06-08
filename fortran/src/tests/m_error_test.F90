!! Copyright (C) 2016 Y. Pouillon
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

module m_error_test

  use fpspio_m
  use fruit

  implicit none

contains

  subroutine test_error_add()

    implicit none

    character(len=*), parameter :: err_file = "test_1_1.F90"
    character(len=*), parameter :: err_func = "dummy1"
    integer, parameter :: err_refcode = PSPIO_EVALUE

    integer :: err_code

    err_code = &
&     fpspio_error_add(err_refcode, err_file, 1234, err_func)

    call assert_equals(err_refcode, err_code, "Add EVALUE error")

  end subroutine test_error_add


  subroutine test_error_fetchall()

    implicit none

    character(len=1), parameter :: ch10 = achar(10)
    character(len=*), parameter :: err_refmsg = &
&     "libpspio: ERROR:" // ch10 // "  * in test_1_1.F90(dummy1):1234:" &
&     // ch10 // "      value error: bad value found (PSPIO_EVALUE)" // ch10
    character(len=PSPIO_STRLEN_ERROR) :: err_msg

    call fpspio_error_fetchall(err_msg)
    call assert_equals(err_refmsg, err_msg, "Retrieve EVALUE error message")

  end subroutine test_error_fetchall

end module m_error_test
