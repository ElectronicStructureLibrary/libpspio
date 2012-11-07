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


module pspio_f90_lib_m
  use pspio_f90_types_m
  implicit none

  integer, parameter ::       &
       PSPIO_SCHRODINGER = 1, &
       PSPIO_SCALAR_REL  = 2, &
       PSPIO_DIRAC       = 3

  integer, parameter ::     &
       PSPIO_UNKNOWN = -1,  &
       PSPIO_UPF     = 9

  integer, parameter ::     &
       PSPIO_SUCCESS = 0

  ! pspio_pspdata
  interface
    integer function pspio_f90_pspdata_init(pspdata)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t), intent(inout) :: pspdata
    end function pspio_f90_pspdata_init

    integer function pspio_f90_pspdata_read(pspdata, format, filename)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t), intent(inout) :: pspdata
      integer,                   intent(in)    :: format
      character(len=*),          intent(in)    :: filename
    end function pspio_f90_pspdata_read

    integer function pspio_f90_pspdata_write(pspdata, format, filename)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t), intent(in) :: pspdata
      integer,                   intent(in) :: format
      character(len=*),          intent(in) :: filename
    end function pspio_f90_pspdata_write

    integer function pspio_f90_pspdata_free(pspdata)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t), intent(inout) :: pspdata
    end function pspio_f90_pspdata_free

    integer function pspio_f90_pspdata_get_symbol(pspdata, symbol)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      character(len=*),          intent(out) :: symbol
    end function pspio_f90_pspdata_get_symbol

    integer function pspio_f90_pspdata_get_z(pspdata, z)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      real(pspio_f90_kind),      intent(out) :: z
    end function pspio_f90_pspdata_get_z

    integer function pspio_f90_pspdata_get_zvalence(pspdata, zvalence)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      real(pspio_f90_kind),      intent(out) :: zvalence
    end function pspio_f90_pspdata_get_zvalence

    integer function pspio_f90_pspdata_get_l_max(pspdata, l_max)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      integer,                   intent(out) :: l_max
    end function pspio_f90_pspdata_get_l_max

    integer function pspio_f90_pspdata_get_wave_eq(pspdata, wave_eq)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      integer,                   intent(out) :: wave_eq
    end function pspio_f90_pspdata_get_wave_eq

    integer function pspio_f90_pspdata_get_mesh(pspdata, mesh)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      type(pspio_f90_mesh_t),    intent(out) :: mesh
    end function pspio_f90_pspdata_get_mesh

    integer function pspio_f90_pspdata_get_n_states(pspdata, n_states)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      integer,                   intent(out) :: n_states
    end function pspio_f90_pspdata_get_n_states

    integer function pspio_f90_pspdata_get_state(pspdata, i, state)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      integer,                   intent(in)  :: i
      type(pspio_f90_state_t),   intent(out) :: state
    end function pspio_f90_pspdata_get_state

    integer function pspio_f90_pspdata_get_n_kbproj(pspdata, n_kbproj)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      integer,                   intent(out) :: n_kbproj
    end function pspio_f90_pspdata_get_n_kbproj

    integer function pspio_f90_pspdata_get_kb_projector(pspdata, i, projector)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t),   intent(in)  :: pspdata
      integer,                     intent(in)  :: i
      type(pspio_f90_projector_t), intent(out) :: projector
    end function pspio_f90_pspdata_get_kb_projector

    integer function pspio_f90_pspdata_get_l_local(pspdata, l_local)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      integer,                   intent(out) :: l_local
    end function pspio_f90_pspdata_get_l_local

    integer function pspio_f90_pspdata_get_kb_l_max(pspdata, kb_l_max)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      integer,                   intent(out) :: kb_l_max
    end function pspio_f90_pspdata_get_kb_l_max

    integer function pspio_f90_pspdata_get_vlocal(pspdata, vlocal)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t),   intent(in)  :: pspdata
      type(pspio_f90_potential_t), intent(out) :: vlocal
    end function pspio_f90_pspdata_get_vlocal

    integer function pspio_f90_pspdata_get_xc(pspdata, xc)
      use pspio_f90_types_m
      type(pspio_f90_pspdata_t), intent(in)  :: pspdata
      type(pspio_f90_xc_t),      intent(out) :: xc
    end function pspio_f90_pspdata_get_xc
  end interface

  ! pspio_mesh
  interface
    integer function pspio_f90_mesh_get_np(mesh, np)
      use pspio_f90_types_m
      type(pspio_f90_mesh_t), intent(in)  :: mesh
      integer,                intent(out) :: np
    end function pspio_f90_mesh_get_np

    integer function pspio_f90_mesh_get_r(mesh, r)
      use pspio_f90_types_m
      type(pspio_f90_mesh_t), intent(in)    :: mesh
      real(pspio_f90_kind),   intent(inout) :: r
    end function pspio_f90_mesh_get_r
  end interface

  ! pspio_state
  interface
    integer function pspio_f90_state_wf_eval(state, r, wf)
      use pspio_f90_types_m
      type(pspio_f90_state_t), intent(in)  :: state
      real(pspio_f90_kind),    intent(in)  :: r
      real(pspio_f90_kind),    intent(out) :: wf
    end function pspio_f90_state_wf_eval

    integer function pspio_f90_state_get_qn(state, n, l, j)
      use pspio_f90_types_m
      type(pspio_f90_state_t), intent(in)  :: state
      integer,                 intent(out) :: n
      integer,                 intent(out) :: l
      real(pspio_f90_kind),    intent(out) :: j
    end function pspio_f90_state_get_qn

    integer function pspio_f90_state_get_occ(state, occ)
      use pspio_f90_types_m
      type(pspio_f90_state_t), intent(in)  :: state
      real(pspio_f90_kind),    intent(out) :: occ
    end function pspio_f90_state_get_occ
  end interface

  ! pspio_potential
  interface
    integer function pspio_f90_potential_eval(potential, r, v)
      use pspio_f90_types_m
      type(pspio_f90_potential_t), intent(in)  :: potential
      real(pspio_f90_kind),        intent(in)  :: r
      real(pspio_f90_kind),        intent(out) :: v
    end function pspio_f90_potential_eval
  end interface

  ! pspio_projector
  interface
    integer function pspio_f90_projector_eval(projector, r, p)
      use pspio_f90_types_m
      type(pspio_f90_projector_t), intent(in)  :: projector
      real(pspio_f90_kind),        intent(in)  :: r
      real(pspio_f90_kind),        intent(out) :: p
    end function pspio_f90_projector_eval

    integer function pspio_f90_projector_get_energy(projector, e)
      use pspio_f90_types_m
      type(pspio_f90_projector_t), intent(in)  :: projector
      real(pspio_f90_kind),        intent(out) :: e
    end function pspio_f90_projector_get_energy

    integer function pspio_f90_projector_get_l(projector, l)
      use pspio_f90_types_m
      type(pspio_f90_projector_t), intent(in)  :: projector
      integer,                     intent(out) :: l
    end function pspio_f90_projector_get_l

    integer function pspio_f90_projector_get_j(projector, j)
      use pspio_f90_types_m
      type(pspio_f90_projector_t), intent(in)  :: projector
      real(pspio_f90_kind),        intent(out) :: j
    end function pspio_f90_projector_get_j
  end interface

  ! pspio_xc
  interface
    integer function pspio_f90_xc_has_nlcc_int(xc, has_nlcc)
      use pspio_f90_types_m
      type(pspio_f90_xc_t), intent(in)  :: xc
      integer,              intent(out) :: has_nlcc
    end function pspio_f90_xc_has_nlcc_int

    integer function pspio_f90_xc_nlcc_eval(xc, r, core_dens)
      use pspio_f90_types_m
      type(pspio_f90_xc_t), intent(in)  :: xc
      real(pspio_f90_kind), intent(in)  :: r
      real(pspio_f90_kind), intent(out) :: core_dens
    end function pspio_f90_xc_nlcc_eval
  end interface

  ! pspio_error
  interface
    integer function pspio_f90_error_flush()
    end function pspio_f90_error_flush
  end interface

  ! pspio_info
  interface
    integer function pspio_f90_version(major, minor, micro)
      integer, intent(out) :: major, minor, micro
    end function pspio_f90_version
  end interface

contains

  integer function pspio_f90_xc_has_nlcc(xc, has_nlcc)
    use pspio_f90_types_m
    type(pspio_f90_xc_t), intent(in)  :: xc
    logical,              intent(out) :: has_nlcc
    
    integer :: i

    pspio_f90_xc_has_nlcc = pspio_f90_xc_has_nlcc_int(xc, i)
    has_nlcc = i /= 0

  end function pspio_f90_xc_has_nlcc


end module pspio_f90_lib_m
