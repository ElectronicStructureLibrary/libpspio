!! Copyright (C) 2016 Yann Pouillon <notifications@materialsevolution.es>
!!
!! This file is part of Libpspio.
!!
!! Libpspio is free software: you can redistribute it and/or modify it under
!! the terms of the GNU Lesser General Public License as published by the Free
!! Software Foundation, version 3 of the License, or (at your option) any later
!! version.
!!
!! Libpspio is distributed in the hope that it will be useful, but WITHOUT ANY
!! WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
!! FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
!! more details.
!!
!! You should have received a copy of the GNU Lesser General Public License
!! along with Libpspio.  If not, see <http://www.gnu.org/licenses/> or write to
!! the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
!! 02110-1301  USA.

#if defined HAVE_CONFIG_H
#include "config.h"
#endif

module m_error_test

  use fpspio_m
  use fruit

  implicit none

contains

  subroutine test_error_empty()

    implicit none

    character(len=1), parameter :: noroutine = " "

    call fpspio_error_free()

    call assert_equals(0, fpspio_error_len(), "Empty error chain length")
    call assert_equals(PSPIO_SUCCESS, fpspio_error_get_last(noroutine), &
&     "Empty error chain last code")

  end subroutine test_error_empty

  subroutine test_error_fetchall()

    implicit none

    character(len=*), parameter :: err_file = "test_1_1.F90"
    character(len=*), parameter :: err_func = "dummy1"
    character(len=1), parameter :: ch10 = achar(10)
    character(len=*), parameter :: err_refmsg = &
&     "libpspio: ERROR:" // ch10 // "  * in test_1_1.F90(dummy1):1234:" &
&     // ch10 // "      value error: bad value found (PSPIO_EVALUE)" // ch10
    integer, parameter :: err_refcode = PSPIO_EVALUE

    character(len=PSPIO_STRLEN_ERROR) :: err_msg
    integer :: err_code

    call fpspio_error_free()
    err_code = &
&     fpspio_error_add(err_refcode, err_file, 1234, err_func)
    err_msg = fpspio_error_fetchall()

    call assert_equals(err_refmsg, err_msg, "Retrieve EVALUE error message")

  end subroutine test_error_fetchall

  subroutine test_error_get_last()

    implicit none

    character(len=*), parameter :: err_file1 = "test_4_1.F90"
    character(len=*), parameter :: err_file2 = "test_4_2.F90"
    character(len=*), parameter :: err_file3 = "test_4_3.F90"
    character(len=*), parameter :: err_func1 = "dummy41"
    character(len=*), parameter :: err_func2 = "dummy42"
    character(len=*), parameter :: err_func3 = "dummy43"
    character(len=*), parameter :: err_func4 = "dummy44"
    integer, parameter :: err_line1 = 411
    integer, parameter :: err_line2 = 422
    integer, parameter :: err_line3 = 433
    integer, parameter :: err_refcode1 = PSPIO_EGSL
    integer, parameter :: err_refcode2 = PSPIO_ENOFILE
    integer, parameter :: err_refcode3 = PSPIO_ERROR

    integer :: err_code1, err_code2, err_code3

    call fpspio_error_free()
    err_code1 = &
&     fpspio_error_add(err_refcode1, err_file1, err_line1, err_func1)
    err_code2 = &
&     fpspio_error_add(err_refcode2, err_file2, err_line2, err_func2)
    err_code3 = &
&     fpspio_error_add(err_refcode3, err_file3, err_line3, err_func3)

    call assert_equals(3, fpspio_error_len(), &
&     "Last error - Chain length")
    call assert_equals(err_refcode1, err_code1, "Last error - Insertion 1")
    call assert_equals(err_refcode2, err_code2, "Last error - Insertion 2")
    call assert_equals(err_refcode3, err_code3, "Last error - Insertion 3")
    call assert_equals(err_refcode1, &
&     fpspio_error_get_last(err_func1), "Last error - Function 1")
    call assert_equals(err_refcode2, &
&     fpspio_error_get_last(err_func2), "Last error - Function 2")
    call assert_equals(err_refcode3, &
&     fpspio_error_get_last(err_func3), "Last error - Function 3")
    call assert_equals(PSPIO_SUCCESS, &
&     fpspio_error_get_last(err_func4), "Last error - Unknown function")

  end subroutine test_error_get_last

  subroutine test_error_single()

    implicit none

    character(len=*), parameter :: err_file1 = "test_1_1.F90"
    character(len=*), parameter :: err_func1 = "dummy1"
    integer, parameter :: err_line1 = 1234
    integer, parameter :: err_refcode1 = PSPIO_EVALUE

    integer :: err_code1

    call fpspio_error_free()
    err_code1 = &
