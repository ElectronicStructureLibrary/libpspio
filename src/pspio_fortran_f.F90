!! Copyright (C) 2012 M. Oliveira
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
!! $Id$


module pspio_f90_types_m
  implicit none

  integer, public, parameter :: pspio_f90_kind = selected_real_kind(14)

  type pspio_f90_pointer_t
    private
    integer, pointer :: buffer
  end type pspio_f90_pointer_t

end module pspio_f90_types_m


module pspio_f90_lib_m
  use pspio_f90_types_m
  implicit none

  integer, parameter ::     &
       PSPIO_UNKNOWN = -1,  &
       PSPIO_UPF     = 9

  integer, parameter ::     &
       PSPIO_SUCCESS = 0

  ! pspio_pspdata
  interface
    integer function pspio_f90_pspdata_init(pspdata)
      use pspio_f90_types_m
      type(pspio_f90_pointer_t), intent(inout) :: pspdata
    end function pspio_f90_pspdata_init

    integer function pspio_f90_pspdata_read(pspdata, format, filename)
      use pspio_f90_types_m
      type(pspio_f90_pointer_t), intent(inout) :: pspdata
      integer,                   intent(in)    :: format
      character(len=*),          intent(in)    :: filename
    end function pspio_f90_pspdata_read

    integer function pspio_f90_pspdata_write(pspdata, format, filename)
      use pspio_f90_types_m
      type(pspio_f90_pointer_t), intent(in) :: pspdata
      integer,                   intent(in) :: format
      character(len=*),          intent(in) :: filename
    end function pspio_f90_pspdata_write

    integer function pspio_f90_pspdata_free(pspdata)
      use pspio_f90_types_m
      type(pspio_f90_pointer_t), intent(inout) :: pspdata
    end function pspio_f90_pspdata_free
  end interface

  ! pspio_mesh
  interface
    integer function pspio_f90_mesh_get_np(pspdata, np)
      use pspio_f90_types_m
      type(pspio_f90_pointer_t), intent(in)  :: pspdata
      integer,                   intent(out) :: np
    end function pspio_f90_mesh_get_np
  end interface

  ! pspio_error
  interface
    integer function pspio_f90_error_flush()
    end function pspio_f90_error_flush
  end interface

end module pspio_f90_lib_m
