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


!*********************************************************************!
! Routines                                                            !
!*********************************************************************!

! info
subroutine fpspio_info_string(info)
  character(len=*), intent(out) :: info

  type(c_ptr) :: info_ptr

  call pspio_info_string(info_ptr)
  call c_to_f_string_ptr(info_ptr, info)

end subroutine fpspio_info_string
