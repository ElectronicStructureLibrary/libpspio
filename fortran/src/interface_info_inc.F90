!! Copyright (C) 2015-2016 Micael Oliveira <micaeljtoliveira@gmail.com>
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

  ! version
  subroutine fpspio_info_version(major, minor, micro) bind(c, name="pspio_info_version")
    import
    integer(c_int), intent(out) :: major, minor, micro
  end subroutine fpspio_info_version

  ! info
  subroutine pspio_info_string(info) bind(c)
    import
    character(kind=c_char) :: info(*)
  end subroutine pspio_info_string

end interface