&     fpspio_error_add(err_refcode1, err_file1, err_line1, err_func1)

    call assert_equals(1, fpspio_error_len(), &
&     "Single error - Chain length")
    call assert_equals(err_refcode1, err_code1, "Single error - Insertion")
    call assert_equals(err_refcode1, fpspio_error_get_last(err_func1), &
&     "Single error - Function last code")

  end subroutine test_error_single

  subroutine test_error_double()

    implicit none

    character(len=*), parameter :: err_file1 = "test_2_1.F90"
    character(len=*), parameter :: err_file2 = "test_2_2.F90"
    character(len=*), parameter :: err_func1 = "dummy21"
    character(len=*), parameter :: err_func2 = "dummy22"
    integer, parameter :: err_line1 = 1234
    integer, parameter :: err_line2 = 202
    integer, parameter :: err_refcode1 = PSPIO_EGSL
    integer, parameter :: err_refcode2 = PSPIO_ENOSUPPORT

    integer :: err_code1, err_code2

    call fpspio_error_free()
    err_code1 = &
&     fpspio_error_add(err_refcode1, err_file1, err_line1, err_func1)
    err_code2 = &
&     fpspio_error_add(err_refcode2, err_file2, err_line2, err_func2)

    call assert_equals(2, fpspio_error_len(), &
&     "Double error - Chain length")
    call assert_equals(err_refcode1, err_code1, "Double error - Insertion 1")
    call assert_equals(err_refcode1, &
&     fpspio_error_get_last(err_func1), "Double error - Function 1")
    call assert_equals(err_refcode2, err_code2, "Double error - Insertion 2")
    call assert_equals(err_refcode2, &
&     fpspio_error_get_last(err_func2), "Double error - Function 2")

  end subroutine test_error_double

  subroutine test_error_triple()

    implicit none

    character(len=*), parameter :: err_file1 = "test_3_1.F90"
    character(len=*), parameter :: err_file2 = "test_3_2.F90"
    character(len=*), parameter :: err_file3 = "test_3_3.F90"
    character(len=*), parameter :: err_func1 = "dummy31"
    character(len=*), parameter :: err_func2 = "dummy32"
    character(len=*), parameter :: err_func3 = "dummy33"
    integer, parameter :: err_line1 = 311
    integer, parameter :: err_line2 = 322
    integer, parameter :: err_line3 = 333
    integer, parameter :: err_refcode1 = PSPIO_EVALUE
    integer, parameter :: err_refcode2 = PSPIO_ENOFILE
    integer, parameter :: err_refcode3 = PSPIO_ERROR

    integer :: err_code1, err_code2, err_code3

    call fpspio_error_free()
    err_code1 = &
&     fpspio_error_add(err_refcode1, err_file1, err_line1, err_func1)
    err_code2 = &
&     fpspio_error_add(err_refcode2, err_file2, err_line2, err_func2)
    err_code3 = &
&     fpspio_error_add(err_refcode3, err_file3, err_line3, err_func3)

    call assert_equals(3, fpspio_error_len(), &
&     "Triple error - Chain length")
    call assert_equals(err_refcode1, err_code1, "Triple error - Insertion 1")
    call assert_equals(err_refcode1, &
&     fpspio_error_get_last(err_func1), "Triple error - Function 1")
    call assert_equals(err_refcode2, err_code2, "Triple error - Insertion 2")
    call assert_equals(err_refcode2, &
&     fpspio_error_get_last(err_func2), "Triple error - Function 2")
    call assert_equals(err_refcode3, err_code3, "Triple error - Insertion 3")
    call assert_equals(err_refcode3, &
&     fpspio_error_get_last(err_func3), "Triple error - Function 3")

  end subroutine test_error_triple

end module m_error_test
