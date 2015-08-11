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

  ! version
  subroutine fpspio_info_version(major, minor, micro) bind(c, name="pspio_version")
    import
    integer(c_int), intent(out) :: major, minor, micro
  end subroutine fpspio_info_version

  ! info
  subroutine pspio_info_string(info) bind(c)
    import
    type(c_ptr) :: info
  end subroutine pspio_info_string

end interface
