!! Copyright (C) 2015-2017 Micael Oliveira <micael.oliveira@mpsd.mpg.de>
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

interface

  !*********************************************************************!
  ! Routines                                                            !
  !*********************************************************************!

  ! add (only used for tests)
  integer(c_int) function pspio_error_add(err_code, err_file, err_line, &
&   err_func) bind(c)
    import
    integer(kind=c_int), value :: err_code, err_line
    character(kind=c_char) :: err_file(*), err_func(*)
  end function pspio_error_add

  ! fetchall
  function pspio_error_fetchall() bind(c)
    import
    type(c_ptr) :: pspio_error_fetchall
  end function pspio_error_fetchall

  ! flush
  subroutine pspiof_error_flush() bind(c, name="pspio_error_flush")
  end subroutine pspiof_error_flush

  ! free
  subroutine pspiof_error_free() bind(c, name="pspio_error_free")
  end subroutine pspiof_error_free

  ! get_last
  integer(c_int) function pspio_error_get_last(routine) bind(c)
    import
    character(kind=c_char) :: routine(*)
  end function pspio_error_get_last

  ! len
  integer(c_int) function pspio_error_len() bind(c) result(len)
    import
  end function pspio_error_len

  ! string
  type(c_ptr) function pspio_error_string(error_id) bind(c) result(error_msg)
    import
    integer(c_int), value :: error_id
  end function pspio_error_string

end interface
