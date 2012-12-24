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

  ! All the following types are just pointers to the actual data structures
  type pspio_f90_pspdata_t
    private
    integer, pointer :: buffer
  end type pspio_f90_pspdata_t

  type pspio_f90_mesh_t
    private
    integer, pointer :: buffer
  end type pspio_f90_mesh_t

  type pspio_f90_state_t
    private
    integer, pointer :: buffer
  end type pspio_f90_state_t

  type pspio_f90_projector_t
    private
    integer, pointer :: buffer
  end type pspio_f90_projector_t

  type pspio_f90_potential_t
    private
    integer, pointer :: buffer
  end type pspio_f90_potential_t

  type pspio_f90_xc_t
    private
    integer, pointer :: buffer
  end type pspio_f90_xc_t

end module pspio_f90_types_m
