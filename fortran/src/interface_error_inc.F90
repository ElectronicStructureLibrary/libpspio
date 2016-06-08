!! Copyright (C) 2012-2015 M. Oliveira
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
  subroutine pspio_error_fetchall(error_msg)  bind(c)
    import
    character(kind=c_char) :: error_msg(PSPIO_STRLEN_ERROR)
  end subroutine pspio_error_fetchall

  ! flush
  subroutine fpspio_error_flush() bind(c, name="pspio_error_flush")
  end subroutine fpspio_error_flush

  ! free
  subroutine fpspio_error_free() bind(c, name="pspio_error_free")
  end subroutine fpspio_error_free

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
  character(kind=c_char) function pspio_error_string(error_id) bind(c) result(error_msg)
    import
    integer(c_int), value :: error_id
  end function pspio_error_string

end interface
