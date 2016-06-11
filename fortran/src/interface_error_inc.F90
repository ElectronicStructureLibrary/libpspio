!! Copyright (C) 2015-2016 Micael Oliveira <micael.oliveira@mpsd.mpg.de>
!!                         Yann Pouillon <notifications@materialsevolution.es>
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
